#ifndef __TINYXML_ELEMENT__
#define __TINYXML_ELEMENT__

#include "..\..\..\include\XmlElement.h"
#include "tinyxml.h"

namespace TR
{
	namespace Xml
	{
		class TinyXmlElement : public Element
		{
		public:
													TinyXmlElement(TiXmlElement *element, TR::Xml::TinyXmlElement *parent = 0) : mElement(element), mParentElement(parent), mNextSibling(0) {  }
			virtual								~TinyXmlElement();

			void									addChild(TR::Xml::Element* child) { mChildren.push_back((TR::Xml::TinyXmlElement*)child); }
			std::string							getAttributeValue(std::string attribute);
			TR::Xml::Element*					getFirstChildByName(std::string name);
			TR::Xml::Element*					getFirstChild() { if(!mChildren.empty()) {mChildrenIterator = mChildren.begin(); return mChildren.front();} else return 0; }
			TR::Xml::Element*					getLastChild() { if(!mChildren.empty()) return mChildren.back(); else return 0; }
			std::string							getName() { return std::string(mElement->Value()); }
			TR::Xml::Element*					getNextChild() { if(++mChildrenIterator != mChildren.end()) return *mChildrenIterator; return 0; }
			TR::Xml::Element*					getNextSibling() { return mNextSibling; }
			TR::Xml::Element*					getParent() { return mParentElement; }
			TR::Xml::Element*					getNextSiblingByName(std::string name);
			void 									setAttributeValue(std::string attribute, std::string value) { mElement->SetAttribute(attribute, value); }
			void									setNextSibling(TR::Xml::Element* nextSibling) { mNextSibling = (TR::Xml::TinyXmlElement*)nextSibling; }

			TiXmlElement*						getTinyXmlElement() { return mElement; }
		private:
			TiXmlElement*				 		mElement;
			TR::Xml::TinyXmlElement*		mParentElement;
			TR::Xml::TinyXmlElement*		mNextSibling;
			std::list<TR::Xml::TinyXmlElement*> mChildren;
			std::list<TR::Xml::TinyXmlElement*>::iterator mChildrenIterator;
		};
	}
}

#endif
