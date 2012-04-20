#include "../include/NovodexWheel.h"
#include "../include/NovodexVehicle.h"

namespace TR
{
	namespace Physics
	{
		NovodexWheel::NovodexWheel(TR::Entity::Vehicle *vehicle, TR::Framework *framework, TR::Physics::WheelDesc &wheelDesc)
		{ 
			mParentVehicle = reinterpret_cast<TR::Entity::NovodexVehicle *>(vehicle);
			mCurrentRollingAngle = 0.0;

			NxWheelShapeDesc wheelShapeDesc;
			NxVec3 direction(wheelDesc.orientation.x, wheelDesc.orientation.y, wheelDesc.orientation.z);
			NxQuat q(direction, wheelDesc.orientation.w);
			NxVec3 p(wheelDesc.position.x, wheelDesc.position.y, wheelDesc.position.z);

			wheelShapeDesc.inverseWheelMass = wheelDesc.inverseWheelMass;

			wheelShapeDesc.lateralTireForceFunction.extremumSlip = wheelDesc.lateralFriction.extremumSlip;
			wheelShapeDesc.lateralTireForceFunction.extremumValue = wheelDesc.lateralFriction.extremumValue;
			wheelShapeDesc.lateralTireForceFunction.asymptoteSlip = wheelDesc.lateralFriction.asymptoteSlip;
			wheelShapeDesc.lateralTireForceFunction.asymptoteValue = wheelDesc.lateralFriction.asymptoteValue;
			wheelShapeDesc.lateralTireForceFunction.stiffnessFactor = wheelDesc.lateralFriction.stiffnessFactor;

			wheelShapeDesc.longitudalTireForceFunction.extremumSlip = wheelDesc.longitudinalFriction.extremumSlip;
			wheelShapeDesc.longitudalTireForceFunction.extremumValue = wheelDesc.longitudinalFriction.extremumValue;
			wheelShapeDesc.longitudalTireForceFunction.asymptoteSlip = wheelDesc.longitudinalFriction.asymptoteSlip;
			wheelShapeDesc.longitudalTireForceFunction.asymptoteValue = wheelDesc.longitudinalFriction.asymptoteValue;
			wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor = wheelDesc.longitudinalFriction.stiffnessFactor;
			
			wheelShapeDesc.localPose.M.fromQuat(q);
			wheelShapeDesc.localPose.t = p;
			wheelShapeDesc.radius = wheelDesc.radius;
			wheelShapeDesc.suspension.damper = wheelDesc.suspensionDamper;
			wheelShapeDesc.suspension.spring = wheelDesc.suspensionSpring;
			wheelShapeDesc.suspension.targetValue = wheelDesc.suspensionTargetValue;
			wheelShapeDesc.suspensionTravel = wheelDesc.suspensionTravel;
			if(wheelDesc.speedOverride)
				wheelShapeDesc.wheelFlags |= NX_WF_AXLE_SPEED_OVERRIDE;

			wheelShapeDesc.materialIndex = reinterpret_cast<TR::Physics::NovodexManager*>(mParentVehicle->_getFramework()->getPhysicsManager())->_getTireMaterial()->getMaterialIndex();

			mWheelShape = static_cast<NxWheelShape *>(reinterpret_cast<TR::Entity::NovodexVehicle *>(vehicle)->_getNxActor()->createShape(wheelShapeDesc));
			mSprite = framework->getEntityManager()->createSprite(wheelDesc.spriteDesc);
		}

		void NovodexWheel::update(TR::Real timeInSeconds)
		{
			TR::Vector3 position;
			TR::Quaternion orientation;

			NxReal rollAngle = mCurrentRollingAngle;
			rollAngle += mWheelShape->getAxleSpeed() * timeInSeconds;
			while (rollAngle > NxTwoPi)	//normally just 1x
				rollAngle -= NxTwoPi;
			while (rollAngle < -NxTwoPi)	//normally just 1x
				rollAngle += NxTwoPi;

			// We have the roll angle for the wheel now
			mCurrentRollingAngle = rollAngle;

			NxMat34 pose;
			//pose = mWheelShape->getLocalPose();
			pose = mWheelShape->getLocalPose();

			NxWheelContactData wheelContactData;
			NxShape* shape = mWheelShape->getContact(wheelContactData);

			NxReal radius = mWheelShape->getRadius();
			NxReal suspensionTravel = mWheelShape->getSuspensionTravel();
			NxReal steerAngle = mWheelShape->getSteerAngle();

			NxVec3 p0;
			NxVec3 dir;

			p0 = pose.t;  //cast from shape origin
			pose.M.getColumn(1, dir);
			dir = -dir;	//cast along -Y.

			if (shape && wheelContactData.contactForce > -1000)
			{
				pose.t.y = wheelContactData.contactPoint.y + pose.t.y;
				pose.t.y -= mWheelShape->getGlobalPose().t.y + (dir.y * radius);	//go from contact pos to center pos.
			}
			else
			{
				pose.t.y = p0.y + dir.y * suspensionTravel;
			}

			NxMat33 rot, axisRot;
			rot.rotY(steerAngle);
			axisRot.rotY(0);

			NxMat33 rollRot;
			rollRot.rotX(rollAngle);

			pose.M = rot * pose.M * axisRot * rollRot;

			NxQuat q;
			pose.M.toQuat(q);

			orientation.x = q.x;
			orientation.y = q.y;
			orientation.z = q.z;
			orientation.w = q.w;

			position.x = pose.t.x;
			position.y = pose.t.y;
			position.z = pose.t.z;

			/*if(mParentVehicle->getPosition().x >= 0)
				position.x = pose.t.x - mParentVehicle->getPosition().x;
			else
				position.x = pose.t.x + mParentVehicle->getPosition().x;

			if(mParentVehicle->getPosition().y >= 0)
				position.y = pose.t.y - mParentVehicle->getPosition().y;
			else
				position.y = pose.t.y + mParentVehicle->getPosition().y;

			if(mParentVehicle->getPosition().z >= 0)
				position.z = pose.t.z - mParentVehicle->getPosition().z;
			else
				position.z = pose.t.z + mParentVehicle->getPosition().z;*/

			mSprite->setPosition(position);
			mSprite->setOrientation(orientation);

			/*position.x = mWheelShape->getLocalPosition().x;
			position.y = mWheelShape->getLocalPosition().y - mWheelShape->getSuspensionTravel();
			position.z = mWheelShape->getLocalPosition().z;*/
			
			/*NxQuat wheelOrientation;
			mWheelShape->getLocalOrientation().toQuat(wheelOrientation);
			//orientation.normalise();
			orientation.x = wheelOrientation.x;
			orientation.y = wheelOrientation.y;
			orientation.z = wheelOrientation.z;
			orientation.w = wheelOrientation.w;*/
		}
	}
}