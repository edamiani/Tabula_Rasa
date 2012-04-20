#ifndef __TR_ENTITY_STATIC__
#define __TR_ENTITY_STATIC__

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
		class Static
		{
		public:
															Static() {  }
			virtual										~Static() {  }

			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Physics::Solid*			getSolid() = 0;
			virtual TR::Entity::Sprite*			getSprite() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual void								setVisible(bool isVisible) = 0;
		};
	}
}

#endif