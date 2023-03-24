#include "Monster.h"
#include "Observer.h"
#include "Assets.h"
namespace texty
{
	Monster::Monster()
	{
		Messager::Notify(EVENT_MESSAGE(&_winWidth, &_winHeight, 0), EVENT_ENGINE_GETLENGTHS);
	}

	void Monster::NewMonster(int id)
	{
		int nummonster = Assets::GetNumMonsters();
		if (id >= nummonster - 2) id = 0;
		_monsterId = id;
		_hp = Assets::s_monsters[id].hp;
		_attack = Assets::s_monsters[id].attack;
		SetBuffer(Assets::s_sprites[Assets::s_monsters[id].type]);
		SetPos(_winWidth / 2 - _buffer.GetWidth() / 2, _winHeight / 2 - _buffer.GetHeight() / 2);
	}

	bool Monster::ProcessAttack(ISprite& other)
	{
		other.ReceiveDamage(_attack);
		return (other.GetHP() <= 0);
	}

	int Monster::GetMonsterId() const
	{ 
		return _monsterId;
	}
}