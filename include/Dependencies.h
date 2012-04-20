#ifndef __TR_DEPENDENCIES__
#define __TR_DEPENDENCIES__

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include "Defines.h"

namespace TR
{
	void	onEnterFrame();

	class Framework;

	class AxisAlignedBox;
	class Degree;
	class Math;
	class Matrix3;
	class Matrix4;
	class Quaternion;
	class Plane;
	class Radian;
	class Ray;
	class Sphere;
	class Timer;
	class Vector2;
	class Vector3;
	class Vector4;

	namespace Entity
	{
		class		Character;
		class		Dynamic;
		class		Kinematic;
		class		Manager;
		class		Object3d;
		struct	Object3dDesc;
		class		Sprite;
		struct	SpriteDesc;
		class		Static;
		class		Trigger;
		class		Vehicle;
		struct	VehicleDesc;
	}

	namespace Graphics
	{
		class		ColorValue;
		class		Manager;
		struct	MeshInfo;
		struct	PagedGeometryDesc;
	}

	namespace Gui
	{
		class		EditBox;
		class		Manager;
		class		StaticImage;
		class		StaticText;
		class		Window;
	}

	namespace Input
	{
		class		Device;
		class		Joystick;
		class		Keyboard;
		struct	KeyEvent;
		class		Manager;
		class		Mouse;
		struct	MouseEvent;
	}

	namespace Physics
	{
		struct	JointDesc;
		struct	Limit;
		class		Manager;
		class		Material;
		struct	MaterialDesc;
		struct	RotationLimits;
		class		Solid;
		struct	SolidDesc;
		struct	TranslationLimits;
		class		Wheel;
		struct	WheelDesc;
	}

	namespace Scene
	{
		class		Camera;
		struct	CameraDesc;
		class		CameraStatic;
		struct	CameraStaticDesc;
		struct	LightDesc;
		class		Manager;
		class		Node;
		struct	NodeDesc;
	}

	namespace Xml
	{
		class		Document;
		class		Element;
		class		Manager;
	}
}

#endif
