#include "Help.h"
#include "Observer.h"
#include "defs.h"


namespace texty
{
	const int HELP_W = 30;
	const int HELP_H = 20;
	Help::Help()
	{
		int winWidth, winHeight;
		Messager::Notify(EVENT_MESSAGE(&winWidth, &winHeight, 0), EVENT_ENGINE_GETLENGTHS);
		Create(winWidth / 2 - HELP_W / 2, winHeight / 2 - HELP_H, HELP_W, HELP_H);
		_buffer.Fill(CHAR_ALTBLOCK, BLACK);
		_buffer.DrawBox({ 0, 0 }, HELP_W, HELP_H, CHAR_ALTBLOCK, DARKPURPLE);
		SetDraw(false);
		WriteToHelp(HELP_W/ 2 - (strlen("COMMANDS") / 2), 1, "COMMANDS", RED);
		WriteHelp();
	}

	Help::~Help()
	{
	}

	//toggles draw
	void Help::Toggle()
	{
		SetDraw(!GetDraw());
	}

	//write text to help box
	void Help::WriteToHelp(int x, int y, const char* txt, unsigned int colour)
	{
		if (!txt) return;
		char* cp = (char*)txt;

		for (; *cp; ++x, ++cp){
			if (x >= _buffer.GetWidth() - 1){
				if (++y <= _buffer.GetHeight() - 1)
					x = 1;
				else return;
			}
			_buffer.Write(x + _buffer.GetWidth()*y, *cp, colour);
		}
	}

	//write contents to help box
	void Help::WriteHelp()
	{
		int centreX = _buffer.GetWidth() / 2;
		for (int y = 2, i = 0; y < _buffer.GetHeight() - 1 && i < MAX_HELPINFO; ++y, ++i){
			WriteToHelp(centreX - (strlen(helpInfos_s[i].info) / 2), y, helpInfos_s[i].info, (y % 2 ? LIGHTBLUE : WHITE));
		}
	}

	//clear inside of help box
	void Help::Clear()
	{
		for (int y = 2; y < _buffer.GetHeight() - 1; ++y){
			for (int x = 1; x < (_buffer.GetWidth() - 1); ++x){
				_buffer.Write(x + _buffer.GetWidth()*y, CHAR_BLOCK, BLACK);
			}
		}
	}

}