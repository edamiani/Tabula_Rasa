#ifndef __TR_GRAPHICS_ENUM__
#define __TR_GRAPHICS_ENUM__

namespace TR
{
	namespace Scene
	{
		enum CameraMode
		{
			CM_STATIC = 0,
			CM_FLY,
			CM_WALK
		};

		enum SceneType
		{
			ST_DEFAULT = 0,
			ST_BSP,
			ST_OCTREE,
			ST_TERRAIN
		};
	}
}

#endif