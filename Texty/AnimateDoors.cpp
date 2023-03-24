#include "AnimateDoors.h"
#include "Assets.h"
#include "Observer.h"
namespace texty
{
#define GET_SPRITE Assets::s_sprites		//make code a bit easier to read
	const int MOVE_LEFT = 0;
	const int MOVE_MID = 1;
	const int MOVE_RIGHT = 2;

	AnimateDoors::AnimateDoors(int way)
		:_totalframe(0),
		_bSideDoor(false)
	{
		Messager::Notify(EVENT_MESSAGE(&_ww, &_wh, 0), EVENT_ENGINE_GETLENGTHS); //mmmm

		_canvas.Create(0, 0, _ww, _wh);
		_centreX = _ww / 2;

		static int leftx = GET_SPRITE[DOOR_LEFT].GetWidth() / 2;
		static int lefty = _wh / 2 - (GET_SPRITE[DOOR_LEFT].GetHeight() / 2);

		static int midx = _ww / 2 - (GET_SPRITE[DOOR_MID].GetWidth() / 2);
		static int midy = GET_SPRITE[DOOR_MID].GetHeight() / 2;

		static int rightx = _ww - GET_SPRITE[DOOR_LEFT].GetWidth() - (GET_SPRITE[DOOR_LEFT].GetWidth());
		static int righty = _wh / 2 - (GET_SPRITE[DOOR_LEFT].GetHeight() / 2);


		_door.SetBuffer(GET_SPRITE[DOOR_MID]);

		if (!way || way == MOVE_RIGHT){
			_bSideDoor = true;
			way ? _door.SetPos(rightx, righty) : _door.SetPos(leftx, lefty);
			_door.MoveTo(midx, midy);
		}
		else {
			_door.SetPos(midx, midy);
		}

		_canvas.Insert(_door.GetId(), &_door);
	}

	AnimateDoors::~AnimateDoors()
	{
	}

	void AnimateDoors::Update()
	{
		++_totalframe;
		switch (_totalframe)
		{
		case 20:							//after 20 frames - if move forward only
		{
			if (!_bSideDoor){
				_door.SetBuffer(GET_SPRITE[DOOR_MID_1]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_1].GetWidth() / 2), 0);
			}
			break;
		}
		case 40:								//after 40 frames
		{
			if (!_bSideDoor){					//if move forward only
				_door.SetBuffer(GET_SPRITE[DOOR_MID_2]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_2].GetWidth() / 2), 0);
			}
			break;
		}
		case 60:								//after 60 frames
		{
			if (_bSideDoor){					//just finished moving left or right
				_door.SetBuffer(GET_SPRITE[DOOR_MID]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID].GetWidth() / 2), 0);
			}
			else {								//moving forward only
				_door.SetBuffer(GET_SPRITE[DOOR_MID_3]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_3].GetWidth() / 2), 0);
			}
			break;
		}
		case 80:								//after 80 frames
		{
			if (_bSideDoor){
				_door.SetBuffer(GET_SPRITE[DOOR_MID_1]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_1].GetWidth() / 2), 0);
			}
			else{							//finished moving forward only
				_door.SetBuffer(GET_SPRITE[DOOR_MID_4]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_4].GetWidth() / 2), 0);
			}
			break;
		}
		case 100:							//after 100 frames
		{
			if (_bSideDoor){
				_door.SetBuffer(GET_SPRITE[DOOR_MID_2]);
				_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_2].GetWidth() / 2), 0);
			}
			else
				Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);
			break;
		}
		case 120:
			_door.SetBuffer(GET_SPRITE[DOOR_MID_3]);
			_door.SetPos(_ww / 2 - (GET_SPRITE[DOOR_MID_3].GetWidth() / 2), 0);
			break;
		case 140:
			Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);
			break;
		}
		_canvas.Update();
	}

	void AnimateDoors::Draw()
	{
		_canvas.Render();
	}
}