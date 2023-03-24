#include "StateManager.h"
#include "State.h"
namespace texty
{
	StateManager::StateManager()
	{
		Messager::Add(this);
	}

	StateManager::~StateManager()
	{

	}

	void StateManager::Update()
	{
		if (_currentState){					//if we have a current state, then call its update
			_currentState->Update();
			if (_wantPop){						//some state has called it wants to be popped
				_wantPop = false;
				Pop();							//pop off state
			}
		}
		else
			Messager::Notify(EMPTY_MESSAGE, EVENT_ENGINE_EXIT);
	}

	void StateManager::Draw()
	{
		if (_currentState)
			_currentState->Draw();
	}

	void StateManager::OnNotify(Message& msg, Event evnt)
	{
		switch (evnt)
		{
		case EVENT_STATE_PUSH:
			Push((*(State*)msg.ptr));
			break;
		case EVENT_STATE_POP:
			_wantPop = true;
			break;
		}
	}

	void StateManager::Push(State& state)
	{
		_states.push_back(&state);
		_currentState = &state;
	}

	void StateManager::Pop()
	{
		_states.pop_back();							//pop current state off
		delete _currentState;						//delete it
		_currentState = nullptr;
		if (!_states.empty())						//if there are more states 
			_currentState = _states.back();			//set current state to top of stack
	}
}