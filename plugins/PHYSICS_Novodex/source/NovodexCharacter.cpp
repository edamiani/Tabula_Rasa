#include "NxController.h"
#include "ControllerManager.h"
#include "NxCapsuleController.h"
#include "../include/NovodexCharacter.h"
#include "../../../include/SceneNode.h"

namespace TR
{
	namespace Entity
	{
		NovodexCharacter::NovodexCharacter(TR::Entity::Sprite *sprite) 
		{  
			mId = 0;
			mCollisionFlags = 0;
			mController = 0;
			mFallingHeight = 0.0;
			mFallingSpeed = 0.0;
			mJumpSpeed = 0.0;
			mJumpTime = 0.0;
			mMovementAccumulator = TR::Vector3::ZERO;
			mSprite = sprite;
			mStartJumping = false;
			mTouchingGround = false;
		}

		TR::Quaternion NovodexCharacter::getOrientation() 
		{ 
			NxQuat nxQuat = mController->getActor()->getGlobalOrientationQuat();
			TR::Quaternion quat;
			quat.x = nxQuat.x;
			quat.y = nxQuat.y;
			quat.z = nxQuat.z;
			quat.w = nxQuat.w;
			return quat;
		}

		void NovodexCharacter::jump(TR::Real height, TR::Real jumpTime)
		{
			if(mTouchingGround)
			{
				mFallingHeight = 0.0;
				mJumpTime = 0.0;
				TR::Real gravity = -9.8;
				TR::Real halfTime = jumpTime / 2.0;
				mJumpSpeed = (height - ((gravity / 2.0) * halfTime * halfTime)) / halfTime;
				mStartJumping = true;
			}
		}

		void NovodexCharacter::move(const TR::Vector3 &offset) 
		{ 
			NxVec3 displacement(offset.x, offset.y, offset.z);
			//if(mCollisionFlags == 0)
				mController->move(displacement, 2, 0.01, mCollisionFlags);
			NxExtendedVec3 position = mController->getPosition();
			mSprite->setPosition(TR::Vector3(position.x, position.y, position.z));
		}

		NxControllerAction NovodexCharacter::onControllerHit(const NxControllersHit& hit)
		{
			return NX_ACTION_NONE;
		}

		NxControllerAction NovodexCharacter::onShapeHit(const NxControllerShapeHit& hit)
		{
			if((int)(hit.shape->getActor().userData) == 10000)
			{
				/*NxExtendedVec3 offset(0.0, (getHeight() / 2.0) - (mController->getPosition().y - hit.worldPos.y), 0.0);
				//NxExtendedVec3 nxPos(mController->getPosition());
				//nxPos.y += 5.0;
				//NxVec3 nxPos(mController->getPosition().x, hit.worldPos.y + 1.0 + (getHeight() / 2), mController->getPosition().z);
				//mController->getActor()->moveGlobalPosition(nxPos);
				//mController->move(offset, 1 << 1, 0.01, mCollisionFlags);
				//NxExtendedVec3 nxPos = mController->getPosition();
				//mController->setPosition(nxPos);
				TR::Vector3 position(nxPos.x, nxPos.y, nxPos.z);
				mSprite->setPosition(position);
				mMovementAccumulator = TR::Vector3::ZERO;
				mFallingSpeed = 0.0;
				mFallingHeight = 0.0;
				mTouchingGround = true;
				mJumpSpeed = 0.0;
				mJumpTime = 0.0;*/
			}
			else if(hit.dir.y < 0.0 && !mStartJumping)
			{
				mFallingSpeed = 0.0;
				mFallingHeight = 0.0;
				mTouchingGround = true;
				mJumpSpeed = 0.0;
				mJumpTime = 0.0;
			}
			return NX_ACTION_NONE;
		}

		void NovodexCharacter::setOrientation(const TR::Quaternion &orientation)
		{
			mSprite->setOrientation(orientation);
			TR::Quaternion quat = mSprite->getNode()->getOrientation();
			NxQuat nxQuat;
			nxQuat.x = quat.x;
			nxQuat.y = quat.y;
			nxQuat.z = quat.z;
			nxQuat.w = quat.w;
			mController->getActor()->setGlobalOrientationQuat(nxQuat);
		}

		void NovodexCharacter::setPosition(const TR::Vector3 &position)
		{
			mController->setPosition(NxExtendedVec3(position.x, position.y, position.z));
			mSprite->setPosition(position);
		}

		void NovodexCharacter::translate(const TR::Vector3 &offset)
		{
			mMovementAccumulator = mMovementAccumulator + offset;
		}

		void NovodexCharacter::update(TR::Real timeInSeconds)
		{
			TR::Real currentFalling = 0.0;

			if(mStartJumping)
			{
				TR::Real oldHeight = mFallingHeight;
				mJumpTime = timeInSeconds;
				mFallingHeight = mJumpSpeed * mJumpTime + (-4.9 * mJumpTime * mJumpTime);
				currentFalling = mFallingHeight - oldHeight;
				mMovementAccumulator.y = currentFalling;
				mStartJumping = false;
				mTouchingGround = false;
			}
			else
			{
				if(mJumpSpeed != 0.0)
				{
					TR::Real oldHeight = mFallingHeight;
					mJumpTime += timeInSeconds;
					mFallingHeight = mJumpSpeed * mJumpTime + (-4.9 * mJumpTime * mJumpTime);
					currentFalling = mFallingHeight - oldHeight;
					mMovementAccumulator.y = currentFalling;
				}
				else
				{
					mFallingSpeed += -9.8 * timeInSeconds;
					currentFalling = mFallingSpeed * timeInSeconds + (-4.9 * timeInSeconds * timeInSeconds);
					mFallingHeight += currentFalling;
					mMovementAccumulator.y = currentFalling;
				}
			}

			mController->reportSceneChanged();

			TR::Vector3 oldPos = mSprite->getPosition();
			mSprite->translate(mMovementAccumulator);
			TR::Vector3 newPos = mSprite->getPosition();
			TR::Vector3 newOffset = newPos - oldPos;
			NxVec3 displacement(newOffset.x, newOffset.y, newOffset.z);
			mController->move(displacement, 1 << 1, 0.01, mCollisionFlags);
			//NxVec3 nxPos = mController->getActor()->getPosition();
			NxExtendedVec3 nxPos = mController->getPosition();
			TR::Vector3 position(nxPos.x, nxPos.y, nxPos.z);
			mSprite->setPosition(position);
			mMovementAccumulator = TR::Vector3::ZERO;
		}

		void NovodexCharacter::yaw(TR::Degree degree)	
		{
			mSprite->getNode()->yaw(degree);
			TR::Quaternion quat = mSprite->getNode()->getOrientation();
			NxQuat nxQuat;
			nxQuat.x = quat.x;
			nxQuat.y = quat.y;
			nxQuat.z = quat.z;
			nxQuat.w = quat.w;
			mController->getActor()->setGlobalOrientationQuat(nxQuat);
		}
	}
}