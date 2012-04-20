#ifndef __TR_CEGUI_STATIC_IMAGE__
#define __TR_CEGUI_STATIC_IMAGE__

#include "../../../include/GuiStaticImage.h"
#include "CeguiManager.h"

namespace TR 
{
	namespace Gui
	{
		class CeguiStaticImage : public TR::Gui::StaticImage
		{
		public:
			CeguiStaticImage(CEGUI::Window *image) { mCeguiImage = image; }
			virtual ~CeguiStaticImage() {  }

			virtual void setVisible(bool visible) { mCeguiImage->setVisible(visible); }

		private:
			CEGUI::Window		*mCeguiImage;
		};
	}
}

#endif