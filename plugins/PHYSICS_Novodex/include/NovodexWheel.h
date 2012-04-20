#ifndef __TR_NOVODEX_WHEEL__
#define __TR_NOVODEX_WHEEL__

#include "../../../include/Dependencies.h"
#include "../../../include/EntityManager.h"
#include "../../../include/PhysicsWheel.h"
#include "../../../include/Framework.h"
#include "../../../include/PhysicsSolid.h"
#include "NovodexManager.h"

namespace TR
{
	namespace Entity
	{
		class NovodexVehicle;
	}
}

namespace TR
{
	namespace Physics
	{
		class NovodexWheel : public TR::Physics::Wheel
		{
		public:
			NovodexWheel(TR::Entity::Vehicle *vehicle, TR::Framework *framework, TR::Physics::WheelDesc &wheelDesc);
			virtual ~NovodexWheel() {  }

			TR::Real			getAxleSpeed() { return mWheelShape->getAxleSpeed(); }
			TR::Real			getMotorTorque() { return mWheelShape->getMotorTorque(); }
			TR::Real			getRadius() { return mWheelShape->getRadius(); }
			void				setAxleSpeed(TR::Real speed) { mWheelShape->setAxleSpeed(speed); }
			void				setBrakeTorque(TR::Real torque) { mWheelShape->setBrakeTorque(torque); }
			void				setMotorTorque(TR::Real torque) { mWheelShape->setMotorTorque(torque); }
			void				setSteeringAngle(TR::Real angle) { mWheelShape->setSteerAngle(angle); }
			void				update(TR::Real timeInSeconds);

			TR::Entity::Sprite* _getSprite() { return mSprite; }
			NxWheelShape*	_getNxWheelShape() { return mWheelShape; }

		private:
			TR::Real								mCurrentRollingAngle;
			TR::Entity::NovodexVehicle		*mParentVehicle;
			TR::Entity::Sprite				*mSprite;
			NxWheelShape						*mWheelShape;
		};
	}
}

#endif