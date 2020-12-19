#2016112720 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef struct node node;

typedef struct node
{
	int row;
	int col;
	char str[20];
	node* link;
}node;

//global variable
int hp = 100;		//health point
char hpText[3]={0};

int a=0;
int i=0;
int length=0;

node *ptr =NULL;	//rain header
char enterText[20]={0};//input text
int enterHere=0;	
int timer=2000000;	//time row down
int level=0;
char levelText[3]={0};
int score=0;
char scoreText[10]={0};

void function(int signum);
void* showWord();			
int itoa(int n,char *str);		//for addstr int to string
void findWord(char *str);		//search word
node* makeNode();			//make node for insert queue
void makePlusOne();			//words in queue row+1
void addQueue(char* str,int col);	//insert queue 
char* returnWord();			//return random word in DB
void draw(int row,int col,char *str);	//move+addstr
void startGame();			//game start
void* levelup();			//every 1o sec after speed up!!
void gameover();			//gameover
void savescore();			//save score in file score.txt
void showscore();			//open score file score.txt

void main()
{
	char option;
	
	initscr();
	clear();
	
	draw(3,10,"---------------------------------------------------");
	draw(4,10,"							");
	draw(5,10,"			acid rain			");
	draw(6,10,"							");
	draw(7,10,"							");
	draw(8,10,"---------------------------------------------------");
	draw(11,10,"1. Start Game");
	draw(12,10,"2.User Score");
	draw(13,10,"3.Exit");
	
	move(16,20);
	refresh();
	
	option=getch();//option get
	
	switch (option) {
	case '1':
		startGame();//play game
		break;
	case '2':
	{
		showscore();
		endwin();
		exit(0);
		break;
	}
		//show score
	case '3':
		break; //exit
	}
		
	gameover();
	endwin();
}

void function(int signum)
{
	curs_set(1);
	endwin();
	exit(1);
}
void* showWord()
{

	while(hp>0)
	{
		node *temp=NULL;
		addQueue(returnWord(),(rand()%50)+1);
		temp=ptr;

		while(temp)
		{
			draw(temp->row,temp->col,temp->str);
			temp=temp->link;
		}
		move(17,8);
		usleep(timer);
	}		
}
int itoa(int n,char *str)
{
	int temp;

	if(n<=0)
	{
		strcpy(str,"0");
		return 0;
	}

	sprintf(str,"%d",n);

	return 0;
}

void findWord(char *str)
{
	node* temp=NULL;
	temp=ptr;
	while(temp)
	{
		if(!strcmp(temp->str,str))
		{
			score +=strlen(str)*level;
			strcpy(temp->str,"");
			return;
		}
		else 
			temp=temp->link;
	}
}

node* makeNode()
{
	node *temp;

	temp=(node*)malloc(sizeof(node));
	temp->link=NULL;
}

void makePlusOne()//evert node in queue row+1
{
	node *temp=ptr->link;

	while(temp)
	{
		temp->row+=1;
		temp=temp->link;
	}
}

void addQueue(char* str,int col)
{
	node *temp=0;
	node *temp2=0;

	if(ptr==NULL)
	{
		ptr=makeNode();
		strcpy(ptr->str,str);
		ptr->row=1;
	}
	else
	{
		temp=makeNode();
		strcpy(temp->str,str);
		temp->row=1;
		temp->col=col;
		temp->link=ptr;
		ptr=temp;
		makePlusOne();
	}

	length++;

	if(length>15)//word arrived botten
	{
		while(temp->link)
		{
			temp2=temp;
			temp=temp->link;
		}

		hp-=strlen(temp->str);//if findword works strlen(temp->str)=0 it means hp no changes

		hpText[2]='\0';
		itoa(hp,hpText);
		move(17,28);
		addstr("	");
		move(17,28);
		addstr(hpText);
		free(temp);
		temp2->link=0;

		length--;
	}
}

char* returnWord()
{
	char *database[]= {"Apple","June","Cocaine","elite"};
	if(a==4)
		a=0;
	else
		a++;
		
	return database[a];
}

void draw(int row,int col,char *str)
{
	move(row,0);
	addstr("								");
	move(row,col);
	addstr(str);
	refresh();
}
void* levelup()//난이도 증가
{
	while(level<10 && hp>0)
	{
	level++;
	timer-=200000;	//row down 0.2 sec decrese
	itoa(level,levelText);
	move(17,55);
	addstr(levelText);
	sleep(10);
	}
	
}
void gameover()
{
		int check=0,n=0;
		FILE* fp;
		char option;
		char scoreText[3]={0};
		
		fp=fopen("score.txt","r");
		if(fp==NULL)//score.txt doesn't exist it means now score is highest score!!!
		{
			n=-1;
		}
		else
		{
		fscanf(fp,"%d\n",&n);//first scan is score num
		fscanf(fp,"%d\n",&n);//this is highest score in file
		}
		itoa(score,scoreText);
		draw(1,10,"							");
		draw(2,10,"							");
		draw(3,10,"---------------------------------------------------");
		draw(4,10,"							");
		draw(5,10,"			GAME OVER			");
		draw(6,10,"							");
		draw(7,10,"							");
		draw(8,10,"---------------------------------------------------");
		if(score<n)
		draw(9,10,"							");
		else
		draw(9,10,"congratulations!!! your score is the highest score!!!");
		draw(10,28,"score:");
		move(10,35);
		addstr(scoreText);
		draw(11,10,"1. save score");
		draw(12,10,"2.User Score");
		draw(13,10,"3.Exit");
		draw(14,10,"							");
		draw(15,10,"							");
		draw(16,10,"							");
		
		refresh();
		fclose(fp);
		option=getch();//option get
		switch (option) 
		{
			case '1':	
			savescore();//savescore
			break;
			case '2':
			showscore();//show score
			break;			
			case '3':
			endwin();
			exit(0);//exit
			break; 
		}
}
void savescore()
{
	int i,j,k=0,n,key;
	int *data;
	FILE *fp,*fp2;
	fp=fopen("score.txt","r");
	if(fp==NULL)
	{
	fp=fopen("score.txt","w+");
	fprintf(fp,"%d\n",1);
	fprintf(fp,"%d\n",score);
	fclose(fp);
	}
	
	fscanf(fp,"%d\n",&n);
	n=n+1;
	data=(int*)malloc(sizeof(int)*n);
	while(fscanf(fp,"%d\n",&i) != EOF)
	{
		data[k]=i;
		k++;
	}
	fclose(fp);
	data[k]=score;
	//insertionsort
	for(i=1; i< n; i++)
	{
        	key = data[i];
        	j=i-1;
        while(j>=0 &&data[j]<key)
        {
              data[j+1] = data[j]; 
              j--;
        }

        data[j+1] = key;             
    }

	fp2=fopen("score.txt","w+");
	fprintf(fp2,"%d\n",n);
	for(i=0; i<n; i++)
	{
		fprintf(fp2,"%d\n",data[i]);
	}
	
	fclose(fp2);
}
void showscore()
{	
	int i,j,k=0,n,key;
	int *data;
	char dataText[10]={0},iText[3]={0},t;
	FILE *fp;
	fp=fopen("score.txt","r");
	if(fp==NULL)
	{
		clear();
		draw(10,25,"!!there is no result!!");
		t=getch();
	}
	else
	{	
	fscanf(fp,"%d\n",&n);
	data=(int*)malloc(sizeof(int)*n);
	while(fscanf(fp,"%d\n",&i) != EOF)
	{
		data[k]=i;
		k++;
	}
	clear();
	draw(2,32,"SCORE");
	for(i=0;i<n;i++)
	{
		itoa(i+1,iText);
		itoa(data[i],dataText);
		draw(i+3,30,iText);
		move(i+3,33);
		addstr(dataText);

	}
	refresh();
	t=getch();
	fclose(fp);
	}
}
void startGame()
{
	pthread_t t1,t2;

	clear();

	draw(16,0,"---------------------------------");
	draw(17,0," |Enter :		|HP:	   |Score:	|level:	");
	draw(18,0,"---------------------------------");

	itoa(hp,hpText);
	itoa(score,scoreText);
	itoa(level,levelText);
	move(17,28);
	addstr(hpText);
	move(17,45);
	addstr(scoreText);
	move(17,55);
	addstr(levelText);

	pthread_create(&t1,NULL,showWord,NULL);
	pthread_create(&t2,NULL,levelup,NULL);

	while(hp>0)
	{
		for(enterHere =0; enterHere <20;)
		{
			int c=getch();

			if(c=='\n')
			{
				enterText[enterHere]='\0';
				findWord(enterText);

				for(i=0;i<20;i++)
				{
					enterText[i]='\0';
				}

				draw(17,0," |Enter :		|HP:	   |Score:	|level:	");
				itoa(score,scoreText);
				itoa(level,levelText);
				move(17,28);
				addstr(hpText);
				move(17,45);
				addstr(scoreText);
				move(17,55);
				addstr(levelText);

				break;
			}
			else if(c==127)
			{
				if(enterHere>0)
				{
					enterText[--enterHere]='\0';
					move(17,12);
					addstr("				");
					move(17,12);
					addstr(enterText);
				}
				else
				{
					move(17,12);
					addstr("				");
				}
			}
			else
			{
				enterText[enterHere++]=c;
				move(17,12);
				addstr(enterText);
			}

			refresh();
		}
	}
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	clear();
}


