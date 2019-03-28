#include "stdafx.h"
#include "PFilePackageTask.h"

#include "libarchive/archive.h"
#include "libarchive/archive_entry.h"

#include "sidevice/PDeviceManager.h"
#include "sicommon/PPath.h"
#include "siio/PNormalFile.h"
#include "libdevtask/PDeviceHasher.h"

using namespace libdt;

namespace devtask {

const size_t BUF_LEN = 1024 * 512;

//const float CopyFileRadio = 0.9;

bool copyFileData( struct archive *a, struct archive_entry *entry, char* buff,
                  const uint64& totalSize, uint64& totalWrited,
                  CPTaskStatusCtrl* statusCtrl)
{
    CPTaskProgress* progress = statusCtrl->progress();
    size_t len;
    /* For now, we use a simpler loop to copy data
    * into the target archive. */
    const char* fpath = archive_entry_sourcepath(entry);
    FILE* f = ::fopen(fpath, "rb");
    if (f == NULL) {
        return false;
    }
    bool bret = true;
    while(true) {
        len = ::fread(buff, 1, BUF_LEN, f);
        if (len <= 0) {
            break;
        }
        archive_write_data(a, buff, len);
        totalWrited += len;
        progress->updateSize(totalWrited);
        progress->updateProgress(totalWrited * 1000 / totalSize);
        statusCtrl->doPause();
        if (statusCtrl->isCanceled()) {
            bret = false;
            break;
        }
    }
    ::fclose(f);
    return bret;
}

//打包/系统根目录时，把其他卷的挂载目录过滤掉
void setDiskReadMatching(struct archive *a, const std::string& srcDir)
{
    if (srcDir != "/") {
        return;
    }

    using namespace pdevice;
    std::vector<std::wstring> mountDirs;
    DeviceDisks disks;
    CPDeviceManager::get()->getAllDisks(disks);
    for(auto& disk : disks) {
        mountDirs.insert(mountDirs.end(), disk.mountPoints.begin(), disk.mountPoints.end());
        for(auto& part : disk.partitions) {
            mountDirs.insert(mountDirs.end(), part.mountPoints.begin(), part.mountPoints.end());
#ifdef OS_PAPPLE
            for(auto& vol : part.volumes) {
                mountDirs.insert(mountDirs.end(), vol.mountPoints.begin(), vol.mountPoints.end());
            }
#endif
        }
    }

    struct archive* m = archive_match_new();
    for(auto& dir : mountDirs) {
        if (dir == L"/") {
            continue;
        }
        std::string strDir = W2LS(dir);
        archive_match_exclude_pattern(m, (strDir + "/*").c_str());
    }
    archive_read_disk_set_matching(a, m, NULL, NULL);
}

void addErrorLog(const std::string& errorLog,const char* pathname)
{
    if (pathname) {
        std::ofstream out(errorLog, std::ios::app);
        out << std::string(pathname) << std::endl;
        out.close();
    }
}

bool createZip(const std::string& srcDir, const std::string& dstZip,
               const std::string& errorLog,
                const uint64& totalSize,
                CPTaskStatusCtrl* statusCtrl)
{
    struct archive *a;
    struct archive_entry *entry;

    //设置编码为本地环境编码,否则在处理中文路径时，会不正常
    char* locale = setlocale(LC_CTYPE, "");

    //TODO: 改为srcDir的父目录
    std::string baseDir = W2LS(CPPath::getDir(LS2W(srcDir)));

    a = archive_write_new();
    archive_write_set_format_zip(a);

    int ret = archive_write_open_filename(a, dstZip.c_str());

    struct archive *disk = archive_read_disk_new();
    archive_read_disk_set_standard_lookup(disk);

    ret = archive_read_disk_open(disk, srcDir.c_str());
    if (ret != ARCHIVE_OK) {
        return false;
    }
    setDiskReadMatching(disk, srcDir);

    bool bret = true;
    std::vector<char> vbuf(BUF_LEN);
    char* buff = vbuf.data();
    uint64 totalWrited = 0;
    while (true) {
        statusCtrl->doPause();
        if (statusCtrl->isCanceled()) {
            bret = false;
            break;
        }

        entry = archive_entry_new();
        ret = archive_read_next_header2(disk, entry);
        const char* pathname = archive_entry_pathname(entry);
        archive_read_disk_descend(disk);

        if (ret == ARCHIVE_EOF) {
            break;
        }
        else if (ret > ARCHIVE_FAILED) {
            const char* relative_path = strstr(pathname, baseDir.c_str()) + baseDir.size();
            if (relative_path != NULL) {
                archive_entry_set_pathname(entry, relative_path);
            }
            ret = archive_write_header(a, entry);
            if (ret > ARCHIVE_FAILED) {
                __LA_MODE_T type = archive_entry_filetype(entry);
                if (AE_IFREG == type) {
                    if (!copyFileData(a, entry, buff, totalSize, totalWrited, statusCtrl)) {
                        addErrorLog(errorLog, pathname);
                    }
                }
            }
            else {
                addErrorLog(errorLog, pathname);
            }
        }
        else if (ret == ARCHIVE_FAILED) {
            addErrorLog(errorLog, pathname);
        }
        else {
            statusCtrl->fail();
            bret = false;
            break;
        }

        archive_entry_free(entry);
    }
    archive_read_close(disk);
    archive_read_free(disk);

    archive_write_close(a);
    archive_write_free(a);
    return bret;
}

bool calcZipHash(const std::wstring& zipPath, const std::vector<siio::HashType>& hashTypes,
                 CPTaskStatusCtrl* statusCtrl, CPHashComputerPtr& hasher)
{
    CPTaskProgress* progress = statusCtrl->progress();
    int packProgress = progress->progress();
    float hashRadio = (1000 - packProgress) / 1000;

    using namespace siio;
    siio::CPNormalFile in;
    if (!in.open(zipPath, CPFile::MODE_READ)) {
        statusCtrl->fail();
        return false;
    }
    uint64 fileSize = 0;
    in.size(fileSize);
    if (fileSize == 0) {
        statusCtrl->fail();
        return false;
    }

    //这里创建的内存池智能指针，会在最后一个BlockDataPtr释放后释放
    CPMemPoolPtr memPool = std::make_shared<CPMemPool>(COMMON_BUF_LEN,
        2, QUEUE_MAX_SIZE);

    hasher = std::make_shared<CPHashComputer>(hashTypes);
    hasher->start();

    size_t blockNum = 0;
    bool bret = true;
    uint64 totalReaded = 0;
    uint readed = 0;
    while(totalReaded < fileSize) {
        statusCtrl->doPause();
        if (statusCtrl->isCanceled()) {
            bret = false;
            break;
        }
        char* buf = (char*)memPool->alloc();
        if (!buf) {
            break;
        }
        uint toRead = (uint)pmin(fileSize - totalReaded, COMMON_BUF_LEN);
        int ret = in.read(buf, toRead, &readed);
        if (ret != CPFile::PERROR_SUCCESS) {
            statusCtrl->fail();
            bret = false;
            break;
        }
        //这里创建的读数据块智能指针,会在写磁盘和计算完hash后释放
        BlockDataPtr blockData = std::make_shared<BlockData>(memPool);
        blockData->block_num = blockNum;
        blockData->data = buf;
        blockData->size = readed;
        blockData->src_dev_num = 1;
        hasher->update(blockData);

        totalReaded += readed;
        blockNum += 1;
        int progressValue = packProgress + (1000 * totalReaded / fileSize) * hashRadio;
        progress->updateProgress(progressValue);
    }
    in.close();
    hasher->setEnd();
    hasher->joinAll();
    hasher->final();
    return bret;
}

bool CPFilePackageTask::doPackage(const devtask::DeviceImageParam& param,
                                  CPTask *task)
{
    CPTaskStatusCtrl* statusCtrl = task->statusCtrl();

    using namespace pdevice;
    CPDeviceManager::get()->update();
    DevicePartition srcPart;
    bool ret = CPDeviceManager::get()->getPartitionByName(
                param.sourceDevice.deviceName, srcPart);
    assert(ret);
    if (srcPart.mountPoints.empty()) {
        statusCtrl->fail();
        return false;
    }
    std::wstring srcDir = srcPart.mountPoints.at(0);
    //已用空间作为总大小
    uint64 totalSize = srcPart.size;
    if (srcPart.capasity > 0 && srcPart.available > 0) {
        totalSize = srcPart.capasity - srcPart.available;
    }
    statusCtrl->progress()->setTotalSize(totalSize);

    CPRecord* record = task->rootRecord();
    record->at(L"source_size") = totalSize;

    //error log
    std::wstring errorLog = param.imagePath + L".error.txt";
    ::remove(W2LS(errorLog).c_str());

    ret = createZip(W2LS(srcDir), W2LS(param.imagePath), W2LS(errorLog), totalSize, statusCtrl);
    if (!ret) {
        return ret;
    }

    CPTaskProgress* progress = statusCtrl->progress();
    record->at(L"current_size") = progress->currentSize();

    CPHashComputerPtr hasher;
    ret = calcZipHash(param.imagePath, param.hashTypes, statusCtrl, hasher);

    if (hasher && !statusCtrl->isCanceled() && !statusCtrl->isFailed()) {
        std::wstring strHash;
        std::map<siio::HashType, std::string> hashValues;
        hasher->getValues(hashValues);
        CPDeviceLogUtils::getHashStr(hashValues, strHash);
        record->at(L"hash") = strHash;
        return true;
    }
    else {
        return false;
    }
}

}
