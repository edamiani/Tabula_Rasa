#ifndef __TINYXML_MANAGER__
#define __TINYXML_MANAGER__

#include "..\..\..\include\XmlManager.h"
#include "..\..\..\include\Singleton.h"
#include "TinyXmlDocument.h"

namespace TR
{
	namespace Xml
	{
		class TinyXmlManager : public Manager
		{
		public:
											TinyXmlManager() {  }
											~TinyXmlManager();

			TR::Xml::Document* 	createDocument(std::string name, std::string fileName);
			void 							destroyDocument(std::string name);
			TR::Xml::Document* 	getDocument(std::string name);

		private:
			std::map<std::string, TR::Xml::TinyXmlDocument*> mDocuments;
			std::map<std::string, TR::Xml::TinyXmlDocument*>::iterator mDocumentsIterator;
		};
	}
}

#endif
