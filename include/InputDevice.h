#ifndef __INPUT_DEVICE__
#define __INPUT_DEVICE__

#include "Dependencies.h"

namespace TR
{
	namespace Input
	{
		class Device
		{
		public:
								Device() { mBuffered = false; }
			virtual			~Device() {  }

			virtual void	capture() = 0;
			bool				isBuffered() { return mBuffered; }
			void				setBuffered(bool buffered) { mBuffered = buffered; }

		protected:
			bool				mBuffered;
		};
	}
}

#endif