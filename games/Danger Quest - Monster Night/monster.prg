import "libmod_gfx";
import "libmod_misc";
import "libmod_sound";
import "libmod_input";

declare PROCESS key_item(control, double y);
declare PROCESS a_card(double x,y,int base_graph);
declare PROCESS a_die(double x,y, int tipe);

include "jkey.lib";
include "data.inc";
include "l_s.inc";
include "func.inc";
include "menu.inc";
include "map.inc";
include "card_game.inc";

BEGIN
	IF(os_id!=OS_GP2X AND os_id!=OS_GP2X_WIZ AND os_id!=OS_CAANOO) screen.scale_resolution=06400480; END
	set_fps(40,0);
	set_mode(320,240);
	RAND_SEED(time());

	window_set_title("DANGER QUEST: MONSTER NIGHT");

	fpg[0]=fpg_load("fpgs/main.fpg");
	fnt[0]=fnt_load("files/mons.fnt");

    FRAME;

	graph=map_load("fpgs/bennugd.png");
	x=160; y=120;

	FOR(a=0;a<12;++a) FRAME; END
	FOR(a=0;a<60;++a)
		FRAME;
	END
    FRAME;
      //  song[0]=sound_load("files/monster2.mp3");


	map_unload(0,graph);

	FOR(a=0;a<12;++a) FRAME; END

	graph=map_load("fpgs/gph.png");
	FOR(a=0;a<60;++a)
		FRAME;
	END
    FRAME;
    // song[1]=sound_load("files/monster.mp3");

	map_unload(0,graph);

	FOR(a=0;a<12;++a) FRAME; END

	graph=23;
	FOR(a=0;a<60;++a)
		FRAME;
	END
    FRAME;
    //song[2]=sound_load("files/monster3.mp3");

	FOR(a=0;a<12;++a) FRAME; END
	graph=0;

	//check if profile is set up
	IF(fexists(get_pref_path("bennugd.org", "DangerQuestMonsterNight") + "prof.dat"))
		load_prof();
		menu_overhead();
	//	keyboard();
	ELSE
		a=menu_but(0,9); //ENGLISH
		a.selected=true;
		menu_but(1,10); //SPANISH
		menu_but(2,11); //GERMAN
	END

	FRAME;

	jkeys_set_default_keys();
	jkeys_controller();

	load_sound();
	set_all_sound();

	music_control();

	x=160;
	y=220;
	z=-450;

	LOOP
		graph=0;

		IF(jkeys_state[ _JKEY_VOLUP ])
			volume_level+=a;
			a++;
			IF(volume_level>203) volume_level=203; END
			set_all_sound();
		ELSE a=1; END

		IF(jkeys_state[ _JKEY_VOLDOWN ])
			volume_level-=b;
			b++;
			IF(volume_level<0) volume_level=0; END
			set_all_sound();
		ELSE b=1; END

		IF(!exists(type volume_control) AND (jkeys_state[ _JKEY_VOLUP ] OR jkeys_state[ _JKEY_VOLDOWN ]))
			graph=16;
			volume_knob2();
		END

//		IF(key(_esc)) exit(); END

        IF(mouse.left)
			mouse.graph=999;
			mouse_release=false;
		ELSE
			mouse_release=false;
			IF(mouse.graph==999) mouse_release=true;  END
			IF(os_id!=OS_GP2X AND os_id!=OS_GP2X_WIZ AND os_id!=OS_CAANOO) mouse.graph=998; ELSE mouse.graph=0; END
		END
                
		FRAME;
	END
END


PROCESS game();
PRIVATE
	int txt[7];
	wrd_set scr;
	wrd_set mvs;
	STRING outp[1];
BEGIN
	scr.txt[_engl]="SCORE"; scr.txt[_span]="PUNTOS"; scr.txt[_germ]="KERBE";
	mvs.txt[_engl]="MOVES: "; mvs.txt[_span]="MOVIDAS: "; mvs.txt[_germ]="DREHT: ";
	moves_left=0;
	who_goes=0;
	bad_moves=0;
	score=0;
	lives=3;
	zoom=1;
	diff=1;
	round=1;

	mole();

	graph=1;
	x=0; y=239;

	z=-100;
	
	text.z=-256;

	txt[0]=write_var(0,304,81,ALIGN_CENTER_RIGHT,outp[1]);
	txt[1]=write_var(0,304,81,ALIGN_CENTER_LEFT,moves_left);
//	txt[2]=write_var(0,280,9,ALIGN_TOP_RIGHT,outp[0]);
//	txt[3]=write_var(0,276,9,ALIGN_TOP_LEFT,score);
	txt[2]=write_var(0,239,9,ALIGN_TOP_LEFT,outp[0]);
	txt[3]=write_var(0,239,18,ALIGN_TOP_LEFT,score);
	txt[4]=write_var(0,304,157,ALIGN_CENTER_RIGHT,outp[1]);
	txt[5]=write_var(0,304,157,ALIGN_CENTER_LEFT,bad_moves);
	txt[6]=write(0,304,9,ALIGN_TOP_RIGHT,"X");
	txt[7]=write_var(0,304,9,ALIGN_TOP_LEFT,multi);

	LOOP
		outp[0]=scr.txt[lang]; outp[1]=mvs.txt[lang];

		graph=zoom;
		IF(change_turn)
			change_turn=false;
			WHILE(exists(type a_die)) signal(type a_die, s_kill); END
			a_die(275-40,198-49,1); a_die(275+2,198-49,1);
		END

		a=get_mouse_row()+base_row;
		b=get_mouse_col()+base_col;

		IF(success)
			success=false;
			//IF(round++%2==0)
			round++;
			diff++;// END

			IF(round<11)
				mole();
			ELSE
				sound_stop(the_song);
				a=get_id(type music_control); a.selected=true;
				//crnt_song=PLAY_SONG(win_song,0);
				the_song=sound_play(win_song,0);
				write_delete(all_text);
				graph=0;
				text.z=-256;
				txt[0]=write_var(0,160,110,ALIGN_BOTTOM,outp[0]);
				txt[1]=write_var(0,160,130,ALIGN_BOTTOM,outp[1]);
				scr.txt[_engl]="THE EARTH WAS"; scr.txt[_span]="LA TIERRA FUE"; scr.txt[_germ]="DIE ERDE WURDE";
				mvs.txt[_engl]="OVERRUN BY MONSTERS"; mvs.txt[_span]="CONQUISTADA POR LOS MONSTRUOS"; mvs.txt[_germ]="VON MONSTERS BESIEGT";
				outp[0]=scr.txt[lang]; outp[1]=mvs.txt[lang];

				FOR(a=0;a<60;++a)
					FRAME;
				END
			//	a=get_id(type music_control); a.selected=true;
				FOR(a=0;a<60;++a)
					FRAME;
				END
			//	a=get_id(type music_control); a.selected=true;
				scr.txt[_engl]="BUT YOU ARE SUPER RICH!"; scr.txt[_span]="PERO USTED ES SUPER RICO!"; scr.txt[_germ]="ABER SIE SIND SUPER REICH!";
				mvs.txt[_engl]="CONGRATULATIONS!"; mvs.txt[_span]="FELICITACIONES!"; mvs.txt[_germ]="GRATULATION!";
				outp[0]=scr.txt[lang]; outp[1]=mvs.txt[lang];

				FOR(a=0;a<60;++a)
					FRAME;
				END
			//	a=get_id(type music_control); a.selected=true;
				FOR(a=0;a<60;++a)
					FRAME;
				END
			//	a=get_id(type music_control); a.selected=true;
				selected=true;
			END
		END

		IF(lives==0) //GAME OVER
			kill_menus=true;
			signal(type mole,s_kill_tree);
			signal(type a_die,s_kill_tree);
			signal(type a_torch,s_kill_tree);
			sound_stop(the_song);
			a=get_id(type music_control); a.selected=true;
			//crnt_song=PLAY_SONG(fal_song,0);
			the_song=sound_play(fal_song,0);
			FOR(a=0;a<8;++a)
				IF(txt[a]!=0) write_delete(txt[a]); txt[a]=0; END
			END
			x=160; y=120; graph=8;
			FOR(a=0;a<120;++a)
				FRAME;
			END
			stuck=true;
		END

		IF(selected)//exit game from menu
			kill_menus=true;

			signal(type mole,s_kill_tree);
			signal(type a_die,s_kill_tree);
			signal(type a_torch,s_kill_tree);
			FOR(a=0;a<8;++a)
				IF(txt[a]!=0) write_delete(txt[a]); txt[a]=0; END
			END
			stuck=true;
		END

		IF(stuck)
			graph=0;
			IF(score>profile[0].hiscore[9].val)
				d=keyboard();
				WHILE(exists(d))
					FRAME;
				END
				save_prof();
			END
			menu_overhead();
			BREAK;
		END

		FRAME;
	END
END


PROCESS mole();
PRIVATE
	int txt[1];
	string outp;
	wrd_set rnd;
	wrd_set dif;
	int heros[2];
BEGIN
	rnd.txt[_engl]="ROUND: "; rnd.txt[_span]="RONDA: "; rnd.txt[_germ]="SCHIEBE: ";
	dif.txt[_engl]="DIFFICULTY: "; dif.txt[_span]="DIFICULTAD: "; dif.txt[_germ]="SCHWULITAT: ";
	moves_left=0;
	who_goes=0;
	bad_moves=0;
	multi=1;

	fully_load_some_level();
	clear_map_grid();


	outp=rnd.txt[lang]+itoa(round);//+"    "+dif.txt[lang]+itoa(diff);
	text.z=-256; txt[0]=write_var(fnt[0],117,100,ALIGN_CENTER,outp);
	FOR(c=0;c<60;++c)
		FRAME;
	END
	rnd.txt[_engl]="BEWARE THE "; rnd.txt[_span]="CUIDADO CON EL "; rnd.txt[_germ]="HUTE DICH VOR DEN";

	outp=rnd.txt[lang];
	text.z=-256; txt[1]=write(fnt[0],117,120,ALIGN_CENTER,describe_monst());
	FOR(c=0;c<60;++c)
		FRAME;
	END
	IF(txt[0]!=0) write_delete(txt[0]); txt[0]=0; END
	IF(txt[1]!=0) write_delete(txt[1]); txt[1]=0; END

    display_controller();
	FOR(c=0;c<12;++c)
	  	map_clear(fpg[0],701+c,0);
	  	itm[c]=item_display(0,c,701+c);
	END

	set_torch2();
	selected_hero=hero(0,get_start(0,0),get_start(1,0));
	IF(lives>1) hero(1,get_start(0,1),get_start(1,1)); END
	IF(lives>2) hero(2,get_start(0,2),get_start(1,2)); END
	hero_highlighter();
	FOR(a=0;a<4;++a) mover_arrow(a); END
	get_max_mon_strt();
	d=rand(0,max_mon_strt);
	monst=the_monster(baddy,get_monster_start(0,d),get_monster_start(1,d));


	roll_but();
	hero_selector_but(0);
	hero_selector_but(1);
	hero_selector_but(2);
	zoom_but();
	map_but();
	gamemenu_but();
	key_but();
	LOOP
		control--;
		IF(jkeys_state[ _JKEY_X] AND control<0)
			a=0;
			WHILE(heros[a]=get_id(type hero)) IF(heros[a]==selected_hero) b=a; END a++; END
			c=b+1; IF(c>a-1) c=0; END
			IF(!exists(type slider)) slider(heros[c],selected_hero); END
                        //selected_hero=heros[c];
			control=7;
		END
		IF(jkeys_state[ _JKEY_Y] AND control<0)
			a=0;
			WHILE(heros[a]=get_id(type hero)) IF(heros[a]==selected_hero) b=a; END a++; END
			c=b-1; IF(c<0) c=a-1; END
			IF(!exists(type slider)) slider(heros[c],selected_hero); END
                        //selected_hero=heros[c];
			control=7;
		END

		IF(newlife)
			lives++;
			newlife=false;
			d=whos_dead();
			hero(d,get_start(0,d),get_start(1,d));
			WHILE(a=get_id(type hero_selector_but))
				a.selected=true;
			END
		END

		FRAME;
	END
ONEXIT
	IF(txt[0]!=0) write_delete(txt[0]); txt[0]=0; END
	IF(txt[1]!=0) write_delete(txt[1]); txt[1]=0; END
END

PROCESS slider(p_to,p_from);
BEGIN
    ident=_slider;
    row=p_from.row;
    col=p_from.col;
    priority=102;
  LOOP
    selected_hero=id;


    IF(++a%2==0)
      IF(col!=p_to.col)
        IF(col<p_to.col)
          col++;
        ELSE
          col--;
        END
      ELSE
        IF(row!=p_to.row)
          IF(row<p_to.row)
            row++;
          ELSE
            row--;
          END
        ELSE
          selected_hero=p_to;
          BREAK;
        END
      END
    END
    x=rs[row]+(x_shift*zoom);
    y=cs[col*zoom]+y_shift*zoom;
    set_base_cord();

    FRAME;
  END
END




PROCESS monster_think();
PRIVATE
BEGIN
	file=fpg[0];
	graph=973;
	z=-900;
	size=200;
	//x=rs[];
	//y=cs[];
	y=162; x=76;//x=277;

	FRAME;
END


PROCESS item_display(row,col,graph);
PRIVATE
     //base_x; base_y;
BEGIN

    file=fpg[0];
    priority=99;
	z=540-y;
	size=100*zoom;

	//IF(zoom==2) size+=1; END
	LOOP
	   	x=rs[row]+(x_shift*zoom);
        y=cs[col*zoom]+y_shift*zoom;
        z=542-y;
        //y=(base_y)-(57*(zoom-1))+(y_shift)*zoom;
        // IF(zoom==2) y+=19*2; END
        IF(zoom==2) size=100*zoom; ELSE size=100; END

	   	FRAME;
	END
END


PROCESS a_torch(row,col,t);
PRIVATE
	STRUCT my_cel[36]
		int ident;
		int item;
		int row;
		int col;
	END=		  			0,0,-1,3, 0,0,0,3, 0,0,1,3,
				  0,0,-2,2, 0,0,-1,2, 0,0,0,2, 0,0,1,2, 0,0,2,2,
		0,0,-3,1, 0,0,-2,1, 0,0,-1,1, 0,0,0,1, 0,0,1,1, 0,0,2,1, 0,0,3,1,
		0,0,-3,0, 0,0,-2,0, 0,0,-1,0, 0,0,0,0, 0,0,1,0, 0,0,2,0, 0,0,3,0,
		0,0,-3,-1, 0,0,-2,-1, 0,0,-1,-1, 0,0,0,-1, 0,0,1,-1, 0,0,2,-1, 0,0,3,-1,
				  0,0,-2,-2, 0,0,-1,-2, 0,0,0,-2, 0,0,1,-2, 0,0,2,-2,
				   			0,0,-1,-3, 0,0,0,-3, 0,0,1,-3;
BEGIN
	file=fpg[0];
	ident=_torch;
	priority=0;

	LOOP    //debug;
		IF(++d>10) d=0; END
		IF(t==0) graph=609+d/2; ELSE graph=_torch2; END
		IF(row>=base_row AND row<base_row+12/zoom AND col>=base_col AND col<base_col+12/zoom)
			x=(rs[(row-base_row)*zoom])+x_shift*zoom;//x=rs[row-base_row]*zoom;
			y=cs[(col-base_col)*zoom]+y_shift*zoom;
		ELSE //off screen
				graph=0;
		END
		z=541-y;
		size=100*zoom;

			FOR(a=0;a<37;++a)
				IF(row-base_row+my_cel[a].row>=0 AND row-base_row+my_cel[a].row<=11/zoom AND col-base_col+my_cel[a].col>=0 AND col-base_col+my_cel[a].col<=11/zoom)
					back_grid[0].cord[(row-base_row+my_cel[a].row)][(col-base_col+my_cel[a].col)]=grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].back;

					IF(grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].back==_wall OR grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item==_pillar)
						the_map_grid[0].cord[row+my_cel[a].row][col+my_cel[a].col]=1;
					END
					IF(grid[0].cord[ row+my_cel[a].row ][ col+my_cel[a].col ].item!=0) //objects
						IF(grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_start AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_monster  AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_mon_path AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_torch2)
                            dis_grid[0].cord[(row-base_row+my_cel[a].row)][(col-base_col+my_cel[a].col)]=grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item;
                        END
					END

				END

				IF(selected_hero!=id AND exists(monst) AND monst.row==row+my_cel[a].row AND monst.col==col+my_cel[a].col AND who_goes==1)
					IF(!monst.selected AND !exists(type slider)) slider(id,selected_hero); END
                                        //selected_hero=id;
				END
				IF(exists(monst) AND monst.row==row+my_cel[a].row AND monst.col==col+my_cel[a].col)
					monst.selected=true;
				END
			END

			IF(selected_hero==id)
				set_base_cord();
			END
		FRAME;
	END
ONEXIT
	FOR(a=0;a<37;++a)
		IF(exists(my_cel[a].ident)) signal(my_cel[a].ident,s_kill); END
		IF(exists(my_cel[a].item)) signal(my_cel[a].item,s_kill); END
	END
END


PROCESS hero(ident,row,col);
PRIVATE
	int last_row,last_col,last_br,last_bc;
	int temp[3];
	int moving;
	int first_stuck;
	int was_stuck;
	int txt;
	int t_cnt;
	string my_t_wrds;

	STRUCT my_cel[20]
		int ident;
		int item;
		int row;
		int col;
	END=0,0,-1,2, 0,0,0,2, 0,0,1,2,
	0,0,-2,1, 0,0,-1,1, 0,0,0,1, 0,0,1,1, 0,0,2,1,
	0,0,-2,0, 0,0,-1,0, 0,0,0,0, 0,0,1,0, 0,0,2,0,
	0,0,-2,-1, 0,0,-1,-1, 0,0,0,-1, 0,0,1,-1, 0,0,2,-1,
	0,0,-1,-2, 0,0,0,-2, 0,0,1,-2;
BEGIN
	//x=100; y=100;
	file=fpg[0];
	graph=501;
	z=540-y;
	priority=1;
	text.z=-99;
	txt=write_var(fnt[0],x,y-40*zoom,ALIGN_CENTER,my_t_wrds);
	text.z=-256;
	frame;
	set_base_cord();
	frame;

//	FOR(a=0;a<21;++a)
//		my_cel[a].ident=cel_display(0,0,0);
//	END

	LOOP
		write_move(txt,x,y-40*zoom);
		IF(--t_cnt<0) my_t_wrds=""; END

		IF(mouse_release)  //clicked on
			IF(get_mouse_row()==row-base_row AND get_mouse_col()==col-base_col)
				mouse_release=false;
				sound_play(s_click2,0);
				selected_hero=id;
			END
		END

		IF(grid[0].cord[row][col].item==_treasdar) //radar treasure
			score+=100*multi;
			t_cnt=30;
			my_t_wrds="100";
			sound_play(s_sonar,0);
			IF(multi==2) my_t_wrds+=" X2"; END
			IF(multi==3) my_t_wrds+=" X3"; END
			grid[0].cord[row][col].item=0;
			the_map=mapp(rand(1,2));
		END

		IF(grid[0].cord[row][col].item==_monsdar) //radar monster
			score+=75*multi;
			t_cnt=30;
			my_t_wrds="75";
			sound_play(s_sonar,0);
			IF(multi==2) my_t_wrds+=" X2"; END
			IF(multi==3) my_t_wrds+=" X3"; END
			grid[0].cord[row][col].item=0;
			the_map=mapp(3);

		END

		IF(grid[0].cord[row][col].back==_trap AND !stuck AND !was_stuck) //trap
			graph=517+30*ident;
			stuck=true; first_stuck=true;
			IF(lang==_engl) my_t_wrds="6 MOVES TO GET OUT"; END
			IF(lang==_span) my_t_wrds="6 MOVIMIENTOS PARA ALEJARSE"; END
			IF(lang==_germ) my_t_wrds="6 DREHT ZUR AUSSTIEG"; END
			t_cnt=100;
		END

		IF(grid[0].cord[row][col].item==_torch) //torch
			score+=125*multi;
			t_cnt=30;
			my_t_wrds="125";
			IF(multi==2) my_t_wrds+=" X2"; END
			IF(multi==3) my_t_wrds+=" X3"; END
			grid[0].cord[row][col].item=0;
			a_torch(row,col,0);
		END

		IF(grid[0].cord[row][col].item==_gold) //found gold
			score+=50*multi;
			sound_play(s_coin,0);
			grid[0].cord[row][col].item=0;
			t_cnt=30;
			IF(my_t_wrds=="")
			  	my_t_wrds="50";
			  	IF(multi==2) my_t_wrds+=" X2"; END
			  	IF(multi==3) my_t_wrds+=" X3"; END
			END
		END

		IF(grid[0].cord[row][col].item==_double) //double multiplier
			IF(multi<3)
				sound_play(s_multi,0);
				multi=2;
				grid[0].cord[row][col].item=0;
				t_cnt=30;
				my_t_wrds="X2";
			END
		END

		IF(grid[0].cord[row][col].item==_triple) //triple multiplier
			sound_play(s_multi,0);
			multi=3;
			grid[0].cord[row][col].item=0;
			t_cnt=30;
			my_t_wrds="X3";
		END

		IF(grid[0].cord[row][col].item==_prisoner) //prisoner
			IF(lives<3)
				sound_play(s_prison,0);
				newlife=true;
				grid[0].cord[row][col].item=0;
			END
		END

		IF(grid[0].cord[row][col].item==_treasure) //reached treasure chest
			score+=1500*multi; //debug;
			t_cnt=30;
			sound_stop(the_song);
			a=get_id(type music_control); a.selected=true;
			//crnt_song=PLAY_SONG(suc_song,0);
			the_song=sound_play(suc_song,0);
			my_t_wrds="1500";
			IF(multi==2) my_t_wrds+=" X2"; END
			IF(multi==3) my_t_wrds+=" X3"; END
			selected_hero=id;
			set_base_cord();
			signal_action(s_kill_tree,s_ign);
			signal(type mole,s_kill_tree);
			signal(type a_torch,s_kill_tree);
			FOR(a=0;a<90;++a)
				IF(key(_0)) debug; END
				write_move(txt,x,y-40*zoom);
				IF(--t_cnt<0) my_t_wrds=""; END
				IF(++temp[0]>15) temp[0]=0; END
				graph=510+temp[0]/2+30*ident;
				x=(rs[(row-base_row)*zoom])+9*zoom;
				y=cs[(col-base_col)*zoom];
				size=100*zoom;
				FRAME;
			END
			success=true;
			signal_action(s_kill_tree,s_dfl);
			signal(type a_die,s_kill_tree);
			signal(id,s_kill);
		END

		IF(selected_hero==id)
            priority=101;
        ELSE
          	priority=100;
        END

		IF(move_row!=0 OR move_col!=0) //moving
			IF(file!=fpg[0]) file=fpg[0]; END
			IF(move_col!=0)
				IF(move_col<0)
					temp[1]++;
					IF(abs(temp[1])==19)//38)
						temp[1]=0; move_col++; col--;
						IF(grid[0].cord[row][col].back==_trap OR grid[0].cord[row][col-1].item==_closed_door OR !square_is_open(row,col-1))
						 	moves_left+=abs(move_col); move_col=0;
						END
						IF(moves_left==0 AND noones_moving() AND grid[0].cord[row][col].item!=_treasure) change_turn=true; END
					END
				ELSE
					temp[1]--;
					IF(abs(temp[1])==19)//38)
						temp[1]=0; move_col--; col++;
						IF(grid[0].cord[row][col].back==_trap OR grid[0].cord[row][col+1].item==_closed_door OR !square_is_open(row,col+1))
							moves_left+=abs(move_col); move_col=0;
						END
						IF(moves_left==0 AND noones_moving() AND grid[0].cord[row][col].item!=_treasure) change_turn=true; END
					END
				END
			END
			IF(move_row!=0)
				IF(move_row>0)
					temp[2]++;
					flags=1;
					IF(abs(temp[2])==19)//38)
						temp[2]=0; move_row--; row++;
						IF(grid[0].cord[row][col].back==_trap OR grid[0].cord[row+1][col].item==_closed_door OR !square_is_open(row+1,col))
							moves_left+=abs(move_row); move_row=0;
						END
						IF(moves_left==0 AND noones_moving() AND grid[0].cord[row][col].item!=_treasure) change_turn=true; END
					END
				ELSE
					temp[2]--;
					flags=0;
					IF(abs(temp[2])==19)//38)
						temp[2]=0; move_row++; row--;
						IF(grid[0].cord[row][col].back==_trap OR grid[0].cord[row-1][col].item==_closed_door OR !square_is_open(row-1,col))
							moves_left+=abs(move_row); move_row=0;
						END
						IF(moves_left==0 AND noones_moving() AND grid[0].cord[row][col].item!=_treasure) change_turn=true; END
					END
				END
			END

			IF(selected_hero==id)
				set_base_cord();
			END

			IF(row-base_row>=0 AND row-base_row<=11 AND col-base_col>=0 AND col-base_col<=11)
				IF(selected_hero==id)
				  	IF(!( ((col<=base_col+5/zoom OR base_col==20) AND move_col>0) OR ((col>=base_col+(12-6)/zoom OR base_col==0 ) AND move_col<0) ) )
                    	y_shift=-(temp[1]);
                  	ELSE
                    	y_shift=0;
                  	END
                  	IF(!(( (row<=base_row+5/zoom OR base_row==20) AND move_row>0) OR ((row>=base_row+(12-6)/zoom OR base_row==0) and move_row<0)))
                    	x_shift=-(temp[2]);
                  	ELSE
                    	x_shift=0;
                  	END
                END
                y=cs[(col-base_col)*zoom]+temp[1]*zoom+y_shift*zoom;
				x=rs[(row-base_row)*zoom]+(temp[2]+9)*zoom+x_shift*zoom;
			ELSE
				x=-100; y=-100;
			END

			IF(++temp[0] > 7 OR temp[0]<0) temp[0]=0; END
			IF(x>rs[0] AND x<rs[11]+19)
				graph=502+temp[0]+30*ident;
			ELSE
				graph=0;
			END

			z=940-y;
		ELSE //standing still
			IF(selected_hero==id)
				set_base_cord();
                x_shift=0; y_shift=0;
			END
			IF(row>=base_row AND row<base_row+12/zoom AND col>=base_col AND col<base_col+12/zoom)
				IF(stuck)
					flags=0;
					IF(file!=fpg[0]) file=fpg[0]; END
					IF(++graph>523+30*ident OR !first_stuck) graph=523+30*ident; first_stuck=false; END
					x=(rs[(row-base_row)*zoom])+9*zoom+x_shift*zoom;
					was_stuck=true;
				ELSE
					IF(grid[0].cord[row][col].item!=_hider)
						IF(file!=fpg[0]) file=fpg[0]; END
						graph=501+30*ident;
						x=(rs[(row-base_row)*zoom])+9*zoom+x_shift*zoom;
						was_stuck=false;
					ELSE
						IF(file!=fpg[1]) file=fpg[1]; END
						graph=11; flags=0;
						x=(rs[(row-base_row)*zoom])+x_shift*zoom;//x=rs[row-base_row]*zoom;
					END
				END
				y=cs[(col-base_col)*zoom]+y_shift*zoom;
			ELSE //off screen
				graph=0; first_stuck=false;
			END
		END
		FOR(a=0;a<21;++a)
			IF(row-base_row+my_cel[a].row>=0 AND row-base_row+my_cel[a].row<=11/zoom AND col-base_col+my_cel[a].col>=0 AND col-base_col+my_cel[a].col<=11/zoom)
	            back_grid[0].cord[(row-base_row+my_cel[a].row)][(col-base_col+my_cel[a].col)]=grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].back;
				IF(grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].back==_wall OR grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item==_pillar)
					the_map_grid[0].cord[row+my_cel[a].row][col+my_cel[a].col]=1;
				END
				IF(grid[0].cord[ row+my_cel[a].row ][ col+my_cel[a].col ].item!=0) //objects
					IF(grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_start AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_monster AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_mon_path AND grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item !=_torch2)
	                    dis_grid[0].cord[(row-base_row+my_cel[a].row)][(col-base_col+my_cel[a].col)]=grid[0].cord[row+my_cel[a].row][col+my_cel[a].col].item;
	                END
				END
			END

			IF(selected_hero!=id AND exists(monst) AND monst.row==row+my_cel[a].row AND monst.col==col+my_cel[a].col AND who_goes==1)
				IF(!monst.selected AND !exists(type slider)) slider(id,selected_hero); END
	            //selected_hero=id;
			END
			IF(exists(monst) AND monst.row==row+my_cel[a].row AND monst.col==col+my_cel[a].col)
				monst.selected=true;
			END
		END
		last_row=row; last_col=col;
		last_br=base_row; last_bc=base_col;
		z=540-y;
		size=100*zoom;
		the_map_grid[0].cord[row][col]=970+ident;
		FRAME;
	END
ONEXIT
	IF(txt!=0) write_delete(txt); txt=0; END
	FOR(a=0;a<21;++a)
		IF(exists(my_cel[a].ident)) signal(my_cel[a].ident,s_kill); END
		IF(exists(my_cel[a].item)) signal(my_cel[a].item,s_kill); END
	END
END


PROCESS the_monster(ident,row,col);
PRIVATE
	int g_marker;
	int temp[4];
	path my_path;
	int cur_pth;
	int base_flag;
	int readjust;
	int last_selected;
	int hunting_human=false;
	loc gotoo;
	int x_plus;
BEGIN
	file=fpg[1];
	graph=301;
	priority=3;
	IF(ident==5) base_flag=16; END
	x_plus=9;
	FRAME;
	LOOP
		IF(move_row!=0 OR move_col!=0) //moving
			IF(move_row!=0)
				IF(move_row>0)
					temp[2]++;
					flags=1+base_flag;
					IF(abs(temp[2])==19)
						temp[2]=0; move_row--; row++;
						IF(!square_is_open(row+1,col))
							bad_moves+=abs(move_row); move_row=0;
						END
						IF(bad_moves==0 AND move_row==0) cur_pth=0; roll_but(); signal(type a_die,s_kill); END
						IF(bad_moves!=0) readjust=true; END
					END
				ELSE
					temp[2]--;
					flags=0+base_flag;
					IF(abs(temp[2])==19)
						temp[2]=0; move_row++; row--;
						IF(!square_is_open(row-1,col))
							bad_moves+=abs(move_row); move_row=0;
						END
						IF(bad_moves==0 AND move_row==0) cur_pth=0; roll_but(); signal(type a_die,s_kill); END
						IF(bad_moves!=0) readjust=true; END
					END
				END
			END
			IF(move_col!=0)
				IF(move_col<0)
					temp[1]++;
					IF(abs(temp[1])==19)
						temp[1]=0; move_col++; col--;
						IF(!square_is_open(row,col-1))
						 	bad_moves+=abs(move_col); move_col=0;
						END
						IF(bad_moves==0 AND move_col==0) cur_pth=0; roll_but(); signal(type a_die,s_kill); END
						IF(bad_moves!=0) readjust=true; END
					END
				ELSE
					temp[1]--;
					IF(abs(temp[1])==19)
						temp[1]=0; move_col--; col++;
						IF(!square_is_open(row,col+1))
							bad_moves+=abs(move_col); move_col=0;
						END
						IF(bad_moves==0 AND move_col==0) cur_pth=0; roll_but(); signal(type a_die,s_kill); END
						IF(bad_moves!=0) readjust=true; END
					END
				END
			END

			IF(hunting_human==false AND anyone_is_close_to_monster() AND bad_moves>0)
				readjust=true; cur_pth=0;
			END

			IF(selected AND row-base_row>=0 AND row-base_row<=11 AND col-base_col>=0 AND col-base_col<=11)
				y=cs[(col-base_col)*zoom]+temp[1]*zoom+y_shift*zoom;
				x=rs[(row-base_row)*zoom]+(temp[2]+x_plus)*zoom+x_shift*zoom;
			ELSE
				x=-100; y=-100;
			END

			IF(++temp[0] > 15 OR temp[0]<0) temp[0]=0; END
			IF(x>rs[0] AND x<rs[11]+19)
				graph=vil[ident].walk[temp[0]/2];
				IF(!last_selected) sound_play(s_dang,0); END
			ELSE
				graph=0; IF(!last_selected AND selected) sound_play(s_dang,0); END
			END

		ELSE //standing
			IF(selected AND row-base_row>=0 AND row-base_row<=11 AND col-base_col>=0 AND col-base_col<=11) //visible
				IF(++g_marker<0 OR g_marker>38 OR vil[ident].stand[g_marker/2]==-1) g_marker=0; END
				graph=vil[ident].stand[g_marker/2];
				x=rs[(row-base_row)*zoom]+x_plus*zoom+x_shift*zoom;
				y=cs[(col-base_col)*zoom]+y_shift*zoom;
			ELSE
				x=-100;
				y=-100;
			END
			IF(selected AND !last_selected) sound_play(s_dang,0); END
			IF(bad_moves>0) readjust=true; END
		END

		WHILE(temp[4]=get_id(type hero))
			IF(temp[4].col==col AND temp[4].row==row)
				g_marker=0; z=540-y-5;
				sound_play(s_dead,0);
				WHILE(++g_marker<39 AND vil[ident].kill[g_marker/2]!=-1)
					graph=vil[ident].kill[g_marker/2];
					size=100*zoom;
					x=rs[(row-base_row)*zoom]+x_plus*zoom;
					y=cs[(col-base_col)*zoom];
					FRAME;
				END
				g_marker=0;
				WHILE(++g_marker<39 AND vil[ident].kill[g_marker/2]!=-1)
					graph=vil[ident].kill[g_marker/2];
					size=100*zoom;
					x=rs[(row-base_row)*zoom]+x_plus*zoom;
					y=cs[(col-base_col)*zoom];
					FRAME;
				END
				lives--;
				signal(temp[4],s_kill);
			END
		END

		IF(readjust) //decide where to move
			readjust=false;
			IF(cur_pth<=0 AND exists(type hero))
				IF(os_id==OS_GP2X OR os_id==OS_GP2X_WIZ OR os_id==OS_CAANOO) monster_think(); END
				FRAME;
				temp[4]=closest_hero();
				clear_path(my_path);
				IF(temp[4]!=0 AND anyone_is_close_to_monster())
					cur_pth=find_path(row,col,temp[4].row,temp[4].col,my_path);
					hunting_human=true;
				ELSE
					get_mon_path(gotoo);
					WHILE(abs(gotoo.row-row)+abs(gotoo.col-col)<4)
						get_mon_path(gotoo);
					END
					cur_pth=find_path(row,col,gotoo.row,gotoo.col,my_path);
					hunting_human=false;
				END
			END
			cur_pth--;
			bad_moves--;
			IF(row==my_path.row[cur_pth])
				IF(col<my_path.col[cur_pth]) move_col=1; ELSE move_col=-1; END
			ELSE
				IF(row<my_path.row[cur_pth]) move_row=1; ELSE move_row=-1; END
			END
		END

		z=540-y;
		last_selected=selected;
		selected=false;
		size=100*zoom;
		FRAME;
	END
END



PROCESS display_controller();
PRIVATE
  	STRUCT comp_grid[0]
	    int cord[11][11];
	END

  	STRUCT comb_grid[0]
       	int cord[11][11];
  	END

  	int reset[11];
  	int reset_bk;
BEGIN
  	file=fpg[0];
  	graph=700;
  	map_clear(fpg[0],700,0);

  	LOOP
    	x=rs[0]+x_shift*zoom; y=cs[0]+y_shift*zoom;
   		z=1000-y;
    	size=100*zoom;
    	FOR(b=0;b<12;++b) reset[b]=false; END
    	reset_bk=false;

    	FOR(a=0;a<12;++a)
      		FOR(b=0;b<12;++b)
	        	IF(comp_grid[0].cord[a][b]!=dis_grid[0].cord[a][b])
	        		reset[b]=true;
	        	END

	        	comp_grid[0].cord[a][b]=dis_grid[0].cord[a][b];
	        	dis_grid[0].cord[a][b]=0;

	        	IF(comb_grid[0].cord[a][b]!=back_grid[0].cord[a][b])
	        		reset_bk=true;
	        	END

	        	comb_grid[0].cord[a][b]=back_grid[0].cord[a][b];
	        	back_grid[0].cord[a][b]=0;
	    	END
    	END

    	FOR(b=0;b<12;++b)
      		IF(reset[b])
        		map_clear(fpg[0],701+b,0);
        		FOR(a=0;a<12;++a)
          			IF(comp_grid[0].cord[a][b]!=0)
            			map_put(fpg[0],701+b,get_file(comp_grid[0].cord[a][b]),comp_grid[0].cord[a][b],rs[a]-3,44,0,100,100,0,255,255,255,255);
          				// map_xputnp(fpg[0],701+b,fpg[0],601,rs[a]-3,44,0,100,100,0);
         			END
        		END
      		END
    	END

    	IF(reset_bk)
      		map_clear(fpg[0],700,0);
      		FOR(a=0;a<12;++a)
        		FOR(b=0;b<12;++b)
          			IF(comb_grid[0].cord[a][b]!=0)
            			map_put(fpg[0],700,fpg[1],comb_grid[0].cord[a][b],rs[a]-3,cs[b]-10,0,100,100,0,255,255,255,255);
          			END
        		END
      		END
    	END

    	FRAME;
  	END
END

PROCESS a_die(double x,y, int tipe);
PRIVATE
	int base_graph;
BEGIN
	z=-200;
    file=fpg[0];
	IF(tipe==0) base_graph=111; ELSE base_graph=101; who_goes=1; END

	FOR(a=0;a<30;++a)
		d=rand(1,6);
		graph=base_graph+d-1;
		FRAME;
	END
	IF(tipe==0)
		moves_left+=d;
	ELSE
		bad_moves+=d;
	END
	LOOP
		d++;
		IF(moves_left<6 AND every_body_is_trapped() AND tipe==0 AND d>36) moves_left=0; change_turn=true; END
		FRAME;
	END
END


PROCESS roll_but();
PRIVATE
	wrd_set roll;
	STRING outp;
	int txt;
	int cnt=120;
	int pulsate;
	int p_dir;
BEGIN
	roll.txt[_engl]="ROLL"; roll.txt[_span]="TIRAR"; roll.txt[_germ]="ROLLE";
	z=-200;
	file=fpg[0];
	graph=10;
	x=275; y=52;

	text.z=-256; txt=write_var(fnt[0],x,y,ALIGN_CENTER,outp);
	FRAME;
	priority=1000;
	who_goes=0;

	LOOP
		IF(--cnt<0)
			pulsate=true;
		END

		graph=10; outp=roll.txt[lang];
		IF(mouse.left OR jkeys_state[ _JKEY_A ])
			IF(collision(type mouse) OR jkeys_state[ _JKEY_A ])
				graph=11; cnt=120; size=100; pulsate=false;
			END
		END

		IF(pulsate)
			IF(p_dir==0)
				IF(size<=105) size++; ELSE p_dir=1; END
			ELSE
				IF(size>=100) size--; ELSE p_dir=0; END
			END
		END

		IF(mouse_release OR jkeys_state[ _JKEY_A ])
			IF(collision(type mouse) OR jkeys_state[ _JKEY_A ])
				sound_play(s_click2,0);
				a_die(x-40,y+20,0);
				a_die(x+2,y+20,0);
				IF(txt!=0) write_delete(txt); txt=0; END
				BREAK;
			END
		END
		FRAME;
	END
ONEXIT
	IF(txt!=0) write_delete(txt); txt=0; END
END


PROCESS zoom_but();
BEGIN
	file=fpg[0];
	graph=9;
	z=-200;
	x=277; y=199;
	priority=1000;
	LOOP
		control--;
		IF(mouse_release OR (jkeys_state[ _JKEY_R ] AND control<0))
			IF(collision(type mouse) OR (jkeys_state[ _JKEY_R ] AND control<0))
				control=15;
				sound_play(s_click2,0);
				IF(zoom==1) zoom=2; ELSE zoom=1; END
			END
		END
		FRAME;
	END
END


PROCESS map_but();
BEGIN
	file=fpg[0];
	graph=14;
	x=237; y=237;
	z=-200;
	priority=1000;
	LOOP
		control--;
		IF(mouse_release OR (jkeys_state[ _JKEY_SELECT ] AND control<0))
			IF(collision(type mouse) OR (jkeys_state[ _JKEY_SELECT ] AND control<0))
				sound_play(s_click2,0);
				control=15;
				IF(exists(the_map))
					the_map.selected=true;
				ELSE
					the_map=mapp(0);
				END
			END
		END

		IF(exists(the_map))
			graph=18;
		ELSE
			graph=14;
		END
		FRAME;
	END
END


PROCESS gamemenu_but();
PRIVATE
 	int men;
BEGIN
	file=fpg[0];
	graph=15;
	x=237; y=199;
	z=-200;
	priority=1000;
	signal_action(s_freeze_tree,s_ign);
	LOOP
		control--;
		IF( ( ( mouse_release OR ( jkeys_state[ _JKEY_MENU ] AND control<0 ) ) AND !exists(type keyy) AND !exists(type card_game) AND !exists(type mapp)) )
			IF(collision(type mouse) OR jkeys_state[ _JKEY_MENU ])
				sound_play(s_click2,0);
				control=15;
				IF(exists(men))
					menu_interp(13);
				ELSE
					signal(type mole,s_freeze_tree);
					kill_menus=true;
					menu_but(0,14); // IN GAME OPTIONS
            		menu_but(1,15); // EXIT TO MAIN MENU 
            		menu_but(2,1); // EXIT
					men=menu_but(3,13); men.selected=true;
            	END
			END
		END
		FRAME;
	END
END

PROCESS key_but();
PRIVATE
 	int men;
BEGIN
	file=fpg[0];
	graph=21;
	x=277; y=237;
	z=-200;
	priority=1000;
	signal_action(s_freeze_tree,s_ign);
	LOOP
		control--;
		IF((mouse_release OR (jkeys_state[ _JKEY_B ] AND control<0)) AND !exists(type menu_but) AND !exists(type card_game) AND !exists(type mapp))
			IF(collision(type mouse) OR (jkeys_state[ _JKEY_B ] AND control<0))
				control=15;
				sound_play(s_click2,0);
				IF(exists(type keyy))
					kill_menus=true;
					signal(type mole,s_wakeup_tree);
					graph=21;
				ELSE
					signal(type mole,s_freeze_tree);
					keyy();
					graph=22;
				END
			END
		END
		FRAME;
	END
END

PROCESS hero_selector_but(ident);
PRIVATE
	int base_graph;
BEGIN
	file=fpg[0];
	base_graph=ident+4;
	IF(lives>ident)

		graph=base_graph;
		WHILE(a=get_id(type hero))
			IF(a.ident==ident)
				control=a; BREAK;
			END
		END
	ELSE
		graph=7;
	END

	z=-200;
	x=240+ident*25; y=105;

	LOOP
		IF(!exists(control)) graph=7; IF(selected_hero==control) d=get_id(type hero); slider(d,selected_hero); END control=-1; ELSE graph=base_graph; END

		IF(selected)
			selected=false;
			WHILE(a=get_id(type hero))
				IF(a.ident==ident)
					control=a; BREAK;
				END
			END
		END

		IF(graph!=7 AND mouse_release)
			IF(collision(type mouse))
				sound_play(s_click2,0);
				slider(control,selected_hero);
				//selected_hero=control;
			END
		END
		IF(selected_hero==control) flags=0; ELSE flags=4; END
		FRAME;
	END
END


PROCESS hero_highlighter();
BEGIN
	file=fpg[0];
	z=999;
	FRAME;
	graph=997;

	LOOP
		IF(exists(selected_hero))
			x=rs[(selected_hero.row-base_row)*zoom]+x_shift*zoom;
			y=cs[(selected_hero.col-base_col)*zoom]+y_shift*zoom;
		END
		size=100*zoom;
		FRAME;
	END
END


PROCESS mover_arrow(dir);
PRIVATE
	int r_ad, c_ad;
	int base_graph;
	int waste[3];
	int butt;
	int oktogo;
BEGIN
	file=fpg[0];
	//z=-50;

	SWITCH(dir)
		CASE 0: base_graph=992; c_ad=1; butt=_JKEY_UP; END
		CASE 1: base_graph=993; c_ad=-1; butt=_JKEY_DOWN; END
		CASE 2: base_graph=994; r_ad=-1; butt=_JKEY_LEFT; END
		CASE 3: base_graph=995; r_ad=1; butt=_JKEY_RIGHT; END
	END
	graph=base_graph;
	LOOP
		oktogo--;
		IF(exists(type move_indicator)) oktogo=8; END

		IF(exists(selected_hero))
			IF(selected_hero.ident!=_torch AND selected_hero.ident!=_slider AND (!(selected_hero.stuck AND moves_left<6) AND moves_left>0 AND square_is_open(selected_hero.row +r_ad,selected_hero.col +c_ad) AND (selected_hero.move_row==0 AND selected_hero.move_col==0)))
				IF(grid[0].cord[selected_hero.row +r_ad][selected_hero.col +c_ad].item==_closed_door) //door
					graph=base_graph-18;
				ELSE
					graph=base_graph;
				END
				z=selected_hero.z+(c_ad*19)-20;
				IF(dir==0) z+=19; END
				x=rs[(selected_hero.row-base_row +r_ad)*zoom ];
				y=cs[(selected_hero.col-base_col +c_ad)*zoom ];
				row=selected_hero.row-base_row +r_ad;
				col=selected_hero.col-base_col +c_ad;
			ELSE
					graph=0;
			END

			IF((mouse.left OR jkeys_state[butt]) AND graph!=0 AND oktogo<0)
				IF((get_mouse_row()==row AND get_mouse_col()==col) OR jkeys_state[butt])
					IF(graph==base_graph)
						IF(!exists(type move_indicator))
							move_indicator(dir,row,col);
						END
					ELSE
						card_game(row+base_row,col+base_col);
					END
				END
			END
		END
		size=100*zoom;
		FRAME;
	END
END


PROCESS move_indicator(tipe,init_row,init_col);
PRIVATE
	int s;
	int l_x,l_y;
	int mouse_was;
	int home;
	int skip;
BEGIN
	file=fpg[0];
	graph=991;
	flags=4;
	z=0;
	ident=tipe;
	row=init_row; col=init_col;
	IF(mouse.left) mouse_was=true; END
	IF(selected_hero.stuck) s=5; ELSE s=0; END

	control=6;

	WHILE(!home AND ((mouse.left AND mouse_was) OR (!jkeys_state[ _JKEY_A ] AND !mouse_was)))
		IF(jkeys_state[ _JKEY_Y] OR jkeys_state[ _JKEY_X])
			skip=true; BREAK;
		END

		SWITCH(tipe)
			CASE 0:
				control--;
				IF(mouse.left)
					col=get_mouse_col();
				ELSE
					IF(jkeys_state[ _JKEY_UP ] AND control<0)
						control=3; col++; IF(col>11/zoom) col=11/zoom; END
					END
					IF(jkeys_state[ _JKEY_DOWN ] AND control<0)
						control=3; col--;
					END
				END
				IF(col<=init_col-1) col=init_col-1; home=true; END
				IF(abs(col-(init_col-1))>moves_left-s) col=init_col-1+(moves_left-(s)); END
				row=init_row;
			END
			CASE 1:
				control--;
				IF(mouse.left)
					col=get_mouse_col();
				ELSE
					IF(jkeys_state[ _JKEY_UP ] AND control<0)
						control=3; col++;
					END
					IF(jkeys_state[ _JKEY_DOWN ] AND control<0)
						control=3; col--; IF(col<0) col=0; END
					END
				END
				IF(col>=init_col+1 OR col==-1) col=init_col+1; home=true; END
				IF(abs(col-(init_col+1))>moves_left-s) col=init_col+1-(moves_left-(s)); END
				row=init_row;
			END
			CASE 2:
				control--;
				IF(mouse.left)
					row=get_mouse_row();
				ELSE
					IF(jkeys_state[ _JKEY_RIGHT ] AND control<0)
						control=3; row++; IF(row>11/zoom) row=11/zoom; END
					END
					IF(jkeys_state[ _JKEY_LEFT ] AND control<0)
						control=3; row--; IF(row<0) row=0; END
					END
				END
				IF(row>=init_row+1 OR row==-1) row=init_row+1; home=true; END
				IF(abs(row-(init_row+1))>moves_left-s) row=init_row+1-(moves_left-(s)); END
				col=init_col;
			END
			CASE 3:
				control--;
				IF(mouse.left)
					row=get_mouse_row();
				ELSE
					IF(jkeys_state[ _JKEY_RIGHT ] AND control<0)
						control=3; row++; IF(row>11/zoom) row=11/zoom; END
					END
					IF(jkeys_state[ _JKEY_LEFT ] AND control<0)
						control=3; row--; IF(row<0) row=0; END
					END
				END
				IF(row<=init_row-1) row=init_row-1; home=true; END
				IF(abs(row-(init_row-1))>moves_left-s) row=init_row-1+(moves_left-(s)); END
				col=init_col;
			END
		END
		x=rs[row*zoom];
		y=cs[col*zoom];
		IF(x!=l_x OR y!=l_y) sound_play(s_click,0); END
		l_x=x;
		l_y=y;
		z=541-y;
		size=100*zoom;
		FRAME;
	END

	//release of mouse
	IF(exists(selected_hero) AND !skip)
		IF(s==5) moves_left-=5; selected_hero.stuck=false; END
		SWITCH(tipe)
			CASE 0:
				selected_hero.move_col= col-(init_col-1) ;
				moves_left-=abs(selected_hero.move_col);
			END
			CASE 1:
				selected_hero.move_col= col-(init_col+1) ;
				moves_left-=abs(selected_hero.move_col);
			END
			CASE 2:
				selected_hero.move_row= row-(init_row+1) ;
				moves_left-=abs(selected_hero.move_row);
			END
			CASE 3:
				selected_hero.move_row= row-(init_row-1) ;
				moves_left-=abs(selected_hero.move_row);
			END
		END
	END
END


PROCESS music_control();
PRIVATE
	int gotmenu, currentsong;
	int newsong=-1;
BEGIN
	//SET_SONG_VOLUME(volume_level);

   	control=volume_level;
   	currentsong=rand(0,2);
   	z=-300;
   	x=320/2; y=220;
   	graph=27; FRAME;
   	SWITCH(currentsong)
     	CASE 0:
       		song[0]=sound_load("files/monster2.mp3");
     	END
     	CASE 1:
       		song[1]=sound_load("files/monster.mp3");
     	END
     	CASE 2:
       		song[2]=sound_load("files/monster3.mp3");
     	END
   	END
   	graph=0;
   	the_song=sound_play(song[currentsong],0);
   	FRAME;
	LOOP
		IF(--newsong==0)
			sound_unload(song[currentsong]);
			currentsong++; IF(currentsong>=3) currentsong=0; END
			graph=27; FRAME;
			SWITCH(currentsong)
				CASE 0:
					song[0]=sound_load("files/monster2.mp3");
				END
				CASE 1:
					song[1]=sound_load("files/monster.mp3");
				END
				CASE 2:
					song[2]=sound_load("files/monster3.mp3");
				END
			END
			the_song=sound_play(song[currentsong],0);
			FRAME; graph=0;
		END

        //IF(key(_4)) sound_stop(the_song); END
		IF((!sound_is_playing(the_song) and newsong<0) OR selected)
            newsong=rand(140,190);
		END
		selected=false;

		FRAME;
	END
END
