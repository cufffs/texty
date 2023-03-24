#include "Entity.h"
#include "Engine.h"
#include "Observer.h"

namespace texty{
	int Entity::s_idGen = 0;

	Entity::Entity()
		:_buffer(),
		_posx(0),
		_posy(0),
		_vx(0),
		_vy(0),
		_id(0),
		_x(-1),
		_y(-1),
		_drawn(true)
	{
		_id = ++s_idGen;
	}

	Entity::Entity(int x, int y, int w, int h)
		:_buffer(w,h),
		_posx(x),
		_posy(y),
		_vx(0),
		_vy(0),
		_x(-1),
		_y(-1),
		_drawn(true)
	{
		_id = ++s_idGen;
	}

	Entity::~Entity()
	{

	}

	void Entity::Create(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		_buffer.NewBuffer(w, h);
		_posx = x;
		_posy = y;
	}

	void Entity::SetBuffer(ConsoleBuffer& cb)
	{
		if (!_buffer.IsStatic())
			_buffer.FreeBuffer();
		_buffer = cb;
		_buffer.SetStatic(true);
	}


	void Entity::Fill(CHAR c, WORD colour)
	{
		_buffer.Fill(c, colour); 
	}

	bool Entity::IsEmpty(int index)
	{
		return (index >= 0 && index < _buffer.GetHeight()*_buffer.GetWidth() && !_buffer.GetCharAt(index));
	}

	void Entity::Move()
	{
		static int winWidth = Engine::Get()->GetWidth();
		static int winHeight = Engine::Get()->GetHeight();

		if (!IsMoving()) return;

		if (_posy == _y){		//for moveto stuff
			_vy = 0;
			_y = -1;
		}
		if (_posx == _x){
			_vx = 0;
			_x = -1;
		}

		if (_posx + _vx < 0 || _posx + _vx + _buffer.GetWidth() > winWidth)
			_vx *= -1;
		if (_posy + _vy < 0 || _posy + _vy + _buffer.GetHeight() > winHeight)
			_vy *= -1;

		_posx += _vx;
		_posy += _vy;
	}

	void Entity::MoveTo(int x, int y)
	{
		if (x > _posx)
			_vx = 1;
		else if (x < _posx)
			_vx = -1;
		else
			_vx = 0;

		if (y > _posy)
			_vy = 1;
		else if (y < _posy)
			_vy = -1;
		else
			_vy = 0;

		_x = x;
		_y = y;
	}
}