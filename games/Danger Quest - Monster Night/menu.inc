PROCESS menu_overhead();
PRIVATE
	wrd_set tit1;
	wrd_set tit2;
	string outp[1];

	int txt[1];
BEGIN
	priority=100; //must be greater than buttons to kill buttons correctly

	tit1.txt[_engl]="DANGER  QUEST: "; tit1.txt[_span]="BUSQUEDA  PELIGROSA: "; tit1.txt[_germ]="BEDROHUNG  QUEST: ";
	tit2.txt[_engl]="MONSTER  NIGHT"; tit2.txt[_span]="NOCHE  MONSTRUOSA"; tit2.txt[_germ]="MONSTER  NACHT";
	file=fpg[0];
	graph=3;
	x=0; y=239; z=1000;
	a=25; b=50;
	txt[0]=write_var(fnt[0],20,22+a*15,ALIGN_CENTER_LEFT,outp[0]);
	txt[1]=write_var(fnt[0],20+b*20,40,ALIGN_CENTER_LEFT,outp[1]);

	FRAME; FRAME; FRAME; FRAME;
	WHILE(b>0)
		IF(a>0) --a;
			write_move(txt[0],20,22+a*15);
		END
		IF(b>0) --b;
			write_move(txt[1],20+b*20,40);
		END
		outp[0]=tit1.txt[lang]; outp[1]=tit2.txt[lang];
		FRAME;
	END

	//call basic menu buttons
	menu_interp(7);
	LOOP
		outp[0]=tit1.txt[lang]; outp[1]=tit2.txt[lang];

		IF(start_game)
			start_game=false;
			FOR(a=0;a<2;++a) write_delete(txt[a]); END
			game();
			BREAK;
		END
		FRAME;
	END
END


PROCESS menu_but(level,ident);
PRIVATE
	wrd_set do_this;
	int txt;
	string outp;
	int allmen[5];
	int totmen;
	int mymen;
BEGIN
	file=fpg[0];
	graph=12;
	x=320/2;
	y=90+25*level;
	z=-250;

	signal_action(s_kill_tree,s_ign);

	text.z=-256;

	set_do_this(ident,do_this);
	outp=do_this.txt[lang];
	txt=write_var(fnt[0],x,y,ALIGN_CENTER,outp);
	FRAME; FRAME;

	WHILE(allmen[totmen]=get_id(type menu_but))
		IF(allmen[totmen]==id) mymen=totmen; END
		totmen++;
	END
	totmen--; control=5;
	kill_menus=false;

	LOOP

		control--;
		graph=12;
		IF(mouse.left)
			IF(collision(type mouse))
				WHILE(a=get_id(type menu_but)) a.selected=false; END
				selected=true;
			END
		END

		IF(selected)
			graph=13;
			IF(jkeys_state[ _JKEY_UP ] AND control<0 AND totmen>1)
				a=mymen+1; IF(a>totmen) a=0; END
				allmen[a].selected=true; selected=false;
				allmen[a].control=5;
			END
			IF(jkeys_state[ _JKEY_DOWN ] AND control<0 AND totmen>1)
				a=mymen-1; IF(a<0) a=totmen; END
				allmen[a].selected=true; selected=false;
				allmen[a].control=5;
			END
		END

		outp=do_this.txt[lang];

		IF(mouse_release OR ( ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND selected AND control<0))
			IF(collision(type mouse) OR ( ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND selected AND control<0))
				sound_play(s_click2,0);
				menu_interp(ident);
				control=10;
			END
		END

		IF(kill_menus) write_delete(txt); BREAK; END
                         
		FRAME;
	END
END

/*
PROCESS banner();
BEGIN
  file=fpg[0];
  graph=26;
  z=-257;
  x=160;
  y=118;
  LOOP
    FRAME;
  END
END
*/

PROCESS set_do_this(ident,wrd_set do_this);
BEGIN
	SWITCH(ident)
		CASE 0:
			do_this.txt[_engl]="NEW GAME"; do_this.txt[_span]="NUEVO JUEGO"; do_this.txt[_germ]="NEUES SPIEL";
		END
		CASE 1:
			do_this.txt[_engl]="EXIT"; do_this.txt[_span]="SALIR"; do_this.txt[_germ]="AUSFAHRT";
		END
		CASE 2:
			do_this.txt[_engl]="OPTIONS"; do_this.txt[_span]="OPCIONES"; do_this.txt[_germ]="OPTIONEN";
		END
		CASE 3:
			do_this.txt[_engl]="LANGUAGE"; do_this.txt[_span]="IDIOMA"; do_this.txt[_germ]="SPRACHLICH";
		END
		CASE 4:
			do_this.txt[_engl]="ENGLISH"; do_this.txt[_span]="ENGLISH"; do_this.txt[_germ]="ENGLISH";
		END
		CASE 5:
			do_this.txt[_engl]="ESPANOL"; do_this.txt[_span]="ESPANOL"; do_this.txt[_germ]="ESPANOL";
		END
		CASE 6:
			do_this.txt[_engl]="DEUTSCHE"; do_this.txt[_span]="DEUTSCHE"; do_this.txt[_germ]="DEUTSCHE";
		END
		CASE 7:
			do_this.txt[_engl]="BACK"; do_this.txt[_span]="VOLVER"; do_this.txt[_germ]="RUCKKEHR";
		END
		CASE 8:
			do_this.txt[_engl]="BACK"; do_this.txt[_span]="VOLVER"; do_this.txt[_germ]="RUCKKEHR";
		END
		CASE 9:
			do_this.txt[_engl]="ENGLISH"; do_this.txt[_span]="ENGLISH"; do_this.txt[_germ]="ENGLISH";
		END
		CASE 10:
			do_this.txt[_engl]="ESPANOL"; do_this.txt[_span]="ESPANOL"; do_this.txt[_germ]="ESPANOL";
		END
		CASE 11:
			do_this.txt[_engl]="DEUTSCHE"; do_this.txt[_span]="DEUTSCHE"; do_this.txt[_germ]="DEUTSCHE";
		END
		CASE 12:
			do_this.txt[_engl]="HIGHSCORES"; do_this.txt[_span]="TABLA DE RECORDS"; do_this.txt[_germ]="HIGHSCORES";
		END
		CASE 13:
			do_this.txt[_engl]="BACK"; do_this.txt[_span]="VOLVER"; do_this.txt[_germ]="RUCKKEHR";
		END
		CASE 14:
			do_this.txt[_engl]="OPTIONS"; do_this.txt[_span]="OPCIONES"; do_this.txt[_germ]="OPTIONEN";
		END
		CASE 15:
		    do_this.txt[_engl]="MAIN MENU"; do_this.txt[_span]="MENU PRINCIPAL"; do_this.txt[_germ]="HAUPTMENU";
		END
		CASE 16:
			do_this.txt[_engl]="LANGUAGE"; do_this.txt[_span]="IDIOMA"; do_this.txt[_germ]="SPRACHLICH";
		END
		CASE 17:
			do_this.txt[_engl]="BACK"; do_this.txt[_span]="VOLVER"; do_this.txt[_germ]="RUCKKEHR";
		END
		CASE 18:
			do_this.txt[_engl]="VOLUME"; do_this.txt[_span]="VOLUMEN"; do_this.txt[_germ]="VOLUMEN";
		END
		CASE 19:
			do_this.txt[_engl]="VOLUME"; do_this.txt[_span]="VOLUMEN"; do_this.txt[_germ]="VOLUMEN";
		END
		CASE 20:
		    do_this.txt[_engl]="RESUME"; do_this.txt[_span]="VOLVER"; do_this.txt[_germ]="RUCKKEHR";
		END
	END
END


PROCESS menu_interp(ident);
BEGIN
	SWITCH(ident)
		CASE 0: //NEW GAME
			kill_menus=true;
			start_game=true;
		END
		CASE 1: //EXIT PROGRAM
			exit();
		END
		CASE 2: //OPTIONS
			kill_menus=true;
			a=menu_but(0,3); a.selected=true;//LANGUAGE
			menu_but(1,18); //VOLUME
			menu_but(2,7); //RETURN TO BASIC MENU
		END
		CASE 3: //LANGUAGE
			kill_menus=true;
			a=menu_but(0,4); a.selected=true;//ENGLISH
			menu_but(1,5); //SPANISH
			menu_but(2,6); //GERMAN
			menu_but(3,8); //RETURN TO OPTIONS
		END
		CASE 4:
			lang=_engl;
			save_prof();
		END
		CASE 5:
			lang=_span;
			save_prof();
		END
		CASE 6:
			lang=_germ;
			save_prof();
		END
		CASE 7: //BACK
			kill_menus=true;
			a=menu_but(0,0); a.selected=true;//NEW_GAME
			menu_but(1,2); //OPTIONS
			menu_but(2,12);//HIGHSCORES
			menu_but(3,1); //EXIT
		END
		CASE 8: //BACK
			save_prof();
			menu_interp(2);
		END
		CASE 9: //INITIAL PROFILE ENGLISH
			kill_menus=true;
			lang=_engl;
			save_prof();
			menu_overhead();
		END
		CASE 10: //INITIAL PROFILE SPANISH
			kill_menus=true;
			lang=_span;
			save_prof();
			menu_overhead();
		END
		CASE 11: //INITIAL PROFILE GERMAN
			kill_menus=true;
			lang=_germ;
			save_prof();
			menu_overhead();
		END
		CASE 12: //HIGHSCORES
			kill_menus=true;
			highscore_display();
			a=menu_but(4,7); a.selected=true;//RETURN TO BASIC MENU
		END
		CASE 13: //BACK TO GAME
			kill_menus=true;
			signal(type mole,s_wakeup_tree);
		END
		CASE 14: //IN GAME OPTIONS
			kill_menus=true;
			save_prof();
			a=menu_but(0,16); a.selected=true;//LANGUAGE
			menu_but(1,19); //VOLUME
			menu_but(2,17); //RETURN TO BASIC IN GAME MENU
		END
		CASE 15:  //EXIT TO MAIN MENU
			if(exists(father) AND exists(father.father))
		    	father.father.selected=true;
		    end
            a=get_id(type game);
			IF(exists(a))
				a.selected=true;
                write_delete(all_text);
            END
		END
		CASE 16://LANGUAGE MENU IN GAME
			kill_menus=true;
			a=menu_but(0,4); a.selected=true;//ENGLISH
			menu_but(1,5); //SPANISH
			menu_but(2,6); //GERMAN
			menu_but(3,14); //RETURN TO IN GAME OPTIONS
		END
		CASE 17: //BACK TO IN GAME MENU
			kill_menus=true;
			menu_but(0,14); // IN GAME OPTIONS
    		menu_but(1,15); // EXIT TO MAIN MENU 
    		menu_but(2,1); // EXIT
			a=menu_but(3,13); a.selected=true;
		END
		CASE 18: //VOLUME CONTROL
			kill_menus=true;
			volume_control();
			volume_knob();
			a=menu_but(3,8); a.selected=true;
		END
		CASE 19: //VOLUME CONTROL IN GAME
			kill_menus=true;
			volume_control();
			volume_knob();
			a=menu_but(3,14); a.selected=true;
		END
	END
END


PROCESS highscore_display();
PRIVATE
	int txt[9][1];
BEGIN
	FOR(a=0;a<10;++a)
		txt[a][0]=write(0,170,70+a*10,ALIGN_CENTER_RIGHT,profile[0].hiscore[a].name);
		txt[a][1]=write(0,180,70+a*10,ALIGN_CENTER_LEFT,profile[0].hiscore[a].val);
	END

	FRAME; FRAME;
	FRAME; FRAME;
	LOOP
		IF(kill_menus)
			FOR(a=0;a<10;++a)
				write_delete(txt[a][0]);
				write_delete(txt[a][1]);
			END
			BREAK;
		END
		FRAME;
	END
END

PROCESS volume_control();
BEGIN
	file=fpg[0];
	graph=16;
	x=160;
	y=100;
	z=-450;
	FRAME; FRAME;
	FRAME; FRAME;
	kill_menus=false;
	LOOP
		IF(mouse.left)
			IF(collision(type mouse))
				volume_level=(mouse.x-58);
				IF(volume_level>203) volume_level=203; END
				IF(volume_level<0) volume_level=0; END
				set_all_sound();
				//SET_SONG_VOLUME(volume_level);
			END
		END

		IF(jkeys_state[ _JKEY_RIGHT ])
			volume_level+=a;
			a++;
			IF(volume_level>203) volume_level=203; END
			set_all_sound();
		ELSE a=1; END

		IF(jkeys_state[ _JKEY_LEFT ])
			volume_level-=b;
			b++;
			IF(volume_level<0) volume_level=0; END
			set_all_sound();
		ELSE b=1; END

		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END


PROCESS volume_knob();
BEGIN
	file=fpg[0];
	graph=17;
	x=58+volume_level;
	y=100;
	z=-451;
	FRAME; FRAME;
	FRAME; FRAME;
	kill_menus=false;
	LOOP
		x=58+volume_level;

		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END

PROCESS volume_knob2();
BEGIN
	file=fpg[0];
	graph=17;
	x=58+volume_level;
	y=220;
	z=-451;

	FRAME;
END
