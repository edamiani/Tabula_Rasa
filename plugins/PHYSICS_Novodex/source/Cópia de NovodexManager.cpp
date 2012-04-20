#include "../include/NovodexManager.h"
#include "../include/NovodexCharacter.h"
#include "../include/NovodexDynamic.h"
#include "../include/NovodexKinematic.h"
#include "../include/NovodexSolid.h"
#include "../include/NovodexStatic.h"
#include "../include/NovodexTrigger.h"
#include "../include/NovodexVehicle.h"
#include "../include/NovodexMaterial.h"
#include "../../../plugins/GRAPHICS_Ogre_1_4/include/OgreSprite.h"
#include "../include/nxOgre_util_stream.h"

namespace TR
{
	namespace Physics
	{
		NovodexManager::NovodexManager()
		{
			mIsInitialized = false;

			mWorld = 0;
			mDefaultScene = 0;
			mDefaultMaterial = 0;
			mDefaultGravity = NxVec3(0, -9.8, 0);
		}

		NovodexManager::~NovodexManager()
		{
			if (mWorld) 
				mWorld->release();

			mIsInitialized = false;
		}

		TR::Entity::Character* NovodexManager::createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			NxCapsuleControllerDesc desc;
			TR::Vector3 size;

			/*if(TR::Math::Abs(spriteDesc.boundingBox.getMaximum().y) > TR::Math::Abs(spriteDesc.boundingBox.getMinimum().y))
				size.y = TR::Math::Abs(spriteDesc.boundingBox.getMaximum().y) * 2;
			else
				size.y = TR::Math::Abs(spriteDesc.boundingBox.getMinimum().y) * 2;*/

			TR::Entity::NovodexCharacter *character = new TR::Entity::NovodexCharacter(mFramework->getEntityManager()->createSprite(spriteDesc));
			size.x = spriteDesc.boundingBox.getMaximum().x - spriteDesc.boundingBox.getMinimum().x;
			size.y = spriteDesc.boundingBox.getMaximum().y - spriteDesc.boundingBox.getMinimum().y;
			desc.callback = character;
			//desc.height = size.y;
			desc.height = size.y - size.x;
			desc.interactionFlag = NXIF_INTERACTION_INCLUDE;
			if(solidDesc.radius < 0.01)
				desc.radius = size.x / 2;
			else
				desc.radius = solidDesc.radius;
			desc.position.x = spriteDesc.position.x;
			desc.position.y = spriteDesc.position.y;
			desc.position.z = spriteDesc.position.z;
			desc.upDirection = NX_Y;
			desc.userData = reinterpret_cast<void*>(solidDesc.triggerFlag);
			NxController *controller = mNxControllerManager.createController(mDefaultScene, desc);
			controller->getActor()->userData = desc.userData;
			character->_setNxController(controller);
			character->setId(solidDesc.triggerFlag);
			return reinterpret_cast<TR::Entity::Character*>(character);
		}

		TR::Entity::Dynamic* NovodexManager::createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::NovodexDynamic *dynamic = new TR::Entity::NovodexDynamic(mFramework, spriteDesc, solidDesc);
			return reinterpret_cast<TR::Entity::Dynamic*>(dynamic);
		}

		void NovodexManager::createJoint(const JointDesc &jointDesc)
		{
			TR::Physics::NovodexSolid *childSolid;
			TR::Physics::NovodexSolid *parentSolid;

			assert(jointDesc.child != "");
			childSolid = static_cast<TR::Physics::NovodexSolid*>(getSolid(jointDesc.child));
			assert(childSolid != 0);

			if(jointDesc.parent != "" && jointDesc.parent != "null")
			{
				parentSolid = static_cast<TR::Physics::NovodexSolid*>(getSolid(jointDesc.parent));
				assert(parentSolid != 0);
			}
			else
				parentSolid = 0;

			switch(jointDesc.jointType)
			{
			case TR::Physics::JT_BALLANDSOCKET:
				break;

			case TR::Physics::JT_FIXED:
				//FIXED_JOINT  Anchor: not used   Axis 0: not used   Axis 1: not used   Axis 2: not used
				{
					NxFixedJointDesc fixedJoint;
					fixedJoint.actor[0] = reinterpret_cast<TR::Physics::NovodexSolid*>(parentSolid)->_getNxActor();
					fixedJoint.actor[1] = reinterpret_cast<TR::Physics::NovodexSolid*>(childSolid)->_getNxActor();
					mDefaultScene->createJoint(fixedJoint);
				}
				break;

			case TR::Physics::JT_HINGE:
				//HINGE_JOINT  Anchor: used   Axis 0: rotational   Axis 1: not used   Axis 2: not used
				{
					NxRevoluteJointDesc revJoint;
					revJoint.actor[0] = reinterpret_cast<TR::Physics::NovodexSolid*>(parentSolid)->_getNxActor();
					revJoint.actor[1] = reinterpret_cast<TR::Physics::NovodexSolid*>(childSolid)->_getNxActor();
					NxVec3 nxAnchor(static_cast<NxReal>(jointDesc.anchor.x), static_cast<NxReal>(jointDesc.anchor.y), static_cast<NxReal>(jointDesc.anchor.z));
					NxVec3 nxAxis0(static_cast<NxReal>(jointDesc.axes[0].x), static_cast<NxReal>(jointDesc.axes[0].y), static_cast<NxReal>(jointDesc.axes[0].z));
					revJoint.setGlobalAnchor(nxAnchor);
					revJoint.setGlobalAxis(nxAxis0);
					mDefaultScene->createJoint(revJoint);
				}
				break;

			case TR::Physics::JT_SLIDER:
				//SLIDER_JOINT  Anchor: not used   Axis 0: linear/rotational   Axis 1: not used   Axis 2: not used
				{
					NxCylindricalJointDesc sliderJoint;
					sliderJoint.actor[0] = reinterpret_cast<TR::Physics::NovodexSolid*>(parentSolid)->_getNxActor();
					sliderJoint.actor[1] = reinterpret_cast<TR::Physics::NovodexSolid*>(childSolid)->_getNxActor();
					NxVec3 nxAnchor(static_cast<NxReal>(jointDesc.anchor.x), static_cast<NxReal>(jointDesc.anchor.y), static_cast<NxReal>(jointDesc.anchor.z));
					NxVec3 nxAxis0(static_cast<NxReal>(jointDesc.axes[0].x), static_cast<NxReal>(jointDesc.axes[0].y), static_cast<NxReal>(jointDesc.axes[0].z));
					sliderJoint.setGlobalAnchor(nxAnchor);
					sliderJoint.setGlobalAxis(nxAxis0);
					NxCylindricalJoint* joint = (NxCylindricalJoint*)mDefaultScene->createJoint(sliderJoint);

					if(jointDesc.rotationLimits[0].enabled)
					{
						NxVec3 nxOffset(static_cast<NxReal>(jointDesc.rotationLimits[0].offset.x), static_cast<NxReal>(jointDesc.rotationLimits[0].offset.y), static_cast<NxReal>(jointDesc.rotationLimits[0].offset.z));
						joint->setLimitPoint(nxAnchor + nxOffset);
					}
					else if(jointDesc.translationLimits.enabled)
						joint->setLimitPoint(nxAnchor);

					if(jointDesc.translationLimits.enabled)
					{
						NxVec3 nxLowPlane(static_cast<NxReal>(jointDesc.translationLimits.low.plane.x), static_cast<NxReal>(jointDesc.translationLimits.low.plane.y), static_cast<NxReal>(jointDesc.translationLimits.low.plane.z));
						NxVec3 nxHighPlane(static_cast<NxReal>(jointDesc.translationLimits.high.plane.x), static_cast<NxReal>(jointDesc.translationLimits.high.plane.y), static_cast<NxReal>(jointDesc.translationLimits.high.plane.z));
						NxVec3 nxLowPosition(static_cast<NxReal>(jointDesc.translationLimits.low.position.x), static_cast<NxReal>(jointDesc.translationLimits.low.position.y), static_cast<NxReal>(jointDesc.translationLimits.low.position.z));
						NxVec3 nxHighPosition(static_cast<NxReal>(jointDesc.translationLimits.high.position.x), static_cast<NxReal>(jointDesc.translationLimits.high.position.y), static_cast<NxReal>(jointDesc.translationLimits.high.position.z));
						joint->addLimitPlane(nxLowPlane, nxLowPosition);
						joint->addLimitPlane(nxHighPlane, nxHighPosition);
					}

					if(jointDesc.rotationLimits[0].enabled)
					{
						NxVec3 nxLowPlane(static_cast<NxReal>(jointDesc.rotationLimits[0].low.plane.x), static_cast<NxReal>(jointDesc.rotationLimits[0].low.plane.y), static_cast<NxReal>(jointDesc.rotationLimits[0].low.plane.z));
						NxVec3 nxHighPlane(static_cast<NxReal>(jointDesc.rotationLimits[0].high.plane.x), static_cast<NxReal>(jointDesc.rotationLimits[0].high.plane.y), static_cast<NxReal>(jointDesc.rotationLimits[0].high.plane.z));
						NxVec3 nxLowPosition(static_cast<NxReal>(jointDesc.rotationLimits[0].low.position.x), static_cast<NxReal>(jointDesc.rotationLimits[0].low.position.y), static_cast<NxReal>(jointDesc.rotationLimits[0].low.position.z));
						NxVec3 nxHighPosition(static_cast<NxReal>(jointDesc.rotationLimits[0].high.position.x), static_cast<NxReal>(jointDesc.rotationLimits[0].high.position.y), static_cast<NxReal>(jointDesc.rotationLimits[0].high.position.z));
						joint->addLimitPlane(nxLowPlane, nxLowPosition);
						joint->addLimitPlane(nxHighPlane, nxHighPosition);
					}

				}
				break;

			case TR::Physics::JT_UNIVERSAL:
				//UNIVERSAL_JOINT  Anchor: used   Axis 0: rotational   Axis 1: rotational   Axis 2: not used
				break;

			case TR::Physics::JT_WHEEL:
				//WHEEL_JOINT  Anchor: used   Axis 0: rotational ("steering axis")   Axis 1: rotational ("wheel axis")   Axis 2: not used
				break;

			default:
				//FIXED_JOINT  Anchor: not used   Axis 0: not used   Axis 1: not used   Axis 2: not used
				{
					NxFixedJointDesc fixedJoint;
					fixedJoint.actor[0] = reinterpret_cast<TR::Physics::NovodexSolid*>(parentSolid)->_getNxActor();
					fixedJoint.actor[1] = reinterpret_cast<TR::Physics::NovodexSolid*>(childSolid)->_getNxActor();
					mDefaultScene->createJoint(fixedJoint);
				}
				break;
			}
		}

		TR::Physics::Material* NovodexManager::createMaterial(const TR::Physics::MaterialDesc &material)
		{
			TR::Physics::NovodexMaterial *newMaterial;
			NxMaterial *nxMaterial;

			if(material.name != "default")
			{
				NxMaterialDesc matDesc;
				matDesc.dynamicFriction = static_cast<NxReal>(material.dynamicFriction);
				matDesc.restitution = static_cast<NxReal>(material.restitution);
				matDesc.staticFriction = static_cast<NxReal>(material.staticFriction);

				nxMaterial = mDefaultScene->createMaterial(matDesc);
				newMaterial = new TR::Physics::NovodexMaterial(material);
				newMaterial->setIndex(static_cast<TR::uint16>(nxMaterial->getMaterialIndex()));
				newMaterial->_setNxMaterial(nxMaterial);
				mMaterials[material.name] = newMaterial;
			}
			else
			{
				nxMaterial = mDefaultScene->getMaterialFromIndex(0);
				newMaterial = new TR::Physics::NovodexMaterial(material);
				newMaterial->setIndex(0);
				newMaterial->_setNxMaterial(nxMaterial);
				mMaterials[material.name] = newMaterial;
			}

			return newMaterial;
		}

		
		TR::Entity::Kinematic* NovodexManager::createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::NovodexKinematic *kinematic = new TR::Entity::NovodexKinematic(mFramework, spriteDesc, solidDesc);
			return reinterpret_cast<TR::Entity::Kinematic*>(kinematic);
		}
		
		TR::Physics::Solid* NovodexManager::createSolid(const TR::Physics::SolidDesc &solidDesc)
		{
			NxActorDesc actorDesc;
			NxBodyDesc bodyDesc;
			TR::Vector3 size;
			NxQuat orientation;
			
			orientation.x = static_cast<NxReal>(solidDesc.orientation.x);
			orientation.y = static_cast<NxReal>(solidDesc.orientation.y);
			orientation.z = static_cast<NxReal>(solidDesc.orientation.z);
			orientation.w = static_cast<NxReal>(solidDesc.orientation.w);

			switch(solidDesc.collisionType)
			{
				case TR::Physics::CT_BOX:
					{
						size = solidDesc.sprite->getBoundingBox().getMaximum() - solidDesc.sprite->getBoundingBox().getMinimum();
						size.x *= solidDesc.scale.x;
						size.y *= solidDesc.scale.y;
						size.z *= solidDesc.scale.z;
						size /= 2; // Novodex uses the 'radii' of a box
						NxBoxShapeDesc boxDesc;
						boxDesc.dimensions.set(static_cast<NxReal>(size.x), static_cast<NxReal>(size.y), static_cast<NxReal>(size.z));
						boxDesc.materialIndex = getMaterial(solidDesc.materialName)->getIndex();
						
						if(solidDesc.isTrigger)
						{
							//boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;
						}

						if(solidDesc.ccd)
						{
							NxU32 triangles[3 * 12] = { 
								0,1,3,
								0,3,2,
								3,7,6,
								3,6,2,
								1,5,7,
								1,7,3,
								4,6,7,
								4,7,5,
								1,0,4,
								5,1,4,
								4,0,2,
								4,2,6
							};

							NxVec3 points[8];

							//static mesh
							points[0].set( 0.005, -0.005, -0.005);
							points[1].set( 0.005, -0.005,  0.005);
							points[2].set( 0.005,  0.005, -0.005);
							points[3].set( 0.005,  0.005,  0.005);

							points[4].set(-0.005, -0.005, -0.005);
							points[5].set(-0.005, -0.005,  0.005);
							points[6].set(-0.005,  0.005, -0.005);
							points[7].set(-0.005,  0.005,  0.005);

							NxSimpleTriangleMesh stm;
							stm.numVertices = 8;
							stm.numTriangles = 6*2;
							stm.pointStrideBytes = sizeof(NxVec3);
							stm.triangleStrideBytes = sizeof(NxU32)*3;

							stm.points = points;
							stm.triangles = triangles;
							stm.flags |= NX_MF_FLIPNORMALS;
							boxDesc.ccdSkeleton = mWorld->createCCDSkeleton(stm);
							boxDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
						}

						actorDesc.shapes.push_back(&boxDesc);
						actorDesc.shapes[0]->localPose.t.x = static_cast<NxReal>(solidDesc.offset.x);
						actorDesc.shapes[0]->localPose.t.y = static_cast<NxReal>(solidDesc.offset.y);
						actorDesc.shapes[0]->localPose.t.z = static_cast<NxReal>(solidDesc.offset.z);
					}
					break;

				case TR::Physics::CT_SPHERE:
					{
						NxSphereShapeDesc sphereDesc;
						sphereDesc.radius = solidDesc.sprite->getBoundingRadius();
						sphereDesc.materialIndex = getMaterial(solidDesc.materialName)->getIndex();
						if(solidDesc.isTrigger)
						{
							sphereDesc.shapeFlags |= NX_TRIGGER_ENABLE;
						}
						actorDesc.shapes.push_back(&sphereDesc);
					}
					break;

				case TR::Physics::CT_CAPSULE:
					{
						size = solidDesc.sprite->getBoundingBox().getMaximum() - solidDesc.sprite->getBoundingBox().getMinimum();
						size.x *= solidDesc.scale.x;
						size.y *= solidDesc.scale.y;
						size.z *= solidDesc.scale.z;
						NxCapsuleShapeDesc capsuleDesc;
						capsuleDesc.height = static_cast<NxReal>(size.y); 
						capsuleDesc.radius = static_cast<NxReal>(size.x / 2.0);
						capsuleDesc.materialIndex = getMaterial(solidDesc.materialName)->getIndex();
						if(solidDesc.isTrigger)
						{
							capsuleDesc.shapeFlags |= NX_TRIGGER_ENABLE;
						}
						actorDesc.shapes.push_back(&capsuleDesc);

					}
					break;

				case TR::Physics::CT_PLANE:
					break;

				case TR::Physics::CT_TREECOLLISION:
					{
						NxTriangleMeshShapeDesc* meshDesc = _createMeshCollision(reinterpret_cast<TR::Entity::OgreSprite*>(solidDesc.sprite)->_getOgreEntity(), solidDesc.scale);
						meshDesc->materialIndex = getMaterial(solidDesc.materialName)->getIndex();
						actorDesc.shapes.push_back(meshDesc);
						TR::Vector3 middle = (solidDesc.sprite->getBoundingBox().getMaximum() + solidDesc.sprite->getBoundingBox().getMinimum()) / 2;
						/*actorDesc.shapes[0]->localPose.t.x = static_cast<NxReal>(-middle.x);
						actorDesc.shapes[0]->localPose.t.y = static_cast<NxReal>(-middle.y);
						actorDesc.shapes[0]->localPose.t.z = static_cast<NxReal>(-middle.z);*/
					}
					break;

				default: // Box
					{
						size = solidDesc.sprite->getBoundingBox().getMaximum() - solidDesc.sprite->getBoundingBox().getMinimum();
						size.x *= solidDesc.scale.x;
						size.y *= solidDesc.scale.y;
						size.z *= solidDesc.scale.z;
						size /= 2; // Novodex uses the 'radii' of a box
						NxBoxShapeDesc boxDesc;
						boxDesc.dimensions.set(static_cast<NxReal>(size.x), static_cast<NxReal>(size.y), static_cast<NxReal>(size.z));
						boxDesc.materialIndex = getMaterial(solidDesc.materialName)->getIndex();
						if(solidDesc.isTrigger)
						{
							boxDesc.shapeFlags |= NX_TRIGGER_ENABLE;
						}
						actorDesc.shapes.push_back(&boxDesc);
					}
					break;
			}

			if(solidDesc.solidType == TR::Physics::ST_STATIC)
			{
				actorDesc.body = 0;
				actorDesc.density = 0.0;
			}
			else
			{
				actorDesc.body = &bodyDesc;
				bodyDesc.solverIterationCount = 30;
				actorDesc.density = static_cast<NxReal>(solidDesc.density);
			}

			TR::Vector3 position = solidDesc.sprite->getPosition();
			actorDesc.globalPose.M.fromQuat(orientation);
			actorDesc.globalPose.t.x = static_cast<NxReal>(position.x);
			actorDesc.globalPose.t.y = static_cast<NxReal>(position.y);
			actorDesc.globalPose.t.z = static_cast<NxReal>(position.z);

			actorDesc.userData = reinterpret_cast<void*>(solidDesc.triggerFlag);

			assert(actorDesc.isValid());
			NxActor* actor = mDefaultScene->createActor(actorDesc);

			if(solidDesc.solidType == TR::Physics::ST_KINEMATIC)
			{
				actor->raiseBodyFlag(NX_BF_KINEMATIC);
				
				NxVec3 nxPos;
				nxPos.x = solidDesc.position.x;
				nxPos.y = solidDesc.position.y;
				nxPos.z = solidDesc.position.z;
				actor->moveGlobalPosition(nxPos);

				NxQuat nxQuat;
				nxQuat.x = solidDesc.orientation.x;
				nxQuat.y = solidDesc.orientation.y;
				nxQuat.z = solidDesc.orientation.z;
				nxQuat.w = solidDesc.orientation.w;
				actor->moveGlobalOrientationQuat(nxQuat);
			}
			else
			{
				NxVec3 nxPos;
				nxPos.x = solidDesc.position.x;
				nxPos.y = solidDesc.position.y;
				nxPos.z = solidDesc.position.z;
				actor->setGlobalPosition(nxPos);

				NxQuat nxQuat;
				nxQuat.x = solidDesc.orientation.x;
				nxQuat.y = solidDesc.orientation.y;
				nxQuat.z = solidDesc.orientation.z;
				nxQuat.w = solidDesc.orientation.w;
				actor->setGlobalOrientationQuat(nxQuat);
			}

			if(solidDesc.isTrigger)
			{
				actor->getShapes()[0]->setFlag(NX_TRIGGER_ENABLE, true);
				actor->setMass(solidDesc.mass);
			}

			if(solidDesc.ccd)
			{
				actor->setCCDMotionThreshold(0.001);
			}

			TR::Physics::NovodexSolid *nxSolid = new TR::Physics::NovodexSolid(solidDesc);
			nxSolid->_setNxActor(actor);
			setSolidGroup(nxSolid, solidDesc.group);
			mSolids[solidDesc.name] = nxSolid;

			return nxSolid;
		}

		TR::Entity::Static* NovodexManager::createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::NovodexStatic *newStatic = new TR::Entity::NovodexStatic(mFramework, spriteDesc, solidDesc);
			return reinterpret_cast<TR::Entity::Static*>(newStatic);
		}

		TR::Entity::Trigger* NovodexManager::createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc)
		{
			TR::Entity::NovodexTrigger *trigger = new TR::Entity::NovodexTrigger(mFramework, spriteDesc, solidDesc);
			return reinterpret_cast<TR::Entity::Trigger*>(trigger);
		}

		TR::Entity::Vehicle* NovodexManager::createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc)
		{
			TR::Entity::NovodexVehicle *vehicle = new TR::Entity::NovodexVehicle(mFramework, spriteDesc, vehicleDesc);
			return reinterpret_cast<TR::Entity::Vehicle*>(vehicle);
		}

		void NovodexManager::destroyDynamicEntity(TR::Entity::Dynamic *dynamic)
		{
			mFramework->getGraphicsManager()->destroySprite(dynamic->getSprite());
			destroySolid(dynamic->getSolid());
		}

		void NovodexManager::destroyKinematicEntity(TR::Entity::Kinematic *kinematic)
		{
			mFramework->getGraphicsManager()->destroySprite(kinematic->getSprite());
			destroySolid(kinematic->getSolid());
		}
		
		void NovodexManager::destroySolid(TR::Physics::Solid *solid)
		{
			mDefaultScene->releaseActor(*static_cast<TR::Physics::NovodexSolid *>(solid)->_getNxActor());
			mSolids.erase((solid->getName()));
		}

		void NovodexManager::destroyStaticEntity(TR::Entity::Static *staticEntity)
		{
			mFramework->getGraphicsManager()->destroySprite(staticEntity->getSprite());
			destroySolid(staticEntity->getSolid());
		}

		TR::Real NovodexManager::getDistanceFromClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance)
		{
			NxVec3 origin(position.x, position.y, position.z);
			NxVec3 dir(direction.x, direction.y, direction.z);
			NxRay ray(origin, dir);
			NxRaycastHit hit;

			mDefaultScene->raycastClosestShape(ray, NX_ALL_SHAPES, hit, -1, rayDistance);
			if(hit.shape)
				return hit.distance;
			else
				return 0.0;
		}

		TR::Physics::Material* NovodexManager::getMaterial(const std::string &name)
		{
			mMaterialsIterator = mMaterials.find(name);
			assert(mMaterialsIterator != mMaterials.end());
			
			return mMaterialsIterator->second;
		}

		TR::Physics::Solid* NovodexManager::getSolid(const std::string &name)
		{
			mSolidsIterator = mSolids.find(name);
			if(mSolidsIterator != mSolids.end())
				return mSolidsIterator->second;
			else
				return 0;
		}

		void NovodexManager::initialize(TR::Framework *framework)
		{
			mFramework = framework;

			mWorld = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
			mWorld->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.0001f);
			mWorld->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
			mWorld->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.01 * 0.01);
			mWorld->setParameter(NX_DEFAULT_SLEEP_ANG_VEL_SQUARED, 0.01 * 0.01);
			mWorld->setParameter(NX_CONTINUOUS_CD, 1);
			mWorld->setParameter(NX_CCD_EPSILON, 0.01f);
			mWorld->setParameter(NX_SKIN_WIDTH,0.00025f);
			mWorld->setParameter(NX_VISUALIZE_WORLD_AXES, true);
			mWorld->setParameter(NX_MAX_ANGULAR_VELOCITY, 90);

			NxSceneDesc sceneDesc;
			sceneDesc.gravity = mDefaultGravity;
			//sceneDesc.collisionDetection = true;
			mDefaultScene = mWorld->createScene(sceneDesc);
			mDefaultScene->setTiming(0.02, 8, NX_TIMESTEP_FIXED);

			//if(triggerReport)
			mDefaultScene->setUserTriggerReport(this);

			//if(contactReport)
			mDefaultScene->setUserContactReport(this);

			TR::Physics::MaterialDesc defaultMaterial;
			defaultMaterial.name = "default";
			defaultMaterial.dynamicFriction = 0.4;
			defaultMaterial.restitution = 0.3;
			defaultMaterial.staticFriction = 0.4;
			mDefaultMaterial = createMaterial(defaultMaterial);

			NxMaterialDesc tireMaterial;
			tireMaterial.flags |= NX_MF_DISABLE_FRICTION;
			mTireMaterial = mDefaultScene->createMaterial(tireMaterial);

			mIsInitialized = true;
		}

		bool NovodexManager::isPointInSphere(const TR::Vector3 &sphereCenter, TR::Real sphereRadius, const TR::Vector3 &point)
		{
			NxSphere sphere;
			NxVec3 nxCenter(sphereCenter.x, sphereCenter.y, sphereCenter.z);
			NxVec3 nxPoint(point.x, point.y, point.z);
			sphere.center = nxCenter;
			sphere.radius = sphereRadius;
			return sphere.Contains(nxPoint);
		}

		void NovodexManager::onContactNotify(NxContactPair& pair, NxU32 events)
		{
			TR::Physics::collisionNotify cn;
			switch(events)
			{
			case NX_NOTIFY_ON_START_TOUCH:
				cn = TR::Physics::CN_START_TOUCH;
				break;
			case NX_NOTIFY_ON_TOUCH:
				cn = TR::Physics::CN_TOUCH;
				break;
			case NX_NOTIFY_ON_END_TOUCH:
				cn = TR::Physics::CN_END_TOUCH;
				break;
			default:
				cn = TR::Physics::CN_TOUCH;
				break;
			}
			mFramework->onContactNotify(reinterpret_cast<TR::uint32>(pair.actors[0]->userData), reinterpret_cast<TR::uint32>(pair.actors[1]->userData), cn);
		}

		void NovodexManager::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
		{
			TR::Entity::triggerNotify tn;
			switch(status)
			{
			case NX_TRIGGER_ON_ENTER:
				tn = TR::Entity::TN_ON_ENTER;
				break;
			case NX_TRIGGER_ON_STAY:
				tn = TR::Entity::TN_ON_STAY;
				break;
			case NX_TRIGGER_ON_LEAVE:
				tn = TR::Entity::TN_ON_LEAVE;
				break;
			default:
				tn = TR::Entity::TN_ON_ENTER;
				break;
			}
			
			mFramework->onTrigger(reinterpret_cast<TR::uint32>(triggerShape.getActor().userData), reinterpret_cast<TR::uint32>(otherShape.getActor().userData), tn);
		}

		TR::uint32 NovodexManager::raycastClosestObject(TR::Vector3 position, TR::Vector3 direction, TR::Real rayDistance)
		{
			NxVec3 origin(position.x, position.y, position.z);
			NxVec3 dir(direction.x, direction.y, direction.z);
			NxRay ray(origin, dir);
			NxRaycastHit hit;

			mDefaultScene->raycastClosestShape(ray, NX_ALL_SHAPES, hit, -1, rayDistance);
			if(hit.shape)
				return reinterpret_cast<TR::uint32>(hit.shape->getActor().userData);
			else
				return 0;
		}

		void NovodexManager::setCollisionPairFlags(TR::Physics::Solid *solid1, TR::Physics::Solid *solid2, TR::uint32 collisionNotify)
		{
			NxActor *actor1 = static_cast<TR::Physics::NovodexSolid *>(solid1)->_getNxActor();
			NxActor *actor2 = static_cast<TR::Physics::NovodexSolid *>(solid2)->_getNxActor();
			
			TR::uint32 contact;
			if(collisionNotify & TR::Physics::CN_IGNORE_PAIR)
				contact = NX_IGNORE_PAIR;
			if(collisionNotify & TR::Physics::CN_START_TOUCH)
				contact |= NX_NOTIFY_ON_START_TOUCH;
			if(collisionNotify & TR::Physics::CN_TOUCH)
				contact |= NX_NOTIFY_ON_TOUCH;
			if(collisionNotify & TR::Physics::CN_END_TOUCH)
				contact |= NX_NOTIFY_ON_END_TOUCH;

			mDefaultScene->setActorPairFlags(*actor1, *actor2, contact);
		}

		void NovodexManager::setSolidGroup(TR::Physics::Solid *solid, TR::uint16 group)
		{
			NxU32 nbShapes = reinterpret_cast<TR::Physics::NovodexSolid*>(solid)->_getNxActor()->getNbShapes();
			NxShape* const* shapes = reinterpret_cast<TR::Physics::NovodexSolid*>(solid)->_getNxActor()->getShapes();

			while (nbShapes--)
			{
				shapes[nbShapes]->setGroup(static_cast<NxCollisionGroup>(group));
			}
		}

		void NovodexManager::simulate(TR::Real dt)
		{
			mDefaultScene->simulate(static_cast<NxReal>(dt) / 1000);
			mDefaultScene->flushStream();
			while (!mDefaultScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
			mNxControllerManager.updateControllers();
		}

		NxTriangleMeshShapeDesc* NovodexManager::_createMeshCollision(const Ogre::Entity* entity, const TR::Vector3 &scale)
		{
			unsigned int	mVertexCount = 0;
			unsigned int	mIndexCount  = 0;
			size_t			vertex_count;
			Ogre::Vector3*	vertices;
			size_t			index_count;
			unsigned long*	indices;

			bool added_shared = false;
			vertex_count = 0;
			index_count = 0;
			size_t current_offset = 0;
			size_t shared_offset = 0;
			size_t next_offset = 0;
			size_t index_offset = 0;


			for ( unsigned short i = 0; i < entity->getMesh()->getNumSubMeshes(); ++i) 
			{
				Ogre::SubMesh* submesh = entity->getMesh()->getSubMesh( i );

				if(submesh->useSharedVertices) 
				{
					if( !added_shared ) 
					{
						mVertexCount += entity->getMesh()->sharedVertexData->vertexCount;
						added_shared = true;
					}
				}
				else 
				{
					mVertexCount += submesh->vertexData->vertexCount;
				}

				mIndexCount += submesh->indexData->indexBuffer->getNumIndexes();
			}


			vertices = new Ogre::Vector3[mVertexCount];
			indices = new unsigned long[mIndexCount];

			NxVec3* mMeshVertices = new NxVec3[mVertexCount];
			NxU32* mMeshFaces = new NxU32[mIndexCount];
			NxMaterialIndex* mMaterials = new NxMaterialIndex[mIndexCount];

			NxMaterialIndex currentMaterialIndex = 0;
			added_shared = false;

			for ( unsigned short i = 0; i < entity->getMesh()->getNumSubMeshes();i++) 
			{			
				Ogre::SubMesh* submesh = entity->getMesh()->getSubMesh(i);
				Ogre::VertexData* vertex_data = submesh->useSharedVertices ? entity->getMesh()->sharedVertexData : submesh->vertexData;
		
				//currentMaterialIndex = mScene->findMaterialIndex(submesh->getMaterialName());

				if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
				{
					if(submesh->useSharedVertices)
					{
						added_shared = true;
						shared_offset = current_offset;
					}

					const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
					Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
					unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

					float* pReal;

					for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) 
					{
						posElem->baseVertexPointerToElement(vertex, &pReal);
						mMeshVertices[current_offset + j] = NxVec3(pReal[0],pReal[1],pReal[2]);
					}

					vbuf->unlock();
					next_offset += vertex_data->vertexCount;
				}

				Ogre::IndexData* index_data = submesh->indexData;
				
				size_t numTris = (index_data->indexBuffer->getNumIndexes() / 3);
				Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

				bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);


				if ( use32bitindexes )	
				{
					unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

					size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;


					for ( size_t k = 0; k < numTris*3; ++k) 
					{
						mMeshFaces[index_offset++] = pInt[k] + static_cast<unsigned int>(offset);
						//mMaterials[index_offset++] = currentMaterialIndex;
					}
				}
				else 
				{
					unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

					for ( size_t k = 0; k < numTris*3; ++k) 
					{
						mMeshFaces[index_offset++] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
						//mMaterials[index_offset++] = currentMaterialIndex;
					}
				}

				ibuf->unlock();
				current_offset = next_offset;
			}

			NxTriangleMeshDesc mTriangleMeshDescription;
			mTriangleMeshDescription.numVertices				= mVertexCount;
			mTriangleMeshDescription.numTriangles				= mIndexCount / 3;
			mTriangleMeshDescription.pointStrideBytes			= sizeof(NxVec3);
			mTriangleMeshDescription.triangleStrideBytes		= 3 * sizeof(NxU32);
			Ogre::LogManager::getSingleton().logMessage("5");
			mTriangleMeshDescription.points						= mMeshVertices;							
			
			mTriangleMeshDescription.flags						= NX_MF_HARDWARE_MESH;
			mTriangleMeshDescription.triangles					= mMeshFaces;
			mTriangleMeshDescription.materialIndexStride		= 0;
			//mTriangleMeshDescription.materialIndexStride		= sizeof(NxMaterialIndex);
			//mTriangleMeshDescription.materialIndices			= mMaterials;
			//Ogre::LogManager::getSingleton().logMessage("6");
			MemoryWriteBuffer buf;

			NxTriangleMeshShapeDesc* triMeshDesc = new NxTriangleMeshShapeDesc();

			NxInitCooking(NULL,NULL);

			if (NxCookTriangleMesh(mTriangleMeshDescription, buf)) 
			{
				triMeshDesc->meshData = mWorld->createTriangleMesh(MemoryReadBuffer(buf.data));
			}
			else 
			{
				return 0;	
			}

			NxCloseCooking();

			delete []vertices;
			delete []indices;

			delete []mMeshVertices;
			delete []mMeshFaces;
			delete []mMaterials;

			return triMeshDesc;
		}
	}
}