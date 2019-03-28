#pragma once

namespace filter
{
	enum E_RELATION_TYPE{
		E_EQUAL,
		E_NEQUAL,
		E_SMALLER,
		E_BIGGER,
		E_SAEQUAL,
		E_GAEQUAL,
		E_CONTAIN,
		E_EXCLUDE,
		E_IN,
		E_RELATION_INVALID
	};
	struct tagRelation{
		std::wstring chChs;
		std::wstring chEn;
		E_RELATION_TYPE type;
	};
}