#ifndef __TR_OBJECT3D_SPRITE__
#define __TR_OBJECT3D_SPRITE__

#include "Dependencies.h"
#include "Quaternion.h"
#include "Vector3.h"
#include "EntityEnumerators.h"

namespace TR
{
	namespace Entity
	{
		struct Object3dDesc
		{
		public:
			std::string						name;
			TR::Scene::Node*				node;
			TR::Quaternion					orientation;
			TR::Vector3						position;
			TR::Vector3						scale;
			TR::Entity::Object3dType	type;

			Object3dDesc() 
			{  
				node = 0;
				orientation = TR::Quaternion::IDENTITY;
				position = TR::Vector3::ZERO;
				scale = TR::Vector3::UNIT_SCALE;
			}

			virtual ~Object3dDesc() {  }
		};

		class Object3d
		{
		public:
			Object3d()
			{  
				mNode = 0;
				mOrientation = TR::Quaternion::IDENTITY;
				mPosition = TR::Vector3::ZERO;
				mScale = TR::Vector3::UNIT_SCALE;
			}

			virtual ~Object3d() {  }

			std::string						getName() { return mName; }
			TR::Scene::Node*				getNode() { return mNode; }
			TR::Quaternion					getOrientation() { return mOrientation; };
			TR::Vector3						getPosition() { return mPosition; };
			TR::Vector3						getScale() { return mScale; };
			TR::Entity::Object3dType	getType() { return mType; };
			void								setOrientation(const TR::Quaternion &orientation) { mOrientation = orientation; };
			void								setPosition(const TR::Vector3 &position) { mPosition = position; };
			void								setScale(const TR::Vector3 &scale) { mScale = scale; };

		protected:
			std::string						mName;
			TR::Scene::Node*				mNode;
			TR::Quaternion					mOrientation;
			TR::Vector3						mPosition;
			TR::Vector3						mScale;
			TR::Entity::Object3dType	mType;
		};
	}
}

#endif