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
//////////��Ϸ��˼��·///////////
/*
1�����õ�ͼ��Χ�����߽磬���ߣ�
2����ʼ���ߡ���ʼ��ʳ�
3�����ߺ�ʳ����ʾ����Ļ�ϣ�
4���ߵ��ƶ���ͨ������AWSD�����ƣ�
	1��������ǽ
	2���������ߵ�����
	3���������ϰ���$
	4��������ʳ��*����������һ�ڣ��������Ӧ�Ļ��֣�
5�����������˳���Ϸ������������û��֣����ܼ���Ϸʱ�䣻
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
}BODY; //ÿһ�ڵ�����

typedef struct snake
{
	BODY list[high*wide];  //����ж��ٽ�
	int size;  //������Ĵ�С
	BODY food; //ʳ�������
	COORD coord;  //���
	BODY shewei; //��β
	BODY barry; //�ϰ���
	BODY barry_coord; //�ϰ�����
}SNAKE;

void get_init_food(SNAKE *snake)
{
	srand((unsigned int)time(NULL));
	int x = rand() % wide + 1;  //1-60
	int y = rand() % high + 1;  //1-20
	snake->food.x = x;
	snake->food.y = y;
	for (int i = 0; i < snake->size; i++)  //����ʳ������������ϣ�
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
	snake->list[0].x = wide / 2;   //��ʼ����ͷ��
	snake->list[0].y = high / 2;
	snake->list[1].x = wide / 2 - 1;   //��ʼ����β��
	snake->list[1].y = high / 2;
	snake->size = 2;
	get_init_food(snake);  //��ʼ��ʳ�����ꣻ
	get_init_barry(snake); //��ʼ���ϰ���;
}

void get_show_ui(SNAKE *snake)
{
	/////��ʾ��/////ÿ����ʾ�߻�ʳ��ʱ����Ҫ���ù��λ�ã�	
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;  //���ù��λ��
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
		if (i == 0)
		{
			printf("@");
		}
		else
			printf("#");
	}
	/////��ʾʳ��//////
	snake->coord.X = snake->food.x;  //���ù��λ��
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
	printf("*");
	/////��ʾ�ϰ���//////
	snake->coord.X = snake->barry.x;  //���ù��λ��
	snake->coord.Y = snake->barry.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
	printf("$");
	/////��ԭ���ϰ����λ����ʾΪ�ո�///////
	snake->coord.X = snake->barry_coord.x;  //���ù��λ��
	snake->coord.Y = snake->barry_coord.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
	printf(" ");
	/////��ʾβ�ͣ���ԭ��β��λ����ʾΪ�ո�///////
	snake->coord.X = snake->shewei.x;  //���ù��λ��
	snake->coord.Y = snake->shewei.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
	printf(" ");	
}

void get_move(SNAKE *snake)
{
	snake->shewei.x = snake->list[snake->size - 1].x; //��ס����ǰ����β����
	snake->shewei.y = snake->list[snake->size - 1].y;

	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];  //�����������ꣻ
	}
}

void get_snake_move(SNAKE *snake, char key)
{	
	///�������ǰ�������A��S��W��D֮��ļ���һ�ɰ�D�������������ƶ���
	switch (key)
	{
	case 'a':
		get_move(snake);  //�����������ꣻ
		snake->list[0].x -= 1;  //������ͷ���ꣻ
		break;
	case 'd':
		get_move(snake);  //�����������ꣻ
		snake->list[0].x += 1;
		break;
	case 'w':
		get_move(snake);  //�����������ꣻ
		snake->list[0].y -= 1;
		break;
	case 's':
		get_move(snake);  //�����������ꣻ
		snake->list[0].y += 1;
		break;
	default:      
		get_move(snake);  //�����������ꣻ
		snake->list[0].x += 1;
		break;
	}
}

void get_game_over_one(SNAKE *snake)
{
	while (snake->list[0].y == 0 || snake->list[0].x == 0 || snake->list[0].y == high+1 || snake->list[0].x == wide+1)
	{
		time_end = time(0);
		snake->coord.X = 0;  //���ù��λ��
		snake->coord.Y = high+2;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
		printf("���߽�ǽ����Ϸ��������û��֣�%d��\n",grade);
		printf("��Ϸ�ܹ���ʱ%ds\n", time_end - time_start);
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
			snake->coord.X = 0;  //���ù��λ��
			snake->coord.Y = high + 2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
			printf("�������壬��Ϸ��������û��֣�%d��\n",grade);
			printf("��Ϸ�ܹ���ʱ%ds\n", time_end - time_start);
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
			snake->coord.X = 0;  //���ù��λ��
			snake->coord.Y = high + 2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);  //���ù��ĺ�����
			printf("���ϰ����Ϸ��������û��֣�%d��\n", grade);
			printf("��Ϸ�ܹ���ʱ%ds\n", time_end - time_start);
			exit(0);
		}
	}
}

void get_game_win(SNAKE *snake)
{
	if (snake->list[0].x == snake->food.x && snake->list[0].y == snake->food.y)
	{
		if (grade < 100) //�ֵȼ������10�κ�ÿ��һ�Σ���20�֣�
			grade += 10;
		else
			grade += 20;
		snake->size = snake->size + 1;
		get_init_food(snake);
		snake->barry_coord.x = snake->barry.x; //��ס����ǰ���ϰ��������
		snake->barry_coord.y = snake->barry.y;
		get_init_barry(snake);
	}
}

void get_start_game(SNAKE *snake)
{
	/////��Ʒ���/////
	/*
	1�������ߵķ���
	2�������ߵ����ꣻ
	3�����ƶ���
	4�����Ƿ�����ǽ��
	5�����Ƿ������Լ���
	6�����Ƿ�Ե�ʳ�����ʳ�����һ�ڣ�;
	*/
	char key = 'd';  //��Ϸ��ʼ����ͷ�����ƶ���
	while (1)
	{
		while (_kbhit())  //�жϰ����Ƿ��£�
		{
			key = _getch();
		}
		get_snake_move(snake, key);  //�����ߵ����ꣻ
		//system("cls");
		get_show_ui(snake);  //���0.5s��ʾ�ߵ��ƶ���
		get_game_win(snake);  //��ʳ��
		get_game_over_one(snake);  //��ǽ
		get_game_over_two(snake);  //���Լ�	
		get_game_over_three(snake);  //���ϰ���	
		Sleep(180);  //�ӳ�**��;500Ϊ0.5s
	}
}

void hide_guangbiao()
{
	//���ؿ���̨���//
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int main()
{
	hide_guangbiao(); //�������ع��ģ�����̫Ӱ�����ۣ�
	//get_ui(); //��ӡ��ͼ������������
	SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));  //��Ϊ��ָ���ַ�����Դ���ʱ����Ҫ�ӵ�ַ��&�ˣ�
	get_game_ui();
	get_init_snake(snake);  //��ʼ����
	get_show_ui(snake);	//��ʾ��
	time_start = time(0);
	get_start_game(snake);  //��ʼ��Ϸ
	while (1);
	system("pause");
	return 0;
}