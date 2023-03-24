#include "Canvas.h"
#include "Entity.h"
#include "Engine.h"

namespace texty{
	Canvas::Canvas()
		:_current(nullptr),
		_back(nullptr),
		_posx(0),
		_posy(0),
		_canvasWidth(0),
		_canvasHeight(0)
	{
	}

	Canvas::Canvas(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		_canvas[0].NewBuffer(w, h);
		_canvas[1].NewBuffer(w, h);
		_current = &_canvas[0];
		_back = &_canvas[1];
		_window = { x, y, w, h };
		_posx = x;
		_posy = y;
		_canvasWidth = w;
		_canvasHeight = h;
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::Create(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		_canvas[0].NewBuffer(w, h);
		_canvas[1].NewBuffer(w, h);
		_current = &_canvas[0];
		_back = &_canvas[1];
		_window = { x, y, w, h };
		_posx = x;
		_posy = y;
		_canvasWidth = w;
		_canvasHeight = h;
	}

	void Canvas::Update()
	{
		Entity* cur = nullptr;
		Entity* next = nullptr;
		int bufheight, bufwidth;
		_back->Clear();
		for (auto it = _renderMap.begin(); it != _renderMap.end(); ++it){
			cur = it->second;
			if (!cur->GetDraw()) continue;
			bufheight = cur->GetBuffer()->GetHeight();
			bufwidth = cur->GetBuffer()->GetWidth();
			cur->Move();
			for (int y = 0, drawind = 0; y < bufheight && y < _back->GetHeight(); ++y){
				for (int x = 0; x < bufwidth && y < _back->GetWidth(); ++x, ++drawind){
					if (cur->GetBuffer()->GetColourAt(drawind) || cur->GetBuffer()->GetCharAt(drawind))
						_back->Write(cur->GetX() + x, cur->GetY() + y, cur->GetBuffer()->GetCharAt(drawind), cur->GetBuffer()->GetColourAt(drawind));
				}
			}
		}
	}

	void Canvas::Render()
	{	
		Swap();
		HANDLE hndw = GetStdHandle(STD_OUTPUT_HANDLE);
		WriteConsoleOutputA(hndw, _current->GetBuffer(), { _canvasWidth, _canvasHeight }, { _posx, _posy }, &_window);
	}

	void Canvas::Swap()
	{
		ConsoleBuffer* tmp = _back;
		_back = _current;
		_current = tmp;
	}

	void Canvas::Insert(int id, Entity* obj)
	{
		_renderMap.insert(std::make_pair(id, obj));
	}

	void Canvas::Remove(int id)
	{
		auto it = _renderMap.find(id);
		if (it != _renderMap.end()){
			_renderMap.erase(id);
		}
	}

	void Canvas::WipeList()
	{
		for (auto it = _renderMap.begin(); it != _renderMap.end(); it++){
			_renderMap.erase(it);
		}
	}
	
	/*
		Gets
	*/

	int Canvas::GetPosX()
	{
		return _posx;
	}

	int Canvas::GetPosY()
	{
		return _posy;
	}

	int Canvas::GetWidth()
	{
		return _canvasWidth;
	}

	int Canvas::GetHeight()
	{
		return _canvasHeight;
	}

}