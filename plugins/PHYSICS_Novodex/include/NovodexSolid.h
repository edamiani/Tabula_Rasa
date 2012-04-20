#ifndef __TR_NOVODEX_SOLID__
#define __TR_NOVODEX_SOLID__

#include "..\..\..\include\PhysicsSolid.h"
#include "NxPhysics.h"

namespace TR
{
	namespace Physics
	{
		class NovodexSolid : public TR::Physics::Solid
		{
		public:
											NovodexSolid(const TR::Physics::SolidDesc &solidDesc);
											~NovodexSolid() { mNxActor = 0; }

			void							applyForce(const TR::Vector3 &force, TR::Physics::forceMode mode = FM_FORCE);
			void							applyForceAtLocalPos(const TR::Vector3 &force, const TR::Vector3 &position, TR::Physics::forceMode mode = FM_FORCE);
			void							applyTorque(const TR::Vector3 &torque, TR::Physics::forceMode mode = FM_FORCE);
			TR::uint32					getTriggerFlag() { return mTriggerFlag; }
			std::string					getName() { return mName; };
			TR::Quaternion				getOrientation();
			TR::Vector3					getPosition();
			TR::Physics::solidType	getType() { return mSolidType; }
			void							moveGlobalOrientation(const TR::Quaternion &orientation);
			void							moveGlobalPosition(const TR::Vector3 &position);
			void							setAngularVelocity(const TR::Vector3 &velocity);
			void							setLinearVelocity(const TR::Vector3 &velocity);
			void							setName(std::string name);
			void							setSolidOffset(const TR::Vector3 &offset) { mOffset = offset; }

			NxForceMode					_convertForceMode(TR::Physics::forceMode mode);
			NxActor*						_getNxActor() { return mNxActor; }
			void							_setNxActor(NxActor* actor) { mNxActor = actor; }

		private:
			TR::Physics::collisionType	mCollisionType;
			TR::Real							mDensity;
			short int						mGroup;
			std::string						mMaterialName;
			std::string						mName;
			TR::Vector3						mOffset;
			TR::Quaternion					mOrientation;
			TR::Vector3						mPosition;
			TR::Vector3						mScale;
			TR::uint32						mTriggerFlag;
			TR::Physics::solidType		mSolidType;

			NxActor							*mNxActor;
		};
	}
}

#endif