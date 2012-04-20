#include <windows.h>
#include "include\TinyXmlManager.h"

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

extern "C"
	__declspec(dllexport) TR::Xml::Manager* loadPlugin()
	{
		return new TR::Xml::TinyXmlManager();
	}
