// PlaneBattle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include<iostream>
#include<windows.h>
#include<time.h>
#include<conio.h>
#define H 22
#define W 22
const char PLANECHAR = '8';
const char ENEMYCHAR = '0';
#define ENEMYNUM 5
using namespace std;
class area
{
public:
	char uarea[H][W] = {' '};
	int bullet[W-4] = {-1};
	int score = 0;
	int enemy[ENEMYNUM][2] = {-1 };//0是H坐标，Y是W坐标
	int gamespeed = 500;
	void firstbuild();
	void setplane(int x, int y,bool appear);
	void setenemy(int x, int y,bool appear);
	void moveplane(int &x, int &y, int forward);
	void setbullet(int x, int y);
	void rungame(int &x,int &y,int enemy[][2],int &score);
	void newenemy(int enemy[][2]);
	int controlenemydead(int bullet[W],int enemy[][2]);
	bool controlplanedead(int x, int y, int enemy[][2]);
	void printarea();
	area();
};
void area::firstbuild()
{
	for (int i = 0; i < W; i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < H; j++)
			{
				uarea[i][j] = '#';
			}
		}
		if (i > 0 && i < W - 1)
		{
			uarea[i][0] = '#';
			uarea[i][H - 1] = '#';
		}
		if (i == W - 1)
		{
			for (int j = 0; j < H; j++)
			{
				uarea[i][j] = '#';
			}
		}
	}
}
void area::setenemy(int x, int y,bool appear)
{
	if (appear == true)
	{
		uarea[x][y] = PLANECHAR;
		if (x != 1 && x != H - 2 && y != 1 && y != W - 2)
		{
			uarea[x - 1][y] = PLANECHAR;
			uarea[x + 1][y] = PLANECHAR;
			uarea[x][y + 1] = PLANECHAR;
		}
	}
	if (appear == false)
	{
		uarea[x][y] = ' ';
		if (x != 1 && x != H - 2 && y != 1 && y != W - 2)
		{
			uarea[x - 1][y] = ' ';
			uarea[x + 1][y] = ' ';
			uarea[x][y + 1] = ' ';
		}
	}
	
	
}
void area::setplane(int x, int y,bool appear)
{
	if (appear == true) 
	{
		uarea[x][y] = ENEMYCHAR;
		if (x != 1 && x != H - 2 && y != 1 && y != W - 2)
		{
			uarea[x - 1][y] = ENEMYCHAR;
			uarea[x + 1][y] = ENEMYCHAR;
			uarea[x][y - 1] = ENEMYCHAR;
		}
	}
	if (appear == false)
	{
		uarea[x][y] = ' ';
		if (x != 1 && x != H - 2 && y != 1 && y != W - 2)
		{
			uarea[x - 1][y] =' ';
			uarea[x + 1][y] = ' ';
			uarea[x][y - 1] = ' ';
		}
	}
	
}
void area::moveplane(int &x, int &y, int forward)
{
	if (forward == 0&&x-1!=0)//左
	{
		setplane(x, y, false);
		setplane(x - 1, y, true);
		x = x - 1;
	}
	if (forward == 1&&x+1!=H-1)//右
	{
		setplane(x, y, false);
		setplane(x + 1, y, true);
		x = x + 1;
	}
	if (forward == 2&&y-1!=2)//上
	{
		setplane(x, y, false);
		setplane(x, y - 1, true);
		y = y - 1;
	}
	if (forward == 3&&y+1!=W-1)//下
	{
		setplane(x, y, false);
		setplane(x , y+1, true);
		y = y + 1;
	}
}
void area::setbullet(int x, int y)
{
	
	for (int i = 0; i <y-2; i++)
	{
		bullet[i] = bullet[i + 1];
	}
	bullet[y - 3] = x;
	for (int i = 0; i < W-4; i++)
	{
		int t = bullet[i];
		if (t != -1)
		{
			uarea[t][i + 1] = '\^';
			
		}
		
	}
}
void area::newenemy(int enemy[][2])
{
	int temp1 = 0;
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (enemy[i][1] != H + 1)
		{
			do
			{
				temp1 = rand() % (H - 2);
			} while (temp1 == 0);
			enemy[i][0] = temp1;
			enemy[i][1] = 1;

		}
	}

}
int area::controlenemydead(int bullet[W], int enemy[][2])
{
	int defeat = 0;
	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < ENEMYNUM; j++)
		{
			if (bullet[i] == enemy[j][0] && enemy[j][1] == i)
			{
				bullet[i] = -1;
				std::cout << "\a";
				enemy[j][1] = H + 1;
				defeat++;
				setenemy(enemy[j][0], enemy[j][1], false);
			}
		}
		
	}
	for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][1] != -1)
		{
			setenemy(enemy[j][0], enemy[j][1], true);
		}
		
	}
	return defeat;
}
bool area:: controlplanedead(int x, int y, int enemy[][2])
{
	for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][0] == x && enemy[j][1] == y)
		{
			return true;
		}
	}
	return false;
}
void area::rungame(int &x,int &y,int enemy[][2],int &score)
{
	firstbuild();
	int timeover = 1;
	int direction = 0;
	newenemy(enemy);
	int start = clock();
	while ((timeover = (clock() - start <= gamespeed)) && !_kbhit());//kbhit 没有输入是0，有是非零
	{
		_getch();
		direction = _getch();
	}
	switch (direction)
	{
	case 72:moveplane(x,y,2); break;
	case 80: moveplane(x,y,3); break;
	case 75: moveplane(x,y,0); break;
	case 77: moveplane(x,y,1); break;
	default:break;
	}

	setbullet(x, y);
	int thisscore=controlenemydead(bullet, enemy);
	bool dead=controlplanedead(x, y, enemy);
	printarea();
	if (dead == true)
	{
		exit(0);
	}
	else
	{
		score = score + thisscore;
	}

}
void area::printarea()
{
	system("cls");
	std::cout << std::endl;
	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			std::cout << uarea[j][i];
		}
		std::cout << std::endl;
	}
}
area::area()
{
	int x1 = H / 2;
	int y1 = W - 2;
	
	//cout << "test";
	//firstbuild();
	for (int i = 0; i < W - 4; i++)
	{
		bullet[i] = -1;
	}
	for (int i = 0; i < ENEMYNUM; i++)
	{
		enemy[i][0] = -1;
		enemy[i][1] = -1;
	}
	while (true)
	{

		rungame(x1, y1, enemy, score);

	}
	//newenemy(enemy);
	//setbullet(15, 18);
	//controlenemydead(bullet, enemy);
	/*for (int i = 0; i < ENEMYNUM; i++)
	{
		cout << enemy[i][0]<<endl;
		cout << enemy[i][1]<<endl;
	}*/
	/*for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][1] != H + 1)
		{
			setenemy(enemy[j][0], enemy[j][1], true);
		}

	}*/
	//setenemy(5, 5, true);
	printarea();
	for (int i = 0; i < W - 4; i++)
	{
		cout << "test bullet :"<<bullet[i] << endl;
	}
	for (int i = 0; i < ENEMYNUM; i++)
	{
		cout <<"test enemy X:"<< enemy[i][0] << endl;
		cout <<"test enemy Y:"<< enemy[i][1] << endl;
	}
	
}
int main()
{
	
	area t;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
