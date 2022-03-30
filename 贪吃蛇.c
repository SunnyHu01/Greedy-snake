#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
static int grade = 0;
time_t time_start = 0;
time_t time_end = 0;
#define wide 60 
#define high 20
//////////游戏构思线路///////////
/*
1、设置地图范围，即边界，宽、高；
2、初始化蛇、初始化食物；
3、将蛇和食物显示在屏幕上；
4、蛇的移动（通过按键AWSD来控制）
	1）蛇碰到墙
	2）蛇碰到蛇的身体
	3）蛇碰到障碍物$
	4）蛇碰到食物*（身体增长一节，并获得相应的积分）
5、蛇死亡后，退出游戏，并计算出所得积分，和总计游戏时间；
*/
void get_ui()
{
	for (int i = 0; i < high; i++)
	{
		for (int j = 0; j < wide; j++)
		{
			printf("*");
		}
		printf("\n");
	}
}

void get_game_ui()
{
	for (int i = 0; i <= high + 1; i++)
	{
		if (i == 0 || i == high + 1)
			for (int j = 0; j <= wide + 1; j++)
				printf("*");
		else
			for (int j = 0; j <= wide + 1; j++)
				if (j == 0 || j == wide + 1)
					printf("*");
				else
					printf(" ");
		printf("\n");
	}
}

typedef struct _body
{
	int x;
	int y;
}BODY; //每一节的坐标

typedef struct snake
{
	BODY list[high*wide];  //最多有多少节
	int size;  //蛇身体的大小
	BODY food; //食物的坐标
	COORD coord;  //光标
	BODY shewei; //蛇尾
	BODY barry; //障碍物
	BODY barry_coord; //障碍物光标
}SNAKE;

void get_init_food(SNAKE *snake)
{
	srand((unsigned int)time(NULL));
	int x = rand() % wide + 1;  //1-60
	int y = rand() % high + 1;  //1-20
	snake->food.x = x;
	snake->food.y = y;
	for (int i = 0; i < snake->size; i++)  //避免食物出现在蛇身上；
	{
		if (snake->food.x == snake->list[i].x && snake->food.y == snake->list[i].y)
		{
			get_init_food(snake);
		}
	}
}

void get_init_barry(SNAKE *snake)
{
	srand(rand());
	int x = rand() % wide + 1;  //1-60
	int y = rand() % high + 1;  //1-20
	snake->barry.x = x;
	snake->barry.y = y;
	for (int i = 0; i < snake->size; i++)  
	{
		if ((snake->barry.x == snake->list[i].x && snake->barry.y == snake->list[i].y) || (snake->food.x == snake->barry.x && snake->food.y == snake->barry.y))
		{
			get_init_barry(snake);
		}
	}
}

void get_init_snake(SNAKE *snake)
{
	snake->list[0].x = wide / 2;   //初始化蛇头；
	snake->list[0].y = high / 2;
	snake->list[1].x = wide / 2 - 1;   //初始化蛇尾；
	snake->list[1].y = high / 2;
	snake->size = 2;
	get_init_food(snake);  //初始化食物坐标；
	get_init_barry(snake); //初始化障碍物;
}

void get_show_ui(SNAKE *snake)
{
	/////显示蛇/////每次显示蛇或食物时，都要设置光标位置；	
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;  //设置光标位置
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
		if (i == 0)
		{
			printf("@");
		}
		else
			printf("#");
	}
	/////显示食物//////
	snake->coord.X = snake->food.x;  //设置光标位置
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
	printf("*");
	/////显示障碍物//////
	snake->coord.X = snake->barry.x;  //设置光标位置
	snake->coord.Y = snake->barry.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
	printf("$");
	/////将原来障碍物的位置显示为空格///////
	snake->coord.X = snake->barry_coord.x;  //设置光标位置
	snake->coord.Y = snake->barry_coord.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
	printf(" ");
	/////显示尾巴，将原来尾巴位置显示为空格///////
	snake->coord.X = snake->shewei.x;  //设置光标位置
	snake->coord.Y = snake->shewei.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
	printf(" ");	
}

void get_move(SNAKE *snake)
{
	snake->shewei.x = snake->list[snake->size - 1].x; //记住更新前的蛇尾坐标
	snake->shewei.y = snake->list[snake->size - 1].y;

	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];  //更新蛇身坐标；
	}
}

void get_snake_move(SNAKE *snake, char key)
{	
	///按键若是按错，即按A、S、W、D之外的键，一律按D建处理，即向右移动；
	switch (key)
	{
	case 'a':
		get_move(snake);  //更新蛇身坐标；
		snake->list[0].x -= 1;  //更新蛇头坐标；
		break;
	case 'd':
		get_move(snake);  //更新蛇身坐标；
		snake->list[0].x += 1;
		break;
	case 'w':
		get_move(snake);  //更新蛇身坐标；
		snake->list[0].y -= 1;
		break;
	case 's':
		get_move(snake);  //更新蛇身坐标；
		snake->list[0].y += 1;
		break;
	default:      
		get_move(snake);  //更新蛇身坐标；
		snake->list[0].x += 1;
		break;
	}
}

void get_game_over_one(SNAKE *snake)
{
	while (snake->list[0].y == 0 || snake->list[0].x == 0 || snake->list[0].y == high+1 || snake->list[0].x == wide+1)
	{
		time_end = time(0);
		snake->coord.X = 0;  //设置光标位置
		snake->coord.Y = high+2;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
		printf("碰边界墙，游戏结束，获得积分：%d分\n",grade);
		printf("游戏总共用时%ds\n", time_end - time_start);
		exit(0);		
	}
}

void get_game_over_two(SNAKE *snake)
{
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x && snake->list[0].y == snake->list[i].y)
		{
			time_end = time(0);
			snake->coord.X = 0;  //设置光标位置
			snake->coord.Y = high + 2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
			printf("碰自身体，游戏结束，获得积分：%d分\n",grade);
			printf("游戏总共用时%ds\n", time_end - time_start);
			exit(0);
		}
	}
}

void get_game_over_three(SNAKE *snake)
{
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->barry.x && snake->list[0].y == snake->barry.y)
		{
			time_end = time(0);
			snake->coord.X = 0;  //设置光标位置
			snake->coord.Y = high + 2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //设置光标的函数；
			printf("碰障碍物，游戏结束，获得积分：%d分\n", grade);
			printf("游戏总共用时%ds\n", time_end - time_start);
			exit(0);
		}
	}
}

void get_game_win(SNAKE *snake)
{
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		if (grade < 100) //分等级，完成10次后，每吃一次，加20分；
			grade += 10;
		else
			grade += 20;
		snake->size = snake->size + 1;
		get_init_food(snake);
		snake->barry_coord.x = snake->barry.x; //记住更新前的障碍物的坐标
		snake->barry_coord.y = snake->barry.y;
		get_init_barry(snake);
	}
}

void get_start_game(SNAKE *snake)
{
	/////设计方案/////
	/*
	1、控制蛇的方向；
	2、更新蛇的坐标；
	3、蛇移动；
	4、蛇是否碰到墙；
	5、蛇是否碰到自己；
	6、蛇是否吃到食物（碰到食物，增长一节）;
	*/
	char key = 'd';  //游戏开始，蛇头向右移动；
	while (1)
	{
		while (_kbhit())  //判断按键是否按下；
		{
			key = _getch();
		}
		get_snake_move(snake, key);  //更新蛇的坐标；
		//system("cls");
		get_show_ui(snake);  //间隔0.5s显示蛇的移动；
		get_game_win(snake);  //碰食物
		get_game_over_one(snake);  //碰墙
		get_game_over_two(snake);  //碰自己	
		get_game_over_three(snake);  //碰障碍物	
		Sleep(180);  //延迟**秒;500为0.5s
	}
}

void hide_guangbiao()
{
	//隐藏控制台光标//
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int main()
{
	hide_guangbiao(); //用来隐藏光标的，否则太影响美观；
	//get_ui(); //打印地图，以做调整；
	SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));  //因为是指向地址，所以传参时不需要加地址符&了；
	get_game_ui();
	get_init_snake(snake);  //初始化蛇
	get_show_ui(snake);	//显示蛇
	time_start = time(0);
	get_start_game(snake);  //开始游戏
	while (1);
	system("pause");
	return 0;
}