#include "../include/OgreSprite.h"
#include "Ogre.h"

namespace TR
{
	namespace Entity
	{
		OgreSprite::OgreSprite(const TR::Entity::SpriteDesc &spriteDesc)
		{
			mOgreEntity = 0;

			mType = spriteDesc.type;
			mName = spriteDesc.name;
			mNode = spriteDesc.node;
			mIdleAnimation = spriteDesc.initialAnimation;
			mIsVisible = spriteDesc.isVisible;
			mPosition = spriteDesc.position;
			mOrientation = spriteDesc.orientation;
			mScale = spriteDesc.scale;
			mMesh = spriteDesc.mesh;
			mBoundingBox = spriteDesc.boundingBox;
			mBoundingRadius = spriteDesc.boundingRadius;
		}

		void OgreSprite::_setOgreEntity(Ogre::Entity *ogreEntity) 
		{ 
			mOgreEntity = ogreEntity; 

			if(mOgreEntity->hasSkeleton())
			{
				for(int i = 0; i < mOgreEntity->getSkeleton()->getNumAnimations(); i++)
				{
					TR::Entity::AnimationState *state = new TR::Entity::AnimationState();
					Ogre::Animation *animation = mOgreEntity->getSkeleton()->getAnimation(i);
					state->ogreAnimationState = mOgreEntity->getAnimationState(animation->getName());
					state->ogreAnimationState->setEnabled(false);
					state->ogreAnimationState->setLoop(true);
					state->ogreAnimationState->setTimePosition(0.0);
					state->ogreAnimationState->setWeight(1.0);
					mAnimationStates[animation->getName()] = state;

					if(animation->getName() == mIdleAnimation)
						enableAnimation(animation->getName());
				}
			}
		}

		void OgreSprite::disableAnimation(const std::string &animName)
		{
			mAnimationStates[animName]->ogreAnimationState->setEnabled(false);
			mEnabledAnimationStates.remove(animName);
		}

		void OgreSprite::enableAnimation(const std::string &animName, bool autoPlay, bool loop, TR::Real weight, TR::Real timePosition)
		{
			bool alreadyExists = false;
			mEnabledAnimationStatesIterator = mEnabledAnimationStates.begin();
			while(mEnabledAnimationStatesIterator != mEnabledAnimationStates.end())
			{
				if((*mEnabledAnimationStatesIterator) == animName)
				{
					alreadyExists = true;
					break;
				}
				mEnabledAnimationStatesIterator++;
			}

			if(!alreadyExists)
			{
				mEnabledAnimationStates.push_back(animName);
				mAnimationStates[animName]->autoplay = autoPlay;
				mAnimationStates[animName]->ogreAnimationState->setEnabled(true);
				mAnimationStates[animName]->ogreAnimationState->setLoop(loop); 
				mAnimationStates[animName]->ogreAnimationState->setTimePosition(timePosition);
				mAnimationStates[animName]->ogreAnimationState->setWeight(weight); 
			}
		}

		TR::Vector3 OgreSprite::getBoneDirection(const std::string &boneName)
		{
			if(mOgreEntity->hasSkeleton())
			{
				Ogre::Quaternion quat = mOgreEntity->getSkeleton()->getBone(boneName)->getWorldOrientation();
				Ogre::Vector3 vec(quat.yAxis());
				return TR::Vector3(vec.x, vec.y, vec.z);
			}
			else
				return TR::Vector3::ZERO;
		}

		TR::Vector3 OgreSprite::getBonePosition(const std::string &boneName)
		{
			if(mOgreEntity->hasSkeleton())
			{
				Ogre::Vector3 vec = mOgreEntity->getSkeleton()->getBone(boneName)->getWorldPosition();
				return TR::Vector3(vec.x, vec.y, vec.z);
			}
			else
				return TR::Vector3::ZERO;
		}

		TR::Vector3 OgreSprite::getWorldPosition()
		{
			Ogre::Vector3 ogreVec = mOgreEntity->getParentNode()->getWorldPosition();
			TR::Vector3 vec(ogreVec.x, ogreVec.y, ogreVec.z);
			return vec;
		}

		void OgreSprite::playAnimationManually(const std::string &animName, TR::Real timeInSeconds)
		{
			mAnimationStates[animName]->ogreAnimationState->setTimePosition(timeInSeconds);
		}

		void OgreSprite::setAnimationBlendMode(AnimationBlendMode blendMode)
		{
			if(mOgreEntity->hasSkeleton())
			{
				if(blendMode == TR::Entity::ANIMBLEND_AVERAGE)
					mOgreEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_AVERAGE);
				else if(blendMode == TR::Entity::ANIMBLEND_CUMULATIVE)
					mOgreEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
			}
		}

		void OgreSprite::setOrientation(const TR::Quaternion &orientation)
		{
			mOgreEntity->getParentNode()->setOrientation(orientation.w, orientation.x, orientation.y, orientation.z);
			mOrientation = orientation;
		}

		void OgreSprite::setPosition(const TR::Vector3 &position)
		{
			mOgreEntity->getParentNode()->setPosition(position.x, position.y, position.z);
			mPosition = position;
		}

		void OgreSprite::setScale(const TR::Vector3 &scale)
		{
			mOgreEntity->getParentNode()->setScale(scale.x, scale.y, scale.z);
			mScale = scale;
		}

		void OgreSprite::translate(const TR::Vector3 &offset) 
		{ 
			mOgreEntity->getParentSceneNode()->translate(Ogre::Vector3(offset.x, offset.y, offset.z), Ogre::Node::TS_LOCAL);
			Ogre::Vector3 newPosition = mOgreEntity->getParentSceneNode()->getPosition();
			mPosition.x = newPosition.x;
			mPosition.y = newPosition.y;
			mPosition.z = newPosition.z;
		}

		void OgreSprite::updateAnimation(TR::Real timeElapsed)
		{
			mEnabledAnimationStatesIterator = mEnabledAnimationStates.begin();
			while(mEnabledAnimationStatesIterator != mEnabledAnimationStates.end())
			{
				if(mAnimationStates[(*mEnabledAnimationStatesIterator)]->autoplay)
					mAnimationStates[(*mEnabledAnimationStatesIterator)]->ogreAnimationState->addTime(timeElapsed);

				mEnabledAnimationStatesIterator++;
			}
		}
	}
}