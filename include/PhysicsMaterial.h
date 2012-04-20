#ifndef __TR_PHYSICS_MATERIAL__
#define __TR_PHYSICS_MATERIAL__

#include "Dependencies.h"

namespace TR
{
	namespace Physics
	{
		struct MaterialDesc
		{
			TR::Real		dynamicFriction;
			std::string	name;
			TR::Real		restitution;
			TR::Real		staticFriction;
		};

		class Material
		{
		public:
										Material(const MaterialDesc &material) {  }
			virtual					~Material() {  }

			virtual TR::Real		getDynamicFriction() = 0 ;
			virtual TR::uint16	getIndex() = 0 ;
			virtual std::string	getName() = 0 ;
			virtual TR::Real		getRestitution() = 0 ;
			virtual TR::Real		getStaticFriction() = 0 ;
			virtual void			setDynamicFriction(TR::Real dynamicFriction) = 0;
			virtual void			setIndex(TR::uint16 index) = 0;
			virtual void			setName(const std::string &name) = 0;
			virtual void			setRestitution(TR::Real restitution) = 0;
			virtual void			setStaticFriction(TR::Real staticFriction) = 0;
		};
	}
}

#endif