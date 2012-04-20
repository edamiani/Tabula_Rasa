#ifndef __TR_GUI_STATIC_IMAGE__
#define __TR_GUI_STATIC_IMAGE__

#include "Dependencies.h"

namespace TR 
{
	namespace Gui
	{
		class StaticImage
		{
		public:
			StaticImage() {  }
			virtual ~StaticImage() {  }

			virtual void setVisible(bool visible) = 0;
		};
	}
}

#endif