#ifndef __INPUT_AXIS__
#define __INPUT_AXIS__

#include "Dependencies.h"

namespace TR
{
	namespace Input
	{
		//! Axis component
		struct Axis
		{
			Axis() : abs(0), rel(0), absOnly(false) {};

			//! Absoulte and Relative value components
			int abs, rel;

			//! Indicates if this Axis only supports Absoulte (ie JoyStick)
			bool absOnly;

			//! Used internally by OIS
			void clear()
			{
				abs = rel = 0;
			}
		};
	}
}

#endif