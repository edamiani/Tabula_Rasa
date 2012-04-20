#include "../Include/CeguiManager.h"
#include "../Include/Cegui_EditBox.h"
#include "../Include/OgreManager.h"
#include <iostream>

namespace TR
{
	namespace Gui
	{
		CeguiManager::CeguiManager(TR::Framework *framework, SceneManager* scene, RenderWindow* window, const std::string &schemeFile, const std::string &layoutFile)
		{
			mFramework = framework;
			mSceneManager = scene;

			// setup CEGUI
			mGUIRenderer = new CEGUI::OgreCEGUIRenderer(window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, scene);
			mGUISystem = new CEGUI::System(mGUIRenderer);
			CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

			// load up CEGUI stuff...
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
			
			mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
			CEGUI::MouseCursor::getSingleton().show();
			mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

			mGUISystem->setGUISheet( CEGUI::WindowManager::getSingleton().loadWindowLayout(layoutFile) );
		}

		CeguiManager::~CeguiManager()
		{
			delete mGUISystem;

			if(mGUIRenderer)
				delete mGUIRenderer;

			mWinManager = 0;
			mRoot = 0;
		}

		TR::Gui::EditBox* CeguiManager::createEditBox(const std::string &name)
		{
			CEGUI::Editbox *ceguiEditBox = static_cast<CEGUI::Editbox *>(mGUISystem->getGUISheet()->getChild(name));
			TR::Gui::CeguiEditBox *editBox = new TR::Gui::CeguiEditBox(ceguiEditBox);
			editBox->_getCeguiEditBox()->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&TR::Gui::CeguiManager::_handleSubmit, this));
			return static_cast<TR::Gui::EditBox *>(editBox);
		}

		TR::Gui::StaticText* CeguiManager::createStaticText(const std::string &name)
		{
			CEGUI::Window *ceguiText = mGUISystem->getGUISheet()->getChild(name);
			TR::Gui::CeguiStaticText *text = new TR::Gui::CeguiStaticText(ceguiText);
			return static_cast<TR::Gui::StaticText *>(text);
		}

		TR::Gui::StaticImage* CeguiManager::createStaticImage(const std::string &name)
		{
			CEGUI::Window *ceguiImage = mGUISystem->getGUISheet()->getChild(name);
			TR::Gui::CeguiStaticImage *image = new TR::Gui::CeguiStaticImage(ceguiImage);
			return static_cast<TR::Gui::StaticImage *>(image);
		}

		void CeguiManager::injectKeyDown(const TR::Input::KeyEvent &e)
		{
			CEGUI::System::getSingleton().injectKeyDown(e.key);
			CEGUI::System::getSingleton().injectChar(e.text);
		}

		void CeguiManager::injectKeyUp(const TR::Input::KeyEvent &e)
		{
			CEGUI::System::getSingleton().injectKeyUp(e.key);
		}

		void CeguiManager::injectMouseButtonDown(const TR::Input::MouseEvent &e)
		{
			CEGUI::System::getSingleton().injectMouseButtonDown(_convertMouseButtonToCegui(e.buttonId));
		}

		void CeguiManager::injectMouseButtonUp(const TR::Input::MouseEvent &e)
		{
			CEGUI::System::getSingleton().injectMouseButtonUp(_convertMouseButtonToCegui(e.buttonId));
		}

		void CeguiManager::injectMouseMoved(const TR::Input::MouseEvent &e)
		{
			CEGUI::System::getSingleton().injectMouseMove(e.relX, e.relY);
		}

		CEGUI::MouseButton CeguiManager::_convertMouseButtonToCegui(int buttonID)
		{
			switch (buttonID)
			{
				case 0: return CEGUI::LeftButton;
				case 1: return CEGUI::RightButton;
				case 2: return CEGUI::MiddleButton;
				case 3: return CEGUI::X1Button;
				default: return CEGUI::LeftButton;
			}
		}
	}
}