#ifndef __TR_PHYSICS_WHEEL__
#define __TR_PHYSICS_WHEEL__

#include "Dependencies.h"
#include "EntitySprite.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace TR
{
	namespace Physics
	{
		struct Friction
		{
			TR::Real asymptoteSlip;
			TR::Real asymptoteValue;
			TR::Real extremumSlip;
			TR::Real extremumValue;
			TR::Real stiffnessFactor;

			Friction() : asymptoteSlip(2.0), asymptoteValue(0.01), extremumSlip(1.0), extremumValue(0.02), stiffnessFactor(1000000) {}
		};

		struct WheelDesc
		{
			TR::Real						inverseWheelMass;
			TR::Physics::Friction	lateralFriction;
			TR::Physics::Friction	longitudinalFriction;
			TR::Quaternion				orientation;
			TR::Vector3					position;
			TR::Real						radius;
			bool							speedOverride;
			TR::Entity::SpriteDesc	spriteDesc;
			TR::Real						suspensionDamper;
			TR::Real						suspensionSpring;
			TR::Real						suspensionTargetValue;
			TR::Real						suspensionTravel;

			WheelDesc()
			{
				inverseWheelMass = 1.0f;
				orientation = TR::Quaternion::IDENTITY;
				position = TR::Vector3::ZERO;
				radius = 1.0f;
				speedOverride = false;
				suspensionDamper = 0.0f;
				suspensionSpring = 0.0f;
				suspensionTargetValue = 0.0f;
				suspensionTravel = 1.0f;
			}
		};

		class Wheel
		{
		public:
			Wheel() {  }
			virtual ~Wheel() {  }

			virtual TR::Real		getAxleSpeed() = 0;
			virtual TR::Real		getMotorTorque() = 0;
			virtual TR::Real		getRadius() = 0;
			virtual void			setAxleSpeed(TR::Real speed) = 0;
			virtual void			setBrakeTorque(TR::Real torque) = 0;
			virtual void			setMotorTorque(TR::Real torque) = 0;
			virtual void			setSteeringAngle(TR::Real angle) = 0;
			virtual void			update(TR::Real timeInSeconds) = 0;
		};
	}
}

#endif