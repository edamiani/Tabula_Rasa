#include "..\include\NovodexMaterial.h"

namespace TR
{
	namespace Physics
	{
		NovodexMaterial::NovodexMaterial(const MaterialDesc &material)
			: Material(material)
		{  
			mDynamicFriction = material.dynamicFriction;
			mIndex = 65535;
			mName = material.name;
			mRestitution = material.restitution;
			mStaticFriction = material.staticFriction;
		}

		void NovodexMaterial::setDynamicFriction(TR::Real dynamicFriction)
		{
			mDynamicFriction = dynamicFriction;
			mNxMaterial->setDynamicFriction(static_cast<NxReal>(dynamicFriction));
		}

		void NovodexMaterial::setName(const std::string &name)
		{
			mName = name;
		}

		void NovodexMaterial::setRestitution(TR::Real restitution)
		{
			mRestitution = restitution;
			mNxMaterial->setRestitution(static_cast<NxReal>(restitution));
		}

		void NovodexMaterial::setStaticFriction(TR::Real staticFriction)
		{
			mStaticFriction = staticFriction;
			mNxMaterial->setStaticFriction(static_cast<NxReal>(staticFriction));
		}
	}
}