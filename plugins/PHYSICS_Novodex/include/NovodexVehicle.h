#ifndef __TR_NOVODEX_VEHICLE__
#define __TR_NOVODEX_VEHICLE__

#include "../../../include/Dependencies.h"
#include "../../../include/EntityManager.h"
#include "../../../include/EntityVehicle.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsManager.h"
#include "../../../include/PhysicsSolid.h"
#include "../../../include/SceneNode.h"
#include "NovodexManager.h"
#include "NovodexWheel.h"

namespace TR
{
	namespace Entity
	{
		class NovodexVehicle : public TR::Entity::Vehicle
		{
		public:
												NovodexVehicle(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc);
			virtual							~NovodexVehicle();

			void								accelerate(TR::Real timeInSeconds);
			void								brake(TR::Real timeInSeconds);
			void								decelerate(TR::Real timeInSeconds);
			void								gearUp() { mGearBox->gearUp(); }
			void								gearDown() { mGearBox->gearDown(); }
			const TR::AxisAlignedBox&	getBoundingBox() { return mSprite->getBoundingBox(); }
			TR::Real							getBoundingRadius() { return mSprite->getBoundingRadius(); }
			int								getCurrentGear() { return mGearBox->getCurrentGear(); }
			TR::Real							getCurrentRpm() { return mCurrentRpm; }
			TR::Real							getCurrentTorque() { return mCurrentTorque; }
			std::string						getName() { return mSprite->getName(); }
			TR::Scene::Node*				getNode() { return mSprite->getNode(); }
			TR::Quaternion					getOrientation() { return mSolid->getOrientation(); };
			TR::Vector3						getPosition() { return mSprite->getNode()->getParent()->getPosition(); };
			TR::Vector3						getScale() { return mSprite->getScale(); };
			TR::Real							getSpeed();
			TR::Real							getSteeringAngle() { return mTotalSteering; }
			TR::Real							getSteeringStep() { return mCurrentSteering; }
			TR::Entity::Object3dType	getType() { return TR::Entity::OT_STATIC; };
			void								moveGlobalPosition(const TR::Vector3 &position) { mSolid->moveGlobalPosition(position); }
			void								realignWheels();
			void								setAngularVelocity(const TR::Vector3 &velocity) { mSolid->setAngularVelocity(velocity); }
			void								setBrakeTorque(TR::Real torque);
			void								setCurrentGear(int gear) { mGearBox->_setCurrentGear(gear); }
			void								setEngineSound(const std::string &soundFile);
			void								setLinearVelocity(const TR::Vector3 &velocity) { mSolid->setLinearVelocity(velocity); }
			void								setMotorTorque(TR::Real torque);
			void								setOrientation(TR::Quaternion orientation);
			void								setPosition(TR::Vector3 position);
			void								setSpeed(TR::Real speed) { mCurrentSpeed = speed; }
			void								setSteeringAngle(TR::Real angle) { mTotalSteering = angle; }
			void								steer(TR::Physics::SteeringSide side, TR::Real timeInSeconds, bool lockValue = true);
			void								setVisible(bool isVisible) { mSprite->setVisible(isVisible); }
			void								setWheel(TR::Entity::WheelPosition position, TR::Physics::Wheel *wheel);
			void								update(TR::Real timeInSeconds);

			TR::Framework*					_getFramework() { return mFramework; }
			NxActor*							_getNxActor() { return reinterpret_cast<TR::Physics::NovodexSolid *>(mSolid)->_getNxActor(); }

		private:
			TR::Real							mAngularSpeed;
			TR::Real							mBrakeForce;
			TR::Real							mCDrag;
			TR::Real							mCRollingResistance;
			TR::Real							mCurrentRpm;
			TR::Real							mCurrentSpeed;
			TR::Real							mCurrentSteering;
			TR::Real							mCurrentTorque;
			TR::Physics::Engine			*mEngine;
			bool								mFrontTraction;
			TR::Physics::GearBox			*mGearBox;
			TR::Real							mMass;
			TR::Real							mMaxSteering;
			TR::Real							mSteeringRatio;
			TR::Real							mTotalSteering;
			TR::Real							mWheelRadius;

			int								mEngineSoundId;
			TR::Framework					*mFramework;
			TR::Physics::NovodexWheel	*mFLWheel;
			TR::Physics::NovodexWheel	*mFRWheel;
			TR::Physics::NovodexWheel	*mRLWheel;
			TR::Physics::NovodexWheel	*mRRWheel;
			TR::Entity::Sprite			*mSprite;
			TR::Physics::Solid			*mSolid;
		};
	}
}

#endif