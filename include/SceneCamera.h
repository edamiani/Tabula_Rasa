#ifndef __TR_SCENE_CAMERA__
#define __TR_SCENE_CAMERA__

#include "Dependencies.h"
#include "SceneEnumerators.h"

namespace TR
{
	namespace Scene
	{
		struct Viewport
		{
			TR::Real height;
			TR::Real left;
			TR::Real top;
			TR::Real width;
			int zOrder;
		};

		struct CameraDesc
 		{
		public:
			TR::Real						farClipDistance;
			bool							isActive;
			TR::Scene::CameraMode	mode;
			std::string					name;
			TR::Real						nearClipDistance;
			TR::Scene::Node			*parentNode;
			TR::Scene::Viewport		viewport;

			CameraDesc() { parentNode = 0; }
		};

		class Camera
		{
		public:
														Camera() {  }
			virtual									~Camera() {  }

			TR::Real									getFarClipDistance() { return mFarClipDistance; }
			TR::Scene::CameraMode				getMode() { return mMode; }
			const std::string&					getName() { return mName; }
			TR::Real									getNearClipDistance() { return mNearClipDistance; }
			virtual const TR::Vector3&			getDirection() = 0;
			TR::Scene::Node*						getParentNode() { return mParentNode; }
			virtual const TR::Vector3&			getPosition() = 0;
			bool										isActive() { return mIsActive; }
			void										setActive (bool isActive) { mIsActive = isActive; }
			void										setFarClipDistance(TR::Real farClipDistance) { mFarClipDistance = farClipDistance; }
			void										setMode(TR::Scene::CameraMode mode) { mMode = mode; }
			void										setName(const std::string& name) { mName = name; }
			void										setNearClipDistance(TR::Real nearClipDistance) { mNearClipDistance = nearClipDistance; }
			virtual void							setDirection(const TR::Vector3 &direction) = 0;
			void										setParentNode(TR::Scene::Node *parentNode) { mParentNode = parentNode; }
			virtual void							setPosition(const TR::Vector3 &position) = 0;

		protected:
			TR::Real									mFarClipDistance;
			bool										mIsActive;
			TR::Scene::CameraMode				mMode;
			std::string								mName;
			TR::Real									mNearClipDistance;
			TR::Scene::Node						*mParentNode;
			TR::Scene::Viewport					mViewport;
		};
	}
}

#endif