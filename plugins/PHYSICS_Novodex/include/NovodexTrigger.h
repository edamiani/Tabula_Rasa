#ifndef __TR_NOVODEX_TRIGGER__
#define __TR_NOVODEX_TRIGGER__

#include "../../../include/AxisAlignedBox.h"
#include "../../../include/Dependencies.h"
#include "../../../include/EntityManager.h"
#include "../../../include/EntityTrigger.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsManager.h"
#include "../include/NovodexManager.h"
#include "../../../include/PhysicsSolid.h"

namespace TR
{
	namespace Entity
	{
		class NovodexTrigger : public TR::Entity::Trigger
		{
		public:
			NovodexTrigger(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
			{ 
				mSprite = framework->getEntityManager()->createSprite(spriteDesc);
				const_cast<TR::Physics::SolidDesc&>(solidDesc).sprite = mSprite;
				mSolid = framework->getPhysicsManager()->createSolid(solidDesc);
			}

			virtual ~NovodexTrigger() {  }

			const TR::AxisAlignedBox&	getBoundingBox() { return mSprite->getBoundingBox(); }
			TR::Real							getBoundingRadius() { return mSprite->getBoundingRadius(); }
			std::string						getName() { return mSprite->getName(); }
			TR::Quaternion					getOrientation() { return mSprite->getOrientation(); };
			TR::Vector3						getPosition() { return mSprite->getPosition(); };
			TR::Vector3						getScale() { return mSprite->getScale(); };
			TR::Entity::Object3dType	getType() { return TR::Entity::OT_STATIC; };
			TR::Vector3						getWorldPosition() { return mSprite->getWorldPosition(); }
			void								setVisible(bool isVisible) { mSprite->setVisible(isVisible); }
			void								update(TR::Real timeInSeconds) { mSolid->moveGlobalPosition(mSprite->getWorldPosition()); }

		private:
			TR::Entity::Sprite	*mSprite;
			TR::Physics::Solid	*mSolid;
		};
	}
}

#endif