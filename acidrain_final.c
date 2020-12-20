/*
 * 2020 System Programming Team Project 9Team "acid rain game"
 * 팀원: 강희철, 강성진, 전우진, 박종민
 * 외부 사용 및 참조 코드: 출처 - https://janghw.tistory.com/entry/%EC%8B%9C%EC%8A%A4%ED%85%9C-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D-%ED%8C%80%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8-4
   외부 사용 및 참조 코드: 사용내용 - 해당 출처의 addQueue, findWord, makePlusOne 함수를 참조하였습니다. 그 외 UI및 함수는 저희가 만든 기능에 맞게 만들었습니다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define ERASE "                                                        "
#define WORD_SIZ 20

typedef struct node node;
typedef struct node //화면에 떨어지는 비
{
	int row;
	int col;
	char str[WORD_SIZ];
	node* link;
}node;

typedef struct namescore //게임 종료후 score.txt에 점수와 이름을 기록하기 위한 구조체
{
	int data;
	char sname[10];
}namescore;

//게임 UI에 보이는 변수들
int hp = 100;
char hpText[3];
int score = 0;
char scoreText[5];
int level = 1;
char levelText[2];

//텍스트 파일에서 불러오는 정보들
char name[10];
char* word_data[WORD_SIZ];
char* word_data_hard[WORD_SIZ];
int word_number;
int hard_word_number;

node* ptr = NULL;
int timer = 2000000;
int length = 0;
char enterText[20];

void draw(int, int, char*); //인자로 받은 행과 열로 가서 문자열을 보이게하는 함수
void eraseScreen(); //필요한 UI를 제외한 나머지를 지우는 함수
void init(); //초기 UI
void startGame();
void setDB(int); //txt파일에서 단어 정보들을 가져오는 함수
char* returnWord();
//화면에서 단어들이 한줄씩 떨어지게 만드는 함수(showWord, addQueue, makePlusOne)
void* showWord();
void addQueue(char*, int);
void makePlusOne();
node* makeNode();
void findWord(char*);
void* levelUp();
void showScore(); //이 때까지 기록한 점수중 10등까지 보여주는 함수
void gameOver(); 
void saveScore();
int main()
{
	char option, option_w;
	
	signal(SIGINT, SIG_IGN);
	initscr();
	clear();
	curs_set(0);
	init();
	draw(4,19, "ACID RAIN GAME");
	draw(12,16, "1. Start Game");
	draw(14,16, "2. Show Score Record");
	draw(16,16, "3. Exit");
	move(LINES-1, 0);
	refresh();
	
	option = getch();
	switch(option)
	{
		case '1':
			eraseScreen();
			draw(12, 16, "1. English");
			move(LINES-1, 0);
			refresh();

			option_w = getch();
			switch(option_w)
			{
				case '1':
					setDB(1);
					eraseScreen();
					draw(11, 16, "Enter your name: ");
					refresh();
					scanw("%s", name);
					break;
			}
			srand(time(NULL));
			startGame();
			break;
		case '2':
			showScore();
			endwin();
			exit(0);
			break;
		case '3':
			endwin();
			exit(0);
			break;
	}
	
	gameOver();
	endwin();
}

void draw(int row, int col, char* str)
{
	move(row,1);
	addstr(ERASE);
	move(row,col);
	addstr(str);
	refresh();
}

void eraseScreen()
{
	clear();
	init();
	refresh();
}

void init()
{
	draw(0,0, " --------------------------------------------------------");
    draw(1,0, "|                                                        |"); //56
    draw(2,0, "|                                                        |");
    draw(3,0, "|                                                        |");
    draw(4,0, "|                                                        |");
    draw(5,0, "|                                                        |");
    draw(6,0, "|                                                        |");
    draw(7,0, "|                                                        |");
    draw(8,0, "|                                                        |");
    draw(9,0, "|                                                        |");
    draw(10,0, "|                                                        |");
    draw(11,0, "|                                                        |");
    draw(12,0, "|                                                        |");
    draw(13,0, "|                                                        |");
    draw(14,0, "|                                                        |");
    draw(15,0, "|                                                        |");
    draw(16,0, "|                                                        |");
    draw(17,0, "|                                                        |");
    draw(18,0, "|                                                        |");
    draw(19,0, "|                                                        |");
    draw(20,0, "|                                                        |");
    draw(21,0, "|                                                        |");
    draw(22,0, "|                                                        |");
    draw(23,0, " --------------------------------------------------------");
}

void setDB(int opt) //bring words from txt file when program starts
{
	FILE* fp, *fph;
        char word[WORD_SIZ];
        int i = 0, j = 0;

	if(opt == 1)
	{
		fp = fopen("words_eng.txt", "r");
		fph = fopen("words_eng_hard.txt", "r");
	}
	/*else
	{
		fp = fopen("words_kor.txt", "r");
		fph = fopen("words_kor_hard.txt", "r");
	}*/
        while(fgets(word, WORD_SIZ, fp))
        {
                word_data[i] = (char*)malloc(sizeof(char*));
                strcpy(word_data[i], word);
		if(word_data[i][strlen(word_data[i]) -1] == '\n')
				word_data[i][strlen(word_data[i]) -1] = '\0';
                i++;
        }

	while(fgets(word, WORD_SIZ, fph))
        {
                word_data_hard[j] = (char*)malloc(sizeof(char*));
                strcpy(word_data_hard[j], word);
		if(word_data_hard[j][strlen(word_data_hard[j]) -1] == '\n')
				word_data_hard[j][strlen(word_data_hard[j]) -1] = '\0';
                j++;
        }
	word_number = --i;
	hard_word_number = --j;
	fclose(fp);
	fclose(fph);
}

char* returnWord()
{
	int randnum;
	randnum = rand() % (word_number + 1);

	if(level >= 6)
		return word_data_hard[randnum];
	return word_data[randnum];
}

void* showWord()
{
	int col, wordlen;
	char* word;

	while(hp > 0)
	{
		node* temp = NULL;
		word = returnWord();
		wordlen = strlen(word);
		while((col = (rand() % 54) + 1) + wordlen > 56);
		addQueue(word, col);
		temp = ptr;

		while(temp)
		{
			draw(temp->row, temp->col, temp->str);
			temp = temp->link;
		}
		move(24, 8);
		usleep(timer);
	}
}

void addQueue(char* str, int col)
{
	node* temp = NULL;
	node* temp2 = NULL;

	if(ptr == NULL)
	{
		ptr = makeNode();
		strcpy(ptr->str, str);
		ptr->row = 1;
		ptr->col = col;
	}
	else
	{
		temp = makeNode();
		strcpy(temp->str, str);
		temp->row = 1;
		temp->col = col;
		temp->link = ptr;
		ptr = temp;
		makePlusOne();
	}

	length++;
	if(length > 22)
	{
		while(temp->link)
		{
			temp2 = temp;
			temp = temp->link;
		}
		hp -= strlen(temp->str);
		sprintf(hpText, "%d", hp);
		move(24,30); addstr("   ");
		move(24,30); addstr(hpText);
		free(temp);
		temp2->link = NULL;
		length--;
	}
}

void makePlusOne()
{
	node* temp = ptr->link;

	while(temp)
	{
		temp->row += 1;
		temp = temp->link;
	}
}

node*  makeNode()
{
	node* temp;

	temp = (node*)malloc(sizeof(node));
	temp->link = NULL;

}

void findWord(char* str)
{
	node* temp = NULL;
	node* delete;
	temp = ptr;

	while(temp)
	{
		if(!strcmp(temp->str, str))
			delete = temp;
		temp = temp->link;
	}
	if(delete != NULL)
	{
		score += strlen(delete->str) * level;
		strcpy(delete->str, "");
	}
}

void* levelUp()
{
	while(level < 10 && hp > 0)
	{
		sleep(10);
		level++;
		timer -= 200000; //down speed decrease 0.2sec
		sprintf(levelText, "%d", level);
		move(24,55); addstr("  |            ");
		move(24,55); addstr(levelText);
	}
}

void gameOver()
{
	int check=0, n=0;
	char option;
	char t[10], finalScore[10];
	FILE *fp;

	fp = fopen("score.txt", "r");
	if(fp == NULL)
		n = -1;
	else
	{
		fscanf(fp, "%d\n", &n);
		fscanf(fp, "%s %d\n", t, &n);
	}
	sprintf(finalScore, "%d", score);
	eraseScreen();
	if(score >= n)
		draw(7,2, "Congratulations!!! Your score is the highest score!!!");
	draw(9,18, "Score: "); move(10,23); addstr(finalScore);
	draw(12,16, "1. Save Score");
	draw(14,16, "2. User Score");
	draw(16,16, "3. Exit");
	refresh();

	if(fp != NULL)
		fclose(fp);
	move(LINES-1,0);
	option = getch();
	switch(option)
	{
		case '1':
			saveScore();
			break;
		case '2':
			showScore();
			break;
		case '3':
			endwin();
			exit(0);
	}
}

void saveScore()
{
	int i, j, k=0, n, key;
	int* data;
	char keyname[10];
	char tname[10];
	namescore* t;
	FILE *fp, *fp2;
	
	fp = fopen("score.txt", "r");
	if(fp == NULL)
	{
		fp = fopen("score.txt", "w+");
		fprintf(fp, "%d\n", 1);
		fprintf(fp, "%s ", name);
		fprintf(fp, "%d\n", score);
		fclose(fp);
		return;
	}
	fscanf(fp, "%d\n", &n);
	n = n+1;
	t = (namescore*)malloc(sizeof(namescore) * n);

	while(fscanf(fp, "%s %d\n", tname, &i) != EOF)
	{
		strcpy(t[k].sname, tname);
		t[k].data = i;
		k++;
	}
	fclose(fp);
	strcpy(t[k].sname, name);
	t[k].data = score;

	for(i=1; i<n; i++)
	{
		namescore temp = t[i];
		j = i-1;
		while(temp.data > t[j].data && j>=0)
		{
			t[j+1] = t[j];
			j--;
		}
		t[j+1] = temp;
	}

	fp2 = fopen("score.txt", "w+");
	fprintf(fp2, "%d\n", n);
	for(i=0; i<n; i++)
		fprintf(fp2, "%s %d\n", t[i].sname, t[i].data);
	fclose(fp2);
}

void showScore()
{
	int i, j, k=0, n, key, row;
	int* data;
	char dataText[10], iText[3], t, tname[10];
	namescore *tt;
	FILE* fp;

	fp = fopen("score.txt", "r");
	if(fp == NULL)
	{
		eraseScreen();
		draw(11,16, "!! There is no result !!");
		t = getch();
	}
	else
	{
		fscanf(fp, "%d\n", &n);
		tt = (namescore*)malloc(sizeof(namescore) * n);
		while(fscanf(fp, "%s %d\n", tname, &i) != EOF)
		{
			strcpy(tt[k].sname, tname);
			tt[k].data = i;
			k++;
		}
		eraseScreen();
		draw(2,26, "SCORE");
		for(i=0; i<10; i++)
		{
			row = 2*i + 4;
			sprintf(iText, "%d", i+1);
			sprintf(dataText, "%d", tt[i].data);
			draw(row,23, iText);
			move(row,26); addstr(tt[i].sname);
			move(row,32); addstr(dataText);
		}
		refresh();
		fclose(fp);
		t = getch();
	}
}

void startGame()
{
	pthread_t t1, t2;

	eraseScreen();
	//                 8                     30         42           55
	draw(24,0, "|Enter:                  |HP:     |Score:      |Level:   |");
	draw(25,0, " --------------------------------------------------------");
	
	sprintf(hpText, "%d", hp);
	sprintf(scoreText, "%d", score);
	sprintf(levelText, "%d", level);
	move(24,30); addstr(hpText);
	move(24,42); addstr(scoreText);
	move(24,55); addstr(levelText);

	pthread_create(&t1, NULL, showWord, NULL);
	pthread_create(&t2, NULL, levelUp, NULL);
	while(hp > 0)
	{
		char input[20];
		scanw("%s", input);
		
		if(strcmp(input, "quit") == 0)
		{
			clear();
			endwin();
			exit(1);
		}
		strcpy(enterText, input);
		findWord(enterText);
		sprintf(scoreText, "%d", score);
		move(24,42); addstr(scoreText);
		move(24,8); addstr("                  ");
		move(24,8);
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	eraseScreen();
}
