// PlaneBattle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include<iostream>
#include<windows.h>
#include<time.h>
#include<conio.h>
#include<fstream>
#define H 22
#define W 22
const char PLANECHAR = '8';
const char ENEMYCHAR = '0';
#define ENEMYNUM 5
using namespace std;
void hello();
class area
{
private:
	char uarea[H][W] = {' '};//uarea:构成整个游戏界面的二维字符数组，H为横坐标，W为纵坐标
	int bullet[W-4] = {-1};//bullet:存储子弹的横坐标，去掉边框和飞机占用的两格总共W-4
	int gamespeed = 500;//gamespeed:刷新速度
	void firstbuild();//firstbuild():向uarea中写入边框
	void setplane(int x, int y,bool appear);//setplane：根据给定的坐标写入或删除玩家飞机x:横坐标y:纵坐标appear:删除还是新增
	void setenemy(int x, int y,bool appear);//setenemy:根据给定的坐标写入或删除敌人x:横坐标y:纵坐标appear:删除还是新增
	void moveplane(int &x, int &y, int forward);//moveplane :根据给定的坐标和方向写入新飞机，删除旧飞机。同时改变坐标。x:横坐标y:纵坐标forward:方向
	void setbullet(int x, int y);//;seybullet:根据给定的飞机位置进行子弹的移动及新增x:横坐标y:纵坐标
	void newenemy(int enemy[][2]);//newenemy:在界面数组的最上方生成新的敌方飞机enemy:敌人数组
	void enemymove(int enemy[][2]);//enemymove:在界面数组中清楚敌人的旧位置，同时在enemy数组中更新敌人的位置
	int controlenemydead(int bullet[W],int enemy[][2]);//controlenemydead:根据给定的子弹数组和敌人位置数组判定敌人是否死亡，同时在界面中打印敌人的位置，更新分数
	bool controlplanedead(int x, int y, int enemy[][2]);//controlplanedead:判定自己的飞机是否死亡x:横坐标y:纵坐标
	void gameover();//gameover:结束游戏并显示分数
	void printarea();//printarea:打印界面数组
public:
	int score ;//score:分数
	int enemy[ENEMYNUM][2];//enemy:敌人的编号及坐标，0是H坐标，Y是W坐标
	area();
	void rungame(int& x, int& y, int enemy[][2], int& score);//rungame:游戏进行的主程序，调用其他函数完成一个循环x:横坐标y:纵坐标enemy:敌人数组score:分数
	void testwrite(int x,int y);
	int round;
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
		uarea[x][y] = ENEMYCHAR;
		if (x > 1 && x < H - 2 && y >1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
		{
			uarea[x - 1][y] =ENEMYCHAR;
			uarea[x + 1][y] = ENEMYCHAR;
			uarea[x][y + 1] = ENEMYCHAR;
		}
	}
	if (appear == false)
	{
		uarea[x][y] = ' ';
		if (x > 1 && x < H - 2 && y >1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
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
		uarea[x][y] = PLANECHAR;
		if (x >1 && x < H - 2 && y >1 && y< W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
		{
			uarea[x - 1][y] = PLANECHAR;
			uarea[x + 1][y] = PLANECHAR;
			uarea[x][y - 1] = PLANECHAR;
		}
	}
	if (appear == false)
	{
		uarea[x][y] = ' ';
		if (x >1 && x < H - 2 && y> 1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
		{
			uarea[x - 1][y] =' ';
			uarea[x + 1][y] = ' ';
			uarea[x][y - 1] = ' ';
		}
	}
	
}
void area::moveplane(int &x, int &y, int forward)
{
	if (forward == 0&&x-1>0)//左
	{
		setplane(x, y, false);
		setplane(x - 1, y, true);
		x = x - 1;
	}
	if (forward == 1&&x+1<H-1)//右
	{
		setplane(x, y, false);
		setplane(x + 1, y, true);
		x = x + 1;
	}
	if (forward == 2&&y-1>2)//上
	{
		setplane(x, y, false);
		setplane(x, y - 1, true);
		y = y - 1;
	}
	if (forward == 3&&y+1<W-1)//下
	{
		setplane(x, y, false);
		setplane(x , y+1, true);
		y = y + 1;
	}
}
void area::setbullet(int x, int y)
{
	
	for (int i = 0; i < W - 4; i++)
	{
		int t = bullet[i];
		if (t != -1&&t<H-2&&t>0)
		{
			uarea[t][i + 1] = ' ';

		}

	}
	
	for (int i = 0; i <y-3; i++)
	{
		bullet[i] = bullet[i + 1];//子弹的移动
	}
	//for (int i = 0; i < W - 4; i++)
	//{
	//	cout << "test break1 "<<i<<"bullet"<< bullet[i] << endl;
	//}
	//Sleep(1000);
	bullet[y - 3] = x;//新增子弹
	/*for (int i = 0; i < W - 4; i++)
	{
		cout << "test break2" << i << "bullet"<<bullet[i] << endl;
	}*/
	//Sleep(1000);
	for (int i = 0; i < W-4; i++)
	{
		int t = bullet[i];
		if (t != -1&&t<=H-2&&t>0)
		{
			uarea[t][i + 1] = '^';
			
		}
		
	}
}
void area::newenemy(int enemy[][2])
{
	int temp1 = 0;
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (enemy[i][1] == -1)
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
			/*下面if中的条件是：碰到敌方飞机的每个点都有效*/
			if ((bullet[i] == enemy[j][0] && enemy[j][1] == i)||(bullet[i] == enemy[j][0] && enemy[j][1] == i-1)|| (bullet[i]+1 == enemy[j][0] && enemy[j][1] == i)||(bullet[i]-1 == enemy[j][0] && enemy[j][1] == i))
			{
				bullet[i] = -1;
				cout << "\a";
				enemy[j][1] = -1;
				enemy[j][0] = -1;
				defeat++;
				setenemy(enemy[j][0], enemy[j][1], false);
			}
		}
		
	}
	for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][1] != -1)
		{
			setenemy(enemy[j][0], enemy[j][1], true);//向界面数组中打印敌机位置
		}
		
	}
	return defeat;
}
bool area:: controlplanedead(int x, int y, int enemy[][2])
{
	for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][0] == x && enemy[j][1] == y)//碰到敌方飞机则死亡，游戏结束
		{
			return true;
		}
	}
	return false;
}
void area::enemymove(int enemy[][2])
{
	for (int j = 0; j < ENEMYNUM; j++)
	{
		if (enemy[j][1] != -1)//删除旧的敌机位置
		{
			setenemy(enemy[j][0], enemy[j][1], false);
		}

	}
	
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (enemy[i][1] == 20)
		{
			enemy[i][0] = -1;
			enemy[i][1] = -1;
		}
		else
		{
			enemy[i][1]++;
		}
	}
}
void area::gameover()
{
	cout << "游\t戏\t结\t束" << endl;
	cout << "您\t的\t分\t数\t是" << endl;
	cout << score<<endl;
	cout << "欢\t迎\t下\t次\t体\t验！"<<endl;
	cout << "任\t意\t键\t退\t出" << endl;
	exit(0);
}
void area::rungame(int &x,int &y,int enemy[][2],int &score)
{
	firstbuild();//边框
	int timeover = 1;
	int direction = 0;
	newenemy(enemy);
	int start = clock();
	while ((clock() - start <= gamespeed) && !_kbhit());//如果没有达到时间伙伴键盘无输入就一直循环
	if (static_cast<bool>(_kbhit()))
	{
		_getch();
		direction = _getch();
	}
	else
	{
		direction = 0;
	}
	switch (direction)
	{
	case 72:moveplane(x,y,2); break;
	case 80: moveplane(x,y,3); break;
	case 75: moveplane(x,y,0); break;
	case 77: moveplane(x,y,1); break;
	case 'q':gameover(); break;
	default:break;
	}
	int x_ = x; int y_ = y;
	setbullet(x_, y_);
	enemymove(enemy);
	int thisscore=controlenemydead(bullet, enemy);
	bool dead=controlplanedead(x_, y_, enemy);
	printarea();
	testwrite(x, y);
	if (dead == true)
	{
		gameover();
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
		if (i == 5)
		{
			cout << "当前分数：" << score;
		}
		cout << std::endl;
	}
}
area::area()
{
	
	
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
	gamespeed = 500;
	score = 0;
	round = 0;
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
	//printarea();
	/*for (int i = 0; i < W - 4; i++)
	{
		cout << "test bullet :"<<bullet[i] << endl;
	}
	for (int i = 0; i < ENEMYNUM; i++)
	{
		cout <<"test enemy X:"<< enemy[i][0] << endl;
		cout <<"test enemy Y:"<< enemy[i][1] << endl;
	}
	*/
}
int main()
{
	hello();
	int x1 = H / 2;
	int y1 = W - 2;
	area t;
	/*while (x1 != 200)
	{
		cin >> x1 >> y1;
		t.setbullet(x1, y1);
		for (int i = 0; i < W - 4; i++)
		{
			cout << "test break" << t.bullet[i] << endl;
		}
	}*/
	//int test = 0;
    while (true)
	{
		/*ofstream out;
		out.open("testbreak.log");
		out << "runtime " << test<<endl;
		for (int i = 0; i < W - 4; i++)
		{
			out << "bullet" << i << "\=";
			out << t.bullet[i];
		}
		out << endl;
		out.close();*/
		t.rungame(x1, y1, t.enemy, t.score);
		//test++;
	}
	
	
}
void hello()
{
	cout << "欢迎体验命令行版本飞机大战！" << endl;
	cout << "说明：方向键控制飞机移动，自动开火，中途可按q键退出" << endl;
	int start1 = clock();
	while (clock() - start1 <= 1000);
	for (int i = 3; i >= 0; i--)
	{
		int start0 = clock();
		while (clock() - start0 <= 1000);
		system("cls");
		if (i > 0)
			cout << "\n\n\t\t倒计时：" << i << endl;
	}
}
void area::testwrite(int x,int y)
{
	fstream fs;
	fs.open("logger.txt", ios::app);
	if (fs.good())
	{
		fs << "the " << round << "round"<<endl;
		fs << "the plane" << "X axis   " << x << endl;
		fs << "the plane" << "Y axis   " << y << endl;
		fs << "the map:" << endl;
		for (int i = 0; i < W; i++)
		{
			for (int j = 0; j < H; j++)
			{
				fs << uarea[j][i];
			}
			if (i == 5)
			{
				fs << "当前分数：" << score;
			}
			fs <<endl;
		}
		fs << "the bullet" << endl;
		for (int i = 0; i < W - 4; i++)
			fs << i << "    " << bullet[i] << "    " << endl;
		fs << "the enemy" << endl;
		for (int i = 0; i < ENEMYNUM; i++)
		{
			fs << i << "x axis   " << enemy[i][0]<<endl;
			fs << i << "y axis    " << enemy[i][1]<<endl;

		}

	}
	fs.close();
	
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
