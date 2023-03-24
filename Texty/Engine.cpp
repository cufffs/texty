#include "Engine.h"
#include "Assets.h"
#include <time.h>

namespace texty{
	/*
	Taken from SetConsolePalette and related typedefs from
		www.catch22.net/sites/default/source/files/setconsoleinfo.c
	*/
	VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName);
	typedef BOOL(WINAPI *PGetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
	typedef BOOL(WINAPI *PSetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
	typedef BOOL(WINAPI *PGetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);
	typedef BOOL(WINAPI *PSetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);

	Engine::Engine()
		: _wantExit(false),
		_currentfps(0)
	{
		srand((unsigned int)time(NULL));	//init srand for entire program
		Messager::Add(this);
	}

	Engine* Engine::Get()
	{
		static Engine seng;
		return &seng;
	}

	void Engine::Start(const char* title, unsigned int w, unsigned int h)
	{
		//handles and stuff to set up window
		HANDLE				hndw = GetStdHandle(STD_OUTPUT_HANDLE);
		HWND				hndwin = GetConsoleWindow();
		SMALL_RECT			window = { 0, 0, w - 1, h - 1 };
		COORD				buffsize = { w, h };
		CONSOLE_CURSOR_INFO cursorinfo;
		int					winwx = GetSystemMetrics(SM_CXSCREEN) / 2;
		int					winwy = GetSystemMetrics(SM_CYSCREEN) / 2;

		//default colour palette
		COLORREF palette[16] =
		{
			0x00000000, 0x00800000, 0x00008000, 0x00808000,
			0x00000080, 0x00800080, 0x00008080, 0x00c0c0c0,
			0x00808080, 0x00ff0000, 0x0000ff00, 0x00ffff00,
			0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
		};

		//hide cursor
		GetConsoleCursorInfo(hndw, &cursorinfo);
		cursorinfo.bVisible = false;

		//setting font - 8x8 font for squareness
		SetConsolePalette(palette, 8, 8, L"");

		//setting window info
		SetConsoleTitleA(title);
		SetConsoleCursorInfo(hndw, &cursorinfo);
		SetConsoleScreenBufferSize(hndw, buffsize);
		SetConsoleWindowInfo(hndw, true, &window);
		SetWindowPos(hndwin, HWND_TOP, (winwx - (w / 2)*FONT_X_SIZE), (winwy - (h / 2)*FONT_Y_SIZE), 0, 0, SWP_NOSIZE);

		//storing info
		strcpy_s(_title, MAX_WINDOW_TITLE, title);
		_width = w;
		_height = h;

		//loading all assets for game
		bool errexit = false;
		if (!Assets::LoadRiddlesFile("Data//Riddles.txt") ||			//looks yukky yuk
			!Assets::LoadItemsFile("Data//Items.txt") ||
			!Assets::LoadMonstersFile("Data//Monsters.txt") ||
			!Assets::LoadSprites({
			"Sprites//door_left.txt",
			"Sprites//door_mid.txt",
			"Sprites//door_right.txt",
			"Sprites//door_mid_1.txt",
			"Sprites//door_mid_2.txt",
			"Sprites//door_mid_3.txt",
			"Sprites//door_mid_4.txt",
			"Sprites//human.txt",
			"Sprites//dog.txt",
			"Sprites//angel.txt",
			"Sprites//drillbot.txt", 
			"Sprites//poultry.txt", 
			"Sprites//yeti.txt", 
			"Sprites//tort.txt",
			"Sprites//riddler.txt",
			"Sprites//texty.txt"}))
			errexit = true;								//if failed to load, free memory and exit
			if (errexit){
				printf_s("Failed to load asset files.");
				getchar();
				Assets::Free();							//free whatever mem we have used
				exit(0);
			}
	}

	void Engine::OnNotify(Message& msg, Event evt)
	{
		switch (evt)
		{
		case EVENT_ENGINE_GETLENGTHS:
			(*(int*)(msg.ptr)) = _width;
			(*(int*)(msg.ptr2)) = _height;
			return;
		case EVENT_ENGINE_EXIT:
			_wantExit = true;
			return;
		}
	}

	void Engine::Run()
	{
		HANDLE hndw = GetStdHandle(STD_OUTPUT_HANDLE);
		double prev = (double)time(0), lag = 0.0, current, elapsed, fpstime = 0.0;
		int fps = 0;
		fpstime = 0;

		while (!_wantExit){
			current = (double)time(0);
			elapsed = current - prev;
			prev = current;
			lag += elapsed;
			fpstime += elapsed;
			++fps;

			_stateManager.Update();
			_stateManager.Draw();

			if (fpstime > 1){
				_currentfps = fps;
				fps = 0;
				fpstime = 0;
			}
		}
		Assets::Free();		//exiting game, free memory
	}
	/*
		Taken from www.catch22.net/sites/default/source/files/setconsoleinfo.c
		Removed all unessessary parts
	*/
	VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName)
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		/* Retrieving pointers for Windows Vista/7 Functions */
		PGetCurrentConsoleFontEx pGetCurrentConsoleFontEx = (PGetCurrentConsoleFontEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "GetCurrentConsoleFontEx");
		PSetCurrentConsoleFontEx pSetCurrentConsoleFontEx = (PSetCurrentConsoleFontEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "SetCurrentConsoleFontEx");
		PGetConsoleScreenBufferInfoEx pGetConsoleScreenBufferInfoEx = (PGetConsoleScreenBufferInfoEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "GetConsoleScreenBufferInfoEx");
		PSetConsoleScreenBufferInfoEx pSetConsoleScreenBufferInfoEx = (PSetConsoleScreenBufferInfoEx)
			GetProcAddress(GetModuleHandle("kernel32.dll"), "SetConsoleScreenBufferInfoEx");

		/* Check for pointers: if they exist, we have Vista/7 and we can use them */
		CONSOLE_SCREEN_BUFFER_INFOEX cbi;
		CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };

		/* Tell the font info how big it is, to avoid memory corruption */
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		pGetCurrentConsoleFontEx(hOutput, TRUE, &cfi);

		/* Set the font type to name indicated by wide string literal */
		/* Set to 0 to keep current settings */
		lstrcpyW(cfi.FaceName, fontName);
		cfi.dwFontSize.X = fontX;
		cfi.dwFontSize.Y = fontY;
		cfi.FontFamily = 0; /* Set to 0x30 for Terminal font */
		cfi.FontWeight = 0;

		/* Set the console font info */
		pSetCurrentConsoleFontEx(hOutput, TRUE, &cfi);

		/* Get the size of the structure before filling the struct */
		cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		pGetConsoleScreenBufferInfoEx(hOutput, &cbi);

		/* Loop through the given palette, copying the data into the color table */
		for (int i = 0; i < 16; ++i)
			cbi.ColorTable[i] = palette[i];

		/* Set the console screen buffer info */
		pSetConsoleScreenBufferInfoEx(hOutput, &cbi);
	}
}