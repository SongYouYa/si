#pragma once

#define PFLAG_MASK(x) (1 << (x))
enum RecordFlags
{
	RFlagNone = PFLAG_MASK(0),
	RFlagAppParsed = PFLAG_MASK(1), //Ӧ���ѷ�����
	RFlagNoReport = PFLAG_MASK(2),  //��Ҫ�����ڱ�����
	RFlagNoShow = PFLAG_MASK(3) //����ʾ���Ƿ�ʹ�ô˱�־���ɸ������Լ�������
};
