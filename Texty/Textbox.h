#pragma once
/*
	TextString - subclass of String, handles reading input each frame.
	Textbox - Object of a textbox, reads in and writes to screen
*/
#include "Entity.h"
#include "myString.h"
namespace texty
{
	class TextString : public myString::String {
	public:
		bool	ReadFromConsole()			override;
		int		GetIntAt(int& index)		const;
		String& operator=(const char* str);
	};
	class Textbox : public Entity {
	public:
		Textbox();
		Textbox(int x, int y, int w, int h);
		~Textbox();

		//create textbox
		void Init(int x, int y, int w, int h);

		void WriteToTextbox(const char* txt, ...);	//write text to buffer so it can be rendered by renderer
		void ClearTextbox();						//clear output box only
		void ClearInputbox();						//clear input area only
		void Echo();								//echo what we type

		TextString		_input;						//public so anyone can change it easily
	private:
		TextString		_output;		//basically a buffer, i think max string is 128bytes so like 2 news whilst running vs new every call
		int				_currentLine;	//current line x)
		unsigned int	_colour;
	};
}