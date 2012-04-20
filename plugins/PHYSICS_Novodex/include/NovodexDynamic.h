#ifndef __TR_NOVODEX_DYNAMIC__
#define __TR_NOVODEX_DYNAMIC__

#include "../../../include/AxisAlignedBox.h"
#include "../../../include/Dependencies.h"
#include "../../../include/EntityManager.h"
#include "../../../include/EntityDynamic.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsManager.h"
#include "../include/NovodexManager.h"
#include "../../../include/PhysicsSolid.h"

namespace TR
{
	namespace Entity
	{
		class NovodexDynamic : public TR::Entity::Dynamic
		{
		public:
			NovodexDynamic(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
			{ 
				mSprite = framework->getEntityManager()->createSprite(spriteDesc);
				const_cast<TR::Physics::SolidDesc&>(solidDesc).sprite = mSprite;
				mSolid = framework->getPhysicsManager()->createSolid(solidDesc);
				TR::Vector3 position = mSolid->getPosition();
				TR::Quaternion orientation = mSolid->getOrientation();
				mSprite->setPosition(position);
				mSprite->setOrientation(orientation);
			}

			virtual ~NovodexDynamic() { mSprite = 0; mSolid = 0; }

			void								applyForce(const TR::Vector3 &force, TR::Physics::forceMode mode = TR::Physics::FM_FORCE) { mSolid->applyForce(force, mode); }
			void								applyForceAtLocalPos(const TR::Vector3 &force, const TR::Vector3 &position, TR::Physics::forceMode mode = TR::Physics::FM_FORCE) { mSolid->applyForceAtLocalPos(force, position, mode); }
			void								applyTorque(const TR::Vector3 &torque, TR::Physics::forceMode mode = TR::Physics::FM_FORCE) { mSolid->applyTorque(torque, mode); }
			const TR::AxisAlignedBox&	getBoundingBox() { return mSprite->getBoundingBox(); }
			TR::Real							getBoundingRadius() { return mSprite->getBoundingRadius(); }
			std::string						getName() { return mSprite->getName(); }
			TR::Quaternion					getOrientation() { return mSprite->getOrientation(); };
			TR::Vector3						getPosition() { return mSprite->getPosition(); };
			TR::Vector3						getScale() { return mSprite->getScale(); };
			TR::Physics::Solid*			getSolid() { return mSolid; }
			TR::Entity::Sprite*			getSprite() { return mSprite; }
			TR::Entity::Object3dType	getType() { return TR::Entity::OT_STATIC; };
			void								moveGlobalPosition(const TR::Vector3 &position) { mSolid->moveGlobalPosition(position); }
			void								setAngularVelocity(const TR::Vector3 &velocity) { mSolid->setAngularVelocity(velocity); }
			void								setLinearVelocity(const TR::Vector3 &velocity) { mSolid->setLinearVelocity(velocity); }
			void								setVisible(bool isVisible) { mSprite->setVisible(isVisible); }
			void								update(unsigned long timeSinceLastFrame);

		protected:
			TR::Entity::Sprite	*mSprite;
			TR::Physics::Solid	*mSolid;
		};
	}
}

#endif