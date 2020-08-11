#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <string>



#define hauteur_win 20
#define largeur_win 70
#define alttic 50
#define limite_hauteur_terrain 6

using namespace std;

string debug_text="ras";







class Terrain {
	public:

		int geo[largeur_win];
		int arbre[largeur_win];


	
		void generate(){

			srand(time(NULL));


			geo[0]=hauteur_win - rand()%limite_hauteur_terrain-2;
			geo[1]=geo[0];

			for(int i=2; i<largeur_win;i=i+2){

				if(geo[i-1]<= hauteur_win - limite_hauteur_terrain){
					geo[i]=geo[i-1]+rand()%2;
				}else if (geo[i-1]==hauteur_win-2){
					geo[i]=geo[i-1]-rand()%2;
				}else{
					geo[i]=geo[i-1]+(1-rand()%3);
				}
				geo[i+1]=geo[i];
				
			}

			for(int i=1; i<largeur_win-1;i++){
				if((geo[i-1]==geo[i]) & (geo[i+1]==geo[i])){
					arbre[i]=rand()%4;
				}

			}


			//debug
			/*
			for(int i=0;i<largeur_win;i++){
				printf("%i,%i\n",i,geo[i]);
			}
			*/
		}


		void creuser(int x){
			if(geo[x]<hauteur_win-1){
				geo[x]=geo[x]+1;
			}
			arbre[x]=0;
		}



		void afficher(WINDOW* win){
			for(int i=0; i<largeur_win; i++){

				//mvwprintw(win ,geo[i], i , "#");
				for(int j=geo[i]; j<hauteur_win-1; j++){
					mvwprintw(win ,j, i , "#");
				}

				if(arbre[i]==1){
					mvwprintw(win ,geo[i]-1, i , "Y");
					//mvwprintw(win ,geo[i], i , "i");
				}
			}
		}

};


class Ennemi{
	public:

		int HP=1;
		double x=-2;
		int y=-2;
		char motif='M';
		int bouge=1;
		int sens=0;
		double vitesse=1;
		

		void spawn(Terrain terrain,char pmotif, int pbouge, double pvitesse, int pHP, double px){
			x=px;
			y=terrain.geo[int(x)]-1;
			motif=pmotif;
			bouge=pbouge;
			vitesse=pvitesse;
			srand(time(NULL));
			sens=rand()%2;
			HP=pHP;

		}
		void spawn(Terrain terrain,char pmotif, int pbouge, double pvitesse, int pHP){
			srand(time(NULL));
			spawn(terrain, pmotif, pbouge, pvitesse ,pHP , 1+rand()%(largeur_win-1));
		}
		void update(Terrain terrain){

			if(bouge){

				if(rand()%40==1) sens=!sens;

				if(!sens){
					if((x<largeur_win-2) & (abs(terrain.geo[int(x)]-terrain.geo[int(x)+1])<2)){
						x=x+vitesse;
						y=terrain.geo[int(x)]-1;
					}else{
						sens=1;
					}
				}else{
					if((x>1) & (abs(terrain.geo[int(x)]-terrain.geo[int(x)-1])<2)){
						x=x-vitesse;
						y=terrain.geo[int(x)]-1;
					}else{
						sens=0;
					}
				}

			}
			
		}
		int hit(){
			HP--;
			if(HP==0){
				return 1;
			}
			return 0;
		}
};


class Bombe {

	public:
		int exist=0;
		int x=-1;
		int y=-1;
		int sens=0;

		void spawn(int px, int py, int psens){
			x=px;
			y=py;
			sens=psens;
			exist=1;
		}
		int update(Terrain &terrain, Ennemi &ennemi){

			if(exist){
				if(sens==0) x++;
				if(sens==1) x--;
				y++;
				if((x>=largeur_win) | (x<=0)){
					exist=0;
					return 0;
				}

				if((x==int(ennemi.x))&(y==ennemi.y)){
					exist=0;
					debug_text="hit";
					return 2;
				}

				if(y>=terrain.geo[x]){
					terrain.creuser(x);
					exist=0;
					//usleep(500000);
					return 1; //boom
				}





			}
			return 0;
		}
};





class Avion {      
  	private:            


	public:
		int HP=5;
		int sens=0; //0 -> et 1 <-
		int x=1;
		int y=1;
		int munition=500;



		void haut(){
			if(y>1){
				y--;
			}
		}
		void bas(){
			if(y<hauteur_win-2){
				y++;
			}
		}
		int update(Terrain terrain){
			if(sens==0){
				if(x>=largeur_win-2){
					sens=1;
					x--;
				}
				else{
					x++;
				}
			}else{
				if (x<=1){
					sens=0;
					x++;
				}else{
					x--;
				}
			}
			

			if(y>=terrain.geo[x]){
				HP--;
				x=1;
				y=1;
				sens=0;
			}

			return 0;

		}

		int largage(){
			if (munition > 0){
				munition--;
				return 1;
			}
			return 0;
		}


};






int main(int argc, char *argv[]){

	int debug=0;

	for(int i=1; i<argc; i++){
		if ( strcmp(argv[i], "debug")==0){
			debug=1;
		}
	}


	printf("STARTING.........\n");


	

	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	WINDOW * win = subwin(stdscr, hauteur_win, largeur_win, 0,0 );
	
   
	
	

	
	WINDOW * debugwin = subwin(stdscr, 10, 30, 0,largeur_win );
	
	

    box(win, ACS_VLINE, ACS_HLINE);
	mvwprintw(win ,0,1,"win");
	
	
	
	
	int count=0;
    
	int ch;

	int altertic=alttic;

	Avion avion;
	Bombe bombe;
	Terrain terrain;
	Ennemi testennemi;
	terrain.generate();
	testennemi.spawn(terrain, '%',1, 0.3, 1);

	int score=0;

	
	while (ch!='q'){
		
		ch = getch(); //recup clavier


		switch (ch)
		{
		case KEY_UP:
			avion.haut();
			break;
		case KEY_DOWN:
			avion.bas();
			break;
		case KEY_LEFT:
			avion.sens=1;
			break;
		case KEY_RIGHT:
			avion.sens=0;
			break;

		case KEY_F(1):
			debug=!debug;
			break;

		case 'p':
			mvwprintw(win ,3,10,"<PAUSE>");
			usleep(100000);
			wrefresh(win);
			wrefresh(debugwin);
			while(getch()!='p');
			break;
		

		case ' ':
			if(!bombe.exist){
				if(avion.largage()) bombe.spawn(avion.x, avion.y, avion.sens);
			}
			break;
		
		case 'r':
			terrain.generate();
			break;
		
		default:
			break;
		}



		


		if(altertic==0){

			werase(win); //effacement fenetre
			//affichage graph


		

			terrain.afficher(win);
			
			if(testennemi.HP >0){
				testennemi.update(terrain);
				mvwprintw(win ,testennemi.y, int(testennemi.x), "%c",testennemi.motif);
			}


			avion.update(terrain);
			if(avion.sens==0) mvwprintw(win ,avion.y, avion.x, ">");
			else mvwprintw(win ,avion.y, avion.x, "<");
			if(bombe.exist){

				switch (bombe.update(terrain,testennemi))
				{
				case 1:
					mvwprintw(win ,bombe.y, bombe.x-2, "{{@}}");
					mvwprintw(win ,bombe.y-1, bombe.x, ".");
					break;
				
				case 2:
					score+=100;
					mvwprintw(win ,bombe.y, bombe.x-2, "{{");
					mvwprintw(win ,bombe.y, bombe.x+1, "}}");
					mvwprintw(win ,bombe.y-1, bombe.x, ". HIT !");
					testennemi.hit();

					mvwprintw(win ,3,10,"<NICE !>");
					box(win, ACS_VLINE, ACS_HLINE);

					wrefresh(win);

					usleep(500000);
					mvwprintw(win ,4,10,"<Press SPACE to continue>");

					wrefresh(win);

					while(getch()!=' ');

					testennemi.spawn(terrain, 'r',1, (rand()&100)/1000.0+0.25, 1);
					//avion.x=1;
					//avion.y=1;
					//terrain.generate();
					break;

				default:
					mvwprintw(win ,bombe.y, bombe.x, "-");
					break;
				}



			}




			box(win, ACS_VLINE, ACS_HLINE);
			
			mvwprintw(win ,0,1,"AMMO: %i  HP: %i SCORE: %i", avion.munition, avion.HP, score);






			if (debug){
				count=count+1;
				if (ch!=-1){
					mvwprintw(debugwin ,1, 2, "Input:               ");
					mvwprintw(debugwin ,1, 2, "Input: %x", ch);
				}
				mvwprintw(debugwin ,2, 2, "tic:      ");
				mvwprintw(debugwin ,2, 2, "tic: %i", count);

				mvwprintw(debugwin ,3, 2, "x:      ");
				mvwprintw(debugwin ,3, 2, "x: %i", avion.x);

				mvwprintw(debugwin ,4, 2, "y:      ");
				mvwprintw(debugwin ,4, 2, "y: %i", avion.y);
				
				mvwprintw(debugwin ,5, 2, "geo[10]:      ");
				mvwprintw(debugwin ,5, 2, "geo[10]: %i", terrain.geo[10]);

				mvwprintw(debugwin ,6, 2, "debug:             ");
				mvwprintw(debugwin ,6, 2, "debug: %s", debug_text.c_str());
			}


			
			
			if(debug){
				box(debugwin, ACS_VLINE, ACS_HLINE);
				mvwprintw(debugwin ,0,1,"debug");
			}
	


			wrefresh(win);
			wrefresh(debugwin);

			altertic=alttic;

		}else{
			altertic--;
		}


		usleep(1000);

		
		
	}
	

	
	
	endwin();

	for(int i=0;i<largeur_win;i++){
		printf("%i,%i\n",i,terrain.geo[i]);
	}
	
	
	return EXIT_SUCCESS;
}



