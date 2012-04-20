#ifndef __TR_CEGUI_STATIC_TEXT__
#define __TR_CEGUI_STATIC_TEXT__

#include "../../../include/GuiStaticText.h"
#include "CeguiManager.h"

namespace TR 
{
	namespace Gui
	{
		class CeguiStaticText : public TR::Gui::StaticText
		{
		public:
			CeguiStaticText(CEGUI::Window *text) { mCeguiText = text; }
			~CeguiStaticText() { mCeguiText = 0; }

			std::string		getText() { return std::string(mCeguiText->getText().c_str()); }
			void				setText(const std::string &text) { mCeguiText->setText(text); }
			void				setVisible(bool visible) { mCeguiText->setVisible(visible); }

		private:
			CEGUI::Window		*mCeguiText;
		};
	}
}

#endif