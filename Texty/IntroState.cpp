#include "IntroState.h"
#include "GameState.h"
#include "Observer.h"
#include "defs.h"
#include <conio.h>

namespace texty
{
	IntroState::IntroState()
		:_play(false)
	{
		Messager::Notify(EVENT_MESSAGE(&_windowWidth, &_windowHeight, 0), EVENT_ENGINE_GETLENGTHS);

		_playAlignX = _windowWidth / 2 - 15;
		_playAlignY = _windowHeight / 2 - 10;

		_canvas.Create(0, 0, _windowWidth, _windowHeight);

		for (int i = 0; i < MAX_PLAYTHING; ++i){
			_plaything[i].Create(rand() % _windowWidth, rand() % _windowHeight, 1, 1);
			_plaything[i].Fill(CHAR_BLOCK, (i < 93 ? WHITE : rand() % COLOUR_MAX));
			_plaything[i].SetV(1, 1);
			_canvas.Insert(_plaything[i].GetId(), &_plaything[i]);
		}
		_logo.SetBuffer(Assets::s_sprites[TEXTY]);
		_logo.SetPos(_windowWidth / 2 - _logo.GetBuffer()->GetWidth() / 2, _windowHeight / 2 - _logo.GetBuffer()->GetHeight() / 2);
		_canvas.Insert(_logo.GetId(), &_logo);
	}

	IntroState::~IntroState()
	{
	}

	void IntroState::Update()
	{
		_canvas.Update();
		if (_kbhit() != 0) {
			if (_getch() == KEYS_ESC){
				Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);
			}
			else if (!_play){
				_logo.SetDraw(false);
				DrawPlay();	
				_play = true;
			}
			else{
				_play = false;
				//setting up this state so when we return it looks nicer
				_logo.SetDraw(true);
				for (int i = 0; i < MAX_PLAYTHING; ++i)	
					_plaything[i].SetV((i % 2 ? 1 : -1), (i % 2 ? 1 : -1));
				//push new gamestate on
				Messager::Notify(EVENT_MESSAGE(new GameState(), nullptr, 0), EVENT_STATE_PUSH);
			}
		}
	}

	void IntroState::Draw()
	{
		_canvas.Render();
	}

	void IntroState::DrawPlay()
	{

		//VERTICAL LINES ONLY -- could have probably done it much shorter but the loop winding might be more fastest :p
		for (int i = 0; i < 7; ++i){
			_plaything[i].MoveTo(_playAlignX, _playAlignY + i);					//P
			_plaything[i + 7].MoveTo(_playAlignX + 1, _playAlignY + i);			//P
			_plaything[i + 14].MoveTo(_playAlignX + 9, _playAlignY + i);			//L
			_plaything[i + 21].MoveTo(_playAlignX + 10, _playAlignY + i);			//L
			_plaything[i + 28].MoveTo(_playAlignX + 16, _playAlignY + i);			//A
			_plaything[i + 35].MoveTo(_playAlignX + 17, _playAlignY + i);			//A
			_plaything[i + 42].MoveTo(_playAlignX + 21, _playAlignY + i);			//A
			_plaything[i + 49].MoveTo(_playAlignX + 22, _playAlignY + i);			//A
			if (i < 4){																			//todo: remove yukky branching in loop
				_plaything[i + 56].MoveTo(_playAlignX + 5, _playAlignY + i);		//P			//edit: compiler may do it for me
				_plaything[i + 60].MoveTo(_playAlignX + 6, _playAlignY + i);		//P
			}
			else{
				_plaything[i + 60].MoveTo(_playAlignX + 28, _playAlignY + i);		//Y
				_plaything[i + 63].MoveTo(_playAlignX + 29, _playAlignY + i);		//Y
			}
		}
		//horizontal lines
		for (int i = 0, j = 70; i < 3; ++i, ++j){
			_plaything[j].MoveTo(_playAlignX + i + 2, _playAlignY);					//P
			_plaything[j + 3].MoveTo(_playAlignX + i + 2, _playAlignY + 3);			//P
			_plaything[j + 6].MoveTo(_playAlignX + i + 11, _playAlignY + 6);		//L
			_plaything[j + 9].MoveTo(_playAlignX + i + 18, _playAlignY);			//A
			_plaything[j + 12].MoveTo(_playAlignX + i + 18, _playAlignY + 3);		//A
			_plaything[j + 15].MoveTo(_playAlignX + i + 25, _playAlignY + i);		//Y
			_plaything[j + 19].MoveTo(_playAlignX + 29 + i, _playAlignY + 3 - i);	//Y
		}
		_plaything[88].MoveTo(_playAlignX + 28, _playAlignY + 3);					//end of y
		_plaything[92].MoveTo(_playAlignX + 32, _playAlignY);
	}
}