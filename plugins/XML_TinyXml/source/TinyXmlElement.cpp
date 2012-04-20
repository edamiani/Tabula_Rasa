#include "..\include\TinyXmlElement.h"

namespace TR
{
	namespace Xml
	{
		TinyXmlElement::~TinyXmlElement()
		{
			mElement = 0;
			mNextSibling = 0;
			mParentElement = 0;

			if(!mChildren.empty())
			{
				mChildrenIterator = mChildren.begin();
				while(mChildrenIterator != mChildren.end())
				{
					delete *mChildrenIterator;
					mChildrenIterator++;
				}
			}

			mChildren.clear();
		}

		std::string TinyXmlElement::getAttributeValue(std::string attribute)
		{ 
			if(mElement->Attribute(attribute.c_str())) 
				return std::string(mElement->Attribute(attribute.c_str()));
			else
				return "";
		}

		TR::Xml::Element*	TinyXmlElement::getFirstChildByName(std::string name)
		{
			if(!mChildren.empty())
			{
				mChildrenIterator = mChildren.begin();

				while(mChildrenIterator != mChildren.end())
				{
					if((*mChildrenIterator)->getName() == name)
						return *mChildrenIterator;

					mChildrenIterator++;
				}

				return 0;
			}
			else
				return 0;
		}

		TR::Xml::Element*	TinyXmlElement::getNextSiblingByName(std::string name)
		{
			TR::Xml::TinyXmlElement *next = mNextSibling;

			while(next)
			{
				if(next->getName() == name)
					return next;
				else
					next = (TR::Xml::TinyXmlElement*)next->getNextSibling();
			}

			return 0;
		}
	}
}
