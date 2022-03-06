#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
const int ROWMAP=29;
const int  COLMAP=90;
int Sleeptime = 200;
int Score = 0;
int Everyscore = 1;
const int SUCCESS_SCORE=10; 

typedef struct Snake {//蛇的定义
	size_t x;
	size_t y;
	struct Snake* next;
}Snake,*pSnake;
enum Direction {//方向变量
	U,
	D,
	L,
	R
}Direction;

enum State {//状态变量
	ERROR_BREAKE,
	ERROR_EATSELF,
	NORMAL,
	SUCCESS
}State;

pSnake Head = NULL;
pSnake Food = NULL;

const char food = '0';
const char snake = '0';



void Pos(int x, int y) {//光标函数
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Face() {//界面函数
	system("cls");
	Pos(50, 15);
	std::cout <<"开始游戏";
}

void Map() {//地图函数
	int i = 0;
	for (i = 0; i <= ROWMAP; i ++) {
		Pos(0, i);
		std::cout << "0";
		Pos(COLMAP,i );
		std::cout << "0";

	}
	for (i = 0; i <=COLMAP; i++) {
		Pos(i, 0);
		std::cout << "0";
		Pos(i, ROWMAP);
		std::cout << "0";
	}
	
}

void PrintSnake() {//画蛇
	pSnake Node = Head;
	while (Node) {
		Pos(Node->x, Node->y);
		std::cout << snake;
		Node = Node->next;
		//Pos(ROWMAP, COLMAP);
	}
}

void InitSnake() {//蛇身长度为5，初始化
	int num = 5;
	int i = 1;
	pSnake Current;
	Head = (pSnake)malloc(sizeof(Snake));
	Head ->x = 10;
	Head->y = 10;
	Head->next = NULL;
	Current = (pSnake)malloc(sizeof(Snake));
	Current = Head;
	for (i = 1; i < num; i++) {
		pSnake NewNode = (pSnake)malloc(sizeof(Snake));
		NewNode->x = 10 - i;
		NewNode->y = 10;
		NewNode -> next = NULL;
		Current->next = NewNode;
		Current = Current->next;

	}
	PrintSnake();
}

void CreateFood() {//随机创造一个食物
	if (Food == NULL) {
		pSnake Current = Head;
		Food = (pSnake)malloc(sizeof(Snake));
		srand((unsigned int)time(NULL));
		Food->next = NULL;
		Food->x = (rand() % (COLMAP - 1)) + 1;
		Food->y = (rand() % (ROWMAP - 1)) + 1;
		while (Current) {
			if (Current->x == Food->x && Current->y == Food->y) {
				free(Food);
				Food = NULL;
				CreateFood();
				return;

			}
			Current = Current->next;

		}
		Pos(Food->x, Food->y);
		std::cout << food;
	}
}

int IsFood(pSnake NewHead) {//吃到食物的函数
	
	if (NewHead->x == Food->x && NewHead->y == Food->y) {
		
		free(Food);
		Food = NULL;
		return 1;
	}
	return 0;


}

int CrossWall(pSnake NewHead) {//撞墙函数
	if (NewHead->x <= 0 || NewHead->x >= COLMAP || NewHead->y <= 0 || NewHead->y >= ROWMAP) {
		State = ERROR_BREAKE;
		return 1;
	}
	return 0;
}

int EatSelf(pSnake NewHead) {//吃到自己的函数
	pSnake cur = Head;
	while (cur) {
		if (cur->x == NewHead->x && cur->y == NewHead->y) {
			State = ERROR_EATSELF;
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}





void SnakeMove() {//蛇移动的方法
	pSnake NewHead = NULL;
	NewHead = (pSnake)malloc(sizeof(Snake));
	switch (Direction) {//判断蛇走的方向
	case U:NewHead->x = Head->x;
		NewHead->y = (Head->y - 1);
		NewHead->next = Head;
		break;
	case D:NewHead->x = Head->x;
		NewHead->y = (Head->y + 1);
		NewHead->next = Head;
		break;
	case R:NewHead->x =( Head->x + 1);
		NewHead->y = Head->y;
		NewHead->next = Head;
		break;
	case L:NewHead->x = (Head->x - 1);
		NewHead->y = Head->y;
		NewHead->next = Head;
		break;
	}
		if (IsFood(NewHead)) {//判断吃到食物没，吃到了就重新打印蛇，给一个新的食物，并加分，如果超过通关分数则退出游戏
			Head = NewHead;
			PrintSnake();
			CreateFood();
			Score += Everyscore;
			Pos(COLMAP + 20, 1);
			std::cout << Score << '/' << SUCCESS_SCORE;
			if (Score >= SUCCESS_SCORE) {
				State = SUCCESS;
			}

		}
		else if ((!CrossWall(NewHead)) && (!EatSelf(NewHead))) {//判断是否撞墙或者吃到自己
			Head = NewHead;
			pSnake cur = NULL;
			cur = (pSnake)malloc(sizeof(Snake));
			cur = Head;
			while (cur->next->next != NULL) {
				cur = cur->next;
			}
			//Pos(cur->x, cur->y);
			//std::cout << snake;
			Pos(cur->next->x, cur->next->y);
			std::cout << ' ';
			free(cur->next);//释放最后一个节点，原来的符号可能需要用空白覆盖
			cur->next = NULL;
			PrintSnake();
		}
	}

void Pause() {//暂停函数
	while (1) {
		Sleep(Sleeptime);
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
}

void ControlSnake() {//按键控制
	if (GetAsyncKeyState(VK_UP) && Direction != D) {
		Direction = U;
	}
	else if (GetAsyncKeyState(VK_DOWN) && Direction != U) {
		Direction = D;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && Direction != L) {
		Direction = R;
	}
	else if (GetAsyncKeyState(VK_LEFT) && Direction != R) {
		Direction = L;
	}
	else if (GetAsyncKeyState(VK_SPACE)) {
		Pause();
	}
	else if (GetAsyncKeyState(VK_ESCAPE)) {
		system("cls");
		exit(0);
	}
	else if (GetAsyncKeyState('L')) {
		if (Sleeptime > 50) {
			Sleeptime = Sleeptime - 50;
		}
	}
	else if (GetAsyncKeyState('M')) {
		if (Sleeptime < 500) {
			Sleeptime = Sleeptime + 50;
		}
	}

}

void State_Judge() {//状态判断函数
	if (State == ERROR_BREAKE) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "你的蛇撞到墙了";
	}
	else if (State == ERROR_EATSELF) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "你的蛇吃到了自己";
		
	}
	else if (State == SUCCESS) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "恭喜过关";
	}
}


void StartGame() {//开始游戏
	Face();
	system("pause");
	if (GetAsyncKeyState(VK_RETURN)) {
		system("cls");
		Pos(COLMAP + 3, 1);
		std::cout << "当前分/通关分:";
		Pos(COLMAP + 20, 1);
		std::cout << Score << '/' << SUCCESS_SCORE;
		Pos(COLMAP + 3, ROWMAP/2);
		std::cout << "每吃到一个食物得分:";
		Pos(COLMAP + 22, ROWMAP/2);
		std::cout << Everyscore;
		Pos(COLMAP + 3, ROWMAP-10);
		std::cout << "按M键减速，按L键加速" << std::endl;
		Pos(COLMAP + 3, ROWMAP - 9);
		std::cout<<"，空格键暂停";
		Pos(COLMAP + 3, ROWMAP - 5);
		std::cout << "按Esc键退出游戏";

		Map();
		InitSnake();
		CreateFood();
	}
	else if (GetAsyncKeyState(VK_ESCAPE)) {
		system("cls");
		exit(0);
	}


}

void RunGame() {//运行游戏
	Direction = R;
	State = NORMAL;
	while (1) {
		ControlSnake();
		SnakeMove();
		if (State != NORMAL) {
			State_Judge();
			Sleep(5000);//结束后显示通关或者蛇死掉的状态停留时间
			break;
		}
		Sleep(Sleeptime);
	}
}

void EndGame() {//结束游戏，清空蛇，清空食物，修改分数
	pSnake Cur = Head;
	while (Cur) {
		pSnake Delete = Cur;
		Cur = Cur->next;
		free(Delete);
		Delete = NULL;
	}
	if (Food != NULL) {
		free(Food);
		Food = NULL;
	}
	Score = 0;
	Everyscore = 1;
	Sleeptime = 200;
}



int main() {//主函数
	
	while (1) {
		StartGame();
		RunGame();
		EndGame();
	}
	//system("pause");

	return 0;
}