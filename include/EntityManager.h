#ifndef __TR_ENTITY_MANAGER__
#define __TR_ENTITY_MANAGER__

#include "Dependencies.h"
#include "Singleton.h"

namespace TR
{
	namespace Entity
	{
		class Manager
		{
		public:
															Manager() {  }
			virtual										~Manager() {  }
			
			virtual TR::Entity::Character*		createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Dynamic*			createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Kinematic*		createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual void								createPagedGeometry(const TR::Graphics::PagedGeometryDesc &pagedGeometryDesc) = 0;
			virtual TR::Entity::Sprite*			createSprite(TR::Entity::SpriteDesc &spriteDesc) = 0;			
			virtual TR::Entity::Static*			createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Trigger*			createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc) = 0;
			virtual TR::Entity::Vehicle*			createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc) = 0;
			virtual TR::Entity::Vehicle*			createVehicle(const std::string &fileSettings, TR::Scene::Node *parentNode) = 0;
			virtual void								destroyDynamicEntity(const std::string &name) = 0;
			virtual void								destroyDynamicEntity(TR::Entity::Dynamic *dynamic) = 0;
			virtual void								destroyKinematicEntity(TR::Entity::Kinematic *kinematic) = 0;
			virtual void								destroyStaticEntity(TR::Entity::Static *staticEntity) = 0;
			virtual TR::Entity::Character*		getCharacter(const std::string &name) = 0;
			virtual TR::Entity::Character*		getCharacterById(TR::uint32 id) = 0;
			virtual TR::Entity::Dynamic*			getDynamicEntity(const std::string &name) = 0;
			virtual TR::Entity::Kinematic*		getKinematicEntity(const std::string &name) = 0;
			virtual TR::Graphics::MeshInfo&		getMesh(const std::string entityName);
			virtual TR::Entity::Sprite*			getSprite(const std::string &name) = 0;
			virtual TR::Entity::Static*			getStaticEntity(const std::string &name) = 0;
			//virtual TR::Entity::Trigger*		getTrigger(std::string name) = 0;
			virtual TR::Entity::Vehicle*			getVehicle(const std::string &name) = 0;
			virtual void								initialize(TR::Framework *framework) = 0;
			virtual void								updateEntities(unsigned long timeSinceLastFrame) = 0;
		};
	}
}

#endif