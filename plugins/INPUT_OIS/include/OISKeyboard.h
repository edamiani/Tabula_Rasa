#ifndef __TR_OIS_KEYBOARD__
#define __TR_OIS_KEYBOARD__

#include "..\..\..\include\InputKeyboard.h"
#include "OIS.h"

namespace TR
{
	namespace Input
	{
		class OISKeyboard : public TR::Input::Keyboard
		{
		public:
								OISKeyboard(OIS::Keyboard *keyboard) { mOISKeyboard = keyboard; }
								~OISKeyboard() { mOISKeyboard = 0; }

			void				capture() { mOISKeyboard->capture(); }
			bool				isKeyDown(TR::Input::KeyCode code) { return mOISKeyboard->isKeyDown(static_cast<OIS::KeyCode>(code)); }

		private:
			OIS::Keyboard	*mOISKeyboard;
		};
	}
}

#endif