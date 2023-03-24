#pragma once
#include "ISprite.h"
#include "Inventory.h"
/*
	Player.h - Just houses data relevent to player.
*/
namespace texty
{
	class Player : public ISprite {
	public:
		Player();
		~Player();

		bool	ProcessAttack(ISprite&);

		//sprite stuff
		int		GetMaxHP()									const	{ return _maxHP; }
		void	IncreaseHP(int hp)									{ _hp = (hp + hp > _maxHP ? _maxHP : _hp + hp); }
		void	IncreaseMaxHP(int hp)								{ _maxHP += hp; }
		void	SetAttack(int atk)									{ _attack = atk + _numKills; }

		//stat stuff
		int		GetKillCount()								const	{ return _numKills; }
		int		GetRiddlesSolved()							const	{ return _riddlesSolved; }
		int		GetItemsUsed()								const	{ return _itemsUsed; }
		int		GetDoorsUnlocked()							const	{ return _doorsUnlocked; }
		void	IncrementKillCount()								{ ++_numKills; }
		void	IncrementRiddlesSolved()							{ ++_riddlesSolved; }
		void	IncrementItemsUsed()								{ ++_itemsUsed; }
		void	IncrementDoorsUnlocked()							{ ++_doorsUnlocked; }

		/*Inventory stuff*/
		Inventory* GetInventory()									{ return &_inventory; }
		void	ToggleInventory()									{ _inventory.Toggle(); }
		bool	CheckInventory(const InventoryItem& item)	const	{ return _inventory.Contains(item); }
		bool	CheckInventory(const char* item)			const	{ return _inventory.Contains(item); }
		int		GetItemAttack(const char* item)				const	{ return _inventory.GetItemAttack(item); }
		void	InsertItem(const InventoryItem& item)				{ _inventory.Insert(item); }
		void	RemoveItem(const InventoryItem& item)				{ return _inventory.Remove(item); }
		void	RemoveItem(const char* item)						{ return _inventory.Remove(item); }
		bool	UseItem(const InventoryItem&);
		bool	UseItem(const char*);

	private:
		Inventory	_inventory;
		int			_maxHP;
		int			_numKills;
		int			_riddlesSolved;
		int			_itemsUsed;
		int			_doorsUnlocked;
	};
}