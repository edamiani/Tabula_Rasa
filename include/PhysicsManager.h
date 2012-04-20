/*
-----------------------------------------------------------------------------
Filename:    PhysicsManager.h
Description: Physics module, generic
-----------------------------------------------------------------------------
*/

#ifndef __TR_PHYSICS_MANAGER__
#define __TR_PHYSICS_MANAGER__

#include "Dependencies.h"
#include "EntitySprite.h"
#include "Vector3.h"

namespace TR
{
	namespace Physics
	{
		enum collisionType
		{
			CT_BOX,
			CT_SPHERE,
			CT_CAPSULE,
			CT_PLANE,
			CT_TREECOLLISION,
		};

		enum forceMode
		{
			FM_ACCELERATION,
			FM_FORCE,
			FM_IMPULSE,
			FM_SMOOTH_IMPULSE,
			FM_SMOOTH_VELOCITY_CHANGE,
			FM_VELOCITY_CHANGE
		};

		enum jointType
		{
			JT_BALLANDSOCKET = 1,
			JT_HINGE,
			JT_SLIDER,
			JT_UNIVERSAL,
			JT_WHEEL,
			JT_FIXED
		};

		enum solidType
		{
			ST_CHARACTER = 1,
			ST_DYNAMIC,
			ST_KINEMATIC,
			ST_STATIC,
			ST_TRIGGER
		};

		struct Limit
		{
			TR::Vector3 position;
			TR::Vector3 plane;

			Limit()
			{
				position = TR::Vector3::ZERO;
				plane = TR::Vector3::ZERO;
			}
		};

		struct RotationLimits
		{
			bool			enabled;
			Limit			high;
			Limit			low;
			TR::Vector3 offset;

			RotationLimits()
			{
				offset.x = 0.0;
				offset.y = 0.0;
				offset.z = 0.0;
			}
		};

		struct TranslationLimits
		{
			bool	enabled;
			Limit	high;
			Limit	low;
		};

		struct JointDesc
		{
			TR::Vector3					anchor;
			TR::Vector3					axes[3];
			std::string					child;
			TR::Physics::jointType	jointType;
			std::string					parent;
			RotationLimits				rotationLimits[3];
			TranslationLimits			translationLimits;

			JointDesc()
			{
				anchor = TR::Vector3::ZERO;
				axes[0] = axes[1] = axes[2] = TR::Vector3::ZERO;
				child = "";
				jointType = TR::Physics::JT_FIXED;
				parent = "";
				rotationLimits[0].enabled = false;
				rotationLimits[1].enabled = false;
				rotationLimits[2].enabled = false;
				translationLimits.enabled = false;
			}

			~JointDesc() { }
		};

		class Manager
		{
		// Public methods
		public:
														Manager() {  }
			virtual									~Manager() {  }

			virtual TR::Entity::Character*	createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Dynamic*		createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual void							createJoint(const JointDesc &jointDesc) = 0;
			virtual TR::Entity::Kinematic*	createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Physics::Material*	createMaterial(const TR::Physics::MaterialDesc &material) = 0;
			virtual TR::Physics::Solid*		createSolid(const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Static*		createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Trigger*		createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Vehicle*		createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc) = 0;
			virtual void							destroyDynamicEntity(TR::Entity::Dynamic *dynamic) = 0;
			virtual void							destroyKinematicEntity(TR::Entity::Kinematic *kinematic) = 0;
			virtual void							destroySolid(TR::Physics::Solid *solid) = 0;
			virtual void							destroyStaticEntity(TR::Entity::Static *staticEntity) = 0;
			virtual TR::Physics::Material*	getDefaultMaterial() = 0;
			virtual TR::Real						getDistanceFromClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance = 3000.0) = 0;
			virtual TR::Physics::Material*	getMaterial(const std::string &name) = 0;
			virtual TR::Physics::Solid*		getSolid(const std::string &name) = 0;
			virtual void							initialize(TR::Framework *framework) = 0;
			virtual bool							isPointInSphere(const TR::Vector3 &sphereCenter, TR::Real sphereRadius, const TR::Vector3 &point) = 0;
			virtual TR::uint32					raycastClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance = 3000.0) = 0;
			virtual void							setCollisionPairFlags(TR::Physics::Solid *solid1, TR::Physics::Solid *solid2, TR::uint32 collisionNotify) = 0;
			virtual void							setGravity(TR::Real gravity) = 0;
			virtual void							setSolidGroup(TR::Physics::Solid *solid, TR::uint16 group) = 0;
			virtual void							simulate(TR::Real dt) = 0;
		};
	}
}

#endif