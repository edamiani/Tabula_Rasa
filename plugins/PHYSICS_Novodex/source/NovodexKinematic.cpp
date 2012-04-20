#include "../include/NovodexKinematic.h"

namespace TR
{
	namespace Entity
	{
		void NovodexKinematic::moveGlobalOrientation(const TR::Quaternion &orientation)
		{ 
			mSolid->moveGlobalOrientation(orientation); 
			mSprite->setOrientation(orientation); 
		}

		void NovodexKinematic::moveGlobalPosition(const TR::Vector3 &position) 
		{ 
			mSolid->moveGlobalPosition(position); 
			mSprite->setPosition(position); 
		}

		void NovodexKinematic::update(unsigned long timeSinceLastFrame)
		{
			TR::Vector3 position = mSolid->getPosition();
			TR::Quaternion orientation = mSolid->getOrientation();
			orientation.normalise();
			mSprite->setPosition(position);
			mSprite->setOrientation(orientation);
			//mSprite->updateAnimation(timeSinceLastFrame);
		}
	}
}