#pragma once
#include "Entity.h"
/*
	Sprite - Interface for Monster and player
*/
namespace texty
{
	class ISprite : public Entity
	{
	public:
		ISprite() :_hp(100), _attack(1){}
		virtual ~ISprite(){}

		virtual bool ProcessAttack(ISprite&) = 0;

		virtual int GetHP()					const			{ return _hp; }
		virtual int GetAttack()				const			{ return _attack; }
		virtual void SetHP(int hp)							{ _hp = hp; }
		virtual void IncreaseHP(int hp)						{ _hp += hp; }
		virtual void SetAttack(int atk)						{ _attack = atk; }
		virtual void IncreaseAttack(int atk)				{ _attack += atk; }
		virtual void ReceiveDamage(int atk)					{ _hp -= atk; if (_hp < 0) _hp = 0; }
	protected:
		int _hp;
		int _attack;
	};
}
