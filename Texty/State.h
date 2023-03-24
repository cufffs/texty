#pragma once
#include "Canvas.h"
/*
	State.h - interface for all states in the game each with its own canvas to draw on!
*/
namespace texty
{
	class State{
	public:
		State(){}
		State(unsigned int x, unsigned int y, unsigned int w, unsigned int h) : _canvas(x, y, w, h){}
		virtual ~State(){}
		virtual void Update() = 0;
		virtual void Draw() = 0;
	protected:
		Canvas _canvas;
	};
}