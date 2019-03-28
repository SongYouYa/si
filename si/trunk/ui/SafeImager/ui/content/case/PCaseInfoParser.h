#pragma once
#define PCASE_NUMBER L"case_number"
#define PCASE_NAME L"case_name"
#define PCASE_TYPE L"case_type"
#define PCASE_INV L"investagator"
#define PCASE_VIT L"case_witness"
#define PCASE_RT L"real_time"
#define PCASE_ADD L"case_address"
#define PCASE_SUSPT L"case_suspect"
#define PCASE_DIR L"case_dir"
#define PCASE_REMARK L"remark"

class CPCaseInfoParser
{
public:
	CPCaseInfoParser();

	typedef std::map<std::wstring, std::wstring> PCaseInfo;
	static void getCaseInfo(PCaseInfo& caseInfo);
	static bool getCaseInfo(const std::wstring& caseFile, PCaseInfo& caseInfo);
	static bool saveCaseInfo(const PCaseInfo& caseInfo,
		const std::wstring& caseFile, bool updateCaseNum);

	static CPRecordType* getCaseType();
	static void caseInfo2Record(PCaseInfo& caseInfo, CPRecord* record);

private:
};