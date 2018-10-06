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
			cout << ID << " ���\�n�J\n\n";
		}
		else
		{
			cout << "���Ƶn�J\n";
		}
	}
	void Logout()
	{
		if (ID != "")
		{
			ID = "";
			cout << ID << " ���\�n�X\n";
		}
		else
			cout << "�L�ĵn�X\n";
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
	cout <<"��Jhelp�d�ݩҦ����O" << "\n��J���O�G";
	Record CONNECT;
	while (true)
	{
		string in;
		cin >> in;
		if (in.find("help") == 0)
		{
			cout << endl;
			cout << "-�Ҧ����A�U-----------------\n";
			cout << setw(25) << "ViewRoom:" << setw(20) << "�[�ݩҦ��ж����p\n";
			cout << setw(25) << "SetPlayer:(�o�̥[ID)" << setw(20) << "�ӽбb��\n";
			cout << setw(25) << "ViewPlayer:(�o�̥[ID)" << setw(20) << "�[�ݪ��a���Z\n";

			cout << "-�n�J�e---------------------\n";
			cout << setw(25) << "Login:(�o�̥[ID)" << setw(20) << "�n�J�b��\n";

			cout << "-�n�J��---------------------\n";
			cout << setw(25) << "Logout:" << setw(20) << "�n�X\n";
			cout << setw(25) << "EnterRoom:(�o�̥[�и�)" << setw(20) << "�i�J�ж�\n";
			
			cout << "-�i�Ы�---------------------\n";
			cout << setw(25) << "Play" << setw(20) << "�}�l�C��\n";
			cout << setw(25) << "ExitRoom" << setw(20) << "���}�ж�\n";
			continue;
		}
		if (in.find("Play") == 0)
		{
			if (CONNECT.GET_ID() != "")
			{
				if (CONNECT.CanILeave() != -1)
				{
					BeforeGO();
					cout << "���b�߰ݹ��O�_�ǳƦn\n";
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
								cout << "�n�F\n";
								in = "";
								Goto = 1;
								break;
							}
							else
							{
								if (Line == "NotYet")
									cout << "�еy��\n";
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
							cout << "�ѱz���}�l\n";
						else
							if (CONNECT.Order == 2)
								cout << "�ѹ����}�l\n";
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
									cout << "��J�U�@�ӼƦr�G";
									int num;
									cin >> num;
									system("cls");
									string Send = DuringGame(num);
									Send = "Playing:" + CONNECT.GET_RoomNum() + '<' + CONNECT.GET_ID() + ">_BingoNum[" + Send + ']';
									if (BINGO_Line >= SIZE)
									{
										Send = Send + ",Win";
										cout << "Ĺ�S�I\n";
										cout << "�����ܫ��O�Ҧ��G";
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
												cout << "���Ĺ�S�I\n";
												string SendLost = "Playing:" + CONNECT.GET_RoomNum() + '<' + CONNECT.GET_ID() + ">,Lost";
												SendAndWaitResponse(SendLost);
												cout << "�����ܫ��O�Ҧ��G";
												oppo_Ex = true;
												break;
											}
											cout << "���z�F\n";
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
					cout << "�z�b�j�U�A�L�k�ϥΦ����O\n";
			}
			else
				cout << "�z�|���n�J�A�L�k�i�J�C��\n";
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
						cout << "�L���ж�\n";
					}
					else
					{
						int num = atoi(in.substr(in.find(':')+1).c_str());
						CONNECT.GotoRoom(num);
						if (Line.find("��") != string::npos)
						{
							CONNECT.SetOrder(1);
						}
						else
						{
							if (Line.find("��") != string::npos)
								CONNECT.SetOrder(2);
						}
						cout << Line;
					}
				}
				else
					cout << "�z�w�i�Ц� " + CONNECT.GET_RoomNum() <<" ����\n";
			}
			else
				cout << "�z�|���n�J�A�L�k�i��\n";
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
							cout << "�z�b�j�U�A�L�k�ϥΦ����O\n";
					}
					else
						cout << "�z�|���n�J�A�L�k�ϥΦ����O\n";
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
						cout << "�L���b��\n";
					else
						cout << Line << endl;
				}
			}
		}
	}
	
	CleanUp();
}
