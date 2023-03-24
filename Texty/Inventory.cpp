#include "Inventory.h"
#include "Observer.h"
#include "defs.h"
namespace texty
{
	//terrible place for these!
	_inline char _toLower(const char c);
	_inline bool _strcmp(const char* s1, const char* s2);	//for case insensitive strcmp

	Inventory::Inventory()
	{
		int winWidth, winHeight;
		Messager::Notify(EVENT_MESSAGE(&winWidth, &winHeight, 0), EVENT_ENGINE_GETLENGTHS);

		Create(winWidth / 2 - INV_W / 2, winHeight / 2 - INV_H, INV_W, INV_H);
		_buffer.Fill(CHAR_BLOCK, BLACK);
		_buffer.DrawBox({ 0, 0 }, INV_W, INV_H, CHAR_ALTBLOCK, DARKPURPLE);
		WriteToInventory(INV_W / 2 - (strlen("INVENTORY") / 2), 1, "INVENTORY", RED);			//the 'inventories' title so to spea
		SetDraw(false);
	}

	Inventory::Inventory(int x, int y, int w, int h)
		:Entity(x, y, w, h)
	{
		_buffer.Fill(CHAR_BLOCK, 0);
		_buffer.DrawBox({ 0, 0 }, w, h, (CHAR)176, DARKPURPLE);
		WriteToInventory(w / 2 - (strlen("INVENTORY") / 2), 1, "INVENTORY", RED);			//the 'inventories' title so to spea
		SetDraw(false);
	}

	void Inventory::Init(int x, int y, int w, int h)
	{
		Create(x, y, w, h);
		_buffer.Fill(CHAR_BLOCK, 0);
		_buffer.DrawBox({ 0, 0 }, w, h, (CHAR)176, DARKPURPLE);
		WriteToInventory(w / 2 - (strlen("INVENTORY") / 2), 1, "INVENTORY", RED);			//the 'inventories' title so to spea
	}

	Inventory::~Inventory()
	{
	}

	void Inventory::Insert(const InventoryItem& item)
	{
		if (item.name[0] != 0 && item.attack < MAX_ITEM_VALUE)
			_items.push_back(item);
	}

	void Inventory::Remove(const InventoryItem& item)	//removes item from bottom of list
	{
		Remove(item.name);
	}

	void Inventory::Remove(const char* item)	//removes item from bottom of list
	{
		for (auto it = _items.end();;){
			it--;							//end points to nothing, so i skip end
			if (_strcmp(it->name, item)){	//match
				_items.erase(it);			//erase it
				return;						//return
			}
			if (it == _items.begin())		//if we've just processed begin then return
				return;
		}
	}

	bool Inventory::Contains(const InventoryItem& item) const
	{
		return Contains(item.name);
	}

	bool Inventory::Contains(const char* item) const
	{
		for (auto it = _items.begin(); it != _items.end(); it++)
			if (_strcmp(it->name, item))
				return true;
		return false;
	}

	int Inventory::GetItemAttack(const char* item) const
	{
		for (auto it = _items.begin(); it != _items.end(); it++)
			if (_strcmp(it->name, item))
				return it->attack;
		return -1;
	}

	void Inventory::Toggle()
	{
		SetDraw(!GetDraw());
		WriteInventory();
	}

	void Inventory::WriteToInventory(int x, int y, const char* txt, unsigned int colour)
	{
		if (!txt) return;
		char* cp = (char*)txt;

		for (; *cp; ++x, ++cp){
			if (x >= _buffer.GetWidth() - 1){
				if (++y <= _buffer.GetHeight() - 1)
					x = 1;
				else return;
			}
			_buffer.Write(x + _buffer.GetWidth()*y, *cp, colour);
		}
	}

	void Inventory::WriteInventory()
	{
		char tmpNum[5] = { 0 };		//going to limit attack to 9999 to make formatting bestest
		Clear();
		int y = 2, x = _buffer.GetWidth() - 5;
		for (auto it = _items.begin(); it != _items.end(); it++, ++y){
			if (it->attack && it->attack < MAX_ITEM_VALUE)
				sprintf_s(tmpNum, sizeof(tmpNum), "%d", it->attack);
			else
				tmpNum[0] = '0', tmpNum[1] = 0;	//set to 0 because 0

			WriteToInventory(1, y, it->name, (y % 2 ? LIGHTBLUE : WHITE));	//write the item name to left of inventory	-- formatting
			WriteToInventory(x, y, tmpNum, (y % 2 ? LIGHTBLUE : WHITE));		//write its attack to the right of inventory
		}
	}

	void Inventory::Clear()
	{
		for (int y = 2; y < _buffer.GetHeight() - 1; ++y)
			for (int x = 1; x < (_buffer.GetWidth() - 1); ++x)
				_buffer.Write(x + _buffer.GetWidth()*y, CHAR_BLOCK, BLACK);
	}

	_inline char _toLower(const char c)
	{
		return ((c >= 65 && c <= 90) ? (c + 32) : (c));
	}

	_inline bool _strcmp(const char* s1, const char* s2)	//for case insensitive strcmp
	{
		char* cp = (char*)s1, *cp2 = (char*)s2;
		while (*cp && *cp2 && _toLower(*cp++) == _toLower(*cp2++));
		return !*cp&&!*cp2;
	}
}