#ifndef __TR_ENTITY_CHARACTER__
#define __TR_ENTITY_CHARACTER__

#include "Defines.h"
#include "Dependencies.h"
#include "EntitySprite.h"

namespace TR
{
	namespace Entity
	{
		class Character
		{
		public:
													Character() {  }
			virtual								~Character() {  }

			virtual void						disableAnimation(const std::string &animName) = 0;
			virtual void						enableAnimation(const std::string &animName, bool autoPlay = true, bool loop = true, TR::Real weight = 1.0, TR::Real timePosition = 0.0) = 0;
			virtual TR::Vector3				getBoneDirection(const std::string &boneName) = 0;
			virtual TR::Vector3				getBonePosition(const std::string &boneName) = 0;
			virtual TR::uint32				getId() = 0;
			virtual TR::Real					getFallingHeight() = 0;
			virtual TR::Real					getHeight() = 0;
			virtual std::string				getName() = 0;
			virtual TR::Vector3			 	getPosition() = 0;
			virtual TR::Quaternion		 	getOrientation() = 0;
			virtual TR::Real					getRadius() = 0;
			virtual bool						isTouchingGround() = 0;
			virtual void						jump(TR::Real height, TR::Real jumpTime) = 0;
			virtual void						move(const TR::Vector3 &offset) = 0;
			virtual void						playAnimationManually(const std::string &animName, TR::Real timeInSeconds) = 0;
			virtual void						setAnimationBlendMode(AnimationBlendMode blendMode) = 0;
			virtual void						setId(TR::uint32 id) = 0;
			virtual void						setPosition(const TR::Vector3 &position) = 0;
			virtual void						setOrientation(const TR::Quaternion &orientation) = 0;
			virtual void						setTouchingGround(bool touching) = 0;
			virtual void						translate(const TR::Vector3 &offset) = 0;
			virtual void						update(TR::Real timeInSeconds) = 0;
			virtual void						yaw(TR::Degree degree) = 0;
		};
	}
}

#endif