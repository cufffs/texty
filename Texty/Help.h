#pragma once
#include "Entity.h"
/*
	Help.h - object to house and display commands
*/
namespace texty
{
	const int	MAX_HELPINFO = 13;
	const int	MAX_HELPINFO_NAME = 64;
	struct HelpInfo{
		char info[MAX_HELPINFO_NAME];
	};
	const static HelpInfo helpInfos_s[] = {	//there will not be any new commands during the
		{ "move left" },					//game nor will i add like 10000 commands so this is fine
		{ "move right" },					//also i dont want these to be changeable in txtfile like the other main structs
		{ "move forward" },
		{ "move back" },
		{ "attack" },						//attack without weap
		{ "attack <item name>" },			//
		{ "attack with <item name>" },		//attack with item
		{ "use <item name>" },				//use item
		{ "drop <item name>" },				//drop item
		{ "help" },							//toggle help
		{ "inv" },							//toggle inventory
		{ "kill" },							//self kill
		{ "quit/esc" }						//quit
	};
	class Help : public Entity{
	public:
		Help();
		~Help();
		Entity* GetHelp();
		void Toggle();
	private:
		void WriteToHelp(int, int, const char*, unsigned int);
		void WriteHelp();
		void Clear();
	};
}
