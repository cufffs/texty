#pragma once
#include "myString.h"
#include "Monster.h"
namespace texty
{
	class GameState;
	class IRoom{
	public:
		IRoom(GameState* gs, int l, int t, int r);
		virtual ~IRoom(){}

		virtual bool OnRoomEnter(int) = 0;
		virtual void OnRoomExit(){};
		virtual void ProcessInput(const myString::String&) = 0;

		bool isLeftDoor()	const	{ return (_layout >> 0) & 1; }
		bool isTopDoor()	const	{ return (_layout >> 1) & 1; }
		bool isRightDoor()	const	{ return (_layout >> 2) & 1; }
	protected:
		void HandleAnimation(int dir);
		GameState*			_currentGS;				//bad but good
		unsigned char		_layout;				//bits 0-2 : doors available
	};

	class EmptyRoom : public IRoom{
	public:
		EmptyRoom(GameState* gs, int l, int t, int r);
		~EmptyRoom();

		bool OnRoomEnter(int dir);
		void OnRoomExit();
		void ProcessInput(const myString::String& input);
	private:
		EmptyRoom();
	};

	class RiddleRoom : public IRoom{
	public:
		RiddleRoom(GameState* gs, int l, int t, int r);
		virtual ~RiddleRoom();

		virtual bool OnRoomEnter(int dir);
		virtual void OnRoomExit();
		virtual void ProcessInput(const myString::String& input);

	protected:
		int				_data;
	private:
		RiddleRoom();
		Entity			_riddler;

	};

	class LockedRoom : public RiddleRoom{
	public:
		LockedRoom(GameState* gs, int l, int t, int r);
		virtual ~LockedRoom(){}

		bool OnRoomEnter(int dir);
		void OnRoomExit();
		void ProcessInput(const myString::String& input);

	private:
		LockedRoom();

		bool _locked;
	};

	class MonsterRoom : public IRoom{
	public:
		MonsterRoom(GameState* gs, int l, int t, int r);
		virtual ~MonsterRoom();
		bool OnRoomEnter(int dir);
		void ProcessInput(const myString::String& input);
		void OnRoomExit();

	private:
		MonsterRoom();

		bool ChooseWeapon(const myString::String& input);
		void HandleAttack();

		Monster _monster;
	};
}