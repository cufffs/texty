/*
	Main.cpp - The starting point
*/
#include "Engine.h"
#include "IntroState.h"
#include "Observer.h"

//memoryleak stuff
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace texty;
int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Engine::Get()->Start("hello", 130, 60);										//fairly safe to change window size as everything is relative
	Messager::Notify(EVENT_MESSAGE(new IntroState(), nullptr, 0), EVENT_STATE_PUSH);		//push first state into game
	Engine::Get()->Run();	//start game
}