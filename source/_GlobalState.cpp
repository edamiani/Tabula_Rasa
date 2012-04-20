#include "../include/GlobalState.h"

GlobalState* GlobalState::mInstance = 0;

void GlobalState::run()
{
	mGui->render();
	mWorld->render();
	mScript->run();
}
