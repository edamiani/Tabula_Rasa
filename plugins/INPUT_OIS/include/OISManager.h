#ifndef __OIS_MANAGER__
#define __OIS_MANAGER__

#include "../../../include/InputManager.h"
#include "../../../include/Singleton.h"
#include "OIS.h"

class TR::Graphics::Manager;

namespace TR
{
	namespace Input
	{
		class OISManager : public TR::Input::Manager, public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
		{
		public:
			OISManager();
			~OISManager();

			TR::Input::Joystick*		createJoystick(bool buffered = false);
			TR::Input::Keyboard*		createKeyboard(bool buffered = false);
			TR::Input::Mouse*			createMouse(bool buffered = false);
			void							destroyJoystick();
			void							destroyKeyboard();
			void							destroyMouse();
			bool							initialize(TR::Framework *framework);

			// OIS events
			// Keyboard
			bool							keyPressed(const OIS::KeyEvent &arg);
			bool							keyReleased(const OIS::KeyEvent &arg);
			//Mouse
			bool							mouseMoved(const OIS::MouseEvent &arg);
			bool							mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
			bool							mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
			// Joystick
			bool							buttonPressed(const OIS::JoyStickEvent &arg, int button);
			bool							buttonReleased(const OIS::JoyStickEvent &arg, int button);
			bool							axisMoved(const OIS::JoyStickEvent &arg, int axis);

		private:
			TR::Framework				*mFramework;
			OIS::InputManager			*mOISInputManager;
			OIS::JoyStick				*mOISJoystick;
			OIS::Keyboard				*mOISKeyboard;
			OIS::Mouse					*mOISMouse;
		};
	}
}

#endif