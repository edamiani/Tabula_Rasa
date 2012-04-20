#ifndef __TR_ENTITY_TRSPRITE__
#define __TR_ENTITY_TRSPRITE__

#include "../../../include/EntitySprite.h"

namespace TR
{
	namespace Entity
	{
		class TRSprite : public TR::Entity::Sprite
		{
		public:
												TRSprite(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc);
												~TRSprite() {  }

			const TR::AxisAlignedBox&	getBoundingBox() { return mBoundingBox; }
			TR::Real							getBoundingRadius() { return mBoundingRadius; }
			void								setVisible(bool isVisible) { mIsVisible = isVisible; }

		private:
			TR::AxisAlignedBox	mBoundingBox;
			TR::Real					mBoundingRadius;
			bool						mIsVisible;
			std::string				mMesh;
		};
	}
}

#endif