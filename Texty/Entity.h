#pragma once
#include "ConsoleBuffer.h"
/*
	Entity.h - Adds basic ability to consoleBuffer, with move, and position etc
	All drawable objects either contain or derive from this,
	---removed collision as wasnt doing much but making code more complex
*/
namespace texty{
	class Entity{
	public:
		Entity();
		Entity(int x, int y, int w, int h);
		virtual ~Entity();

		virtual void Create(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

		virtual void Move();
		virtual void MoveTo(int x, int y);

		//draw
		void Fill(CHAR c, WORD colour);


		//sets
		void			SetBuffer(ConsoleBuffer&);
		void			SetV(int x, int y)		{ _vx = x; _vy = y; }
		void			SetPos(int x, int y)	{ _posx = x; _posy = y; }
		void			SetDraw(bool state)		{ _drawn = state; }
		void			ReverseV()				{ _vx *= -1; _vy *= -1; }

		//gets
		bool			IsEmpty(int index);
		ConsoleBuffer*	GetBuffer()				{ return &_buffer; }
		int				GetId()			const	{ return _id; }
		int				GetX()			const	{ return _posx; }
		int				GetY()			const	{ return _posy; }
		bool			GetDraw()		const	{ return _drawn; }
		bool			IsMoving()		const	{ return (_vx || _vy); }



	protected:
		ConsoleBuffer _buffer;
		//position onscreen
		int _posx, _posy;
		//velocities
		int _vx, _vy;
		//unique id
		int _id;
		//is it drawn
		bool _drawn;

		//moveto
		int	_x, _y;

		static int s_idGen;
	};
}