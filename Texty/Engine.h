#ifndef ENGINE_H_
#define ENGINE_H_
/*
	Engine.h - Customises the window and controls the main game loop.
*/

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include "Observer.h"
#include "StateManager.h"

namespace texty
{
	const int MAX_WINDOW_TITLE = 64;
	const int FONT_X_SIZE = 8;
	const int FONT_Y_SIZE = 8;
	const int MS_PER_UPDATE = 1;

	class Engine : public Observer{
	public:
		//singleton
		static Engine* Get();
		//create game window, and load all assets
		void Start(const char* title, unsigned int w, unsigned int h);
		//hop into main gameloop - needs to have a state
		void Run();
		//process messages
		void OnNotify(Message&, Event);

		//get window widths/height
		int GetWidth()		const	{ return _width; }
		int GetHeight()		const	{ return _height; }

	private:
		//no instance
		Engine();
		//handle all our game states
		StateManager _stateManager;

		//data
		char _title[MAX_WINDOW_TITLE];
		int _width;
		int _height;

		//exit gameloop
		bool _wantExit;

		//testing purpose
		unsigned int _currentfps;
	};
}

#endif