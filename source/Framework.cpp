#include "..\include\Framework.h"
#include "..\plugins\GRAPHICS_Ogre_1_4\include\OgreManager.h"

template<> TR::Framework* TR::Singleton<TR::Framework>::ms_Singleton = 0;

namespace TR
{
	Framework::Framework()
	{
		//mAudioManager = TR::Audio::Manager::createManager();
		//mAudioManager->init();
		mAudioManager = 0;
		mEntityManager = 0;
		mGraphicsManager = 0;
		mInputManager = 0;
		mIsRunning = true;
		mKeyboard = 0;
		mMouse = 0;
		mPhysicsManager = 0;
		mSceneManager = 0;
		mXmlManager = 0;
	}

	Framework::~Framework()
	{
		if(mInputManager) delete mInputManager;
		if(mPhysicsManager) delete mPhysicsManager;
		if(mGraphicsManager) delete mGraphicsManager;
		if(mEntityManager) delete mEntityManager;
		if(mXmlManager) delete mXmlManager;
		if(mSceneManager) delete mSceneManager;
		//if(mAudioManager) TR::Audio::Manager::destroyManager();
		if(mAudioManager) delete mAudioManager;

	#ifdef _WINDOWS
		if(hinstAudio) FreeLibrary(hinstAudio);
		if(hinstEntity) FreeLibrary(hinstEntity);
		if(hinstGraphics) FreeLibrary(hinstGraphics);
		if(hinstInput) FreeLibrary(hinstInput);
		if(hinstPhysics) FreeLibrary(hinstPhysics);
		if(hinstScene) FreeLibrary(hinstScene);
		if(hinstXml) FreeLibrary(hinstXml);
	#endif
	}

	void Framework::loadAudioPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstAudio = LoadLibraryA(fileName.c_str());
		assert(hinstAudio != NULL);

		audioFunction loadPlugin;
		loadPlugin = (audioFunction)GetProcAddress(hinstAudio, "loadPlugin");
		assert(loadPlugin != NULL);

		mAudioManager = loadPlugin();
		assert(mAudioManager != 0);
		mAudioManager->initialize(this);
	#endif
	}

	void Framework::loadEntityPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstEntity = LoadLibraryA(fileName.c_str());
		assert(hinstEntity != NULL);

		entityFunction loadPlugin;
		loadPlugin = (entityFunction)GetProcAddress(hinstEntity, "loadPlugin");
		assert(loadPlugin != NULL);

		mEntityManager = loadPlugin();
		assert(mEntityManager != 0);
		mEntityManager->initialize(this);
	#endif
	}

	bool Framework::loadGraphicsPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstGraphics = LoadLibraryA(fileName.c_str());
		assert(hinstGraphics != NULL);

		graphicsFunction loadPlugin;
		loadPlugin = (graphicsFunction)GetProcAddress(hinstGraphics, "loadPlugin");
		assert(loadPlugin != NULL);

		mGraphicsManager = loadPlugin();
		assert(mGraphicsManager != 0);
		if(!mGraphicsManager->initialize(this))
			return false;

		return true;
	#endif
	}

	void Framework::loadInputPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstInput = LoadLibraryA(fileName.c_str());
		assert(hinstInput != NULL);

		inputFunction loadPlugin;
		loadPlugin = (inputFunction)GetProcAddress(hinstInput, "loadPlugin");
		assert(loadPlugin != NULL);

		mInputManager = loadPlugin();
		assert(mInputManager != 0);
		mInputManager->initialize(this);
		mKeyboard = mInputManager->createKeyboard(true);
		mMouse = mInputManager->createMouse(true);
	#endif
	}

	void Framework::loadPhysicsPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstPhysics = LoadLibraryA(fileName.c_str());
		assert(hinstPhysics != NULL);

		physicsFunction loadPlugin;
		loadPlugin = (physicsFunction)GetProcAddress(hinstPhysics, "loadPlugin");
		assert(loadPlugin != NULL);

		mPhysicsManager = loadPlugin();
		assert(mPhysicsManager != 0);
		mPhysicsManager->initialize(this);
	#endif
	}

	void Framework::loadScenePlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstScene = LoadLibraryA(fileName.c_str());
		assert(hinstScene != NULL);

		sceneFunction loadPlugin;
		loadPlugin = (sceneFunction)GetProcAddress(hinstScene, "loadPlugin");
		assert(loadPlugin != NULL);

		mSceneManager = loadPlugin();
		assert(mEntityManager != 0);
		mSceneManager->initialize(this);
	#endif
	}

	void Framework::loadXmlPlugin(const std::string &fileName)
	{
	#ifdef _WINDOWS
		hinstXml = LoadLibraryA(fileName.c_str());
		assert(hinstXml != NULL);

		xmlFunction loadPlugin;
		loadPlugin = (xmlFunction)GetProcAddress(hinstXml, "loadPlugin");
		assert(loadPlugin != NULL);

		mXmlManager = loadPlugin();
		assert(mXmlManager != 0);
	#endif
	}

	void Framework::play() 
	{
		onSceneStart();
		mTimer.reset();

		while(mIsRunning)
		{
			mTimer.markTime();
			onEnterFrame(mTimer.getTimeSinceLastMark());
			mGraphicsManager->renderFrame();
		}

		onSceneFinish();
	}
}