#include "GameState.h"
#include "Engine.h"
#include "Assets.h"
#include "Rooms.h"
#include <conio.h>

namespace texty
{
//just to make it more readable as some lines were unreadable
#define CURRENT_ROOM _rooms[_cRoom.x][_cRoom.y]
#define NEXT_ROOM _rooms[_nRoom.x][_nRoom.y]
#define GET_DOOR_WIDTH(a) _doors[(a)].GetBuffer()->GetWidth()
#define GET_DOOR_HEIGHT(a) _doors[(a)].GetBuffer()->GetHeight()
	const int DOOR_BACK = 3;

	//constructor sets everything in gamestate to be ready for use
	GameState::GameState()
		:_cRoom({ 0 }),
		_pRoom({ 0 }),										
		_nRoom({ -1 }),
		_changeRoom(false),
		_changeDir(0),
		_gameover(false),
		_winWidth(Engine::Get()->GetWidth()),
		_winHeight(Engine::Get()->GetHeight())
	{
		//create canvas so we can render
		_canvas.Create(0, 0, _winWidth, _winHeight);

		//create game objects
		_textbox.Init(TEXTBOX_X, _winHeight - TEXTBOX_H, _winWidth-2*TEXTBOX_X, TEXTBOX_H);

		_doors[DOOR_LEFT].SetBuffer(Assets::s_sprites[DOOR_LEFT]);
		_doors[DOOR_LEFT].SetBuffer(Assets::s_sprites[DOOR_LEFT]);
		_doors[DOOR_MID].SetBuffer(Assets::s_sprites[DOOR_MID]);
		_doors[DOOR_RIGHT].SetBuffer(Assets::s_sprites[DOOR_RIGHT]);

		//yukkkkkkkkkkkkkkkkk
		_doors[DOOR_LEFT].SetPos(GET_DOOR_WIDTH(DOOR_LEFT) / 2, _winHeight / 2 - (GET_DOOR_HEIGHT(DOOR_LEFT) / 2));
		_doors[DOOR_MID].SetPos(_winWidth / 2 - (GET_DOOR_WIDTH(DOOR_MID) / 2), GET_DOOR_HEIGHT(DOOR_MID) / 2);
		_doors[DOOR_RIGHT].SetPos(_winWidth - GET_DOOR_WIDTH(DOOR_RIGHT) - (GET_DOOR_WIDTH(DOOR_RIGHT) / 2),
			_winHeight / 2 - (GET_DOOR_HEIGHT(DOOR_RIGHT) / 2));

		//insert to canvas for rendering ability
		_canvas.Insert(_doors[DOOR_LEFT].GetId(), &_doors[DOOR_LEFT]);
		_canvas.Insert(_doors[DOOR_MID].GetId(), &_doors[DOOR_MID]);
		_canvas.Insert(_doors[DOOR_RIGHT].GetId(), &_doors[DOOR_RIGHT]);
		_canvas.Insert(_help.GetId(), &_help);
		_canvas.Insert(_textbox.GetId(), &_textbox);
		_canvas.Insert(_player.GetInventory()->GetId(), _player.GetInventory());

		//create all our rooms - somewhat random
		for (int y = 0; y < ROOM_H; ++y){
			for (int x = 0; x < ROOM_W; ++x){
				if (!x&&!y)															//empty - always first empty for reasons
					_rooms[x][y] = new EmptyRoom(this, 1, 1, 1);
				else if (!(rand() % 4))
					_rooms[x][y] = new LockedRoom(this, rand() % 3, rand() % 3, rand() % 3);
				else if (!(rand() % 2))
					_rooms[x][y] = new MonsterRoom(this, rand() % 3, rand() % 3, rand() % 3);
				else if (!(rand() % 2))
					_rooms[x][y] = new RiddleRoom(this, rand() % 3, rand() % 3, rand() % 3);
				else
					_rooms[x][y] = new EmptyRoom(this, rand() % 3, rand() % 3, rand() % 3);
			}
		}
		//simple info to start us off
		_textbox.WriteToTextbox("_7_>Type help for commands.");
		
		//enter first room and draw it
		CURRENT_ROOM->OnRoomEnter(3);
		DrawRoom(CURRENT_ROOM->isLeftDoor(), CURRENT_ROOM->isTopDoor(), CURRENT_ROOM->isRightDoor());
	}

	GameState::~GameState()
	{
		for (int y = 0; y < ROOM_H; ++y)
			for (int x = 0; x < ROOM_W; ++x)
				delete _rooms[x][y];
	}

	void GameState::Update()
	{
		if (_gameover){
			getchar();											//pause to read endgame info
			Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);	//back to intro we go
		}
		else{
			GetInput();											//get input for frame
			if (_changeRoom && _nRoom.x != -1)					//to handle locked doors (because must be in a room to process input!)
				EnterNextRoom(true);							//Enter new room based on input
		}
		_canvas.Update();
	}

	void GameState::Draw()
	{
		_canvas.Render();
	}

	void GameState::GetInput()
	{
		if (_textbox._input.ReadFromConsole()){					//readinput from console using string class :P
			_textbox.ClearInputbox();							//clear input box
			ProcessInput();										//now we have input, lets process it
			_textbox._input.Clear();
		}
		else{
			_textbox.ClearInputbox();							//clear input incase used backspace
			_textbox.Echo();									//print the letters we type so we can see them
		}
	}

	void GameState::ProcessInput()
	{
		myString::String input = _textbox._input.ToLower();
		if (input == "move right")
			ProcessRoomChange(MOVE_RIGHT);
		else if (input == "move back")
			ProcessRoomChange(3);
		else if (input == "move left")
			ProcessRoomChange(MOVE_LEFT);
		else if (input == "move forward")
			ProcessRoomChange(MOVE_MID);
		else if (input.Find("use") != -1)					//find is case inseitive!
			UseItem();
		else if (input.Find("drop") != -1)
			DropItem();
		else if (input == "help")
			_help.Toggle();
		else if (input == "inv")
			_player.ToggleInventory();
		else if (input == "kill")
			GameOver();
		else if (input == "quit")
			Messager::Notify(EMPTY_MESSAGE, EVENT_STATE_POP);
		else
			CURRENT_ROOM->ProcessInput(_textbox._input);
	}

	//print boring stats 
	void GameState::GameOver()
	{
		_gameover = true;
		ClearTextbox();
		WriteToTextbox("You have died. Press any key to continue");
		WriteToTextbox("_15_Kills:[_14_%i_15_] - ItemsUsed:[_14_%i_15_] - RiddlesSolved:[_14_%i_15_] - DoorsUnlocked:[_14_%i_15_]",
			_player.GetKillCount(), _player.GetItemsUsed(), _player.GetRiddlesSolved(), _player.GetDoorsUnlocked());
	}

	void GameState::EnterNextRoom(bool firsttime)
	{
		CURRENT_ROOM->OnRoomExit();

		_pRoom = _cRoom;
		_cRoom = _nRoom;
		_nRoom.x = -1;

		if (firsttime)
			CURRENT_ROOM->OnRoomEnter(_changeDir);

		DrawRoom(CURRENT_ROOM->isLeftDoor(), CURRENT_ROOM->isTopDoor(), CURRENT_ROOM->isRightDoor());
	}

	void GameState::DrawRoom(bool l, bool m, bool r)
	{
		_doors[DOOR_LEFT].SetDraw(l);
		_doors[DOOR_MID].SetDraw(m);
		_doors[DOOR_RIGHT].SetDraw(r);
	}

	void GameState::ProcessRoomChange(int dir)
	{
		_nRoom = _cRoom;
		_changeRoom = false;
		_changeDir = dir;
		switch (dir)
		{
		case DOOR_LEFT:
			if (!CURRENT_ROOM->isLeftDoor()){	//if there is not a door to left
				_textbox.WriteToTextbox("_10_Unable to move left");		//no left door
				return;
			}
			_nRoom.y = ((_nRoom.y - 1 <= -1) ? ROOM_H - 1 : _nRoom.y - 1);	//set next room
			break;
		case DOOR_MID:
			if (!CURRENT_ROOM->isTopDoor()){			//if we cant move forward
				_textbox.WriteToTextbox("_10_Unable to move forward");
				return;
			}
			_nRoom.x = ((_nRoom.x + 1 >= ROOM_W) ? 0 : _nRoom.x + 1);	//set next room
			break;
		case DOOR_RIGHT:
			if (!CURRENT_ROOM->isRightDoor()){	//if there is a no door door to left
				_textbox.WriteToTextbox("_10_Unable to move right");
				return;
			}
			_nRoom.y = ((_nRoom.y + 1 >= ROOM_H) ? 0 : _nRoom.y + 1);	//set next room
			break;
		case DOOR_BACK:
			_nRoom = _pRoom;											//dont need door check to move back
			break;
		default:
			return;
		}
		if (NEXT_ROOM->OnRoomEnter(_changeDir))							//check we can enter room
			EnterNextRoom(false);										//enter next room
	}

	//here for ease of writing to textbox
	void GameState::UseItem()
	{
		_textbox._input.Replace("use ", "");		//left with just item to use
		InventoryItem* item = Assets::GetItem(_textbox._input.CStr());
		if (_player.UseItem(_textbox._input.CStr())){
			if (item->type == ITEM_ARMOUR)
				_textbox.WriteToTextbox("_15_You equip the _14_%s_15_ and gain some HP. _15_HP:_14_%i_15_(_14_+%i_15_) MAXHP:_14_%i_15_(_14_+%i_15_)",
				item->name, _player.GetHP(), item->attack, _player.GetMaxHP(), item->attack);
			else
				_textbox.WriteToTextbox("_15_You drink the _14_%s_15_ and recover some HP. _15_HP:_14_%i_15_(_14_+%i_15_) MAXHP:_14_%i_15_(_14_+0_15_)",
				item->name, _player.GetHP(), item->attack, _player.GetMaxHP());
		}
		else if (item && item->type == ITEM_WEAPON)
			_textbox.WriteToTextbox("_15_That's to kill not to heal!");
		else
			_textbox.WriteToTextbox("_15_You cannot use an item you do not have!");
	}

	//here for ease of writing to textbox
	void GameState::DropItem()
	{
		_textbox._input.Replace("drop ", "");		//left with just item to use
		if (_player.CheckInventory(_textbox._input.CStr())){
			_player.RemoveItem(_textbox._input.CStr());
			_textbox.WriteToTextbox("_14_%s_15_ has been removed from your inventory.", _textbox._input.Formatted().CStr());
		}
		else
			_textbox.WriteToTextbox("_15_You don't have such an item :(");
	}

	//wrapper for textbox - should update to variadic but lazy
	void GameState::WriteToTextbox(const char* txt, ...)
	{
		char buffer[512] = { 0 };
		va_list va;
		va_start(va, txt);
		vsprintf_s(buffer, sizeof(buffer), txt, va);
		va_end(va);
		_textbox.WriteToTextbox(buffer);
	}

	//wrapper for textbox
	void GameState::ClearTextbox()
	{
		_textbox.ClearTextbox(); 
	}
}