#ifndef __TR_PHYSICS_SOLID__
#define __TR_PHYSICS_SOLID__

#include "Dependencies.h"
#include "PhysicsManager.h"
#include "EntitySprite.h"

namespace TR
{
	namespace Physics
	{
		enum collisionNotify
		{
			CN_IGNORE_PAIR = 1,
			CN_START_TOUCH = 2,
			CN_TOUCH = 4,
			CN_END_TOUCH = 8
		};

		struct SolidDesc
		{
			bool								ccd;
			TR::Physics::collisionType	collisionType;
			TR::Real							density;
			TR::uint16						group;
			bool								isTrigger;
			TR::Real							mass;
			std::string						materialName;
			std::string						name;
			TR::Vector3						offset;
			TR::Quaternion					orientation;
			TR::Vector3						position;
			TR::Real							radius;
			TR::Vector3						scale;
			TR::Entity::Sprite*			sprite;
			TR::uint32						triggerFlag;
			TR::Physics::solidType		solidType;

			SolidDesc()
			{
				ccd = false;
				collisionType = TR::Physics::CT_BOX;
				density = 10.0;
				group = 0;
				isTrigger = false;
				mass = 0.0;
				materialName = "";
				offset = TR::Vector3::ZERO;
				orientation = TR::Quaternion::IDENTITY;
				position = TR::Vector3::ZERO;
				radius = 0.0;
				scale = TR::Vector3::UNIT_SCALE;
				solidType = TR::Physics::ST_STATIC;
				triggerFlag = 0;
			}

			~SolidDesc() { }
		};

		class Solid
		{
		public:
														Solid(const TR::Physics::SolidDesc &solidDesc) {  }			
			virtual									~Solid() {  }

			virtual void							applyForce(const TR::Vector3 &force, TR::Physics::forceMode mode = FM_FORCE) = 0;
			virtual void							applyForceAtLocalPos(const TR::Vector3 &force, const TR::Vector3 &position, TR::Physics::forceMode mode = FM_FORCE) = 0;
			virtual void							applyTorque(const TR::Vector3 &torque, TR::Physics::forceMode mode = FM_FORCE) = 0;
			virtual TR::uint32					getTriggerFlag() = 0;
			virtual std::string					getName() = 0;
			virtual TR::Quaternion				getOrientation() = 0;
			virtual TR::Vector3					getPosition() = 0;
			virtual TR::Physics::solidType	getType() = 0;
			virtual void							moveGlobalOrientation(const TR::Quaternion &orientation) = 0;
			virtual void							moveGlobalPosition(const TR::Vector3 &position) = 0;
			virtual void							setAngularVelocity(const TR::Vector3 &velocity) = 0;
			virtual void							setLinearVelocity(const TR::Vector3 &velocity) = 0;
			virtual void							setName(std::string name) = 0;
			virtual void							setSolidOffset(const TR::Vector3 &offset) = 0;
		};
	}
}

#endif