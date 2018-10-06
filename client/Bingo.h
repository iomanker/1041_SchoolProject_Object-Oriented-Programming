#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <sstream>
using namespace std;
int map[10][10];
int check[100];
int Bingo_Col[10], Bingo_Row[10], Bingo_Dia[2], BINGO_Line;
int SIZE, temp, Counter;
bool Accept;
bool Exist(int x)
{
	if (check[x] == 1)
		return true;
	else
		return false;
}

void TAG(int i, int j, int SIZE)
{
	++Bingo_Row[i];  ++Bingo_Col[j];
	if (i == j)
	{
		++Bingo_Dia[0];
		if (Bingo_Dia[0] == SIZE)
			++BINGO_Line;
	}
	if (i + j == SIZE - 1)
	{
		++Bingo_Dia[1];
		if (Bingo_Dia[1] == SIZE)
			++BINGO_Line;
	}
	if (Bingo_Row[i] == SIZE)
		++BINGO_Line;
	if (Bingo_Col[j] == SIZE)
		++BINGO_Line;
}

void ViewMap()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << setw(5);
			if (map[i][j] == -1)
				cout << "��";
			else
				cout << map[i][j];
			cout << ' ';
		}
		cout << endl << endl;
	}
	cout << setw(15) << BINGO_Line << endl;
	return;
}


void BeforeGO()
{
	//�ƫe�ǳ�
	SIZE = 5;
	cout << "\n���C���� 5x5 Bingo\n";
	string str;
		memset(check, 0, sizeof(check));
		for (int i = 0; i < SIZE; ++i)
		{
			Counter = 0;
			vector<int> stk;
			Accept = true;
			cout << "�п�J�� " << i + 1 << "�C�Ʀr(" << SIZE << "��)�G";
			while (cin >> temp)
			{
				Counter++;
				stk.push_back(temp);
				if (Counter == SIZE)
					break;
			}
			for (int k = 0; k<SIZE; k++)
			{
				if (stk[k]>SIZE*SIZE || stk[k] <= 0)
				{
					Accept = false;
					cout << "�W�L�d��\n";
					stk.clear();
					i--;
					break;
				}

				for (int j = k + 1; j < SIZE; j++)
				{
					if (stk[k] == stk[j])
					{
						Accept = false;
						break;
					}
				}
				bool CHECK = Exist(stk[k]);
				if (Accept == false || CHECK == true)
				{
					Accept = false;
					cout << "���ƿ�J\n";
					stk.clear();
					i--;
					break;
				}

			}
			if (Accept)
			{
				for (int j = 0; j < SIZE; j++)
				{
					map[i][j] = stk[j];
					check[stk[j]] = 1;
				}
				stk.clear();
			}
		}
		return;
}



void StartGame()
{
		//�i�J�C��
		system("cls");
		for (int i = 0; i<SIZE; i++)
		{
			for (int j = 0; j<SIZE; j++)
				cout << setw(5) << map[i][j] << ' ';
			cout << endl << endl;
		}

		memset(Bingo_Row, 0, sizeof(Bingo_Row));
		memset(Bingo_Col, 0, sizeof(Bingo_Col));
		memset(Bingo_Dia, 0, sizeof(Bingo_Dia));
		BINGO_Line = 0;
		cout << "��J�Ʀr�H�}�l�C���G";
		return;
}

string DuringGame(int num)
{
	temp = num;
	while (1)
	{
		if (temp>25 || temp < 1)
		{
			ViewMap();
			cout << "\n�W�L�Ʀr�d��A�Э��s��J�G";
			cin >> temp;
			system("cls");
			continue;
		}
		else
		{
			string status = "";
			Accept = true;
			if (check[temp] != 2)
			{
				for (int i = 0; i < SIZE; i++)
				{
					for (int j = 0; j < SIZE; j++)
					{
						if (map[i][j] == temp)
						{
							Accept = false;
							map[i][j] = -1;
							TAG(i, j, SIZE);
							break;
						}
					}
					if (Accept == false)
						break;
				}
				check[temp] = 2;
				stringstream ss;
				ss << temp;
				status = ss.str();
			}
			else
			{
				ViewMap();
				cout << "\n���g��L�S�I�Э��s��J�G";
				cin >> temp;
				system("cls");
				continue;
			}
			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					cout << setw(5);
					if (map[i][j] == -1)
						cout << "��";
					else
						cout << map[i][j];
					cout << ' ';
				}
				cout << endl << endl;
			}
			cout << setw(15) << BINGO_Line << endl;
			return status;
		}
	}
}