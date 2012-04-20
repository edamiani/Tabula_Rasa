#include "..\include\SceneNode.h"
#include "..\include\Framework.h"
#include "..\include\GraphicsManager.h"

namespace TR
{
	namespace Scene
	{
		Node::Node(const TR::Scene::NodeDesc &nodeDesc, TR::Graphics::Manager *graphicsManager)
		{
			mGraphicsManager = graphicsManager;
			mName = nodeDesc.name;
			mOrientation = nodeDesc.orientation;
			mParent = nodeDesc.parent;
			mPosition = nodeDesc.position;
			mScale = nodeDesc.scale;
		}

		void Node::attachTo(TR::Scene::Node *parentNode)
		{
			mGraphicsManager->attachSceneNode(this, parentNode);
		}

		void Node::pitch(TR::Degree degree) 
		{ 
			mGraphicsManager->pitchSceneNode(mName, degree); 
		}

		void Node::roll(TR::Degree degree) 
		{ 
			mGraphicsManager->rollSceneNode(mName, degree); 
		}

		void Node::setOrientation(const TR::Quaternion &quaternion) 
		{ 
			mOrientation = quaternion;
			mGraphicsManager->setSceneNodeOrientation(mName, quaternion);
		}

		void Node::setPosition(const TR::Vector3 &position) 
		{ 
			mPosition = position;
			mGraphicsManager->setSceneNodePosition(mName, position);
		}

		void Node::setScale(const TR::Vector3 &scale) 
		{ 
			mScale = scale; 
			mGraphicsManager->setSceneNodeScale(mName, scale);
		}

		void Node::yaw(TR::Degree degree) 
		{ 
			mOrientation = mGraphicsManager->yawSceneNode(mName, degree); 
		}
	}
}