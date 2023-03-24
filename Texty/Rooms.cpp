#include "Rooms.h"
#include "GameState.h"
#include "AnimateDoors.h"
#include "Observer.h"

namespace texty
{
	/*
		IROOM
	*/
	IRoom::IRoom(GameState* gs, int l, int t, int r)
		:_layout(0)
	{
		_currentGS = gs;
		if (l) _layout |= 1 << 0;
		if (t) _layout |= 1 << 1;
		if (r) _layout |= 1 << 2;
	}

	void IRoom::HandleAnimation(int dir)
	{
		if (dir != 3){
			Messager::Notify(EVENT_MESSAGE(new AnimateDoors(dir), nullptr, 0), EVENT_STATE_PUSH);
		}
	}

	/*
		EMPTY ROOM
	*/
	EmptyRoom::EmptyRoom(GameState* gs, int l, int t, int r)
		: IRoom(gs, l, t, r)
	{
	}

	EmptyRoom::~EmptyRoom()
	{
	}

	bool EmptyRoom::OnRoomEnter(int dir)
	{
#ifndef SKIPANIMATION
		HandleAnimation(dir);
#endif
		_currentGS->WriteToTextbox("_3_This looks to be just an empty room!");
		return true;
	}

	void EmptyRoom::OnRoomExit()
	{
	}

	void EmptyRoom::ProcessInput(const myString::String&)
	{
		_currentGS->WriteToTextbox("_15_IT'S EMPTY! YOU CAN'T DO THAT!");
	}

	/*
		RIDDLE ROOM
	*/
	RiddleRoom::RiddleRoom(GameState* gs, int l, int t, int r)
		: IRoom(gs, l, t, r)
	{
	}

	RiddleRoom::~RiddleRoom()
	{
	}

	void RiddleRoom::OnRoomExit()
	{
		_riddler.SetDraw(false);
	}

	bool RiddleRoom::OnRoomEnter(int dir)
	{
#ifndef SKIPANIMATION
		HandleAnimation(dir);
#endif
		static bool first = true; static int winWidth; static int winHeight;
		if (first){	//lame cheap fix
			Messager::Notify(EVENT_MESSAGE(&winWidth, &winHeight, 0), EVENT_ENGINE_GETLENGTHS);
			_riddler.SetBuffer(Assets::s_sprites[RIDDLER]);
			_riddler.SetPos(winWidth / 2 - _riddler.GetBuffer()->GetWidth() / 2, winHeight / 2 - _riddler.GetBuffer()->GetHeight() / 2);
			_currentGS->InsertToCanvas(&_riddler);
		}
		_riddler.SetDraw(true);
		_data = rand() % Assets::GetNumRiddles();		//generate random riddle on each entry
		_currentGS->ClearTextbox();
		_currentGS->WriteToTextbox("_15_A Riddler appears to be holding something that you want...-nHe offers a riddle.");
		_currentGS->WriteToTextbox("_3_%s", Assets::s_riddles[_data].riddle);
		return true;
	}

	void RiddleRoom::ProcessInput(const myString::String& input)
	{
		int item = 0;
		if (input.Find(Assets::s_riddles[_data].answer) >= 0){
			item = rand() % Assets::GetNumItems();					//spawn random item
			_currentGS->WriteToTextbox("_15_A look of horror comes across the Riddler's face, you've beaten him!");
			_currentGS->WriteToTextbox("_15_The riddler reluctantly hands over his precious _14_%s_15_ to you!", Assets::s_items[item].name);
			_currentGS->InsertPlayerItem(Assets::s_items[item]);	//give item to player
			_currentGS->IncrementRiddlesSolved();
		}
		else
			_currentGS->WriteToTextbox("_15_The Riddler laughs are how foolish you appear to be!");
	}

	LockedRoom::LockedRoom(GameState* gs, int l, int t, int r)
		: RiddleRoom(gs, l, t, r),
		_locked(true)
	{
		_data = rand() % Assets::GetNumItems();
	}

	bool LockedRoom::OnRoomEnter(int dir)
	{
		if (_locked){
			_currentGS->ClearTextbox();
			_currentGS->WriteToTextbox("_15_This room requires a _14_%s_15_ in order to be accessed.", Assets::s_items[_data].name);
			if (_currentGS->CheckPlayerInventory(Assets::s_items[_data]))
				_currentGS->WriteToTextbox("_15_Do you wish to use your _14_%s_15_ to unlock the room? (yes/no)", Assets::s_items[_data].name);
			else
				_currentGS->WriteToTextbox("_15_Sadly, you do not have such an amazing item, you cannot access this room.");
			return false;
		}
#ifndef SKIPANIMATION
		HandleAnimation(dir);
#endif
		_currentGS->WriteToTextbox("_15_You entered a now unlocked room.");
		return true;
	}

	void LockedRoom::OnRoomExit()
	{
	}

	void LockedRoom::ProcessInput(const myString::String& input)
	{
		if (_currentGS->CheckPlayerInventory(Assets::s_items[_data])){
			if (input == "yes"){
				_currentGS->WriteToTextbox("_15_You unlock the room, losing your _14_%s_15_ in the process", Assets::s_items[_data].name);
				_currentGS->RemovePlayerItem(Assets::s_items[_data]);
				_currentGS->ChangeRoom(true);
				_currentGS->IncrementDoorsUnlocked();
				_currentGS->IncrementItemsUsed();
				_locked = false;
			}
			else{
				_currentGS->WriteToTextbox("_15_You save your _14_%s_15_ and but do not enter the room", Assets::s_items[_data].name);
				_currentGS->ChangeRoom(false);
			}
		}
		else{
			_currentGS->WriteToTextbox("_15_Sadly, you do not have such an amazing item, you cannot access this room.");
			_currentGS->ChangeRoom(false);
		}
	}

	MonsterRoom::MonsterRoom(GameState* gs, int l, int t, int r)
		: IRoom(gs, l, t, r)
	{
	}

	MonsterRoom::~MonsterRoom()
	{
	}

	bool MonsterRoom::OnRoomEnter(int dir)
	{
#ifndef SKIPANIMATION
		HandleAnimation(dir);
#endif
		_monster.NewMonster(rand() % (Assets::GetNumMonsters()));	//new monster everytime we enter room, restore full hp to monster 
		_currentGS->InsertToCanvas(&_monster);
		_monster.SetDraw(true);
		_currentGS->WriteToTextbox("_15_A scary looking _12_%s_15_ has appeared. Ohno!", Assets::s_monsters[_monster.GetMonsterId()].name);

		return true;
	}

	void MonsterRoom::OnRoomExit()
	{
		_monster.SetDraw(false);
	}

	void MonsterRoom::ProcessInput(const myString::String& input)
	{
		if (input.Find("attack") != -1 && ChooseWeapon(input))
			HandleAttack();				//do attack
	}

	bool MonsterRoom::ChooseWeapon(const myString::String& input)
	{
		myString::String tmp = input.ToLower();
		if (tmp.ToLower() == "attack"){
			_currentGS->ClearTextbox();	//gotta keep formatting
			_currentGS->WriteToTextbox("_15_You're too cool to use a weapon! Barehands it is!");
			_currentGS->SetPlayerAttack(1);
			return true;
		}
		else{
			tmp.Replace("attack with ", "");		//remove attack so we are left with just item name
			tmp.Replace("attack ", "");
			InventoryItem* item = Assets::GetItem(tmp.CStr());
			if (item && _currentGS->CheckPlayerInventory(*item)){
				if (item->type == ITEM_WEAPON){
					_currentGS->ClearTextbox();	//gotta keep formatting
					_currentGS->SetPlayerAttack(Assets::GetItem(tmp.CStr())->attack);
					_currentGS->WriteToTextbox("_15_You whip out your _14_%s_15_ and play with it in your hands!", tmp.Formatted().CStr());
					return true;
				}
				_currentGS->WriteToTextbox("_15_That's not a weapon!");
				return false;
			}
			_currentGS->WriteToTextbox("_15_You don't have such an item...");
			_currentGS->SetPlayerAttack(1);
			return false;
		}
	}

	void MonsterRoom::HandleAttack()
	{
		if (_monster.GetHP() > 0){
			_currentGS->WriteToTextbox("_15_You attack the _12_%s_15_ with your whopping _12_%i_15_ attack power!",
				Assets::s_monsters[_monster.GetMonsterId()].name, _currentGS->GetPlayerAttack());

			if (!_currentGS->PlayerAttack(_monster)){			//if monster isnt dead, it will attack back
				_currentGS->WriteToTextbox("_15_It looks like you've made a poor decision...");
				if (!(rand() % 5)){
					_currentGS->WriteToTextbox("_12_Uhoh! The _14_%s_12_ is getting fired up! _15_It's damage doubles!",
						Assets::s_monsters[_monster.GetMonsterId()].name);
					_monster.SetAttack(2 * _monster.GetAttack());
				}
				_currentGS->WriteToTextbox("_15_The _12_%s_15_ fights back, dealing a costly_12_ %i_15_ damage!",
					Assets::s_monsters[_monster.GetMonsterId()].name, _monster.GetAttack());
				_currentGS->HurtPlayer(_monster.GetAttack());	//attack player with monster
			}
			else{	//monster died
				int item = rand() % Assets::GetNumItems();				//generate random drop item
				_currentGS->InsertPlayerItem(Assets::s_items[item]);	//insert item to our inventory
				_currentGS->WriteToTextbox("_15_The _12_%s_15_ dies, and drops an awesome _14_%s",
					Assets::s_monsters[_monster.GetMonsterId()].name,
					Assets::s_items[item].name);
				_monster.SetDraw(false);								//set its draw to 0, no need to remove static object from canvas
				_currentGS->IncrementKillCount();						//+1 kills
			}
			if (!_currentGS->GetPlayerHP()){							//if player died
				_currentGS->GameOver();
			}
			else
				_currentGS->WriteToTextbox("_15_You have %iHP left.", _currentGS->GetPlayerHP());
		}
		else		//trying to attack dead monster
			_currentGS->WriteToTextbox("There is nothing else to attack");
	}
}