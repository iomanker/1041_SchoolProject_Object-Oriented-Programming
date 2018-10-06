#include "simplec.h"
#include "Bingo.h"
#include <conio.h>
using namespace std;
class Record
{
private:
	string ID;
	int Room_num;
public:
	int Order;
	Record()
	{
		ID = "";
		Room_num = -1;
	}
	string GET_ID()
	{
		return ID;
	}
	string GET_RoomNum()
	{
		stringstream ss_num;
		ss_num << Room_num;
		return ss_num.str();
	}
	void Login(string Pitch_ID)
	{
		if (ID == "")
		{
			ID = Pitch_ID;
			cout << ID << " 成功登入\n\n";
		}
		else
		{
			cout << "重複登入\n";
		}
	}
	void Logout()
	{
		if (ID != "")
		{
			ID = "";
			cout << ID << " 成功登出\n";
		}
		else
			cout << "無效登出\n";
	}
	int CanILeave()
	{
		if (ID != "")
		{
			return Room_num;
		}
		return -1;
	}
	void BacktoHall()
	{
		Room_num = -1;
	}
	void GotoRoom(int num)
	{
		Room_num = num;
	}
	void SetOrder(int num)
	{
		Order = num;
	}
};

int __cdecl main(int argc, char **argv)
{
	InitAndConn(argc, argv);
	int temp;
	cout <<"輸入help查看所有指令" << "\n輸入指令：";
	Record CONNECT;
	while (true)
	{
		string in;
		cin >> in;
		if (in.find("help") == 0)
		{
			cout << endl;
			cout << "-所有狀態下-----------------\n";
			cout << setw(25) << "ViewRoom:" << setw(20) << "觀看所有房間狀況\n";
			cout << setw(25) << "SetPlayer:(這裡加ID)" << setw(20) << "申請帳號\n";
			cout << setw(25) << "ViewPlayer:(這裡加ID)" << setw(20) << "觀看玩家戰績\n";

			cout << "-登入前---------------------\n";
			cout << setw(25) << "Login:(這裡加ID)" << setw(20) << "登入帳號\n";

			cout << "-登入後---------------------\n";
			cout << setw(25) << "Logout:" << setw(20) << "登出\n";
			cout << setw(25) << "EnterRoom:(這裡加房號)" << setw(20) << "進入房間\n";
			
			cout << "-進房後---------------------\n";
			cout << setw(25) << "Play" << setw(20) << "開始遊戲\n";
			cout << setw(25) << "ExitRoom" << setw(20) << "離開房間\n";
			continue;
		}
		if (in.find("Play") == 0)
		{
			if (CONNECT.GET_ID() != "")
			{
				if (CONNECT.CanILeave() != -1)
				{
					BeforeGO();
					cout << "正在詢問對方是否準備好\n";
					string Ask = "Ask:" + CONNECT.GET_RoomNum() + "_" + CONNECT.GET_ID();

					clock_t begin_t = clock();
					clock_t end_t;
					double elapsed_secs = 0.0;
					int Goto = 0;
					while (1)
					{
						end_t = clock();
						elapsed_secs = double(end_t - begin_t) / CLOCKS_PER_SEC;
						if (elapsed_secs > 3.0)
						{
							string Line = SendAndWaitResponse(Ask);
							if (Line == "Ready")
							{
								cout << "好了\n";
								in = "";
								Goto = 1;
								break;
							}
							else
							{
								if (Line == "NotYet")
									cout << "請稍後\n";
							}
							begin_t = end_t;
							elapsed_secs = 0.0;
						}
					}
					if (Goto == 1)
					{
						system("cls");
						StartGame();
						if (CONNECT.Order == 1)
							cout << "由您先開始\n";
						else
							if (CONNECT.Order == 2)
								cout << "由對方先開始\n";
						while (1)
						{
							bool oppo_Ex = false;
							if (BINGO_Line >= SIZE)
							{
								break;
							}
							else
							{
								if (CONNECT.Order == 1)
								{
									cout << "輸入下一個數字：";
									int num;
									cin >> num;
									system("cls");
									string Send = DuringGame(num);
									Send = "Playing:" + CONNECT.GET_RoomNum() + '<' + CONNECT.GET_ID() + ">_BingoNum[" + Send + ']';
									if (BINGO_Line >= SIZE)
									{
										Send = Send + ",Win";
										cout << "贏惹！\n";
										cout << "切換至指令模式：";
									}
									SendAndWaitResponse(Send);
									if (BINGO_Line >= SIZE)
									{
										break;
									}
									CONNECT.Order = 2;

								}


								clock_t Wait_Begin = clock();
								clock_t Wait_End;
								double Update_secs = 0.0;

								while (1)
								{
									Wait_End = clock();
									Update_secs = double(Wait_End - Wait_Begin) / CLOCKS_PER_SEC;
									if (Update_secs > 3.0 && CONNECT.Order != 1)
									{
										string Get = "Get:" + CONNECT.GET_RoomNum() + '<' + CONNECT.GET_ID() + '>';
										Get = SendAndWaitResponse(Get);
										string ID_Ex = Get.substr(Get.find('<') + 1, Get.find('>') - Get.find('<') - 1);
										if (Get.find("BingoNum") != string::npos && ID_Ex != CONNECT.GET_ID())
										{
											system("cls");
											string num = Get.substr(Get.find('[') + 1, Get.find(']') - Get.find('[') - 1);
											int Tran = atoi(num.c_str());
											DuringGame(Tran);
											CONNECT.Order = 1;
											if (Get.find("Win")!=string::npos)
											{
												cout << "對方贏惹！\n";
												string SendLost = "Playing:" + CONNECT.GET_RoomNum() + '<' + CONNECT.GET_ID() + ">,Lost";
												SendAndWaitResponse(SendLost);
												cout << "切換至指令模式：";
												oppo_Ex = true;
												break;
											}
											cout << "換您了\n";
											break;
										}
										Wait_Begin = Wait_End;
										Update_secs = 0.0;
									}
								}
								if (oppo_Ex == true)
									break;
							}
						}
					}
					in = "";
				}
				else
					cout << "您在大廳，無法使用此指令\n";
			}
			else
				cout << "您尚未登入，無法進入遊戲\n";
		}
		if (in.find("EnterRoom:") == 0)
		{
			if (CONNECT.GET_ID() != "")
			{
				if (CONNECT.GET_RoomNum() == "-1")
				{
					in = in +":"+ CONNECT.GET_ID();
					string Line = SendAndWaitResponse(in);
					if (Line.find("Fail") == 0)
					{
						cout << "無此房間\n";
					}
					else
					{
						int num = atoi(in.substr(in.find(':')+1).c_str());
						CONNECT.GotoRoom(num);
						if (Line.find("１") != string::npos)
						{
							CONNECT.SetOrder(1);
						}
						else
						{
							if (Line.find("２") != string::npos)
								CONNECT.SetOrder(2);
						}
						cout << Line;
					}
				}
				else
					cout << "您已進房至 " + CONNECT.GET_RoomNum() <<" 號房\n";
			}
			else
				cout << "您尚未登入，無法進房\n";
		}
		else
		{
			if (in.find("Logout") == 0)
			{
				CONNECT.Logout();
			}
			else
			{
				if (in.find("ExitRoom") == 0)
				{
					if (CONNECT.GET_ID() != "")
					{
						if (CONNECT.CanILeave() != -1)
						{
							string Send = "ExitRoom:" + CONNECT.GET_RoomNum() + ":" + CONNECT.GET_ID();
							cout << SendAndWaitResponse(Send);
							CONNECT.BacktoHall();
						}
						else
							cout << "您在大廳，無法使用此指令\n";
					}
					else
						cout << "您尚未登入，無法使用此指令\n";
					continue;
				}
				if (in == "")
					continue;
				string Line = SendAndWaitResponse(in);
				if (Line.find("Connect") == 0)
				{
					string ID = Line.substr(8);
					CONNECT.Login(ID);
				}
				else
				{
					if (Line.find("Fail:Login") == 0)
						cout << "無此帳號\n";
					else
						cout << Line << endl;
				}
			}
		}
	}
	
	CleanUp();
}
