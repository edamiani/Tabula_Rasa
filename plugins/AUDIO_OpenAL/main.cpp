#include <windows.h>
#include "include\TRAudioManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Audio::Manager* loadPlugin()
{
	TR::Audio::Manager* audioManager = new TR::Audio::OpenALManager();
	audioManager->init();
	return audioManager;
}
