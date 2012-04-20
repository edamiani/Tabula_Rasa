#ifndef __TR_CEGUI_EDIT_BOX__
#define __TR_CEGUI_EDIT_BOX__

#include "../../../include/GuiEditBox.h"
#include "CeguiManager.h"

namespace TR 
{
	namespace Gui
	{
		class CeguiEditBox : public TR::Gui::EditBox
		{
		public:
			CeguiEditBox(CEGUI::Editbox *editBox) { mCeguiEditBox = editBox; }
			~CeguiEditBox() { mCeguiEditBox = 0; }

			TR::uint32			getId() { return mCeguiEditBox->getID(); }
			std::string			getText() { return std::string(mCeguiEditBox->getText().c_str()); }
			void					setText(const std::string &text)  { mCeguiEditBox->setText(text); }
			void					setVisible(bool visible) { mCeguiEditBox->setVisible(visible); }

			CEGUI::Editbox*	_getCeguiEditBox() { return mCeguiEditBox; }

		private:
			CEGUI::Editbox		*mCeguiEditBox;
		};
	}
}

#endif