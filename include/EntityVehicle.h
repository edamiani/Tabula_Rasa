#ifndef __TR_ENTITY_VEHICLE__
#define __TR_ENTITY_VEHICLE__

#include "Dependencies.h"
#include "PhysicsSolid.h"
#include "PhysicsWheel.h"

namespace TR
{
	namespace Physics
	{
		enum SteeringSide
		{
			STEER_LEFT = -1,
			NO_STEERING = 0,
			STEER_RIGHT = 1
		};

		struct Gear
		{
			TR::Real		ratio;
			TR::Real		maxRpm;
			TR::Real		minRpm;

			Gear()
			{
				ratio = 1.0;
				maxRpm = 0.0;
				minRpm = 0.0;
			}
		};

		class GearBox
		{
		public:
							GearBox();

			void			gearDown();			
			void			gearUp();
			int			getCurrentGear();
			TR::Real		getCurrentGearRatio();
			TR::Real		getCurrentGearMaxRpm();
			TR::Real		getCurrentGearMinRpm();
			int			getNumberOfGears();
			void			insertGear(TR::Physics::Gear gear);
			void			setReverseGear(TR::Physics::Gear gear);
			void			_setCurrentGear(int gear);

		protected:
			int												mCurrentGear;
			std::list<TR::Physics::Gear>				mGears;
			std::list<TR::Physics::Gear>::iterator mGearsIterator;
			int												mNumberOfGears;
		};

		struct TorquePoint
		{
			TorquePoint(TR::Real r, TR::Real t) { rpm = r; torque = t; }

			TR::Real	ratio;
			TR::Real	rpm;
			TR::Real	torque;
		};

		struct EngineDesc
		{
			TR::Real											differentialRatio;
			std::list<TR::Physics::TorquePoint *>	torqueCurve;
		};

		class Engine
		{
		public:
						Engine(TR::Physics::EngineDesc &engineDesc, TR::Physics::GearBox *gearBox);
						~Engine();

			TR::Real getCurrentRpm();			
			TR::Real getDifferentialRatio();
			TR::Real getMaxRpm();
			TR::Real getMinRpm();
			TR::Real getTorque(TR::Real rpm);
			void		insertTorquePoint(TR::Real rpm, TR::Real torque);

		protected:
			TR::Real											mCurrentRpm;
			TR::Real											mDifferentialRatio;
			TR::Physics::GearBox							*mGearBox;
			std::list<TR::Physics::TorquePoint *>	mTorqueCurve;
		};
	}

	namespace Entity
	{
		enum WheelPosition
		{
			WP_FRONT_LEFT,
			WP_FRONT_RIGHT,
			WP_REAR_LEFT,
			WP_REAR_RIGHT
		};

		struct VehicleDesc: public TR::Physics::SolidDesc
		{
			TR::Real								angularSpeed;
			TR::Real								brakeForce;
			TR::Vector3							cMassOffset;
			TR::Physics::EngineDesc			engineDesc;
			bool									frontTraction;
			std::list<TR::Physics::Gear>	gears;
			TR::Real								steeringRatio;
			TR::Real								maxSteering;
			TR::Physics::WheelDesc			frontWheelDesc;
			TR::Physics::WheelDesc			rearWheelDesc;

			VehicleDesc()
			{
				cMassOffset = TR::Vector3::ZERO;
				frontTraction = true;
				maxSteering = 0.785;
			}
		};

		class Vehicle
		{
		public:
															Vehicle() {  }
			virtual										~Vehicle() {  }

			virtual void								accelerate(TR::Real timeInSeconds) = 0;
			virtual void								brake(TR::Real timeInSeconds) = 0;
			virtual void								decelerate(TR::Real timeInSeconds) = 0;
			virtual void								gearUp() = 0;
			virtual void								gearDown() = 0;
			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual int									getCurrentGear() = 0;
			virtual TR::Real							getCurrentRpm() = 0;
			virtual TR::Real							getCurrentTorque() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Scene::Node*				getNode() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Real							getSpeed() = 0;
			virtual TR::Real							getSteeringAngle() = 0;
			virtual TR::Real							getSteeringStep() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual void								moveGlobalPosition(const TR::Vector3 &position) = 0;
			virtual void								realignWheels() = 0;
			virtual void								setAngularVelocity(const TR::Vector3 &velocity) = 0;
			virtual void								setBrakeTorque(TR::Real torque) = 0;
			virtual void								setCurrentGear(int gear) = 0;
			virtual void								setEngineSound(const std::string &soundFile) = 0;
			virtual void								setLinearVelocity(const TR::Vector3 &velocity) = 0;
			virtual void								setMotorTorque(TR::Real torque) = 0;
			virtual void								setOrientation(TR::Quaternion orientation) = 0;
			virtual void								setPosition(TR::Vector3 position) = 0;
			virtual void								setSpeed(TR::Real speed) = 0;
			virtual void								setSteeringAngle(TR::Real angle) = 0;
			virtual void								steer(TR::Physics::SteeringSide side, TR::Real timeInSeconds, bool lockValue = true) = 0;
			virtual void								setVisible(bool isVisible) = 0;
			virtual void								setWheel(TR::Entity::WheelPosition position, TR::Physics::Wheel *wheel) = 0;
			virtual void								update(TR::Real timeInSeconds) = 0;
		};
	}
}

#endif