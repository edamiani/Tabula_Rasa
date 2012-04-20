#ifndef __GLOBAL_FSM__
#define __GLOBAL_FSM__

#include "GlobalState.h"

class GlobalFSM
{
public:
	GlobalFSM() { }
	~GlobalFSM() { }

	void run();

private:
	GlobalState *mCurrentState;
};

#end if
