PROCESS keyy();
BEGIN
	FRAME;
	kill_menus=false;
	file=fpg[0];
	graph=982;
	x=117;
	y=124;
	z=-100;

	key_item(0,60);
	key_item(1,100);
	key_item(2,140);
	key_item(3,180);
	key_item(4,220);

	keyy_up();
	keyy_dn();
	LOOP
		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END


PROCESS key_item(control, double y);
PRIVATE
	int txt;
	string outp;
	wrd_set ttl;
BEGIN
	x=30;
	z=-101;
	txt=write_var(0,x+30,y-5,ALIGN_BOTTOM_LEFT,outp);
	LOOP
		SWITCH(control+father.control)
		CASE 5:
			ttl.txt[_engl]="TREASURE"; ttl.txt[_span]="TESORO"; ttl.txt[_germ]="SCHATZ";
			file=fpg[0];
			graph=_treasure;
		END
		CASE 6:
			ttl.txt[_engl]="GOLD"; ttl.txt[_span]="ORO"; ttl.txt[_germ]="GELD";
			file=fpg[0];
			graph=_gold;
		END
		CASE 7:
			ttl.txt[_engl]="TREASURE RADAR"; ttl.txt[_span]="RADAR DE TESORO"; ttl.txt[_germ]="SCHATZ RADAR";
			file=fpg[0];
			graph=_treasdar;
		END
		CASE 8:
			ttl.txt[_engl]="MONSTER RADAR"; ttl.txt[_span]="RADAR DE MONSTRUOS"; ttl.txt[_germ]="MONSTER RADAR";
			file=fpg[0];
			graph=_monsdar;
		END
		CASE 0:
			ttl.txt[_engl]="HIDING PLACE"; ttl.txt[_span]="ESCONDRIJO"; ttl.txt[_germ]="UNTERSCHLUPF";
			file=fpg[1];
			graph=_hider;
		END
		CASE 1:
			ttl.txt[_engl]="BARRIER"; ttl.txt[_span]="BARRERA"; ttl.txt[_germ]="BARRIERE";
			file=fpg[1];
			graph=_wall;
		END
		CASE 2:
			ttl.txt[_engl]="BARRIER"; ttl.txt[_span]="BARRERA"; ttl.txt[_germ]="BARRIERE";
			file=fpg[1];
			graph=_pillar;
		END
		CASE 3:
			ttl.txt[_engl]="TRAP"; ttl.txt[_span]="TRAMPA"; ttl.txt[_germ]="FALLE";
			file=fpg[1];
			graph=_trap;
		END
		CASE 4:
			ttl.txt[_engl]="GATEWAY"; ttl.txt[_span]="PUERTA"; ttl.txt[_germ]="DURCHGANG";
			file=fpg[1];
			graph=_closed_door;
		END
		CASE 9:
			ttl.txt[_engl]="PRISONER"; ttl.txt[_span]="PRISIONERO"; ttl.txt[_germ]="GEFANGENE";
			file=fpg[0];
			graph=_prisoner;
		END
		CASE 11:
			ttl.txt[_engl]="POINTS X2"; ttl.txt[_span]="PUNTOS X2"; ttl.txt[_germ]="KERBE X2";
			file=fpg[0];
			graph=_double;
		END
		CASE 12:
			ttl.txt[_engl]="POINTS X3"; ttl.txt[_span]="PUNTOS X3"; ttl.txt[_germ]="KERBE X3";
			file=fpg[0];
			graph=_triple;
		END
		CASE 10:
			ttl.txt[_engl]="TORCH"; ttl.txt[_span]="ANTORCHA"; ttl.txt[_germ]="FACKEL";
			file=fpg[0];
			graph=_torch;
		END
	END
		outp=ttl.txt[lang];

		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
ONEXIT
	write_delete(txt);
END


PROCESS keyy_up();
BEGIN
	file=fpg[0];
	x=196; y=44;
	z=-101;
	LOOP
		control--;
		IF((mouse.x>=x AND mouse.x<=x+18 AND mouse.y>=y-18 AND mouse.y<=y AND mouse.left) OR jkeys_state[ _JKEY_UP ])
			graph=979;
		ELSE
			graph=978;
		END

		IF((mouse.x>=x AND mouse.x<=x+18 AND mouse.y>=y-18 AND mouse.y<=y AND mouse_release) OR jkeys_state[ _JKEY_UP ] AND control<0 )
			sound_play(s_click2,0);
			IF(--father.control<0) father.control=0; END
			control=5;
		END

		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END

PROCESS keyy_dn();
BEGIN
	file=fpg[0];
	x=196; y=220;
	z=-101;
	LOOP
		control--;
		IF((mouse.x>=x AND mouse.x<=x+18 AND mouse.y>=y-18 AND mouse.y<=y AND mouse.left) OR jkeys_state[ _JKEY_DOWN ])
			graph=981;
		ELSE
			graph=980;
		END

		IF((mouse.x>=x AND mouse.x<=x+18 AND mouse.y>=y-18 AND mouse.y<=y AND mouse_release) OR jkeys_state[ _JKEY_DOWN ] AND control<0 )
			sound_play(s_click2,0);
			IF(++father.control>8) father.control=8; END
			control=5;
		END

		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END

PROCESS mapp(control);
PRIVATE
	int alone=true;
BEGIN FRAME;
	WHILE(d=get_id(type mapp))
		IF(d!=id)
			the_map=d;
			alone=false;
		END
	END

	x=53; y=187;

	IF(control==1) tres(0); END
	IF(control==2) tres(1); END
	IF(control==3) mon_blip(); END

	IF(alone)
		file=fpg[0];
		graph=990;

		priority=1000;
		z=-500;

		FOR(a=0;a<32;++a)
			FOR(b=0;b<32;++b)
				IF(the_map_grid[0].cord[a][b]==1)
					wall_blip(a,b);
				END
				IF(the_map_grid[0].cord[a][b]>=970)
					trail_blip(a,b,the_map_grid[0].cord[a][b]);
				END
			END
		END

		WHILE(a=get_id(type hero)) blip(a); END
		LOOP
			IF(selected)
				signal(type blip,s_kill_tree);
				signal(type tres,s_kill_tree);
				signal(type mon_blip,s_kill_tree);
				signal(type wall_blip,s_kill_tree);
				signal(type trail_blip,s_kill_tree);
				the_map=-1;
				BREAK;
			END
			FRAME;
		END
	END
END

PROCESS blip(control);
BEGIN
	file=fpg[0];
	z=-510;
	graph=control.ident+985;
	LOOP
		IF(!exists(control)) BREAK; END
		x=the_map.x+control.row*4; y=the_map.y-control.col*4;
		FRAME;
	END
END


PROCESS tres(tipe);
BEGIN
	file=fpg[0];
	z=-509;

	IF(tipe==0)
		x=father.x+get_treasure_loc(tipe)*4; y=father.y;
		graph=989;
	ELSE
		x=the_map.x; y=the_map.y-get_treasure_loc(tipe)*4;
		graph=988;
	END

	LOOP
		FRAME;
	END
END

PROCESS mon_blip();
BEGIN
	file=fpg[0];
	graph=984;
	z=-501;
	x=the_map.x+(monst.row/8)*8*4;
	y=the_map.y-(monst.col/8)*8*4;
	LOOP
		FRAME;
	END
END


PROCESS wall_blip(row,col);
BEGIN
	file=fpg[0];
	graph=983;
	z=-509;
	x=the_map.x+row*4; y=the_map.y-col*4;
	LOOP
		FRAME;
	END
END

PROCESS trail_blip(row,col,graph);
BEGIN
	file=fpg[0];
	//graph=983;
	z=-508;
	x=the_map.x+row*4; y=the_map.y-col*4;
	LOOP
		FRAME;
	END
END


PROCESS letter_but(x,y,string let);
PRIVATE
	int txt;
BEGIN
	file=fpg[0];
	graph=19;
	z=-100;

	FRAME; FRAME;
	kill_menus=false;

	txt=write(fnt[0],x,y,ALIGN_CENTER,let);
	LOOP
		graph=19;
		control--;
		IF(mouse.left)
			IF(collision(type mouse))
				WHILE(a=get_id(type letter_but)) a.selected=false; END
				a=get_id(type delete_but); a.selected=false;
				a=get_id(type enter_but); a.selected=false;
				selected=true;
			END
		END

		IF(selected)
			graph=20;
		END

        IF(mouse_release OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
            IF(collision(type mouse) OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
				sound_play(s_click2,0);
				output=substr(output,0,len(output)-1)+let+"_";
				control=8;
			END
		END

		IF(kill_menus) BREAK; END

		FRAME;
	END
ONEXIT
	write_delete(txt);
END


PROCESS delete_but();
PRIVATE
	wrd_set del;
	string outp;
	int txt;
BEGIN
	del.txt[_engl]="DEL"; del.txt[_span]="SUPR"; del.txt[_germ]="ENTF";

	file=fpg[0];
	graph=24;
	z=-100;
	x=228;
	y=150;

	FRAME; FRAME;
	kill_menus=false;

	txt=write_var(fnt[0],x,y,ALIGN_CENTER,outp);
	LOOP
		control--;
		outp=del.txt[lang];
		graph=24;

		IF(mouse.left)
			IF(collision(type mouse))
				WHILE(a=get_id(type letter_but)) a.selected=false; END
				a=get_id(type delete_but); a.selected=false;
				a=get_id(type enter_but); a.selected=false;
				selected=true;
			END
		END

		IF(selected)
			graph=25;
		END

        IF(mouse_release OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
            IF(collision(type mouse) OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
				control=8;
				sound_play(s_click2,0);
				IF(len(output)<2)
					output="_";
				ELSE
					output=substr(output,0,len(output)-2)+"_";
				END
			END
		END

		IF(kill_menus) BREAK; END

		FRAME;
	END
ONEXIT
	write_delete(txt);
END

PROCESS enter_but();
PRIVATE
	wrd_set del;
	string outp;
	int txt;
BEGIN
	del.txt[_engl]="ENT"; del.txt[_span]="ENT"; del.txt[_germ]="ENT";

	file=fpg[0];
	graph=24;
	z=-100;
	x=92;
	y=150;

	FRAME; FRAME;
	kill_menus=false;

	txt=write_var(fnt[0],x,y,ALIGN_CENTER,outp);
	LOOP
		control--;
		outp=del.txt[lang];
		graph=24;

		IF(mouse.left)
			IF(collision(type mouse))
				WHILE(a=get_id(type letter_but)) a.selected=false; END
				a=get_id(type delete_but); a.selected=false;
				a=get_id(type enter_but); a.selected=false;
				selected=true;
			END
		END

		IF(selected)
			graph=25;
		END

        IF(mouse_release OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
            IF(collision(type mouse) OR (selected AND ( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND control<0))
				control=8;
				IF(len(output)>1)
					sound_play(s_click2,0);
					output=substr(output,0,len(output)-1);
					assign_highscore();
					kill_menus=true;
				END
			END
		END

		IF(kill_menus) BREAK; END

		FRAME;
	END
ONEXIT
	write_delete(txt);
END



PROCESS keyboard();
PRIVATE
	int txt;
	int butt[27];
BEGIN
	output="_";
	txt=write_var(0,70,75,ALIGN_BOTTOM_LEFT,output);

	FRAME; FRAME;
	kill_menus=false;


	FOR(a=0;a<26;++a)
		butt[a]=letter_but(70+15*(a%13),90+30*(a/13),chr(a+65));
	END
	butt[27]=delete_but();
	butt[26]=enter_but();

	butt[0].selected=true;
	LOOP
		control--;
		IF(control<0)
			IF(jkeys_state[ _JKEY_UP ])
				IF(d==26)
					butt[d].selected=false; d=16; butt[d].selected=true; control=6;
				ELSE IF(d==27)
					butt[d].selected=false; d=22; butt[d].selected=true; control=6;
				ELSE IF(d>=13)
					butt[d].selected=false; d-=13; butt[d].selected=true; control=6;
				END END END
			END
			IF(jkeys_state[ _JKEY_DOWN ])
				IF(d<=12)
					butt[d].selected=false; d+=13; butt[d].selected=true; control=6;
				ELSE IF(d>12 AND d<20)
					butt[d].selected=false; d=26; butt[d].selected=true; control=6;
				ELSE IF(d>19 AND d<26)
					butt[d].selected=false; d=27; butt[d].selected=true; control=6;
				END END END
			END

			IF(jkeys_state[ _JKEY_LEFT ] AND d>=1) butt[d].selected=false; d-=1; butt[d].selected=true; control=2; END
			IF(jkeys_state[ _JKEY_RIGHT ] AND d<=26) butt[d].selected=false; d+=1; butt[d].selected=true; control=2; END
		END

		IF(kill_menus) BREAK; END

		FRAME;
	END
ONEXIT
	write_delete(txt);
END
