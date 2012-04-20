#include "../include/TREntityManager.h"
#include "../../../include/EntityDynamic.h"
#include "../../../include/EntityKinematic.h"
#include "../include/TREntitySprite.h"
#include "../../../include/EntityStatic.h"

namespace TR
{
	namespace Entity
	{
		TRSprite::TRSprite(TR::Framework *framework, TR::Entity::SpriteDesc &spriteDesc)
		{
			mType = spriteDesc.type;
			mName = spriteDesc.name;
			mNode = spriteDesc.node;
			mIsVisible = spriteDesc.isVisible;
			mPosition = spriteDesc.position;
			mOrientation = spriteDesc.orientation;
			mScale = spriteDesc.scale;
			mMesh = spriteDesc.mesh;

			// createVisualObject() must fill 'spriteDesc.boundingBox' and 'spriteDesc.boundingRadius'
			framework->getGraphicsManager()->createVisualObject(spriteDesc);
			mBoundingBox = spriteDesc.boundingBox;
			mBoundingRadius = spriteDesc.boundingRadius;
		}
	}
}