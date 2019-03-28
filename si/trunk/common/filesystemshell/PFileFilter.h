#pragma once
#include "base/PRecordFilter.h"
#include "base/PRecordType.h"


namespace filesystem
{
	class CPFileFilter
	{
	public:
		CPFileFilter();
		static bool isMatch(PSIHANDLE handle, CPRecordType* recordType, const RecordCondss& condss);

	private:
	};
}
