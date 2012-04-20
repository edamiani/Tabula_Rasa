#ifndef __GLOBAL_STATE__
#define __GLOBAL_STATE__

class GlobalState
{
public:
	~GlobalState();

	static GlobalState* instance()
	{
		if (mInstance == 0) mInstance = new GlobalState();
		return mInstance;
	}
	void run();

protected:
	GlobalState() { }

private:
	static GlobalState* mInstance;

	Gui 		*mGui;
	Script 	*mScript;
	World 	*mWorld;
};

#end if
