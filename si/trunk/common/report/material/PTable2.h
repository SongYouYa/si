#pragma once
#include "./PTableBase.h"


namespace report
{
	class CPTable2 : public CPTableBase
	{
		friend class CPContent2;
	protected:
		CPTable2(TemplateDictionary* pTableDict, uint columnCount, bool bNumCol = true);
	public:
		void addProperty(const std::string& text);
		//ÔÚSetHeadÖ®ºó
		void addHeadRowProperty(const std::string& text);
		void addHeadNumCellProperty(const std::string& text);
	};
}
