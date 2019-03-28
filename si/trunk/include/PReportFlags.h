#pragma once

#define PFLAG_MASK(x) (1 << (x))
enum RecordFlags
{
	RFlagNone = PFLAG_MASK(0),
	RFlagAppParsed = PFLAG_MASK(1), //应用已分析过
	RFlagNoReport = PFLAG_MASK(2),  //不要生成在报告里
	RFlagNoShow = PFLAG_MASK(3) //不显示（是否使用此标志，由各界面自己决定）
};
