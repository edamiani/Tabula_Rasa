#ifndef __XML_DOCUMENT__
#define __XML_DOCUMENT__

#include "Dependencies.h"

namespace TR
{
	namespace Xml
	{
		class Document
		{
		public:
													Document() {  }
													Document(std::string filename) {  }
			virtual 								~Document() {  }

			virtual TR::Xml::Element* 		getRootElement() = 0;
		};
	}
}

#endif
