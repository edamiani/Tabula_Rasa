#ifndef __XML_MANAGER__
#define __XML_MANAGER__

#include "Dependencies.h"
#include "Singleton.h"

namespace TR
{
	namespace Xml
	{
		class Manager
		{
		public:
										Manager() {  }
			virtual						~Manager() {  }

			virtual TR::Xml::Document*	createDocument(std::string name, std::string fileName) = 0;
			virtual void 				destroyDocument(std::string name) = 0;
			virtual TR::Xml::Document*	getDocument(std::string name) = 0;
		};
	}
}

#endif
