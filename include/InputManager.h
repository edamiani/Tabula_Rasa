#ifndef __INPUT_MANAGER__
#define __INPUT_MANAGER__

#include "Dependencies.h"

namespace TR
{
	namespace Input
	{
		class Manager
		{
		public:
													Manager() { mJoystick = 0; mKeyboard = 0; mMouse = 0; }
			virtual								~Manager() { mJoystick = 0; mKeyboard = 0; mMouse = 0; }

			virtual TR::Input::Joystick*	createJoystick(bool buffered = false) = 0;
			virtual TR::Input::Keyboard*	createKeyboard(bool buffered = false) = 0;
			virtual TR::Input::Mouse*		createMouse(bool buffered = false) = 0;
			TR::Input::Joystick*				getJoystick() { return mJoystick; };
			TR::Input::Keyboard*				getKeyboard() { return mKeyboard; };
			TR::Input::Mouse*					getMouse() { return mMouse; };
			virtual bool						initialize(TR::Framework *framework) = 0;

		protected:
			TR::Input::Joystick				*mJoystick;
			TR::Input::Keyboard				*mKeyboard;
			TR::Input::Mouse					*mMouse;
		};
	}
}

#endif