#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CPRecordHandler
	{
	public:
		static void removeLastSameChilren(CPRecord* parent,
			const std::vector<std::wstring>& list);

		//�ݹ鿽��record���ӽڵ㣬ֻ����Ŀ¼�ڵ�
		//excFlags: �ų���ָ����־�Ľڵ�
		static void copyTree(CPRecord* srcRecord, CPRecord* dstRecord, int excFlags);
		//����Դ�ڵ��ѡ��������ݹ�����Ŀ��ڵ��ָ����־���������Ŀ�������ָ����־����ֻ����Ŀ¼�ڵ�
		//add: ��ӻ�ɾ��
		static void setCheckedFlags(CPRecord* srcRecord, CPRecord* dstRecord, int flags, bool add);

		static void checkRecordByTypes(CPRecord* record, const std::set<std::wstring>& apps);
	};
}