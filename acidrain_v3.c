
void function(int signum)
{
	curs_set(1);
	//endwin();
	//exit(1);
}
void startGame()
{
	signal(SIGINT, function);
	pthread_t t1,t2;
	char quit[4] = "quit";

	clear();
	draw(0, 0, "|                                                  |");
	draw(1, 0, "|                                                  |");
	draw(2, 0, "|                                                  |");
	draw(3, 0, "|                                                  |");
	draw(4, 0, "|                                                  |");
	draw(5, 0, "|                                                  |");
	draw(6, 0, "|                                                  |");
	draw(7, 0, "|                                                  |");
	draw(8, 0, "|                                                  |");
	draw(9, 0, "|                                                  |");
	draw(10, 0, "|                                                  |");
	draw(11, 0, "|                                                  |");
	draw(12, 0, "|                                                  |");
	draw(13, 0, "|                                                  |");
	draw(14, 0, "|                                                  |");
	draw(15, 0, "|                                                  |");
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
				if (strcmp(quit, enterText) == 0) {
					endwin();
					exit(0);
				}
				enterText[enterHere++]=c;
				move(17,12);
				addstr(enterText);
				//while (getchar() != '\n');
			}

			refresh();
		}
	}
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	clear();
}


