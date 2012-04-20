#ifndef __TR_CEGUI_MANAGER__
#define __TR_CEGUI_MANAGER__

#include <CEGUI.h>
#include <elements/CEGUIEditbox.h>

#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"

#include "../../../include/GuiManager.h"
#include "../../../include/Framework.h"
#include "Cegui_EditBox.h"
#include "Cegui_StaticImage.h"
#include "Cegui_StaticText.h"

using namespace Ogre;

namespace TR
{
	namespace Gui
	{
		class CeguiManager : public TR::Gui::Manager
		{
		public:
			CeguiManager(TR::Framework *framework, SceneManager* scene, RenderWindow* window, const std::string &schemeFile, const std::string &layoutFile);
			~CeguiManager();

			TR::Gui::EditBox*			createEditBox(const std::string &name);
			TR::Gui::StaticImage*	createStaticImage(const std::string &name);
			TR::Gui::StaticText*		createStaticText(const std::string &name);
			TR::Gui::EditBox*			getEditBox(const std::string &name) { return reinterpret_cast<TR::Gui::EditBox*>(new TR::Gui::CeguiEditBox(reinterpret_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingleton().getWindow(name)))); }
			TR::Gui::StaticImage*	getStaticImage(const std::string &name) { return reinterpret_cast<TR::Gui::StaticImage*>(new TR::Gui::CeguiStaticImage(reinterpret_cast<CEGUI::Window*>(CEGUI::WindowManager::getSingleton().getWindow(name)))); }
			TR::Gui::StaticText*		getStaticText(const std::string &name) { return reinterpret_cast<TR::Gui::StaticText*>(new TR::Gui::CeguiStaticText(reinterpret_cast<CEGUI::Window*>(CEGUI::WindowManager::getSingleton().getWindow(name)))); }
			void							hideMouse() { CEGUI::MouseCursor::getSingleton().hide(); }
			void							injectKeyDown(const TR::Input::KeyEvent &e);
			void							injectKeyUp(const TR::Input::KeyEvent &e);
			void							injectMouseButtonDown(const TR::Input::MouseEvent &e);
			void							injectMouseButtonUp(const TR::Input::MouseEvent &e);
			void							injectMouseMoved(const TR::Input::MouseEvent &e);
			void							setActiveCamera(const std::string &name) { mActiveCamera = name; }
			void							showMouse() { CEGUI::MouseCursor::getSingleton().show(); }

			CEGUI::MouseButton		_convertMouseButtonToCegui(int buttonID);
			bool							_handleSubmit(const CEGUI::EventArgs &args) { mFramework->onGuiSubmit(static_cast<const CEGUI::WindowEventArgs &>(args).window->getID()); return true; }

		protected:
			String							mActiveCamera;
			TR::Framework					*mFramework;
			CEGUI::OgreCEGUIRenderer	*mGUIRenderer;
			CEGUI::SchemeManager			*mScheme;
			CEGUI::System					*mGUISystem;
			CEGUI::Window					*mRoot;
			SceneManager					*mSceneManager;
			CEGUI::WindowManager			*mWinManager;
		};
	}
}

#endif