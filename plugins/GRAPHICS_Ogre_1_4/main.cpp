#include <windows.h>
#include "include\OgreManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Graphics::Manager* loadPlugin()
{
	TR::Graphics::Manager *manager = new TR::Graphics::OgreManager();
	assert(manager != 0);
	return manager;
}
