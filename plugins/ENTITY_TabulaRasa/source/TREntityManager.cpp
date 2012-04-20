#include "../include/TREntityManager.h"
#include "../../../include/EntityCharacter.h"
#include "../../../include/EntityDynamic.h"
#include "../../../include/EntityKinematic.h"
#include "../../../include/EntitySprite.h"
#include "../../../include/EntityStatic.h"
#include "../../../include/EntityVehicle.h"
#include "../../../include/PhysicsManager.h"
#include "../../../include/PhysicsSolid.h"
#include "../../../include/SceneManager.h"
#include "../../../include/SceneNode.h"
#include "../../../include/XmlDocument.h"
#include "../../../include/Utils.h"
#include "../../../include/XmlElement.h"

namespace TR
{
	namespace Entity
	{
		TRManager::~TRManager()
		{
			for (mSpritesIterator = mSprites.begin(); mSpritesIterator != mSprites.end(); mSpritesIterator++)
				delete mSpritesIterator->second;

			mSprites.clear();
		}

		TR::Entity::Character* TRManager::createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::Character *character = mFramework->getPhysicsManager()->createCharacter(spriteDesc, solidDesc);
			mCharacters[spriteDesc.name] = character;

			return character;
		}

		TR::Entity::Dynamic* TRManager::createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::Dynamic *dynamic = mFramework->getPhysicsManager()->createDynamicEntity(spriteDesc, solidDesc);
			mDynamics[spriteDesc.name] = dynamic;

			return dynamic;
		}

		TR::Entity::Kinematic* TRManager::createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::Kinematic *kinematic = mFramework->getPhysicsManager()->createKinematicEntity(spriteDesc, solidDesc);
			mKinematics[spriteDesc.name] = kinematic;

			return kinematic;
		}

		void TRManager::createPagedGeometry(const TR::Graphics::PagedGeometryDesc &pagedGeometryDesc)
		{
			TR::Scene::NodeDesc nodeDesc;
			TR::Scene::Node *node;
			std::stringstream nodeName;
			nodeDesc.name = "pagedGeometryNode";
			nodeDesc.position = pagedGeometryDesc.placementMeshPosition;
			nodeDesc.orientation = pagedGeometryDesc.placementMeshOrientation;
			nodeDesc.scale = TR::Vector3::UNIT_SCALE;
			nodeDesc.parent = mFramework->getSceneManager()->getRootSceneNode();
			node = mFramework->getSceneManager()->createSceneNode(nodeDesc);

			TR::Entity::SpriteDesc spriteDesc;
			spriteDesc.name = pagedGeometryDesc.name;
			spriteDesc.node = node;
			spriteDesc.mesh = pagedGeometryDesc.placementMesh;
			spriteDesc.orientation = pagedGeometryDesc.placementMeshOrientation;
			spriteDesc.position = pagedGeometryDesc.placementMeshPosition;

			TR::Physics::SolidDesc solidDesc;
			solidDesc.collisionType = TR::Physics::CT_TREECOLLISION;
			solidDesc.group = 1;
			solidDesc.materialName = "default";
			solidDesc.name = pagedGeometryDesc.name;
			solidDesc.orientation = pagedGeometryDesc.placementMeshOrientation;
			solidDesc.position = pagedGeometryDesc.placementMeshPosition;
			solidDesc.solidType = TR::Physics::ST_STATIC;
			//solidDesc.sprite = sprite;
			
			TR::Entity::Static *staticEntity = createStaticEntity(spriteDesc, solidDesc);
			TR::AxisAlignedBox AABB = staticEntity->getSprite()->getBoundingBox();
			std::vector<TR::Vector3> copiesPosition(pagedGeometryDesc.copies);

			for(int i = 0; i < pagedGeometryDesc.copies; i++)
			{
				TR::Real distance = 0.0;
				TR::Real height = AABB.getMaximum().y - AABB.getMinimum().y;
				TR::Real delta = height * 0.2;
				while(distance <= delta)
				{
					copiesPosition[i].x = TR::Math::RangeRandom(AABB.getMinimum().x, AABB.getMaximum().x);
					copiesPosition[i].z = TR::Math::RangeRandom(AABB.getMinimum().z, AABB.getMaximum().z);
					TR::Vector3 position(copiesPosition[i].x, AABB.getMaximum().y, copiesPosition[i].z);
					distance = mFramework->getPhysicsManager()->getDistanceFromClosestObject(position, TR::Vector3(0.0, -1.0, 0.0), (AABB.getMaximum().y - AABB.getMinimum().y) * 2.0);
					int x = 1;
				}
				copiesPosition[i].y = AABB.getMaximum().y - distance;
			}

			destroyStaticEntity(staticEntity);
			mFramework->getGraphicsManager()->destroySceneNode("pagedGeometryNode");

			mFramework->getGraphicsManager()->createPagedGeometry(const_cast<TR::Graphics::PagedGeometryDesc &>(pagedGeometryDesc), copiesPosition, AABB);
		}

		TR::Entity::Sprite* TRManager::createSprite(TR::Entity::SpriteDesc &spriteDesc)
		{
			TR::Entity::Sprite *sprite = mFramework->getGraphicsManager()->createSprite(spriteDesc);
			mSprites[spriteDesc.name] = sprite;

			return sprite;
		}

		TR::Entity::Static* TRManager::createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::Static *newStatic = mFramework->getPhysicsManager()->createStaticEntity(spriteDesc, solidDesc);
			mStatics[spriteDesc.name] = newStatic;

			return newStatic;
		}

		TR::Entity::Trigger* TRManager::createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::Trigger *trigger = mFramework->getPhysicsManager()->createTrigger(spriteDesc, solidDesc);
			mTriggers[spriteDesc.name] = trigger;

			return trigger;
		}

		TR::Entity::Vehicle* TRManager::createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc)
		{
			TR::Entity::Vehicle *vehicle = mFramework->getPhysicsManager()->createVehicle(spriteDesc, vehicleDesc);
			mVehicles[spriteDesc.name] = vehicle;

			return vehicle;
		}

		TR::Entity::Vehicle* TRManager::createVehicle(const std::string &fileSettings, TR::Scene::Node *parentNode)
		{
			TR::Xml::Document *xmlDoc = mFramework->getXmlManager()->createDocument("car", fileSettings);
			TR::Xml::Element *vehicle = xmlDoc->getRootElement();

			TR::Entity::SpriteDesc spriteDesc;
			TR::Entity::VehicleDesc vehicleDesc;

			assert(vehicle->getAttributeValue("name") != "" &&
					 vehicle->getAttributeValue("mesh") != "" &&
					 vehicle->getAttributeValue("density") != "" &&
					 vehicle->getAttributeValue("triggerFlag") != "");

			spriteDesc.name = vehicleDesc.name = vehicle->getAttributeValue("name");
			spriteDesc.mesh = vehicle->getAttributeValue("mesh");
			if(vehicle->getAttributeValue("shadowCaster") != "")
				spriteDesc.shadowCaster = TR::Utils::parseBool(vehicle->getAttributeValue("shadowCaster"));

			assert(mFramework->getPhysicsManager() != 0);

			vehicleDesc.solidType = TR::Physics::ST_DYNAMIC;

			vehicleDesc.density = TR::Utils::parseReal(vehicle->getAttributeValue("density"));

			std::string collisionType = vehicle->getAttributeValue("collisionType");
			if(collisionType == "box")
				vehicleDesc.collisionType = TR::Physics::CT_BOX;
			else if(collisionType == "capsule")
				vehicleDesc.collisionType = TR::Physics::CT_CAPSULE;
			else if(collisionType == "plane")
				vehicleDesc.collisionType = TR::Physics::CT_PLANE;
			else if(collisionType == "sphere")
				vehicleDesc.collisionType = TR::Physics::CT_SPHERE;
			else if(collisionType == "treeCollision")
				vehicleDesc.collisionType = TR::Physics::CT_TREECOLLISION;

			if(vehicle->getAttributeValue("group") != "")
				vehicleDesc.group = TR::Utils::parseReal(vehicle->getAttributeValue("group"));

			if(vehicle->getAttributeValue("material") != "")
				vehicleDesc.materialName = vehicle->getAttributeValue("material");

			if(vehicle->getAttributeValue("triggerFlag") != "")
				vehicleDesc.triggerFlag = TR::Utils::parseInt(vehicle->getAttributeValue("triggerFlag"));

			spriteDesc.node = parentNode;
			spriteDesc.orientation = vehicleDesc.orientation = parentNode->getOrientation();
			//spriteDesc.position = vehicleDesc.position = parentNode->getPosition();
			spriteDesc.position = parentNode->getPosition();
			vehicleDesc.position = parentNode->getParent()->getPosition();
			spriteDesc.scale = vehicleDesc.scale = parentNode->getScale();

			if(vehicle->getAttributeValue("frontTraction") != "")
				vehicleDesc.frontTraction = TR::Utils::parseBool(vehicle->getAttributeValue("frontTraction"));

			TR::Xml::Element *XmlChildElement = vehicle->getFirstChildByName("cMassOffset");
			if(XmlChildElement)
			{
				assert(XmlChildElement->getAttributeValue("x") != "" &&
						 XmlChildElement->getAttributeValue("y") != "" &&
						 XmlChildElement->getAttributeValue("z") != "");
				vehicleDesc.cMassOffset.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("x"));
				vehicleDesc.cMassOffset.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("y"));
				vehicleDesc.cMassOffset.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("z"));
			}

			if(vehicle->getAttributeValue("brakeForce") != "")
				vehicleDesc.brakeForce = TR::Utils::parseReal(vehicle->getAttributeValue("brakeForce"));

			if(vehicle->getAttributeValue("steeringRatio") != "")
				vehicleDesc.steeringRatio = TR::Utils::parseReal(vehicle->getAttributeValue("steeringRatio"));
			else
				vehicleDesc.steeringRatio = 0.785;

			if(vehicle->getAttributeValue("maxSteering") != "")
				vehicleDesc.maxSteering = TR::Utils::parseReal(vehicle->getAttributeValue("maxSteering"));

			TR::Xml::Element *XmlGrandChildElement;

			// Engine
			XmlChildElement = vehicle->getFirstChildByName("engine");
			if(XmlChildElement)
			{
				if(XmlChildElement->getAttributeValue("differentialRatio") != "")
					vehicleDesc.engineDesc.differentialRatio = TR::Utils::parseReal(XmlChildElement->getAttributeValue("differentialRatio"));
				else
					vehicleDesc.engineDesc.differentialRatio = 1.0;

				// Get the torque curve
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("torquePoint");
				while(XmlGrandChildElement)
				{
					TR::Real rpm = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("rpm"));
					TR::Real torque = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("torque"));
					TR::Physics::TorquePoint *tp = new TR::Physics::TorquePoint(rpm, torque);
					vehicleDesc.engineDesc.torqueCurve.push_back(tp);
					XmlGrandChildElement = XmlGrandChildElement->getNextSiblingByName("torquePoint");
				}
			}

			// GearBox
			std::list<TR::Real> gears;
			XmlChildElement = vehicle->getFirstChildByName("gearBox");
			if(XmlChildElement)
			{
				// Get the gears
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("gear");
				while(XmlGrandChildElement)
				{
					TR::Physics::Gear gear;
					gear.ratio = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("ratio"));
					gear.minRpm = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("minRpm"));
					gear.maxRpm = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("maxRpm"));
					vehicleDesc.gears.push_back(gear);
					XmlGrandChildElement = XmlGrandChildElement->getNextSiblingByName("gear");
				}
			}

			TR::Xml::Element *XmlGrandGrandChildElement;

			//Front axle
			XmlChildElement = vehicle->getFirstChildByName("frontAxle");
			if(XmlChildElement)
			{
				// Now position it...
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("position");
				if(XmlGrandChildElement)
				{
					assert(XmlGrandChildElement->getAttributeValue("x") != "" &&
							 XmlGrandChildElement->getAttributeValue("y") != "" &&
							 XmlGrandChildElement->getAttributeValue("z") != "");
					vehicleDesc.frontWheelDesc.position.x = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("x"));
					vehicleDesc.frontWheelDesc.position.y = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("y"));
					vehicleDesc.frontWheelDesc.position.z = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("z"));
				}

				// Rotate it...
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("orientation");
				if(XmlGrandChildElement)
				{
					assert(XmlGrandChildElement->getAttributeValue("qx") != "" &&
							 XmlGrandChildElement->getAttributeValue("qy") != "" &&
							 XmlGrandChildElement->getAttributeValue("qz") != "" &&
							 XmlGrandChildElement->getAttributeValue("qw") != "");
					vehicleDesc.frontWheelDesc.orientation.x = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qx"));
					vehicleDesc.frontWheelDesc.orientation.y = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qy"));
					vehicleDesc.frontWheelDesc.orientation.z = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qz"));
					vehicleDesc.frontWheelDesc.orientation.w = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qw"));
				}

				// Wheels
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("wheels");
				if(XmlGrandChildElement)
				{
					if(XmlGrandChildElement->getAttributeValue("radius") != "")
						vehicleDesc.frontWheelDesc.radius = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("radius"));

					if(XmlGrandChildElement->getAttributeValue("inverseWheelMass") != "")
						vehicleDesc.frontWheelDesc.inverseWheelMass = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("inverseWheelMass"));

					// Longitudinal Friction
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("longitudinalFriction");
					if(XmlGrandGrandChildElement)
					{
						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip") != "")
							vehicleDesc.frontWheelDesc.longitudinalFriction.asymptoteSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue") != "")
							vehicleDesc.frontWheelDesc.longitudinalFriction.asymptoteValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumSlip") != "")
							vehicleDesc.frontWheelDesc.longitudinalFriction.extremumSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumValue") != "")
							vehicleDesc.frontWheelDesc.longitudinalFriction.extremumValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor") != "")
							vehicleDesc.frontWheelDesc.longitudinalFriction.stiffnessFactor = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor"));
					}
					
					// Lateral Friction
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("lateralFriction");
					if(XmlGrandGrandChildElement)
					{
						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip") != "")
							vehicleDesc.frontWheelDesc.lateralFriction.asymptoteSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue") != "")
							vehicleDesc.frontWheelDesc.lateralFriction.asymptoteValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumSlip") != "")
							vehicleDesc.frontWheelDesc.lateralFriction.extremumSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumValue") != "")
							vehicleDesc.frontWheelDesc.lateralFriction.extremumValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor") != "")
							vehicleDesc.frontWheelDesc.lateralFriction.stiffnessFactor = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor"));
					}

					// Suspension
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("suspension");
					if(XmlGrandGrandChildElement)
					{
						vehicleDesc.frontWheelDesc.suspensionDamper = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("damper"));
						vehicleDesc.frontWheelDesc.suspensionSpring = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("spring"));
						vehicleDesc.frontWheelDesc.suspensionTargetValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("targetValue"));
						vehicleDesc.frontWheelDesc.suspensionTravel = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("travel"));
					}

					// Sprite
					vehicleDesc.frontWheelDesc.spriteDesc.type = TR::Entity::OT_SPRITE;

					// The wheel name will be filled by the vehicle
					vehicleDesc.frontWheelDesc.spriteDesc.name = "";

					assert(XmlGrandChildElement->getAttributeValue("mesh") != "");
					vehicleDesc.frontWheelDesc.spriteDesc.mesh = XmlGrandChildElement->getAttributeValue("mesh");
					vehicleDesc.frontWheelDesc.spriteDesc.shadowCaster = spriteDesc.shadowCaster;

					vehicleDesc.frontWheelDesc.spriteDesc.node = parentNode;

					if(XmlGrandChildElement->getAttributeValue("visible") != "")
						vehicleDesc.frontWheelDesc.spriteDesc.isVisible = TR::Utils::parseBool(XmlGrandChildElement->getAttributeValue("visible"));
					else
						vehicleDesc.frontWheelDesc.spriteDesc.isVisible = true;
				}
			}

			//Rear axle
			XmlChildElement = vehicle->getFirstChildByName("rearAxle");
			if(XmlChildElement)
			{
				// Now position it...
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("position");
				if(XmlGrandChildElement)
				{
					assert(XmlGrandChildElement->getAttributeValue("x") != "" &&
							 XmlGrandChildElement->getAttributeValue("y") != "" &&
							 XmlGrandChildElement->getAttributeValue("z") != "");
					vehicleDesc.rearWheelDesc.position.x = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("x"));
					vehicleDesc.rearWheelDesc.position.y = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("y"));
					vehicleDesc.rearWheelDesc.position.z = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("z"));
				}

				// Rotate it...
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("orientation");
				if(XmlGrandChildElement)
				{
					assert(XmlGrandChildElement->getAttributeValue("qx") != "" &&
							 XmlGrandChildElement->getAttributeValue("qy") != "" &&
							 XmlGrandChildElement->getAttributeValue("qz") != "" &&
							 XmlGrandChildElement->getAttributeValue("qw") != "");
					vehicleDesc.rearWheelDesc.orientation.x = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qx"));
					vehicleDesc.rearWheelDesc.orientation.y = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qy"));
					vehicleDesc.rearWheelDesc.orientation.z = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qz"));
					vehicleDesc.rearWheelDesc.orientation.w = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("qw"));
				}

				// Wheels
				XmlGrandChildElement = XmlChildElement->getFirstChildByName("wheels");
				if(XmlGrandChildElement)
				{
					if(XmlGrandChildElement->getAttributeValue("radius") != "")
						vehicleDesc.rearWheelDesc.radius = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("radius"));

					if(XmlGrandChildElement->getAttributeValue("inverseWheelMass") != "")
						vehicleDesc.rearWheelDesc.inverseWheelMass = TR::Utils::parseReal(XmlGrandChildElement->getAttributeValue("inverseWheelMass"));

					// Longitudinal Friction
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("longitudinalFriction");
					if(XmlGrandGrandChildElement)
					{
						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip") != "")
							vehicleDesc.rearWheelDesc.longitudinalFriction.asymptoteSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue") != "")
							vehicleDesc.rearWheelDesc.longitudinalFriction.asymptoteValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumSlip") != "")
							vehicleDesc.rearWheelDesc.longitudinalFriction.extremumSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumValue") != "")
							vehicleDesc.rearWheelDesc.longitudinalFriction.extremumValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor") != "")
							vehicleDesc.rearWheelDesc.longitudinalFriction.stiffnessFactor = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor"));
					}
					
					// Lateral Friction
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("lateralFriction");
					if(XmlGrandGrandChildElement)
					{
						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip") != "")
							vehicleDesc.rearWheelDesc.lateralFriction.asymptoteSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue") != "")
							vehicleDesc.rearWheelDesc.lateralFriction.asymptoteValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("asymptoteValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumSlip") != "")
							vehicleDesc.rearWheelDesc.lateralFriction.extremumSlip = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumSlip"));

						if(XmlGrandGrandChildElement->getAttributeValue("extremumValue") != "")
							vehicleDesc.rearWheelDesc.lateralFriction.extremumValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("extremumValue"));

						if(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor") != "")
							vehicleDesc.rearWheelDesc.lateralFriction.stiffnessFactor = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("stiffnessFactor"));
					}

					// Suspension
					XmlGrandGrandChildElement = XmlGrandChildElement->getFirstChildByName("suspension");
					if(XmlGrandGrandChildElement)
					{
						vehicleDesc.rearWheelDesc.suspensionDamper = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("damper"));
						vehicleDesc.rearWheelDesc.suspensionSpring = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("spring"));
						vehicleDesc.rearWheelDesc.suspensionTargetValue = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("targetValue"));
						vehicleDesc.rearWheelDesc.suspensionTravel = TR::Utils::parseReal(XmlGrandGrandChildElement->getAttributeValue("travel"));
					}

					// Sprite
					vehicleDesc.rearWheelDesc.spriteDesc.type = TR::Entity::OT_SPRITE;

					// The wheel name will be filled by the vehicle
					vehicleDesc.rearWheelDesc.spriteDesc.name = "";

					assert(XmlGrandChildElement->getAttributeValue("mesh") != "");
					vehicleDesc.rearWheelDesc.spriteDesc.mesh = XmlGrandChildElement->getAttributeValue("mesh");
					vehicleDesc.rearWheelDesc.spriteDesc.shadowCaster = spriteDesc.shadowCaster;

					vehicleDesc.rearWheelDesc.spriteDesc.node = parentNode;

					if(XmlGrandChildElement->getAttributeValue("visible") != "")
						vehicleDesc.rearWheelDesc.spriteDesc.isVisible = TR::Utils::parseBool(XmlGrandChildElement->getAttributeValue("visible"));
					else
						vehicleDesc.rearWheelDesc.spriteDesc.isVisible = true;
				}
			}
			
			TR::Entity::Vehicle *tempVehicle = createVehicle(spriteDesc, vehicleDesc);

			std::list<TR::Physics::TorquePoint *>::iterator it = vehicleDesc.engineDesc.torqueCurve.begin();
			for( ; it != vehicleDesc.engineDesc.torqueCurve.end(); it++)
				delete (*it);

			mFramework->getXmlManager()->destroyDocument("car");

			return tempVehicle;
		}

		void TRManager::destroyDynamicEntity(const std::string &name)
		{
			mFramework->getPhysicsManager()->destroyDynamicEntity(mDynamics[name]);
			mDynamics.erase(name);
			mSprites.erase(name);
		}

		void TRManager::destroyDynamicEntity(TR::Entity::Dynamic *dynamic)
		{
			std::string name = dynamic->getName();
			mFramework->getPhysicsManager()->destroyDynamicEntity(dynamic);
			mDynamics.erase(name);
			mSprites.erase(name);
		}

		void TRManager::destroyKinematicEntity(TR::Entity::Kinematic *kinematic)
		{
			std::string name = kinematic->getName();
			mFramework->getPhysicsManager()->destroyKinematicEntity(kinematic);
			mKinematics.erase(name);
			mSprites.erase(name);
		}

		void TRManager::destroyStaticEntity(TR::Entity::Static *staticEntity)
		{
			std::string name = staticEntity->getName();
			mFramework->getPhysicsManager()->destroyStaticEntity(staticEntity);
			mStatics.erase(name);
			mSprites.erase(name);
		}

		TR::Entity::Character* TRManager::getCharacter(const std::string &name)
		{
			mCharactersIterator = mCharacters.find(name);
			if(mCharactersIterator != mCharacters.end())
				return mCharactersIterator->second;
			else
				return 0;
		}

		TR::Entity::Character* TRManager::getCharacterById(TR::uint32 id)
		{
			mCharactersIterator = mCharacters.begin();
			while(mCharactersIterator != mCharacters.end())
			{
				if(mCharactersIterator->second->getId() == id)
					return mCharactersIterator->second;
				mCharactersIterator++;
			}			
			return 0;
		}

		TR::Entity::Dynamic* TRManager::getDynamicEntity(const std::string &name)
		{
			mDynamicsIterator = mDynamics.find(name);
			if(mDynamicsIterator != mDynamics.end())
				return mDynamicsIterator->second;
			else
				return 0;
		}

		TR::Entity::Kinematic* TRManager::getKinematicEntity(const std::string &name)
		{
			mKinematicsIterator = mKinematics.find(name);
			if(mKinematicsIterator != mKinematics.end())
				return mKinematicsIterator->second;
			else
				return 0;
		}

		TR::Graphics::MeshInfo& TRManager::getMesh(const std::string entityName)
		{
			TR::Graphics::MeshInfo &meshInfo = mFramework->getGraphicsManager()->getMeshInfo(entityName);
			return meshInfo;
		}

		TR::Entity::Sprite* TRManager::getSprite(const std::string &name)
		{
			mSpritesIterator = mSprites.find(name);
			if(mSpritesIterator != mSprites.end())
				return mSpritesIterator->second;
			else
				return 0;
		}

		TR::Entity::Static* TRManager::getStaticEntity(const std::string &name)
		{
			mStaticsIterator = mStatics.find(name);
			if(mStaticsIterator != mStatics.end())
				return mStaticsIterator->second;
			else
				return 0;
		}

		TR::Entity::Vehicle* TRManager::getVehicle(const std::string &name)
		{
			mVehiclesIterator = mVehicles.find(name);
			if(mVehiclesIterator != mVehicles.end())
				return mVehiclesIterator->second;
			else
				return 0;
		}

		void TRManager::updateEntities(unsigned long timeSinceLastFrame)
		{
			assert(mFramework->getSceneManager() != 0);

			TR::Real newTime = timeSinceLastFrame / 1000.0f;

			// Update the entities.
			mCharactersIterator = mCharacters.begin();
			while(mCharactersIterator != mCharacters.end())
			{
				mCharactersIterator->second->update(newTime);			
				mCharactersIterator++;
			}

			mDynamicsIterator = mDynamics.begin();
			while(mDynamicsIterator != mDynamics.end())
			{
				mDynamicsIterator->second->update(newTime);			
				mDynamicsIterator++;
			}

			mKinematicsIterator = mKinematics.begin();
			while(mKinematicsIterator != mKinematics.end())
			{
				mKinematicsIterator->second->update(newTime);			
				mKinematicsIterator++;
			}

			mSpritesIterator = mSprites.begin();
			while(mSpritesIterator != mSprites.end())
			{
				mSpritesIterator->second->updateAnimation(newTime);			
				mSpritesIterator++;
			}

			mTriggersIterator = mTriggers.begin();
			while(mTriggersIterator != mTriggers.end())
			{
				mTriggersIterator->second->update(newTime);			
				mTriggersIterator++;
			}

			mVehiclesIterator = mVehicles.begin();
			while(mVehiclesIterator != mVehicles.end())
			{
				mVehiclesIterator->second->update(newTime);			
				mVehiclesIterator++;
			}
		}
	}
}