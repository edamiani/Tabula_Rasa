#include "../include/NovodexDynamic.h"

namespace TR
{
	namespace Entity
	{
		void NovodexDynamic::update(unsigned long timeSinceLastFrame)
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