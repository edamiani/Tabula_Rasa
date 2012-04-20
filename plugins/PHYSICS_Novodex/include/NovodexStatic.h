#ifndef __TR_NOVODEX_STATIC__
#define __TR_NOVODEX_STATIC__

#include "../../../include/AxisAlignedBox.h"
#include "../../../include/Dependencies.h"
#include "../../../include/EntityManager.h"
#include "../../../include/EntityStatic.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsManager.h"
#include "../include/NovodexManager.h"
#include "../../../include/PhysicsSolid.h"

namespace TR
{
	namespace Entity
	{
		class NovodexStatic : public TR::Entity::Static
		{
		public:
			NovodexStatic(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
			{ 
				mSprite = framework->getEntityManager()->createSprite(spriteDesc);
				const_cast<TR::Physics::SolidDesc&>(solidDesc).sprite = mSprite;
				mSolid = framework->getPhysicsManager()->createSolid(solidDesc);
			}

			virtual ~NovodexStatic() {  }

			const TR::AxisAlignedBox&	getBoundingBox() { return mSprite->getBoundingBox(); }
			TR::Real							getBoundingRadius() { return mSprite->getBoundingRadius(); }
			std::string						getName() { return mSprite->getName(); }
			TR::Quaternion					getOrientation() { return mSprite->getOrientation(); };
			TR::Vector3						getPosition() { return mSprite->getPosition(); };
			TR::Vector3						getScale() { return mSprite->getScale(); };
			TR::Physics::Solid*			getSolid() { return mSolid; }
			TR::Entity::Sprite*			getSprite() { return mSprite;}
			TR::Entity::Object3dType	getType() { return TR::Entity::OT_STATIC; };
			void								setVisible(bool isVisible) { mSprite->setVisible(isVisible); }

		private:
			TR::Entity::Sprite	*mSprite;
			TR::Physics::Solid	*mSolid;
		};
	}
}

#endif