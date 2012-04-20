#include <windows.h>
#include "include\NovodexManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Physics::Manager* loadPlugin()
{
	TR::Physics::Manager *manager = new TR::Physics::NovodexManager();
	assert(manager != 0);
	return manager;
}
