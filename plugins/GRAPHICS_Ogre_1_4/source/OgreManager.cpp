#include "..\include\OgreManager.h"
#include "..\include\OgreSprite.h"
#include "..\include\CeguiManager.h"

//PagedGeometry headers
#include "PagedGeometry.h"
#include "BatchPage.h"
#include "ImpostorPage.h"
#include "TreeLoader3D.h"


namespace TR
{
	namespace Graphics
	{
		OgreManager::OgreManager()
		{
			mFramework = 0;
			mGuiManager = 0;
			mRoot = 0;
			mRootNode = 0;
			mSceneManager = 0;
			mWaterRefractionListener = 0;
			mWaterReflectionListener = 0;
			mWindow = 0;
		}

		OgreManager::~OgreManager()
		{
			if(mGuiManager)
				delete mGuiManager;

			if(mRoot)
				delete mRoot;
		}

		void OgreManager::attachLightToBone(const std::string &lightName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation, const TR::Vector3 &offset)
		{
			Ogre::Entity *entity = mSceneManager->getEntity(boneOwner);
			Ogre::Light *ogreLight;
			TR::Scene::Node *node = mFramework->getSceneManager()->getSceneNode(lightName);
			
			if(node->getName() == "root")
				ogreLight = static_cast<Ogre::Light *>(mSceneManager->getRootSceneNode()->detachObject(node->getName()));
			else
				ogreLight = static_cast<Ogre::Light *>(mSceneManager->getSceneNode(node->getName())->detachObject(node->getName()));

			Ogre::Quaternion ogreQuat;
			ogreQuat.x = orientation.x;
			ogreQuat.y = orientation.y;
			ogreQuat.z = orientation.z;
			ogreQuat.w = orientation.w;
			entity->attachObjectToBone(bone, ogreLight, ogreQuat);
		}

		void OgreManager::attachObjectToBone(const std::string &objectName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation)
		{
			Ogre::Entity *entity = mSceneManager->getEntity(boneOwner);
			Ogre::Entity *ogreEntity;
			TR::Scene::Node *node = mFramework->getSceneManager()->getSceneNode(objectName);
			
			if(node->getName() == "root")
				ogreEntity = static_cast<Ogre::Entity *>(mSceneManager->getRootSceneNode()->detachObject(node->getName()));
			else
				ogreEntity = static_cast<Ogre::Entity *>(mSceneManager->getSceneNode(node->getName())->detachObject(node->getName()));

			Ogre::Quaternion ogreQuat;
			ogreQuat.x = orientation.x;
			ogreQuat.y = orientation.y;
			ogreQuat.z = orientation.z;
			ogreQuat.w = orientation.w;
			entity->attachObjectToBone(bone, ogreEntity, ogreQuat);
		}

		void OgreManager::attachParticleSystemToNode(const std::string &systemName, const std::string &nodeName)
		{
			mSceneManager->getSceneNode(nodeName)->attachObject(mSceneManager->getParticleSystem(systemName));
		}

		void OgreManager::attachSceneNode(TR::Scene::Node *node, TR::Scene::Node *parentNode)
		{
			if(node->getParent()->getName() == "root")
				mSceneManager->getRootSceneNode()->removeChild(node->getName());
			else
				mSceneManager->getSceneNode(node->getParent()->getName())->removeChild(node->getName());

			mSceneManager->getSceneNode(parentNode->getName())->addChild(mSceneManager->getSceneNode(node->getName()));
		}

		void OgreManager::cameraLookAt(const std::string &cameraName, const std::string &nodeName)
		{
			mSceneManager->getCamera(cameraName)->lookAt(mSceneManager->getSceneNode(nodeName)->getPosition());
		}

		TR::Scene::Camera* OgreManager::createCamera(TR::Scene::CameraDesc &cameraDesc)
		{
			if(cameraDesc.mode == TR::Scene::CM_STATIC)
				cameraDesc = reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc);

			Ogre::Camera *camera = mSceneManager->createCamera(cameraDesc.name);

			camera->setNearClipDistance(cameraDesc.nearClipDistance);

			camera->setFarClipDistance(cameraDesc.farClipDistance);

			camera->setPosition(static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).position.x),
									  static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).position.y),
									  static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).position.z));

			camera->setDirection(static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).direction.x),
										static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).direction.y),
										static_cast<float>((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc)).direction.z));

			if(cameraDesc.parentNode != 0)
				mSceneManager->getSceneNode(cameraDesc.parentNode->getName())->attachObject(camera);

			Ogre::Viewport* vp = mWindow->addViewport(camera, cameraDesc.viewport.zOrder, cameraDesc.viewport.left,
										cameraDesc.viewport.top, cameraDesc.viewport.width, cameraDesc.viewport.height);
			/*Ogre::Viewport* vp = mWindow->addViewport(camera, cameraDesc.viewport.zOrder, 0.0f,
										0.0f, 0.5f, 0.5f);*/

			//vp->setBackgroundColour(Ogre::ColourValue(0.0f, 1.0f, 0.0f));
			camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
			
			if(cameraDesc.mode = TR::Scene::CM_STATIC)
				return (new TR::Scene::CameraStatic((reinterpret_cast<TR::Scene::CameraStaticDesc &>(cameraDesc))));
			
			return 0;
		}

		TR::Gui::Manager* OgreManager::createGui(const std::string &schemeFile, const std::string &layoutFile)
		{
			if(!mGuiManager)
				mGuiManager = new TR::Gui::CeguiManager(mFramework, mSceneManager, mWindow, schemeFile, layoutFile);
			
			mRoot->getRenderSystem()->_initRenderTargets();

			return reinterpret_cast<TR::Gui::Manager*>(mGuiManager);
		}
		
		void OgreManager::createLight(const TR::Scene::LightDesc &lightDesc)
		{
			Ogre::Light *light = mSceneManager->createLight(lightDesc.name);
			switch(lightDesc.type)
			{
			case TR::Scene::LT_POINT:
				light->setType(Ogre::Light::LT_POINT);
				break;
			case TR::Scene::LT_DIRECTIONAL:
				light->setType(Ogre::Light::LT_DIRECTIONAL);
				break;
			case TR::Scene::LT_SPOT:
				light->setType(Ogre::Light::LT_SPOTLIGHT);
				break;
			default:
				light->setType(Ogre::Light::LT_POINT);
				break;
			}

			light->setDiffuseColour(static_cast<float>(lightDesc.diffuse.r),
											static_cast<float>(lightDesc.diffuse.g),
											static_cast<float>(lightDesc.diffuse.b));

			light->setSpecularColour(static_cast<float>(lightDesc.specular.r),
											 static_cast<float>(lightDesc.specular.g),
											 static_cast<float>(lightDesc.specular.b));

			light->setAttenuation(static_cast<float>(lightDesc.attenuationRange),
										 static_cast<float>(lightDesc.attenuationConstant),
										 static_cast<float>(lightDesc.attenuationLinear),
										 static_cast<float>(lightDesc.attenuationQuadratic));

			light->setPosition(static_cast<float>(lightDesc.position.x),
									 static_cast<float>(lightDesc.position.y),
									 static_cast<float>(lightDesc.position.z));

			light->setDirection(static_cast<float>(lightDesc.direction.x),
									  static_cast<float>(lightDesc.direction.y),
									  static_cast<float>(lightDesc.direction.z));

			light->setCastShadows(lightDesc.shadowCaster);

			light->setVisible(lightDesc.visible);

			if(lightDesc.node)
				mSceneManager->getSceneNode(lightDesc.node->getName())->attachObject(light);
		}

		void OgreManager::createPagedGeometry(TR::Graphics::PagedGeometryDesc &pagedGeometryDesc, const std::vector<TR::Vector3> &copies, const TR::AxisAlignedBox &boundaries)
		{
			PagedGeometry *pagedGeometry = new PagedGeometry();
			pagedGeometry->setCamera(mSceneManager->getCamera(pagedGeometryDesc.cameraName));
			pagedGeometry->setPageSize(pagedGeometryDesc.pageSize);
			std::list<TR::Graphics::DetailLevel>::iterator it = pagedGeometryDesc.detailLevels.begin();
			while(it != pagedGeometryDesc.detailLevels.end())
			{
				if(it->type == TR::Graphics::DLT_BATCH)
					pagedGeometry->addDetailLevel<BatchPage>(it->farDistance, it->fade);
				else if(it->type == TR::Graphics::DLT_IMPOSTOR)
					pagedGeometry->addDetailLevel<ImpostorPage>(it->farDistance, it->fade);

				it++;
			}
			Ogre::Entity *entity = mSceneManager->createEntity(pagedGeometryDesc.name, pagedGeometryDesc.mesh);
			TreeLoader3D *treeLoader = new TreeLoader3D(pagedGeometry, TBounds(boundaries.getMinimum().x, boundaries.getMinimum().z, boundaries.getMaximum().x, boundaries.getMaximum().z));
			pagedGeometry->setPageLoader(treeLoader);
			for(int i = 0; i < copies.size(); i++)
			{
				Ogre::Vector3 position;
				position.x = copies[i].x;
				position.y = copies[i].y;
				position.z = copies[i].z;
				treeLoader->addTree(entity, position);
			}
			mPagedGeometries[pagedGeometryDesc.name] = pagedGeometry;
		}

		void OgreManager::createParticleSystem(const std::string systemName, const std::string &templateName)
		{
			mSceneManager->createParticleSystem(systemName, templateName);
		}

		TR::Scene::Node* OgreManager::createSceneNode(const TR::Scene::NodeDesc &nodeDesc)
		{
			Ogre::SceneNode *node;
			if(nodeDesc.parent->getName() == "root")
				node = mSceneManager->getRootSceneNode()->createChildSceneNode(nodeDesc.name);
			else
				node = mSceneManager->getSceneNode(nodeDesc.parent->getName())->createChildSceneNode(nodeDesc.name);
			node->setOrientation(nodeDesc.orientation.w, nodeDesc.orientation.x, nodeDesc.orientation.y, nodeDesc.orientation.z);
			node->setPosition(nodeDesc.position.x, nodeDesc.position.y, nodeDesc.position.z);
			node->setScale(nodeDesc.scale.x, nodeDesc.scale.y, nodeDesc.scale.z);

			TR::Scene::Node* newNode = new TR::Scene::Node(nodeDesc, this);
			return newNode;
		}
		
		TR::Entity::Sprite* OgreManager::createSprite(TR::Entity::SpriteDesc &spriteDesc)
		{
			Ogre::Entity *ogreEntity = mSceneManager->createEntity(spriteDesc.name, spriteDesc.mesh);
			
			TR::Vector3 scale = spriteDesc.node->getScale();
			if(scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f)
				ogreEntity->setNormaliseNormals(true);

			ogreEntity->setVisible(spriteDesc.isVisible);

			mSceneManager->getSceneNode(spriteDesc.node->getName())->attachObject(ogreEntity);

			spriteDesc.boundingBox = reinterpret_cast<const TR::AxisAlignedBox &>(ogreEntity->getBoundingBox());
			spriteDesc.boundingRadius = static_cast<TR::Real>(ogreEntity->getBoundingRadius());

			if(spriteDesc.shadowCaster)
				ogreEntity->setCastShadows(true);
			else
				ogreEntity->setCastShadows(false);

			TR::Entity::Sprite *sprite = new TR::Entity::OgreSprite(spriteDesc);
			reinterpret_cast<TR::Entity::OgreSprite*>(sprite)->_setOgreEntity(ogreEntity);

			if(spriteDesc.initialAnimation != "")
				sprite->enableAnimation(spriteDesc.initialAnimation);

			return sprite;
		}

		void OgreManager::createWaterPlane(const TR::Graphics::WaterPlaneDesc &waterPlaneDesc)
		{
			mWaterPlane.normal = Ogre::Vector3::UNIT_Y;
			mWaterPlane.d = waterPlaneDesc.position.y;

			MeshManager::getSingleton().createPlane("WaterPlane", // name
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, // groupName
				mWaterPlane, // plane
				waterPlaneDesc.width, waterPlaneDesc.depth,
				waterPlaneDesc.xSegments, waterPlaneDesc.ySegments, 
				true, 1, waterPlaneDesc.uTiles, waterPlaneDesc.vTiles, Ogre::Vector3::UNIT_Z);

			Ogre::Entity *waterEntity = mSceneManager->createEntity( "plane", "WaterPlane" );
			waterEntity->setMaterialName("TR/WaterEffect");
			Ogre::SceneNode *node = mSceneManager->getRootSceneNode()->createChildSceneNode();
			node->attachObject(waterEntity);
			Ogre::Vector3 position;
			position.x = waterPlaneDesc.position.x;
			position.y = waterPlaneDesc.position.y;
			position.z = waterPlaneDesc.position.z;
			node->setPosition(position.x, 0.0, position.z);

			mWaterRefractionListener = new WaterRefractionListener(waterEntity);
			mWaterReflectionListener = new WaterReflectionListener(waterEntity, mWaterPlane, mSceneManager->getCamera(waterPlaneDesc.cameraName));

			Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual( "Refraction",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
				512, 512, 0, PF_R8G8B8, TU_RENDERTARGET );
			
			Ogre::RenderTarget *rttTex = texture->getBuffer()->getRenderTarget();
			Ogre::Viewport *v = rttTex->addViewport(mSceneManager->getCamera(waterPlaneDesc.cameraName));
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("TR/WaterEffect");
			mat->getTechnique(0)->getPass(0)->getTextureUnitState(2)->setTextureName("Refraction");
			v->setOverlaysEnabled(false);
			rttTex->addListener(mWaterRefractionListener);

			texture = Ogre::TextureManager::getSingleton().createManual( "Reflection",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
				512, 512, 0, PF_R8G8B8, TU_RENDERTARGET );

			rttTex = texture->getBuffer()->getRenderTarget();
			v = rttTex->addViewport(mSceneManager->getCamera(waterPlaneDesc.cameraName));
			mat = Ogre::MaterialManager::getSingleton().getByName("TR/WaterEffect");
			mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName("Reflection");
			v->setOverlaysEnabled(false);
			rttTex->addListener(mWaterReflectionListener);
		}

		void OgreManager::destroyParticleSystem(const std::string systemName)
		{
			mSceneManager->destroyParticleSystem(systemName);
		}

		void OgreManager::destroySceneNode(const std::string &nodeName)
		{
			mSceneManager->destroySceneNode(nodeName);
		}

		void OgreManager::destroySprite(TR::Entity::Sprite *sprite)
		{
			mSceneManager->destroyEntity(reinterpret_cast<TR::Entity::OgreSprite*>(sprite)->_getOgreEntity());
			delete sprite;
		}

		void OgreManager::detachParticleSystemFromNode(const std::string &systemName, const std::string &nodeName)
		{
			mSceneManager->getSceneNode(nodeName)->detachObject(mSceneManager->getParticleSystem(systemName));
		}

		void OgreManager::getMeshInfo(const std::string meshName, TR::Graphics::MeshInfo *meshInfo)
		{
			Ogre::Entity *entity = mSceneManager->getEntity(meshName);

			meshInfo->vertexCount = 0;
			meshInfo->indexCount  = 0;
			size_t			vertex_count;
			size_t			index_count;
			unsigned long*	indices; // NEEDED?!?!?!??!

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
						meshInfo->vertexCount += entity->getMesh()->sharedVertexData->vertexCount;
						added_shared = true;
					}
				}
				else 
				{
					meshInfo->vertexCount += submesh->vertexData->vertexCount;
				}

				meshInfo->indexCount += submesh->indexData->indexBuffer->getNumIndexes();
			}

			meshInfo->vertexBuffer = new TR::Vector3[meshInfo->vertexCount];
			indices = new unsigned long[meshInfo->indexCount];

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
						meshInfo->vertexBuffer[current_offset + j] = TR::Vector3(pReal[0],pReal[1],pReal[2]);
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
						meshInfo->faceBuffer[index_offset++] = pInt[k] + static_cast<unsigned int>(offset);
						//mMaterials[index_offset++] = currentMaterialIndex;
					}
				}
				else 
				{
					unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

					for ( size_t k = 0; k < numTris*3; ++k) 
					{
						meshInfo->faceBuffer[index_offset++] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
						//mMaterials[index_offset++] = currentMaterialIndex;
					}
				}

				ibuf->unlock();
				current_offset = next_offset;
			}
		}

		TR::Scene::Node* OgreManager::getRootNode()
		{
			if(!mRootNode)
			{
				TR::Scene::NodeDesc nodeDesc;

				nodeDesc.name = "root";

				nodeDesc.position.x = mSceneManager->getRootSceneNode()->getPosition().x;
				nodeDesc.position.y = mSceneManager->getRootSceneNode()->getPosition().y;
				nodeDesc.position.z = mSceneManager->getRootSceneNode()->getPosition().z;

				nodeDesc.orientation.x = mSceneManager->getRootSceneNode()->getOrientation().x;
				nodeDesc.orientation.y = mSceneManager->getRootSceneNode()->getOrientation().y;
				nodeDesc.orientation.z = mSceneManager->getRootSceneNode()->getOrientation().z;
				nodeDesc.orientation.w = mSceneManager->getRootSceneNode()->getOrientation().w;

				nodeDesc.scale.x = mSceneManager->getRootSceneNode()->getScale().x;
				nodeDesc.scale.y = mSceneManager->getRootSceneNode()->getScale().y;
				nodeDesc.scale.z = mSceneManager->getRootSceneNode()->getScale().z;

				mRootNode = new TR::Scene::Node(nodeDesc, this);
			}

			return mRootNode;
		}

		size_t OgreManager::getWindowHandle() 
		{ 
			size_t windowHnd = 0; 
			mWindow->getCustomAttribute("WINDOW", &windowHnd); 
			return windowHnd; 
		}

		TR::Vector3 OgreManager::getCameraDirection(const std::string &cameraName)
		{
			Ogre::Vector3 ogreVec = mSceneManager->getCamera(cameraName)->getRealDirection();
			TR::Vector3 vec(ogreVec.x, ogreVec.y, ogreVec.z);
			return vec;
		}

		TR::Quaternion OgreManager::getCameraOrientation(const std::string &cameraName)
		{
			Ogre::Quaternion ogreQuat = mSceneManager->getCamera(cameraName)->getRealOrientation();
			TR::Quaternion quat;
			quat.x = ogreQuat.x;
			quat.y = ogreQuat.y;
			quat.z = ogreQuat.z;
			quat.w = ogreQuat.w;
			return quat;
		}

		TR::Vector3 OgreManager::getCameraWorldPosition(const std::string &cameraName)
		{
			Ogre::Vector3 ogreVec = mSceneManager->getCamera(cameraName)->getRealPosition();
			TR::Vector3 vec(ogreVec.x, ogreVec.y, ogreVec.z);
			return vec;
		}

		TR::Vector3 OgreManager::getWorldPosition(TR::Scene::Node *node)
		{
			Ogre::Vector3 vec = mSceneManager->getSceneNode(node->getName())->getWorldPosition();
			TR::Vector3 newVector(vec.x, vec.y, vec.z);
			return newVector;
		}

		bool OgreManager::initialize(TR::Framework *framework)
		{
			mFramework = framework;

			Ogre::String pluginsPath;
			// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
			pluginsPath = "plugins.cfg";
#endif

			mRoot = new Ogre::Root(pluginsPath, "ogre.cfg", "Ogre.log");

			if(mRoot->showConfigDialog())
			{
				// If returned true, user clicked OK so initialise
				// Here we choose to let the system create a default rendering window by passing 'true'
				mWindow = mRoot->initialise(true);
			}
			else
			{
				return false;
			}

			//mRoot->addFrameListener(this);

			// Set default mipmap level (NB some APIs ignore this)
			Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

			Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);	
			Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);

			return true;
		}

		void OgreManager::loadResources(const std::string &file)
		{
			// Load resource paths from config file
			Ogre::ConfigFile cf;
			cf.load(file);

			// Go through all sections & settings in the file
			Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

			Ogre::String secName, typeName, archName;
			while (seci.hasMoreElements())
			{
				secName = seci.peekNextKey();
				Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
				Ogre::ConfigFile::SettingsMultiMap::iterator i;
				for (i = settings->begin(); i != settings->end(); ++i)
				{
					typeName = i->first;
					archName = i->second;
					Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
						archName, typeName, secName);
				}
			}

			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}

		void OgreManager::renderFrame() 
		{
			Ogre::WindowEventUtilities::messagePump(); 
			try
			{
				mRoot->renderOneFrame();
				//mRoot->startRendering();
				mPagedGeometriesIterator = mPagedGeometries.begin();
				while(mPagedGeometriesIterator != mPagedGeometries.end())
				{
					mPagedGeometriesIterator->second->update();			
					mPagedGeometriesIterator++;
				}
			}
			catch(Ogre::Exception &e)
			{
				assert("" == e.getFullDescription().c_str());
			}
			catch (CEGUI::Exception& exc)
			{
				MessageBoxA(0, exc.getMessage().c_str(), "CEGUI - Exception", MB_OK|MB_ICONERROR);
			}
		}

		void OgreManager::setAmbientLight(const TR::Graphics::ColorValue &ambientColor)
		{
			Ogre::ColourValue newColor;
			newColor.r = static_cast<float>(ambientColor.r);
			newColor.g = static_cast<float>(ambientColor.g);
			newColor.b = static_cast<float>(ambientColor.b);
			newColor.a = static_cast<float>(ambientColor.a);

			mSceneManager->setAmbientLight(newColor);
		}

		void OgreManager::setFog(const TR::Graphics::ColorValue &color, TR::Graphics::FogMode fogMode, TR::Real expDensity, TR::Real linearStart, TR::Real linearEnd)
		{
			Ogre::FogMode ogreFog;
			switch(fogMode)
			{
			case TR::Graphics::FOG_NONE: ogreFog = Ogre::FOG_NONE; break;
			case TR::Graphics::FOG_LINEAR: ogreFog = Ogre::FOG_LINEAR; break;
			case TR::Graphics::FOG_EXP: ogreFog = Ogre::FOG_EXP; break;
			case TR::Graphics::FOG_EXP2: ogreFog = Ogre::FOG_EXP2; break;
			default: break;
			}

			Ogre::ColourValue ogreColor;
			ogreColor.r = color.r;
			ogreColor.g = color.g;
			ogreColor.b = color.b;

			mSceneManager->setFog(ogreFog, ogreColor, expDensity, linearStart, linearEnd);
		}

		void OgreManager::setInheritOrientation(const std::string &nodeName, bool inherit)
		{
			mSceneManager->getSceneNode(nodeName)->setInheritOrientation(inherit);
		}

		void OgreManager::setLightVisibility(const std::string &lightName, bool visible)
		{
			mSceneManager->getLight(lightName)->setVisible(visible);
		}

		void OgreManager::setObjectVisibility(const std::string &objectName, bool visible)
		{
			mSceneManager->getEntity(objectName)->setVisible(visible);
		}

		void OgreManager::setSceneManager(TR::Scene::SceneType type)
		{
			switch(type)
			{
			case TR::Scene::ST_DEFAULT:
				mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "DefaultInstance");
				break;

			case TR::Scene::ST_OCTREE:
				mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "OctreeInstance");
				break;

			case TR::Scene::ST_BSP:
				mSceneManager = mRoot->createSceneManager(Ogre::ST_INTERIOR, "BSPInstance");
				break;

			case TR::Scene::ST_TERRAIN:
				mSceneManager = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "TerrainInstance");
				break;
			}

			mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		}

		void OgreManager::setSceneNodeDirection(const std::string &nodeName, TR::Vector3 direction)
		{
			mSceneManager->getSceneNode(nodeName)->setDirection(direction.x, direction.y, direction.z);
		}

		void OgreManager::setSceneNodeOrientation(const std::string &nodeName, const TR::Quaternion &orientation)
		{
			mSceneManager->getSceneNode(nodeName)->setOrientation(orientation.w, orientation.x, orientation.y, orientation.z);
		}

		void OgreManager::setSceneNodePosition(const std::string &nodeName, const TR::Vector3 &position)
		{
			mSceneManager->getSceneNode(nodeName)->setPosition(position.x, position.y, position.z);
		}

		void OgreManager::setSceneNodeScale(const std::string &nodeName, const TR::Vector3 &scale)
		{
			mSceneManager->getSceneNode(nodeName)->setScale(scale.x, scale.y, scale.z);
		}

		void OgreManager::setShadowColor(const TR::Graphics::ColorValue &color)
		{
			Ogre::ColourValue ogreColor;
			ogreColor.r = color.r;
			ogreColor.g = color.g;
			ogreColor.b = color.b;

			mSceneManager->setShadowColour(ogreColor);
		}

		void OgreManager::setSkyBox(const std::string &materialName)
		{
			mSceneManager->setSkyBox(true, materialName);
		}

		void OgreManager::translateCamera(const std::string &cameraName, TR::Vector3 offset, TR::Graphics::TransformSpace space)
		{
			Ogre::Vector3 vec(offset.x, offset.y, offset.z);
			if(space == TR::Graphics::TS_LOCAL)
				mSceneManager->getCamera(cameraName)->moveRelative(vec);
			else
				mSceneManager->getCamera(cameraName)->move(vec);
		}

		void OgreManager::translateSceneNode(TR::Scene::Node *node, TR::Vector3 offset, TR::Graphics::TransformSpace space)
		{
			Ogre::Vector3 translation(offset.x, offset.y, offset.z);
			mSceneManager->getSceneNode(node->getName())->translate(translation, static_cast<Ogre::Node::TransformSpace>(space));
		}

		void OgreManager::pitchSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space) 
		{ 
			const Ogre::Radian r(degree.valueRadians());
			mSceneManager->getSceneNode(nodeName)->pitch(r, static_cast<Ogre::Node::TransformSpace>(space)); 
		}

		void OgreManager::rollSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space) 
		{ 
			const Ogre::Radian r(degree.valueRadians());
			mSceneManager->getSceneNode(nodeName)->roll(r, static_cast<Ogre::Node::TransformSpace>(space)); 
		}

		TR::Quaternion OgreManager::yawSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space) 
		{ 
			const Ogre::Radian r(degree.valueRadians());
			mSceneManager->getSceneNode(nodeName)->yaw(r, static_cast<Ogre::Node::TransformSpace>(space)); 
			Ogre::Quaternion ogreQuat = mSceneManager->getSceneNode(nodeName)->getOrientation();
			TR::Quaternion quat;
			quat.x = ogreQuat.x;
			quat.y = ogreQuat.y;
			quat.z = ogreQuat.z;
			quat.w = ogreQuat.w;
			return quat;
		}
	}
}