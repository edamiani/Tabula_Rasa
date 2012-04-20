#ifndef __TR_GRAPHICS_LIGHT__
#define __TR_GRAPHICS_LIGHT__

#include "Dependencies.h"
#include "GraphicsColorValue.h"

namespace TR
{
	namespace Scene
	{
		enum LightType
		{
			LT_POINT = 0,
			LT_DIRECTIONAL,
			LT_SPOT
		};

		struct LightDesc
		{
		public:
			TR::Graphics::ColorValue	ambient;
			TR::Real							attenuationConstant;
			TR::Real							attenuationLinear;
			TR::Real							attenuationQuadratic;
			TR::Real							attenuationRange;
			TR::Graphics::ColorValue	diffuse;
			TR::Vector3						direction;
			std::string						name;
			TR::Scene::Node*				node;
			TR::Vector3						position;
			bool								shadowCaster;
			TR::Graphics::ColorValue	specular;
			TR::Scene::LightType			type;
			bool								visible;

			LightDesc() 
			{
				ambient = TR::Graphics::ColorValue::Black;
				attenuationConstant = 1.0f;
				attenuationLinear = 0.0f;
				attenuationQuadratic = 0.0f;
				attenuationRange = 100000;
				diffuse = TR::Graphics::ColorValue::Black;
				direction = TR::Vector3::NEGATIVE_UNIT_Y;
				node = 0;
				position = TR::Vector3::ZERO;
				shadowCaster = true;
				specular = TR::Graphics::ColorValue::Black;
				type = TR::Scene::LT_POINT;
				visible = true;
			}
		};
	}
}

#endif