#include "..\include\NovodexSolid.h"

namespace TR
{
	namespace Physics
	{
		NovodexSolid::NovodexSolid(const TR::Physics::SolidDesc &solidDesc) 
			: Solid(solidDesc) 
		{
			mCollisionType = solidDesc.collisionType;
			mDensity = solidDesc.density;
			mGroup = solidDesc.group;
			mMaterialName = solidDesc.materialName;
			mName = solidDesc.name;
			mOffset = solidDesc.offset;
			mOrientation = solidDesc.orientation;
			mPosition = solidDesc.position;
			mScale = solidDesc.scale;
			mTriggerFlag = solidDesc.triggerFlag;
			mSolidType = solidDesc.solidType;

			mNxActor = 0; 
		}

		void NovodexSolid::applyForce(const TR::Vector3 &force, TR::Physics::forceMode mode)
		{
			NxVec3 nxForce(static_cast<NxReal>(force.x), static_cast<NxReal>(force.y), static_cast<NxReal>(force.z));
			mNxActor->addForce(nxForce, _convertForceMode(mode));
		}

		void NovodexSolid::applyForceAtLocalPos(const TR::Vector3 &force, const TR::Vector3 &position, TR::Physics::forceMode mode)
		{
			NxVec3 nxForce(static_cast<NxReal>(force.x), static_cast<NxReal>(force.y), static_cast<NxReal>(force.z));
			NxVec3 nxPosition(static_cast<NxReal>(position.x), static_cast<NxReal>(position.y), static_cast<NxReal>(position.z));
			mNxActor->addForceAtLocalPos(nxForce, nxPosition, _convertForceMode(mode));
		}

		void NovodexSolid::applyTorque(const TR::Vector3 &torque, TR::Physics::forceMode mode)
		{
			NxVec3 nxTorque(static_cast<NxReal>(torque.x), static_cast<NxReal>(torque.y), static_cast<NxReal>(torque.z));
			mNxActor->addTorque(nxTorque, _convertForceMode(mode));
		}

		TR::Quaternion NovodexSolid::getOrientation() 
		{
			NxQuat quat = mNxActor->getGlobalOrientationQuat();
			mOrientation.x = quat.x;
			mOrientation.y = quat.y;
			mOrientation.z = quat.z;
			mOrientation.w = quat.w;
			return mOrientation; 
		}

		TR::Vector3 NovodexSolid::getPosition() 
		{ 
			NxVec3 vec = mNxActor->getGlobalPosition();
			mPosition.x = vec.x;
			mPosition.y = vec.y;
			mPosition.z = vec.z;
			return mPosition; 
		}

		void NovodexSolid::moveGlobalOrientation(const TR::Quaternion &orientation)
		{
			NxQuat nxOrientation;
			nxOrientation.x = orientation.x;
			nxOrientation.y = orientation.y;
			nxOrientation.z = orientation.z;
			nxOrientation.w = orientation.w;
			mNxActor->moveGlobalOrientationQuat(nxOrientation);
		}

		void NovodexSolid::moveGlobalPosition(const TR::Vector3 &position)
		{
			NxVec3 nxPosition(static_cast<NxReal>(position.x), static_cast<NxReal>(position.y), static_cast<NxReal>(position.z));
			mNxActor->moveGlobalPosition(nxPosition);
		}

		void NovodexSolid::setAngularVelocity(const TR::Vector3 &velocity)
		{
			NxVec3 nxVelocity(static_cast<NxReal>(velocity.x), static_cast<NxReal>(velocity.y), static_cast<NxReal>(velocity.z));
			mNxActor->setAngularVelocity(nxVelocity);
		}

		void NovodexSolid::setLinearVelocity(const TR::Vector3 &velocity)
		{
			NxVec3 nxVelocity(static_cast<NxReal>(velocity.x), static_cast<NxReal>(velocity.y), static_cast<NxReal>(velocity.z));
			mNxActor->setLinearVelocity(nxVelocity);
		}

		void NovodexSolid::setName(std::string name)
		{
			mName = name;
			mNxActor->setName(name.c_str());
		}

		NxForceMode NovodexSolid::_convertForceMode(TR::Physics::forceMode mode)
		{
			switch(mode)
			{
				case TR::Physics::FM_ACCELERATION:
					return NX_ACCELERATION;
					break;
				case TR::Physics::FM_FORCE:
					return NX_FORCE;
					break;
				case TR::Physics::FM_IMPULSE:
					return NX_IMPULSE;
					break;
				case TR::Physics::FM_SMOOTH_IMPULSE:
					return NX_SMOOTH_IMPULSE;
					break;
				case TR::Physics::FM_SMOOTH_VELOCITY_CHANGE:
					return NX_SMOOTH_VELOCITY_CHANGE;
					break;
				case TR::Physics::FM_VELOCITY_CHANGE:
					return NX_VELOCITY_CHANGE;
					break;
				default:
					return NX_FORCE;
					break;
			}
		}
	}
}