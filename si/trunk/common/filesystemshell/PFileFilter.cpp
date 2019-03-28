#include "stdafx.h"
#include "PFileFilter.h"
#include "filesystemshell/FileSystemShell.h"
#include "filesystemshell/PFileProperty.h"


const std::wstring F_SIZE = L"size";
const std::wstring F_CT = L"create_time";
const std::wstring F_MT = L"modify_time";
const std::wstring F_AT = L"access_time";
const std::wstring F_NAME = L"name";
const std::wstring F_EXT = L"ext";
const std::wstring F_DES = L"description";

const std::wstring F_DIR = L"dir";
const std::wstring F_APP = L"appfile";
const std::wstring F_ENC = L"encrypt";


namespace filesystem
{
	inline int64 getInt64Value(PSIHANDLE handle, const std::wstring& fieldName)
	{
		if (fieldName == F_SIZE) {
			return CFileSystem::GetItemSize(handle);
		}
		else if (fieldName == F_DIR) {
			bool ret = ITEM_TYPE_DIRECTORTY == CFileSystem::GetItemType(handle);
			return ret ? 1 : 0;
		}
		else if (fieldName == F_APP) {
			return CPFileProperty::get()->isAppFile(handle);
		}
		else if (fieldName == F_ENC) {
			return CPFileProperty::get()->isEncrypt(handle);
		}
		return -1;
	}

	inline PPTIME getTimeValue(PSIHANDLE handle, const std::wstring& fieldName)
	{
		if (fieldName == F_CT) {
			return CFileSystem::GetItemCreateTime(handle);
		}
		else if (fieldName == F_MT) {
			return CFileSystem::GetItemModfiyTime(handle);
		}
		else if (fieldName == F_AT) {
			return CFileSystem::GetItemLastAccessTime(handle);
		}
		return NULL;
	}

	inline std::wstring getStringValue(PSIHANDLE handle, const std::wstring& fieldName)
	{
		if (fieldName == F_NAME) {
			return CFileSystem::GetItemName(handle);
		}
		else if (fieldName == F_EXT) {
			return CFileSystem::GetFileExtName(handle);
		}
		else if (fieldName == F_DES) {
			tagDevice deviceInfo;
			CFileSystem::GetDeviceInfo(handle, &deviceInfo);
			return deviceInfo.strDeviceDesc;
		}
		return L"";
	}

	bool isMatchCond(PSIHANDLE handle, CPRecordType* recordType,
		const RecordCond& cond)
	{
		CPVariant::VarType varType = cond.field->type();
		if (varType == CPVariant::TInt64) {
			int64 value = getInt64Value(handle, cond.field->name());
			return CPRecordFilter::isMatchInt(value, cond);
		}
		else if (varType == CPVariant::TDateTime) {
			PPTIME value = getTimeValue(handle, cond.field->name());
			return CPRecordFilter::isMatchTime(value, cond);
		}
		else if (varType == CPVariant::TString) {
			std::wstring value = getStringValue(handle, cond.field->name());
			return CPRecordFilter::isMatchString(value, cond);
		}
		return false;
	}

	bool CPFileFilter::isMatch(PSIHANDLE handle, CPRecordType* recordType, const RecordCondss& condss)
	{
		if (condss.empty()) {
			return true;
		}

		for (auto& conds : condss) {
			bool isMatchConds = true;
			for (auto& cond : conds) {
				if (!isMatchCond(handle, recordType, cond)) {
					isMatchConds = false;
					break;
				}
			}
			if (isMatchConds) {
				return true;
			}
		}
		return false;
	}
}