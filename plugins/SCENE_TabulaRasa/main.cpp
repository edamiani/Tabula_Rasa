#include <windows.h>
#include "include\TRSceneManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Scene::Manager* loadPlugin()
{
	return new TR::Scene::TRManager();
}
