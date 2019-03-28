#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CPRecordHandler
	{
	public:
		static void removeLastSameChilren(CPRecord* parent,
			const std::vector<std::wstring>& list);

		//递归拷贝record的子节点，只拷贝目录节点
		//excFlags: 排除有指定标志的节点
		static void copyTree(CPRecord* srcRecord, CPRecord* dstRecord, int excFlags);
		//根据源节点的选中情况，递归设置目标节点的指定标志（会先清除目标的所有指定标志），只操作目录节点
		//add: 添加或删除
		static void setCheckedFlags(CPRecord* srcRecord, CPRecord* dstRecord, int flags, bool add);

		static void checkRecordByTypes(CPRecord* record, const std::set<std::wstring>& apps);
	};
}