#ifndef __INPUT_MOUSE__
#define __INPUT_MOUSE__

#include "Dependencies.h"
#include "InputDevice.h"

namespace TR
{
	namespace Input
	{
		enum MouseButton
		{
			MOUSE_LEFT = 0, MOUSE_RIGHT, MOUSE_MIDDLE,
			MOUSE_BUTTON3, MOUSE_BUTTON4,	MOUSE_BUTTON5, MOUSE_BUTTON6,	MOUSE_BUTTON7
		};

		class Mouse : public TR::Input::Device
		{
		public:
								Mouse() {  }
			virtual			~Mouse() {  }

			virtual int		getAbsoluteX() = 0;
			virtual int		getAbsoluteY() = 0;
			virtual int		getRelativeX() = 0;
			virtual int		getRelativeY() = 0;
			virtual bool	isButtonDown(TR::Input::MouseButton button) = 0;
		};
	}
}

#endif