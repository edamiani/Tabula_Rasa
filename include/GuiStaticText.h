#ifndef __TR_GUI_STATIC_TEXT__
#define __TR_GUI_STATIC_TEXT__

#include "Dependencies.h"

namespace TR 
{
	namespace Gui
	{
		class StaticText
		{
		public:
			StaticText() {  }
			virtual ~StaticText() {  }

			virtual std::string	getText() = 0;
			virtual void			setText(const std::string &text) = 0;
			virtual void			setVisible(bool visible) = 0;
		};
	}
}

#endif