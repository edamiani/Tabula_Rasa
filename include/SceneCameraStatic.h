#ifndef __TR_SCENE_CAMERA_STATIC__
#define __TR_SCENE_CAMERA_STATIC__

#include "Dependencies.h"
#include "SceneEnumerators.h"
#include "SceneCamera.h"

namespace TR
{
	namespace Scene
	{
		struct CameraStaticDesc : public TR::Scene::CameraDesc
		{
		public:
			TR::Vector3					direction;
			TR::Vector3					position;

			CameraStaticDesc()
			{
				direction = TR::Vector3::NEGATIVE_UNIT_Z;
				position = TR::Vector3::ZERO;
			}
		};

		class CameraStatic : public TR::Scene::Camera
		{
		public:
			CameraStatic(const TR::Scene::CameraStaticDesc &cameraStaticDesc)
			{
				mFarClipDistance = cameraStaticDesc.farClipDistance;
				mIsActive = cameraStaticDesc.isActive;
				mMode = cameraStaticDesc.mode;
				mName = cameraStaticDesc.name;
				mNearClipDistance = cameraStaticDesc.nearClipDistance;
				mDirection = cameraStaticDesc.direction;
				mParentNode = cameraStaticDesc.parentNode;
				mPosition = cameraStaticDesc.position;
			}

			~CameraStatic() {  }

			const TR::Vector3&		getDirection() { return mDirection; }
			const TR::Vector3&		getPosition() { return mPosition; }
			void							setDirection(const TR::Vector3 &direction) { mDirection = direction; }
			void							setPosition(const TR::Vector3 &position) { mPosition = position; }

		protected:
			TR::Vector3					mDirection;
			TR::Vector3					mPosition;
		};
	}
}

#endif