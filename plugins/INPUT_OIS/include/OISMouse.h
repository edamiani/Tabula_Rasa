#ifndef __TR_OIS_MOUSE__
#define __TR_OIS_MOUSE__

#include "OIS.h"
#include "..\..\..\include\InputMouse.h"

namespace TR
{
	namespace Input
	{
		class OISMouse : public TR::Input::Mouse
		{
		public:
							OISMouse(OIS::Mouse *mouse) { mOISMouse = mouse; }
							~OISMouse() { mOISMouse = 0; }
	
			void			capture() { mOISMouse->capture(); }
			int			getAbsoluteX() { return mOISMouse->getMouseState().X.abs; }
			int			getAbsoluteY() { return mOISMouse->getMouseState().Y.abs; }
			int			getRelativeX() { return mOISMouse->getMouseState().X.rel; }
			int			getRelativeY() { return mOISMouse->getMouseState().Y.rel; }
			bool			isButtonDown(TR::Input::MouseButton button) { return mOISMouse->getMouseState().buttonDown((OIS::MouseButtonID)(button)); }

		private:
			OIS::Mouse	*mOISMouse;
		};
	}
}

#endif