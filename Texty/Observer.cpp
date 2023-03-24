#include "Observer.h"

namespace texty {

	Observer* Messager::_head = nullptr;

	Messager::Messager()
	{

	}

	void Messager::Add(Observer* observer)	//insert to front of list
	{
		observer->_next = _head;
		_head = observer;
	}

	void Messager::Remove(Observer* observer)
	{
		if (_head == observer){
			_head = observer->_next;
			observer->_next = nullptr;
			return;
		}
		Observer* cur = _head;
		while (cur != nullptr){
			if (cur->_next == observer){
				cur->_next = observer->_next;
				observer->_next = nullptr;
				return;
			}
			cur = cur->_next;
		}
	}

	void Messager::Notify(Message& entity, Event vent)
	{
		Observer* obs = _head;
		while (obs != nullptr){
			obs->OnNotify(entity, vent);
			obs = obs->_next;
		}
	}


}