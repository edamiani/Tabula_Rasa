#ifndef __TR_SCENE_TRMANAGER__
#define __TR_SCENE_TRMANAGER__

#include "../../../include/SceneManager.h"

namespace TR 
{
	namespace Scene
	{
		class TRManager : public TR::Scene::Manager
		{
		public:
										TRManager() { mRootSceneNode = 0; }
										~TRManager();

			TR::Scene::Node*		createSceneNode(const TR::Scene::NodeDesc &nodeDesc);
			void						destroySceneNode(const std::string &nodeName);
			TR::Scene::Camera*	getCamera(const std::string &name);
			TR::Scene::Node*		getRootSceneNode() { return mRootSceneNode; }
			TR::Scene::Node*		getSceneNode(const std::string &nodeName);
			void						initialize(TR::Framework *framework) { mFramework = framework; }
			void						loadScene(TR::Xml::Document *scene);
			
			void						_loadCamera(TR::Xml::Element *camera, TR::Scene::Node *parentNode);
			void						_loadEntity(TR::Xml::Element *entity, TR::Scene::Node *parent);
			void						_loadEnvironment(TR::Xml::Element *environment);
			void						_loadGlobalElements(TR::Xml::Element *scene);
			void						_loadJoint(TR::Xml::Element *joint);
			void						_loadLight(TR::Xml::Element *light, TR::Scene::Node *parentNode);
			void						_loadMaterial(TR::Xml::Element *material);
			void						_loadPagedGeometry(TR::Xml::Element *pagedGeometry);
			TR::Scene::Node*		_loadSceneNode(TR::Xml::Element *node, TR::Scene::Node *parentNode);
			void						_loadSprite(TR::Xml::Element *sprite, TR::Scene::Node *parentNode);
			void						_loadVehicle(TR::Xml::Element *vehicle, TR::Scene::Node *parentNode);
			void						_parseNodeTree(TR::Xml::Element *node, TR::Scene::Node *parentNode = 0);

		private:
			TR::Scene::Node													*mRootSceneNode;
			std::map<std::string, TR::Scene::Camera*>					mCameras;
			std::map<std::string, TR::Scene::Camera*>::iterator	mCamerasIterator;
			TR::Framework														*mFramework;
			std::map<std::string, TR::Scene::Node*>					mSceneNodes;
			std::map<std::string, TR::Scene::Node*>::iterator		mSceneNodesIterator;
		};
	}
}

#endif