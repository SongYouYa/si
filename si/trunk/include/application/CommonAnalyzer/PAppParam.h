#pragma once


namespace app
{
	struct AppParam
	{
		CPRecord* rawRootRecord; //δ�����ļ�ϵͳǰ����õ�record���ڵ�
		CPRecord* rootRecord;
		bool appInited; //�Ѿ������ļ�ϵͳ����ʼ��
		AppParam()
			: rawRootRecord(nullptr), rootRecord(nullptr), appInited(false)
		{}
	};
}