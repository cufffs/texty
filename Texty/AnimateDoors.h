#pragma once
#include "State.h"
#include "Entity.h"
/*
	Animatedoors.h - hanndles room animation to make gamestate class a little less bloated
*/
namespace texty
{
	class AnimateDoors : public State{
	public:
		AnimateDoors(int);
		virtual ~AnimateDoors();
		void Update();
		void Draw();
	private:
		AnimateDoors(){}
		int _totalframe;
		bool _bSideDoor;
		Entity _door;
		int _centreX, _ww, _wh;
	};
}
