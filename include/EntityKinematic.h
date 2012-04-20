#ifndef __TR_ENTITY_KINEMATIC__
#define __TR_ENTITY_KINEMATIC__

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
		class Kinematic
		{
		public:
															Kinematic() {  }
			virtual										~Kinematic() {  }

			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual TR::uint32						getId() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Scene::Node*				getNode() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Physics::Solid*			getSolid() = 0;
			virtual TR::Entity::Sprite*			getSprite() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual void								moveGlobalPosition(const TR::Vector3 &position) = 0;
			virtual void								moveGlobalOrientation(const TR::Quaternion &orientation) = 0;
			virtual void								setVisible(bool isVisible) = 0;
			virtual void								update(unsigned long timeSinceLastFrame) = 0;
		};
	}
}

#endif