#pragma once
#include <unordered_map>
#include "ConsoleBuffer.h"

/*
	Canvas.h - Defines a canvas, with double buffering, ability to draw things to screen
*/

namespace texty{
	class Entity;
	class Canvas {
	public:
		Canvas();
		Canvas(unsigned int, unsigned int, unsigned int, unsigned int);
		virtual ~Canvas();
		void Create(unsigned int, unsigned int, unsigned int, unsigned int);
		void Update();
		void Render();
		void Swap();
		void Insert(int id, Entity* obj);
		void Remove(int id);
		void WipeList();

		//gets
		int GetPosX();
		int GetPosY();
		int GetWidth();
		int GetHeight();

	private:
		ConsoleBuffer _canvas[2];
		ConsoleBuffer* _current;
		ConsoleBuffer* _back;

		SMALL_RECT _window;

		std::unordered_map < int, Entity* > _renderMap;

		int _posx, _posy;
		int _canvasWidth, _canvasHeight;
	};
}