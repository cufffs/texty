#pragma once

/*
	Simple editor to draw ascii art and save it to a file
	**has some magic numbers and lacks comments but is kind've bonus project :P**

	I pretty much copied the design off a picture i found somewhere on the net
	really needs a load function so can alter created images!
	the colour is bugged so cant mix background and foreground colours :<

	Created all art with this for game, so works well enough

	controles:
	Draw : mouse1
	Erase : mouse2
	Wipe : shift
	Save : space
*/

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

const int WINDOW_WIDTH = 200;
const int WINDOW_HEIGHT = 100;

const int OPTION_PLANE = 18;
const int EDIT_WIDTH = WINDOW_WIDTH - OPTION_PLANE;

class Editor {
public:
	Editor();
	~Editor();

	void Start();

	int GetInput();
	void ProcessInput();
	void WriteToBuffer(int index, unsigned char c, unsigned int colour);
	void Write();

	void ClearBuffer(CHAR_INFO*, int);
	void FollowCursor(int x, int y);
	void DrawOptions();
	void Save();
private:
	HANDLE _hndW;
	HANDLE _hndR;
	INPUT_RECORD* _eventbuffer;

	bool _play;
	bool _write;

	CHAR_INFO* _buffer;
	SMALL_RECT _editwindow;

	CHAR_INFO _cursor;
	SMALL_RECT _cursorwindow;
	unsigned int _cursorFG;
	unsigned int _cursorBG;

	CHAR_INFO* _optionsbuffer;
	SMALL_RECT _optionswindow;

	void SetCursor(unsigned char c, unsigned int colour);
	void SelectOption(int x, int y);
	void GetBounds(int& startx, int& endx, int& width, int& height);
};