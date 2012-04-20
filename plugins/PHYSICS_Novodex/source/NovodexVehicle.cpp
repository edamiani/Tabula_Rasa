#include "../include/NovodexVehicle.h"
//#include "../../../include/SceneNode.h"
//#include "../../../include/AudioManager.h"

namespace TR
{
	namespace Entity
	{
		NovodexVehicle::NovodexVehicle(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc)
			: mCDrag(0.4257), mCRollingResistance(12.8), mCurrentSpeed(0), 
			  mMaxSteering(0.785), mCurrentSteering(0), mTotalSteering(0), 
			  mCurrentTorque(0), mBrakeForce(0), mCurrentRpm(0)
		{
			mGearBox = new TR::Physics::GearBox();
			mGearBox->setReverseGear(vehicleDesc.gears.front());
			std::list<TR::Physics::Gear>::iterator it = vehicleDesc.gears.begin(); it++;
			for( ; it != vehicleDesc.gears.end(); it++)
				mGearBox->insertGear((*it));

			mEngine = new TR::Physics::Engine(vehicleDesc.engineDesc, mGearBox);

			mFramework = framework;
			mSprite = mFramework->getEntityManager()->createSprite(spriteDesc);
			reinterpret_cast<TR::Physics::SolidDesc &>(const_cast<TR::Entity::VehicleDesc &>(vehicleDesc)).sprite = mSprite;
			mSolid = mFramework->getPhysicsManager()->createSolid(reinterpret_cast<const TR::Physics::SolidDesc &>(vehicleDesc));
			NxVec3 vec(vehicleDesc.position.x, vehicleDesc.position.y, vehicleDesc.position.z);
			_getNxActor()->setGlobalPosition(vec);

			NxVec3 cMass = _getNxActor()->getCMassLocalPosition();
			cMass.x += vehicleDesc.cMassOffset.x;
			cMass.y += vehicleDesc.cMassOffset.y;
			cMass.z += vehicleDesc.cMassOffset.z;
			_getNxActor()->setCMassOffsetLocalPosition(cMass);

			mEngineSoundId = -1;
			mAngularSpeed = 0.0;
			mFrontTraction = vehicleDesc.frontTraction;
			mMass = _getNxActor()->getMass();
			_getNxActor()->wakeUp(1e10);
			mWheelRadius = vehicleDesc.frontWheelDesc.radius;
			mBrakeForce = vehicleDesc.brakeForce;
			mSteeringRatio = vehicleDesc.steeringRatio;
			mMaxSteering = vehicleDesc.maxSteering;

			TR::Scene::NodeDesc nodeDesc;

			// FRWheel
			//if(mFrontTraction)
				vehicleDesc.frontWheelDesc.speedOverride = true;
			//else
				//vehicleDesc.wheelDesc.speedOverride = false;
			vehicleDesc.frontWheelDesc.spriteDesc.name = mSprite->getName() + "_FRWheel";
			nodeDesc.name = mSprite->getName() + "_FRWheel";
			nodeDesc.position = vehicleDesc.frontWheelDesc.position;
			nodeDesc.orientation = vehicleDesc.frontWheelDesc.orientation;
			//nodeDesc.scale = vehicleDesc.wheelDesc.scale;
			nodeDesc.parent = mSprite->getNode();
			vehicleDesc.frontWheelDesc.spriteDesc.node = framework->getSceneManager()->createSceneNode(nodeDesc);
			TR::Physics::NovodexWheel *wheel = new TR::Physics::NovodexWheel(this, framework, vehicleDesc.frontWheelDesc);
			setWheel(TR::Entity::WP_FRONT_RIGHT, wheel);

			// FLWheel
			//if(mFrontTraction)
				vehicleDesc.frontWheelDesc.speedOverride = true;
			//else
				//vehicleDesc.wheelDesc.speedOverride = false;
			vehicleDesc.frontWheelDesc.spriteDesc.name = mSprite->getName() + "_FLWheel";
			vehicleDesc.frontWheelDesc.position.x = -vehicleDesc.frontWheelDesc.position.x;
			nodeDesc.name = mSprite->getName() + "_FLWheel";
			nodeDesc.position = vehicleDesc.frontWheelDesc.position;
			nodeDesc.orientation = vehicleDesc.frontWheelDesc.orientation;
			//nodeDesc.scale = vehicleDesc.wheelDesc.scale;
			nodeDesc.parent = mSprite->getNode();
			vehicleDesc.frontWheelDesc.spriteDesc.node = framework->getSceneManager()->createSceneNode(nodeDesc);
			wheel = new TR::Physics::NovodexWheel(this, framework, vehicleDesc.frontWheelDesc);
			setWheel(TR::Entity::WP_FRONT_LEFT, wheel);

			// RRWheel
			//if(mFrontTraction)
				//vehicleDesc.wheelDesc.speedOverride = false;
			//else
				vehicleDesc.rearWheelDesc.speedOverride = true;
			vehicleDesc.rearWheelDesc.spriteDesc.name = mSprite->getName() + "_RRWheel";
			nodeDesc.name = mSprite->getName() + "_RRWheel";
			nodeDesc.position = vehicleDesc.rearWheelDesc.position;
			nodeDesc.orientation = vehicleDesc.rearWheelDesc.orientation;
			//nodeDesc.scale = vehicleDesc.wheelDesc.scale;
			nodeDesc.parent = mSprite->getNode();
			vehicleDesc.rearWheelDesc.spriteDesc.node = framework->getSceneManager()->createSceneNode(nodeDesc);
			wheel = new TR::Physics::NovodexWheel(this, framework, vehicleDesc.rearWheelDesc);
			setWheel(TR::Entity::WP_REAR_RIGHT, wheel);

			// RLWheel
			//if(mFrontTraction)
				//vehicleDesc.wheelDesc.speedOverride = false;
			//else
				vehicleDesc.rearWheelDesc.speedOverride = true;
			vehicleDesc.rearWheelDesc.spriteDesc.name = mSprite->getName() + "_RLWheel";
			vehicleDesc.rearWheelDesc.position.x = -vehicleDesc.rearWheelDesc.position.x;
			//vehicleDesc.rearWheelDesc.position.z = -vehicleDesc.rearWheelDesc.position.z;
			nodeDesc.name = mSprite->getName() + "_RLWheel";
			nodeDesc.position = vehicleDesc.rearWheelDesc.position;
			nodeDesc.orientation = vehicleDesc.rearWheelDesc.orientation;
			//nodeDesc.scale = vehicleDesc.wheelDesc.scale;
			nodeDesc.parent = mSprite->getNode();
			vehicleDesc.rearWheelDesc.spriteDesc.node = framework->getSceneManager()->createSceneNode(nodeDesc);
			wheel = new TR::Physics::NovodexWheel(this, framework, vehicleDesc.rearWheelDesc);
			setWheel(TR::Entity::WP_REAR_LEFT, wheel);

			NxScene *scene = static_cast<TR::Physics::NovodexManager *>(framework->getPhysicsManager())->_getNxScene();
			NxShape *const *shapes = _getNxActor()->getShapes();
			scene->setShapePairFlags(*shapes[0], *shapes[1], NX_IGNORE_PAIR);
			scene->setShapePairFlags(*shapes[0], *shapes[2], NX_IGNORE_PAIR);
			scene->setShapePairFlags(*shapes[0], *shapes[3], NX_IGNORE_PAIR);
			scene->setShapePairFlags(*shapes[0], *shapes[4], NX_IGNORE_PAIR);
		}

		NovodexVehicle::~NovodexVehicle() 
		{ 
			delete mEngine;
			delete mGearBox; 
		}

		void NovodexVehicle::accelerate(TR::Real timeInSeconds)
		{
			float rpm;

			if(mGearBox->getCurrentGear() != 0)
			{
				if(getCurrentGear() == -1 && mCurrentSpeed > -2.0)
					mCurrentSpeed = -2.0;
				else if(getCurrentGear() > 0 && mCurrentSpeed < 2.0)
					mCurrentSpeed = 2.0;

				mAngularSpeed = mCurrentSpeed / mWheelRadius;

				if(getCurrentGear() == -1)
					rpm = mAngularSpeed * (-mGearBox->getCurrentGearRatio()) * (60.0 / (2 * TR::Math::PI));
				else
					rpm = mAngularSpeed * mGearBox->getCurrentGearRatio() * (60.0 / (2 * TR::Math::PI));
				mCurrentTorque = mEngine->getTorque(rpm);
				if(mEngine->getCurrentRpm() > mGearBox->getCurrentGearMaxRpm())
				{
					brake(timeInSeconds);
				}
				else 
				{
					if(mEngine->getCurrentRpm() < mGearBox->getCurrentGearMinRpm())
					{
						mCurrentRpm = mGearBox->getCurrentGearMinRpm();
						mCurrentTorque = (mCurrentTorque * (mEngine->getCurrentRpm() / mGearBox->getCurrentGearMinRpm())) * mGearBox->getCurrentGearRatio() * mEngine->getDifferentialRatio();
					}
					else
					{
						mCurrentRpm = mEngine->getCurrentRpm();
						mCurrentTorque = mCurrentTorque * mGearBox->getCurrentGearRatio() * mEngine->getDifferentialRatio();
					}
					TR::Real force = mCurrentTorque / mWheelRadius;
					TR::Real acceleration = force / mMass;
					mCurrentSpeed += acceleration * timeInSeconds;

					if(mEngineSoundId != -1)
					{
						TR::Real tempRpm;
						if(mCurrentRpm / mGearBox->getCurrentGearMaxRpm() > 0.98)
							tempRpm = mGearBox->getCurrentGearMaxRpm();
						else
							tempRpm = mCurrentRpm;

						mFramework->getAudioManager()->setSourcePosition(mEngineSoundId, getPosition());
						mFramework->getAudioManager()->playAudio(mEngineSoundId, false);
						TR::Real currentPitch = 1 + (((mCurrentRpm - mEngine->getMinRpm()) / (mEngine->getMaxRpm() - mEngine->getMinRpm())) * 0.5);
						mFramework->getAudioManager()->setSourcePitch(mEngineSoundId, currentPitch);
					}

					if(mFrontTraction)
					{
						mFLWheel->setAxleSpeed(mCurrentSpeed);
						mFRWheel->setAxleSpeed(mCurrentSpeed);
						mRLWheel->setAxleSpeed(mCurrentSpeed);
						mRRWheel->setAxleSpeed(mCurrentSpeed);
					}
					else
					{
						mRLWheel->setAxleSpeed(mCurrentSpeed);
						mRRWheel->setAxleSpeed(mCurrentSpeed);
					}
				}
			}
		}

		void NovodexVehicle::brake(TR::Real timeInSeconds)
		{
			//setBrakeTorque(100.0);
			TR::Real acceleration;
			if(getCurrentGear() == -1)
				acceleration = -mBrakeForce / mMass;
			else
				acceleration = mBrakeForce / mMass;

			mCurrentSpeed += acceleration * timeInSeconds;

			if(getCurrentGear() == -1 && mCurrentSpeed > 0.0)
				mCurrentSpeed = 0.0;
			else if(getCurrentGear() > -1 && mCurrentSpeed < 0.0)
				mCurrentSpeed = 0.0;

			mAngularSpeed = mCurrentSpeed / mWheelRadius;
			mCurrentRpm = mAngularSpeed * mGearBox->getCurrentGearRatio() * (60.0 / (2 * TR::Math::PI));

			if(mEngineSoundId != -1)
			{
				mFramework->getAudioManager()->playAudio(mEngineSoundId, false);
				//TR::Real currentPitch = 1 + (((mCurrentRpm - mGearBox->getCurrentGearMinRpm()) / (mGearBox->getCurrentGearMaxRpm() - mGearBox->getCurrentGearMinRpm())) * (mGearBox->getCurrentGear() / mGearBox->getNumberOfGears()) * 0.5);
				TR::Real currentPitch = 1 + (((mCurrentRpm - mEngine->getMinRpm()) / (mEngine->getMaxRpm() - mEngine->getMinRpm())) * 0.5);
				mFramework->getAudioManager()->setSourcePitch(mEngineSoundId, currentPitch);
			}

			if(mFrontTraction)
			{
				mFLWheel->setAxleSpeed(mCurrentSpeed);
				mFRWheel->setAxleSpeed(mCurrentSpeed);
				mRLWheel->setAxleSpeed(mCurrentSpeed);
				mRRWheel->setAxleSpeed(mCurrentSpeed);
			}
			else
			{
				mFLWheel->setAxleSpeed(mCurrentSpeed);
				mFRWheel->setAxleSpeed(mCurrentSpeed);
				mRLWheel->setAxleSpeed(mCurrentSpeed);
				mRRWheel->setAxleSpeed(mCurrentSpeed);
			}
		}

		void NovodexVehicle::decelerate(TR::Real timeInSeconds)
		{
			if(mGearBox->getCurrentGear() == -1)
			{
				mCurrentSpeed *= 0.98;
				mCurrentRpm *= 0.98;
				if(mCurrentSpeed > -2.0) 
				{
					mCurrentSpeed = 0.0;
					mCurrentRpm = 0.0;
				}
			}
			else
			{
				if(mCurrentSpeed < 40.0)
				{
					mCurrentSpeed *= 0.96;
					mCurrentRpm *= 0.96;
				}
				else if(mCurrentSpeed >= 40.0 && mCurrentSpeed < 80.0)
				{
					mCurrentSpeed *= 0.98;
					mCurrentRpm *= 0.98;
				}
				else
				{
					mCurrentSpeed *= 0.995;
					mCurrentRpm *= 0.995;
				}

				if(mCurrentSpeed < 2.0) 
				{
					mCurrentSpeed = 0.0;
					mCurrentRpm = 0.0;
				}
			}
			/*mCurrentRpm *= 0.95 * timeInSeconds;
			mCurrentTorque = mEngine->getTorque(mCurrentRpm);
			TR::Real force = mCurrentTorque / mWheelRadius;
			TR::Real acceleration = -(force / mMass);
			mCurrentSpeed += acceleration * timeInSeconds; */

			if(mEngineSoundId != -1)
			{
				mFramework->getAudioManager()->playAudio(mEngineSoundId, false);
				//TR::Real currentPitch = 1 + (((mCurrentRpm - mGearBox->getCurrentGearMinRpm()) / (mGearBox->getCurrentGearMaxRpm() - mGearBox->getCurrentGearMinRpm())) * (mGearBox->getCurrentGear() / mGearBox->getNumberOfGears()) * 0.5);
				TR::Real currentPitch = 1 + (((mCurrentRpm - mEngine->getMinRpm()) / (mEngine->getMaxRpm() - mEngine->getMinRpm())) * 0.5);
				mFramework->getAudioManager()->setSourcePitch(mEngineSoundId, currentPitch);
			}

			if(mFrontTraction)
			{
				mFLWheel->setAxleSpeed(mCurrentSpeed);
				mFRWheel->setAxleSpeed(mCurrentSpeed);
				mRLWheel->setAxleSpeed(mCurrentSpeed);
				mRRWheel->setAxleSpeed(mCurrentSpeed);
			}
			else
			{
				mRLWheel->setAxleSpeed(mCurrentSpeed);
				mRRWheel->setAxleSpeed(mCurrentSpeed);
			}
		}

		TR::Real NovodexVehicle::getSpeed()
		{
			mCurrentSpeed = mFLWheel->getAxleSpeed();
			return mCurrentSpeed;
		}

		void NovodexVehicle::realignWheels()
		{
			if(mCurrentSpeed <= -2.0 || mCurrentSpeed >= 2.0)
			{
				mCurrentSteering = -(mTotalSteering * 0.3);
				mTotalSteering *= 0.7;
				if(mTotalSteering >= -0.02 && mTotalSteering <= 0.02) mTotalSteering = 0.0;

				mFLWheel->setSteeringAngle(mTotalSteering);
				mFRWheel->setSteeringAngle(mTotalSteering);
			}
			else
				mCurrentSteering = 0;
		}

		void NovodexVehicle::setBrakeTorque(TR::Real torque) 
		{ 
			if(mFrontTraction)
			{
				mFLWheel->setBrakeTorque(torque);
				mFRWheel->setBrakeTorque(torque);
				mRLWheel->setBrakeTorque(torque);
				mRRWheel->setBrakeTorque(torque);
			}
			else
			{
				mRLWheel->setBrakeTorque(torque);
				mRRWheel->setBrakeTorque(torque);
			}
		}

		void NovodexVehicle::setEngineSound(const std::string &soundFile)
		{
			mFramework->getAudioManager()->attachAudioToSource(soundFile, reinterpret_cast<unsigned int *>(&mEngineSoundId), true);
			mFramework->getAudioManager()->setSourcePosition(mEngineSoundId, getPosition());
		}

		void NovodexVehicle::setMotorTorque(TR::Real torque) 
		{ 
			if(mFrontTraction)
			{
				mFLWheel->setMotorTorque(torque);
				mFRWheel->setMotorTorque(torque);
				mRLWheel->setMotorTorque(torque);
				mRRWheel->setMotorTorque(torque);
			}
			else
			{
				mFLWheel->setMotorTorque(torque);
				mFRWheel->setMotorTorque(torque);
				mRLWheel->setMotorTorque(torque);
				mRRWheel->setMotorTorque(torque);
			} 
		}

		void NovodexVehicle::setOrientation(TR::Quaternion orientation)
		{
			NxQuat quat;
			quat.x = orientation.x;
			quat.y = orientation.y;
			quat.z = orientation.z;
			quat.w = orientation.w;
			static_cast<TR::Physics::NovodexSolid *>(mSolid)->_getNxActor()->setGlobalOrientationQuat(quat);
			mSprite->setOrientation(orientation);
		}

		void NovodexVehicle::setPosition(TR::Vector3 position)
		{
			NxVec3 vec;
			vec.x = position.x;
			vec.y = position.y;
			vec.z = position.z;
			static_cast<TR::Physics::NovodexSolid *>(mSolid)->_getNxActor()->setGlobalPosition(vec);
			mSprite->getNode()->getParent()->setPosition(position);
		}

		void NovodexVehicle::steer(TR::Physics::SteeringSide side, TR::Real timeInSeconds, bool lockValue) 
		{ 
			if(lockValue)
			{
				if(side == TR::Physics::STEER_LEFT)
				{
					mTotalSteering = mMaxSteering * timeInSeconds;
				}
				else
				{
					mTotalSteering = (-mMaxSteering * timeInSeconds);
				}

				if(TR::Math::Abs(mTotalSteering) > mMaxSteering)
				{
					if(side == TR::Physics::STEER_LEFT)
					{
						mTotalSteering = mMaxSteering;
					}
					else
					{
						mTotalSteering = -mMaxSteering;
					}
				}
			}
			else
			{
				if(side == TR::Physics::STEER_LEFT)
				{
					mCurrentSteering = mSteeringRatio * timeInSeconds;
					mTotalSteering += mSteeringRatio * timeInSeconds;
				}
				else
				{
					mCurrentSteering = -mSteeringRatio * timeInSeconds;
					mTotalSteering += (-mSteeringRatio * timeInSeconds);
				}

				if(TR::Math::Abs(mTotalSteering) > mMaxSteering)
				{
					if(side == TR::Physics::STEER_LEFT)
					{
						mCurrentSteering = mCurrentSteering - (mTotalSteering - mMaxSteering);
						mTotalSteering = mMaxSteering;
					}
					else
					{
						mCurrentSteering = mCurrentSteering - (mTotalSteering + mMaxSteering);
						mTotalSteering = -mMaxSteering;
					}
				}
			}

			mFLWheel->setSteeringAngle(mTotalSteering);
			mFRWheel->setSteeringAngle(mTotalSteering);
		}

		void NovodexVehicle::setWheel(TR::Entity::WheelPosition position, TR::Physics::Wheel *wheel)
		{
			switch(position)
			{
			case WP_FRONT_LEFT:
				mFLWheel = reinterpret_cast<TR::Physics::NovodexWheel *>(wheel);
				//if(!mFrontTraction)
					//mFLWheel->_getNxWheelShape()->setWheelFlags(NX_WF_AXLE_SPEED_OVERRIDE);
				break;
				
			case WP_FRONT_RIGHT:
				mFRWheel = reinterpret_cast<TR::Physics::NovodexWheel *>(wheel);
				//if(!mFrontTraction)
					//mFRWheel->_getNxWheelShape()->setWheelFlags(NX_WF_AXLE_SPEED_OVERRIDE);
				break;

			case WP_REAR_LEFT:
				mRLWheel = reinterpret_cast<TR::Physics::NovodexWheel *>(wheel);
				//if(mFrontTraction)
					//mRLWheel->_getNxWheelShape()->setWheelFlags(NX_WF_AXLE_SPEED_OVERRIDE);
				break;

			case WP_REAR_RIGHT:
				mRRWheel = reinterpret_cast<TR::Physics::NovodexWheel *>(wheel);
				//if(mFrontTraction)
					//mRRWheel->_getNxWheelShape()->setWheelFlags(NX_WF_AXLE_SPEED_OVERRIDE);
				break;

			default:
				break;
			}
		}

		void NovodexVehicle::update(TR::Real timeInSeconds)
		{
			TR::Vector3 position = mSolid->getPosition();
			TR::Quaternion orientation = mSolid->getOrientation();
			orientation.normalise();
			mSprite->getNode()->getParent()->setPosition(position);
			mSprite->setOrientation(orientation);

			if(mEngineSoundId != -1)
				mFramework->getAudioManager()->setSourcePosition(mEngineSoundId, getPosition());

			mFLWheel->update(timeInSeconds);
			mFRWheel->update(timeInSeconds);
			mRLWheel->update(timeInSeconds);
			mRRWheel->update(timeInSeconds);

			//TR::Degree deg = TR::Radian(mCurrentSteering);
			//mFramework->getGraphicsManager()->yawSceneNode(mFLWheel->_getSprite()->getNode()->getName(), deg, TR::Graphics::TS_PARENT); 
			//mFramework->getGraphicsManager()->yawSceneNode(mFRWheel->_getSprite()->getNode()->getName(), deg, TR::Graphics::TS_PARENT);

			//TR::Real tempAngle = mAngularSpeed / (2 * TR::Math::PI);
			//tempAngle -= (int)(tempAngle);
			//deg = TR::Radian(tempAngle * (2 * TR::Math::PI) * timeInSeconds);
			/*deg = TR::Radian(mAngularSpeed * timeInSeconds);
			mFramework->getGraphicsManager()->pitchSceneNode(mFLWheel->_getSprite()->getNode()->getName(), deg); 
			mFramework->getGraphicsManager()->pitchSceneNode(mFRWheel->_getSprite()->getNode()->getName(), deg);
			mFramework->getGraphicsManager()->pitchSceneNode(mRLWheel->_getSprite()->getNode()->getName(), deg); 
			mFramework->getGraphicsManager()->pitchSceneNode(mRRWheel->_getSprite()->getNode()->getName(), deg);*/

			//TR::Vector3 vx, vy, vz;
			//orientation.ToAxes(vx, vy, vz);
			//TR::Quaternion q(vx, TR::Vector3::UNIT_Y, TR::Vector3::UNIT_Z);
			//TR::Quaternion q(TR::Vector3::UNIT_X, vy, TR::Vector3::UNIT_Z);
			//TR::Quaternion q(TR::Vector3::UNIT_X, TR::Vector3::UNIT_Y, vz);
			//mSprite->getNode()->getParent()->setOrientation(q);
		}
	}
}