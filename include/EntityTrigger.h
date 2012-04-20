#ifndef __TR_ENTITY_TRIGGER__
#define __TR_ENTITY_TRIGGER__

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
		enum triggerNotify
		{
			TN_ON_ENTER,
			TN_ON_STAY,
			TN_ON_LEAVE
		};

		class Trigger
		{
		public:
															Trigger() {  }
			virtual										~Trigger() {  }

			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual TR::Vector3						getWorldPosition() = 0;
			virtual void								setVisible(bool isVisible) = 0;
			virtual void								update(TR::Real timeInSeconds) = 0;
		};
	}
}

#endif