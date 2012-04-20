#ifndef __TR_ENTITY_SPRITE__
#define __TR_ENTITY_SPRITE__

#include "Dependencies.h"
//#include "EntityManager.h"
//#include "GraphicsManager.h"
#include "EntityObject3d.h"
#include "SceneNode.h"
#include "AxisAlignedBox.h"

namespace TR
{
	namespace Entity
	{
		enum AnimationBlendMode
		{
			ANIMBLEND_AVERAGE,
			ANIMBLEND_CUMULATIVE
		};

		struct SpriteDesc
		{
		public:
			TR::AxisAlignedBox			boundingBox;
			TR::Real							boundingRadius;
			std::string						initialAnimation;
			bool								isVisible;
			std::string						mesh;
			std::string						name;
			TR::Scene::Node*				node;
			TR::Quaternion					orientation;
			TR::Vector3						position;
			TR::Vector3						scale;
			bool								shadowCaster;
			TR::Entity::Object3dType	type;

			SpriteDesc()
			{
				initialAnimation = "";
				isVisible = true;
				node = 0;
				orientation = TR::Quaternion::IDENTITY;
				position = TR::Vector3::ZERO;
				scale = TR::Vector3::UNIT_SCALE;
				shadowCaster = false;
			}

			~SpriteDesc() { node = 0; }
		};

		class Sprite
		{
		public:
															Sprite() {  }
			virtual										~Sprite() {  }

			virtual void								disableAnimation(const std::string &animName) = 0;
			virtual void								enableAnimation(const std::string &animName, bool autoPlay = true, bool loop = true, TR::Real weight = 1.0, TR::Real timePosition = 0.0) = 0;
			virtual TR::Vector3						getBoneDirection(const std::string &boneName) = 0;
			virtual TR::Vector3						getBonePosition(const std::string &boneName) = 0;
			virtual const TR::AxisAlignedBox&	getBoundingBox() = 0;
			virtual TR::Real							getBoundingRadius() = 0;
			virtual std::string						getName() = 0;
			virtual TR::Scene::Node*				getNode() = 0;
			virtual TR::Quaternion					getOrientation() = 0;
			virtual TR::Vector3						getPosition() = 0;
			virtual TR::Vector3						getScale() = 0;
			virtual TR::Entity::Object3dType		getType() = 0;
			virtual TR::Vector3						getWorldPosition() = 0;
			virtual void								playAnimationManually(const std::string &animName, TR::Real timeInSeconds) = 0;
			virtual void								setAnimationBlendMode(AnimationBlendMode blendMode) = 0;
			virtual void								setOrientation(const TR::Quaternion &orientation) = 0;
			virtual void								setPosition(const TR::Vector3 &position) = 0;
			virtual void								setScale(const TR::Vector3 &scale) = 0; 
			virtual void								setVisible(bool isVisible) = 0;
			virtual void								translate(const TR::Vector3 &offset) = 0;
			virtual void								updateAnimation(TR::Real time) = 0;
		};
	}
}

#endif