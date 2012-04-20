#ifndef __TR_TIMER__
#define __TR_TIMER__

#include "Dependencies.h"
#include "Defines.h"

//Bring in the specific platform's header file
#if TR_PLATFORM == TR_PLATFORM_WIN32
# include "WIN32/TimerWin32.h"
#elif TR_PLATFORM == TR_PLATFORM_LINUX
# include "GLX/TimerLinux.h"
#elif TR_PLATFORM == TR_PLATFORM_APPLE
# include "OSX/TimerApple.h"
#endif

#endif
