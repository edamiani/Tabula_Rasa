#include <windows.h>
#include "include\TREntityManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
__declspec(dllexport) TR::Entity::Manager* loadPlugin()
{
	return new TR::Entity::TRManager();
}
