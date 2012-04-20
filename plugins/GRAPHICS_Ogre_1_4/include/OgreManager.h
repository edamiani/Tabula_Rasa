#ifndef __OGRE_MANAGER__
#define __OGRE_MANAGER__

#include "Ogre.h"
#include "..\..\..\include\Dependencies.h"
#include "..\..\..\include\EntityManager.h"
#include "..\..\..\include\EntitySprite.h"
#include "..\..\..\include\GraphicsManager.h"
#include "..\..\..\include\SceneCameraStatic.h"
#include "..\..\..\include\SceneLight.h"
#include "..\..\..\include\SceneNode.h"
#include "..\..\..\include\Singleton.h"
#include "CeguiManager.h"

//PagedGeometry headers
#include "PagedGeometry.h"
#include "BatchPage.h"
#include "ImpostorPage.h"
#include "TreeLoader3D.h"

namespace TR
{
	namespace Graphics
	{
		class WaterRefractionListener : public RenderTargetListener
		{
		public:
			WaterRefractionListener(Ogre::Entity *entity) : mPlaneEntity(entity) { }
			void preRenderTargetUpdate(const RenderTargetEvent& evt)
			{
				// Hide plane and objects above the water
				mPlaneEntity->setVisible(false);
			}
			void postRenderTargetUpdate(const RenderTargetEvent& evt)
			{
				// Show plane and objects above the water
				mPlaneEntity->setVisible(true);
			}

		private:
			Ogre::Entity *mPlaneEntity;
		};

		class WaterReflectionListener : public RenderTargetListener
		{
		public:
			WaterReflectionListener(Ogre::Entity *planeEntity, const Ogre::Plane &plane, Ogre::Camera *camera) : mPlaneEntity(planeEntity), mPlane(plane), mCamera(camera) { }
			void preRenderTargetUpdate(const RenderTargetEvent& evt)
			{
				// Hide plane and objects below the water
				mPlaneEntity->setVisible(false);
				mCamera->enableReflection(mPlane);
			}
			void postRenderTargetUpdate(const RenderTargetEvent& evt)
			{
				// Show plane and objects below the water
				mPlaneEntity->setVisible(true);
				mCamera->disableReflection();
			}

		private:
			Ogre::Camera			*mCamera;
			const Ogre::Plane		&mPlane;
			Ogre::Entity			*mPlaneEntity;
		};

		class OgreManager : public TR::Graphics::Manager, public Ogre::WindowEventListener
		{
		public:
												OgreManager();
			 									~OgreManager();

			void								attachLightToBone(const std::string &lightName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation = TR::Quaternion::IDENTITY, const TR::Vector3 &offset = TR::Vector3::ZERO);
			void								attachObjectToBone(const std::string &nodeName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation = TR::Quaternion::IDENTITY);
			void								attachParticleSystemToNode(const std::string &systemName, const std::string &nodeName);
			void								attachSceneNode(TR::Scene::Node *node, TR::Scene::Node *parentNode);
			void								cameraLookAt(const std::string &cameraName, const std::string &nodeName);
			TR::Scene::Camera*			createCamera(TR::Scene::CameraDesc &cameraDesc);
			TR::Gui::Manager*				createGui(const std::string &schemeFile, const std::string &layoutFile);
			void								createLight(const TR::Scene::LightDesc& light);
			void								createPagedGeometry(TR::Graphics::PagedGeometryDesc &pagedGeometryDesc, const std::vector<TR::Vector3> &copies, const TR::AxisAlignedBox &boundaries);
			void								createParticleSystem(const std::string systemName, const std::string &templateName);
			TR::Scene::Node*				createSceneNode(const TR::Scene::NodeDesc &nodeDesc);
			//void								createViewport(const std::wstring &cameraName, TR::Graphics::ColorValue backgroundColor = TR::Graphics::ColorValue(0.0, 0.0, 0.0)) {  }
			TR::Entity::Sprite*			createSprite(TR::Entity::SpriteDesc &spriteDesc);
			void								createWaterPlane(const TR::Graphics::WaterPlaneDesc &waterPlaneDesc);
			//void								destroyCamera(const std::wstring &cameraName) {  }
			//void								destroyLight(const std::wstring &lightName) {  }
			void								destroyParticleSystem(const std::string systemName);
			void								destroySceneNode(const std::string &nodeName);
			void								destroySprite(TR::Entity::Sprite *sprite);
			//void								destroyViewport(const std::wstring &cameraName) {  }
			void								detachParticleSystemFromNode(const std::string &systemName, const std::string &nodeName);
			TR::Vector3						getCameraDirection(const std::string &cameraName);
			TR::Quaternion					getCameraOrientation(const std::string &cameraName);
			TR::Vector3						getCameraWorldPosition(const std::string &cameraName);
			void								getMeshInfo(const std::string meshName, TR::Graphics::MeshInfo *meshInfo);
			TR::Scene::Node*				getRootNode();
			size_t							getWindowHandle();
			TR::Vector3						getWorldPosition(TR::Scene::Node *node);
			bool								initialize(TR::Framework *framework);
			void								loadResources(const std::string &file);
			//void								setConfigOption(const std::wstring &name, const std::wstring &value) {  }
			void								pitchSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL);
			void								renderFrame(); 
			void								rollSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL);
			void								setAmbientLight(const TR::Graphics::ColorValue &ambientColor);
			void								setFog(const TR::Graphics::ColorValue &color, TR::Graphics::FogMode fogMode, TR::Real expDensity = 0.0001, TR::Real linearStart = 0.0, TR::Real linearEnd = 1.0);
			void								setInheritOrientation(const std::string &nodeName, bool inherit);
			void								setLightVisibility(const std::string &lightName, bool visible);
			void								setObjectVisibility(const std::string &objectName, bool visible);
			void								setSceneManager(TR::Scene::SceneType type);
			void								setSceneNodeDirection(const std::string &nodeName, TR::Vector3 direction);
			void								setSceneNodeOrientation(const std::string &nodeName, const TR::Quaternion &orientation);
			void								setSceneNodePosition(const std::string &nodeName, const TR::Vector3 &position);
			void								setSceneNodeScale(const std::string &nodeName, const TR::Vector3 &scale);
			void								setShadowColor(const TR::Graphics::ColorValue &color);
			void								setSkyBox(const std::string &materialName);
			//void								setVideoResolution(int width, int height, int bpp) {  }
			void								translateCamera(const std::string &cameraName, TR::Vector3 offset, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL);
			void								translateSceneNode(TR::Scene::Node *node, TR::Vector3 offset, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL);
			TR::Quaternion					yawSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL);

		private:
			//bool								frameStarted(const Ogre::FrameEvent& evt) { return true; }
			//bool								frameEnded(const Ogre::FrameEvent& evt) { return true; }

			private:
			TR::Framework					*mFramework;
			TR::Gui::CeguiManager		*mGuiManager;
			Ogre::Root						*mRoot;
			TR::Scene::Node				*mRootNode;
			Ogre::SceneManager			*mSceneManager;
			Ogre::Plane						mWaterPlane;
			WaterRefractionListener		*mWaterRefractionListener;
			WaterReflectionListener		*mWaterReflectionListener;
			Ogre::RenderWindow			*mWindow;
			
			std::map<std::string, PagedGeometry*> mPagedGeometries;
			std::map<std::string, PagedGeometry*>::iterator mPagedGeometriesIterator;
		};
	}
}

#endif
