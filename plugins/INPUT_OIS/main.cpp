#include <windows.h>
#include "include\OISManager.h"
#include "..\..\include\Framework.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Input::Manager* loadPlugin()
{
	TR::Input::OISManager *manager = new TR::Input::OISManager();
	assert(manager != 0);
	return manager;
}
