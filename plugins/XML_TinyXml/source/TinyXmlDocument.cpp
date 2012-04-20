#include "..\include\TinyXmlDocument.h"
#include "..\include\TinyXmlElement.h"

namespace TR
{
	namespace Xml
	{
		TinyXmlDocument::TinyXmlDocument()
		{
			mDocument = new TiXmlDocument();
			assert(mDocument != 0);
		}

		TinyXmlDocument::TinyXmlDocument(std::string filename)
		{
			mDocument = new TiXmlDocument();
			assert(mDocument != 0);

			if(mDocument->LoadFile(filename))				
				_parse();
		}

		TinyXmlDocument::~TinyXmlDocument()
		{
			if(mDocument)
			{
				delete mDocument;
				mDocument = 0;
			}

			if(mRootElement)
			{
				delete mRootElement;
				mRootElement = 0;
			}
		}

		void TinyXmlDocument::_parse()
		{
			mRootElement = new TR::Xml::TinyXmlElement(mDocument->RootElement());
			assert(mRootElement != 0);
			_parseElement(mRootElement);
		}

		void TinyXmlDocument::_parseElement(TR::Xml::TinyXmlElement *element)
		{
			TiXmlElement *child = 0;
			child = element->getTinyXmlElement()->FirstChildElement();

			TR::Xml::TinyXmlElement *currentElement = 0;
			TR::Xml::TinyXmlElement *previousElement = 0;

			while(child)
			{
				if(currentElement)
					previousElement = currentElement;

				currentElement = new TR::Xml::TinyXmlElement(child, element);

				if(previousElement)
					previousElement->setNextSibling((TR::Xml::Element*)currentElement);

				element->addChild(currentElement);
				if(child->FirstChildElement())
					_parseElement((TR::Xml::TinyXmlElement*)element->getLastChild());

				child = child->NextSiblingElement();
			}
		}
	}
}
