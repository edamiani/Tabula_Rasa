#ifndef __TR_AUDIO_MANAGER__
#define __TR_AUDIO_MANAGER__

#include "Dependencies.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace TR
{
	namespace Audio
	{
		class Manager
		{
		public:
			Manager() {  }
			virtual ~Manager() {  }

			virtual bool init() = 0;
			virtual void initialize(TR::Framework *framework) = 0;

			virtual bool attachAudioToSource(std::string filename, unsigned int *sourceId, bool loop) = 0;
			virtual bool setSourceSettings(unsigned int sourceId, TR::Vector3 position, TR::Vector3 velocity, TR::Vector3 direction, float maxDistance, bool playNow, bool forceRestart, float minGain) = 0;

			virtual bool playAudio(unsigned int sourceId, bool forceRestart) = 0;
			virtual bool pauseAudio(unsigned int sourceId) = 0;
			virtual bool pauseAllAudio() = 0;
			virtual bool resumeAudio(unsigned int sourceId) = 0;
			virtual bool resumeAllAudio() = 0;
			virtual bool stopAudio(unsigned int sourceId) = 0;
			virtual bool stopAllAudio() = 0;
			virtual bool releaseSource(unsigned int sourceId) = 0;
			virtual bool setSourcePitch(unsigned int sourceId, TR::Real pitch) = 0;
			virtual bool setSourcePosition(unsigned int sourceId, TR::Vector3 position) = 0;
			virtual bool setListenerPosition(TR::Vector3 position, TR::Vector3 velocity, TR::Quaternion orientation) = 0;

			// Singleton?!?!?! Bad implementation!!!!!!
			//static Manager* mAudio;
		};
	}
}

#endif