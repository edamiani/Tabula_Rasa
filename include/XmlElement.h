#ifndef __XML_ELEMENT__
#define __XML_ELEMENT__

#include "Dependencies.h"

namespace TR
{
	namespace Xml
	{
		class Element
		{
		public:
													Element() {  }
			virtual								~Element() {  }

			virtual void						addChild(TR::Xml::Element* child) = 0;
			virtual std::string				getAttributeValue(std::string attribute) = 0;
			virtual TR::Xml::Element*		getFirstChildByName(std::string name) = 0;
			virtual TR::Xml::Element*		getFirstChild() = 0;
			virtual TR::Xml::Element*		getLastChild() = 0;
			virtual std::string				getName() = 0;
			virtual TR::Xml::Element*		getNextChild() = 0;
			virtual TR::Xml::Element*		getNextSibling() = 0;
			virtual TR::Xml::Element*		getParent() = 0;
			virtual TR::Xml::Element*		getNextSiblingByName(std::string name) = 0;
			virtual void 						setAttributeValue(std::string attribute, std::string value) = 0;
			virtual void						setNextSibling(TR::Xml::Element* nextSibling) = 0;
		};
	}
}

#endif
