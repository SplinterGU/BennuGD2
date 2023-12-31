PROCESS card_game(row,col);
PRIVATE
	int the_c[15]=201,201,205,205,209,209,213,213,217,217,221,221,225,225,229,229;
	int a_c[15];
BEGIN
	signal_action(s_freeze_tree,s_ign);
	signal(type mole,s_freeze_tree);
	
	FRAME;
	kill_menus=false;
	file=fpg[0];
	graph=982;
	x=117;
	y=124;
	z=-100;
	//a_card(40+a*50,50,the_c[0]);
	
	FOR(a=15;a>=0;--a)
		b=rand(0,a);
		a_c[a]=a_card(46+(a%4)*47,50+(a/4)*49,the_c[b]);
		the_c[b]=the_c[a];		
	END
	c=rand(0,15);
	a_c[c].selected=true;
	card_selector(a_c[c]);
	LOOP
		
		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
ONEXIT
	IF(selected) 
		grid[0].cord[row][col].item=_open_door;
		IF(selected_hero.col==col)
			IF(selected_hero.row<row)
				selected_hero.move_row=1;
			ELSE
				selected_hero.move_row=-1;
			END
		ELSE
			IF(selected_hero.col<col)
				selected_hero.move_col=1;
			ELSE
				selected_hero.move_col=-1;
			END
		END
	END 
	moves_left--; 
	
	IF(moves_left==0 AND noones_moving()) change_turn=true; END
	signal(type mole,s_wakeup_tree);
	
END
	
	
PROCESS a_card(double x,y,int base_graph);
PRIVATE
	int total_selected;
	int start_selected;
BEGIN
	graph=base_graph;
	FRAME; 
	kill_menus=false;
	file=fpg[0];
	
	z=-101;
	IF(selected) start_selected=true; END
	FOR(a=0;a<100-(diff*3);++a)
		FRAME;
	END
	
	LOOP
		IF(selected)
			IF(--graph<base_graph) graph=base_graph; END
			IF(graph>base_graph+3 AND graph<233) graph=base_graph+3; END
		ELSE
			IF(++graph>236) graph=236; END
			IF(graph>base_graph+3 AND graph<233) graph=233; END
		END
	
		total_selected=0;
		WHILE(d=get_id(type a_card))
			IF(d.selected) 
				total_selected++; 
				IF(selected)
					IF(d!=id) c=d; END
				END
			END
		END
	
		IF(total_selected<2 AND mouse_release)
			IF(collision(type mouse))
				b=get_id(type card_selector); b.control=id;
				selected=true;
			END
		END
		
		
		IF(selected AND !start_selected)
			IF(exists(c))
				IF(graph==base_graph)
					
					FOR(a=0;a<20;++a)
						FRAME;
					END
			
					IF(c.graph==graph) father.selected=true; sound_play(s_open,0); ELSE sound_play(s_fail,0); END
					kill_menus=true;
				END
			END
		END
	
		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END

PROCESS card_selector(control);
PRIVATE
	int the_cs[15];
	int move_count;
BEGIN
	WHILE(the_cs[a]=get_id(type a_card)) IF(the_cs[a]==control) d=a; END a++; END
	IF(the_cs[0]==control) b=1; ELSE b=0; END
	
	WHILE(the_cs[b].graph<236) FRAME; END
	file=fpg[0];
	graph=200;
	
	 z=-102;
	LOOP
		IF(move_count--<0)
			IF(jkeys_state[ _JKEY_UP    ] AND d>=4) d-=4; move_count=3; control=the_cs[d]; END
			IF(jkeys_state[ _JKEY_DOWN  ] AND d<=11) d+=4; move_count=3; control=the_cs[d]; END
			IF(jkeys_state[ _JKEY_LEFT  ] AND d>=1) d-=1; move_count=3; control=the_cs[d]; END
			IF(jkeys_state[ _JKEY_RIGHT ] AND d<=14) d+=1; move_count=3; control=the_cs[d]; END
		END
	
		IF(( jkeys_state[ _JKEY_A ] OR key(_ENTER) ) AND the_cs[d].graph==236) the_cs[d].selected=true; END
	
		IF(exists(control))
			x=control.x; y=control.y;
		END
		IF(kill_menus)
			BREAK;
		END
		FRAME;
	END
END

