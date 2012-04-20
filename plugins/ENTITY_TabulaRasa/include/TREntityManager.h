#ifndef __TR_ENTITY_TRMANAGER__
#define __TR_ENTITY_TRMANAGER__

#include "../../../include/EntityManager.h"
#include "../../../include/EntityVehicle.h"

namespace TR
{
	namespace Entity
	{
		class TRManager : public TR::Entity::Manager
		{
		public:
													TRManager() { mVehicles.empty(); }
													~TRManager();
			
			TR::Entity::Character*			createCharacter(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Dynamic*				createDynamicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Kinematic*			createKinematicEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			void									createPagedGeometry(const TR::Graphics::PagedGeometryDesc &pagedGeometryDesc);
			TR::Entity::Sprite*				createSprite(TR::Entity::SpriteDesc &spriteDesc);			
			TR::Entity::Static*				createStaticEntity(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Trigger*				createTrigger(TR::Entity::SpriteDesc &spriteDesc, const TR::Physics::SolidDesc &solidDesc);
			TR::Entity::Vehicle*				createVehicle(TR::Entity::SpriteDesc &spriteDesc, TR::Entity::VehicleDesc &vehicleDesc);
			TR::Entity::Vehicle*				createVehicle(const std::string &fileSettings, TR::Scene::Node *parentNode);
			void									destroyDynamicEntity(const std::string &name);
			void									destroyDynamicEntity(TR::Entity::Dynamic *dynamic);
			void									destroyKinematicEntity(TR::Entity::Kinematic *kinematic);
			void									destroyStaticEntity(TR::Entity::Static *staticEntity);
			TR::Entity::Character*			getCharacter(const std::string &name);
			TR::Entity::Character*			getCharacterById(TR::uint32 id);
			TR::Entity::Dynamic*				getDynamicEntity(const std::string &name);
			TR::Entity::Kinematic*			getKinematicEntity(const std::string &name);
			TR::Entity::MeshInfo&			getMesh(const std::string entityName);
			TR::Entity::Sprite*				getSprite(const std::string &name);
			TR::Entity::Static*				getStaticEntity(const std::string &name);
			//TR::Entity::Trigger*			getTrigger(std::string name);
			TR::Entity::Vehicle*				getVehicle(const std::string &name);
			void									initialize(TR::Framework *framework) { mFramework = framework; }
			void									updateEntities(unsigned long timeSinceLastFrame);
		
		private:
			std::map<std::string, TR::Entity::Character*>				mCharacters;
			std::map<std::string, TR::Entity::Character*>::iterator	mCharactersIterator;
			std::map<std::string, TR::Entity::Dynamic*>					mDynamics;
			std::map<std::string, TR::Entity::Dynamic*>::iterator		mDynamicsIterator;
			TR::Framework															*mFramework;
			std::map<std::string, TR::Entity::Kinematic*>				mKinematics;
			std::map<std::string, TR::Entity::Kinematic*>::iterator	mKinematicsIterator;
			std::map<std::string, TR::Entity::Sprite*>					mSprites;
			std::map<std::string, TR::Entity::Sprite*>::iterator		mSpritesIterator;
			std::map<std::string, TR::Entity::Static*>					mStatics;
			std::map<std::string, TR::Entity::Static*>::iterator		mStaticsIterator;
			std::map<std::string, TR::Entity::Trigger*>					mTriggers;
			std::map<std::string, TR::Entity::Trigger*>::iterator		mTriggersIterator;
			std::map<std::string, TR::Entity::Vehicle*>					mVehicles;
			std::map<std::string, TR::Entity::Vehicle*>::iterator		mVehiclesIterator;
		};
	}
}

#endif