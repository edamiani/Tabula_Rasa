#include "..\include\GraphicsColorValue.h"

namespace TR 
{
	namespace Graphics
	{
		 const ColorValue ColorValue::ZERO = ColorValue(0.0,0.0,0.0,0.0);
		 const ColorValue ColorValue::Black = ColorValue(0.0,0.0,0.0);
		 const ColorValue ColorValue::White = ColorValue(1.0,1.0,1.0);
		 const ColorValue ColorValue::Red = ColorValue(1.0,0.0,0.0);
		 const ColorValue ColorValue::Green = ColorValue(0.0,1.0,0.0);
		 const ColorValue ColorValue::Blue = ColorValue(0.0,0.0,1.0);

		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 ABGR ColorValue::getAsABGR(void) const
	#else
		 RGBA ColorValue::getAsRGBA(void) const
	#endif
		 {
			  uint8 val8;
			  uint32 val32 = 0;

			  // Convert to 32bit pattern
			  // (RGBA = 8888)

			  // Red
			  val8 = static_cast<uint8>(r * 255);
			  val32 = val8 << 24;

			  // Green
			  val8 = static_cast<uint8>(g * 255);
			  val32 += val8 << 16;

			  // Blue
			  val8 = static_cast<uint8>(b * 255);
			  val32 += val8 << 8;

			  // Alpha
			  val8 = static_cast<uint8>(a * 255);
			  val32 += val8;

			  return val32;
		 }
		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 BGRA ColorValue::getAsBGRA(void) const
	#else
		 ARGB ColorValue::getAsARGB(void) const
	#endif
		 {
			  uint8 val8;
			  uint32 val32 = 0;

			  // Convert to 32bit pattern
			  // (ARGB = 8888)

			  // Alpha
			  val8 = static_cast<uint8>(a * 255);
			  val32 = val8 << 24;

			  // Red
			  val8 = static_cast<uint8>(r * 255);
			  val32 += val8 << 16;

			  // Green
			  val8 = static_cast<uint8>(g * 255);
			  val32 += val8 << 8;

			  // Blue
			  val8 = static_cast<uint8>(b * 255);
			  val32 += val8;


			  return val32;
		 }
		//---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		ARGB ColorValue::getAsARGB(void) const
	#else
		BGRA ColorValue::getAsBGRA(void) const
	#endif
		{
			uint8 val8;
			uint32 val32 = 0;

			// Convert to 32bit pattern
			// (ARGB = 8888)

			// Blue
			val8 = static_cast<uint8>(b * 255);
			val32 = val8 << 24;

			// Green
			val8 = static_cast<uint8>(g * 255);
			val32 += val8 << 16;

			// Red
			val8 = static_cast<uint8>(r * 255);
			val32 += val8 << 8;

			// Alpha
			val8 = static_cast<uint8>(a * 255);
			val32 += val8;


			return val32;
		}
		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 RGBA ColorValue::getAsRGBA(void) const
	#else
		 ABGR ColorValue::getAsABGR(void) const
	#endif
		 {
			  uint8 val8;
			  uint32 val32 = 0;

			  // Convert to 32bit pattern
			  // (ABRG = 8888)

			  // Alpha
			  val8 = static_cast<uint8>(a * 255);
			  val32 = val8 << 24;

			  // Blue
			  val8 = static_cast<uint8>(b * 255);
			  val32 += val8 << 16;

			  // Green
			  val8 = static_cast<uint8>(g * 255);
			  val32 += val8 << 8;

			  // Red
			  val8 = static_cast<uint8>(r * 255);
			  val32 += val8;


			  return val32;
		 }
		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 void ColorValue::setAsABGR(const ABGR val)
	#else
		 void ColorValue::setAsRGBA(const RGBA val)
	#endif
		 {
			  uint32 val32 = val;

			  // Convert from 32bit pattern
			  // (RGBA = 8888)

			  // Red
			  r = ((val32 >> 24) & 0xFF) / 255.0f;

			  // Green
			  g = ((val32 >> 16) & 0xFF) / 255.0f;

			  // Blue
			  b = ((val32 >> 8) & 0xFF) / 255.0f;

			  // Alpha
			  a = (val32 & 0xFF) / 255.0f;
		 }
		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 void ColorValue::setAsBGRA(const BGRA val)
	#else
		 void ColorValue::setAsARGB(const ARGB val)
	#endif
		 {
			  uint32 val32 = val;

			  // Convert from 32bit pattern
			  // (ARGB = 8888)

			  // Alpha
			  a = ((val32 >> 24) & 0xFF) / 255.0f;

			  // Red
			  r = ((val32 >> 16) & 0xFF) / 255.0f;

			  // Green
			  g = ((val32 >> 8) & 0xFF) / 255.0f;

			  // Blue
			  b = (val32 & 0xFF) / 255.0f;
		 }
		//---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		void ColorValue::setAsARGB(const ARGB val)
	#else
		void ColorValue::setAsBGRA(const BGRA val)
	#endif
		{
			uint32 val32 = val;

			// Convert from 32bit pattern
			// (ARGB = 8888)

			// Blue
			b = ((val32 >> 24) & 0xFF) / 255.0f;

			// Green
			g = ((val32 >> 16) & 0xFF) / 255.0f;

			// Red
			r = ((val32 >> 8) & 0xFF) / 255.0f;

			// Alpha
			a = (val32 & 0xFF) / 255.0f;
		}
		 //---------------------------------------------------------------------
	#if TR_ENDIAN == TR_ENDIAN_BIG
		 void ColorValue::setAsRGBA(const RGBA val)
	#else
		 void ColorValue::setAsABGR(const ABGR val)
	#endif
		 {
			  uint32 val32 = val;

			  // Convert from 32bit pattern
			  // (ABGR = 8888)

			  // Alpha
			  a = ((val32 >> 24) & 0xFF) / 255.0f;

			  // Blue
			  b = ((val32 >> 16) & 0xFF) / 255.0f;

			  // Green
			  g = ((val32 >> 8) & 0xFF) / 255.0f;

			  // Red
			  r = (val32 & 0xFF) / 255.0f;
		 }
		 //---------------------------------------------------------------------
		 bool ColorValue::operator==(const ColorValue& rhs) const
		 {
			  return (r == rhs.r &&
					g == rhs.g &&
					b == rhs.b &&
					a == rhs.a);
		 }
		 //---------------------------------------------------------------------
		 bool ColorValue::operator!=(const ColorValue& rhs) const
		 {
			  return !(*this == rhs);
		 }
		//---------------------------------------------------------------------
		void ColorValue::setHSB(Real hue, Real saturation, Real brightness)
		{
			// wrap hue
			if (hue > 1.0f)
			{
				hue -= (int)hue;
			}
			else if (hue < 0.0f)
			{
				hue += (int)hue + 1;
			}
			// clamp saturation / brightness
			saturation = std::min(saturation, (Real)1.0);
			saturation = std::max(saturation, (Real)0.0);
			brightness = std::min(brightness, (Real)1.0);
			brightness = std::max(brightness, (Real)0.0);

			if (brightness == 0.0f)
			{   
				// early exit, this has to be black
				r = g = b = 0.0f;
				return;
			}

			if (saturation == 0.0f)
			{   
				// early exit, this has to be grey

				r = g = b = brightness;
				return;
			}


			Real hueDomain  = hue * 6.0f;
			if (hueDomain >= 6.0f)
			{
				// wrap around, and allow mathematical errors
				hueDomain = 0.0f;
			}
			unsigned short domain = (unsigned short)hueDomain;
			Real f1 = brightness * (1 - saturation);
			Real f2 = brightness * (1 - saturation * (hueDomain - domain));
			Real f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

			switch (domain)
			{
			case 0:
				// red domain; green ascends
				r = brightness;
				g = f3;
				b = f1;
				break;
			case 1:
				// yellow domain; red descends
				r = f2;
				g = brightness;
				b = f1;
				break;
			case 2:
				// green domain; blue ascends
				r = f1;
				g = brightness;
				b = f3;
				break;
			case 3:
				// cyan domain; green descends
				r = f1;
				g = f2;
				b = brightness;
				break;
			case 4:
				// blue domain; red ascends
				r = f3;
				g = f1;
				b = brightness;
				break;
			case 5:
				// magenta domain; blue descends
				r = brightness;
				g = f1;
				b = f2;
				break;
			}


		}
	}
}

