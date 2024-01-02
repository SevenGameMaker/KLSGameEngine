// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Logger.h"

namespace KLS
{
	class KLS_API KLS_AnimationEnd
	{
	public:
		virtual void onAnimationEnd() = 0;
		virtual void onAnimationChanged() = 0;
		virtual void onAnimationTag() = 0;
	};

} // end namespace