#ifndef __INPUT_EVENTS__
#define __INPUT_EVENTS__

#include "Dependencies.h"
#include "InputJoystick.h"
#include "InputKeyboard.h"
#include "InputMouse.h"

namespace TR
{
	namespace Input
	{
		struct JoystickEvent
		{
			int axisPosition;
			int button;
		};

		struct KeyEvent
		{
			//! KeyCode of event
			TR::Input::KeyCode key;
			//! Text character, depends on current TextTranslationMode
			unsigned int text;
		};

		struct MouseEvent
		{
			MouseEvent() : absX(0), absY(0), buttons(0), relX(0), relY(0) {}
			
			int absX, absY;
			TR::Input::MouseButton buttonId;
			int buttons;
			int relX, relY;

			//! Button down test
			inline bool isButtonDown(MouseButton button) const
			{
				return ((buttons & (1L << button)) == 0) ? false : true;
			}

			//! Clear all the values
			void clear()
			{
				absX = absY = relX = relY = buttons = 0;
			}
		};
	}
}

#endif