#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>

#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define TRUE 1
#define FALSE 0

/* 타이머  */
#define CCHAR 0
#ifdef CTIME
#undef CTIME
#endif
#define CTIME 1

/* 테트리스 보드 높이, 넓이*/
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 21

/* 왼쪽, 오른쪽, 아래, 회전  */
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3

/* 블록 모양 */
#define I_BLOCK 0
#define	T_BLOCK 1
#define S_BLOCK 2
#define Z_BLOCK 3
#define L_BLOCK 4
#define J_BLOCK 5
#define O_BLOCK 6

#define X_OFFSET 6
#define Y_OFFSET 5
/* 게임 시작, 게임 종료 */
#define GAME_START 0
#define GAME_END 1

/*
 * 블록 모양(I, T, S, Z, L, J, O)
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 블록의 모양을 표시
 * 왼쪽, 오른쪽, 아래, 회전
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 4*4*4 배열의 3차원 배열
 */
char i_block[4][4][4] = {
	{  // 첫 번째 회전 상태
		{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
	{  // 두 번째 회전 상태
		{0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1} },
	{  // 세 번째 회전 상태
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1} },
	{  // 네 번째 회전 상태
		{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0} }
};
char t_block[4][4][4] = {
		{{1, 0, 0, 0},   {1, 1, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 1, 0},   {0, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{0, 0, 1, 0},   {0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
		{{0, 0, 0, 0},   {0, 1, 0, 0},   {1, 1, 1, 0},   {0, 0, 0, 0}}
};
char s_block[4][4][4] = {
		{{1, 0, 0, 0},   {1, 1, 0, 0},   {0, 1, 0, 0},   {0, 0, 0, 0}},
		{{0, 1, 1, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{0, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
		{{0, 0, 0, 0},   {0, 1, 1, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}}
};
char z_block[4][4][4] = {
		{{0, 1, 0, 0},   {1, 1, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{0, 0, 1, 0},   {0, 1, 1, 0},   {0, 1, 0, 0},   {0, 0, 0, 0}},
		{{0, 0, 0, 0},   {1, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 0, 0}}
};
char l_block[4][4][4] = {
		{{1, 0, 0, 0},   {1, 0, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 1, 0},   {1, 0, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
		{{0, 0, 0, 0},   {0, 0, 1, 0},   {1, 1, 1, 0},   {0, 0, 0, 0}}
};
char j_block[4][4][4] = {
		{{0, 1, 0, 0},   {0, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}},
		{{1, 0, 0, 0},   {1, 1, 1, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 0, 0},   {1, 0, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
		{{0, 0, 0, 0},   {0, 0, 0, 0},   {1, 1, 1, 0},   {0, 0, 1, 0}}
};
char o_block[4][4][4] = {
		{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
		{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}}
};
static struct result
{
	char name[30];
	long point;
	int year;
	int month;
	int day;
	int hour;
	int min;
	int rank;
	struct result* next;
}temp_result;
typedef struct result Result;
Result* head = NULL;

#if defined(_WIN32)
enum WIN_KEYCODE
{
	KEY_LEFT = 106,   // j
	KEY_RIGHT = 108,  // l
	KEY_DOWN = 107,   // k
	KEY_DROP = 97,  // a
	KEY_TURN = 105,    // i키 (방향회전)
	KEY_END = 112, //p

	KEY_LEFT2 = 75,   // ←
	KEY_RIGHT2 = 77,  // ↑
	KEY_DOWN2 = 80,   // →
	KEY_DROP2 = 32,  // 스페이스바
	KEY_TURN2 = 122,    // z
};
enum COLORCODE {
	D_BLUE = 1,	//1
	D_GREEN = 2,	//2
	D_RED = 4,  	//4
	D_PURPLE = 5,	//5
	BLUE = 9,		//9
	SKYBLUE = 11,	//11
	PURPLE = 13,		//13
	YELLOW = 14,		//14
	WHITE = 15		//15
};
#elif defined(__linux__)|| defined(__APPLE__)||defined(__MACH__)
enum input_KeyCode
{
	KEY_LEFT = 106,   // j
	KEY_RIGHT = 108,  // l
	KEY_DOWN = 107,   // k
	KEY_DROP = 97,  // a
	KEY_TURN = 105,    // i키 (방향회전)
	KEY_END = 113, //q

	KEY_LEFT2 = 68,   // ←
	KEY_RIGHT2 = 67,  // →
	KEY_DOWN2 = 66,   // ↓
	KEY_DROP2 = 32,  // 스페이스바
	KEY_TURN2 = 122,    // z
};
enum Color {
	D_BLUE = 34,
	D_GREEN = 32,
	D_RED = 31,
	D_PURPLE = 35,
	BLUE = 94,
	SKYBLUE = 96,
	PURPLE = 95,
	YELLOW = 93,
	WHITE = 97
};
#endif

int Block_Color[7] = { BLUE,D_PURPLE,D_GREEN,D_RED,SKYBLUE,D_BLUE,YELLOW };
char(*Block[7])[4][4] = { i_block, t_block, s_block, z_block, l_block, j_block ,o_block };

/* 테트리스 판을 2차원 배열로 표현
 * 2차원 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 20*8 배열
 * 모든 블록의 모양을 표시
 * 모든 블록의 모양을 표시*/

char Tetris_Table[21][10] = { 0 };
char Tetris_Color_Table[21][10] = { 0 };

/* 게임 종료 때마다
 * 이름과 득점수와
 * 날짜와 시간과 순위를 저장
 * */

int block_number = 0;  /*블록 번호*/
int next_block_number = 0; /*다음 블록 번호 */
int block_state = 0; /*블록 상태, 왼쪽, 오른쪽, 아래, 회전  */

//블럭 초기 생성 위치 x 8 y 1
int x = 8, y = 1; /*블록의 위치*/

int game = GAME_END; /*게임 시작, 게임 종료*/
long best_point = 10; /* 최고 점수*/
long point = 0; /* 현재 점수*/

/* 테트리스 화면 출력 관련 함수 */
void TetrisInit(); //초기 테트리스 보드 초기화
void printNextBlock(); // 오른쪽에 다음 블럭 시각적 표시

/* 테트리스 블럭 관련 함수 */
void BlockPrintControl(int x, int y); //블럭 출력 제어
void BlockPrint(int x, int y); //단순 블럭 출력
void BlockDelete(int x, int y); //이전 상태 블럭 지우기
int NextPositionCheck(int x, int y, int round);//다음 상태 이동 가능 체크
void FillTable(int x, int y); //이동 불가한 블럭 고정

void RemoveBlock(); //가로로 줄 완성시 제거
void RefreshTable();  //제거 하면 화면 새로고침
int HardDrop(int x, int y);
void ifHardDrop(int x, int y);

/* 프로그램 흐름 관련 함수 */
int display_menu(void); /* 메뉴 표시*/
int game_start(void);
void search_result(void);
void print_result(void);
void gameover(void);

/*결과값 리스트 관리 함수*/
Result* makeNewResult();
void InsertResult();

/*기타 운영체제 별로 따로 정의하기 위해 만든 함수*/
void delay(int ms); //프로그램 딜레이, ms단위
void ChangePosition(int x, int y);
void Hidefocus();
void ChangeColor(int color);
void clear();

#if defined(_WIN32)
void delay(int ms)
{
	Sleep(ms);
}
void ChangePosition(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Hidefocus() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void ChangeColor(int color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }
void clear() { system("cls"); }

#elif defined(__linux__)|| defined(__APPLE__)||defined(__MACH__)
int original_flags;
struct termios original_termios;

void setRawMode() {
	tcgetattr(STDIN_FILENO, &original_termios);
	struct termios raw = original_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
void resetMode() {
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

char _getch(void) {
	setRawMode();
	char ch = getchar();
	resetMode();
	return ch;
}
void setNonBlocking() {
	original_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, original_flags | O_NONBLOCK);
}
void resetBlocking() {
	fcntl(STDIN_FILENO, F_SETFL, original_flags);
}
int _kbhit(void) {
	setRawMode();
	setNonBlocking();

	int ch = getchar();

	resetMode();
	resetBlocking();

	if (ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}
void delay(int ms) { usleep(ms * 1000); }
void ChangePosition(int x, int y) {
	printf("\033[%d;%dH", y, x);
}
void Hidefocus() {
	printf("\033[?25l");
}
void ChangeColor(int color) {
	printf("\033[0;%dm", color);
}
void clear() { system("clear"); }
#endif

void TetrisInit()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 1; j < BOARD_HEIGHT - 1; j++) {
			Tetris_Table[i][j] = 0;
			Tetris_Color_Table[i][j] = 0;
		}
		Tetris_Table[i][0] = 1;
		Tetris_Table[i][BOARD_WIDTH - 1] = 1;

		Tetris_Color_Table[i][0] = 1;
		Tetris_Color_Table[i][BOARD_WIDTH - 1] = 1;
	}
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		Tetris_Table[BOARD_HEIGHT - 1][i] = 1;
		Tetris_Color_Table[BOARD_HEIGHT - 1][i] = 1;
	}
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (Tetris_Table[i][j] == 1) {
				ChangePosition(X_OFFSET + 2 * j, i + Y_OFFSET);
				printf("□");
			}
		}
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (i == 0 || i == 5 || j == 0 || j == 5)
				{
					ChangePosition(30 + 2 * j, i + Y_OFFSET);
					printf("□");
				}
			}
		}
	}
	ChangePosition(31, Y_OFFSET - 1); printf("Next Block");
	ChangePosition(31, 16); printf("  △\tI / Z : Rotation");
	ChangePosition(31, 17); printf("\t\tJ / L : Left / Right");
	ChangePosition(29, 18); printf("◁  ▣  ▷\tA / SPACE : Hard Drop");
	ChangePosition(31, 19); printf("\t\tK : Soft Drop");
	ChangePosition(31, 20); printf("  ▽\tQ: Exit");
}

void BlockPrintControl(int x, int y) {
	ChangeColor(Block_Color[block_number]);
	BlockPrint(x, y);
	ChangeColor(WHITE);
}

void BlockPrint(int x, int y) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Block[block_number][block_state % 4][i][j] == 1)
			{
				int nx = x + 2 * j;
				int ny = y + i;
				ChangePosition(nx + X_OFFSET, ny + Y_OFFSET);
				printf("■");
			}
		}
	}
}
void BlockDelete(int x, int y) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Block[block_number][block_state % 4][i][j] == 1)
			{
				int nx = x + 2 * j;
				int ny = y + i;
				ChangePosition(nx + X_OFFSET, ny + Y_OFFSET);
				printf("  ");
			}
		}
	}
}
int NextPositionCheck(int x, int y, int round)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			char blockVal = Block[block_number][(block_state + round) % 4][i][j];
			if (blockVal == 0) continue;
			int nx = (x / 2) + j;
			int ny = y + i;
			if (blockVal + Tetris_Table[ny][nx] == 2) return 0;
		}
	}
	return 1;
}

void FillTable(int x, int y) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Block[block_number][block_state % 4][i][j] == 1)
			{
				int nx = x / 2 + j;
				int ny = y + i;
				Tetris_Table[ny][nx] = 1;
				Tetris_Color_Table[ny][nx] = Block_Color[block_number];
			}
		}
	}
}
void RemoveBlock()
{
	for (int i = 0; i < BOARD_HEIGHT - 1; i++)
	{
		int cnt = 0;
		for (int j = 1; j < BOARD_WIDTH - 1; j++)
		{
			if (Tetris_Table[i][j]) cnt++;
		}
		if (cnt == BOARD_WIDTH - 2)
		{
			for (int k = i; k > 0; k--)
			{
				for (int j = 1; j < BOARD_WIDTH - 1; j++)
				{
					Tetris_Table[k][j] = Tetris_Table[k - 1][j];
					Tetris_Table[k - 1][j] = 0;

					Tetris_Color_Table[k][j] = Tetris_Color_Table[k - 1][j];
					Tetris_Color_Table[k - 1][j] = 0;
				}
			}
			for (int j = 1; j < BOARD_WIDTH - 1; j++)
			{
				Tetris_Table[0][j] = 0;
				Tetris_Color_Table[0][j] = 0;
			}
			point += 100;
			RefreshTable();
		}
	}
}
void RefreshTable()
{
	for (int i = 0; i < BOARD_HEIGHT - 1; i++)
	{
		for (int j = 1; j < BOARD_WIDTH - 1; j++)
		{
			ChangePosition(j * 2 + X_OFFSET, i + Y_OFFSET);
			if (Tetris_Color_Table[i][j] == 0)
			{
				printf("  ");
			}
			else if (Tetris_Color_Table[i][j] != 0)
			{
				ChangeColor(Tetris_Color_Table[i][j]);
				printf("■");
			}
		}
	}
	if (game == GAME_END)
	{
		ChangeColor(WHITE);
		for (int i = 0; i < BOARD_HEIGHT - 1; i++)
		{
			for (int j = 1; j < BOARD_WIDTH - 1; j++)
			{
				ChangePosition(j * 2 + X_OFFSET, i + Y_OFFSET);
				if (Tetris_Color_Table[i][j] != 0)
				{
					printf("▣"); delay(5);
				}
			}
		}
	}
}
int HardDrop(int x, int y) {
	for (int i = y; i < BOARD_HEIGHT - 1; i++)
	{
		if (!NextPositionCheck(x, i + 1, FALSE)) return i;
	}
	return x;
}
void ifHardDrop(int x, int y)
{
	ChangeColor(Block_Color[block_number]);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Block[block_number][block_state % 4][i][j] == 1)
			{
				int nx = x + 2 * j;
				int ny = y + i;
				ChangePosition(nx + X_OFFSET, ny + Y_OFFSET);
				printf("□");
			}
		}
	}
	ChangeColor(WHITE);
}
void printNextBlock() //32~40, 5~9에 다음 블럭 출력
{
	ChangeColor(Block_Color[next_block_number]);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			ChangePosition(32 + 2 * j, 6 + i);
			printf("  ");
			if (Block[next_block_number][1][i][j] == 1) {
				ChangePosition(32 + 2 * j, 6 + i);
				printf("■");
			}
		}
	}
	ChangeColor(WHITE);
}
/* 메뉴 표시*/
int display_menu(void)
{
	int menu = 0;
	while (1)
	{
		clear();
		ChangePosition(0, 4);
		ChangeColor(PURPLE);
		printf("  MMP\"\"MM\"\"YMM `7MM\"\"\"YMM  MMP\"\"MM\"\"YMM `7MM\"\"\"Mq.  `7MMF' .M\"\"\"bgd\n"); delay(50);
		printf("  P`   MM   `7   MM    `7  P'   MM   `7   MM   `MM.   MM   ,MI    \"Y \n"); delay(50);
		ChangeColor(D_BLUE);
		printf("       MM        MM   d         MM        MM   ,M9    MM   `MMb.     \n"); delay(50);
		printf("       MM        MMmmMM         MM        MMmmdM9     MM     `YMMNq. \n"); delay(50);
		ChangeColor(SKYBLUE);
		printf("       MM        MM   Y  ,      MM        MM  YM.     MM   .     `MM \n"); delay(50);
		printf("       MM        MM     ,M      MM        MM   `Mb.   MM   Mb     dM \n"); delay(50);
		printf("     .JMML.    .JMMmmmmMMM    .JMML.    .JMML. .JMM..JMML. P\"Ybmmd\"  \n"); delay(50);

		ChangeColor(WHITE);
		ChangePosition(56, 28);
		printf("Made by MSC");
		ChangePosition(26, 10);
		printf("\n\n\n\t\t\t     Text Tetris"); delay(50);
		printf("\n\t\t    ============================"); delay(50);
		printf("\n\t\t\t     GAME MENU\t\n"); delay(50);
		printf("\n\t\t    ============================"); delay(50);
		printf("\n\t\t       1) Game Start"); delay(50);
		printf("\n\t\t       2) Search history"); delay(50);
		printf("\n\t\t       3) Record Output"); delay(50);
		printf("\n\t\t       4) QUIT"); delay(50);
		printf("\n\t\t    ============================"); delay(50);
		printf("\n\t\t\t\t     SELECT : ");
		scanf("%d", &menu);
		while (getchar() != '\n'); //개행문자 비우기
		if (menu < 1 || menu > 4) { continue; }
		else { return menu; }
	}
	return 0;
}

int game_start(void)
{
	clear();
	TetrisInit();
	clock_t start = clock();
	srand((unsigned int)time(NULL));
	point = 0;
	block_number = rand() % 7;
	next_block_number = rand() % 7;
	int Harddropflag = 0;

	printNextBlock();
	ChangePosition(30, 12); printf("YOUR SCORE : %ld", point);
	ChangePosition(30, 14); printf("BEST SCORE : %ld", best_point > point ? best_point : point);

	while (1)
	{
		clock_t now = clock();
		if ((now - start) * 1000 / CLOCKS_PER_SEC >= CTIME * 500 || Harddropflag) //0.5초를 기준으로 아래로 내려옴.
		{
			if (NextPositionCheck(x, y + 1, FALSE)) {
				ChangePosition(X_OFFSET + 2, Y_OFFSET + 1);
				for (int i = 1; i < BOARD_WIDTH - 1; i++)printf("_ ");
				BlockDelete(x, y); y++;
				BlockPrintControl(x, y);
			}
			else
			{
				FillTable(x, y);
				RemoveBlock();
				block_number = next_block_number;
				next_block_number = rand() % 7;
				printNextBlock();
				ChangePosition(30, 12);
				printf("YOUR SCORE : %ld", point);
				ChangePosition(30, 14);
				printf("BEST SCORE : %ld", best_point > point ? best_point : point);
				block_state = 0;
				x = 8;
				y = 1;

				if (!NextPositionCheck(x, y, FALSE)) //게임 종료 조건문, 블럭을 생성함과 동시에 움직일 수 없으면 게임 종료
				{
					BlockPrintControl(x, y);
					game = GAME_END;
					if (point >= best_point)best_point = point;
					RefreshTable();
					gameover();
					break;
				}
				ifHardDrop(x, HardDrop(x, y));
				BlockPrintControl(x, y);
			}
			start = now;
			Harddropflag = 0;
		}
		if (_kbhit()) {
			int key = _getch();
			BlockDelete(x, y);
			BlockDelete(x, HardDrop(x, y));
			switch (key) {
			case KEY_LEFT:case KEY_LEFT2: if (NextPositionCheck(x - 2, y, FALSE)) x -= 2; break;
			case KEY_RIGHT:case KEY_RIGHT2: if (NextPositionCheck(x + 2, y, FALSE)) x += 2; break;
			case KEY_DOWN:case KEY_DOWN2: if (NextPositionCheck(x, y + 1, FALSE)) y++; break;
			case KEY_DROP:case KEY_DROP2: y = HardDrop(x, y); Harddropflag = 1; break;
			case KEY_TURN:case KEY_TURN2:if (NextPositionCheck(x, y, TRUE)) block_state++; break;
			case KEY_END: return 5;
			}
			ifHardDrop(x, HardDrop(x, y));
			BlockPrintControl(x, y);
		}
	}
	return 5;
}
void search_result() {
	char name[30];
	clear();
	ChangePosition(5, 3); printf("* ================[ INSERT SEARCH NAME ]================ *\n");
	ChangePosition(5, 5); printf("* ====================================================== *");
	ChangePosition(62, 4); printf("|");
	ChangePosition(5, 4); printf("|\tNAME : ");
	scanf("%s", name);


	clear();
	ChangePosition(5, 3);
	printf("* ===========[ TETRIS RECORD  SEARCH RESULT ]=========== *");
	ChangePosition(5, 4);
	printf("|  NAME		POINT			 DATE/TIME    |");
	ChangePosition(5, 5);
	printf("* ====================================================== *");
	int i = 1;
	Result* tmp = head;
	for (i = 1; i <= 10; tmp = tmp->next) {
		if (tmp == NULL) break;
		if (strcmp(tmp->name, name) == 0)
		{
			ChangePosition(5, 5 + i++);
			printf("|  %-14s\t%-6ld\t\t    %04d-%02d-%02d %02d:%02d  |", tmp->name, tmp->point, tmp->year, tmp->month, tmp->day, tmp->hour, tmp->min);
		}
	}
	ChangePosition(5, 5 + i++); printf("* ====================================================== *");
	ChangePosition(36, 7 + i);
	_getch();
	printf("Press ANY KEY TO CONTINUE...");
	_getch();

}
void print_result() {
	Result* tmp = head;
	clear();
	ChangePosition(5, 3);
	printf("* =================★ TETRIS  RANKING ★================= *");
	ChangePosition(5, 4);
	printf("|  RANK			NAME		      POINT    |");
	ChangePosition(5, 5);
	printf("* ======================================================= *");

	int i = 1;
	for (i = 1; i <= 10; tmp = tmp->next, i++) {
		if (tmp == NULL) break;
		ChangePosition(5, 5 + i);
		printf("|%4d\t%20s\t\t%11ld    |", tmp->rank, tmp->name, tmp->point);
	}
	ChangePosition(5, 6 + i);
	printf("* ======================================================= *");

	ChangePosition(36, 8 + i);
	_getch();
	printf("Press ANY KEY TO CONTINUE...");
	_getch();
}

void gameover()
{
	ChangePosition(30, 23); printf("+--[ INSERT YOUR NAME ]--+\n");
	ChangePosition(30, 25); printf("+------------------------+");
	ChangePosition(55, 24); printf("|");
	ChangePosition(30, 24); printf("| NAME : ");
	scanf("%s", temp_result.name);

	time_t t = time(NULL); //time_t로 현재 시간을 가져옴
	struct tm* now = localtime(&t);
	temp_result.year = now->tm_year + 1900;
	temp_result.month = now->tm_mon + 1;
	temp_result.day = now->tm_mday;
	temp_result.hour = now->tm_hour;
	temp_result.min = now->tm_min;
	temp_result.point = point;
	InsertResult();
}
Result* makeNewResult()
{
	Result* r = (Result*)malloc(sizeof(Result));
	if (!r) exit(1);
	memset(r, 0, sizeof(Result));
	strncpy(r->name, temp_result.name,sizeof(temp_result.name)-1);
	r->rank = 0;
	r->year = temp_result.year;
	r->month = temp_result.month;
	r->day = temp_result.day;
	r->hour = temp_result.hour;
	r->min = temp_result.min;
	r->point = temp_result.point;
	r->next = NULL;
	return r;
}

void InsertResult()
{
	Result* tmp = makeNewResult();
	if (head == NULL)
	{
		head = tmp;
	}
	else if (tmp->point >= head->point)
	{
		tmp->next = head;
		head = tmp;
	}
	else
	{
		Result* cnt = head;
		while (cnt->next != NULL && cnt->next->point >= tmp->point) {
			cnt = cnt->next;
		}
		tmp->next = cnt->next;
		cnt->next = tmp;
	}
	int rank = 1;
	tmp = head;
	while (tmp != NULL)
	{
		tmp->rank = rank++;
		tmp = tmp->next;
	}
}

/// 테트리스 게임 메인 함수
/// 메뉴를 표시하고 사용자의 선택에 따라 게임을 시작하거나 결과를 검색하거나 종료합니다.
/// @param
/// @return
int main(void)
{

	int menu = 1;
	Hidefocus();
	while (menu)
	{
		menu = display_menu();
		if (menu == 1)
		{
			game = GAME_START;
			menu = game_start();
		}
		else if (menu == 2)
		{
			search_result();
		}
		else if (menu == 3)
		{
			print_result();
		}
		else if (menu == 4)
		{
			exit(0);
		}
	}
	return 0;
}