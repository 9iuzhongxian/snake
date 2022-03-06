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

typedef struct Snake {//�ߵĶ���
	size_t x;
	size_t y;
	struct Snake* next;
}Snake,*pSnake;
enum Direction {//�������
	U,
	D,
	L,
	R
}Direction;

enum State {//״̬����
	ERROR_BREAKE,
	ERROR_EATSELF,
	NORMAL,
	SUCCESS
}State;

pSnake Head = NULL;
pSnake Food = NULL;

const char food = '0';
const char snake = '0';



void Pos(int x, int y) {//��꺯��
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Face() {//���溯��
	system("cls");
	Pos(50, 15);
	std::cout <<"��ʼ��Ϸ";
}

void Map() {//��ͼ����
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

void PrintSnake() {//����
	pSnake Node = Head;
	while (Node) {
		Pos(Node->x, Node->y);
		std::cout << snake;
		Node = Node->next;
		//Pos(ROWMAP, COLMAP);
	}
}

void InitSnake() {//������Ϊ5����ʼ��
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

void CreateFood() {//�������һ��ʳ��
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

int IsFood(pSnake NewHead) {//�Ե�ʳ��ĺ���
	
	if (NewHead->x == Food->x && NewHead->y == Food->y) {
		
		free(Food);
		Food = NULL;
		return 1;
	}
	return 0;


}

int CrossWall(pSnake NewHead) {//ײǽ����
	if (NewHead->x <= 0 || NewHead->x >= COLMAP || NewHead->y <= 0 || NewHead->y >= ROWMAP) {
		State = ERROR_BREAKE;
		return 1;
	}
	return 0;
}

int EatSelf(pSnake NewHead) {//�Ե��Լ��ĺ���
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





void SnakeMove() {//���ƶ��ķ���
	pSnake NewHead = NULL;
	NewHead = (pSnake)malloc(sizeof(Snake));
	switch (Direction) {//�ж����ߵķ���
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
		if (IsFood(NewHead)) {//�жϳԵ�ʳ��û���Ե��˾����´�ӡ�ߣ���һ���µ�ʳ����ӷ֣��������ͨ�ط������˳���Ϸ
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
		else if ((!CrossWall(NewHead)) && (!EatSelf(NewHead))) {//�ж��Ƿ�ײǽ���߳Ե��Լ�
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
			free(cur->next);//�ͷ����һ���ڵ㣬ԭ���ķ��ſ�����Ҫ�ÿհ׸���
			cur->next = NULL;
			PrintSnake();
		}
	}

void Pause() {//��ͣ����
	while (1) {
		Sleep(Sleeptime);
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
}

void ControlSnake() {//��������
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

void State_Judge() {//״̬�жϺ���
	if (State == ERROR_BREAKE) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "�����ײ��ǽ��";
	}
	else if (State == ERROR_EATSELF) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "����߳Ե����Լ�";
		
	}
	else if (State == SUCCESS) {
		system("cls");
		Pos(COLMAP / 2, ROWMAP / 2);
		std::cout << "��ϲ����";
	}
}


void StartGame() {//��ʼ��Ϸ
	Face();
	system("pause");
	if (GetAsyncKeyState(VK_RETURN)) {
		system("cls");
		Pos(COLMAP + 3, 1);
		std::cout << "��ǰ��/ͨ�ط�:";
		Pos(COLMAP + 20, 1);
		std::cout << Score << '/' << SUCCESS_SCORE;
		Pos(COLMAP + 3, ROWMAP/2);
		std::cout << "ÿ�Ե�һ��ʳ��÷�:";
		Pos(COLMAP + 22, ROWMAP/2);
		std::cout << Everyscore;
		Pos(COLMAP + 3, ROWMAP-10);
		std::cout << "��M�����٣���L������" << std::endl;
		Pos(COLMAP + 3, ROWMAP - 9);
		std::cout<<"���ո����ͣ";
		Pos(COLMAP + 3, ROWMAP - 5);
		std::cout << "��Esc���˳���Ϸ";

		Map();
		InitSnake();
		CreateFood();
	}
	else if (GetAsyncKeyState(VK_ESCAPE)) {
		system("cls");
		exit(0);
	}


}

void RunGame() {//������Ϸ
	Direction = R;
	State = NORMAL;
	while (1) {
		ControlSnake();
		SnakeMove();
		if (State != NORMAL) {
			State_Judge();
			Sleep(5000);//��������ʾͨ�ػ�����������״̬ͣ��ʱ��
			break;
		}
		Sleep(Sleeptime);
	}
}

void EndGame() {//������Ϸ������ߣ����ʳ��޸ķ���
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



int main() {//������
	
	while (1) {
		StartGame();
		RunGame();
		EndGame();
	}
	//system("pause");

	return 0;
}