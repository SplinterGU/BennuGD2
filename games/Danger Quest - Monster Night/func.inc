PROCESS get_start(tipe,num);
BEGIN

	//debug;
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_start)
				IF(c==num)
					IF(tipe==0) d=a; ELSE d=b; END RETURN(d);
				ELSE
					c++; //debug;
				END

			END
		END
	END

END

PROCESS get_max_mon_strt();
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_monster)
				d++;
			END
		END
	END
	max_mon_strt=d-1;
END


PROCESS get_monster_start(tipe,control);
BEGIN

	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_monster)
				IF(c==control)
					IF(tipe==0) d=a; ELSE d=b; END RETURN(d);
				END
				c++;
			END
		END
	END
END

FUNCTION furthest_monster_start();
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_monster)
				IF(abs(monst.row-a)+abs(monst.col-b)>a)
					d=c; a=abs(monst.row-a)+abs(monst.col-b);
				END
				c++;
			END
		END
	END

	 RETURN(d);
END

FUNCTION get_mon_path(loc gotoo);
PRIVATE
	loc h[9];
	loc closest;
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_mon_path)
				h[c].row=a; h[c].col=b;
				c++;
			END
		END
	END
	d=rand(0,c-1);
	gotoo.row=h[d].row; gotoo.col=h[d].col;
END


PROCESS square_is_open(row,col);
PRIVATE
	int answer=true;
BEGIN
	IF(grid[0].cord[row][col].back==_wall OR grid[0].cord[row][col].item==_pillar)
		answer=false;
	END
	IF(monst.row==row AND monst.col==col)
		answer=false;
	END
	WHILE(control=get_id(type hero))
		IF(control!=father)
			IF(((row<=control.row AND row>=control.row+control.move_row) OR  (row>=control.row AND row<=control.row+control.move_row))AND
			((col<=control.col AND col>=control.col+control.move_col) OR  (col>=control.col AND col<=control.col+control.move_col)))
				answer=false;
			END
		END
	END

	RETURN(answer);
END


PROCESS get_mouse_row();
PRIVATE
	int _rs[12]=3,22,41,60,79,98,117,136,155,174,193,212,231;
BEGIN
	d=-1;
	FOR(a=0;a<12/zoom;++a)
		IF(mouse.x>=_rs[a*zoom] AND mouse.x<_rs[(a+1)*zoom])
			d=a;
		END
	END
	RETURN(d);
END

PROCESS get_mouse_col();
PRIVATE
	int _cs[12]=236,217,198,179,160,141,122,103,84,65,46,27,8;
BEGIN
	d=-1;
	FOR(a=0;a<12/zoom;++a)
		IF(mouse.y<=_cs[a*zoom] AND mouse.y>_cs[(a+1)*zoom])
			d=a;
		END
	END
	RETURN(d);
END

PROCESS noones_moving();
PRIVATE
	int answer=true;
BEGIN
	WHILE(a=get_id(type hero))
		IF(a.move_row!=0 OR a.move_col!=0)
			answer=false;
		END
	END
	RETURN(answer);
END


PROCESS closest_hero();
BEGIN
	WHILE(a=get_id(type hero))
		IF(grid[0].cord[a.row][a.col].item!=_hider)
			IF(b==0)
				b=a; c=abs(monst.row-a.row)+abs(monst.col-a.col);
			END
			IF(abs(monst.row-a.row)+abs(monst.col-a.col)<c)
				b=a; c=abs(monst.row-a.row)+abs(monst.col-a.col);
			END
		END
	END
	RETURN(b);
END


FUNCTION set_torch2();
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_torch2)
				a_torch(a,b,1);
			END
		END
	END
END


PROCESS find_path(i_r,i_c,e_r,e_c,path my_path);
PRIVATE
	int pth=0;
	STRUCT spot[31][31];
		int par_r;
		int par_c;
		int g;
		int h;
		int stat;
	END
	int cur_c;
	int cur_r;
	int lowest_f;
	int f_r,f_c;
BEGIN

	cur_r=i_r;
	cur_c=i_c;

	WHILE(spot[e_r][e_c].stat!=2)
		spot[cur_r][cur_c].stat=2;	//close current spot

		a=cur_r-1; b=cur_c;
		IF((baddy==7 OR (grid[0].cord[a][b].back!=_wall AND grid[0].cord[a][b].item!=_pillar)) AND spot[a][b].stat!=2 AND a>=0 AND a<=31 AND b>=0 AND b<=31 AND !hiding_spot_occ(a,b)) //if adj spot is not barrier or spot closed
			spot[a][b].stat=1; spot[a][b].g=spot[cur_r][cur_c].g+10; spot[a][b].h=abs(a-i_r)+abs(b-i_c)*10;
			spot[a][b].par_r=cur_r; spot[a][b].par_c=cur_c;
		END

		a=cur_r+1; b=cur_c;
		IF((baddy==7 OR (grid[0].cord[a][b].back!=_wall AND grid[0].cord[a][b].item!=_pillar)) AND spot[a][b].stat!=2 AND a>=0 AND a<=31 AND b>=0 AND b<=31 AND !hiding_spot_occ(a,b)) //if adj spot is not barrier or spot closed
			spot[a][b].stat=1; spot[a][b].g=spot[cur_r][cur_c].g+10; spot[a][b].h=abs(a-i_r)+abs(b-i_c)*10;
			spot[a][b].par_r=cur_r; spot[a][b].par_c=cur_c;
		END

		a=cur_r; b=cur_c-1;
		IF((baddy==7 OR (grid[0].cord[a][b].back!=_wall AND grid[0].cord[a][b].item!=_pillar)) AND spot[a][b].stat!=2 AND a>=0 AND a<=31 AND b>=0 AND b<=31 AND !hiding_spot_occ(a,b)) //if adj spot is not barrier or spot closed
			spot[a][b].stat=1; spot[a][b].g=spot[cur_r][cur_c].g+10; spot[a][b].h=abs(a-i_r)+abs(b-i_c)*10;
			spot[a][b].par_r=cur_r; spot[a][b].par_c=cur_c;
		END

		a=cur_r; b=cur_c+1;
		IF((baddy==7 OR (grid[0].cord[a][b].back!=_wall AND grid[0].cord[a][b].item!=_pillar)) AND spot[a][b].stat!=2 AND a>=0 AND a<=31 AND b>=0 AND b<=31 AND !hiding_spot_occ(a,b)) //if adj spot is not barrier or spot closed
			spot[a][b].stat=1; spot[a][b].g=spot[cur_r][cur_c].g+10; spot[a][b].h=abs(a-i_r)+abs(b-i_c)*10;
			spot[a][b].par_r=cur_r; spot[a][b].par_c=cur_c;
		END

		lowest_f=10000;
		d=0;
		FOR(a=0;a<32;++a)
			FOR(b=0;b<32;++b)
				IF(spot[a][b].stat==2) d++; END
				IF(spot[a][b].stat==1)
					IF(spot[a][b].h+spot[a][b].g<lowest_f)
						lowest_f=spot[a][b].h+spot[a][b].g;
						f_r=a; f_c=b;
					END
				END
				IF(d==32*32) BREAK; END
			END
			IF(d==32*32) BREAK; END
		END
		IF(d==32*32) debug; END
		cur_r=f_r; cur_c=f_c;
		IF(cur_r==e_r AND cur_c==e_c) spot[e_r][e_c].stat=2;  END //hit target
	END

	WHILE(true) //trace back
		my_path.row[pth]=cur_r; my_path.col[pth]=cur_c; pth++;
		f_r=cur_r; f_c=cur_c;
		a=spot[cur_r][cur_c].par_r; b=spot[cur_r][cur_c].par_c;
		cur_r=a; cur_c=b;
		//say(itoa(f_r)+","+itoa(f_c));
		IF(cur_r==i_r AND cur_c==i_c) BREAK; END
	END
	/*
	IF(f_r==i_r)//move col
		IF(f_c<i_c)
			d=3;
		ELSE
			d=2;
		END
	ELSE //move row
		IF(f_r<i_r)
			d=1;
		ELSE
			d=0;
		END
	END*/
	RETURN(pth);
END

FUNCTION hiding_spot_occ(row,col);
PRIVATE
	int answer;
BEGIN
	IF(grid[0].cord[row][col].item==_hider)
		WHILE(a=get_id(type hero))
			IF(a.row==row AND a.col==col)
				answer=true;
			END
		END
	END

	RETURN(answer);
END

PROCESS anyone_is_close_to_monster();
PRIVATE
	int answer=false;
BEGIN
	WHILE(a=get_id(type hero))
		IF(abs(a.col-monst.col)<5+(diff*3)/7 AND abs(a.row-monst.row)<5+(diff*3)/7)
			IF(grid[0].cord[a.row][a.col].item!=_hider)
				answer=true;
			END
		END
	END
	RETURN(answer);
END

FUNCTION get_file(graph);
BEGIN
    IF(graph==_treasure OR graph==_gold OR graph==_treasdar OR graph==_monsdar OR graph==_double OR graph==_triple OR graph==_prisoner OR graph==_torch)
		file=fpg[0];
	ELSE
		file=fpg[1];
	END
   RETURN(file);
END


PROCESS clear_path(path my_path);
BEGIN
	FOR(a=0;a<501;++a)
		my_path.row[a]=-1;
		my_path.col[a]=-1;
	END
END


PROCESS set_base_cord();
BEGIN

	IF(selected_hero.row<base_row+5/zoom) base_row=selected_hero.row-5/zoom; END IF(base_row<0) base_row=0; END
	IF(selected_hero.row>base_row+(12-6)/zoom) base_row=selected_hero.row-6/zoom; END IF(base_row>=32-(12/zoom)) base_row=32-(12/zoom); END

	IF(selected_hero.col<base_col+5/zoom) base_col=selected_hero.col-5/zoom; END IF(base_col<0) base_col=0; END
	IF(selected_hero.col>base_col+(12-6)/zoom) base_col=selected_hero.col-6/zoom; END IF(base_col>=32-(12/zoom)) base_col=32-(12/zoom); END

END

PROCESS get_treasure_loc(tipe);
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_treasure)
				IF(tipe==0) d=a; ELSE d=b; END
				RETURN(d);
			END
		END
	END

END

FUNCTION get_baddy(string theme);
BEGIN
	theme=lcase(theme);
	SWITCH(theme)
		CASE "woods": d=0; END
		CASE "pyram": d=1; END
		CASE "castl": d=2; END
		CASE "swamp": d=3; END
		CASE "space": d=4; END
		CASE "mansi": d=5; END
		CASE "arcti": d=6; END
		CASE "citya": d=7; END
		CASE "volca": d=8; END
		CASE "cornf": d=9; END
		CASE "jungl": d=10; END
		CASE "junky": d=11; END
		CASE "grave": d=12; END
	END
	RETURN(d);
END

PROCESS fully_load_some_level();
PRIVATE
	int tot_poss=0;
	string fullfile;
	STRING flies[99];
	int first=true;
BEGIN
	tot_poss=0;

	fullfile="levels/"+interp_diff(diff)+"/*.lvl";
	//debug;
	glob("");
	flies[tot_poss]=glob(fullfile);

	WHILE(flies[tot_poss]!="")
		flies[++tot_poss]=glob(fullfile);
	END


	WHILE(first OR (diff!=10 AND (last_level_file[diff%3]==level_file OR last_last_level_file[diff%3]==level_file)))
		first=false;

		a=rand(0,tot_poss-1); //choosing a random file

		fullfile="levels/"+interp_diff(diff)+"/"+flies[a];
		load_level(fullfile);
		level_file=substr(fullfile,11,5);
	END

	fullfile="fpgs/"+substr(flies[a],0,5)+".fpg";
	IF(fpg[1]!=0) fpg_unload(fpg[1]); END
	fpg[1]=fpg_load(fullfile);

	//last_last_last_level_file[diff%3]=last_last_level_file[diff%3];
	last_last_level_file[diff%3]=last_level_file[diff%3];
	last_level_file[diff%3]=level_file;
	baddy=get_baddy(substr(flies[a],0,5));
	set_up_level();
END



FUNCTION set_up_level();
PRIVATE
	int max_treas=0;
	loc treas[9];

	int gold_allow[10]=0,35,45,55,65,75,90,100,110,125,135;
BEGIN
	FOR(a=0;a<32;++a) //locating treasure chests
		FOR(b=0;b<32;++b)
			IF(grid[0].cord[a][b].item==_treasure)
				treas[max_treas].row=a; treas[max_treas].col=b; //mark location
				grid[0].cord[a][b].item=0; // remove treasure chest from grid
				max_treas++;
			END
		END
	END
	max_treas-=1;
	d=rand(0,max_treas); //randomly select one of the treasure chests
	grid[0].cord[treas[d].row][treas[d].col].item=_treasure; //re insert selected treasure chest into grid

	c=0;
	FOR(a=-4;a<=4;++a)
		FOR(b=-4;b<=4;++b)
			IF(grid[0].cord[treas[d].row+a][treas[d].col+b].back!=_wall AND grid[0].cord[treas[d].row+a][treas[d].col+b].item==0 AND
			treas[d].row+a>=0 AND treas[d].row+a<=31 AND treas[d].col+b>=0 AND treas[d].col+b<=31)
				IF(rand(0,99)<100-((abs(a)+abs(b))*15)) grid[0].cord[treas[d].row+a][treas[d].col+b].item=_gold; c++; END
			END
		END
	END


	FOR(;c<gold_allow[diff];++c)
		a=rand(0,31); b=rand(0,31);
		WHILE(grid[0].cord[a][b].back==_wall OR grid[0].cord[a][b].item!=0)
			a=rand(0,31); b=rand(0,31);
		END
		 grid[0].cord[a][b].item=_gold;
	END
END


FUNCTION every_body_is_trapped();
PRIVATE
	int answer=true;
BEGIN
	WHILE(a=get_id(type hero))
		IF(a.stuck==false) answer=false; BREAK; END
	END
	RETURN(answer);
END

FUNCTION whos_dead();
PRIVATE
	int guys[2]=0,1,2;
BEGIN
	WHILE(a=get_id(TYPE hero))
		guys[a.ident]=-1;
	END
	FOR(a=0;a<3;++a)
		IF(guys[a]!=-1)
			d=a; BREAK;
		END
	END

	RETURN(d);
END


FUNCTION assign_highscore();
PRIVATE
	string spare_name;
	int spare_score;

	string hold_name;
	int hold_score;
BEGIN
	hold_score=score;
	hold_name=output;
	FOR(a=0;a<10;++a)
		IF(hold_score>profile[0].hiscore[a].val)
			spare_name=profile[0].hiscore[a].name;
			spare_score=profile[0].hiscore[a].val;
			profile[0].hiscore[a].val=hold_score;
			profile[0].hiscore[a].name=hold_name;
			hold_name=spare_name;
			hold_score=spare_score;
		END
	END

END


FUNCTION clear_map_grid();
BEGIN
	FOR(a=0;a<32;++a)
		FOR(b=0;b<32;++b)
			the_map_grid[0].cord[a][b]=0;
		END
	END
END


FUNCTION set_all_sound();
BEGIN
	//SET_SONG_VOLUME(volume_level);
	sound_set_volume(song[0],volume_level);
	sound_set_volume(song[1],volume_level);
	sound_set_volume(song[2],volume_level);
	sound_set_volume(s_click,volume_level);
	sound_set_volume(s_click2,volume_level);
	sound_set_volume(s_dang,volume_level);
	sound_set_volume(s_dead,volume_level);
	sound_set_volume(s_coin,volume_level);
	sound_set_volume(s_sonar,volume_level);
	sound_set_volume(s_multi,volume_level);
	sound_set_volume(s_open,volume_level);
	sound_set_volume(s_prison,volume_level);
	sound_set_volume(s_fail,volume_level);
END

FUNCTION STRING describe_monst();
PRIVATE
	string answer;
BEGIN
	SWITCH(baddy)
		CASE 0:
			SWITCH(lang)
				CASE _engl: answer="WEREWOLF"; END
				CASE _span: answer="HOMBRE LOBO"; END
				CASE _germ: answer="WERWOLF"; END
			END
		END
		CASE 1:
			SWITCH(lang)
				CASE _engl: answer="MUMMY"; END
				CASE _span: answer="MOMIA"; END
				CASE _germ: answer="MUMIE"; END
			END
		END
		CASE 2:
			SWITCH(lang)
				CASE _engl: answer="VAMPIRE"; END
				CASE _span: answer="VAMPIRO"; END
				CASE _germ: answer="VAMPIR"; END
			END
		END

		CASE 3:
			SWITCH(lang)
				CASE _engl: answer="SWAMP MONSTER"; END
				CASE _span: answer="MONSTRUO DEL PANTANO"; END
				CASE _germ: answer="SUMPF MONSTER"; END
			END
		END
		CASE 4:
			SWITCH(lang)
				CASE _engl: answer="ALIEN"; END
				CASE _span: answer="EXTRATERRESTRE"; END
				CASE _germ: answer="AUSSERIRDISCH"; END
			END
		END
		CASE 5:
			SWITCH(lang)
				CASE _engl: answer="GHOST"; END
				CASE _span: answer="FANTASMA"; END
				CASE _germ: answer="GESPENST"; END
			END
		END
		CASE 6:
			SWITCH(lang)
				CASE _engl: answer="YETI"; END
				CASE _span: answer="MONSTRUO DE LA NIEVE"; END
				CASE _germ: answer="YETI"; END
			END
		END
		CASE 7:
			SWITCH(lang)
				CASE _engl: answer="CREATURE"; END
				CASE _span: answer="CRIATURA"; END
				CASE _germ: answer="KREATUR"; END
			END
		END
		CASE 8:
			SWITCH(lang)
				CASE _engl: answer="MAGMA MAN"; END
				CASE _span: answer="HOMBRE DE MAGMA"; END
				CASE _germ: answer="MAGMA MANN"; END
			END
		END
		CASE 9:
			SWITCH(lang)
				CASE _engl: answer="SCARE CROW"; END
				CASE _span: answer="ESPANTA PAJAROS"; END
				CASE _germ: answer="VOGELSCHEUCHE"; END
			END
		END
		CASE 10:
                        SWITCH(lang)
				CASE _engl: answer="JAGUAR WOMAN"; END
				CASE _span: answer="MUJER JAGUAR"; END
				CASE _germ: answer="JAGUAR FRAU"; END
			END
		END
		CASE 11:
                        SWITCH(lang)
				CASE _engl: answer="MACHINE"; END
				CASE _span: answer="MAQUINA"; END
				CASE _germ: answer="MASCHINE"; END
			END
		END
		CASE 12:
		        SWITCH(lang)
				CASE _engl: answer="SKELETON"; END
				CASE _span: answer="ESQUELETO"; END
				CASE _germ: answer="SKELETT"; END
			END
		END

	END
	RETURN(answer);
END
