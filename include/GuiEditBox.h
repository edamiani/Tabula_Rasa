#ifndef __TR_GUI_EDIT_BOX__
#define __TR_GUI_EDIT_BOX__

#include "Dependencies.h"

namespace TR 
{
	namespace Gui
	{
		class EditBox
		{
		public:
			EditBox() {  }
			virtual ~EditBox() {  }

			virtual TR::uint32		getId() = 0;
			virtual std::string		getText() = 0;
			virtual void				setText(const std::string &text) = 0;
			virtual void				setVisible(bool visible) = 0;
		};
	}
}

#endif