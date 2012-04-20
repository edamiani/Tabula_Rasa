#include "../include/TRAudioManager.h"

namespace TR
{
	namespace Audio
	{
		//TRManager* TRManager::mAudio = 0;

		/*****************************************************************************/
		OpenALManager::OpenALManager()
			: MAX_AUDIO_BUFFERS(64), MAX_AUDIO_SOURCES(16)
		{
			//mAudio = this;
	 
			isInitialised = false;
			isSoundOn = false;
	   
			mAudioPath = "";

			// Initial position of the listener
			position[0] = 0.0;
			position[1] = 0.0;
			position[2] = 0.0;
		  
			// Initial velocity of the listener
			velocity[0] = 0.0;
			velocity[1] = 0.0;
			velocity[2] = 0.0;
		  
			// Initial orientation of the listener = direction + direction up
			orientation[0] = 0.0;
			orientation[1] = 0.0;
			orientation[2] = -1.0;
			orientation[3] = 0.0;
			orientation[4] = 1.0;
			orientation[5] = 0.0;

			// Needed because of hardware limitation
			mAudioSourcesInUseCount = 0;
			mAudioSources=new unsigned int[MAX_AUDIO_SOURCES];
			mAudioSourceInUse=new bool[MAX_AUDIO_SOURCES];

			for ( int i=0; i < MAX_AUDIO_SOURCES; i++ )
			{
				mAudioSources[i] = 0;
				mAudioSourceInUse[i] = false;
			}

			mAudioBuffersInUseCount = 0;
			mAudioBuffers=new unsigned int[MAX_AUDIO_BUFFERS];
			mAudioBufferInUse=new bool[MAX_AUDIO_BUFFERS];

			for ( int i=0; i < MAX_AUDIO_BUFFERS; i++ )
			{
				mAudioBuffers[i] = 0;
				mAudioBufferInUse[i] = false;
			}

			// EAX related
			isEAXPresent = false;
		}

		/*****************************************************************************/
		OpenALManager::OpenALManager(int max_buffers = 64, int max_sources = 16)
			: MAX_AUDIO_BUFFERS(max_buffers), MAX_AUDIO_SOURCES(max_sources)
		{
			//mAudio = this;
	 
			isInitialised = false;
			isSoundOn = false;
	   
			mAudioPath = "";

			// Initial position of the listener
			position[0] = 0.0;
			position[1] = 0.0;
			position[2] = 0.0;
		  
			// Initial velocity of the listener
			velocity[0] = 0.0;
			velocity[1] = 0.0;
			velocity[2] = 0.0;
		  
			// Initial orientation of the listener = direction + direction up
			orientation[0] = 0.0;
			orientation[1] = 0.0;
			orientation[2] = -1.0;
			orientation[3] = 0.0;
			orientation[4] = 1.0;
			orientation[5] = 0.0;

			// Needed because of hardware limitation
			mAudioSourcesInUseCount = 0;
			mAudioSources=new unsigned int[MAX_AUDIO_SOURCES];
			mAudioSourceInUse=new bool[MAX_AUDIO_SOURCES];

			for ( int i=0; i < MAX_AUDIO_SOURCES; i++ )
			{
				mAudioSources[i] = 0;
				mAudioSourceInUse[i] = false;
			}

			mAudioBuffersInUseCount = 0;
			mAudioBuffers=new unsigned int[MAX_AUDIO_BUFFERS];
			mAudioBufferInUse=new bool[MAX_AUDIO_BUFFERS];

			for ( int i=0; i < MAX_AUDIO_BUFFERS; i++ )
			{
				mAudioBuffers[i] = 0;
				mAudioBufferInUse[i] = false;
			}

			// EAX related
			isEAXPresent = false;
		}

		/*****************************************************************************/
		OpenALManager::~OpenALManager()
		{
			// Delete the sources and buffers
			alDeleteSources( MAX_AUDIO_SOURCES, mAudioSources );
			alDeleteBuffers( MAX_AUDIO_BUFFERS, mAudioBuffers );
			delete mAudioSources;
			delete mAudioSourceInUse;
			delete mAudioBuffers;
			delete mAudioBufferInUse;

			mAudioBufferFileName.clear();

			alutExit();
		}

		/*****************************************************************************/
		bool OpenALManager::init()
		{
			// It's an error to initialise twice OpenAl
			if (isInitialised) 
				return true;
	 
			// Init openAL
			alutInit(NULL, NULL);
			// Clear Error Code (so we can catch any new errors)
			alGetError();
	  
			// Check for EAX 2.0 support and
			// Retrieves function entry addresses to API ARB extensions, in this case,
			// for the EAX extension. See Appendix 1 (Extensions) of
			// http://www.openal.org/openal_webstf/specs/OpenAL1-1Spec_html/al11spec7.html
			//
			// TODO EAX fct not used anywhere in the code ... !!!
			isEAXPresent = alIsExtensionPresent( "EAX2.0" );
			if ( isEAXPresent )
			{ 
			#ifdef _USEEAX
				eaxSet = (EAXSet) alGetProcAddress( "EAXSet" );
				if ( eaxSet == NULL )
					isEAXPresent = false;
	 
				eaxGet = (EAXGet) alGetProcAddress( "EAXGet" );
				if ( eaxGet == NULL )
					isEAXPresent = false;
	  
				if ( !isEAXPresent )
					checkALError( "Failed to get the EAX extension functions adresses.\n" );
			#else
				isEAXPresent = false;
			#endif // _USEEAX 
			}
	 
			// Test if Ogg Vorbis extension is present
			//isOggExtensionPresent();
	  
			// Create the Audio Buffers
			alGenBuffers( MAX_AUDIO_BUFFERS, mAudioBuffers );
			if (checkALError("init::alGenBuffers:"))
				return false;
	  
			// Generate Sources
			alGenSources( MAX_AUDIO_SOURCES, mAudioSources );
			if (checkALError("init::alGenSources :"))
				return false;
	  
			// Setup the initial listener parameters
			// -> location
			alListenerfv( AL_POSITION, position );
	   
			// -> velocity
			alListenerfv( AL_VELOCITY, velocity );
	  
			// -> orientation
			alListenerfv( AL_ORIENTATION, orientation );
	  
			// Gain
			alListenerf( AL_GAIN, 1.0 );
	  
			// Initialise Doppler
			alDopplerFactor( 1.0 ); // 1.2 = exaggerate the pitch shift by 20%
			alDopplerVelocity( 343.0f ); // m/s this may need to be scaled at some point
	  
			// Ok
			isInitialised = true;
			isSoundOn = true;
	 
			//printf( "SoundManager initialised.\n\n");
	  
			return true;
		}

		/*****************************************************************************/
		bool OpenALManager::checkALError()
		{
			ALenum error = 0;
			if((error = alGetError()) != AL_NO_ERROR)
			{
				std::string err = "ERROR SoundManager:: ";
				err += (char*) alGetString(error);
	     
				//printf( "%s\n", err.c_str());
				return true;
			}
			return false;
		}

		/*****************************************************************************/
		bool OpenALManager::checkALError(std::string pMsg)
		{
			ALenum error = 0;
	  
			if ( (error = alGetError()) == AL_NO_ERROR )
			return false;
	 
			char mStr[256];
			switch ( error )
			{
				case AL_INVALID_NAME:
					//sprintf(mStr,"ERROR SoundManager::%s Invalid Name", pMsg.c_str());
					break;
				case AL_INVALID_ENUM:
					//sprintf(mStr,"ERROR SoundManager::%s Invalid Enum", pMsg.c_str());
					break;
				case AL_INVALID_VALUE:
					//sprintf(mStr,"ERROR SoundManager::%s Invalid Value", pMsg.c_str());
					break;
				case AL_INVALID_OPERATION:
					//sprintf(mStr,"ERROR SoundManager::%s Invalid Operation", pMsg.c_str());
					break;
				case AL_OUT_OF_MEMORY:
					//sprintf(mStr,"ERROR SoundManager::%s Out Of Memory", pMsg.c_str());
					break;
				default:
					//sprintf(mStr,"ERROR SoundManager::%s Unknown error (%i) case in testALError()", pMsg.c_str(), error);
					break;
			}
	  
			//printf( "%s\n", mStr );
	  
			return true;
		}

		/*****************************************************************************/
		/*bool TRManager::isOggExtensionPresent( void )
		{
			if(alIsExtensionPresent("AL_EXT_vorbis") != AL_TRUE)
			{
				printf("ERROR: SoundManager::isOggExtensionPresent : Ogg Vorbis extension not   availablee!\n");
				bOggExtensionPresent = false;
				return false;
			}
			return true;
		}*/

		// Attempts to aquire an empty audio source and assign it back to the caller
		// via sourceID. This will lock the source
		/*****************************************************************************/
		bool OpenALManager::attachAudioToSource(std::string filename, unsigned int *sourceId, bool loop)
		{
			if ( filename.empty() || filename.length() > MAX_FILENAME_LENGTH )
				return false;
	  
			if ( mAudioSourcesInUseCount == MAX_AUDIO_SOURCES )
				return false;   // out of Audio Source slots!
	 
			int bufferID = -1;   // Identity of the Sound Buffer to use
			int freeSourceId = -1;   // Identity of the Source Buffer to use
	 
			alGetError();    // Clear Error Code
	 
			// Check and see if the pSoundFile is already loaded into a buffer
			bufferID = _locateAudioBuffer(filename);
			if ( bufferID < 0 )
			{
				// The sound file isn't loaded in a buffer, lets attempt to load it on the fly
				bufferID = _loadAudioInToBuffer(filename);
				if ( bufferID < 0 ) return false;   // failed!
			}
	 
			// If you are here, the sound the requester wants to reference is in a buffer.
			// Now, we need to find a free Audio Source slot in the sound system
			freeSourceId = 0;
	  
			while (mAudioSourceInUse[freeSourceId] == true) freeSourceId++;
	  
			// When you are here, 'mSourceID' now represents a free Audio Source slot
			// The free slot may not be at the end of the array but in the middle of it.
			*sourceId = freeSourceId; // return the Audio Source ID to the caller
			mAudioSourceInUse[ freeSourceId ] = true; // mark this Source slot as in use
			mAudioSourcesInUseCount++; // bump the 'in use' counter
	  
			// Now inform OpenAL of the sound assignment and attach the audio buffer to the audio source
			alSourcei( mAudioSources[freeSourceId], AL_BUFFER, mAudioBuffers[bufferID] );
	  
			// Set the looping option
			alSourcei( mAudioSources[freeSourceId], AL_LOOPING, loop );
	  
			if ( checkALError( "loadSource()::alSourcei" ) )
				return false;
	  
			return true;
		}

		/*****************************************************************************/
		bool OpenALManager::setSourceSettings(unsigned int sourceId, TR::Vector3 position, TR::Vector3 velocity, TR::Vector3 direction,
			float maxDistance, bool playNow, bool forceRestart, float minGain)
		{
			if ( sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[ sourceId ] )
				return false;
	  
			// Set the position
			ALfloat pos[] = { position.x, position.y, position.z };
	  
			alSourcefv( mAudioSources[ sourceId ], AL_POSITION, pos );
	  
			if ( checkALError( "setSound::alSourcefv:AL_POSITION" ) )
				return false;
	  
			// Set the veclocity
			ALfloat vel[] = { velocity.x, velocity.y, velocity.z };
	  
			alSourcefv( mAudioSources[ sourceId ], AL_VELOCITY, vel );
	  
			if ( checkALError( "setSound::alSourcefv:AL_VELOCITY" ) )
				return false;
	   
			// Set the direction
			ALfloat dir[] = { velocity.x, velocity.y, velocity.z };
	  
			alSourcefv( mAudioSources[ sourceId ], AL_DIRECTION, dir );
	  
			if ( checkALError( "setSound::alSourcefv:AL_DIRECTION" ) )
				return false;
	  
			// Set the max audible distance
			alSourcef( mAudioSources[ sourceId ], AL_MAX_DISTANCE, maxDistance );
	  
			// Set the MIN_GAIN ( IMPORTANT - if not set, nothing audible! )
			alSourcef( mAudioSources[ sourceId ], AL_MIN_GAIN, minGain );
	  
			// Set the max gain
			alSourcef( mAudioSources[ sourceId ], AL_MAX_GAIN, 1.0f ); // TODO as parameter ? global ?
	  
			// Set the rollof factor
			alSourcef( mAudioSources[ sourceId ], AL_ROLLOFF_FACTOR, 1.0f ); // TODO as parameter ?
	  
			// Do we play the sound now ?
			if ( playNow ) return playAudio( sourceId, forceRestart ); // TODO bof... not in this fct
	  
			return true;
		}

		/*****************************************************************************/
		bool OpenALManager::playAudio(unsigned int sourceId, bool forceRestart)
		{
			// Make sure the audio source ident is valid and usable
			if ( sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;
	  
			int sourceAudioState = 0;
	  
			alGetError();
	  
			// Are we currently playing the audio source?
			alGetSourcei( mAudioSources[sourceId], AL_SOURCE_STATE, &sourceAudioState );
	  
			if ( sourceAudioState == AL_PLAYING )
			{
				if ( forceRestart )
					stopAudio( sourceId );
				else
					return false; // Not forced, so we don't do anything
			}
	  
			alSourcePlay(mAudioSources[sourceId]);
			if ( checkALError( "playAudio::alSourcePlay: ") )
				return false;
	  
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::pauseAudio(unsigned int sourceId)
		{
			// Make sure the audio source ident is valid and usable
			if (sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;
	 
			alGetError();
	 
			alSourcePause(mAudioSources[sourceId]);
	 
			if (checkALError( "pauseAudio::alSourceStop "))
				return false;
	 
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::pauseAllAudio()
		{
			if ( mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES )
				return false;
	 
			alGetError();
	 
			alSourcePausev( mAudioSourcesInUseCount, mAudioSources );
	 
			if ( checkALError( "pauseAllAudio::alSourceStop ") )
				return false;
	 
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::resumeAudio(unsigned int sourceId)
		{
			// Make sure the audio source id is valid and usable
			if (sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;
	 
			alGetError();
	 
			// Are we currently playing the audio source?
			int sourceAudioState = 0;
			alGetSourcei(mAudioSources[sourceId], AL_SOURCE_STATE, &sourceAudioState);
	 
			if (sourceAudioState == AL_PAUSED)
			{
				playAudio(sourceId, false);
			}
	 
			if (checkALError("resumeAudio::alSourceStop "))
				return false;
	 
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::resumeAllAudio()
		{
			if ( mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES )
				return false;
	 
			alGetError();
	 
			int sourceAudioState = 0;
	 
			for ( int i=0; i<mAudioSourcesInUseCount; i++ )
			{
				// Are we currently playing the audio source?
				alGetSourcei( mAudioSources[i], AL_SOURCE_STATE, &sourceAudioState );
	 
				if ( sourceAudioState == AL_PAUSED )
				{
					resumeAudio(i);
				}
			}
	 
			if ( checkALError( "resumeAllAudio::alSourceStop ") )
				return false;
	 
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::stopAudio(unsigned int sourceId)
		{
			// Make sure the audio source ident is valid and usable
			if (sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;
	  
			alGetError();
	  
			alSourceStop( mAudioSources[sourceId] );
	  
			if ( checkALError("stopAudio::alSourceStop "))
				return false;
	  
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::stopAllAudio()
		{
			if (mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES)
				return false;
	 
			alGetError();
	 
			for (int i = 0; i < mAudioSourcesInUseCount; i++)
			{
				stopAudio(i);
			}
	 
			if (checkALError("stopAllAudio::alSourceStop "))
				return false;
	 
			return true;
		}

		/****************************************************************************/
		bool OpenALManager::releaseSource(unsigned int sourceId)
		{
			if ( sourceId >= MAX_AUDIO_SOURCES )
				return false;

			alSourceStop(mAudioSources[sourceId]);

			mAudioSourceInUse[sourceId] = false;
			mAudioSourcesInUseCount--;

			return true;
		}

		/****************************************************************************/
		bool OpenALManager::setSourcePosition(unsigned int sourceId, TR::Vector3 position)
		{
			if(sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;
	  
			// Set the position
			ALfloat pos[] = { position.x, position.y, position.z };
	  
			alSourcefv(mAudioSources[sourceId], AL_POSITION, pos );
	  
			if(checkALError( "setSound::alSourcefv:AL_POSITION"))
				return false;
	 
			return true;
		}
		/****************************************************************************/
		bool OpenALManager::setSourcePitch(unsigned int sourceId, TR::Real pitch)
		{
			if(sourceId >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[sourceId])
				return false;

			if(pitch <= 0.0)
				pitch = 0.01;
	  
			alSourcef(mAudioSources[sourceId], AL_PITCH, pitch );
	  
			if(checkALError( "setSound::alSourcef:AL_PITCH"))
				return false;
	 
			return true;
		} 
		/****************************************************************************/
		bool OpenALManager::setListenerPosition(TR::Vector3 position, TR::Vector3 velocity, TR::Quaternion orientation)
		{
			Vector3 axis;
	  
			// Set the position
			ALfloat pos[] = { position.x, position.y, position.z };
	  
			alListenerfv( AL_POSITION, pos );
	  
			if(checkALError("setListenerPosition::alListenerfv:AL_POSITION"))
				return false;
	  
			// Set the veclocity
			ALfloat vel[] = { velocity.x, velocity.y, velocity.z };
	  
			alListenerfv(AL_VELOCITY, vel);
	  
			if(checkALError("setListenerPosition::alListenerfv:AL_VELOCITY"))
				return false;
	  
			// Orientation of the listener : look at then look up
			axis = Vector3::ZERO;
			axis.x = orientation.getYaw().valueRadians();
			axis.y = orientation.getPitch().valueRadians();
			axis.z = orientation.getRoll().valueRadians();
	  
			// Set the direction
			ALfloat dir[] = { axis.x, axis.y, axis.z };
	  
			alListenerfv( AL_ORIENTATION, dir );
	  
			if(checkALError("setListenerPosition::alListenerfv:AL_DIRECTION"))
				return false;
	  
			// TODO as parameters ?
			alListenerf( AL_MAX_DISTANCE, 10000.0f );
			alListenerf( AL_MIN_GAIN, 0.0f );
			alListenerf( AL_MAX_GAIN, 1.0f );
			alListenerf( AL_GAIN, 1.0f );
	  
			return true;
		}

		/*****************************************************************************/
		int OpenALManager::_locateAudioBuffer( std::string filename )
		{
			for ( unsigned int b = 0; b < MAX_AUDIO_BUFFERS; b++ )
			{
				if (filename == mAudioBufferFileName[b] ) return (int) b; 
				//Surely a way to optimize it with the MAP
			}
			return -1; // not found
		}

		/*****************************************************************************/
		int OpenALManager::_loadAudioInToBuffer(std::string filename)
		{
			if ( filename.empty() )
				return -1;
	  
			// Make sure we have audio buffers available
			if ( mAudioBuffersInUseCount == MAX_AUDIO_BUFFERS ) return -1;
	  
			// Find a free Audio Buffer slot
			int bufferID = 0; // Identity of the Sound Buffer to use
	  
			while ( mAudioBufferInUse[ bufferID ] == true ) bufferID++;
	  
			// load .wav, .ogg or .au
			if ( filename.find( ".wav", 0 ) != std::string::npos )
			{
				// When you are here, bufferID now represents a free Audio Buffer slot
				// Attempt to load the SoundFile into the buffer
				if (!_loadWAV(filename, bufferID)) 
					return -1;
			}

			// Successful load of the file into the Audio Buffer.
			mAudioBufferInUse[ bufferID ] = true; // Buffer now in use
			mAudioBufferFileName[bufferID] = filename; // save the file descriptor
	 
			mAudioBuffersInUseCount++; // bump the 'in use' counter

			return bufferID;
		}

		/*****************************************************************************/
		bool OpenALManager::_loadWAV(std::string filename, ALuint pDestAudioBuffer)
		{
			mAudioBuffers[pDestAudioBuffer] = alutCreateBufferFromFile(filename.c_str());

			if(mAudioBuffers[pDestAudioBuffer] != AL_NONE)
				return true;
			else
				return false;
		}
	}
}