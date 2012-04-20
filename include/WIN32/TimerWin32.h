#ifndef __TR_TIMER_WIN32__
#define __TR_TIMER_WIN32__

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"

//Get around Windows hackery
#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif

#define FREQUENCY_RESAMPLE_RATE 200

namespace TR
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		/** Resets timer */
		void reset();

		/** Returns milliseconds since initialisation or last reset */
		unsigned long getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		unsigned long getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMicrosecondsCPU();

		void markTime() { mLastMark = mCurrentMark; mCurrentMark = getMilliseconds(); }
		unsigned long getCurrentMark() { return mCurrentMark; }
		unsigned long getLastMark() { return mLastMark; }
		unsigned long getTimeSinceLastMark() { return (mCurrentMark - mLastMark); }

	private:
		clock_t zeroClock;

		unsigned long mCurrentMark;
		unsigned long mLastMark;

		DWORD mStartTick;
		LONGLONG mLastTime;
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mFrequency;

		DWORD mProcMask;
		DWORD mSysMask;

		HANDLE mThread;

		DWORD mQueryCount;
	};
} 
#endif
