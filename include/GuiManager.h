#ifndef __TR_GUI_MANAGER__
#define __TR_GUI_MANAGER__

#include "Dependencies.h"

namespace TR 
{
	namespace Gui
	{
		class Manager
		{
		public:
			Manager() {  }
			virtual ~Manager() {  }

			virtual TR::Gui::EditBox*			createEditBox(const std::string &name) = 0;
			virtual TR::Gui::StaticImage*		createStaticImage(const std::string &name) = 0;
			virtual TR::Gui::StaticText*		createStaticText(const std::string &name) = 0;
			virtual TR::Gui::EditBox*			getEditBox(const std::string &name) = 0;
			virtual TR::Gui::StaticImage*		getStaticImage(const std::string &name) = 0;
			virtual TR::Gui::StaticText*		getStaticText(const std::string &name) = 0;
			virtual void							hideMouse() = 0;
			virtual void							injectKeyDown(const TR::Input::KeyEvent &e) = 0;
			virtual void							injectKeyUp(const TR::Input::KeyEvent &e) = 0;
			virtual void							injectMouseButtonDown(const TR::Input::MouseEvent &e) = 0;
			virtual void							injectMouseButtonUp(const TR::Input::MouseEvent &e) = 0;
			virtual void							injectMouseMoved(const TR::Input::MouseEvent &e) = 0;
			virtual void							setActiveCamera(const std::string &name) = 0;
			virtual void							showMouse() = 0;
		};
	}
}

#endif