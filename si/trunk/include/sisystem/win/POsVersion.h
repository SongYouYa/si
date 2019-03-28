#pragma once
#include"sisystem/globals.h"
namespace psystem
{
	class SYSTEM_API CPOsVersion
	{
	public:
		enum OSVersion
		{
			UNKNOWN = 0,
			WIN_2K = 500,
			WIN_XP = 510,
			WIN_2K3 = 520,
			WIN_2K3_R2 = 521,
			WIN_2K8 = 600,
			WIN_VISTA = 601,
			WIN_7 = 610,
			WIN_2K8_R2 = 611,
			WIN_2K12 = 620,
			WIN_8 = 621,
			WIN_2K12_R2 = 630,
			WIN_81 = 631,
			WIN_2K16 = 1000,
			WIN_10 = 1001,
		};
		static bool isVistaOrLater();
		static bool isBeforeVista();
		static bool isWin64();

	private:
		static OSVersion getOsVersion();
	};
}