1  ﻿// PlaneBattle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
2  //
3  
4  
5  #include<iostream>
6  #include<windows.h>
7  #include<time.h>
8  #include<conio.h>
9  #include<fstream>
10  #include <string>
11  #define H 22
12  #define W 22
13  const char PLANECHAR = '8';
14  const char ENEMYCHAR = '0';
15  #define ENEMYNUM 5
16  using namespace std;
17  void hello();
18  class area
19  {
20  private:
21  	char uarea[H][W] = {' '};//uarea:构成整个游戏界面的二维字符数组，H为横坐标，W为纵坐标
22  	int bullet[W-4] = {-1};//bullet:存储子弹的横坐标，去掉边框和飞机占用的两格总共W-4
23  	int gamespeed = 500;//gamespeed:刷新速度
24  	void firstbuild();//firstbuild():向uarea中写入边框
25  	void setplane(int x, int y,bool appear);//setplane：根据给定的坐标写入或删除玩家飞机x:横坐标y:纵坐标appear:删除还是新增
26  	void setenemy(int x, int y,bool appear);//setenemy:根据给定的坐标写入或删除敌人x:横坐标y:纵坐标appear:删除还是新增
27  	void moveplane(int &x, int &y, int forward);//moveplane :根据给定的坐标和方向写入新飞机，删除旧飞机。同时改变坐标。x:横坐标y:纵坐标forward:方向
28  	void setbullet(int x, int y);//;seybullet:根据给定的飞机位置进行子弹的移动及新增x:横坐标y:纵坐标
29  	void newenemy(int enemy[][2]);//newenemy:在界面数组的最上方生成新的敌方飞机enemy:敌人数组
30  	void enemymove(int enemy[][2]);//enemymove:在界面数组中清楚敌人的旧位置，同时在enemy数组中更新敌人的位置
31  	int controlenemydead(int bullet[W],int enemy[][2]);//controlenemydead:根据给定的子弹数组和敌人位置数组判定敌人是否死亡，同时在界面中打印敌人的位置，更新分数
32  	bool controlplanedead(int x, int y, int enemy[][2]);//controlplanedead:判定自己的飞机是否死亡x:横坐标y:纵坐标
33  	void gameover();//gameover:结束游戏并显示分数
34  	void printarea();//printarea:打印界面数组
35  public:
36  	int score ;//score:分数
37  	int enemy[ENEMYNUM][2];//enemy:敌人的编号及坐标，0是H坐标，Y是W坐标
38  	area();
39  	void rungame(int& x, int& y, int enemy[][2], int& score);//rungame:游戏进行的主程序，调用其他函数完成一个循环x:横坐标y:纵坐标enemy:敌人数组score:分数
40  	void testwrite(int x,int y);
41  	int round;
42  	bool con;
43  };
44  void area::firstbuild()
45  {
46  	for (int i = 0; i < W; i++)
47  	{
48  		if (i == 0)
49  		{
50  			for (int j = 0; j < H; j++)
51  			{
52  				uarea[i][j] = '#';
53  			}
54  		}
55  		if (i > 0 && i < W - 1)
56  		{
57  			uarea[i][0] = '#';
58  			uarea[i][H - 1] = '#';
59  		}
60  		if (i == W - 1)
61  		{
62  			for (int j = 0; j < H; j++)
63  			{
64  				uarea[i][j] = '#';
65  			}
66  		}
67  	}
68  }
69  void area::setenemy(int x, int y,bool appear)
70  {
71  	if (appear == true)
72  	{
73  		uarea[x][y] = ENEMYCHAR;
74  		if (x > 1 && x < H - 2 && y >1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
75  		{
76  			uarea[x - 1][y] =ENEMYCHAR;
77  			uarea[x + 1][y] = ENEMYCHAR;
78  			uarea[x][y + 1] = ENEMYCHAR;
79  		}
80  	}
81  	if (appear == false)
82  	{
83  		uarea[x][y] = ' ';
84  		if (x > 1 && x < H - 2 && y >1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
85  		{
86  			uarea[x - 1][y] = ' ';
87  			uarea[x + 1][y] = ' ';
88  			uarea[x][y + 1] = ' ';
89  		}
90  	}
91  	
92  	
93  }
94  void area::setplane(int x, int y,bool appear)
95  {
96  	if (appear == true) 
97  	{
98  		uarea[x][y] = PLANECHAR;
99  		if (x >1 && x < H - 2 && y >1 && y< W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
100  		{
101  			uarea[x - 1][y] = PLANECHAR;
102  			uarea[x + 1][y] = PLANECHAR;
103  			uarea[x][y - 1] = PLANECHAR;
104  		}
105  	}
106  	if (appear == false)
107  	{
108  		uarea[x][y] = ' ';
109  		if (x >1 && x < H - 2 && y> 1 && y < W - 2)//如果碰到边框就不显示飞机形状，只显示一个点
110  		{
111  			uarea[x - 1][y] =' ';
112  			uarea[x + 1][y] = ' ';
113  			uarea[x][y - 1] = ' ';
114  		}
115  	}
116  	
117  }
118  void area::moveplane(int &x, int &y, int forward)
119  {
120  	if (forward == 0&&x-1>0)//左
121  	{
122  		setplane(x, y, false);
123  		setplane(x - 1, y, true);
124  		x = x - 1;
125  	}
126  	if (forward == 1&&x+1<H-1)//右
127  	{
128  		setplane(x, y, false);
129  		setplane(x + 1, y, true);
130  		x = x + 1;
131  	}
132  	if (forward == 2&&y-1>2)//上
133  	{
134  		setplane(x, y, false);
135  		setplane(x, y - 1, true);
136  		y = y - 1;
137  	}
138  	if (forward == 3&&y+1<W-1)//下
139  	{
140  		setplane(x, y, false);
141  		setplane(x , y+1, true);
142  		y = y + 1;
143  	}
144  }
145  void area::setbullet(int x, int y)
146  {
147  	
148  	for (int i = 0; i < W - 4; i++)
149  	{
150  		int t = bullet[i];
151  		if (t != -1&&t<H-2&&t>0)
152  		{
153  			uarea[t][i + 1] = ' ';
154  
155  		}
156  
157  	}
158  	
159  	for (int i = 0; i <y-3; i++)
160  	{
161  		bullet[i] = bullet[i + 1];//子弹的移动
162  	}
163  	//for (int i = 0; i < W - 4; i++)
164  	//{
165  	//	cout << "test break1 "<<i<<"bullet"<< bullet[i] << endl;
166  	//}
167  	//Sleep(1000);
168  	bullet[y - 3] = x;//新增子弹
169  	/*for (int i = 0; i < W - 4; i++)
170  	{
171  		cout << "test break2" << i << "bullet"<<bullet[i] << endl;
172  	}*/
173  	//Sleep(1000);
174  	for (int i = 0; i < W-4; i++)
175  	{
176  		int t = bullet[i];
177  		if (t != -1&&t<=H-2&&t>0)
178  		{
179  			uarea[t][i + 1] = '^';
180  			
181  		}
182  		
183  	}
184  }
185  void area::newenemy(int enemy[][2])
186  {
187  	int temp1 = 0;
188  	for (int i = 0; i < ENEMYNUM; i++)
189  	{
190  		if (enemy[i][1] == -1)
191  		{
192  			do
193  			{
194  				temp1 = rand() % (H - 2);
195  			} while (temp1 == 0);
196  			enemy[i][0] = temp1;
197  			enemy[i][1] = 1;
198  
199  		}
200  	}
201  
202  }
203  int area::controlenemydead(int bullet[W], int enemy[][2])
204  {
205  	int defeat = 0;
206  	for (int i = 0; i < W; i++)
207  	{
208  		for (int j = 0; j < ENEMYNUM; j++)
209  		{
210  			/*下面if中的条件是：碰到敌方飞机的每个点都有效*/
211  			if ((bullet[i] == enemy[j][0] && enemy[j][1] == i)||(bullet[i] == enemy[j][0] && enemy[j][1] == i-1)|| (bullet[i]+1 == enemy[j][0] && enemy[j][1] == i)||(bullet[i]-1 == enemy[j][0] && enemy[j][1] == i))
212  			{
213  				uarea[bullet[i]][i + 1] = ' ';
214  				bullet[i] = -1;
215  				cout << "\a";
216  				enemy[j][1] = -1;
217  				enemy[j][0] = -1;
218  				defeat++;
219  				setenemy(enemy[j][0], enemy[j][1], false);
220  			}
221  		}
222  		
223  	}
224  	for (int j = 0; j < ENEMYNUM; j++)
225  	{
226  		if (enemy[j][1] != -1)
227  		{
228  			setenemy(enemy[j][0], enemy[j][1], true);//向界面数组中打印敌机位置
229  		}
230  		
231  	}
232  	return defeat;
233  }
234  bool area:: controlplanedead(int x, int y, int enemy[][2])
235  {
236  	for (int j = 0; j < ENEMYNUM; j++)
237  	{
238  		if (enemy[j][0] == x && enemy[j][1] == y)//碰到敌方飞机则死亡，游戏结束
239  		{
240  			return true;
241  		}
242  	}
243  	return false;
244  }
245  void area::enemymove(int enemy[][2])
246  {
247  	for (int j = 0; j < ENEMYNUM; j++)
248  	{
249  		if (enemy[j][1] != -1)//删除旧的敌机位置
250  		{
251  			setenemy(enemy[j][0], enemy[j][1], false);
252  		}
253  
254  	}
255  	
256  	for (int i = 0; i < ENEMYNUM; i++)
257  	{
258  		if (enemy[i][1] == 20)
259  		{
260  			enemy[i][0] = -1;
261  			enemy[i][1] = -1;
262  		}
263  		else
264  		{
265  			enemy[i][1]++;
266  		}
267  	}
268  }
269  void area::gameover()
270  {
271  	cout << "游\t戏\t结\t束" << endl;
272  	cout << "您\t的\t分\t数\t是" << endl;
273  	cout << score<<endl;
274  	cout << "欢\t迎\t下\t次\t体\t验！"<<endl;
275  	cout << "任\t意\t键\t退\t出" << endl;
276  	exit(0);
277  }
278  void area::rungame(int &x,int &y,int enemy[][2],int &score)
279  {
280  	firstbuild();//边框
281  	int timeover = 1;
282  	int direction = 0;
283  	newenemy(enemy);
284  	int start = clock();
285  	while ((clock() - start <= gamespeed) && !_kbhit());//如果没有达到时间伙伴键盘无输入就一直循环
286  	if (static_cast<bool>(_kbhit()))
287  	{
288  		_getch();
289  		direction = _getch();
290  	}
291  	else
292  	{
293  		direction = 0;
294  	}
295  	switch (direction)
296  	{
297  	case 72:moveplane(x,y,2); break;
298  	case 80: moveplane(x,y,3); break;
299  	case 75: moveplane(x,y,0); break;
300  	case 77: moveplane(x,y,1); break;
301  	case 'q':gameover(); break;
302  	default:break;
303  	}
304  	int x_ = x; int y_ = y;
305  	setbullet(x_, y_);
306  	enemymove(enemy);
307  	int thisscore=controlenemydead(bullet, enemy);
308  	bool dead=controlplanedead(x_, y_, enemy);
309  	printarea();
310  	//testwrite(x, y);
311  	if (dead == true)
312  	{
313  		gameover();
314  	}
315  	else
316  	{
317  		score = score + thisscore;
318  	}
319  
320  }
321  void area::printarea()
322  {
323  	system("cls");
324  	std::cout << std::endl;
325  	for (int i = 0; i < W; i++)
326  	{
327  		for (int j = 0; j < H; j++)
328  		{
329  			std::cout << uarea[j][i];
330  		}
331  		if (i == 5)
332  		{
333  			cout << "当前分数：" << score;
334  		}
335  		cout << std::endl;
336  	}
337  }
338  area::area()
339  {
340  	
341  	
342  	//cout << "test";
343  	//firstbuild();
344  	for (int i = 0; i < W - 4; i++)
345  	{
346  		bullet[i] = -1;
347  	}
348  	for (int i = 0; i < ENEMYNUM; i++)
349  	{
350  		enemy[i][0] = -1;
351  		enemy[i][1] = -1;
352  	}
353  	gamespeed = 500;
354  	score = 0;
355  	round = 0;
356  	//newenemy(enemy);
357  	//setbullet(15, 18);
358  	//controlenemydead(bullet, enemy);
359      /*for (int i = 0; i < ENEMYNUM; i++)
360  	{
361  		cout << enemy[i][0]<<endl;
362  		cout << enemy[i][1]<<endl;
363  	}*/
364  	/*for (int j = 0; j < ENEMYNUM; j++)
365  	{
366  		if (enemy[j][1] != H + 1)
367  		{
368  			setenemy(enemy[j][0], enemy[j][1], true);
369  		}
370  
371  	}*/
372  	//setenemy(5, 5, true);
373  	//printarea();
374  	/*for (int i = 0; i < W - 4; i++)
375  	{
376  		cout << "test bullet :"<<bullet[i] << endl;
377  	}
378  	for (int i = 0; i < ENEMYNUM; i++)
379  	{
380  		cout <<"test enemy X:"<< enemy[i][0] << endl;
381  		cout <<"test enemy Y:"<< enemy[i][1] << endl;
382  	}
383  	*/
384  }
385  
386  void hello()
387  {
388  	cout << "欢迎体验命令行版本飞机大战！" << endl;
389  	cout << "说明：方向键控制飞机移动，自动开火，中途可按q键退出" << endl;
390  	int start1 = clock();
391  	while (clock() - start1 <= 1000);
392  	for (int i = 3; i >= 0; i--)
393  	{
394  		int start0 = clock();
395  		while (clock() - start0 <= 1000);
396  		system("cls");
397  		if (i > 0)
398  			cout << "\n\n\t\t倒计时：" << i << endl;
399  	}
400  }
401  void area::testwrite(int x,int y)
402  {
403  	fstream fs;
404  	fs.open("logger.txt", ios::app);
405  	if (fs.good())
406  	{
407  		fs << "the " << round << "round"<<endl;
408  		fs << "the plane" << "X axis   " << x << endl;
409  		fs << "the plane" << "Y axis   " << y << endl;
410  		fs << "the map:" << endl;
411  		for (int i = 0; i < W; i++)
412  		{
413  			for (int j = 0; j < H; j++)
414  			{
415  				fs << uarea[j][i];
416  			}
417  			if (i == 5)
418  			{
419  				fs << "当前分数：" << score;
420  			}
421  			fs <<endl;
422  		}
423  		fs << "the bullet" << endl;
424  		for (int i = 0; i < W - 4; i++)
425  			fs << i << "    " << bullet[i] << "    " << endl;
426  		fs << "the enemy" << endl;
427  		for (int i = 0; i < ENEMYNUM; i++)
428  		{
429  			fs << i << "x axis   " << enemy[i][0]<<endl;
430  			fs << i << "y axis    " << enemy[i][1]<<endl;
431  
432  		}
433  
434  	}
435  	fs.close();
436  	
437  }
438  #define LENGTH 8
439  #define TIME 3
440  class Login {
441  private:
442  	struct user
443  	{
444  		string name;
445  		string score;
446  		string password;
447  	};
448  	user* userp;
449  public:
450  	void set(int num0);
451  	void set_password(string& usename, string& password);
452  	bool login_in(const string& usename, const string& password);
453  	Login(int num);
454  	bool log(int num0);
455  	~Login();
456  	void setscore(int num, int score0);
457  };
458  void Login::set_password(string& usename, string& password)
459  {
460  	bool again = true;
461  	bool tmp1 = true;
462  	bool tmp2 = true;
463  	cout << "please input your name:";
464  	cin>>usename;
465  	do
466  	{
467  		cout << "please input your password:";
468  		getline(cin, password, '\n');
469  		for (int i = 0; i < password.length(); i++)
470  		{
471  			if (islower(password.at(i)) || isupper(password.at(i)))
472  			{
473  				for (int j = 0; j < password.length(); j++)
474  				{
475  					if (isalpha(password.at(j)))
476  					{
477  						tmp2 = false;
478  						break;
479  					}
480  
481  				}
482  			}
483  
484  
485  		}
486  		if (tmp2 == true)
487  		{
488  			cout << "your password must include number and letters" << endl;
489  		}
490  		if (password.length() < LENGTH)
491  		{
492  			cout << "Password length must be greater than 8" << endl;
493  			tmp1 = true;
494  		}
495  		if (password.length() >= LENGTH)
496  		{
497  			tmp1 = false;
498  		}
499  		if (!(tmp1 || tmp2))
500  		{
501  			string password2;
502  			cout << "please enter your password again:";
503  			getline(cin, password2, '\n');
504  			if (password == password2)
505  			{
506  				cout << "Register Success!" << endl;
507  				again = false;
508  			}
509  			else
510  			{
511  				cout << "the password must be same" << endl;
512  				again = true;
513  			}
514  		}
515  	} while (again);
516  
517  };
518  
519  bool Login::login_in(const string& usename, const string& password)
520  {
521  	int logintime = TIME;
522  	string passwordlogin;
523  	cout << "your name is " << usename << endl;
524  	cout << "input password to login,you have three times:";
525  
526  	while (logintime > 0)
527  	{
528  		cin >> passwordlogin;
529  		if (passwordlogin == password)
530  		{
531  			cout << "Login Success!" << endl;
532  			return true;
533  		}
534  		else
535  		{
536  			logintime--;
537  			if (logintime != 0)
538  				cout << "your password is not right,please try again:";
539  
540  		}
541  
542  	}
543  	if (logintime <= 0)
544  	{
545  		cout << "Fail to login,time is up!" << endl;
546  	}
547  	return false;
548  }
549  Login::Login(int num)
550  {
551  	userp = new user[num];
552  }
553  void Login::set(int num0)
554  {
555  	set_password((userp + num0)->name, (userp + num0)->password);
556  }
557  bool Login::log(int num0)
558  {
559  	 return login_in((userp + num0)->name, (userp + num0)->password);
560  }
561  Login::~Login()
562  {
563  	delete[] userp;
564  }
565  void Login::setscore(int num,int score0)
566  {
567  	(userp + num)->score = score0;
568  }
569  int main()
570  {
571  	HANDLE consolehwnd;
572  	consolehwnd =GetStdHandle(STD_OUTPUT_HANDLE);
573  	SetConsoleTextAttribute(consolehwnd, 250);
574  	cout << "命\t令\t行\t版\t本\t飞\t机\t大\t战\n" << endl;
575  	cout << "请输入玩家数量" << endl;
576  	int num;
577  	cin >> num;
578  	Login log_ = Login(num);
579  	for (int i = 0; i < num; i++)
580  	{
581  		cout << "请输入第" << i + 1 << "玩家的账号信息" << endl;
582  		log_.set(i);
583  	}
584  	cout << "您要登录哪位玩家的账号" << endl;
585  	cin >> num;
586  	if (log_.log(num - 1))
587  	{
588  		hello();
589  		int x1 = H / 2;
590  		int y1 = W - 2;
591  		area t;
592  		/*while (x1 != 200)
593  		{
594  			cin >> x1 >> y1;
595  			t.setbullet(x1, y1);
596  			for (int i = 0; i < W - 4; i++)
597  			{
598  				cout << "test break" << t.bullet[i] << endl;
599  			}
600  		}*/
601  		//int test = 0;
602  		while (true)
603  		{
604  			/*ofstream out;
605  			out.open("testbreak.log");
606  			out << "runtime " << test<<endl;
607  			for (int i = 0; i < W - 4; i++)
608  			{
609  				out << "bullet" << i << "\=";
610  				out << t.bullet[i];
611  			}
612  			out << endl;
613  			out.close();*/
614  			t.rungame(x1, y1, t.enemy, t.score);
615  			//test++;
616  		}
617  	}
618  	else
619  	{
620  		cout << "密码错误次数过多，账号自动注销" << endl << "任意键退出";
621  		while (!_kbhit());
622  	}
623  }
624  // 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
625  // 调试程序: F5 或调试 >“开始调试”菜单
626  
627  // 入门使用技巧: 
628  //   1. 使用解决方案资源管理器窗口添加/管理文件
629  //   2. 使用团队资源管理器窗口连接到源代码管理
630  //   3. 使用输出窗口查看生成输出和其他消息
631  //   4. 使用错误列表窗口查看错误
632  //   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
633  //   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
634  
