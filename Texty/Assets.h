#pragma once
#include <utility>
/*
	Assets is in charge of loading all commonish assets, like riddles, sprites, monsters etc and cache them somewhat
	Yukky static class but faster access than using messaging system as well as a couple less lines of code when using it
	Started using this as a bit of a common.h since lots include it
*/
namespace texty
{	
	/*
		RIDDLES
	*/
	const int MAX_RIDDLE_NAME = 128;
	const int MAX_RIDDLE_ANSWER = 32;
	struct Riddles{
		char riddle[MAX_RIDDLE_NAME];
		char answer[MAX_RIDDLE_ANSWER];
	};
	/*
		MONSTERS
	*/
	const int MAX_MONSTER_NAME = 32;
	struct Monsters{
		char name[MAX_MONSTER_NAME];
		int hp, attack, type;
	};
	/*
		ITEMS
	*/
	const int MAX_ITEM_NAME = 32;
	const int MAX_ITEM_VALUE = 10000; //formatting set to 4 digits max ;p
	struct InventoryItem{
		char name[MAX_ITEM_NAME];
		unsigned int attack;
		unsigned char type;
	};
	enum ItemType{
		ITEM_WEAPON = 'w',
		ITEM_ARMOUR = 'a',
		ITEM_POTION = 'p',
		ITEM_MAX
	};
	/*
		SPRITES - very touchy as too lazy to make nice, this must match load order!, 
		and also monsters type in txt file must match enum value! (or get wrong images for them)
	*/
	enum Sprites {
		DOOR_LEFT,
		DOOR_MID,
		DOOR_RIGHT,
		DOOR_MID_1,
		DOOR_MID_2,
		DOOR_MID_3, 
		DOOR_MID_4,
		MONSTER_HUMAN,
		MONSTER_DOG,
		MONSTER_ANGEL,
		MONSTER_DRILL,
		MONSTER_KFC,
		MONSTER_YETI,
		MONSTER_TORT,
		RIDDLER,
		TEXTY
	};
	class ConsoleBuffer;
	class Assets{
	public:
		static void Free();

		static int LoadSprites(std::initializer_list<char*>);
		static int LoadMonstersFile(const char*);	//wrapper for loading
		static int LoadItemsFile(const char*);
		static int LoadRiddlesFile(const char*);

		static int GetNumMonsters()	{ return s_numMonsters; }
		static int GetNumItems()		{ return s_numItems; }
		static int GetNumRiddles()	{ return s_numRiddles; }

		static InventoryItem* GetItem(const char*);

		static ConsoleBuffer* s_sprites;
		static Monsters*		s_monsters;
		static InventoryItem*	s_items;
		static Riddles*		s_riddles;
	private:
		Assets();
		static int s_numSprites;
		static int s_numMonsters;
		static int s_numItems;
		static int s_numRiddles;
	};
}
