#pragma once

/*
	ConsoleBuffer.h - Wrapper to house CHAR_INFO* needed for low level write to console
*/

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

namespace texty
{
	class ConsoleBuffer{
	public:
		ConsoleBuffer();
		ConsoleBuffer(int w, int h);
		~ConsoleBuffer();

		//allocate/deallocate mem
		void NewBuffer(int w, int h);
		void FreeBuffer();

		bool IsStatic()					const		{ return _static; }
		void SetStatic(bool state)					{ _static = state; }

		//writing
		void Write(int x, int y, const char* txt, WORD colour);
		void Write(int x, int y, CHAR c, WORD colour);
		void Write(int index, CHAR c, WORD colour);

		//drawing functions
		void Clear();
		void Fill(CHAR c, WORD colour);
		void DrawBox(const COORD& start, int w, int h, CHAR c, WORD colour);
		void Plot(const COORD& pt, CHAR c, WORD colour);
		void HorLine(int x, int y, int w, CHAR c, WORD colour);

		//gets
		CHAR_INFO* GetBuffer();
		int GetWidth()				const	{ return _width; }
		int GetHeight()				const	{ return _height; }
		CHAR GetCharAt(int index)	const	{ return ((index >= 0 && index < _width*_height) ? _buffer[index].Char.AsciiChar : 0);	}
		WORD GetColourAt(int index) const	{ return ((index >= 0 && index < _width*_height) ? _buffer[index].Attributes : 0); }

	private:
		CHAR_INFO* _buffer;
		int _width, _height;
		bool _static;
	};
}