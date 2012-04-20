#ifndef __TR_SCENE_NODE__
#define __TR_SCENE_NODE__

#include "Dependencies.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace TR 
{
	namespace Scene
	{
		struct NodeDesc
		{
		public:
			NodeDesc() 
			{ 
				orientation = TR::Quaternion::IDENTITY;
				parent = 0;
				position = TR::Vector3::ZERO;
				scale = TR::Vector3::UNIT_SCALE;
			}

			~NodeDesc() {  }

			std::string					name;
			TR::Quaternion				orientation;
			TR::Scene::Node			*parent;
			TR::Vector3					position;
			TR::Vector3					scale;
		};

		class Node
		{
		public:
			Node(const TR::Scene::NodeDesc &nodeDesc, TR::Graphics::Manager *graphicsManager);			
			~Node() { mParent = 0; }

			void							attachTo(TR::Scene::Node *parentNode);
			std::string					getName() { return mName; }
			const TR::Quaternion&	getOrientation() { return mOrientation; }
			TR::Scene::Node*			getParent() { return mParent; }
			const TR::Vector3&		getPosition() { return mPosition; }
			const TR::Vector3&		getScale() { return mScale; }
			void							pitch(TR::Degree degree);
			void							roll(TR::Degree degree);
			void							setOrientation(const TR::Quaternion &quaternion);
			void							setPosition(const TR::Vector3 &position);
			void							setScale(const TR::Vector3 &scale);
			void							yaw(TR::Degree degree);

			void							_setPositionInternal(const TR::Vector3 &position) { mPosition = position; }

		private:
			TR::Graphics::Manager	*mGraphicsManager;
			std::string					mName;
			TR::Quaternion				mOrientation;
			TR::Scene::Node			*mParent;
			TR::Vector3					mPosition;
			TR::Vector3					mScale;
		};
	}
}

#endif