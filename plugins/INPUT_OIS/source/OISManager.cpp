#include "../include/OISManager.h"
#include "../include/OISJoystick.h"
#include "../include/OISKeyboard.h"
#include "../include/OISMouse.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsManager.h"
#include "../../../include/InputEvents.h"

namespace TR
{
	namespace Input
	{
		OISManager::OISManager()
		{
			mOISJoystick = 0;
			mOISKeyboard = 0;
			mOISMouse = 0;
			mOISInputManager = 0;
		}

		OISManager::~OISManager()
		{
			if(mJoystick) delete mJoystick;
			if(mKeyboard) delete mKeyboard;
			if(mMouse) delete mMouse;
			if(mOISJoystick) mOISInputManager->destroyInputObject(mOISJoystick);
			if(mOISKeyboard) mOISInputManager->destroyInputObject(mOISKeyboard);
			if(mOISMouse) mOISInputManager->destroyInputObject(mOISMouse);
			OIS::InputManager::destroyInputSystem(mOISInputManager);
			mOISInputManager = 0;
		}

		bool OISManager::axisMoved(const OIS::JoyStickEvent &arg, int axis) 
		{
			TR::Input::JoystickEvent e;
			e.axisPosition = arg.state.mAxes[axis].abs;
			mFramework->onJoyAxisMoved(e, axis);

			return true;
		}

		bool OISManager::buttonPressed(const OIS::JoyStickEvent &arg, int button) 
		{
			TR::Input::JoystickEvent e;
			e.button = button;
			mFramework->onJoyButtonDown(e);

			return true;
		}

		bool OISManager::buttonReleased(const OIS::JoyStickEvent &arg, int button) 
		{
			TR::Input::JoystickEvent e;
			e.button = button;
			mFramework->onJoyButtonUp(e);

			return true;
		}

		TR::Input::Joystick* OISManager::createJoystick(bool buffered)
		{
			try 
			{
				mOISJoystick = static_cast<OIS::JoyStick*>(mOISInputManager->createInputObject(OIS::OISJoyStick, buffered));
				mOISJoystick->setEventCallback(this);
				mJoystick = new TR::Input::OISJoystick(mOISJoystick);
			}
			catch(...) 
			{
				mJoystick = 0;
			}
			return mJoystick;
		}

		TR::Input::Keyboard* OISManager::createKeyboard(bool buffered)
		{
			mOISKeyboard = static_cast<OIS::Keyboard*>(mOISInputManager->createInputObject(OIS::OISKeyboard, buffered));
			mOISKeyboard->setEventCallback(this);
			mKeyboard = new TR::Input::OISKeyboard(mOISKeyboard);
			return mKeyboard;
		}

		TR::Input::Mouse* OISManager::createMouse(bool buffered)
		{
			mOISMouse = static_cast<OIS::Mouse*>(mOISInputManager->createInputObject( OIS::OISMouse, buffered ));
			mOISMouse->setEventCallback(this);
			mMouse = new TR::Input::OISMouse(mOISMouse);
			return mMouse;
		}

		void OISManager::destroyJoystick()
		{
			if(mOISJoystick) mOISInputManager->destroyInputObject( mOISJoystick );
			delete mJoystick;
			mOISJoystick = 0;
			mJoystick = 0;
		}

		void OISManager::destroyKeyboard()
		{
			if(mOISKeyboard) mOISInputManager->destroyInputObject(mOISKeyboard);
			delete mKeyboard;
			mOISKeyboard = 0;
			mKeyboard = 0;
		}

		void OISManager::destroyMouse()
		{
			if(mOISMouse) mOISInputManager->destroyInputObject(mOISMouse);
			delete mMouse;
			mOISMouse = 0;
			mMouse = 0;
		}

		bool OISManager::initialize(TR::Framework *framework)
		{
			mFramework = framework;

			OIS::ParamList paramList;
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;

			windowHnd = mFramework->getGraphicsManager()->getWindowHandle();
			assert(windowHnd != 0);
			
			windowHndStr << windowHnd;
			paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

			mOISInputManager = OIS::InputManager::createInputSystem(paramList);
			assert(mOISInputManager != 0);

			return true;
		}

		bool OISManager::keyPressed(const OIS::KeyEvent &arg) 
		{
			TR::Input::KeyEvent e;
			e.key = static_cast<TR::Input::KeyCode>(arg.key);
			e.text = static_cast<TR::Input::KeyCode>(arg.text);
			mFramework->onKeyDown(e);

			return true;
		}

		bool OISManager::keyReleased(const OIS::KeyEvent &arg) 
		{
			TR::Input::KeyEvent e;
			e.key = static_cast<TR::Input::KeyCode>(arg.key);
			e.text = static_cast<TR::Input::KeyCode>(arg.text);
			mFramework->onKeyUp(e); 

			return true;
		}

		bool OISManager::mouseMoved(const OIS::MouseEvent &arg) 
		{
			TR::Input::MouseEvent e;
			e.absX = arg.state.X.abs;
			e.absY = arg.state.Y.abs;
			e.relX = arg.state.X.rel;
			e.relY = arg.state.Y.rel;
			mFramework->onMouseMoved(e);

			return true;
		}

		bool OISManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
		{
			TR::Input::MouseEvent e;
			e.buttonId = static_cast<TR::Input::MouseButton>(id);
			e.buttons = arg.state.buttons;
			mFramework->onMouseButtonDown(e);

			return true;
		}

		bool OISManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
		{
			TR::Input::MouseEvent e;
			e.buttonId = static_cast<TR::Input::MouseButton>(id);
			e.buttons = arg.state.buttons;
			mFramework->onMouseButtonUp(e); 

			return true;
		}
	}
}