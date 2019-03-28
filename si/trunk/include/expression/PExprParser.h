#pragma once
#include "expression/node.h"

namespace expr
{
	class CPExprParser
	{
	public:
		static bool parseExpr(const std::wstring& text, Group** pgroup);
		static void freeNode(Node* node);
	};
}