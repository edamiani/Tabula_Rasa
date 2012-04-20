#ifndef __TR_NOVODEX_MATERIAL__
#define __TR_NOVODEX_MATERIAL__

#include "..\..\..\include\PhysicsMaterial.h"
#include "NxPhysics.h"

namespace TR
{
	namespace Physics
	{
		class NovodexMaterial : public TR::Physics::Material
		{
		public:
								NovodexMaterial(const MaterialDesc &material);
			virtual			~NovodexMaterial() {  }

			TR::Real			getDynamicFriction() { return mDynamicFriction; }
			TR::uint16		getIndex() { return mIndex; }
			std::string		getName() { return mName; }
			TR::Real			getRestitution() { return mRestitution; }
			TR::Real			getStaticFriction() { return mStaticFriction; }
			void				setDynamicFriction(TR::Real dynamicFriction);
			void				setIndex(TR::uint16 index) { mIndex = index; }
			void				setName(const std::string &name);
			void				setRestitution(TR::Real restitution);
			void				setStaticFriction(TR::Real staticFriction);

			NxMaterial*		_getNxMaterial() { return mNxMaterial; }
			void				_setNxMaterial(NxMaterial *nxMaterial) { mNxMaterial = nxMaterial; }

		protected:
			TR::Real		mDynamicFriction;
			TR::uint16	mIndex;
			std::string	mName;
			TR::Real		mRestitution;
			TR::Real		mStaticFriction;

			NxMaterial	*mNxMaterial;
		};
	}
}

#endif