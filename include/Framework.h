#ifndef __TR_FRAMEWORK__
#define __TR_FRAMEWORK__

#include "Dependencies.h"
#include "AudioManager.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "EntityManager.h"
#include "EntityTrigger.h"
#include "GraphicsManager.h"
#include "InputEvents.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Singleton.h"
#include "Timer.h"
#include "XmlDocument.h"
#include "XmlManager.h"

typedef TR::Audio::Manager* (*audioFunction)(void);
typedef TR::Entity::Manager* (*entityFunction)(void);
typedef TR::Graphics::Manager* (*graphicsFunction)(void);
typedef TR::Input::Manager* (*inputFunction)(void);
typedef TR::Physics::Manager* (*physicsFunction)(void);
typedef TR::Scene::Manager* (*sceneFunction)(void);
typedef TR::Xml::Manager* (*xmlFunction)(void);

namespace TR 
{
	class Framework : public TR::Singleton<TR::Framework>
	{
	public:
		Framework();
		virtual ~Framework();

		void							createGuiManager(const std::string &schemeFile, const std::string &layoutFile) { mGuiManager = mGraphicsManager->createGui(schemeFile, layoutFile); }
		TR::Audio::Manager*		getAudioManager() { return mAudioManager; }
		TR::Entity::Manager*		getEntityManager() { return mEntityManager; }
		TR::Graphics::Manager*	getGraphicsManager() { return mGraphicsManager; }
		TR::Gui::Manager*			getGuiManager() { return mGuiManager; }
		TR::Input::Manager*		getInputManager() { return mInputManager; }
		TR::Physics::Manager*	getPhysicsManager() { return mPhysicsManager; }
		TR::Scene::Manager*		getSceneManager() { return mSceneManager; }
		TR::Timer*					getTimer() { return &mTimer; }
		TR::Xml::Manager*			getXmlManager() { return mXmlManager; }
		void							loadAudioPlugin(const std::string &fileName);
		void							loadEntityPlugin(const std::string &fileName);
		bool							loadGraphicsPlugin(const std::string &fileName);
		void							loadInputPlugin(const std::string &fileName);
		void							loadPhysicsPlugin(const std::string &fileName);
		void							loadScenePlugin(const std::string &fileName);
		void							loadXmlPlugin(const std::string &fileName);
		virtual void				onContactNotify(TR::uint32 object1Flag, TR::uint32 object2Flag, TR::Physics::collisionNotify cn) = 0;
		virtual void				onEnterFrame(TR::Real timeSinceLastFrame) = 0;
		virtual void				onGuiSubmit(TR::uint32 widgetId) = 0;
		virtual void				onJoyAxisMoved(const TR::Input::JoystickEvent &e, int axis) = 0;
		virtual void				onJoyButtonDown(const TR::Input::JoystickEvent &e) = 0;
		virtual void				onJoyButtonUp(const TR::Input::JoystickEvent &e) = 0;
		virtual void				onKeyDown(const TR::Input::KeyEvent &e) = 0;
		virtual void				onKeyUp(const TR::Input::KeyEvent &e) = 0;
		virtual void				onMouseButtonDown(const TR::Input::MouseEvent &e) = 0;
		virtual void				onMouseButtonUp(const TR::Input::MouseEvent &e) = 0;
		virtual void				onMouseMoved(const TR::Input::MouseEvent &e) = 0;
		virtual void				onSceneFinish() = 0;
		virtual void				onSceneStart() = 0;
		virtual void				onTrigger(TR::uint32 triggerFlag, TR::uint32 objectFlag, TR::Entity::triggerNotify tn) = 0;
		void							play();
		void							stop() { mIsRunning = false; }

	protected:
		
		TR::Audio::Manager		*mAudioManager;
		TR::Entity::Manager		*mEntityManager;
		TR::Graphics::Manager	*mGraphicsManager;
		TR::Gui::Manager			*mGuiManager;
		TR::Input::Manager		*mInputManager;
		bool							mIsRunning;
		TR::Input::Keyboard		*mKeyboard;
		TR::Input::Mouse			*mMouse;
		TR::Physics::Manager		*mPhysicsManager;
		TR::Scene::Manager		*mSceneManager;
		TR::Timer					mTimer;
		TR::Xml::Manager			*mXmlManager;

	#ifdef _WINDOWS
		HINSTANCE hinstAudio;
		HINSTANCE hinstEntity;
		HINSTANCE hinstGraphics;
		HINSTANCE hinstInput;
		HINSTANCE hinstPhysics;
		HINSTANCE hinstScene;
		HINSTANCE hinstXml;
	#endif
	};
}

#endif