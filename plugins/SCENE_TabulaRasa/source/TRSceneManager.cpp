#include "../../../include/EntitySprite.h"
#include "../../../include/EntityVehicle.h"
#include "../../../include/Framework.h"
#include "../../../include/GraphicsColorValue.h"
#include "../../../include/GraphicsManager.h"
#include "../../../include/PhysicsManager.h"
#include "../../../include/PhysicsMaterial.h"
#include "../../../include/PhysicsSolid.h"
#include "../../../include/PhysicsWheel.h"
#include "../../../include/SceneLight.h"
#include "../../../include/SceneManager.h"
#include "../../../include/SceneNode.h"
#include "../../../include/SceneCameraStatic.h"
#include "../../../include/Utils.h"
#include "../../../include/XmlDocument.h"
#include "../../../include/XmlElement.h"

#include "../include/TRSceneManager.h"

namespace TR
{
	namespace Scene
	{
		TRManager::~TRManager()
		{
			for (mSceneNodesIterator = mSceneNodes.begin(); mSceneNodesIterator != mSceneNodes.end(); mSceneNodesIterator++)
				delete mSceneNodesIterator->second;
			mSceneNodes.clear();

			for (mCamerasIterator = mCameras.begin(); mCamerasIterator != mCameras.end(); mCamerasIterator++)
				delete mCamerasIterator->second;
			mCameras.clear();
		}

		TR::Scene::Node* TRManager::createSceneNode(const TR::Scene::NodeDesc &nodeDesc)
		{
			TR::Scene::Node* node = mFramework->getGraphicsManager()->createSceneNode(nodeDesc);
			mSceneNodes[nodeDesc.name] = node;

			return node;
		}

		void TRManager::destroySceneNode(const std::string &nodeName)
		{
			mFramework->getGraphicsManager()->destroySceneNode(nodeName);
			mSceneNodes.erase(nodeName);
		}

		TR::Scene::Camera* TRManager::getCamera(const std::string &name)
		{
			mCamerasIterator = mCameras.find(name);
			if(mCamerasIterator != mCameras.end())
				return mCamerasIterator->second;
			else
				return 0;
		}

		TR::Scene::Node* TRManager::getSceneNode(const std::string &nodeName)
		{
			mSceneNodesIterator = mSceneNodes.find(nodeName);
				if(mSceneNodesIterator != mSceneNodes.end())
				return mSceneNodesIterator->second;
			else
				return 0;
		}

		void TRManager::loadScene(TR::Xml::Document *scene)
		{
			std::string sceneType = scene->getRootElement()->getAttributeValue("type");
			assert(sceneType != "");
			if(sceneType == "default")
				mFramework->getGraphicsManager()->setSceneManager(TR::Scene::ST_DEFAULT);
			else if(sceneType == "octree")
				mFramework->getGraphicsManager()->setSceneManager(TR::Scene::ST_OCTREE);
			else if(sceneType == "bsp")
				mFramework->getGraphicsManager()->setSceneManager(TR::Scene::ST_BSP);
			else if(sceneType == "terrain")
				mFramework->getGraphicsManager()->setSceneManager(TR::Scene::ST_TERRAIN);

			TR::Xml::Element *XmlRoot = scene->getRootElement();
			_parseNodeTree(XmlRoot);

			// Water Plane
			TR::Xml::Element *XmlElement = XmlRoot->getFirstChildByName("waterPlane");
			if(XmlElement)
			{
				TR::Graphics::WaterPlaneDesc waterPlaneDesc;

				waterPlaneDesc.cameraName = XmlElement->getAttributeValue("camera");
				waterPlaneDesc.position.x = TR::Utils::parseReal(XmlElement->getAttributeValue("x"));
				waterPlaneDesc.position.y = TR::Utils::parseReal(XmlElement->getAttributeValue("y"));
				waterPlaneDesc.position.z = TR::Utils::parseReal(XmlElement->getAttributeValue("z"));
				waterPlaneDesc.width = waterPlaneDesc.position.x = TR::Utils::parseReal(XmlElement->getAttributeValue("width"));
				waterPlaneDesc.depth = waterPlaneDesc.position.x = TR::Utils::parseReal(XmlElement->getAttributeValue("depth"));
				waterPlaneDesc.uTiles = waterPlaneDesc.position.x = TR::Utils::parseInt(XmlElement->getAttributeValue("uTiles"));
				waterPlaneDesc.vTiles = waterPlaneDesc.position.x = TR::Utils::parseInt(XmlElement->getAttributeValue("vTiles"));
				waterPlaneDesc.xSegments = waterPlaneDesc.position.x = TR::Utils::parseInt(XmlElement->getAttributeValue("xSegments"));
				waterPlaneDesc.ySegments = waterPlaneDesc.position.x = TR::Utils::parseInt(XmlElement->getAttributeValue("ySegments"));

				mFramework->getGraphicsManager()->createWaterPlane(waterPlaneDesc);
			}
		}

		void TRManager::_loadCamera(TR::Xml::Element *camera, TR::Scene::Node *parentNode)
		{
			std::string mode;
			assert(camera->getAttributeValue("mode") != "");
			mode = camera->getAttributeValue("mode");

			if(mode == "static")
			{
				TR::Scene::CameraStaticDesc cameraDesc;
				cameraDesc.mode = TR::Scene::CM_STATIC;
				cameraDesc.parentNode = parentNode;

				cameraDesc.name = camera->getAttributeValue("name");
				assert(cameraDesc.name != "");

				if(camera->getAttributeValue("active") != "")
					cameraDesc.isActive = TR::Utils::parseBool(camera->getAttributeValue("active"));
				else
					cameraDesc.isActive = false;

				cameraDesc.nearClipDistance = 0.1;
				cameraDesc.farClipDistance = 100.0;
				TR::Xml::Element *XmlClipping = camera->getFirstChildByName("clipping");
				if(XmlClipping)
				{
					if(XmlClipping->getAttributeValue("near") != "")
						cameraDesc.nearClipDistance = TR::Utils::parseReal(XmlClipping->getAttributeValue("near"));

					if(XmlClipping->getAttributeValue("far") != "")
						cameraDesc.farClipDistance = TR::Utils::parseReal(XmlClipping->getAttributeValue("far"));
				}

				TR::Xml::Element *XmlPosition = camera->getFirstChildByName("position");
				if(XmlPosition)
				{
					assert(XmlPosition->getAttributeValue("x") != "" &&
							 XmlPosition->getAttributeValue("y") != "" &&
							 XmlPosition->getAttributeValue("z") != "");
					cameraDesc.position.x = TR::Utils::parseReal(XmlPosition->getAttributeValue("x"));
					cameraDesc.position.y = TR::Utils::parseReal(XmlPosition->getAttributeValue("y"));
					cameraDesc.position.z = TR::Utils::parseReal(XmlPosition->getAttributeValue("z"));
				}

				TR::Xml::Element *XmlDirection = camera->getFirstChildByName("direction");
				if(XmlDirection)
				{
					assert(XmlDirection->getAttributeValue("x") != "" &&
							 XmlDirection->getAttributeValue("y") != "" &&
							 XmlDirection->getAttributeValue("z") != "");
					cameraDesc.direction.x = TR::Utils::parseReal(XmlDirection->getAttributeValue("x"));
					cameraDesc.direction.y = TR::Utils::parseReal(XmlDirection->getAttributeValue("y"));
					cameraDesc.direction.z = TR::Utils::parseReal(XmlDirection->getAttributeValue("z"));
				}
				cameraDesc.direction.normalise();

				assert(camera->getFirstChildByName("viewport") != 0);
				TR::Xml::Element *XmlViewport = camera->getFirstChildByName("viewport");
				if(XmlViewport)
				{
					assert(XmlViewport->getAttributeValue("zOrder") != "" &&
							 XmlViewport->getAttributeValue("left") != "" &&
							 XmlViewport->getAttributeValue("top") != "" &&
							 XmlViewport->getAttributeValue("width") != "" &&
							 XmlViewport->getAttributeValue("height") != "");

					cameraDesc.viewport.zOrder = TR::Utils::parseInt(XmlViewport->getAttributeValue("zOrder"));
					cameraDesc.viewport.left = TR::Utils::parseReal(XmlViewport->getAttributeValue("left"));
					cameraDesc.viewport.top = TR::Utils::parseReal(XmlViewport->getAttributeValue("top"));
					cameraDesc.viewport.width = TR::Utils::parseReal(XmlViewport->getAttributeValue("width"));
					cameraDesc.viewport.height = TR::Utils::parseReal(XmlViewport->getAttributeValue("height"));
				}

				// Now, create the camera
				mCameras[cameraDesc.name] = mFramework->getGraphicsManager()->createCamera(cameraDesc);
			}
		}

		void TRManager::_loadEntity(TR::Xml::Element *entity, TR::Scene::Node *parent)
		{
			TR::Entity::SpriteDesc spriteDesc;
			TR::Physics::SolidDesc solidDesc;

			assert(entity->getAttributeValue("name") != "" &&
					 entity->getAttributeValue("mesh") != "" &&
					 entity->getAttributeValue("density") != "" &&
					 entity->getAttributeValue("material") != "");

			spriteDesc.name = solidDesc.name = entity->getAttributeValue("name");
			spriteDesc.mesh = entity->getAttributeValue("mesh");
			if(entity->getAttributeValue("shadowCaster") != "")
				spriteDesc.shadowCaster = TR::Utils::parseBool(entity->getAttributeValue("shadowCaster"));

			assert(mFramework->getPhysicsManager() != 0);

			if(entity->getAttributeValue("solidType") == "static")
				solidDesc.solidType = TR::Physics::ST_STATIC;
			else if(entity->getAttributeValue("solidType") == "character")
				solidDesc.solidType = TR::Physics::ST_CHARACTER;
			else if(entity->getAttributeValue("solidType") == "kinematic")
				solidDesc.solidType = TR::Physics::ST_KINEMATIC;
			else if(entity->getAttributeValue("solidType") == "dynamic")
				solidDesc.solidType = TR::Physics::ST_DYNAMIC;

			if(entity->getAttributeValue("isTrigger") != "")
				solidDesc.isTrigger = TR::Utils::parseBool(entity->getAttributeValue("isTrigger"));
			else
				solidDesc.isTrigger = false;

			if(entity->getAttributeValue("ccd") != "")
				solidDesc.ccd = TR::Utils::parseBool(entity->getAttributeValue("ccd"));
			else
				solidDesc.ccd = false;

			if(entity->getAttributeValue("mass") != "")
				solidDesc.mass = TR::Utils::parseReal(entity->getAttributeValue("mass"));
			else
				solidDesc.mass = 0.0;

			if(entity->getAttributeValue("visible") != "")
				spriteDesc.isVisible = TR::Utils::parseBool(entity->getAttributeValue("visible"));
			else
			{
				if(!solidDesc.isTrigger)
					spriteDesc.isVisible = true;
				else
					spriteDesc.isVisible = false;
			}

			if(entity->getAttributeValue("density") != "")
				solidDesc.density = TR::Utils::parseReal(entity->getAttributeValue("density"));
			else
				solidDesc.density = 0;

			std::string collisionType = entity->getAttributeValue("collisionType");
			if(collisionType == "box")
				solidDesc.collisionType = TR::Physics::CT_BOX;
			else if(collisionType == "capsule")
				solidDesc.collisionType = TR::Physics::CT_CAPSULE;
			else if(collisionType == "plane")
				solidDesc.collisionType = TR::Physics::CT_PLANE;
			else if(collisionType == "sphere")
				solidDesc.collisionType = TR::Physics::CT_SPHERE;
			else if(collisionType == "treeCollision")
				solidDesc.collisionType = TR::Physics::CT_TREECOLLISION;

			if(entity->getAttributeValue("group") != "")
				solidDesc.group = TR::Utils::parseReal(entity->getAttributeValue("group"));

			if(entity->getAttributeValue("material") != "")
				solidDesc.materialName = entity->getAttributeValue("material");

			if(entity->getAttributeValue("triggerFlag") != "")
				solidDesc.triggerFlag = TR::Utils::parseInt(entity->getAttributeValue("triggerFlag"));

			if(entity->getAttributeValue("id") != "")
				solidDesc.triggerFlag = TR::Utils::parseInt(entity->getAttributeValue("id"));

			spriteDesc.node = parent;
			spriteDesc.orientation = solidDesc.orientation = parent->getOrientation();
			spriteDesc.position = solidDesc.position = parent->getPosition();
			spriteDesc.scale = solidDesc.scale = parent->getScale();

			// Is there an initial animation?
			if(entity->getAttributeValue("initialAnimation") != "")
				spriteDesc.initialAnimation = entity->getAttributeValue("initialAnimation");
			
			switch(solidDesc.solidType)
			{
			case TR::Physics::ST_CHARACTER:
				mFramework->getEntityManager()->createCharacter(spriteDesc, solidDesc);
				break;

			case TR::Physics::ST_DYNAMIC:
				mFramework->getEntityManager()->createDynamicEntity(spriteDesc, solidDesc);
				break;
			
			case TR::Physics::ST_KINEMATIC:
				mFramework->getEntityManager()->createKinematicEntity(spriteDesc, solidDesc);
				break;
			
			case TR::Physics::ST_STATIC:
				mFramework->getEntityManager()->createStaticEntity(spriteDesc, solidDesc);
				break;
			}
		}

		void TRManager::_loadEnvironment(TR::Xml::Element *environment)
		{
			TR::Xml::Element *XmlElement;

			XmlElement = environment->getFirstChildByName("ambientLight");
			if(XmlElement)
			{
				TR::Graphics::ColorValue ambientColor = TR::Graphics::ColorValue::Black;
				ambientColor.r = TR::Utils::parseReal(XmlElement->getAttributeValue("r"));
				ambientColor.g = TR::Utils::parseReal(XmlElement->getAttributeValue("g"));
				ambientColor.b = TR::Utils::parseReal(XmlElement->getAttributeValue("b"));

				mFramework->getGraphicsManager()->setAmbientLight(ambientColor);
			}

			XmlElement = environment->getFirstChildByName("shadowColor");
			if(XmlElement)
			{
				TR::Graphics::ColorValue shadowColor = TR::Graphics::ColorValue::Black;
				shadowColor.r = TR::Utils::parseReal(XmlElement->getAttributeValue("r"));
				shadowColor.g = TR::Utils::parseReal(XmlElement->getAttributeValue("g"));
				shadowColor.b = TR::Utils::parseReal(XmlElement->getAttributeValue("b"));

				mFramework->getGraphicsManager()->setShadowColor(shadowColor);
			}

			XmlElement = environment->getFirstChildByName("fog");
			if(XmlElement)
			{
				TR::Graphics::FogMode fogMode = TR::Graphics::FOG_NONE;
				TR::Graphics::ColorValue fogColor = TR::Graphics::ColorValue::White;
				TR::Real expDensity = 0.0001;
				TR::Real linearStart = 0.0;
				TR::Real linearEnd = 1.0;

				fogColor.r = TR::Utils::parseReal(XmlElement->getAttributeValue("r"));
				fogColor.g = TR::Utils::parseReal(XmlElement->getAttributeValue("g"));
				fogColor.b = TR::Utils::parseReal(XmlElement->getAttributeValue("b"));
				if(XmlElement->getAttributeValue("mode") != "")
				{
					if(XmlElement->getAttributeValue("mode") == "none")
						fogMode = TR::Graphics::FOG_NONE;
					else if(XmlElement->getAttributeValue("mode") == "linear")
						fogMode = TR::Graphics::FOG_LINEAR;
					else if(XmlElement->getAttributeValue("mode") == "exponential")
						fogMode = TR::Graphics::FOG_EXP;
					else if(XmlElement->getAttributeValue("mode") == "exponentialSquared")
						fogMode = TR::Graphics::FOG_EXP2;
				}
				if(XmlElement->getAttributeValue("expDensity") != "")
					expDensity = TR::Utils::parseReal(XmlElement->getAttributeValue("expDensity"));
				if(XmlElement->getAttributeValue("linearStart") != "")
					expDensity = TR::Utils::parseReal(XmlElement->getAttributeValue("linearStart"));
				if(XmlElement->getAttributeValue("linearEnd") != "")
					expDensity = TR::Utils::parseReal(XmlElement->getAttributeValue("linearEnd"));

				mFramework->getGraphicsManager()->setFog(fogColor, fogMode, expDensity, linearStart, linearEnd);
			}

			XmlElement = environment->getFirstChildByName("skyBox");
			if(XmlElement)
			{
				assert(XmlElement->getAttributeValue("material") != "");				
				mFramework->getGraphicsManager()->setSkyBox(XmlElement->getAttributeValue("material"));
			}
		}

		void TRManager::_loadGlobalElements(TR::Xml::Element *scene)
		{
			TR::Xml::Element *XmlElement;

			//Loads resources
			XmlElement = scene->getFirstChildByName("resources");
			assert(XmlElement != 0);
			assert(XmlElement->getAttributeValue("file") != "");
			mFramework->getGraphicsManager()->loadResources(XmlElement->getAttributeValue("file"));
			
			//Loads Sky Box
			XmlElement = scene->getFirstChildByName("environment");
			if(XmlElement)
			{
				_loadEnvironment(XmlElement);
			}

			// Check for cameras
			XmlElement = scene->getFirstChildByName("camera");
			while(XmlElement)
			{
				_loadCamera(XmlElement, 0);
				XmlElement = XmlElement->getNextSiblingByName("camera");
			}

			// Check for lights
			XmlElement = scene->getFirstChildByName("light");
			while(XmlElement)
			{
				_loadLight(XmlElement, 0);
				XmlElement = XmlElement->getNextSiblingByName("light");
			}
		}

		void TRManager::_loadJoint(TR::Xml::Element *joint)
		{
			TR::Physics::JointDesc jointDesc;
			std::string childName("");
			std::string parentName("");
			TR::Vector3 anchor;
			TR::Vector3 axes[3];

			if(joint->getAttributeValue("child") != "")
				childName = joint->getAttributeValue("child");

			if(joint->getAttributeValue("parent") != "")
				parentName = joint->getAttributeValue("parent");

			TR::Xml::Element* XMLChild = joint->getFirstChildByName("axis0");
			if(XMLChild)
			{
				if(XMLChild->getAttributeValue("x") != "")
					axes[0].x = TR::Utils::parseReal(XMLChild->getAttributeValue("x"));
				if(XMLChild->getAttributeValue("y") != "")
					axes[0].y = TR::Utils::parseReal(XMLChild->getAttributeValue("y"));
				if(XMLChild->getAttributeValue("z") != "")
					axes[0].z = TR::Utils::parseReal(XMLChild->getAttributeValue("z"));

				TR::Xml::Element* XMLLimits = XMLChild->getFirstChildByName("limits");
				if(XMLLimits)
				{
					jointDesc.rotationLimits[0].enabled = true;

					TR::Xml::Element* XMLOffset = XMLLimits->getFirstChildByName("offset");
					if(XMLOffset)
					{
						if(XMLOffset->getAttributeValue("x") != "")
							jointDesc.rotationLimits[0].offset.x = TR::Utils::parseReal(XMLOffset->getAttributeValue("x"));
						if(XMLOffset->getAttributeValue("y") != "")
							jointDesc.rotationLimits[0].offset.y = TR::Utils::parseReal(XMLOffset->getAttributeValue("y"));
						if(XMLOffset->getAttributeValue("z") != "")
							jointDesc.rotationLimits[0].offset.z = TR::Utils::parseReal(XMLOffset->getAttributeValue("z"));
					}

					TR::Xml::Element* XMLLimitValue = XMLLimits->getFirstChildByName("low");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[0].low.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[0].low.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[0].low.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}

					XMLLimitValue = XMLLimits->getFirstChildByName("high");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[0].high.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[0].high.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[0].high.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}
				}
			}

			XMLChild = joint->getFirstChildByName("axis1");
			if(XMLChild)
			{
				if(XMLChild->getAttributeValue("x") != "")
					axes[1].x = TR::Utils::parseReal(XMLChild->getAttributeValue("x"));
				if(XMLChild->getAttributeValue("y") != "")
					axes[1].y = TR::Utils::parseReal(XMLChild->getAttributeValue("y"));
				if(XMLChild->getAttributeValue("z") != "")
					axes[1].z = TR::Utils::parseReal(XMLChild->getAttributeValue("z"));
				
				TR::Xml::Element* XMLLimits = XMLChild->getFirstChildByName("limits");
				if(XMLLimits)
				{
					jointDesc.rotationLimits[1].enabled = true;

					TR::Xml::Element* XMLLimitValue = XMLLimits->getFirstChildByName("low");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[1].low.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[1].low.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[1].low.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}

					XMLLimitValue = XMLLimits->getFirstChildByName("high");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[1].high.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[1].high.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[1].high.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}
				}
			}

			XMLChild = joint->getFirstChildByName("axis2");
			if(XMLChild)
			{
				if(XMLChild->getAttributeValue("x") != "")
					axes[2].x = TR::Utils::parseReal(XMLChild->getAttributeValue("x"));
				if(XMLChild->getAttributeValue("y") != "")
					axes[2].y = TR::Utils::parseReal(XMLChild->getAttributeValue("y"));
				if(XMLChild->getAttributeValue("z") != "")
					axes[2].z = TR::Utils::parseReal(XMLChild->getAttributeValue("z"));
				
				TR::Xml::Element* XMLLimits = XMLChild->getFirstChildByName("limits");
				if(XMLLimits)
				{
					jointDesc.rotationLimits[2].enabled = true;

					TR::Xml::Element* XMLLimitValue = XMLLimits->getFirstChildByName("low");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[2].low.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[2].low.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[2].low.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}

					XMLLimitValue = XMLLimits->getFirstChildByName("high");
					if(XMLLimitValue)
					{
						if(XMLLimitValue->getAttributeValue("x") != "")
							jointDesc.rotationLimits[2].high.plane.x = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("x"));
						if(XMLLimitValue->getAttributeValue("y") != "")
							jointDesc.rotationLimits[2].high.plane.y = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("y"));
						if(XMLLimitValue->getAttributeValue("z") != "")
							jointDesc.rotationLimits[2].high.plane.z = TR::Utils::parseReal(XMLLimitValue->getAttributeValue("z"));
					}
				}
			}

			XMLChild = joint->getFirstChildByName("anchor");
			if(XMLChild)
			{
				if(XMLChild->getAttributeValue("x") != "")
					anchor.x = TR::Utils::parseReal(XMLChild->getAttributeValue("x"));
				if(XMLChild->getAttributeValue("y") != "")
					anchor.y = TR::Utils::parseReal(XMLChild->getAttributeValue("y"));
				if(XMLChild->getAttributeValue("z") != "")
					anchor.z = TR::Utils::parseReal(XMLChild->getAttributeValue("z"));

				if(jointDesc.rotationLimits[0].enabled)
				{
					jointDesc.rotationLimits[0].low.position = anchor;
					jointDesc.rotationLimits[0].high.position = anchor;
				}
				if(jointDesc.rotationLimits[1].enabled)
				{
					jointDesc.rotationLimits[1].low.position = anchor;
					jointDesc.rotationLimits[1].high.position = anchor;
				}
				if(jointDesc.rotationLimits[2].enabled)
				{
					jointDesc.rotationLimits[2].low.position = anchor;
					jointDesc.rotationLimits[2].high.position = anchor;
				}

				TR::Xml::Element* XMLLimits = XMLChild->getFirstChildByName("limits");
				if(XMLLimits)
				{
					jointDesc.translationLimits.enabled = true;

					if(XMLLimits->getAttributeValue("low") != "")
					{
						jointDesc.translationLimits.low.plane = axes[0];
						TR::Real low = TR::Utils::parseReal(XMLLimits->getAttributeValue("low"));
						jointDesc.translationLimits.low.position = anchor + low * axes[0];
					}
					if(XMLLimits->getAttributeValue("high") != "")
					{
						jointDesc.translationLimits.high.plane = -axes[0];
						TR::Real high = TR::Utils::parseReal(XMLLimits->getAttributeValue("high"));
						jointDesc.translationLimits.high.position = anchor + high * axes[0];
					}
				}
			}

			if(joint->getAttributeValue("type") != "")
			{
				jointDesc.child = childName;
				jointDesc.parent = parentName;

				std::string type(joint->getAttributeValue("type"));
				if(type == "ballAndSocket")
				{
					//jointDesc->jointType = TR::Physics::JT_BALLANDSOCKET;
					//Anima::PhysicsManager::getSingleton().createJoint(jointType, childName, parentName, axes, anchor);
				}
				else if(type == "fixed")
				{
					jointDesc.jointType = TR::Physics::JT_FIXED;
					mFramework->getPhysicsManager()->createJoint(jointDesc);
				}
				else if(type == "hinge")
				{
					jointDesc.jointType = TR::Physics::JT_HINGE;
					jointDesc.anchor = anchor;
					jointDesc.axes[0] = axes[0];
					mFramework->getPhysicsManager()->createJoint(jointDesc);
				}
				else if(type == "slider")
				{
					jointDesc.jointType = TR::Physics::JT_SLIDER;
					jointDesc.anchor = anchor;
					jointDesc.axes[0] = axes[0];
					mFramework->getPhysicsManager()->createJoint(jointDesc);
				}
				else if(type == "universal")
				{
					//jointDesc->jointType = TR::Physics::JT_UNIVERSAL;
					//Anima::PhysicsManager::getSingleton().createJoint(jointType, childName, parentName, axes, anchor);
				}
				else if(type == "wheel")
				{
					//jointDesc->jointType = TR::Physics::JT_WHEEL;
					//Anima::PhysicsManager::getSingleton().createJoint(jointType, childName, parentName, axes, anchor);
				}
			}
		}

		void TRManager::_loadLight(TR::Xml::Element *light, TR::Scene::Node *parentNode)
		{
			TR::Xml::Element *XmlElement;
			TR::Scene::LightDesc lightDesc;

			assert(light->getAttributeValue("name") != "");
			lightDesc.name = light->getAttributeValue("name");

			if(parentNode)
				lightDesc.node = parentNode;

			// Create a light (point | directional | spot)
			if(light->getAttributeValue("type") == "point")
				lightDesc.type = TR::Scene::LT_POINT;
			else if(light->getAttributeValue("type") == "directional")
				lightDesc.type = TR::Scene::LT_DIRECTIONAL;
			else if(light->getAttributeValue("type") == "spot")
				lightDesc.type = TR::Scene::LT_SPOT;

			XmlElement = light->getFirstChildByName("diffuse");
			if(XmlElement)
			{
				lightDesc.diffuse.r = TR::Utils::parseReal(XmlElement->getAttributeValue("r"));
				lightDesc.diffuse.g = TR::Utils::parseReal(XmlElement->getAttributeValue("g"));
				lightDesc.diffuse.b = TR::Utils::parseReal(XmlElement->getAttributeValue("b"));
				lightDesc.diffuse.a = 1;
			}

			XmlElement = light->getFirstChildByName("specular");
			if(XmlElement)
			{
				lightDesc.specular.r = TR::Utils::parseReal(XmlElement->getAttributeValue("r"));
				lightDesc.specular.g = TR::Utils::parseReal(XmlElement->getAttributeValue("g"));
				lightDesc.specular.b = TR::Utils::parseReal(XmlElement->getAttributeValue("b"));
				lightDesc.specular.a = 1;
			}

			XmlElement = light->getFirstChildByName("attenuation");
			if(XmlElement)
			{
				if(XmlElement->getAttributeValue("range") != "")
					lightDesc.attenuationRange = TR::Utils::parseReal(XmlElement->getAttributeValue("range"));
				if(XmlElement->getAttributeValue("constant") != "")
					lightDesc.attenuationConstant = TR::Utils::parseReal(XmlElement->getAttributeValue("constant"));
				if(XmlElement->getAttributeValue("linear") != "")
					lightDesc.attenuationLinear = TR::Utils::parseReal( XmlElement->getAttributeValue("linear") );
				if(XmlElement->getAttributeValue("quadratic") != "")
					lightDesc.attenuationQuadratic = TR::Utils::parseReal(XmlElement->getAttributeValue("quadratic"));
			}

			XmlElement = light->getFirstChildByName("position");
			if(XmlElement)
			{
				if(XmlElement->getAttributeValue("x") != "")
					lightDesc.position.x = TR::Utils::parseReal(XmlElement->getAttributeValue("x"));
				if(XmlElement->getAttributeValue("y") != "")
					lightDesc.position.y = TR::Utils::parseReal(XmlElement->getAttributeValue("y"));
				if(XmlElement->getAttributeValue("z") != "")
					lightDesc.position.z = TR::Utils::parseReal(XmlElement->getAttributeValue("z"));
			}

			XmlElement = light->getFirstChildByName("direction");
			if(XmlElement)
			{
				if(XmlElement->getAttributeValue("x") != "")
					lightDesc.direction.x = TR::Utils::parseReal(XmlElement->getAttributeValue("x"));
				if(XmlElement->getAttributeValue("y") != "")
					lightDesc.direction.y = TR::Utils::parseReal(XmlElement->getAttributeValue("y"));
				if(XmlElement->getAttributeValue("z") != "")
					lightDesc.direction.z = TR::Utils::parseReal(XmlElement->getAttributeValue("z"));
			}

			//visible			(true | false) "true"
			if(light->getAttributeValue("visible") != "")
				if(light->getAttributeValue("visible") == "false")
					lightDesc.visible = false;

			//castShadows		(true | false) "true"
			if(lightDesc.visible)
			{
				if(lightDesc.visible == true)
				{
					if(light->getAttributeValue("shadowCaster") != "" && light->getAttributeValue("shadowCaster") == "false")
						lightDesc.shadowCaster = false;
				}
				else
					lightDesc.shadowCaster = false;
			}
			else
				if(light->getAttributeValue("shadowCaster") != "" && light->getAttributeValue("shadowCaster") == "false")
					lightDesc.shadowCaster = false;

			mFramework->getGraphicsManager()->createLight(lightDesc);
		}
		
		void TRManager::_loadMaterial(TR::Xml::Element *material)
		{
			TR::Physics::MaterialDesc matDesc;
			matDesc.name = material->getAttributeValue("name");

			if(material->getAttributeValue("staticFriction") != "")
				matDesc.staticFriction = TR::Utils::parseReal(material->getAttributeValue("staticFriction"));
			else
				matDesc.staticFriction = 0.0;

			if(material->getAttributeValue("dynamicFriction") != "")
				matDesc.dynamicFriction = TR::Utils::parseReal(material->getAttributeValue("dynamicFriction"));
			else
				matDesc.dynamicFriction = 0.0;

			if(material->getAttributeValue("restitution") != "")
				matDesc.restitution = TR::Utils::parseReal(material->getAttributeValue("restitution"));
			else
				matDesc.restitution = 0.0;

			mFramework->getPhysicsManager()->createMaterial(matDesc);
		}

		void TRManager::_loadPagedGeometry(TR::Xml::Element *pagedGeometry)
		{
			TR::Xml::Element *XmlElement, *XmlChildElement;
			TR::Graphics::PagedGeometryDesc pagedGeometryDesc;

			pagedGeometryDesc.name = pagedGeometry->getAttributeValue("name");
			pagedGeometryDesc.mesh = pagedGeometry->getAttributeValue("mesh");
			pagedGeometryDesc.copies = TR::Utils::parseInt(pagedGeometry->getAttributeValue("copies"));
			pagedGeometryDesc.cameraName = pagedGeometry->getAttributeValue("cameraName");
			pagedGeometryDesc.pageSize = TR::Utils::parseReal(pagedGeometry->getAttributeValue("pageSize"));

			XmlElement = pagedGeometry->getFirstChildByName("detailLevel");
			while(XmlElement)
			{
				TR::Graphics::DetailLevel detailLevel;

				if(XmlElement->getAttributeValue("type") == "batch")
					detailLevel.type = TR::Graphics::DLT_BATCH;
				else if(XmlElement->getAttributeValue("type") == "impostor")
					detailLevel.type = TR::Graphics::DLT_IMPOSTOR;
				
				detailLevel.farDistance = TR::Utils::parseReal(XmlElement->getAttributeValue("farDistance"));
				detailLevel.fade = TR::Utils::parseReal(XmlElement->getAttributeValue("fade"));

				pagedGeometryDesc.detailLevels.push_back(detailLevel);
				XmlElement = XmlElement->getNextSiblingByName("detailLevel");
			}

			XmlElement = pagedGeometry->getFirstChildByName("placement");
			pagedGeometryDesc.placementMesh = XmlElement->getAttributeValue("mesh");

			XmlChildElement = XmlElement->getFirstChildByName("position");
			TR::Vector3 position;
			position.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("x"));
			position.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("y"));
			position.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("z"));
			pagedGeometryDesc.placementMeshPosition = position;

			XmlChildElement = XmlElement->getFirstChildByName("orientation");
			TR::Quaternion orientation;
			orientation.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qx"));
			orientation.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qy"));
			orientation.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qz"));
			orientation.w = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qw"));
			pagedGeometryDesc.placementMeshOrientation = orientation;

			mFramework->getEntityManager()->createPagedGeometry(pagedGeometryDesc);
		}
		
		TR::Scene::Node* TRManager::_loadSceneNode(TR::Xml::Element *node, TR::Scene::Node *parentNode)
		{
			TR::Scene::NodeDesc nodeDesc;
			TR::Xml::Element *XmlElement;

			// Set the parent node
			nodeDesc.parent = parentNode;

			// Process the current node
			// Grab the name of the node
			std::string nodeName = node->getAttributeValue("name");
			nodeDesc.name = nodeName;

			// Now position it...
			XmlElement = node->getFirstChildByName("position");
			nodeDesc.position = TR::Vector3::ZERO;
			if(XmlElement)
			{
				assert(XmlElement->getAttributeValue("x") != "" &&
						 XmlElement->getAttributeValue("y") != "" &&
						 XmlElement->getAttributeValue("z") != "");
				nodeDesc.position.x = TR::Utils::parseReal(XmlElement->getAttributeValue("x"));
				nodeDesc.position.y = TR::Utils::parseReal(XmlElement->getAttributeValue("y"));
				nodeDesc.position.z = TR::Utils::parseReal(XmlElement->getAttributeValue("z"));
			}

			// Rotate it...
			XmlElement = node->getFirstChildByName("orientation");
			nodeDesc.orientation = TR::Quaternion::IDENTITY;
			if(XmlElement)
			{
				assert(XmlElement->getAttributeValue("qx") != "" &&
						 XmlElement->getAttributeValue("qy") != "" &&
						 XmlElement->getAttributeValue("qz") != "" &&
						 XmlElement->getAttributeValue("qw") != "");
				nodeDesc.orientation.x = TR::Utils::parseReal(XmlElement->getAttributeValue("qx"));
				nodeDesc.orientation.y = TR::Utils::parseReal(XmlElement->getAttributeValue("qy"));
				nodeDesc.orientation.z = TR::Utils::parseReal(XmlElement->getAttributeValue("qz"));
				nodeDesc.orientation.w = TR::Utils::parseReal(XmlElement->getAttributeValue("qw"));
			}

			// Scale it.
			XmlElement = node->getFirstChildByName("scale");
			nodeDesc.scale = TR::Vector3::UNIT_SCALE;
			if(XmlElement)
			{
				assert(XmlElement->getAttributeValue("x") != "" &&
						 XmlElement->getAttributeValue("y") != "" &&
						 XmlElement->getAttributeValue("z") != "");
				nodeDesc.scale.x = TR::Utils::parseReal(XmlElement->getAttributeValue("x"));
				nodeDesc.scale.y = TR::Utils::parseReal(XmlElement->getAttributeValue("y"));
				nodeDesc.scale.z = TR::Utils::parseReal(XmlElement->getAttributeValue("z"));
			}

			// Now create the scene node
			TR::Scene::Node *sceneNode = createSceneNode(nodeDesc);
			mSceneNodes[nodeDesc.name] = sceneNode;

			// Check for Cameras
			XmlElement = node->getFirstChildByName("camera");
			while(XmlElement)
			{
				_loadCamera(XmlElement, sceneNode);
				XmlElement = XmlElement->getNextSiblingByName("camera");
			}

			// Check for Sprites
			XmlElement = node->getFirstChildByName("sprite");
			while(XmlElement)
			{
				_loadSprite(XmlElement, sceneNode);
				XmlElement = XmlElement->getNextSiblingByName("sprite");
			}

			// Check for lights
			XmlElement = node->getFirstChildByName("light");
			while(XmlElement)
			{
				_loadLight(XmlElement, sceneNode);
				XmlElement = XmlElement->getNextSiblingByName("light");
			}

			// Check for Entities
			XmlElement = node->getFirstChildByName("entity");
			while(XmlElement)
			{
				_loadEntity(XmlElement, sceneNode);
				XmlElement = XmlElement->getNextSiblingByName("entity");
			}

			// Check for Vehicle
			XmlElement = node->getFirstChildByName("vehicle");
			while(XmlElement)
			{
				_loadVehicle(XmlElement, sceneNode);
				XmlElement = XmlElement->getNextSiblingByName("entity");
			}

			return sceneNode;
		}

		void TRManager::_loadSprite(TR::Xml::Element *sprite, TR::Scene::Node *parentNode)
		{
			TR::Entity::SpriteDesc spriteDesc;
			spriteDesc.type = TR::Entity::OT_SPRITE;

			assert(sprite->getAttributeValue("name") != "");
			spriteDesc.name = sprite->getAttributeValue("name");

			spriteDesc.node = parentNode;

			assert(sprite->getAttributeValue("mesh") != "");
			spriteDesc.mesh = sprite->getAttributeValue("mesh");
			if(sprite->getAttributeValue("shadowCaster") != "")
				spriteDesc.shadowCaster = TR::Utils::parseBool(sprite->getAttributeValue("shadowCaster"));

			if(sprite->getAttributeValue("visible") != "")
				spriteDesc.isVisible = TR::Utils::parseBool(sprite->getAttributeValue("visible"));
			else
				spriteDesc.isVisible = true;

			TR::Xml::Element *XmlChildElement;

			// Now position it...
			XmlChildElement = sprite->getFirstChildByName("position");
			spriteDesc.position = TR::Vector3::ZERO;
			if(XmlChildElement)
			{
				assert(XmlChildElement->getAttributeValue("x") != "" &&
						 XmlChildElement->getAttributeValue("y") != "" &&
						 XmlChildElement->getAttributeValue("z") != "");
				spriteDesc.position.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("x"));
				spriteDesc.position.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("y"));
				spriteDesc.position.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("z"));
			}

			// Rotate it...
			XmlChildElement = sprite->getFirstChildByName("orientation");
			spriteDesc.orientation = TR::Quaternion::IDENTITY;
			if(XmlChildElement)
			{
				assert(XmlChildElement->getAttributeValue("qx") != "" &&
						 XmlChildElement->getAttributeValue("qy") != "" &&
						 XmlChildElement->getAttributeValue("qz") != "" &&
						 XmlChildElement->getAttributeValue("qw") != "");
				spriteDesc.orientation.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qx"));
				spriteDesc.orientation.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qy"));
				spriteDesc.orientation.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qz"));
				spriteDesc.orientation.w = TR::Utils::parseReal(XmlChildElement->getAttributeValue("qw"));
			}

			// Scale it.
			XmlChildElement = sprite->getFirstChildByName("scale");
			spriteDesc.scale = TR::Vector3::UNIT_SCALE;
			if(XmlChildElement)
			{
				assert(XmlChildElement->getAttributeValue("x") != "" &&
						 XmlChildElement->getAttributeValue("y") != "" &&
						 XmlChildElement->getAttributeValue("z") != "");
				spriteDesc.scale.x = TR::Utils::parseReal(XmlChildElement->getAttributeValue("x"));
				spriteDesc.scale.y = TR::Utils::parseReal(XmlChildElement->getAttributeValue("y"));
				spriteDesc.scale.z = TR::Utils::parseReal(XmlChildElement->getAttributeValue("z"));
			}

			// Is there an initial animation?
			if(sprite->getAttributeValue("initialAnimation") != "")
				spriteDesc.initialAnimation = sprite->getAttributeValue("initialAnimation");

			// Now create the sprite
			mFramework->getEntityManager()->createSprite(spriteDesc);
		}

		void TRManager::_loadVehicle(TR::Xml::Element *vehicle, TR::Scene::Node *parentNode)
		{
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
			
			mFramework->getEntityManager()->createVehicle(spriteDesc, vehicleDesc);

			std::list<TR::Physics::TorquePoint *>::iterator it = vehicleDesc.engineDesc.torqueCurve.begin();
			for( ; it != vehicleDesc.engineDesc.torqueCurve.end(); it++)
				delete (*it);
		}
		
		void TRManager::_parseNodeTree(TR::Xml::Element *node, TR::Scene::Node *parentNode)
		{
			assert(mFramework->getGraphicsManager() != 0);

			if(parentNode == 0)
			{
				// We're talking about the <scene> tag, so load the global elements
				_loadGlobalElements(node);

				// Check for physical materials
				TR::Xml::Element *XmlElement = node->getFirstChildByName("material");
				while(XmlElement)
				{
					_loadMaterial(XmlElement);
					XmlElement = XmlElement->getNextSiblingByName("material");
				}

				mRootSceneNode = mFramework->getGraphicsManager()->getRootNode();
				parentNode = mRootSceneNode;
				node = node->getFirstChildByName("nodes");
				
				TR::Xml::Element *childNode = node->getFirstChildByName("node");
				while(childNode)
				{
					_parseNodeTree(childNode, parentNode);
					childNode = childNode->getNextSiblingByName("node");
				}

				// Paged geometries
				XmlElement = node->getFirstChildByName("pagedGeometry");
				while(XmlElement)
				{
					_loadPagedGeometry(XmlElement);
					XmlElement = XmlElement->getNextSiblingByName("pagedGeometry");
				}

				// Check for joints
				XmlElement = node->getFirstChildByName("joint");
				while(XmlElement)
				{
					_loadJoint(XmlElement);
					XmlElement = XmlElement->getNextSiblingByName("joint");
				}
			}
			else
			{
				TR::Scene::Node *newNode = _loadSceneNode(node, parentNode);
				TR::Xml::Element *childNode = node->getFirstChildByName("node");
				while(childNode)
				{
					_parseNodeTree(childNode, newNode);
					childNode = childNode->getNextSiblingByName("node");
				}
			}
		}
	}
}