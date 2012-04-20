#ifndef __TINYXML_DOCUMENT__
#define __TINYXML_DOCUMENT__

#include "..\..\..\include\XmlDocument.h"
#include "TinyXmlElement.h"
#include "tinyxml.h"

namespace TR
{
	namespace Xml
	{
		class TinyXmlDocument : public Document
		{
		public:
													TinyXmlDocument();
													TinyXmlDocument(std::string filename);
			virtual								~TinyXmlDocument();

			TR::Xml::Element*					getRootElement() { return mRootElement; }

		private:
			void 									_parse();
			void 									TinyXmlDocument::_parseElement(TR::Xml::TinyXmlElement *element);

			TiXmlDocument* 					mDocument;
			TR::Xml::TinyXmlElement*		mRootElement;
		};
	}
}

#endif
