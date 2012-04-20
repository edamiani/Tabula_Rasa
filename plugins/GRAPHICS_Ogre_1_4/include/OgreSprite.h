#ifndef __TR_ENTITY_OGRESPRITE__
#define __TR_ENTITY_OGRESPRITE__

#include "OgreManager.h"
#include "../../../include/EntitySprite.h"

namespace TR
{
	namespace Entity
	{
		struct AnimationState
		{
			bool						autoplay;
			Ogre::AnimationState *ogreAnimationState;

			AnimationState() : autoplay(true), ogreAnimationState(0) {  }
		};

		class OgreSprite : public TR::Entity::Sprite
		{
		public:
												OgreSprite(const TR::Entity::SpriteDesc &spriteDesc);
												~OgreSprite() { mOgreEntity = 0; mAnimationStates.clear(); }

			void								disableAnimation(const std::string &animName);
			void								enableAnimation(const std::string &animName, bool autoPlay = true, bool loop = true, TR::Real weight = 1.0, TR::Real timePosition = 0.0);
			TR::Vector3						getBoneDirection(const std::string &boneName);
			TR::Vector3						getBonePosition(const std::string &boneName);
			const TR::AxisAlignedBox&	getBoundingBox() { return mBoundingBox; }
			TR::Real							getBoundingRadius() { return mBoundingRadius; }
			std::string						getName() { return mName; }
			TR::Scene::Node*				getNode() { return mNode; }
			TR::Quaternion					getOrientation() { return mOrientation; };
			TR::Vector3						getPosition() { return mPosition; };
			TR::Vector3						getScale() { return mScale; };
			TR::Entity::Object3dType	getType() { return mType; };
			TR::Vector3						getWorldPosition();
			void								playAnimationManually(const std::string &animName, TR::Real timeInSeconds);
			void								setAnimationBlendMode(AnimationBlendMode blendMode);
			void								setOrientation(const TR::Quaternion &orientation);
			void								setPosition(const TR::Vector3 &position);
			void								setScale(const TR::Vector3 &scale);
			void								setVisible(bool isVisible) { mIsVisible = isVisible; mOgreEntity->setVisible(isVisible); }
			void								translate(const TR::Vector3 &offset);
			void								updateAnimation(TR::Real time);

			Ogre::Entity*					_getOgreEntity() { return mOgreEntity; }
			void								_setOgreEntity(Ogre::Entity *ogreEntity);

		private:
			TR::AxisAlignedBox			mBoundingBox;
			TR::Real							mBoundingRadius;
			std::string						mCurrentAnimation;
			bool								mCurrentAnimationAutoPlay;
			std::string						mIdleAnimation;
			bool								mIsVisible;
			std::string						mMesh;
			std::string						mName;
			TR::Scene::Node*				mNode;
			TR::Quaternion					mOrientation;
			TR::Vector3						mPosition;
			TR::Vector3						mScale;
			TR::Entity::Object3dType	mType;

			Ogre::Entity					*mOgreEntity;

			std::map<std::string, TR::Entity::AnimationState *>				mAnimationStates;
			std::list<std::string>					mEnabledAnimationStates;
			std::list<std::string>::iterator		mEnabledAnimationStatesIterator;
		};
	}
}

#endif