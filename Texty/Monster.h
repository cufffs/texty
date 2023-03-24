#pragma once
#include "ISprite.h"
/*
	Monster.h - All the bad guys in one place
*/
namespace texty
{
	class Monster : public ISprite{
	public:
		Monster();
		virtual bool ProcessAttack(ISprite&);
		//just get data from Assets, and point to asset sprite to save memory etc
		void NewMonster(int);
		int GetMonsterId() const;
	private:
		int _monsterId;
		int _winWidth, _winHeight;
	};
}