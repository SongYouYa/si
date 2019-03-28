#include "stdafx.h"
#include "siutility/PFileSiUtils.h"

#include "siio/PNormalFile.h"
#include "siio/PHash.h"

#include "filesystemshell/FileSystemShell.h"
#include "task/PTaskStatusCtrl.h"
#include "task/PTaskProgress.h"

#include <mutex>

using namespace filesystem;
using namespace siio;

namespace siutil
{
    static std::mutex g_mutex;

    CPSiFileUtils::CPSiFileUtils()
    {
    }

    bool CPSiFileUtils::exportFile(const std::wstring& sorcePath, const std::wstring& targetPath)
    {
#ifdef OS_PUNIX
        return false;
#elif OS_PWINDOWS
        PSIHANDLE handle = CFileSystem::GetItemHandleByPath(sorcePath);
        return exportFile(handle, targetPath);
#endif
    }

    bool CPSiFileUtils::exportFile(PSIHANDLE handle, const std::wstring& otargetPath,
        std::string* md5Value, CPTaskStatusCtrl* statusCtrl,
        const uint64* pTotalSize, uint64* pCurSize)
    {
        ITEM_TYPE type = CFileSystem::GetItemType(handle);
        if (type != ITEM_TYPE_FILE) {
			try{
				//如果是目录，就创建文件夹
				boost::filesystem::create_directories(otargetPath);
				return true;
			}
			catch (...) {
				return false;
			}
        }

        std::wstring targetPath = otargetPath;
        boost::replace_all(targetPath, L"*", L"_");
        boost::replace_all(targetPath, L"?", L"_");
        boost::replace_all(targetPath, L"|", L"_");
        boost::replace_all(targetPath, L"\"", L"_");
        boost::replace_all(targetPath, L"<", L"_");
        boost::replace_all(targetPath, L">", L"_");
#ifdef OS_PWINDOWS
        size_t strsize = targetPath.size();
		//盘符（H:\）的大小为3
        if (strsize < 3) {
            return false;
        }
        std::wstring pathHeader = targetPath.substr(0, 3);//Windows在线时，如H:\test路径中的‘:’不替换
        std::wstring pathBody = targetPath.substr(3);
        boost::replace_all(pathBody, L":", L"");
        targetPath = pathHeader + pathBody;
#else
        boost::replace_all(targetPath, L":", L"_");
#endif

        try {
            boost::filesystem::path destination_path(targetPath);
            if (!boost::filesystem::exists(destination_path.parent_path())) {
                boost::filesystem::create_directories(destination_path.parent_path());
            }
         //fat32或ntfs文件系统下，文件名不能是单个“.”
            if (destination_path.filename().wstring() == L".") {
                return false;
            }
        }
        catch (...) {
            return false;
        }

        CPNormalFile out;
        if (!out.open(targetPath)) {
            return false;
        }

        CPHashPtr hasher;
        if (md5Value) {
            hasher = CPHash::createHash(HASH_MD5);
        }

         //加锁是为了防止多线程同时导出文件，造成buf冲突
        std::lock_guard<std::mutex> lock(g_mutex);
        const uint BUF_LENGTH = 1 * 1024 * 1024;
        //申请静态句柄变量，可以在导出多个文件时，只申请一次内存，加快速度
        static std::vector<uchar> vbuf(BUF_LENGTH, 0);
        uchar* buf = vbuf.data();

        uint64 fileSize = CFileSystem::GetItemSize(handle);

        uint readed = 0;
        uint64 offset = 0;
        bool finished = false;
        while (!finished) {
            if (statusCtrl) {
                statusCtrl->doPause();
            }
            if (statusCtrl && statusCtrl->isCanceled()) {
                return false;
            }
            readed = (uint)CFileSystem::ReadFile(handle, offset, BUF_LENGTH, buf);
            if (readed == 0) {
                finished = true;
            }
            else if (readed > BUF_LENGTH) { //异常
                break;
            }
            else if (readed < BUF_LENGTH) {
                finished = true;
            }
            //目前readsize可能不对,超出文件大小，这里处理一下
            //有时返回的是文件的占用空间，而不是文件大小，所以这里处理一下
            if (offset + readed > fileSize) {
                readed = (uint)(fileSize - offset);
                finished = true;
            }

            offset += readed;
            int ret = out.write(buf, readed);
            if (ret != CPFile::PERROR_SUCCESS) {
                return false;
            }
            if (hasher) {
                hasher->update(buf, readed);
            }

            if (statusCtrl && pTotalSize && pCurSize) {
                *pCurSize += readed;
                int progress = (int)((*pCurSize) * 1000 / (*pTotalSize));
                statusCtrl->progress()->updateProgress(progress);
                statusCtrl->progress()->updateSize(*pCurSize);
            }
        }

        out.close();
        if (md5Value && hasher) {
            hasher->final();
            *md5Value = hasher->getValue();
        }
        return true;
    }
    std::wstring CPSiFileUtils::calcHashValue( PSIHANDLE fileHandle)
    {
        ITEM_TYPE type = CFileSystem::GetItemType(fileHandle);
        if (type != ITEM_TYPE_FILE)
        {
            return L"";
        }

        CPHashPtr hasher;
        hasher = CPHash::createHash(HASH_MD5);   //create a hash object
        const uint32 length = 1024 * 1024;
        unsigned char* buf = new unsigned char[length];
        uint64 fileSize = CFileSystem::GetItemSize(fileHandle);

        bool finished = false;
        uint64 offset = 0;
        uint64 readedSize = 0;
        while(!finished)
        {
            readedSize = CFileSystem::ReadFile(fileHandle, offset, length, buf);
            if (readedSize == 0) //已读完
            {
                finished = true;
            }
            else if(readedSize > length) //异常
            {
                break;
            }
            else if (readedSize < length)
            {
                finished = true;
            }

            if (offset + readedSize > fileSize) //目前readsize可能不对,超出文件大小，这里处理一下
            {
                readedSize = fileSize- offset;
                finished = true;
            }

            offset += readedSize;
            hasher->update(buf, (uint)readedSize); //update the hash value according to readed  buf
        }
        hasher->final();
        delete [] buf;
        std::string hashv = hasher->getValue();
        return CPCodeChanger::UTF8ToUnicode(hashv.c_str());
    }
 }
