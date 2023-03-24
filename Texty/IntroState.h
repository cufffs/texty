#pragma once
#include "State.h"
#include "Entity.h"
/*
	IntroState.h - snazzy little intro to make it less boring
*/
namespace texty
{
	const int MAX_PLAYTHING = 118;
	class IntroState : public State {
	public:
		IntroState();

		virtual ~IntroState();

		void Update();
		void Draw();

	private:
		void DrawPlay();

		bool _play;
		Entity _logo;
		Entity _plaything[MAX_PLAYTHING];

		int _windowWidth, _windowHeight;
		int _playAlignX, _playAlignY;
	};
}