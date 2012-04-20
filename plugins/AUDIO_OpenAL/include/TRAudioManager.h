#ifndef __TR_OPENAL_MANAGER__
#define __TR_OPENAL_MANAGER__

#include "../../../include/AudioManager.h"
#include "../../../include/Dependencies.h"
#include "../../../include/Framework.h"
#include "../../../include/Quaternion.h"
#include "../../../include/Vector3.h"

#include <al.h>
#include <alc.h>
#include <alut.h>
#include <efx.h>

//#include "vorbis\codec.h"
//#include "vorbis\vorbisfile.h"

// Used to store sound filenames
#define MAX_FILENAME_LENGTH 40

#ifdef _USEEAX
   #include "eax.h"
#endif

namespace TR
{
	namespace Audio
	{
		class OpenALManager : public TR::Audio::Manager
		{
		public:
			OpenALManager();
			OpenALManager(int max_buffers, int max_sources);
			~OpenALManager();

			bool init();
			void initialize(TR::Framework *framework) { mFramework = framework; }

			bool checkALError();
			bool checkALError(std::string pMsg);
			//bool isOggExtensionPresent();

			bool attachAudioToSource(std::string filename, unsigned int *sourceId, bool loop);
			bool setSourceSettings(unsigned int sourceId, TR::Vector3 position, TR::Vector3 velocity, TR::Vector3 direction, float maxDistance, bool playNow, bool forceRestart, float minGain);

			bool playAudio(unsigned int sourceId, bool forceRestart);
			bool pauseAudio(unsigned int sourceId);
			bool pauseAllAudio();
			bool resumeAudio(unsigned int sourceId);
			bool resumeAllAudio();
			bool stopAudio(unsigned int sourceId);
			bool stopAllAudio();
			bool releaseSource(unsigned int sourceId);
			bool setSourcePitch(unsigned int sourceId, TR::Real pitch);
			bool setSourcePosition(unsigned int sourceId, TR::Vector3 position);
			bool setListenerPosition(TR::Vector3 position, TR::Vector3 velocity, TR::Quaternion orientation);

			// Singleton?!?!?! Bad implementation!!!!!!
			//static OpenALManager* mAudio;

		private:
			// Function to check if the soundFile is already loaded into a buffer
			int _locateAudioBuffer(std::string filename);

			// Loads an audio file into a buffer
			int _loadAudioInToBuffer(std::string filename);

			// Loads a .WAV file; used by _loadAudioInToBuffer()
			bool _loadWAV(std::string filename, ALuint pDestAudioBuffer);

			TR::Framework *mFramework;

			bool isInitialised;

			// Ogg Vorbis extension
			//bool bOggExtensionPresent;
	     
			std::string mAudioPath;

			bool isSoundOn;

			ALfloat position[3];
			ALfloat velocity[3];
			ALfloat orientation[6];

			// Needed because of hardware limitation
			// Audio sources
			unsigned const short MAX_AUDIO_SOURCES;
			unsigned int mAudioSourcesInUseCount;
			unsigned int* mAudioSources;
			bool* mAudioSourceInUse;

			// Audio buffers
			unsigned const short int MAX_AUDIO_BUFFERS;
			unsigned int mAudioBuffersInUseCount;
			unsigned int* mAudioBuffers;
			bool* mAudioBufferInUse;
			std::map<int, std::string, std::less<int> > mAudioBufferFileName;

			// EAX related
			ALboolean isEAXPresent;

		#ifdef _USEEAX
			// EAX 2.0 GUIDs
			const GUID DSPROPSETID_EAX20_ListenerProperties
				= { 0x306a6a8, 0xb224, 0x11d2, { 0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };
		     
			const GUID DSPROPSETID_EAX20_BufferProperties
				= { 0x306a6a7, 0xb224, 0x11d2, {0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };
		     
			EAXSet eaxSet; // EAXSet function, retrieved if EAX Extension is supported
			EAXGet eaxGet; // EAXGet function, retrieved if EAX Extension is supported
		#endif // _USEEAX
		};
	}
}

#endif