#pragma once
/*
	StateManager.h - controls each state and call their update, draw functions
*/
#include "Observer.h"
#include <vector>
namespace texty{
	class State;
	class StateManager : public Observer{
	public:
		StateManager();
		~StateManager();

		void Update();
		void Draw();
		void OnNotify(Message&, Event);

		void Push(State& state);
		void Pop();
	private:
		bool	_wantPop;
		State*	_currentState;
		std::vector<State*> _states;
	};
}