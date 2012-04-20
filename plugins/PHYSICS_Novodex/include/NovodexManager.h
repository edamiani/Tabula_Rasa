/*
-----------------------------------------------------------------------------
Filename:    PhysicsManager.h
Description: Physics module, generic
-----------------------------------------------------------------------------
*/

#ifndef __TR_NOVODEX_MANAGER__
#define __TR_NOVODEX_MANAGER__

#include "..\..\..\include\PhysicsManager.h"
#include "..\..\..\include\Singleton.h"
#include "NovodexMaterial.h"
#include "NovodexSolid.h"
#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxController.h"
#include "ControllerManager.h"
#include "NxCapsuleController.h"
#include "Ogre.h"

namespace TR
{
	namespace Physics
	{
		class NovodexManager : public TR::Physics::Manager, public NxUserTriggerReport, public NxUserContactReport
		{
		// Public methods
		public:
											NovodexManager();
											~NovodexManager();

			TR::Entity::Character*	createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Dynamic*		createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			void							createJoint(const JointDesc &jointDesc);
			TR::Entity::Kinematic*	createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Physics::Material*	createMaterial(const TR::Physics::MaterialDesc &material);
			TR::Physics::Solid*		createSolid(const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Static*		createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Trigger*		createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Vehicle*		createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc);
			void							destroyDynamicEntity(TR::Entity::Dynamic *dynamic);
			void							destroyKinematicEntity(TR::Entity::Kinematic *kinematic);
			void							destroySolid(TR::Physics::Solid *solid);
			void							destroyStaticEntity(TR::Entity::Static *staticEntity);
			TR::Physics::Material*	getDefaultMaterial() { return mDefaultMaterial; }
			TR::Real						getDistanceFromClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance = 3000.0);
			TR::Physics::Material*	getMaterial(const std::string &name);
			TR::Physics::Solid*		getSolid(const std::string &name);
			void							initialize(TR::Framework *framework);
			bool							isPointInSphere(const TR::Vector3 &sphereCenter, TR::Real sphereRadius, const TR::Vector3 &point);
			TR::uint32					raycastClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance = 3000.0);
			void							setCollisionPairFlags(TR::Physics::Solid *solid1, TR::Physics::Solid *solid2, TR::uint32 collisionNotify);
			void							setGravity(TR::Real gravity) { mDefaultScene->setGravity(NxVec3(0.0, gravity, 0.0)); }
			void							setSolidGroup(TR::Physics::Solid *solid, TR::uint16 group);
			void							simulate(TR::Real dt);

			// Novodex contact and trigger reports
			void onContactNotify(NxContactPair& pair, NxU32 events);
			void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);

			NxScene*						_getNxScene() { return mDefaultScene; }
			NxMaterial*					_getTireMaterial() { return mTireMaterial; }

		// Private methods
		private:
			NxTriangleMeshShapeDesc*	_createMeshCollision(const Ogre::Entity* entity, const TR::Vector3 &scale);

			NxVec3							mDefaultGravity;
			//NxMaterial						*mDefaultMaterial;
			TR::Physics::Material		*mDefaultMaterial;
			NxMaterial						*mTireMaterial;
			NxScene							*mDefaultScene;
			TR::Framework					*mFramework;
			bool								mIsInitialized;
			ControllerManager				mNxControllerManager;
			NxPhysicsSDK					*mWorld;

			std::map<std::string, TR::Physics::NovodexMaterial*>				mMaterials;
			std::map<std::string, TR::Physics::NovodexMaterial*>::iterator	mMaterialsIterator;
			std::map<std::string, TR::Physics::NovodexSolid*>					mSolids;
			std::map<std::string, TR::Physics::NovodexSolid*>::iterator		mSolidsIterator;
		};
	}
}

#endif