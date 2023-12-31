 TYPE cel;
    int32 back;
    int32 item;
 END
 
 type loc;
 	int row;
 	int col;
 END
 
 type path
 	int row[500];
 	int col[500];
 END
 
 type wrd_set
 	STRING txt[4];
 END

CONST
	_treasure=601;
	_gold=602;
	_pillar=50;
	_hider=10;
	_wall=3;
	_trap=4;
	_closed_door=60;
	_open_door=61;
	_monster=301;
	_start=501;
	_treasdar=603;
	_monsdar=604;
	_prisoner=605;
	_double=606;
	_triple=607;
	_torch=608;
	_torch2=616;
	_mon_path=615;
	_slider=-50;
	
	_engl=0;
	_span=1;
	_germ=2;

GLOBAL
	int fpg[1];
	int fnt[1];
	
	int the_song;

	int song[2];
	int suc_song;
	int fal_song;
	int win_song;
	int s_click;
	int s_click2;
	int s_dang;
	int s_dead;
	int s_coin;
	int s_sonar;
	int s_prison;
	int s_multi;
	int s_open;
	int s_fail;
	
	string level_file;
	string last_level_file[2];
	string last_last_level_file[2];
	string last_last_last_level_file;
	
	string output;

	int mouse_release=false;
	int kill_menus=false;
	int start_game=false;

	int base_row;
	int base_col;
	
	int up_row;
	int up_col;
	
	int x_shift;
	int y_shift;
	
	int rs[11]=3,22,41,60,79,98,117,136,155,174,193,212;
	int cs[11]=236,217,198,179,160,141,122,103,84,65,46,27;
	
	int itm[11];

	int lives=3;
	int selected_hero;
	int monst;
	int the_map;
	int max_mon_strt;
	
	
	int change_turn=false;
	int success=false;
	int newlife=false;
	int who_goes=0;
	int moves_left;
	int bad_moves;
	int score;
	int round=1;
	int multi=1;
	
	
	int zoom=1;
	int lang=0;
	int diff=1;
	int iter=1;
	int volume_level=30;
	int baddy;
	int crnt_song;
		
	STRUCT vil[12]
		int stand[19];
		int walk[7];
		int kill[19];
	END=
	//WOLF
	301,301,302,302,303,303,304,304,304,304,304,304,305,305,306,306,-01,-01,-01,-01, //stand
	307,308,309,310,311,312,313,314,	//walk
	315,315,316,316,317,318,319,319,320,320,320,320,319,318,317,316,316,315,315,-01,	
	//MUMMY
	301,301,301,302,302,302,303,303,303,304,304,304,305,305,305,306,306,306,-01,-01, //stand
	307,308,309,310,311,312,313,314,	//walk
	315,315,316,317,318,318,318,317,316,315,315,315,316,316,317,318,318,317,316,-01,	
	//VAMPIRE
	301,301,301,301,301,302,302,302,302,303,303,303,303,303,304,304,304,304,-01,-01, //stand
	305,306,307,308,309,310,311,312,	//walk
	313,313,313,314,314,315,315,316,316,316,316,316,315,315,314,314,313,313,-01,-01,	
	//SWAMP MONSTER
	301,301,301,302,302,303,303,304,304,305,305,306,306,307,307,-01,-01,304,-01,-01, //stand
	308,309,310,311,312,313,314,315,	//walk
	316,316,316,316,317,317,317,318,318,318,318,319,320,320,-01,314,313,313,-01,-01,	
	//ALIEN
	301,301,301,302,302,302,303,303,303,304,304,304,-01,-01,-01,-01,-01,-01,-01,-01, //stand
	305,306,307,308,309,310,311,312,	//walk
	313,313,314,314,315,315,316,316,317,317,317,317,318,318,-01,314,313,313,-01,-01,	
	//GHOST
	301,301,302,302,303,303,304,304,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01, //stand
	301,301,302,302,303,303,304,304,	//walk
	305,306,307,308,308,309,310,311,311,310,309,308,308,309,310,311,311,-01,-01,-01,	
	//YETI
	301,301,301,302,302,303,303,304,304,304,305,305,306,306,-01,-01,-01,-01,-01,-01, //stand
	307,308,309,310,311,312,313,314,	//walk
	315,315,316,317,317,317,316,315,315,315,316,317,317,317,316,-01,311,-01,-01,-01,	
	//GODZIRA
	301,301,301,302,302,302,303,303,303,304,304,304,305,305,305,306,306,306,-01,-01, //stand
	307,308,309,310,311,312,313,314,	//walk
	315,315,315,316,316,317,317,317,318,318,318,318,317,317,316,316,-01,-01,-01,-01,
	//MAGMA MAN
	301,301,302,302,303,303,304,304,305,305,305,304,304,303,303,302,302,301,-01,-01, //stand
	306,307,308,309,310,311,312,313,	//walk
	314,315,316,317,318,319,319,318,317,316,315,314,-01,-01,-01,-01,-01,-01,-01,-01,
	//SCARE CROW
	301,301,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01, //stand
	302,303,304,305,306,307,308,309,	//walk
	310,310,310,310,310,310,311,312,313,313,313,313,-01,-01,-01,-01,-01,-01,-01,-01,
	//JAGUAR WOMAN
	301,301,302,302,303,303,304,304,305,305,306,306,-01,-01,-01,-01,-01,-01,-01,-01, //stand
	307,308,309,310,311,312,313,314,	//walk
	315,315,315,315,316,316,317,318,318,318,318,318,-01,-01,-01,-01,-01,-01,-01,-01,
	//MACHINE
	301,301,302,302,303,303,304,304,304,304,303,303,302,302,301,301,-01,-01,-01,-01, //stand
	305,305,306,306,307,307,308,308,	//walk
	309,309,309,310,310,311,311,311,311,310,310,-01,-01,-01,-01,-01,-01,-01,-01,-01,
	//SKELETON
	301,301,301,302,302,302,303,303,303,304,304,304,-01,-01,-01,-01,-01,-01,-01,-01, //stand
	305,306,307,308,309,310,311,312,	//walk
	313,313,313,313,314,314,314,314,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01,-01;

	STRUCT the_map_grid[0];
		int cord[31][31];
	END

	STRUCT vis_grid[0];
		int cord[31][31];
	END

	STRUCT	grid[0] 
		cel cord[31][31];
	END
	
	STRUCT dis_grid[0]
	    int cord[11][11];
	END
	
	STRUCT back_grid[0]
	    int cord[11][11];
	END
	
	STRUCT profile[0];
		int lang;
		int vol;
		STRUCT hiscore[9];
			int val;
			STRING name;
		END
	END=0,30,
	40000,"ANTWAN",38575,"FIREFLY",35525,"TMONEY",31025,"KIM",26575,"ANDY",23000,"FIREFLY",19900,"AAA",19125,"FIREFLY",16250,"ANTWAN",15725,"COOLBEANS";
	
LOCAL
	int a,b,c,d;
	int control;
	int selected;
	int stuck;
	int ident;
	int row,col;
	int move_row,move_col;
