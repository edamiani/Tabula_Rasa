#ifndef __TR_ENTITY_DYNAMIC__
#define __TR_ENTITY_DYNAMIC__

#include "AxisAlignedBox.h"
#include "Dependencies.h"
#include "EntityManager.h"
#include "EntitySprite.h"
#include "Framework.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "PhysicsSolid.h"

namespace TR
{
	namespace Entity
	{
		class Dynamic
		{
		public:
															Dynamic() {  }
			virtual										~Dynamic() {  }

			virtual void								applyForce(const TR::Vector3 &force, TR::Physics::forceMode mode = TR::Physics::FM_FORCE) = 0;
			virtual void								applyForceAtLocalPos(const TR::Vector3 &force, const TR::Vector3 &position, TR::Physics::forceMode = TR::Physics::FM_FORCE) = 0;
			virtual void								applyTorque(const TR::Vector3 &torque, TR::Physics::forceMode mode = TR::Physics::FM_FORCE) = 0;
			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Physics::Solid*			getSolid() = 0;
			virtual TR::Entity::Sprite*			getSprite() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual void								moveGlobalPosition(const TR::Vector3 &position) = 0;
			virtual void								setAngularVelocity(const TR::Vector3 &velocity) = 0;
			virtual void								setLinearVelocity(const TR::Vector3 &velocity) = 0;
			virtual void								setVisible(bool isVisible) = 0;
			virtual void								update(unsigned long timeSinceLastFrame) = 0;
		};
	}
}

#endif