#include "Assets.h"
#include "FileReader.h"
#include "ConsoleBuffer.h"
#include <stdio.h>
namespace texty
{
	char _toLower(const char c);
	bool _strcmp(const char* s1, const char* s2);

	ConsoleBuffer* Assets::s_sprites = nullptr;
	int Assets::s_numSprites = 0;

	Monsters* Assets::s_monsters = nullptr;
	int Assets::s_numMonsters = 0;

	InventoryItem* Assets::s_items = nullptr;
	int Assets::s_numItems = 0;

	Riddles* Assets::s_riddles = nullptr;
	int Assets::s_numRiddles = 0;

	void Assets::Free()
	{
		delete[]s_sprites;
		delete[]s_monsters;
		delete[]s_items;
		delete[]s_riddles;
	}

	//load all sprites into an array
	int Assets::LoadSprites(std::initializer_list<char*> li)
	{
		int w = 0, h = 0, colour = 0;
		int c;
		FILE* fp;
		s_numSprites = li.size();
		s_sprites = new ConsoleBuffer[s_numSprites];
		auto it = li.begin();
		for (int i = 0; i < s_numSprites; ++i, ++it){
			fopen_s(&fp, *it, "r");
			if (!fp) return 0;
			fscanf_s(fp, "%d %d\n", &w, &h);
			s_sprites[i].NewBuffer(w, h);
			for (int y = 0; y < h; ++y){
				for (int x = 0; x < w; ++x){
					fscanf_s(fp, "%d %d", &c, &colour);
					s_sprites[i].Write(x, y, c, colour);
				}
				fgetc(fp);
			}
			fclose(fp);							//close file
		}
		return s_numSprites;							//return count
	}

	//wrapper for filereader
	int Assets::LoadMonstersFile(const char* fname)
	{
		s_numMonsters = FileReader::Read((void*&)s_monsters, sizeof(Monsters), "%s32%i%i%i", fname);	//fill monsters struct with info from file
		return s_numMonsters;																			//must pass it format of struct so memory can be
	}																									//traversed
	
	//wrapper for filereader
	int Assets::LoadItemsFile(const char* fname)
	{
		s_numItems = FileReader::Read((void*&)s_items, sizeof(InventoryItem), "%s32%i%c%p3", fname);	//must manually insert padding^^
		return s_numItems;																				//could be auto but if padding is between
	}																									//things it'd have to be manual
	
	//wrapper for filereader
	int Assets::LoadRiddlesFile(const char* fname)
	{
		s_numRiddles = FileReader::Read((void*&)s_riddles, sizeof(Riddles), "%s128%s32", fname);
		return s_numRiddles;
	}

	//sees if item exists!
	InventoryItem* Assets::GetItem(const char* item)
	{
		for (int i = 0; i < s_numItems; ++i){	//easy way to get an item from anywhere in program
			if (_strcmp(s_items[i].name, item))
				return &s_items[i];
		}
		return 0;
	}
}
