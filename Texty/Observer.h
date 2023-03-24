#pragma once
/*
	Observer.h - relays messages to all listening observers
*/

//#define EMPTY_MESSAGE Message { nullptr, 0 }
#define EVENT_MESSAGE(a, b, c) Message {(void*)(a), (void*)(b), (c)}

namespace texty {
	struct Message {
		void* ptr;
		void* ptr2;
		unsigned int flags;
	};

	static Message EMPTY_MESSAGE = { nullptr, nullptr, 0 };

	enum Event {
		EVENT_ENGINE_GETLENGTHS,
		EVENT_ENGINE_EXIT,
		EVENT_ENTITY_MOVE,
		EVENT_STATE_POP,
		EVENT_STATE_PUSH,
		EVENT_RENDERER_ADD
	};

	class Observer;

	class Messager {
	public:
		static void Add(Observer*);
		static void Remove(Observer*);
		static void Notify(Message&, Event);
	private:
		Messager();
		static Observer* _head;
	};

	class Observer {
		friend class Messager;
	public:
		Observer(){}
		virtual void OnNotify(Message&, Event) = 0;
	private:
		Observer* _next;
	};

}