#include "Editor.h"

#define DEFAULT_FONT_X 8
#define DEFAULT_FONT_Y 8
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
/*
	Taken from SetConsolePalette and related typedefs from
	www.catch22.net/sites/default/source/files/setconsoleinfo.c
*/
VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName);
typedef BOOL(WINAPI *PGetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
typedef BOOL(WINAPI *PSetCurrentConsoleFontEx)(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
typedef BOOL(WINAPI *PGetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);
typedef BOOL(WINAPI *PSetConsoleScreenBufferInfoEx)(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx);

Editor::Editor()
	:_write(false),
	 _play(true),
	 _cursorwindow({ 0, 0, 0, 0 }),
	 _cursorFG(0),
	 _cursorBG(0)
{
	_hndW = GetStdHandle(STD_OUTPUT_HANDLE);
	_hndR = GetStdHandle(STD_INPUT_HANDLE);

	_buffer = new CHAR_INFO[EDIT_WIDTH*WINDOW_HEIGHT];
	_editwindow = { OPTION_PLANE, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };
	ClearBuffer(_buffer, EDIT_WIDTH*WINDOW_HEIGHT);

	_optionsbuffer = new CHAR_INFO[OPTION_PLANE*WINDOW_HEIGHT];
	_optionswindow = { 0, 0, OPTION_PLANE - 1, WINDOW_HEIGHT - 1 };
	ClearBuffer(_optionsbuffer, OPTION_PLANE*WINDOW_HEIGHT);
	DrawOptions();

	COLORREF palette[16] =
	{
		0x00000000, 0x00800000, 0x00008000, 0x00808000,
		0x00000080, 0x00800080, 0x00008080, 0x00c0c0c0,
		0x00808080, 0x00ff0000, 0x0000ff00, 0x00ffff00,
		0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
	};
	SetConsolePalette(palette, 8, 8, L"");

	SetCursor(0, 255);

	HWND WINDOW_HANDLE = GetConsoleWindow();
	SMALL_RECT window = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };
	COORD buffsz = { WINDOW_WIDTH, WINDOW_HEIGHT };

	CONSOLE_CURSOR_INFO cursorInfo;					//hide blinking cursor
	GetConsoleCursorInfo(_hndW, &cursorInfo);
	cursorInfo.bVisible = false;

	SetConsoleTitleA("EditMe");						//set all window stuff
	SetConsoleCursorInfo(_hndW, &cursorInfo);
	SetConsoleScreenBufferSize(_hndW, buffsz);
	SetConsoleWindowInfo(_hndW, TRUE, &window);

	int posx = GetSystemMetrics(SM_CXSCREEN) / 2;	//center window
	int posy = GetSystemMetrics(SM_CYSCREEN) / 2;
	SetWindowPos(WINDOW_HANDLE, HWND_TOP, (posx - (WINDOW_WIDTH / 2)*DEFAULT_FONT_X), (posy - (WINDOW_HEIGHT / 2)*DEFAULT_FONT_Y), 0, 0, SWP_NOSIZE);
}

Editor::~Editor()
{
	delete[]_buffer;
	delete[]_optionsbuffer;
}

void Editor::Start()
{
	while (_play){
		ProcessInput();
		Write();
	}
}

int Editor::GetInput()
{
	DWORD numevents = 0;
	DWORD numread = 0;

	GetNumberOfConsoleInputEvents(_hndR, &numevents);

	if (numevents){
		_eventbuffer = new INPUT_RECORD[numevents];
		ReadConsoleInput(_hndR, _eventbuffer, numevents, &numread);
	}
	return numread;
}

//processes input
void Editor::ProcessInput()
{
	int numread = GetInput();
	int x, y;
	bool ctrldown = false;
	if (numread){
		for (int i = 0; i < numread; ++i){
			switch (_eventbuffer[i].EventType)
			{
				case KEY_EVENT:
				switch (_eventbuffer[i].Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_ESCAPE:
					_play = false;
					break;
				case VK_SPACE:
					Save();
					break;
				case VK_SHIFT:
					for (int y = 0; y < WINDOW_HEIGHT; ++y){
						for (int x = 0; x < EDIT_WIDTH; ++x){
							_buffer[x + EDIT_WIDTH*y].Attributes = 155;
						}
					}
					//ClearBuffer(_buffer, EDIT_WIDTH*WINDOW_HEIGHT);
					_write = true;
					break;
				case VK_ACCEPT:
					for (int y = 0; y < WINDOW_HEIGHT; ++y){
						for (int x = 0; x < EDIT_WIDTH; ++x){
							_buffer[x + EDIT_WIDTH*y].Attributes = 255;
						}
					}
					break;
				break;
				}
			case MOUSE_EVENT:
				x = _eventbuffer[i].Event.MouseEvent.dwMousePosition.X;
				y = _eventbuffer[i].Event.MouseEvent.dwMousePosition.Y;
				FollowCursor(x, y);
				switch (_eventbuffer[i].Event.MouseEvent.dwButtonState)
				{
				case FROM_LEFT_1ST_BUTTON_PRESSED:
					if (x > OPTION_PLANE){
						if (ctrldown){
							_cursor.Attributes = _buffer[(x - OPTION_PLANE) + y*EDIT_WIDTH].Attributes;
							_cursor.Char.AsciiChar = _buffer[(x - OPTION_PLANE) + y*EDIT_WIDTH].Char.AsciiChar;
						}
						else{
							WriteToBuffer((x - OPTION_PLANE) + y*EDIT_WIDTH, _cursor.Char.AsciiChar, _cursor.Attributes);
							_write = true;
						}
					}
					else {	//in options
						SelectOption(x,y);
					}
					break;
				case RIGHTMOST_BUTTON_PRESSED:
					if (x > OPTION_PLANE){
						WriteToBuffer((x - OPTION_PLANE) + y*EDIT_WIDTH, 0, 0);
						_write = true;
					}
					else {	//in options
						SelectOption(x, y);
					}
					break;
				}
			}
		}
		delete _eventbuffer;
	}
}

void Editor::WriteToBuffer(int index, unsigned char c, unsigned int colour)
{
	_buffer[index].Char.AsciiChar = c;
	_buffer[index].Attributes = colour;
}

void Editor::Write()	//writes the buffer to screen, bypass renderer
{
	if (_write){
		WriteConsoleOutputA(_hndW, _buffer, { EDIT_WIDTH, WINDOW_HEIGHT }, { 0, 0 }, &_editwindow);
		_write = false;
	}
	if (_buffer[(_cursorwindow.Left - OPTION_PLANE) + EDIT_WIDTH*_cursorwindow.Top].Attributes == 0 &&
		_optionsbuffer[(_cursorwindow.Left) + OPTION_PLANE*_cursorwindow.Top].Attributes == 0){
		WriteConsoleOutputA(_hndW, &_cursor, { 1, 1 }, { 0, 0 }, &_cursorwindow);
	}
}

void Editor::ClearBuffer(CHAR_INFO* buf, int size)
{
	for (int i = 0; i < size; ++i){
		buf[i].Char.AsciiChar = 0;
		buf[i].Attributes = 0;
	}
}

void Editor::FollowCursor(int x, int y)
{
	static CHAR_INFO tmp;
	tmp.Char.AsciiChar = 0;
	if (_buffer[(_cursorwindow.Left - OPTION_PLANE) + EDIT_WIDTH*_cursorwindow.Top].Attributes == 0 &&
		_optionsbuffer[(_cursorwindow.Left) + OPTION_PLANE*_cursorwindow.Top].Attributes == 0)
		WriteConsoleOutputA(_hndW, &tmp, { 1, 1 }, { 0, 0 }, &_cursorwindow);
	else {
		tmp.Attributes = 0;
	}
	tmp.Attributes = 0;
	_cursorwindow.Left = x;
	_cursorwindow.Top = y;
	_cursorwindow.Right = x;
	_cursorwindow.Bottom = y;
}


void Editor::SetCursor(unsigned char c, unsigned int colour)
{
	_cursor.Char.AsciiChar = c;
	_cursor.Attributes = colour;
}

void Editor::DrawOptions()
{
	unsigned char startc = 0x0001;
	for (int x = 1; x < 8; ++x, startc += 0x0001){
		_optionsbuffer[x + OPTION_PLANE * 3].Char.AsciiChar = (CHAR)219;
		_optionsbuffer[x + OPTION_PLANE * 3].Attributes = startc;
		_optionsbuffer[x + OPTION_PLANE * 4].Char.AsciiChar = (CHAR)219;
		_optionsbuffer[x + OPTION_PLANE*4].Attributes = startc + 0x0008;
	}
	startc = 0x0010;
	for (int x = 1; x < 8; ++x, startc += 0x0010){
		_optionsbuffer[x+OPTION_PLANE*6].Attributes = startc;
		_optionsbuffer[x+OPTION_PLANE*7].Attributes = startc+0x0080;
	}


	for (int y = 0, i = 0; i < 255; ++y){
		for (int x = 0; x < OPTION_PLANE-1 && i < 255; ++x, ++i){
			_optionsbuffer[(x + 1) + OPTION_PLANE*(y + 9)].Attributes = 15;
			_optionsbuffer[(x + 1) + OPTION_PLANE*(y + 9)].Char.AsciiChar = (unsigned char)i;
		}
	}
	WriteConsoleOutputA(_hndW, _optionsbuffer, { OPTION_PLANE, WINDOW_HEIGHT }, { 0, 0 }, &_optionswindow);
}

void Editor::SelectOption(int x, int y)
{
	if (y <= 3){	//foreground;
		_cursorFG = _optionsbuffer[x + OPTION_PLANE*y].Attributes;
		_cursor.Attributes = _cursorFG | _cursorBG;
	}
	else if (y <= 8){
		_cursorBG = _optionsbuffer[x + OPTION_PLANE*y].Attributes;
		_cursor.Attributes = _cursorFG | _cursorBG;
	}
	else{
		_cursor.Char.AsciiChar = _optionsbuffer[x + OPTION_PLANE*y].Char.AsciiChar;
	}
}

//saves a square around the area drawn to file
void Editor::Save()
{
	char save;
	char fname[64] = { 0 };

	std::cout << "Save?(y/n)";
	std::cin >> save;
	if (save != 'y')return;
	std::cout << "Enter Filename: ";
	std::cin >> fname;
	strcat_s(fname, ".txt");

	int startx = 0, starty = 0, width = 0, height = 0;
	GetBounds(startx, starty, width, height);

	std::ofstream fs;
	fs.open(fname, std::fstream::out);

	fs << width << ' ' << height << '\n';

	for (int y = starty; y < starty + height; ++y){
		for (int x = startx; x < startx + width; ++x){
			int q = (_buffer[x + EDIT_WIDTH*y].Char.AsciiChar ? _buffer[x + EDIT_WIDTH*y].Char.AsciiChar : 0);
			fs << q << ' ';
			fs << (_buffer[x + EDIT_WIDTH*y].Attributes ? (int)_buffer[x + EDIT_WIDTH*y].Attributes : 0) << '\t';
		}
		fs << '\n';
	}
	
	fs.close();
	DrawOptions();
}

//finds the smallest square around a drawn area
void Editor::GetBounds(int& startx, int& starty, int& width, int& height)
{
	bool unessessarylongflagnameinsteadofgotostatement = false;
	for (int x = 0; x < EDIT_WIDTH && !unessessarylongflagnameinsteadofgotostatement; ++x){
		for (int y = 0; y < WINDOW_HEIGHT && !unessessarylongflagnameinsteadofgotostatement; ++y){
			if (_buffer[x + EDIT_WIDTH*y].Attributes || _buffer[x + EDIT_WIDTH*y].Char.AsciiChar){
				startx = x;
				unessessarylongflagnameinsteadofgotostatement = true;
			}
		}
	}

	unessessarylongflagnameinsteadofgotostatement = false;

	for (int y = 0; y < WINDOW_HEIGHT && !unessessarylongflagnameinsteadofgotostatement; ++y){
		for (int x = 0; x < EDIT_WIDTH && !unessessarylongflagnameinsteadofgotostatement; ++x){
			if (_buffer[x + EDIT_WIDTH*y].Attributes || _buffer[x + EDIT_WIDTH*y].Char.AsciiChar){
				starty = y;
				unessessarylongflagnameinsteadofgotostatement = true;
			}
		}
	}

	unessessarylongflagnameinsteadofgotostatement = false;

	for (int x = EDIT_WIDTH - 1; x >= 0 && !unessessarylongflagnameinsteadofgotostatement; --x){
		for (int y = 0; y < WINDOW_HEIGHT && !unessessarylongflagnameinsteadofgotostatement; ++y){
			if (_buffer[x + EDIT_WIDTH*y].Attributes || _buffer[x + EDIT_WIDTH*y].Char.AsciiChar){
				width = x-startx+1;
				unessessarylongflagnameinsteadofgotostatement = true;
			}
		}
	}

	for (int y = WINDOW_HEIGHT - 1; y >= 0; --y){
		for (int x = 0; x < EDIT_WIDTH; ++x){
			if (_buffer[x + EDIT_WIDTH*y].Attributes || _buffer[x + EDIT_WIDTH*y].Char.AsciiChar){
				height = y-starty+1;
				return;
			}
		}
	}
}

/*
	Taken from SetConsolePalette and related typedefs from
	www.catch22.net/sites/default/source/files/setconsoleinfo.c
*/
VOID WINAPI SetConsolePalette(COLORREF palette[], int fontX, int fontY, wchar_t *fontName)
{
	int i;
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
		for (i = 0; i < 16; ++i)
			cbi.ColorTable[i] = palette[i];

		/* Set the console screen buffer info */
		pSetConsoleScreenBufferInfoEx(hOutput, &cbi);
}