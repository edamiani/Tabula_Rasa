#ifndef __GRAPHICS_MANAGER__
#define __GRAPHICS_MANAGER__

#include "Dependencies.h"
#include "Singleton.h"
#include "SceneEnumerators.h"

namespace TR
{
	namespace Graphics
	{
		enum FogMode
		{
			FOG_NONE,
			FOG_EXP,
			FOG_EXP2,
			FOG_LINEAR
		};

		enum TransformSpace
		{
			TS_LOCAL,
			TS_PARENT,
			TS_WORLD
		};

		enum DetailLevelType
		{
			DLT_BATCH,
			DLT_IMPOSTOR
		};

		struct DetailLevel
		{
			TR::Real fade;
			TR::Real farDistance;
			TR::Graphics::DetailLevelType type;
		};

		struct MeshInfo
		{
			MeshInfo::MeshInfo()
			{
				faceBuffer = 0;
				indexCount = 0;
				vertexBuffer = 0;
				vertexCount = 0;
			}

			TR::uint32 *faceBuffer;
			TR::uint32 indexCount;
			TR::Vector3 *vertexBuffer;
			TR::uint32 vertexCount;
		};

		struct PagedGeometryDesc
		{
			std::string	cameraName;
			TR::uint32 copies;
			std::list<TR::Graphics::DetailLevel> detailLevels;
			std::string	mesh;
			std::string	name;
			TR::Real		pageSize;
			std::string	placementMesh;
			TR::Vector3 placementMeshPosition;
			TR::Quaternion placementMeshOrientation;
		};

		struct WaterPlaneDesc
		{
			std::string	cameraName;
			TR::Real		depth;
			TR::Vector3 position;
			TR::Real		uTiles;
			TR::Real		vTiles;
			TR::Real		width;
			TR::uint32	xSegments;
			TR::uint32	ySegments;

			WaterPlaneDesc() : depth(100), position(TR::Vector3::ZERO), uTiles(1), vTiles(1), width(100), xSegments(1), ySegments(1) {  }
		};

		class Manager
		{
		public:
															Manager() {  }
			virtual 										~Manager() {  }

			virtual void								attachLightToBone(const std::string &lightName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation = TR::Quaternion::IDENTITY, const TR::Vector3 &offset = TR::Vector3::ZERO) = 0;
			virtual void								attachObjectToBone(const std::string &nodeName, const std::string &bone, const std::string &boneOwner, const TR::Quaternion &orientation = TR::Quaternion::IDENTITY) = 0;
			virtual void								attachParticleSystemToNode(const std::string &systemName, const std::string &nodeName) = 0;
			virtual void								attachSceneNode(TR::Scene::Node *node, TR::Scene::Node *parentNode) = 0;
			virtual void								cameraLookAt(const std::string &cameraName, const std::string &nodeName) = 0;
			virtual TR::Scene::Camera*				createCamera(TR::Scene::CameraDesc &cameraDesc) = 0;
			virtual TR::Gui::Manager*				createGui(const std::string &schemeFile, const std::string &layoutFile) = 0;
			virtual void								createLight(const TR::Scene::LightDesc &lightDesc) = 0;
			virtual void								createPagedGeometry(TR::Graphics::PagedGeometryDesc &pagedGeometryDesc, const std::vector<TR::Vector3> &copies, const TR::AxisAlignedBox &boundaries) = 0;
			virtual void								createParticleSystem(const std::string systemName, const std::string &templateName) = 0;
			virtual TR::Scene::Node*				createSceneNode(const TR::Scene::NodeDesc &nodeDesc) = 0;
			virtual TR::Entity::Sprite*			createSprite(TR::Entity::SpriteDesc &spriteDesc) = 0;
			virtual void								createWaterPlane(const TR::Graphics::WaterPlaneDesc &waterPlaneDesc) = 0;
			virtual void								destroyParticleSystem(const std::string systemName) = 0;
			virtual void								destroySceneNode(const std::string &nodeName) = 0;
			virtual void								destroySprite(TR::Entity::Sprite *sprite) = 0;
			virtual void								detachParticleSystemFromNode(const std::string &systemName, const std::string &nodeName) = 0;
			virtual TR::Vector3						getCameraDirection(const std::string &cameraName) = 0;
			virtual TR::Quaternion					getCameraOrientation(const std::string &cameraName) = 0;
			virtual TR::Vector3						getCameraWorldPosition(const std::string &cameraName) = 0;
			virtual void								getMeshInfo(const std::string meshName, TR::Graphics::MeshInfo *meshInfo) = 0;
			virtual TR::Scene::Node*				getRootNode() = 0;
			virtual size_t								getWindowHandle() = 0;
			virtual TR::Vector3						getWorldPosition(TR::Scene::Node *node) = 0;
			virtual bool								initialize(TR::Framework *framework) = 0;
			virtual void								loadResources(const std::string &file) = 0;
			virtual void								pitchSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL) = 0;
			virtual void								renderFrame() = 0;
			virtual void								rollSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL) = 0;
			virtual void								setAmbientLight(const TR::Graphics::ColorValue &ambientColor) = 0;
			virtual void								setFog(const TR::Graphics::ColorValue &color, TR::Graphics::FogMode fogMode, TR::Real expDensity = 0.0001, TR::Real linearStart = 0.0, TR::Real linearEnd = 1.0) = 0;
			virtual void								setInheritOrientation(const std::string &nodeName, bool inherit) = 0;
			virtual void								setLightVisibility(const std::string &lightName, bool visible) = 0;
			virtual void								setObjectVisibility(const std::string &objectName, bool visible) = 0;
			virtual void								setSceneManager(TR::Scene::SceneType type) = 0;
			virtual void								setSceneNodeDirection(const std::string &nodeName, TR::Vector3 direction) = 0;
			virtual void								setSceneNodeOrientation(const std::string &nodeName, const TR::Quaternion &orientation) = 0;
			virtual void								setSceneNodePosition(const std::string &nodeName, const TR::Vector3 &position) = 0;
			virtual void								setSceneNodeScale(const std::string &nodeName, const TR::Vector3 &scale) = 0;
			virtual void								setShadowColor(const TR::Graphics::ColorValue &color) = 0;
			virtual void								setSkyBox(const std::string &materialName) = 0;
			virtual void								translateCamera(const std::string &cameraName, TR::Vector3 offset, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL) = 0;
			virtual void								translateSceneNode(TR::Scene::Node *node, TR::Vector3 offset, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL) = 0;
			virtual TR::Quaternion					yawSceneNode(const std::string &nodeName, const TR::Degree &degree, TR::Graphics::TransformSpace space = TR::Graphics::TS_LOCAL) = 0;
		};
	}
}

#endif
