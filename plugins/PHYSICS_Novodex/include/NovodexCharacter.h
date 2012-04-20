#ifndef __NOVODEX_CHARACTER__
#define __NOVODEX_CHARACTER__

#include "../../../include/EntityCharacter.h"
#include "NovodexManager.h"

namespace TR
{
	namespace Entity
	{
		class NovodexCharacter : public TR::Entity::Character, public NxUserControllerHitReport
		{
		public:
										NovodexCharacter(TR::Entity::Sprite *sprite);
										~NovodexCharacter() { mController = 0; mSprite = 0; }

			void						disableAnimation(const std::string &animName) { mSprite->disableAnimation(animName); }
			void						enableAnimation(const std::string &animName, bool autoPlay = true, bool loop = true, TR::Real weight = 1.0, TR::Real timePosition = 0.0) { mSprite->enableAnimation(animName, autoPlay, loop, weight, timePosition); }
			TR::Vector3				getBoneDirection(const std::string &boneName) { return mSprite->getBoneDirection(boneName); }
			TR::Vector3				getBonePosition(const std::string &boneName) { return mSprite->getBonePosition(boneName); }
			TR::Real					getFallingHeight() { return mFallingHeight; }
			TR::Real					getHeight() { return static_cast<TR::Real>(reinterpret_cast<NxCapsuleController*>(mController)->getHeight()); }
			TR::uint32				getId() { return mId; }
			std::string				getName() { return mSprite->getName(); }
			TR::Quaternion			getOrientation();
			TR::Vector3				getPosition() { return mSprite->getPosition(); }
			TR::Real					getRadius() { return static_cast<TR::Real>(reinterpret_cast<NxCapsuleController*>(mController)->getRadius()); }
			bool						isTouchingGround() { return mTouchingGround; }
			void						jump(TR::Real height, TR::Real jumpTime);
			void						move(const TR::Vector3 &offset);
			void						playAnimationManually(const std::string &animName, TR::Real timeInSeconds) { mSprite->playAnimationManually(animName, timeInSeconds); }
			void						setAnimationBlendMode(AnimationBlendMode blendMode) { mSprite->setAnimationBlendMode(blendMode); }
			void						setId(TR::uint32 id) { mId = id; }
			void						setOrientation(const TR::Quaternion &orientation);
			void						setPosition(const TR::Vector3 &position);
			void						setTouchingGround(bool touching) { mTouchingGround = touching; }
			void						translate(const TR::Vector3 &offset);
			void						update(TR::Real timeInSeconds);
			void						yaw(TR::Degree degree);

			void						_setNxController(NxController *controller) { mController = controller; }

			// Novodex callbacks for characters
			NxControllerAction	onControllerHit(const NxControllersHit& hit);
			NxControllerAction	onShapeHit(const NxControllerShapeHit& hit);

		private:
			TR::uint32				mId;
			NxU32						mCollisionFlags;
			NxController			*mController;
			TR::Real					mFallingHeight;
			TR::Real					mFallingSpeed;
			TR::Real					mJumpSpeed;
			TR::Real					mJumpTime;
			TR::Vector3				mMovementAccumulator;
			TR::Entity::Sprite	*mSprite;
			bool						mStartJumping;
			bool						mTouchingGround;
		};
	}
}

#endif