#include "..\include\TinyXmlManager.h"

namespace TR
{
	namespace Xml
	{
		TinyXmlManager::~TinyXmlManager()
		{
			mDocumentsIterator = mDocuments.begin();
			while(mDocumentsIterator != mDocuments.end())
			{
				delete (mDocumentsIterator->second);
				mDocumentsIterator++;
			}

			mDocuments.clear();
		}

		TR::Xml::Document* TinyXmlManager::createDocument(std::string name, std::string fileName)
		{
			TR::Xml::TinyXmlDocument *document = new TR::Xml::TinyXmlDocument(fileName);
			assert(document != 0);

			mDocuments[name] = document;

			return document;
		}

		void TinyXmlManager::destroyDocument(std::string name)
		{
			mDocumentsIterator = mDocuments.find(name);
			if(mDocumentsIterator != mDocuments.end())
			{
				delete (mDocumentsIterator->second);
				mDocuments.erase(mDocumentsIterator);
			}
		}

		TR::Xml::Document* TinyXmlManager::getDocument(std::string name)
		{
			mDocumentsIterator = mDocuments.find(name);
			if(mDocumentsIterator != mDocuments.end())
				return mDocumentsIterator->second;
			else
				return 0;
		}
	}
}
