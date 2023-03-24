#include "Textbox.h"
#include "Observer.h"
#include <conio.h>
#include <stdlib.h>
#include <cstdio>
#include "defs.h"
using namespace myString;
namespace texty
{
	bool TextString::ReadFromConsole()
	{
		if (_kbhit()) {
			unsigned char c = (unsigned char)_getch();
			switch (c)
			{
			case KEYS_BS:
				this->RemoveLast();
				break;
			case KEYS_RET:
				return true;
			case KEYS_ESC:
				Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);
				break;
			default:
				this->Append(c);
				break;
			}
		}
		return false;
	}

	int TextString::GetIntAt(int& index) const
	{
		if (index > _strlen(_cstr) || !_cstr || !*_cstr) return -1;	//some checks
		char* cp = (char*)(_cstr + index), number[5] = { 0 };		//set charpointer to current spot, and a char array size32
		char* c1 = (char*)number;			//set char1 to start of number
		int offset = 1;

		if (*cp++ == '_'){				//if its in right format
			while (_isint(*cp)){		//while there is a number
				*c1++ = *cp++;			//copy number to number
				++offset;
			}
			if (*cp == '_'){			//if its in right format
				index += offset + 1;	//increment index to just past the _number_
				return atoi(number);	//im such a cheater :(
			}
		}
		return -1;	//not a number or not in right format _e_ or _1d_ or _13 etc
	}

	String& TextString::operator=(const char* str)
	{
		if (!*str){					//if we just type string = "" then it means i want to free memory
			delete[]_cstr;
			_cstr = nullptr;			//so destructor is safe
		}
		else
			_strcpy(str);			//copy string (handles memory alloc and everything)
		return *this;
	}

	Textbox::Textbox()
		:_currentLine(1),
		_colour(15)
	{

	}

	Textbox::Textbox(int x, int y, int w, int h)
		:Entity(x, y, w, h),
		_currentLine(1),
		_colour(15)
	{
		_buffer.DrawBox({ 0, 0 }, w, h, CHAR_ALTBLOCK, DARKPURPLE);
		_buffer.HorLine(1, h - 3, w - 1, CHAR_HYPHEN, DARKPURPLE);
		_buffer.Plot({ 1, h - 2 }, CHAR_RIGHTARROW, LIGHTBLUE);
	}

	void Textbox::Init(int x, int y, int w, int h)
	{
		Create(x, y, w, h);
		_buffer.DrawBox({ 0, 0 }, w, h, CHAR_ALTBLOCK, DARKPURPLE);
		_buffer.HorLine(1, h - 3, w - 1, CHAR_HYPHEN, DARKPURPLE);
		_buffer.Plot({ 1, h - 2 }, CHAR_RIGHTARROW, LIGHTBLUE);
	}


	Textbox::~Textbox()
	{

	}

	void Textbox::WriteToTextbox(const char* txt, ...)
	{
		//i would like to be able to change colour on the fly, _#_ where # represents colour code
		char buffer[512] = { 0 };
		va_list va;
		va_start(va, txt);
		vsprintf_s(buffer, sizeof(buffer), txt, va);
		va_end(va);

		_output = buffer;

		if (_output.Length()){								//if there is text to write
			if (_currentLine > _buffer.GetHeight() - 4){	//if there are no more lines, then clear box and restart from line 1
				_currentLine = 1;
				ClearTextbox();
			}
			for (int x = _posx + 1, i = 0; i<_output.Length() && x < _buffer.GetWidth()+_posx - 1; ++x, ++i){
				if (_output.CharAt(i) == '-' && _output.CharAt(i + 1) == 'n'){		//if newline then add a new line
					i += 2;
					++_currentLine;
					x = _posx + 1;
				}
				if (_output.CharAt(i) == '_'){			//see if colour code in string
					_colour = _output.GetIntAt(i);		//set our colour
					if (_colour == -1)					//if it wasnt a colour code, set it back to default
						_colour = 15;
				}
				_buffer.Write(x + _buffer.GetWidth()*_currentLine - _posx, _output.CharAt(i), _colour);
			}
			_currentLine++;		//increment line
		}
	}

	void Textbox::ClearTextbox()
	{
		_currentLine = 1;
		for (int y = 1; y < _buffer.GetHeight() - 3; ++y){
			for (int x = 1; x < (_buffer.GetWidth() - 1); ++x){
				_buffer.Write(x + _buffer.GetWidth()*y, 0, 0);
			}
		}
	}

	void Textbox::ClearInputbox()
	{
		const static int echoline = _buffer.GetHeight() - 2;
		if (_input.Length()){
			for (int x = _posx + 2, i = 0; x < _buffer.GetWidth() - 1; ++x, ++i){
				_buffer.Write(x + _buffer.GetWidth()*echoline - _posx, 0, 0);
			}
		}
	}

	void Textbox::Echo()
	{
		const static int echoline = _buffer.GetHeight() - 2;
		if (_input.Length()){
			for (int x = _posx + 2, i = 0; x < _input.Length() + 2 + _posx; ++x, ++i){
				_buffer.Write(x + _buffer.GetWidth()*echoline - _posx, _input.CharAt(i), 15);
			}
		}
	}
}