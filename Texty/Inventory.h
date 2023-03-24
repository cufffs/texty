#pragma once
/*
	Inventory.h - Simple inventory system that holds items, and can write inventory to screen
*/
#include "Entity.h"
#include "Assets.h"		//assets is kindve a common.h these days
#include <vector>
namespace texty
{
	const int INV_W = 30;
	const int INV_H = 20;

	class Inventory : public Entity{
	public:
		Inventory();
		Inventory(int x, int y, int w, int h);
		~Inventory();

		//create the inventory!
		void Init(int x, int y, int w, int h);

		void	Insert(const InventoryItem&);						//Insert an item into _items vector
		void	Remove(const InventoryItem&);						//Remove an item from _items vector
		void	Remove(const char* item);
		bool	Contains(const InventoryItem&)	const;				//Check if _items vector contains item
		bool	Contains(const char*)			const;
		int		GetItemAttack(const char*)		const;
		void	Toggle();											//Toggles between draw and not draw, rewrites inventory incase change in items

	private:
		std::vector<InventoryItem>	_items;

		void	WriteToInventory(int x, int y, const char* txt, unsigned int colour);		//writes to inventory buffer
		void	WriteInventory();	//writes all items to inventory buffer
		void	Clear();			//fills draw area with black dots, so it draws over whatever is behind it
	};
}