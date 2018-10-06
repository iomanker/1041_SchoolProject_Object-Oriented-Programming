#include "Main.h"
#include <vector>
#include <sstream>
using namespace std;


string reply;
class Player
{
private:
	string ID;
	int Win, Lost;
	int Location;
public:
	Player(string Add_name)
	{
		Win = 0, Lost = 0;
		ID = Add_name;
		cout << ID << "�ظm����\n";
	}
	string Match_ID()
	{
		return ID;
	}
	void Read_Record()
	{
		stringstream ss_WIN,ss_LOST;
		ss_WIN << Win;
		ss_LOST <<Lost;

		string Record; 
		string win = ss_WIN.str() , lost = ss_LOST.str();
		reply = "ID:" + ID + "\nĹ�G " + win + " ���A��G " + lost + " ���C\n";
	}
	void ADD_Win()
	{
		++Win;
	}
	void ADD_Lost()
	{
		++Lost;
	}
};

class Room
{
private:
	int Room_No;
	string Room_Name;
	string player_1, player_2;
	int Al_1P, Al_2P;
	string New_Message;
	int Already_Read;
public:
	Room(int num)
	{
		Room_No = num;
		Room_Name = "�L�ЦW";
		player_1 = "";
		player_2 = "";
		New_Message = "";
		Already_Read = 0;
	}

	string Read_RoomName()
	{
		return Room_Name;
	}
	string Read_RoomValue()
	{
		stringstream ss;
		ss << Room_No + 1;
		string num = ss.str();
		return num;
	}
	string GET_Player1()
	{
		return player_1;
	}
	string GET_Player2()
	{
		return player_2;
	}
	// �i�� �h��
	void EnterRoom(string Enter_player)
	{
		if (player_1 == "")
		{
			player_1 = Enter_player;
			reply = player_1 + "�H���a�������A���\�i�J�ж��C\n";
			Al_1P = 0;
		}
		else
		{
			if (player_2 == "")
			{
				player_2 = Enter_player;
				reply = player_2 + "�H���a�������A���\�i�J�ж��C\n";
				Al_2P = 0;
			}
			else
			{
				reply = "���ж��w���A�w��^�j�U�C\n";
			}
		}
		New_Message = ""; Already_Read = 0;
	}
	void ExitRoom(string Exit_player)
	{
		if (Exit_player == player_1)
		{
			player_1 = "";
			reply = Exit_player + " ���\���}�ж��C\n";
			Al_1P = 0;
			return;
		}
		else
		{
			if (Exit_player == player_2)
			{
				player_2 = "";
				reply = Exit_player + " ���\���}�ж��C\n";
				Al_2P = 0;
				return;
			}
			else
				reply = "�d�L���H\n";
		}
		New_Message = ""; Already_Read = 0;
	}
	// �s���B�ШD�T��
	void Store_Message(string Context)
	{
		New_Message = Context;
	}
	string Load_Message(string Player_ID) // about Already_Read to:Who_BingoNum[] , EndofGame , SetGame:OK or No
	{
		return New_Message;
	}
	string Ask(string Already)
	{
		if (Already == player_1)
		{
			Al_1P = 1;
		}
		if (Already == player_2)
		{
			Al_2P = 1;
		}
		if (Al_1P + Al_2P == 2)
		{
			return "Ready";
		}
		else
			return "NotYet";
	}
};
vector <Room> Room_stk;
vector <Player> Player_stk;
void Construct_Room()
{
	for (int i = 0; i < 5; ++i)
	{
		Room_stk.push_back(i);
	}
}
void Construct_Player(string ID)
{
	for (int i = 0; i < Player_stk.size(); i++)
	{
		if (ID == Player_stk[i].Match_ID())
		{
			reply = ID + " �A���ƫإ�\n";
			return;
		}
	}
	Player_stk.push_back(ID);
	reply = "�b���G " + ID + " �A�Ыئ��\�C\n";
	return;
}

void Travel_Player(string ID)
{
	for (int i = 0; i < Player_stk.size();i++)
	{
		if (ID == Player_stk[i].Match_ID() )
		{
			Player_stk[i].Read_Record();
			break;
		}
	}
}

string MATCH_ID(string ID)
{
	for (int i = 0; i < Player_stk.size(); i++)
	{
		if (ID == Player_stk[i].Match_ID())
		{
			string temp = "Connect:" + Player_stk[i].Match_ID();
			return temp;
		}
	}
	return "Fail:Login\n";
}

string ViewRoom()
{
	string Record="";
	for (int i = 0; i < 5; ++i)
	{
		stringstream ss;
		ss << i+1;
		string num = ss.str();
		Record = Record + "\n�и��G " + Room_stk[i].Read_RoomValue() /*+ " / �ЦW�G" + Room_stk[i].Read_RoomName()*/;
		string PLAYER = Room_stk[i].GET_Player1() +"�B"+ Room_stk[i].GET_Player2();
		if (PLAYER == "�B")
		{
			Record += "\n          �L���a\n";
		}
		else
		{
			Record += "\n          ���a�G" + PLAYER + "\n";
		}
	}
	return Record;
}

void Init()
{
	Construct_Room();
	cout << "�ж��ظm����\n";
}

string WaitAndReply(string rec)
{
	cout << "[Receive] " << rec << endl;
	reply = "";
	if (rec.find("Playing:") == 0 || rec.find("Get") == 0)
	{
		string Name = rec.substr(rec.find('<') + 1, rec.find('>') - rec.find('<') - 1);
		string Room_Num = rec.substr(rec.find(':') + 1, 1);
		int num = atoi(Room_Num.c_str()) - 1;

		if (rec.find("Playing:") == 0)
		{
			string Context = rec.substr(rec.find('<'));
			if (Context.find("Win") != string::npos)
			{
				for (int i = 0; i < Player_stk.size(); i++)
				{
					if (Name == Player_stk[i].Match_ID())
					{
						Player_stk[i].ADD_Win();
					}
				}
			}
			if (Context.find("Lost") != string::npos)
			{
				for (int i = 0; i < Player_stk.size(); i++)
				{
					if (Name == Player_stk[i].Match_ID())
					{
						Player_stk[i].ADD_Lost();
						break;
					}
				}
			}
			else
			{
				cout << rec << endl;
				cout << Context << endl;
				Room_stk[num].Store_Message(Context);
			}
		}
		else
		{
			if (rec.find("Get") == 0)
			{
				string temp = Room_stk[num].Load_Message(Name);
				cout << rec << endl;
				cout << temp << endl;
				return temp;
			}
		}
	}
	else
	{
		if (rec.find(':') != string::npos)
		{
			bool Find_Command = false;
			string Command = rec.substr(0, rec.find(':'));
			if (Command == "ViewRoom")
			{
				reply = ViewRoom();
				Find_Command = true;
			}
			if (Command == "SetPlayer")
			{
				string ID = rec.substr(rec.find(':') + 1);
				Construct_Player(ID);
				Find_Command = true;
			}
			if (Command == "ViewPlayer")
			{
				string ID = rec.substr(rec.find(':') + 1);
				Travel_Player(ID);
				Find_Command = true;
			}
			if (Command == "Login")
			{
				string ID = rec.substr(rec.find(':') + 1);
				reply = MATCH_ID(ID);
				Find_Command = true;
			}
			if (Command == "EnterRoom" && Command != "EnterRoom:")
			{
				string Room_Num = rec.substr(rec.find(':') + 1, rec.find(':', 10));
				string Enter_ID = rec.substr(rec.find(':', 10) + 1);
				int num = atoi(Room_Num.c_str()) - 1;
				if (num > 5 || num < 0)
					reply = "Fail";
				else
					Room_stk[num].EnterRoom(Enter_ID);
				Find_Command = true;
			}
			if (Command == "ExitRoom")
			{
				string Room_Num = rec.substr(rec.find(':') + 1, rec.find(':', 9));
				string Exit_ID = rec.substr(rec.find(':', 9) + 1);
				int num = atoi(Room_Num.c_str()) - 1;
				cout << Exit_ID << endl;
				Room_stk[num].ExitRoom(Exit_ID);
				Find_Command = true;
			}
			if (Command == "Ask")
			{
				string Room_Num = rec.substr(rec.find(':') + 1 , rec.find('_') - rec.find(':') - 1);
				int num = atoi(Room_Num.c_str()) - 1;
				string Already = rec.substr(rec.find('_') + 1);
				reply = Room_stk[num].Ask(Already);
				Find_Command = true;
			}
			if (Find_Command == false)
				reply = "�L�����O\n";
		}
		else
		{
			reply = "�L�ī��O\n";
		}
	}
	
	cout << "[Reply] " << reply << endl;
	return reply;
}
