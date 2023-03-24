#pragma once
#include "ConsoleBuffer.h"
namespace texty {
	ConsoleBuffer::ConsoleBuffer()
		:_buffer(nullptr),
		_width(0),
		_height(0),
		_static(false)
	{
	}

	ConsoleBuffer::ConsoleBuffer(int w, int h)
		:_static(false),
		_width(w),
		_height(h)
	{
		if (w >= 0 && h >= 0){
			_buffer = new CHAR_INFO[w*h];
			Clear();
		}
	}

	ConsoleBuffer::~ConsoleBuffer()
	{
		if (!_static)
			delete[]_buffer;
	}

	//allocate new buffer
	void ConsoleBuffer::NewBuffer(int w, int h)
	{
		if (!_static){
			delete[]_buffer;
			_buffer = nullptr;
		}
		_buffer = new CHAR_INFO[w*h];
		_width = w;
		_height = h;
		Clear();
	}

	void ConsoleBuffer::FreeBuffer()
	{
		if (_buffer)
			delete[]_buffer;
		_buffer = nullptr;
		_static = false;
	}

	CHAR_INFO* ConsoleBuffer::GetBuffer()
	{
		return _buffer;
	}

	//write text to buffer
	void ConsoleBuffer::Write(int x, int y, const char* txt, WORD colour)
	{
		if (!txt || !*txt) return;
		char* cp = (char*)txt;

		for (; *cp; ++x, ++cp){
			if (x >= _width){
				if (++y <= _height)
					x = 0;
				else return;
			}
			Write(x, y, (CHAR)*cp, colour);
		}
	}

	//write char to buffer
	void ConsoleBuffer::Write(int x, int y, CHAR c, WORD colour)
	{
		if (x < _width && y < _height){
			_buffer[x + _width*y].Char.AsciiChar = c;
			_buffer[x + _width*y].Attributes = colour;
		}
	}

	//write char to buffer, 1d index
	void ConsoleBuffer::Write(int index, CHAR c, WORD colour)
	{
		if (index < _width*_height){
			_buffer[index].Char.AsciiChar = c;
			_buffer[index].Attributes = colour;
		}
	}

	//sets all buffer to transparent
	void ConsoleBuffer::Clear()
	{
		int len = _width*_height;
		while (len){
			_buffer[--len].Char.AsciiChar = 0;
			_buffer[len].Attributes = 0;
		}
	}

	//fill buffer with a char
	void ConsoleBuffer::Fill(CHAR c, WORD colour)
	{
		int len = _width*_height;
		while (len){
			_buffer[--len].Char.AsciiChar = c;
			_buffer[len].Attributes = colour;
		}
	}

	//plot a point on buffer
	void ConsoleBuffer::Plot(const COORD& pt, CHAR c, WORD colour)
	{
		int index = pt.X + _width*pt.Y;			//calcs index of point to our buffer
		_buffer[index].Char.AsciiChar = c;
		_buffer[index].Attributes = colour;
	}

	//draw a horizontal line on buffer
	void ConsoleBuffer::HorLine(int x, int y, int w, CHAR c, WORD colour)
	{
		if (y < _height){
			while (x < _width && x != w)
				Plot({ x++, y }, c, colour);
		}
	}
	
	//draw a box on buffer
	void ConsoleBuffer::DrawBox(const COORD& start, int w, int h, CHAR c, WORD colour) //draws box
	{
		h = start.Y + h;
		w = start.X + w;
		for (int y = start.Y; y < h && y <= _height; ++y){
			for (int x = start.X; x < w && x <= _width; ++x){
				if (y == start.Y || x == start.X || x == w - 1 || y == h - 1)
					Plot({ x, y }, c, colour);
			}
		}
	}
}