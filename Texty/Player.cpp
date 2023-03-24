#include "Player.h"
#include "Engine.h"
namespace texty
{
	Player::Player()
		:_maxHP(100),
		_numKills(0),
		_riddlesSolved(0),
		_itemsUsed(0),
		_doorsUnlocked(0)
	{
		_inventory.SetDraw(false);
	}

	Player::~Player()
	{
	}

	bool Player::ProcessAttack(ISprite& monster)	//player causes damage to sprite only, sprite can retaliate in its own instance
	{
		monster.ReceiveDamage(_attack);
		if (monster.GetHP() <= 0)
			return true;
		return false;
	}

	bool Player::UseItem(const InventoryItem& item)
	{
		return UseItem(item.name);
	}

	bool Player::UseItem(const char* item)
	{
		InventoryItem* m = Assets::GetItem(item);
		if (m && m->type != ITEM_WEAPON){
			switch (m->type)
			{
			case ITEM_ARMOUR:
			{
				IncreaseMaxHP(m->attack);		//increase maxhp
				IncreaseHP(m->attack);			//increase hp
				RemoveItem(item);
				++_itemsUsed;
				return true;
			}
			case ITEM_POTION:
			{
				IncreaseHP(m->attack);			//just increase hp
				RemoveItem(item);
				++_itemsUsed;
				return true;
			}
			}
		}
		return false;
	}
}