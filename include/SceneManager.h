#ifndef __TR_SCENE_MANAGER__
#define __TR_SCENE_MANAGER__

#include "Dependencies.h"
#include "Singleton.h"

namespace TR 
{
	namespace Scene
	{
		class Manager
		{
		public:
												Manager() {  }
			virtual							~Manager() {  }

			virtual TR::Scene::Node*	createSceneNode(const TR::Scene::NodeDesc &nodeDesc) = 0;
			virtual void					destroySceneNode(const std::string &nodeName) = 0;
			virtual TR::Scene::Camera*	getCamera(const std::string &name) = 0;
			virtual void					initialize(TR::Framework *framework) = 0;
			virtual TR::Scene::Node*	getRootSceneNode() = 0;
			virtual TR::Scene::Node*	getSceneNode(const std::string &nodeName) = 0;
			virtual void					loadScene(TR::Xml::Document *scene) = 0;
		};
	}
}

#endif