#pragma once
#include "State.h"
#include "Textbox.h"
#include "Entity.h"
#include "Help.h"
#include "Player.h"
/*
	GameState.h - The heart of the game, it controls all game stuff pretty much
*/
namespace texty
{
	const int TEXTBOX_X = 20;
	const int TEXTBOX_H = 10;

	const int MOVE_LEFT = 0;
	const int MOVE_MID = 1;
	const int MOVE_RIGHT = 2;

	const int ROOM_W = 4;			//how many rooms there will be, i think 4 or 5 is good, easy to get lost!
	const int ROOM_H = 4;
	struct ArrayIndex2D{			//to make working with 2d array a little easier
		int x; int y;
	};
	class IRoom;
	class GameState : public State{
	public:
		GameState();
		~GameState();
		void Draw();
		void Update();
		void GameOver();						//handle game over

		//gameplay stuff
		void EnterNextRoom(bool firsttime);
		void DrawRoom(bool l, bool m, bool r);
		void ProcessRoomChange(int dir);

		//textbox stuff
		void WriteToTextbox(const char* txt, ...);
		void ClearTextbox();

		//
		void ChangeRoom(bool state)						{ _changeRoom = state; if (!state) _nRoom.x = -1; }

		//canvas access
		void InsertToCanvas(Entity* obj)				{ _canvas.Insert(obj->GetId(), obj);	}
		void RemoveFromCanvas(Entity* obj)				{ _canvas.Remove(obj->GetId());			}

		//Player gets/sets
		void UseItem();
		void DropItem();
		bool CheckPlayerInventory(InventoryItem item)	{ return _player.CheckInventory(item);	}
		void InsertPlayerItem(InventoryItem item)		{ _player.InsertItem(item);				}
		void RemovePlayerItem(InventoryItem item)		{ _player.RemoveItem(item);				}
		void IncrementKillCount()						{ _player.IncrementKillCount();			}	
		void IncrementRiddlesSolved()					{ _player.IncrementRiddlesSolved();		}
		void IncrementItemsUsed()						{ _player.IncrementItemsUsed();			}
		void IncrementDoorsUnlocked()					{ _player.IncrementDoorsUnlocked();		}
		void SetPlayerAttack(int atk)					{ _player.SetAttack(atk);				}
		int GetPlayerAttack()							{ return _player.GetAttack();			}
		void HurtPlayer(int atk)						{ _player.ReceiveDamage(atk);			}
		bool PlayerAttack(ISprite& m)					{ return _player.ProcessAttack(m);		}
		int GetPlayerHP()								{ return _player.GetHP();				}


	private:
		void GetInput();
		void ProcessInput();
		void SetDoorDraws(bool state);

		//objects/drawables
		Textbox _textbox;
		Help _help;
		Player _player;
		
		Entity _doors[3];

		//room stuff
		IRoom* _rooms[ROOM_W][ROOM_H];
		ArrayIndex2D _cRoom, _pRoom, _nRoom;
		bool	_changeRoom;					//whether its ok to change room or not
		int		_changeDir;						//direction we want to change
		bool	_gameover;						//whether gameover or not

		//info
		unsigned int _winWidth, _winHeight;
	};
}