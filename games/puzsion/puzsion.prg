//****************************************************************
//****************************************************************
//***                         PUZSION                          ***
//****************************************************************
//***    PARTICIPACION EN CONCURSO DE http://www.bennugd.org/  ***
//***                    WRITTEN IN BENNU GD                   ***
//****************************************************************
//***                   Coded  By  Hokutoy                     ***
//***                   Graphics  By  Amzg                     ***
//****************************************************************
//***                        06/12/2009                        ***
//****************************************************************
//****************************************************************
// Este juego es un excelente ejemplo de como NO hay que programar en Bennu... XDDDDD
// Este es mi segundo juego/codigo largo y he realizado TODOS los pecados capitales del programador noob:
// Codigo 'Wall-of-text', uso de nombres para variables confusos, comentarios en lineas obsoletos o inciertos,
// fallos/falta de planteamiento inicial del juego arreglados a golpe de chapuza~procesos extra, optimizacion 0, imposible
// seguir el "esquema de flujo" del juego gracias a mi caotica organizacion y por hacer los 3 modos de juego
// a la vez, no usar funciones porque no sabia ni que existian, etc....
// Sorry! Lo importante es que al darle al "play" todo funciona como debe (lucky!). Tambien es cierto que gracias
// a estos errores he aprendido bastante para poder gestionar mi proximo proyecto infinitamente mejor.
// Pd: BennuGd es la caña! Amzg tambien!

import "libmod_gfx";
import "libmod_misc";
import "libmod_sound";
import "libmod_input";

include "includes/jkey.h"; // CARGA LA LISTA DE CONSTANTES PARA LOS BOTONES DE LA WIZ

declare PROCESS PUT(int file,graph,double x,y);
declare PROCESS XPUT(int file,graph,double x,y,int angle, double size, int flags,region);
declare PROCESS MAIN();
declare PROCESS MENU(MODO);
declare PROCESS INTRO(opcion);
declare PROCESS ENTERNAME(GRAPH,double X,Y, int MODOJUEGO);
declare PROCESS FAKEJEWEL(casilla,double X,Y, int FGRAPH,BRILLA);
declare PROCESS JEWEL(casilla,pos_X,pos_Y,pos_GRAPH);
declare PROCESS PUNTERO();
declare PROCESS LEVEL_QUOTA(NIVEL);
declare PROCESS LEVEL_GLASS(GLASS_NIVEL);
declare PROCESS LEVEL_RUSH();
declare PROCESS HIT(double X,Y, int Z);
declare PROCESS PAUSA();
declare PROCESS sound(SFX);
declare PROCESS GAMEOVER(modojuego);
declare PROCESS CHECK(); 
declare PROCESS CHECK_GLASS(); 
declare PROCESS combo(grafico,TIPO,INCREMENTO); 
declare PROCESS comboRUSH(grafico,TIPO); 
declare PROCESS FLY(double X,Y,int Z,FLY_NUM,LOL);
declare PROCESS SPEAKER(NAME);
declare PROCESS FLYS(double X,Y,int Z,FLYSCORE);
declare PROCESS FLYT(double X,Y,int Z,FLYTIME);
declare PROCESS mergerhighscore();
declare PROCESS mergeqhighscore();
declare PROCESS mergeghighscore();
declare PROCESS initHighscore();
declare PROCESS printrhighscore();
declare PROCESS printghighscore();
declare PROCESS printqhighscore();
declare PROCESS RESET(INICIO,FINAL,ELECCION);
declare PROCESS EXPLOSION();

#define write_string(font_id,x,y,alignment,pointer_to_data) WRITE_VALUE(font_id,x,y,alignment,text_string,pointer_to_data)
#define write_int(font_id,x,y,alignment,pointer_to_data) WRITE_VALUE(font_id,x,y,alignment,text_int,pointer_to_data)

#define init_bgd1_background_emulation() background.file = 0; background.graph = map_new(320,240)
#define put_screen(f,g) map_clear(0, background.graph ); map_put(0, background.graph, f, g, 160, 120)
#define clear_screen() map_clear(0, background.graph )
#define put(f,g,x,y) map_put(0, background.graph, f, g, x, y)
#define xput(f,g,x,y,angle,size,flags,region) map_put(0, background.graph, f, g, x, y, angle, size, size, flags, 255, 255, 255, 255)
#define map_xput(fdst,gdst,gsrc,x,y,angle,size,flags) map_put(fdst, gdst, fdst, gsrc, x, y, angle, size, size, flags, 255, 255, 255, 255)

GLOBAL
	INT JOYAS; //--> ID DEL FPG PARA LOS GFX DE LAS "JOYAS"
	INT	FONDO; //--> ID DEL FPG PARA LOS GFX DE LOS FONDOS
	INT	CHAIN=0; //--> VARIABLE QUE CONTIENE EL NUMERO DE JOYAS SELECCIONADAS CONSECUTIVAS (combo o chain)
	INT	ID_EXPLOSION; //--> ID DEL PROCESO EXPLOSION() QUE ELIMINA LAS JOYAS SELECIONADAS EN CHAIN CORRECTA
	INT	CANDIDATO=1;  //--> VARIABLE QUE SE USA PARA CONTAR LOS CANDIDATOS A COMBO VALIDO O PARA ENTRAR EN LA LISTA DE ELIMINACION
	INT	SCORE=0; //--> VARIABLE QUE CONTIENE LA PUNTUACION ACTUAL DE LA PARTIDA
	INT	REFILL=1; //--> VARIABLE QUE DETERMINA EL tipo de juego
	INT	blanconegro; // --> ID DE LA TABLA BLEND;
	INT	PUZSION=0;
	INT	DIFERENCIA; //--> RESTO DEL SCORE
	INT	QC3,QC4,QC5,QC6,QC7,QC8,QC9,QC10,QC11,QC12,QC13,QC14,QC15,QC16,QC17,QC18,QC19,QC20;
	INT	QC21,QC22,QC23,QC24,QC25,QC26,QC27,QC28,QC29,QC30,QC31,QC32,QC33,QC34,QC35,QC36,QC37,QC38,QC39,QC40;
	INT	QC41,QC42,QC43,QC44,QC45,QC46,QC47,QC48,QC49;//--> QUOTA DE CHAINS
	INT	QJ1,QJ2,QJ3,QJ4; //--> QUOTA DE JEWELS
	INT	TIPO_GLOW=1; //--> TIPO DE EFECTO AL SELECCIONAR LA JOYAS
	INT	NUMTIME,NUMSCORE,NUMQUOTA,NUMLEVEL,numjoyas, NUMBIGSCORE;//--> IDS DE LA FONT NUMERICA
	INT	NUMEROSQUOTA,NUMEROSRUSH,NUMEROSGLASS; //--> ID DEL FPG NUMERICO
	INT	marcador;
	INT	WAITCHECK=0;
	INT	reloj;
	INT	MODE=1; //--> INDICA EL TIPO DE MODO DE JUEGO, QUOTA, TIME ATACK, ETC...
	INT	NIVEL_GLOBAL=1; //--> INDICA EL NIVEL_GLOBAL (1-5) DENTRO DEL SUB NIVEL "LEVEL"(1-10);
	INT	check_si=0;
	INT	check_who=0;
	INT	entername_id; //--> id fpg letras
	INT	gfx_hit;
	INT	SFX_G1;
	INT	sfx_J1,sfx_J2,sfx_J3,sfx_J4,sfx_J5,sfx_J6,sfx_J7;
	INT	sfx_puzsion,sfx_qmode,sfx_rmode,sfx_gmode,sfx_options,sfx_hiscore,sfx_exitg;
	INT	sfx_start,sfx_howto,sfx_exit,sfx_pause;
	INT	sfx_levelup, sfx_fantastic,sfx_great,sfx_combo,sfx_awesome,SFX_SWEET;
	INT	sfx_perfect,sfx_marve,sfx_outs,sfx_increible;
	INT	SFX_superb, SFX_excellent,SFX_priceless,SFX_brillant,SFX_smooth, SFX_killer;
	INT	sfx_extra,SFX_super,SFX_hyper,SFX_ultra,SFX_giga,SFX_mega;
	INT	sfx_nomoves, sfx_destruction;
	INT	sfx_hurryup,sfx_gameover,sfx_entername;
	INT	firstime=0; //--> variable para el modo quota... evita el Ftime.

	String fecha; //--> Guarda la fecha
	int tiempo_juego;
    INT		GTM; //DETERMINA EL GTM PARA EL FTIME

	int SFX_VOL=128;
	int MUS_VOL=128;
	int STOP_REMOVER=0; //--> FARA FORZAR LA CONTINUACION CUANDO SE HACE JACKPOT
	int FRAMES_DESTRUCCION=5; //FRAMES TOTALES DE JOYA+SU DESTRUCCION
	int QUOTA_DESTROYED=0;
	int GLASS_NIVEL=0;
	int splash; //--> ID DEL FPG SPLASH
	int SHAKE=0;
	int RUSH_LEVEL=4,num_nivel=0;
	int rhiscore,ghiscore,qhiscore,whiscore,yhiscore;
	STRING FRELOJ;
	struct rhighscore[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo;end //--> VECTOR DE HIGHSCORES MODO RUSH
	struct ghighscore[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo;end //--> VECTOR DE HIGHSCORES MODO RUSH
	struct qhighscore[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo; end //--> VECTOR DE HIGHSCORES MODO RUSH

	struct mergerhighscore_dat[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo;end //--> VECTOR DE HIGHSCORES MODO RUSH
	struct mergeghighscore_dat[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo;end //--> VECTOR DE HIGHSCORES MODO RUSH
	struct mergeqhighscore_dat[10] int hscore, Hfase, last;  string hname[6];string dia; STRING tiempo;end //--> VECTOR DE HIGHSCORES MODO RUSH

    struct saved[5]; int option; end = 3, 38, 38, 0, 3; // FEMALE VOICE, SFX VOL 38, MUSIC VOL 38, GMT+0, SCALE_2X
	int NOMBRE=0;
	int PUSHED=0; //--> DICE SI SE ESTA PULSANDO CON EL MOUSE O NO
	int ENCIENDEME=0;
	int music;
	STRUCT TABLERO[56] //--> STRUCT PRINCIPAL DONDE SE ALMACENA LA CORDENADAS, EL GRAFICO, LOS VECINOS Y VARIOS STATS DE LA JOYAS QUE CONFORMAN EL TABLERO
	INT X,   Y,   GRAPH,   ARRIBA,   DERECHA,   ABAJO,   IZQUIERDA, GLOW, DESTRUIR,ILUM ;end= // AUNQUE LOS VECINOS SE PODRIAN SACAR CON SIMPLES SUMAS Y RESTAS, LA CREACION DE LA TABLA ME AYUDA MUCHO A LA HORA DE CREAR EL JUEGO Y SU "FISICA"
		  0,  0,  99,       0,           0,       0,          0,     -1,    0,    0, // ESTA ES UNA JOYA FALSA (LA JOYA ZERO) QUE NO SE MUESTRA NI SIQUIERA EN PANTALLA. SIRVE DE REFERENCIA PARA LAS JOYAS FINALES DE LOS BORDES.

		 92, -18,  1,       0,         2,        8,          0,     0,    0,    0, //1
		126, -18,  1,       0,         3,        9,          1,     0,    0,    0, //2
		160, -18,  1,       0,         4,       10,          2,     0,    0,    0, //3
		194, -18,  1,       0,         5,       11,          3,     0,    0,    0, //4
		228, -18,  1,       0,         6,       12,          4,     0,    0,    0, //5
		262, -18,  1,       0,         7,       13,          5,     0,    0,    0, //6
		296, -18,  1,       0,         0,       14,          6,     0,    0,    0, //7



 		 92, 18,  1,       1,          9,       15,          0,     0,    0,    0, //8
		126, 18,  1,       2,         10,       16,          8,     0,    0,    0, //9
		160, 18,  1,       3,         11,       17,          9,     0,    0,    0, //10
		194, 18,  1,       4,         12,       18,         10,     0,    0,    0, //11
		228, 18,  1,       5,         13,       19,         11,     0,    0,    0, //12
		262, 18,  1,       6,         14,       20,         12,     0,    0,    0, //13
		296, 18,  1,       7,          0,       21,         13,     0,    0,    0, //14


		 92, 52,  1,       8,         16,       22,          0,     0,    0,    0, //15
	    126, 52,  1,       9,         17,       23,         15,     0,    0,    0, //16
		160, 52,  1,      10,         18,       24,         16,     0,    0,    0, //17
		194, 52,  1,      11,         19,       25,         17,     0,    0,    0, //18
		228, 52,  1,      12,         20,       26,         18,     0,    0,    0, //19
		262, 52,  1,      13,         21,       27,         19,     0,    0,    0, //20
		296, 52,  1,      14,          0,       28,         20,     0,    0,    0, //21

		 92, 86,  1,      15,         23,       29,          0,     0,    0,    0, //22
		126, 86,  1,      16,         24,       30,         22,     0,    0,    0, //23
		160, 86,  1,      17,         25,       31,         23,     0,    0,    0, //24
		194, 86,  1,      18,         26,       32,         24,     0,    0,    0, //25
		228, 86,  1,      19,         27,       33,         25,     0,    0,    0, //26
		262, 86,  1,      20,         28,       34,         26,     0,    0,    0, //27
		296, 86,  1,      21,          0,       35,         27,     0,    0,    0, //28

		 92, 120,  1,     22,         30,       36,          0,     0,    0,    0, //29
		126, 120,  1,     23,         31,       37,         29,     0,    0,    0, //30
		160, 120,  1,     24,         32,       38,         30,     0,    0,    0, //31
		194, 120,  1,     25,         33,       39,         31,     0,    0,    0, //32
		228, 120,  1,     26,         34,       40,         32,     0,    0,    0, //33
		262, 120,  1,     27,         35,       41,         33,     0,    0,    0, //34
		296, 120,  1,     28,          0,       42,         34,     0,    0,    0, //35

		 92, 154,  1,     29,         37,       43,          0,     0,    0,    0, //36
		126, 154,  1,     30,         38,       44,         36,     0,    0,    0, //37
		160, 154,  1,     31,         39,       45,         37,     0,    0,    0, //38
		194, 154,  1,     32,         40,       46,         38,     0,    0,    0, //39
		228, 154,  1,     33,         41,       47,         39,     0,    0,    0, //40
		262, 154,  1,     34,         42,       48,         40,     0,    0,    0, //41
		296, 154,  1,     35,          0,       49,         41,     0,    0,    0, //42

		 92, 188,  1,     36,         44,       50,          0,     0,    0,    0, //43
		126, 188,  1,     37,         45,       51,         43,     0,    0,    0, //44
		160, 188,  1,     38,         46,       52,         44,     0,    0,    0, //45
		194, 188,  1,     39,         47,       53,         45,     0,    0,    0, //46
		228, 188,  1,     40,         48,       54,         46,     0,    0,    0, //47
		262, 188,  1,     41,         49,       55,         47,     0,    0,    0, //48
		296, 188,  1,     42,          0,       56,         48,     0,    0,    0, //49

	     92, 222,  1,     43,         51,        0,          0,     0,    0,    0, //50
	    126, 222,  1,     44,         52,        0,         50,     0,    0,    0, //51
		160, 222,  1,     45,         53,        0,         51,     0,    0,    0, //52
		194, 222,  1,     46,         54,        0,         52,     0,    0,    0, //53
		228, 222,  1,     47,         55,        0,         53,     0,    0,    0, //54
		262, 222,  1,     48,         56,        0,         54,     0,    0,    0, //55
		296, 222,  1,     49,          0,        0,         55,     0,    0,    0; //56

	struct LISTA[56] INT ELIMINAR;END //--> STRUCT QUE CONTIENE LA LISTA DE JOYAS QUE SE VAN A ELIMINAR
	Struct SORTEO[3] INT NUMERO;END=1,11,21,31; // STRUCT CON EL NUJMERO DEL GRAFICO DE LAS JOYAS ORIGINALES
END

//****************************************************************
//***                     PROCESS MAIN                         ***
//****************************************************************

process MAIN();
	PRIVATE int CONTADOR=0;
	BEGIN
	load(get_pref_path("bennugd.org", "puzsion") + "options.file", saved);

	SWITCH(saved[3].option)
	CASE 0: GTM=43200;END   //-12
	CASE 1: GTM=39600;END   // -11
	CASE 2: GTM=36000;END  // -10
	CASE 3: GTM=32400;END  // -9
	CASE 4: GTM=28800;END    // -8
	CASE 5: GTM=25200;END  // -7
	CASE 6: GTM=21600;END  // -6
	CASE 7: GTM=18000;END  // -5
	CASE 8: GTM=16200;END  //- 4.3
	CASE 9: GTM=14400;END   // -4
	CASE 10: GTM=12600;END // -3.3
	CASE 11: GTM=10800;END  // -3
	CASE 12: GTM=7200;END  // -2
	CASE 13: GTM=3600;END // -1
	CASE 14: GTM=0;END // 0
	CASE 15: GTM=82800;END // 1
	CASE 16: GTM=79200;END // 2
	CASE 17: GTM=75600;END // 3
	CASE 18: GTM=73800;END // 3.5
	CASE 19: GTM=72000;END // 4
	CASE 20: GTM=70200;END // 4.3
	CASE 21: GTM=68400;END // 5
	CASE 22: GTM=66600;END // 5.3
	CASE 23: GTM=65700;END // 5.45
	CASE 24: GTM=64800;END // 6
	CASE 25: GTM=63000;END // 6.3
	CASE 26: GTM=61200;END // 7
	CASE 27: GTM=57600;END // 8
	CASE 28: GTM=54000;END //9
	CASE 29: GTM=52200;END // 9.3
	CASE 30: GTM=50400;END //10
	CASE 31: GTM=46800;END //11
	CASE 32: GTM=43200;END //12
	CASE 33: GTM=39600;END  //13
	END// SWITCH


	if(os_id==OS_GP2X_WIZ) screen.scale_resolution = -1; screen.scale_resolution_aspectratio = SRA_PRESERVE; set_mode(320,240); set_fps(75,3);gtm=0;
	ELSEIF(saved[4].option==1) set_fps(75,0); screen.scale_resolution = -1; screen.scale_resolution_aspectratio = SRA_PRESERVE; set_mode(320,240); MOUSE.LEFT=0;
	ELSEIF(saved[4].option==3) set_fps(75,0); screen.scale_resolution = 06400480; screen.scale_resolution_aspectratio = SRA_PRESERVE; set_mode(320,240); MOUSE.LEFT=0;
	ELSEIF(saved[4].option==5) set_fps(75,0); screen.scale_resolution = -1; screen.scale_resolution_aspectratio = SRA_PRESERVE; set_mode(320,240,mode_fullscreen); MOUSE.LEFT=0;
    ELSE
    set_fps(75,0);
    set_mode(320,240);
	END

	init_bgd1_background_emulation();

	SFX_VOL=saved[1].option;channel_set_volume(-1,SFX_VOL);
	MUS_VOL=saved[2].option;music_set_volume(MUS_VOL);

	fecha = ftime("%d/%m/%Y",time()); //--> se calcula la fecha
	initHighscore();

	splash=fpg_load("graphics/splash.fpg");
	fondo=fpg_load("graphics/fondo.fpg");

	entername_id=fpg_load("graphics/entername.fpg");

	NUMQUOTA=fnt_load("fonts/numquota.fnt");
	NUMTIME=fnt_load("fonts/numtime.fnt");
	NUMSCORE=fnt_load("fonts/numscore.fnt");
	NUMBIGSCORE=fnt_load("fonts/numbigscore.fnt");
	NUMLEVEL=fnt_load("fonts/numlevel.fnt");
	NUMJOYAS=fnt_load("fonts/numjoyas.fnt");

	rhiscore=fnt_load("fonts/rhiscore.fnt");
	qhiscore=fnt_load("fonts/qhiscore.fnt");
	ghiscore=fnt_load("fonts/ghiscore.fnt");
	whiscore=fnt_load("fonts/whiscore.fnt");
	yhiscore=fnt_load("fonts/yhiscore.fnt");


	NUMEROSQUOTA=fpg_load("graphics/numerosquota.fpg");
	NUMEROSGLASS=fpg_load("graphics/numerosglass.fpg");
	NUMEROSRUSH=fpg_load("graphics/numerosrush.fpg");

	marcador=fpg_load("graphics/marcador.fpg");
	gfx_hit=fpg_load("graphics/hit.fpg");
// JJP	BLANCONEGRO=blendop_new();   // --> ESCALADO DE GRISES... NO VA
// JJP	blendop_grayscale (BLANCONEGRO,1);   // --> ESCALADO DE GRISES... NO VA

	//--> CARGA DE SFX// FAKE PARA UNLOAD
	SFX_G1= sound_load("sfx/g1.wav");
	SFX_J1= sound_load("sfx/j1.wav");
	SFX_J2= sound_load("sfx/j2.wav");
	SFX_J3= sound_load("sfx/j3.wav");
	SFX_J4= sound_load("sfx/j4.wav");
	SFX_J5= sound_load("sfx/j5.wav");
	SFX_J6= sound_load("sfx/j6.wav");
	SFX_J7= sound_load("sfx/j7.wav");

	sfx_puzsion= sound_load("sfx/j7.wav");
	sfx_qmode= sound_load("sfx/j7.wav");
	sfx_rmode= sound_load("sfx/j7.wav");
	sfx_gmode= sound_load("sfx/j7.wav");
	sfx_options= sound_load("sfx/j7.wav");
	sfx_hiscore= sound_load("sfx/j7.wav");
	sfx_exitg= sound_load("sfx/j7.wav");
	sfx_start= sound_load("sfx/j7.wav");
	sfx_howto= sound_load("sfx/j7.wav");
	sfx_exit= sound_load("sfx/j7.wav");
	sfx_levelup= sound_load("sfx/j7.wav");
	sfx_fantastic= sound_load("sfx/j7.wav");
	sfx_great= sound_load("sfx/j7.wav");
	sfx_combo= sound_load("sfx/j7.wav");
	sfx_awesome= sound_load("sfx/j7.wav");
	SFX_SWEET= sound_load("sfx/j7.wav");
	sfx_perfect= sound_load("sfx/j7.wav");
	sfx_extra= sound_load("sfx/j7.wav");
	sfx_marve=sound_load("sfx/j7.wav");
	sfx_outs= sound_load("sfx/j7.wav");
	sfx_increible= sound_load("sfx/j7.wav");

	SFX_superb= sound_load("sfx/j7.wav");
	SFX_excellent= sound_load("sfx/j7.wav");
	SFX_priceless= sound_load("sfx/j7.wav");
	SFX_brillant= sound_load("sfx/j7.wav");
	SFX_smooth= sound_load("sfx/j7.wav");
	SFX_killer= sound_load("sfx/j7.wav");

	SFX_super= sound_load("sfx/j7.wav");
	SFX_hyper= sound_load("sfx/j7.wav");
	SFX_ultra= sound_load("sfx/j7.wav");
	SFX_giga= sound_load("sfx/j7.wav");
	SFX_mega=sound_load("sfx/j7.wav");

	sfx_hurryup = sound_load("sfx/j7.wav");
	sfx_gameover = sound_load("sfx/j7.wav");
	sfx_entername = sound_load("sfx/j7.wav");
	sfx_pause  = sound_load("sfx/j7.wav");
	sfx_nomoves = sound_load("sfx/j7.wav");
	sfx_destruction = sound_load("sfx/j7.wav");


	rand_seed(TIME());
	file=splash;
	x=160;y=120;z=-100;
	fade_off(1000);
	FRAME(5000);
	GRAPH=100;
	fade_on(1000);
	FRAME(20000);
	fade_off(1000);
	FRAME(15000);
	GRAPH=101;
	fade_on(1000);
	FRAME(20000);
	fade_off(1000);
	FRAME(15000);

	SPEAKER(saved[0].option);
	//--> Merge on
	if(fexists(get_pref_path("bennugd.org", "puzsion") + "newrhighscore.file"))
		mergerhighscore();
	end
	if(fexists(get_pref_path("bennugd.org", "puzsion") + "newqhighscore.file"))
		mergeqhighscore();
	end
	if(fexists(get_pref_path("bennugd.org", "puzsion") + "newghighscore.file"))
		mergeghighscore();
	end

	MENU(0);


	END

//****************************************************************
//***                   PROCESS MENU(MODO)                     ***
//****************************************************************

Process MENU(MODO) //--> EL PROCESO PRINCIPAL. ACTUALMENTE SOLO SIRVE DE LANZADOR Y MUERE
	PRIVATE
		int  CONTADOR; //--> VARIABLE DE SERVICIO
	END

	Begin
	let_me_alone();
	write_delete(all_text);
    clear_screen();
	MODE=MODO;
	RESET(1,57,1);
	RESET(1,57,2);
	RESET(1,57,3);
	SCORE=0;
	RELOJ=0;
	//--> FALTA EL LANZADOR DE MODOS/NIVELES
	SWITCH (MODE)

		CASE 0: //-->
			music_stop();
			music=music_load("music/musicintro.mp3");
			music_play(music,-1);
			tiempo_juego=GTM;
			INTRO(0);

		END

		CASE 1: //--> QUOTA MODE
			music_stop();
			music_unload(music); music=music_load("music/musicquota.mp3");music_play(music,-1);
			RELOJ+=75;
			NIVEL_GLOBAL=1;
			firstime=0;
			LEVEL_QUOTA(1);
			PUNTERO();
			combo(1,1,0);
			FOR(CONTADOR=1;CONTADOR<57;CONTADOR++);
			JEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,SORTEO[Rand(0,3)].NUMERO);
			END
			END

		CASE 2: //--> GLASS MODE
		    music_stop();
			music_unload(music);music= music_load("music/musicglass.mp3");music_play(music,-1);
			reloj=75;
			QJ1=0; QJ2=0; QJ3=0; QJ4=0;
			NIVEL_GLOBAL=1;
			LEVEL_glass(1);
			PUNTERO();
			combo(1,1,0);

		END
		CASE 3: //--> RUSH MODE
			music_stop();music_unload(music);music=music_load("music/musicrush.mp3");music_play(music,-1);
			RUSH_LEVEL=4;
			LEVEL_RUSH();
			PUNTERO();
			combo(1,1,0);
			FOR(CONTADOR=1;CONTADOR<57;CONTADOR++);
			JEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,SORTEO[Rand(0,3)].NUMERO);
			END
		END
	END

End

//****************************************************************
//***                PROCESS INTRO(OPCION)                     ***
//****************************************************************

PROCESS INTRO(opcion);
PRIVATE
STRUCT GTMOP[34];STRING TION;END=
"-12:00","-11:00","-10:00","-09:00","-08:00","-07:00","-06:00","-05:00","-04:30","-04:00","-03:30","-03:00","-02:00","-01:00","00:00","+01:00","+02:00","+03:00","+03:30","+04:00","+04:30","+05:00","+05:30","+05:45","+06:00","+06:30","+07:00","+08:00","+09:00","+09:30","+10:00","+11:00","+12:00","+13:00";
BYTE CAMBIO=0; int I;
BYTE CONTADOR=15; int contador2=0,CONTAOP1=0,CONTAOP2=0,CONTADOR3=0,CONTADOR4;INT FAKE;
STRING OPCION1="VOICE SELECTION", R1A="MALE" , R1B="FEMALE" , R1C="NONE";
STRING OPCION2="SOUND FX VOLUMEN SELECTION", R2A="OFF", R2B="1",R2C="2",R2D="3",R2E="4",R2F="5",R2G="6",R2H="7",R2I="8",R2J="9";
STRING OPCION3="MUSIC VOLUMEN SELECTIONN", R3A="OFF", R3B="1",R3C="2",R3D="3",R3E="4",R3F="5",R3G="6",R3H="7",R3I="8",R3J="9";
STRING OPCION4="GREENWICH MEAN TIME (GTM)",STRING RGTM;
STRING OPCION5="RESET HIGHSCORES TABLES", R5A="QUOTA MODE" , R5B="RUSH MODE" , R5C="GLASS MODE";
STRING OPCION6="SCREEN MODE", R6A="NORMAL" , R6B="2XSCALE" ,R6C="FULLSCR";

STRUCT OPCIONES[5] INT RESPUESTA[9];



BEGIN
	LET_ME_ALONE();
	write_delete(ALL_TEXT);
	CLEAR_SCREEN();
	puntero();
	file=splash;
	x=160;y=120;z=-100;
	if(os_id==OS_GP2X_WIZ) CONTADOR4=14; ELSE CONTADOR4=saved[3].option; END

	RGTM=GTMOP[CONTADOR4].TION;

	FROM CONTAOP1=0 TO 5;
		FROM CONTAOP2=0 TO 9;
		OPCIONES[CONTAOP1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
	END

SWITCH(OPCION)
CASE 0: // INTRO 1
	tiempo_juego=GTM;
	fade_on(1000);
	graph=1;
	WHILE(!MOUSE.LEFT)
	contador2++;
	if(contador2<30) graph=1; else graph=2; end
	if(contador2>60) contador2=0;end
	FRAME;
	END
	sound(200);//PUZSION
	graph=3;
	FRAME(1000);
	GRAPH=2;
	fade_off(1000);
	FRAME(10000);
	INTRO(1);
	RETURN;
END //--> CASE 0

CASE 1: //--> MENU GENERAL
	graph=4;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		// QUOTA MODE
		IF(MOUSE.X>25 AND MOUSE.X<100 AND MOUSE.Y>125 AND MOUSE.Y<170)
			sound(201);
			graph=5;
			FRAME(1000);
			GRAPH=4;
			fade_off(1000);
			FRAME(10000);
			INTRO(2);
			RETURN;
		END

		// RUSH MODE
		IF(MOUSE.X>126 AND MOUSE.X<188 AND MOUSE.Y>135 AND MOUSE.Y<183)
			sound(202);
			graph=6;
			FRAME(1000);
			GRAPH=4;
			fade_off(1000);
			FRAME(10000);
			INTRO(3);
			RETURN;
		END

		// GLASS MODE
		IF(MOUSE.X>212 AND MOUSE.X<288 AND MOUSE.Y>125 AND MOUSE.Y<170)
			sound(203);
			graph=7;
			FRAME(1000);
			GRAPH=4;
			fade_off(1000);
			FRAME(10000);
			INTRO(4);

			RETURN;
		END

		// OPTIONS
		IF(MOUSE.X>25 AND MOUSE.X<90 AND MOUSE.Y>200 AND MOUSE.Y<215)
			sound(204);
			graph=8;
			FRAME(1000);
			GRAPH=4;
			fade_off(1000);
			FRAME(10000);
			INTRO(5);
			RETURN;
		END

		// HIGH SCORES
		IF(MOUSE.X>117 AND MOUSE.X<197 AND MOUSE.Y>207 AND MOUSE.Y<223)
			sound(205);
			graph=9;
			FRAME(1000);
			GRAPH=4;
			fade_off(1000);
			FRAME(10000);
			INTRO(6);
			RETURN;
		END

		// EXIT
		IF(MOUSE.X>221 AND MOUSE.X<303 AND MOUSE.Y>199 AND MOUSE.Y<216)
			sound(206);
			graph=10;
			FRAME(1000);
			GRAPH=4;
			music_fade_OFF(1000);
			fade_off(1000);
			FRAME(10000);
			fade_on(1000);
			GRAPH=300;
			FRAME(50000);
			EXIT();
		END
	end //--> IF GENERAL
	end//--> LOOP
END //--> CASE 1

CASE 2: //--> QUOTA MODE
	graph=11;
	fade_on(1000);
Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>129 AND MOUSE.X<183 AND MOUSE.Y>152 AND MOUSE.Y<167) //--> START QUOTA MODE
			sound(210);
			graph=12;
			FRAME(1000);
			GRAPH=11;
			fade_off(1000);
			music_fade_OFF(1000);
			FRAME(10000);
			fade_on(1000);
			menu(1);
			RETURN;
		END

		IF(MOUSE.X>94 AND MOUSE.X<218 AND MOUSE.Y>179 AND MOUSE.Y<193) //--> QUOTA HOWTO
			sound(207);
			graph=13;
			FRAME(1000);
			GRAPH=11;
			fade_off(1000);
			FRAME(10000);
			fade_on(1000);
			INTRO(12);
			RETURN;

		END

		IF(MOUSE.X>138 AND MOUSE.X<176 AND MOUSE.Y>207 AND MOUSE.Y<223) //--> QUOTA EXIT
			sound(208);
			graph=14;
			FRAME(1000);
			GRAPH=11;

			fade_off(1000);
			FRAME(10000);
			INTRO(1);
			RETURN;
		END
	END

	//END
END //--> LOOP
END //--> CASE 2



CASE 3: //--> RUSH MODE
	graph=15;
	fade_on(1000);
Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>129 AND MOUSE.X<183 AND MOUSE.Y>152 AND MOUSE.Y<167) //--> START RUSH MODE
			sound(210);
			graph=16;
			FRAME(1000);
			GRAPH=15;

			fade_off(1000);
			music_fade_OFF(1000);
			FRAME(10000);
			fade_on(1000);
			menu(3);
			RETURN;
		END

		IF(MOUSE.X>94 AND MOUSE.X<218 AND MOUSE.Y>179 AND MOUSE.Y<193) //--> RUSH HOWTO
			sound(207);
			graph=17;
			FRAME(1000);
			GRAPH=15;
			fade_off(1000);
			FRAME(10000);
			fade_on(1000);
			INTRO(9);
			RETURN;

		END

		IF(MOUSE.X>138 AND MOUSE.X<176 AND MOUSE.Y>207 AND MOUSE.Y<223) //--> RUSH EXIT
			sound(208);
			graph=18;
			FRAME(1000);
			GRAPH=15;
			fade_off(1000);
			FRAME(10000);
			INTRO(1);
			RETURN;
		END
	END

	//END
END //--> LOOP
END //--> CASE 3




CASE 4: //--> GLASS MODE
	graph=19;
	fade_on(1000);
Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>129 AND MOUSE.X<183 AND MOUSE.Y>152 AND MOUSE.Y<167) //--> START GLASS MODE
			sound(210);
			graph=20;
			FRAME(1000);
			GRAPH=19;
			fade_off(1000);
			music_fade_OFF(1000);
			FRAME(10000);
			fade_on(1000);
			menu(2);
			RETURN;
		END


		IF(MOUSE.X>94 AND MOUSE.X<218 AND MOUSE.Y>179 AND MOUSE.Y<193) //--> GLASS HOWTO
			sound(207);
			graph=21;
			FRAME(1000);
			GRAPH=19;
			fade_off(1000);
			FRAME(10000);
			fade_on(1000);
			INTRO(15);
			RETURN;

		END

		IF(MOUSE.X>138 AND MOUSE.X<176 AND MOUSE.Y>207 AND MOUSE.Y<223) //--> GLASS EXIT
			sound(208);
			graph=22;
			FRAME(1000);
			GRAPH=19;
			fade_off(1000);
			FRAME(10000);
			INTRO(1);
			RETURN;
		END
	END

	//END
END //--> LOOP
END //--> CASE 4








CASE 5: //--> OPTIONS
	graph=60;
	fade_on(1000);

	LOOP
	IF(CAMBIO==0) CAMBIO=1; write_delete(ALL_TEXT);
	write_string(Yhiscore, 160,60,4,&OPCION1); // --> SPEAKER
		write_string(OPCIONES[0].RESPUESTA[0], 120,72,4,&R1A);
		Write_string(OPCIONES[0].RESPUESTA[1], 160,72,4,&R1B);
		write_string(OPCIONES[0].RESPUESTA[2], 200,72,4,&R1C);

	write_string(Yhiscore, 160,92,4,&OPCION2); //--> SFX VOLUMEN
		write_string(OPCIONES[1].RESPUESTA[0], 95,104,4,&R2A);
		write_string(OPCIONES[1].RESPUESTA[1], 115,104,4,&R2B);
		write_string(OPCIONES[1].RESPUESTA[2], 130,104,4,&R2C);
		write_string(OPCIONES[1].RESPUESTA[3], 145,104,4,&R2D);
		write_string(OPCIONES[1].RESPUESTA[4], 160,104,4,&R2E);
		write_string(OPCIONES[1].RESPUESTA[5], 175,104,4,&R2F);
		write_string(OPCIONES[1].RESPUESTA[6], 190,104,4,&R2G);
		write_string(OPCIONES[1].RESPUESTA[7], 205,104,4,&R2H);
		write_string(OPCIONES[1].RESPUESTA[8], 220,104,4,&R2I);
		write_string(OPCIONES[1].RESPUESTA[9], 235,104,4,&R2J);

	write_string(Yhiscore, 160,124,4,&OPCION3); //--> MUSIC VOLUMEN
		write_string(OPCIONES[2].RESPUESTA[0], 95,136,4,&R3A);
		write_string(OPCIONES[2].RESPUESTA[1], 115,136,4,&R3B);
		write_string(OPCIONES[2].RESPUESTA[2], 130,136,4,&R3C);
		write_string(OPCIONES[2].RESPUESTA[3], 145,136,4,&R3D);
		write_string(OPCIONES[2].RESPUESTA[4], 160,136,4,&R3E);
		write_string(OPCIONES[2].RESPUESTA[5], 175,136,4,&R3F);
		write_string(OPCIONES[2].RESPUESTA[6], 190,136,4,&R3G);
		write_string(OPCIONES[2].RESPUESTA[7], 205,136,4,&R3H);
		write_string(OPCIONES[2].RESPUESTA[8], 220,136,4,&R3I);
		write_string(OPCIONES[2].RESPUESTA[9], 235,136,4,&R3J);

		write_string(Yhiscore, 160,156,4,&OPCION5); // --> RESET RECORDS
		write_string(OPCIONES[3].RESPUESTA[0], 90,168,4,&R5A);
		Write_string(OPCIONES[3].RESPUESTA[1], 160,168,4,&R5B);
		write_string(OPCIONES[3].RESPUESTA[2], 230,168,4,&R5C);
	END
if(os_id==OS_GP2X_WIZ) // --> WIZ
	write_string(Yhiscore, 160,188,4,&OPCION4); //--> GTM SELECTION
		write_string(Whiscore, 160,200,4,&RGTM);


	write_string(Yhiscore, 160,215,4,&OPCION6); // --> SCREEN MODE
		write_string(Whiscore, 110,227,4,&R6A);
		Write_string(Whiscore, 160,227,4,&R6B);
		Write_string(Whiscore, 215,227,4,&R6C);
ELSE // --> WINDOWS LINUX, ETC...

	write_string(Yhiscore, 160,188,4,&OPCION4); //--> GTM SELECTION

		write_string(Rhiscore, 160,200,4,&RGTM);


	write_string(Yhiscore, 160,215,4,&OPCION6); // --> SCREEN MODE
		write_string(OPCIONES[5].RESPUESTA[0], 110,227,4,&R6A);
		Write_string(OPCIONES[5].RESPUESTA[1], 160,227,4,&R6B);
		Write_string(OPCIONES[5].RESPUESTA[2], 215,227,4,&R6C);
END
	// OPTION 1
	IF(MOUSE.LEFT AND MOUSE.X>106 AND MOUSE.X<136 AND MOUSE.Y>66 AND MOUSE.Y<79 or saved[0].option==1); //--> MALE
	OPCIONES[0].RESPUESTA[0]=Rhiscore;OPCIONES[0].RESPUESTA[1]=Qhiscore;OPCIONES[0].RESPUESTA[2]=Qhiscore; CAMBIO=0;
	saved[0].option=2;
	END
	IF(MOUSE.LEFT AND MOUSE.X>141 AND MOUSE.X<180 AND MOUSE.Y>66 AND MOUSE.Y<79 or saved[0].option==3); 	//--> FEMALE
	OPCIONES[0].RESPUESTA[0]=Qhiscore;OPCIONES[0].RESPUESTA[1]=Rhiscore;OPCIONES[0].RESPUESTA[2]=Qhiscore; CAMBIO=0;
	saved[0].option=4;
	END
	IF(MOUSE.LEFT AND MOUSE.X>186 AND MOUSE.X<215 AND MOUSE.Y>66 AND MOUSE.Y<79 or saved[0].option==5); //--> NONE
	OPCIONES[0].RESPUESTA[0]=Qhiscore;OPCIONES[0].RESPUESTA[1]=Qhiscore;OPCIONES[0].RESPUESTA[2]=Rhiscore; CAMBIO=0;
	saved[0].option=6;
	END

	// OPTION 2
	IF(MOUSE.LEFT AND MOUSE.X>85 AND MOUSE.X<105 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==0);//--> OFF
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[0]=Rhiscore;
		channel_set_volume(-1,SFX_VOL=0);
		CAMBIO=0;saved[1].option=0;
	END
	IF(MOUSE.LEFT AND MOUSE.X>110 AND MOUSE.X<120 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==8); //--> 1
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[1]=Rhiscore;
		CAMBIO=0;saved[1].option=8;
		channel_set_volume(-1,SFX_VOL=8);
	END
	IF(MOUSE.LEFT AND MOUSE.X>125 AND MOUSE.X<135 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==23); //--> 2
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[2]=Rhiscore;
		CAMBIO=0;saved[1].option=23;
		channel_set_volume(-1,SFX_VOL=23);
	END
	IF(MOUSE.LEFT AND MOUSE.X>140 AND MOUSE.X<150 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==38); //--> 3
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[3]=Rhiscore;
		CAMBIO=0;saved[1].option=38;
		channel_set_volume(-1,SFX_VOL=38);
	END
	IF(MOUSE.LEFT AND MOUSE.X>155 AND MOUSE.X<165 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==53); //--> 4
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[4]=Rhiscore;
		CAMBIO=0;saved[1].option=53;
		channel_set_volume(-1,SFX_VOL=53);
	END
	IF(MOUSE.LEFT AND MOUSE.X>170 AND MOUSE.X<180 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==68); //--> 5
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[5]=Rhiscore;
		CAMBIO=0;saved[1].option=68;
		channel_set_volume(-1,SFX_VOL=68);
	END
	IF(MOUSE.LEFT AND MOUSE.X>185 AND MOUSE.X<195 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==83);//--> 6
	FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[6]=Rhiscore;
		CAMBIO=0;saved[1].option=83;
		channel_set_volume(-1,SFX_VOL=83);
	END
	IF(MOUSE.LEFT AND MOUSE.X>200 AND MOUSE.X<210 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==98);//--> 7
	FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[7]=Rhiscore;
		CAMBIO=0;saved[1].option=98;
		channel_set_volume(-1,SFX_VOL=98);
	END
	IF(MOUSE.LEFT AND MOUSE.X>215 AND MOUSE.X<225 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==113);//--> 8
	FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[8]=Rhiscore;
		CAMBIO=0;saved[1].option=113;
		channel_set_volume(-1,SFX_VOL=113);
	END
	IF(MOUSE.LEFT AND MOUSE.X>230 AND MOUSE.X<240 AND MOUSE.Y>97 AND MOUSE.Y<111 or saved[1].option==128); //--> 9
		FROM CONTAOP2=0 TO 9;
			OPCIONES[1].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[1].RESPUESTA[9]=Rhiscore;
		CAMBIO=0;saved[1].option=128;
		channel_set_volume(-1,SFX_VOL=128);
	END

	// OPTION 3
	IF(MOUSE.LEFT AND MOUSE.X>85 AND MOUSE.X<105 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==0); //--> OFF
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[0]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=0);saved[2].option=0;
	END
	IF(MOUSE.LEFT AND MOUSE.X>110 AND MOUSE.X<120 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==8); //--> 1
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[1]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=8);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>125 AND MOUSE.X<135 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==23); //--> 2
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[2]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=23);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>140 AND MOUSE.X<150 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==38); //--> 3
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[3]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=38);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>155 AND MOUSE.X<165 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==53); //--> 4
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[4]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=53);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>170 AND MOUSE.X<180 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==68); //--> 5
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[5]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=68);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>185 AND MOUSE.X<195 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==83); //--> 6
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[6]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=83);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>200 AND MOUSE.X<210 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==98); //--> 7
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[7]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=98);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>215 AND MOUSE.X<225 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==113); //--> 8
	FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[8]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=113);saved[2].option=MUS_VOL;
	END
	IF(MOUSE.LEFT AND MOUSE.X>230 AND MOUSE.X<240 AND MOUSE.Y>130 AND MOUSE.Y<145 or saved[2].option==128); //--> 9
FROM CONTAOP2=0 TO 9;
			OPCIONES[2].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[2].RESPUESTA[9]=Rhiscore;
		CAMBIO=0;
		music_set_volume(MUS_VOL=128);saved[2].option=MUS_VOL;


	END

	// OPTION 4
	IF(MOUSE.LEFT AND MOUSE.X>60 AND MOUSE.X<121 AND MOUSE.Y>160 AND MOUSE.Y<175 AND OPCIONES[3].RESPUESTA[0]==Qhiscore); //--> QUOTA
		FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		FRAME(1000);OPCIONES[3].RESPUESTA[0]=Ghiscore;
		CAMBIO=0;
	END

	IF(MOUSE.LEFT AND MOUSE.X>60 AND MOUSE.X<121 AND MOUSE.Y>160 AND MOUSE.Y<175); //--> QUOTA
	FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[3].RESPUESTA[0]=Rhiscore;
		CAMBIO=0;
		 //File does not exist, fill the highscore table with standard values:
 for(i = 0;i < 10; i++ )
   //Fill in a generated score(lower index means higher score)
   qhighscore[i].hscore = (10-i)*10000;;
   qhighscore[i].hfase = (10-i);
   qhighscore[i].dia=fecha;
   qhighscore[i].tiempo=ftime("%H:%M:%S",GTM+(10-i)*10);
 end
   FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
	qhighscore[CONTADOR].hname[0]="Q";
	qhighscore[CONTADOR].hname[1]="U";
	qhighscore[CONTADOR].hname[2]="Z";
	qhighscore[CONTADOR].hname[3]="S";
	qhighscore[CONTADOR].hname[4]="I";
	qhighscore[CONTADOR].hname[5]="O";
	qhighscore[CONTADOR].hname[6]="N";
	END

   	SORT(qhighscore,10);
	save(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file",qhighscore);
end



	IF(MOUSE.LEFT AND MOUSE.X>130 AND MOUSE.X<190 AND MOUSE.Y>160 AND MOUSE.Y<175 AND OPCIONES[3].RESPUESTA[1]==Qhiscore); //--> RUSH
		FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		FRAME(1000);OPCIONES[3].RESPUESTA[1]=Ghiscore;
		CAMBIO=0;
	END

IF(MOUSE.LEFT AND MOUSE.X>130 AND MOUSE.X<190 AND MOUSE.Y>160 AND MOUSE.Y<175 AND OPCIONES[3].RESPUESTA[1]==Ghiscore); //--> RUSH
	FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[3].RESPUESTA[1]=Rhiscore;
		CAMBIO=0;

//File does not exist, fill the highscore table with standard values:
 for(i = 0;i < 10; i++ )
   //Fill in a generated score(lower index means higher score)
   rhighscore[i].hscore = (10-i)*10000;;
   rhighscore[i].hfase = (10-i);
   rhighscore[i].dia=fecha;
   rhighscore[i].tiempo=ftime("%H:%M:%S",GTM+(10-i)*10);
 end
   FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
	rhighscore[CONTADOR].hname[0]="R";
	rhighscore[CONTADOR].hname[1]="U";
	rhighscore[CONTADOR].hname[2]="Z";
	rhighscore[CONTADOR].hname[3]="S";
	rhighscore[CONTADOR].hname[4]="I";
	rhighscore[CONTADOR].hname[5]="O";
	rhighscore[CONTADOR].hname[6]="N";
	END

   SORT(rhighscore,10);
   save(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file",rhighscore);

	END

	IF(MOUSE.LEFT AND MOUSE.X>200 AND MOUSE.X<264 AND MOUSE.Y>160 AND MOUSE.Y<175 AND OPCIONES[3].RESPUESTA[2]==Qhiscore); //--> GLASS
		FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		FRAME(1000);OPCIONES[3].RESPUESTA[2]=Ghiscore;
		CAMBIO=0;
	END

	IF(MOUSE.LEFT AND MOUSE.X>200 AND MOUSE.X<264 AND MOUSE.Y>160 AND MOUSE.Y<175); //--> GLASS
	FROM CONTAOP2=0 TO 2;
			OPCIONES[3].RESPUESTA[CONTAOP2]=Qhiscore;
		END
		OPCIONES[3].RESPUESTA[2]=Rhiscore;
		CAMBIO=0;
	 //File does not exist, fill the highscore table with standard values:
 	for(i = 0;i < 10; i++ )
   		//Fill in a generated score(lower index means higher score)
   		ghighscore[i].hscore = (10-i)*10000;;
   		ghighscore[i].hfase = (10-i);
   		ghighscore[i].dia=fecha;
   		ghighscore[i].tiempo=ftime("%H:%M:%S",GTM+(10-i)*10);

 	end
   	FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
		ghighscore[CONTADOR].hname[0]="G";
		ghighscore[CONTADOR].hname[1]="U";
		ghighscore[CONTADOR].hname[2]="Z";
		ghighscore[CONTADOR].hname[3]="S";
		ghighscore[CONTADOR].hname[4]="I";
		ghighscore[CONTADOR].hname[5]="O";
		ghighscore[CONTADOR].hname[6]="N";
	END
   	SORT(ghighscore,10);
   	save(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file",ghighscore);




	END

	// OPTION 5
	IF(MOUSE.LEFT AND os_id<>8 AND MOUSE.X>140 AND MOUSE.X<210 AND MOUSE.Y>190 AND MOUSE.Y<210);

	RGTM=GTMOP[CONTADOR4].TION;
	FRAME(2000);
																										//14
	SWITCH(CONTADOR4)
	CASE 0: GTM=43200;END   //-12
	CASE 1: GTM=39600;END   // -11
	CASE 2: GTM=36000;END  // -10
	CASE 3: GTM=32400;END  // -9
	CASE 4: GTM=28800;END    // -8
	CASE 5: GTM=25200;END  // -7
	CASE 6: GTM=21600;END  // -6
	CASE 7: GTM=18000;END  // -5
	CASE 8: GTM=16200;END  //- 4.3
	CASE 9: GTM=14400;END   // -4
	CASE 10: GTM=12600;END // -3.3
	CASE 11: GTM=10800;END  // -3
	CASE 12: GTM=7200;END  // -2
	CASE 13: GTM=3600;END // -1
	CASE 14: GTM=0;END // 0
	CASE 15: GTM=82800;END // 1
	CASE 16: GTM=79200;END // 2
	CASE 17: GTM=75600;END // 3
	CASE 18: GTM=73800;END // 3.5
	CASE 19: GTM=72000;END // 4
	CASE 20: GTM=70200;END // 4.3
	CASE 21: GTM=68400;END // 5
	CASE 22: GTM=66600;END // 5.3
	CASE 23: GTM=65700;END // 5.45
	CASE 24: GTM=64800;END // 6
	CASE 25: GTM=63000;END // 6.3
	CASE 26: GTM=61200;END // 7
	CASE 27: GTM=57600;END // 8
	CASE 28: GTM=54000;END //9
	CASE 29: GTM=52200;END // 9.3
	CASE 30: GTM=50400;END //10
	CASE 31: GTM=46800;END //11
	CASE 32: GTM=43200;END //12
	CASE 33: GTM=39600;END  //13
	END// SWITCH
	saved[3].option=coNTADOR4;
	CONTADOR4++;IF(CONTADOR4>33) CONTADOR4=0; END
	END//--> GTM

	// OPTION 6
	IF(MOUSE.LEFT AND os_id<>8 AND MOUSE.X>92 AND MOUSE.X<128 AND MOUSE.Y>224 AND MOUSE.Y<239 OR saved[4].option==1); //--> NORMAL
	   screen.scale_resolution = -1; screen.scale_resolution_aspectratio = SRA_PRESERVE; OPCIONES[5].RESPUESTA[0]=Rhiscore;OPCIONES[5].RESPUESTA[1]=Qhiscore;OPCIONES[5].RESPUESTA[2]=Qhiscore;CAMBIO=0; set_mode(320,240,0); MOUSE.LEFT=0;saved[4].option=2;
    END
	IF(MOUSE.LEFT AND os_id<>8 AND MOUSE.X>140 AND MOUSE.X<182 AND MOUSE.Y>224 AND MOUSE.Y<239 OR saved[4].option==3); //--> 2XSCALE
	   screen.scale_resolution = 06400480; screen.scale_resolution_aspectratio = SRA_PRESERVE; OPCIONES[5].RESPUESTA[0]=Qhiscore;OPCIONES[5].RESPUESTA[1]=Rhiscore;OPCIONES[5].RESPUESTA[2]=Qhiscore;CAMBIO=0; set_mode(320,240); MOUSE.LEFT=0; saved[4].option=4;
    END
	IF(MOUSE.LEFT AND os_id<>8 AND MOUSE.X>195 AND MOUSE.X<255 AND MOUSE.Y>224 AND MOUSE.Y<239 OR saved[4].option==5); //--> FULLSCREEN
	   screen.scale_resolution = -1; screen.scale_resolution_aspectratio = SRA_PRESERVE; OPCIONES[5].RESPUESTA[0]=Qhiscore;OPCIONES[5].RESPUESTA[1]=Qhiscore;OPCIONES[5].RESPUESTA[2]=Rhiscore;CAMBIO=0; set_mode(320,240,mode_fullscreen); MOUSE.LEFT=0; saved[4].option=6;
    END



		IF(MOUSE.LEFT AND MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> OPTIONS EXIT
			sound(208);
			graph=61;
			FRAME(1000);
			GRAPH=60;
			fade_off(1000);
			FRAME(10000);
			saved[0].option--;
			saved[4].option--;
			save(get_pref_path("bennugd.org", "puzsion") + "options.file",saved);
			SPEAKER(saved[0].option);
			INTRO(0); RETURN;
		END

	FRAME;

	END	//
END //--> CASE 5



CASE 6: //--> QUOTA HIGHSCORES
	graph=23;
	printqhighscore();
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HIGHSCORES QUOTA EXIT
			sound(208);
			graph=24;
			FRAME(1000);
			GRAPH=23;
			fade_off(1000);
			FRAME(10000);
			INTRO(0); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> GO TO HIGHSCORES RUSH
			sound(202);
			graph=25;
			FRAME(1000);
			GRAPH=23;
			fade_off(1000);
			FRAME(10000);
			INTRO(7); RETURN;
		END
	END
END //--> CASE 6
END
CASE 7: //--> RUSH HIGHSCORES
	graph=29;
	printrhighscore();
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HIGHSCORES RUSH EXIT
			sound(208);
			graph=30;
			FRAME(1000);
			GRAPH=29;
			fade_off(1000);
			FRAME(10000);
			INTRO(0); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> GO TO HIGHSCORES GLASS
			sound(203);
			graph=31;
			FRAME(1000);
			GRAPH=29;
			fade_off(1000);
			FRAME(10000);
			INTRO(8); RETURN;
		END
	END
END //--> CASE 7
END

CASE 8: //--> GLASS HIGHSCORES
	graph=26;
	printghighscore();
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HIGHSCORES GLASS EXIT
			sound(208);
			graph=27;
			FRAME(1000);
			GRAPH=26;
			fade_off(1000);
			FRAME(10000);
			INTRO(0); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> GO TO HIGHSCORES QUOTA
			sound(201);
			graph=28;
			FRAME(1000);
			GRAPH=26;
			fade_off(1000);
			FRAME(10000);
			INTRO(6); RETURN;
		END
	END
END //--> CASE 8
END

CASE 9: //--> HOW TO RUSH A
	graph=80;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=81;
			FRAME(1000);
			GRAPH=80;
			fade_off(1000);
			FRAME(10000);
			INTRO(3); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=82;
			FRAME(1000);
			GRAPH=80;
			fade_off(1000);
			FRAME(10000);
			INTRO(10); RETURN;
		END
	END
END //--> CASE 9
END

CASE 10: //--> HOW TO RUSH B
	graph=83;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=84;
			FRAME(1000);
			GRAPH=83;
			fade_off(1000);
			FRAME(10000);
			INTRO(3); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=85;
			FRAME(1000);
			GRAPH=83;
			fade_off(1000);
			FRAME(10000);
			INTRO(11); RETURN;
		END
	END
END //--> CASE 10
END

CASE 11: //--> HOW TO RUSH C
	graph=86;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=87;
			FRAME(1000);
			GRAPH=86;
			fade_off(1000);
			FRAME(10000);
			INTRO(3); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=88;
			FRAME(1000);
			GRAPH=86;
			fade_off(1000);
			FRAME(10000);
			INTRO(9); RETURN;
		END
	END
END //--> CASE 11
END
//-----------------------------------------------

CASE 12: //--> HOW TO RUSH A
	graph=70;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=71;
			FRAME(1000);
			GRAPH=70;
			fade_off(1000);
			FRAME(10000);
			INTRO(2); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=72;
			FRAME(1000);
			GRAPH=70;
			fade_off(1000);
			FRAME(10000);
			INTRO(13); RETURN;
		END
	END
END //--> CASE 12
END

CASE 13: //--> HOW TO RUSH B
	graph=73;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=74;
			FRAME(1000);
			GRAPH=73;
			fade_off(1000);
			FRAME(10000);
			INTRO(2); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=75;
			FRAME(1000);
			GRAPH=73;
			fade_off(1000);
			FRAME(10000);
			INTRO(14); RETURN;
		END
	END
END //--> CASE 13
END

CASE 14: //--> HOW TO RUSH C
	graph=76;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=77;
			FRAME(1000);
			GRAPH=76;
			fade_off(1000);
			FRAME(10000);
			INTRO(2); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=78;
			FRAME(1000);
			GRAPH=76;
			fade_off(1000);
			FRAME(10000);
			INTRO(12); RETURN;
		END
	END
END //--> CASE 14
END

///------------//-----------//------------///
CASE 15: //--> HOW TO RUSH A
	graph=90;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=91;
			FRAME(1000);
			GRAPH=90;
			fade_off(1000);
			FRAME(10000);
			INTRO(4); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=92;
			FRAME(1000);
			GRAPH=90;
			fade_off(1000);
			FRAME(10000);
			INTRO(16); RETURN;
		END
	END
END //--> CASE 15
END

CASE 16: //--> HOW TO RUSH B
	graph=93;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=94;
			FRAME(1000);
			GRAPH=93;
			fade_off(1000);
			FRAME(10000);
			INTRO(4); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=95;
			FRAME(1000);
			GRAPH=93;
			fade_off(1000);
			FRAME(10000);
			INTRO(17); RETURN;
		END
	END
END //--> CASE 16
END

CASE 17: //--> HOW TO RUSH C
	graph=96;
	fade_on(1000);
	Loop
	frame;
	IF(MOUSE.LEFT)
		IF(MOUSE.X>10 AND MOUSE.X<43 AND MOUSE.Y>14 AND MOUSE.Y<30) //--> HOW TO RUSH EXIT
			sound(208);
			graph=97;
			FRAME(1000);
			GRAPH=96;
			fade_off(1000);
			FRAME(10000);
			INTRO(4); RETURN;
		END

		IF(MOUSE.X>275 AND MOUSE.X<303 AND MOUSE.Y>11 AND MOUSE.Y<35) //--> NEXT
			sound(207);
			graph=98;
			FRAME(1000);
			GRAPH=96;
			fade_off(1000);
			FRAME(10000);
			INTRO(15); RETURN;
		END
	END
END //--> CASE 14
END

END //--> SWITCH
END


//****************************************************************
//***                   PROCESS ENTERNAME                      ***
//****************************************************************

PROCESS ENTERNAME(GRAPH,double X,Y,int MODOJUEGO);
PRIVATE
int CONTADOR=0,CONTADOR2, CONTADOR3;//LLETRA0;LLETRA1;LLETRA2;LLETRA3;LLETRA4;LLETRA5;LLETRA6;
BEGIN
	CONTADOR2=GRAPH;CONTADOR3=GRAPH+50;
	FILE=ENTERNAME_ID;
	IF(Y<TABLERO[15].Y) SIZE=50; END
	LOOP
	IF(MODOJUEGO==0)
		CONTADOR=RAND(1,2);
		IF(CONTADOR==1) GRAPH=CONTADOR2; END
		IF(CONTADOR==2) GRAPH=CONTADOR3; END
	END //--> IF

	IF(MODOJUEGO==4)
	SWITCH(X)
		CASE TABLERO[8].X:	  GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(12000); GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+15: FRAME(1000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(11000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+30: FRAME(2000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(10000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+45: FRAME(3000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(9000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+60: FRAME(4000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(8000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END

		CASE TABLERO[8].X+90: FRAME(5000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(7000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+105: FRAME(6000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(6000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+120: FRAME(7000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(5000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+135: FRAME(8000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(4000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END

		CASE TABLERO[8].X+165: FRAME(9000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(3000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+180: FRAME(10000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(2000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+195: FRAME(11000);GRAPH+=50; FRAME(500); GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
		CASE TABLERO[8].X+210: FRAME(12000);GRAPH+=50; FRAME(500); GRAPH-=50; GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);GRAPH+=50; FRAME(1000);GRAPH-=50; FRAME(1000);END
	END	//--> SWITCH
	END //--> IF

	IF (NOT MOUSE.LEFT) PUSHED=0; END
	IF(MOUSE.LEFT AND collision(TYPE MOUSE) AND Y>TABLERO[22].Y AND NOMBRE<7 AND PUSHED==0)
	PUSHED=1;
	GRAPH+=50;
	sound(150);
	FRAME(1500);
	GRAPH-=50;
	SWITCH(MODOJUEGO)
	CASE 1:
	SWITCH(GRAPH)
		CASE 1: qhighscore[0].hname[NOMBRE]="A"; END
		CASE 2: qhighscore[0].hname[NOMBRE]="B"; END
		CASE 3: qhighscore[0].hname[NOMBRE]="C"; END
		CASE 4: qhighscore[0].hname[NOMBRE]="D"; END
		CASE 5: qhighscore[0].hname[NOMBRE]="E"; END
		CASE 6: qhighscore[0].hname[NOMBRE]="F"; END
		CASE 7: qhighscore[0].hname[NOMBRE]="G"; END
		CASE 8: qhighscore[0].hname[NOMBRE]="H"; END
		CASE 9: qhighscore[0].hname[NOMBRE]="I"; END
		CASE 10: qhighscore[0].hname[NOMBRE]="J"; END
		CASE 11: qhighscore[0].hname[NOMBRE]="K"; END
		CASE 12: qhighscore[0].hname[NOMBRE]="L"; END
		CASE 13: qhighscore[0].hname[NOMBRE]="M"; END
		CASE 14: qhighscore[0].hname[NOMBRE]="N"; END
		CASE 15: qhighscore[0].hname[NOMBRE]="O"; END
		CASE 16: qhighscore[0].hname[NOMBRE]="P"; END
		CASE 17: qhighscore[0].hname[NOMBRE]="Q"; END
		CASE 18: qhighscore[0].hname[NOMBRE]="R"; END
		CASE 19: qhighscore[0].hname[NOMBRE]="S"; END
		CASE 20: qhighscore[0].hname[NOMBRE]="T"; END
		CASE 21: qhighscore[0].hname[NOMBRE]="U"; END
		CASE 22: qhighscore[0].hname[NOMBRE]="V"; END
		CASE 23: qhighscore[0].hname[NOMBRE]="W"; END
		CASE 24: qhighscore[0].hname[NOMBRE]="X"; END
		CASE 25: qhighscore[0].hname[NOMBRE]="Y"; END
		CASE 26: qhighscore[0].hname[NOMBRE]="Z"; END
		CASE 27: qhighscore[0].hname[NOMBRE]=" "; END
	END
	END //--> CASE 1



	CASE 2:
	SWITCH(GRAPH)
		CASE 1: ghighscore[0].hname[NOMBRE]="A"; END
		CASE 2: ghighscore[0].hname[NOMBRE]="B"; END
		CASE 3: ghighscore[0].hname[NOMBRE]="C"; END
		CASE 4: ghighscore[0].hname[NOMBRE]="D"; END
		CASE 5: ghighscore[0].hname[NOMBRE]="E"; END
		CASE 6: ghighscore[0].hname[NOMBRE]="F"; END
		CASE 7: ghighscore[0].hname[NOMBRE]="G"; END
		CASE 8: ghighscore[0].hname[NOMBRE]="H"; END
		CASE 9: ghighscore[0].hname[NOMBRE]="I"; END
		CASE 10: ghighscore[0].hname[NOMBRE]="J"; END
		CASE 11: ghighscore[0].hname[NOMBRE]="K"; END
		CASE 12: ghighscore[0].hname[NOMBRE]="L"; END
		CASE 13: ghighscore[0].hname[NOMBRE]="M"; END
		CASE 14: ghighscore[0].hname[NOMBRE]="N"; END
		CASE 15: ghighscore[0].hname[NOMBRE]="O"; END
		CASE 16: ghighscore[0].hname[NOMBRE]="P"; END
		CASE 17: ghighscore[0].hname[NOMBRE]="Q"; END
		CASE 18: ghighscore[0].hname[NOMBRE]="R"; END
		CASE 19: ghighscore[0].hname[NOMBRE]="S"; END
		CASE 20: ghighscore[0].hname[NOMBRE]="T"; END
		CASE 21: ghighscore[0].hname[NOMBRE]="U"; END
		CASE 22: ghighscore[0].hname[NOMBRE]="V"; END
		CASE 23: ghighscore[0].hname[NOMBRE]="W"; END
		CASE 24: ghighscore[0].hname[NOMBRE]="X"; END
		CASE 25: ghighscore[0].hname[NOMBRE]="Y"; END
		CASE 26: ghighscore[0].hname[NOMBRE]="Z"; END
		CASE 27: ghighscore[0].hname[NOMBRE]=" "; END
	END
	END //--> CASE 2

	CASE 3:
	SWITCH(GRAPH)
		CASE 1: rhighscore[0].hname[NOMBRE]="A"; END
		CASE 2: rhighscore[0].hname[NOMBRE]="B"; END
		CASE 3: rhighscore[0].hname[NOMBRE]="C"; END
		CASE 4: rhighscore[0].hname[NOMBRE]="D"; END
		CASE 5: rhighscore[0].hname[NOMBRE]="E"; END
		CASE 6: rhighscore[0].hname[NOMBRE]="F"; END
		CASE 7: rhighscore[0].hname[NOMBRE]="G"; END
		CASE 8: rhighscore[0].hname[NOMBRE]="H"; END
		CASE 9: rhighscore[0].hname[NOMBRE]="I"; END
		CASE 10: rhighscore[0].hname[NOMBRE]="J"; END
		CASE 11: rhighscore[0].hname[NOMBRE]="K"; END
		CASE 12: rhighscore[0].hname[NOMBRE]="L"; END
		CASE 13: rhighscore[0].hname[NOMBRE]="M"; END
		CASE 14: rhighscore[0].hname[NOMBRE]="N"; END
		CASE 15: rhighscore[0].hname[NOMBRE]="O"; END
		CASE 16: rhighscore[0].hname[NOMBRE]="P"; END
		CASE 17: rhighscore[0].hname[NOMBRE]="Q"; END
		CASE 18: rhighscore[0].hname[NOMBRE]="R"; END
		CASE 19: rhighscore[0].hname[NOMBRE]="S"; END
		CASE 20: rhighscore[0].hname[NOMBRE]="T"; END
		CASE 21: rhighscore[0].hname[NOMBRE]="U"; END
		CASE 22: rhighscore[0].hname[NOMBRE]="V"; END
		CASE 23: rhighscore[0].hname[NOMBRE]="W"; END
		CASE 24: rhighscore[0].hname[NOMBRE]="X"; END
		CASE 25: rhighscore[0].hname[NOMBRE]="Y"; END
		CASE 26: rhighscore[0].hname[NOMBRE]="Z"; END
		CASE 27: rhighscore[0].hname[NOMBRE]=" "; END
	END
	END //--> CASE 3
	END //--> SWITCH

	ENTERNAME(GRAPH,TABLERO[NOMBRE+15].X,TABLERO[NOMBRE+15].Y,0);
	NOMBRE++;
	END
	SWITCH(GRAPH)
		CASE 29..30:
		if(nombre>0 and x==TABLERO[22].X) GRAPH=30; end
		if(nombre>6 and x==TABLERO[28].X) GRAPH=30; end
		end
		CASE 31..32:
		if(nombre>1 and x==TABLERO[23].X) GRAPH=32; end
		if(nombre>5 and x==TABLERO[27].X) GRAPH=32; end
		end
		CASE 33..34:
		if(nombre>2 and x==TABLERO[24].X) GRAPH=34; end
		if(nombre>4 and x==TABLERO[26].X) GRAPH=34; end
				end
		CASE 35..36:
		if(nombre>3 and x==TABLERO[25].X) GRAPH=36;end
		end





	end
	FRAME;
	END
END



//****************************************************************
//***                   PROCESS FAKEJEWEL                      ***
//****************************************************************

PROCESS FAKEJEWEL(casilla,double X,Y, int FGRAPH,BRILLA);
PRIVATE
BEGIN
	FILE=JOYAS;
	//z=-150;
	FRAME(1000);
	GRAPH=FGRAPH;
	FRAME(1000);
	GRAPH--;
	FRAME(1000);
	GRAPH--;
	FRAME(1000);
	GRAPH--;
	FRAME(1000);
	GRAPH--;
	IF(BRILLA==1);
		FRAME(1000);
	GRAPH++;
	else
	flags=4;
	END


	FRAME(10000);
	// MAX ILUM
	GRAPH++;
	FRAME(1000);
	GRAPH++;
	FRAME(1000);
	GRAPH++;
	FRAME(1000);
	GRAPH++;
	//FRAME(1000);

END

//****************************************************************
//***                    PROCESS JEWEL                         ***
//****************************************************************

PROCESS JEWEL(casilla,pos_X,pos_Y,pos_GRAPH);
PRIVATE
//--> LAS VARIABLES 'CONTADOR' SON DE SERVICIO. LAS USO SIEMPRE PARA CONTAR DENTRO DE BUCLES, ITERACIONES O AZAR
int CONTADOR,CONTADOR2,contador3,EXTRATIME=0;
int DESTROYED=0;
BEGIN

	x=pos_x;
	y=pos_y;
	graph=pos_graph;


	//--> SE ACTUALIZA EL STRUCT TABLERO CON EL GRAFICO PASADO AL CREAR LA JEWEL
	TABLERO[CASILLA].GRAPH=GRAPH;

	LOOP
		//--> INTERESA MANTENER DENTRO DEL LOOP ESTAS VARIABLES (QUE PUEDEN CAMBIAR INGAME) PARA QUE SE ACTUALIZEN A CADA FRAME
		file=JOYAS;
		GRAPH=TABLERO[CASILLA].GRAPH;
		Y=TABLERO[CASILLA].Y;
		X=TABLERO[CASILLA].X;

	IF(ENCIENDEME==1) FLAGS=0; END

		//--> SI LA PRIMERA LINEA DE JOYAS (QUE NO SE VE) SE ELIMINA O BAJA, SE CREAN NUEVAS JOYAS AL AZAR PARA "ALIMENTAR EL TABLERO"
		IF(GRAPH==0 && (CASILLA==1 OR CASILLA==2 OR CASILLA==3 OR CASILLA==4 OR CASILLA==5 OR CASILLA==6 OR CASILLA==7))
			TABLERO[CASILLA].GRAPH=SORTEO[Rand(0,3)].NUMERO;

		END

		//---> LISTA DE EFECTOS DIFERENTES AL SELECCIONAR LA JOYA	(ACTUALMENTE SOLO SE USA 1)

		SWITCH(TIPO_GLOW)
		CASE 1: if(TABLERO[CASILLA].GLOW<>0) graph=graph+1;X+=RAND(-1,1); Y+=RAND(-1,1); end; END
		CASE 2: If(TABLERO[CASILLA].GLOW<>0) graph=graph+1;X+=RAND(-2,2); Y+=RAND(-2,2); end; END
		CASE 3: If(TABLERO[CASILLA].GLOW<>0) graph=graph+1; end; END
		CASE 4: If(TABLERO[CASILLA].GLOW<>0) X+=RAND(-2,2); Y+=RAND(-2,2); end; END
		CASE 5: If(TABLERO[CASILLA].GLOW<>0) X+=RAND(-1,1); Y+=RAND(-1,1); end; END
		CASE 6: if(TABLERO[CASILLA].GLOW<>0) FLAGS=RAND(0,4); else FLAGS=0; end; END
		CASE 7: if(TABLERO[CASILLA].GLOW<>0) graph=graph+RAND(0,1); end; END
		CASE 8: if(TABLERO[CASILLA].GLOW<>0) graph=graph+RAND(0,1);X+=RAND(-1,1); Y+=RAND(-1,1); end; END
		CASE 9: If(TABLERO[CASILLA].GLOW<>0) graph=graph+RAND(0,1);X+=RAND(-2,2); Y+=RAND(-2,2); end; END
		CASE 10:If(TABLERO[CASILLA].GLOW<>0) ANGLE+=RAND(0,-90000); ELSE ANGLE=0; end; END
		CASE 11:If(TABLERO[CASILLA].GLOW<>0) ANGLE+=RAND(0,-90000); graph=graph+1;  ELSE ANGLE=0;end; END
		CASE 12:If(TABLERO[CASILLA].GLOW<>0) ANGLE+=RAND(0,-90000); graph=graph+1;X+=RAND(-1,1); Y+=RAND(-1,1); ELSE ANGLE=0;end; END
		CASE 13:If(TABLERO[CASILLA].GLOW<>0) ANGLE+=RAND(0,-90000); graph=graph+1;X+=RAND(-2,2); Y+=RAND(-2,2); ELSE ANGLE=0;end; END
		END
		//--> CUANDO QUEDA POCO TIEMPO.... SHAKE!

		if(reloj<21 and reloj>5) X+=RAND(-1,1); Y+=RAND(-1,1); end
		if(reloj<6) X+=RAND(-2,2); Y+=RAND(-2,2); end
		IF(RELOJ=<0) /*blendop_assign(JOYAS,GRAPH,BLANCONEGRO);*/
            color_r=color_g=color_b=128;
        END




		//--> SI LA JOYA QUEDA MARCADA PARA DESTRUIR SE ELIMINA Y DEPENDIENDO DEL MODO DE JUEGO SE CREA O SE HACE UN "AGUJERO"
		If(TABLERO[CASILLA].DESTRUIR==1)
				Y=TABLERO[CASILLA].Y;
				X=TABLERO[CASILLA].X;
				SIZE=100;
				DESTROYED++;
				if(tipo_glow==1 OR tipo_glow==2 or tipo_glow==3 or tipo_glow==7 or tipo_glow==8 or tipo_glow==9 or tipo_glow==10 or tipo_glow==11 or tipo_glow==12)
                    graph=graph-1;
                End
				//--> CONTADOR DE ROTURA DE JOYAS PAREA QUOTA
				SWITCH (GRAPH)
					CASE 1:   if(QJ1>0 or mode==2) QJ1--; END; END
					CASE 11:  if(QJ2>0 or mode==2) QJ2--; END; END
					CASE 21:  if(QJ3>0 or mode==2) QJ3--; END; END
					CASE 31:  if(QJ4>0 or mode==2) QJ4--; END; END
				END

				//--> ANIMACION DE DESTRUCCION DE JOYA 283
				FLY(X,Y,Z,DIFERENCIA,NUMJOYAS);sound(0);HIT(X+RAND(-8,8),Y+RAND(-8,8),Z);
				signal(type combo,s_kill);
				combo(candidato,1,0);
				FRAME;
				for(contador3=1;contador3<FRAMES_DESTRUCCION;contador3++)
				    TABLERO[CASILLA].GRAPH++;GRAPH=TABLERO[CASILLA].GRAPH;
				    FRAME(1000);
				end
				TABLERO[CASILLA].GRAPH=0;
				GRAPH=TABLERO[CASILLA].GRAPH;
				TABLERO[CASILLA].DESTRUIR=0;




				//--> ESTA LINEA DETERMINA EL MODO ILUMINAR PANEL 303
				IF(mode==2 AND DESTROYED=>QUOTA_DESTROYED)
					if(extratime==0) reloj+=NUM_NIVEL;FLYT(38,128,Z,NUM_NIVEL); extratime=1; end
					FLAGS=4; TABLERO[CASILLA].ILUM=1; ELSE FLAGS=0; extratime=0; END

				//--> MODO REFILL 1: LA JOYA DESTRUIDA SE RELLENA AL AZAR NOTA: EL GRAPH=0 NUNCA LLEGA A REALIZARSE (NO FRAME) POR LO QUE NO SE ACTIVA EL EFECTO FALL
				IF(REFILL==1)
					TABLERO[CASILLA].GRAPH=SORTEO[Rand(0,3)].NUMERO;
					TABLERO[CASILLA].GRAPH+=FRAMES_DESTRUCCION-1;
					GRAPH=TABLERO[CASILLA].GRAPH;
					FRAME(1000);
					TABLERO[CASILLA].GRAPH--;
					GRAPH=TABLERO[CASILLA].GRAPH;
					FRAME(1000);
					TABLERO[CASILLA].GRAPH--;
					GRAPH=TABLERO[CASILLA].GRAPH;
					FRAME(1000);
					TABLERO[CASILLA].GRAPH--;
					GRAPH=TABLERO[CASILLA].GRAPH;
					FRAME(1000);
					TABLERO[CASILLA].GRAPH--;
					GRAPH=TABLERO[CASILLA].GRAPH;


					FRAME(1000);
				END
		END

		//--> MODO REFILL 2: JA JOYA DESTRUIDA CREA UN AGUJERO QUE ES RELLENADO POR LA JOYA DE ARRIBA CREANDO UN EFECTO CAIDA. SE ACTIVA CON GRAPH=0
		WHILE (TABLERO[TABLERO[CASILLA].ABAJO].GRAPH==0  and TABLERO[CASILLA].GRAPH<>0 and REFILL==2 and (EXISTS(ID_EXPLOSION)<>1 OR STOP_REMOVER==1));//--> SI SE DECTA QUE LA JOYA DE ABAJO ES DESTRUIDA LA ACTUAL SE DESPLAZA HACIA ABAJO HASTA SUSTITUIR A LA DESTRUIDA
			REPEAT
				Y++;
				FRAME(10);
			UNTIL (Y==TABLERO[TABLERO[CASILLA].ABAJO].Y)

			TABLERO[TABLERO[CASILLA].ABAJO].GRAPH=TABLERO[CASILLA].GRAPH;
			TABLERO[CASILLA].GRAPH=0;
		END

		//--> COMPRUEBA SI PULSAMOS ENCIMA DE UNA JEWEL SELECIONANDOLA (GLOW EFFECT)
		IF(MOUSE.LEFT AND collision(TYPE MOUSE) AND EXISTS(ID_EXPLOSION)<>1 and !EXISTS(TYPE GAMEOVER))

			//--> SI ES LA JOYA EXPLOSIVA SE ELIGE UN TIPO DE JOYA AL AZAR Y SE ELIMINAN TODAS SUS REPRESENTACIONES EN EL TABLERO
			IF(TABLERO[CASILLA].GRAPH==100 AND CHAIN==0 AND EXISTS(ID_EXPLOSION)<>1) //--> MIRA SI SE PULSA, QUE NO ESTEMOS EN MITAD DE UNA CHAIN Y QUE NO SE ESTEN DESTRUYENDO JOYAS

				sound(505);
				CONTADOR2=SORTEO[Rand(0,3)].NUMERO; //--> SE ELIGE UN TIPO DE JOYA AL AZAR
				FOR(CONTADOR=1;CONTADOR<57;CONTADOR++); //--> SE ELIMINAN TODAS LAS REPRESENTACIONES DE ESA JOYA DEL TABLERO
					IF(TABLERO[CONTADOR].GRAPH==CONTADOR2); TABLERO[CONTADOR].DESTRUIR=1;END
				END
			TABLERO[CASILLA].DESTRUIR=1;// LA CASILLA EXPLOSIVA SE AUTODESTRUYE TAMBIEN
			FRAME(500);CHECK();WAITCHECK=1;
			if(mode==2) check_glass(); end // para el modo glass testeo
			END

			int joy = joy_query( JOY_QUERY_FIRST_ATTACHED );

			//--> MIRAR SI LA JOYA PULSADA NO ESTA PULSADA YA (GLOW=0), SI ES INICIO DEL COMBO (CHAIN=0) Y QUE NO SE ESTEN DESTRUYENDO JOYAS
			IF(TABLERO[CASILLA].GLOW==0 AND CHAIN==0 AND EXISTS(ID_EXPLOSION)<>1 and ( os_id!=OS_GP2X_WIZ or ( joy != -1 or !joy_query( joy, JOY_BUTTON_LEFTSHOULDER ) ) ) and !mouse.right)
				//HIT(X,Y);
				RESET(0,57,2);
				SOUND(GRAPH);
				candidato=1;
				TABLERO[CASILLA].GLOW++;
				CHAIN++;
				signal(type combo,s_kill);
				combo(chain+1,1,0);
				LISTA[CANDIDATO].ELIMINAR=CASILLA; //--> SE AÑADE EL NUMERO DE LA CASILLA AL STRUCT LISTA COMO CANDIDATO A DESTRUIR EN CASO DE QUE EL CHAIN SEA VALIDO
				CANDIDATO++;


			END
			IF(mouse.right or ( os_id==OS_GP2X_WIZ and ( joy != -1 AND joy_query( joy, JOY_BUTTON_LEFTSHOULDER ) ) ) OR TABLERO[CASILLA].GLOW<>0 AND CHAIN<>TABLERO[CASILLA].GLOW) CHAIN=0;signal(type combo,s_kill); combo(chain+1,1,0);  CANDIDATO=1; RESET(0,57,2);RESET(0,57,1);END // CANCELA AL HACER MARCHA ATRAS
			IF(TABLERO[CASILLA].GLOW==0 AND CHAIN>0) // CONTINUA EL CHAIN

				IF((TABLERO[TABLERO[CASILLA].ARRIBA].GRAPH==GRAPH OR TABLERO[TABLERO[CASILLA].ARRIBA].GRAPH==41 OR TABLERO[CASILLA].GRAPH==41) AND TABLERO[TABLERO[CASILLA].ARRIBA].GLOW==CHAIN)
					//HIT(X,Y,Z);
					SOUND(GRAPH);
					CHAIN++;
					TABLERO[CASILLA].GLOW=CHAIN;
					LISTA[CANDIDATO].ELIMINAR=CASILLA;
					CANDIDATO++;
				END
				IF((TABLERO[TABLERO[CASILLA].DERECHA].GRAPH==GRAPH OR TABLERO[TABLERO[CASILLA].DERECHA].GRAPH==41 OR TABLERO[CASILLA].GRAPH==41) AND TABLERO[TABLERO[CASILLA].DERECHA].GLOW==CHAIN)
					//HIT(X,Y,Z);
					SOUND(GRAPH);
					CHAIN++;
					TABLERO[CASILLA].GLOW=CHAIN;
					LISTA[CANDIDATO].ELIMINAR=CASILLA;
					CANDIDATO++;
				END
				IF((TABLERO[TABLERO[CASILLA].ABAJO].GRAPH==GRAPH OR TABLERO[TABLERO[CASILLA].ABAJO].GRAPH==41 OR TABLERO[CASILLA].GRAPH==41) AND TABLERO[TABLERO[CASILLA].ABAJO].GLOW==CHAIN)
					//HIT(X,Y,Z);
					SOUND(GRAPH);
					CHAIN++;
					TABLERO[CASILLA].GLOW=CHAIN;
					LISTA[CANDIDATO].ELIMINAR=CASILLA;
					CANDIDATO++;
				END
				IF((TABLERO[TABLERO[CASILLA].IZQUIERDA].GRAPH==GRAPH OR TABLERO[TABLERO[CASILLA].IZQUIERDA].GRAPH==41 OR TABLERO[CASILLA].GRAPH==41) AND TABLERO[TABLERO[CASILLA].IZQUIERDA].GLOW==CHAIN)
					//HIT(X,Y,Z);
					SOUND(GRAPH);
					CHAIN++;
					TABLERO[CASILLA].GLOW=CHAIN;
					LISTA[CANDIDATO].ELIMINAR=CASILLA;
					CANDIDATO++;
				END
				signal(type combo,s_kill);
				combo(chain+1,1,0);

			END

		END
		If (not mouse.left)
			IF(CHAIN=>5 and CHAIN<10);PUZSION=1; end // CREAMOS JACKPOT
			IF(CHAIN=>10 and CHAIN<15) PUZSION=2; END // SI EL CHAIN EN DE 10 O MAS SE CREA UNA UNITE JOYA
			IF(CHAIN=>15 and CHAIN<20); PUZSION=3;end  // SI EL CHAIN EN DE 10 O MAS SE CREA UNA PUZSION TILE
			IF(CHAIN=>20) PUZSION=4; END// SI EL CHAIN EN DE 20 O MAS SE PETA EL TABLERO
			IF(CHAIN=>3)
				ID_EXPLOSION=EXPLOSION();DIFERENCIA=(25+(25*CHAIN));
				//--> CONTADOR/RESTADOR DE CHAINS
				SWITCH (CHAIN)
					CASE 3: IF(QC3>0 and mode<>3) QC3--; else if(mode==3) QC3--;end; END; END
					CASE 4: IF(QC4>0 and mode<>3)QC4--; else if(mode==3) QC4--;end;END; END
					CASE 5: IF(QC5>0 and mode<>3)QC5--; else if(mode==3)QC5--;end;END; END
					CASE 6: IF(QC6>0 and mode<>3)QC6--; else if(mode==3)QC6--;end;END; END
					CASE 7: IF(QC7>0 and mode<>3)QC7--; else if(mode==3)QC7--;end;END; END
					CASE 8: IF(QC8>0 and mode<>3)QC8--; else if(mode==3)QC8--;END; END; end
					CASE 9: IF(QC9>0 and mode<>3)QC9--; else if(mode==3)QC9--;END; END; end
					CASE 10: IF(QC10>0 and mode<>3)QC10--; else if(mode==3)QC10--;END; END; end
					CASE 11: IF(QC11>0 and mode<>3)QC11--; else if(mode==3)QC11--;END; END; end
					CASE 12: IF(QC12>0 and mode<>3)QC12--; else if(mode==3)QC12--;END; END; end
					CASE 13: IF(QC13>0 and mode<>3)QC13--; else if(mode==3)QC13--;END; END; end
					CASE 14: IF(QC14>0 and mode<>3)QC14--; else if(mode==3)QC14--;END; END; end
					CASE 15: IF(QC15>0 and mode<>3)QC15--; else if(mode==3)QC15--;END; END; end
					CASE 16: IF(QC16>0 and mode<>3)QC16--; else if(mode==3)QC16--;END; END; end
					CASE 17: IF(QC17>0 and mode<>3)QC17--; else if(mode==3)QC17--;END; END; end
					CASE 18: IF(QC18>0 and mode<>3)QC18--; else if(mode==3)QC18--;END; END; end
					CASE 19: IF(QC19>0 and mode<>3)QC19--; else if(mode==3)Qc19--;END; END; end
					CASE 20: IF(QC20>0 and mode<>3)QC20--;else if(mode==3)QC20--; END; END; end
					CASE 21: IF(QC21>0 and mode<>3)QC21--;else if(mode==3)QC21--; END; END; end
					CASE 22: IF(QC22>0 and mode<>3)QC22--;else if(mode==3)QC22--; END; END; end
					CASE 23: IF(QC23>0 and mode<>3)QC23--;else if(mode==3)QC23--; END; END; end
					CASE 24: IF(QC24>0 and mode<>3)QC24--;else if(mode==3)QC24--; END; END; end
					CASE 25: IF(QC25>0 and mode<>3)QC25--;else if(mode==3)QC25--; END; END; end
					CASE 26: IF(QC26>0 and mode<>3)QC26--;else if(mode==3)QC26--; END; END; end
					CASE 27: IF(QC27>0 and mode<>3)QC27--;else if(mode==3)QC27--; END; END; end
					CASE 28: IF(QC28>0 and mode<>3)QC28--;else if(mode==3)QC28--; END; END; end
					CASE 29: IF(QC29>0 and mode<>3)QC29--;else if(mode==3)QC29--; END; END; end
					CASE 30: IF(QC30>0 and mode<>3)QC30--;else if(mode==3)QC30--; END; END; end
					CASE 31: IF(QC31>0 and mode<>3)QC31--;else if(mode==3)QC31--; END; END; end
					CASE 32: IF(QC32>0 and mode<>3)QC32--;else if(mode==3)QC32--; END; END; end
					CASE 33: IF(QC33>0 and mode<>3)QC33--;else if(mode==3)QC33--; END; END; end
					CASE 34: IF(QC34>0 and mode<>3)QC34--;else if(mode==3)QC34--; END; END; end
					CASE 35: IF(QC35>0 and mode<>3)QC35--;else if(mode==3)QC35--; END; END; end
					CASE 36: IF(QC36>0 and mode<>3)QC36--;else if(mode==3)QC36--; END; END; end
					CASE 37: IF(QC37>0 and mode<>3)QC37--;else if(mode==3)QC37--; END; END; end
					CASE 38: IF(QC38>0 and mode<>3)QC38--;else if(mode==3)QC38--; END; END; end
					CASE 39: IF(QC39>0 and mode<>3)QC39--;else if(mode==3)QC39--; END; END; end
					CASE 40: IF(QC30>0 and mode<>3)QC40--;else if(mode==3)QC40--; END; END; end
					CASE 41: IF(QC41>0 and mode<>3)QC41--;else if(mode==3)QC41--; END; END; end
					CASE 42: IF(QC42>0 and mode<>3)QC42--;else if(mode==3)QC42--; END; END; end
					CASE 43: IF(QC43>0 and mode<>3)QC43--;else if(mode==3)QC43--; END; END; end
					CASE 44: IF(QC44>0 and mode<>3)QC44--;else if(mode==3)QC44--; END; END; end
					CASE 45: IF(QC45>0 and mode<>3)QC45--;else if(mode==3)QC45--; END; END; end
					CASE 46: IF(QC46>0 and mode<>3)QC46--;else if(mode==3)QC46--; END; END; end
					CASE 47: IF(QC47>0 and mode<>3)QC47--;else if(mode==3)QC47--; END; END; end
					CASE 48: IF(QC48>0 and mode<>3)QC48--;else if(mode==3)QC48--; END; END; end
					CASE 49: IF(QC49>0 and mode<>3)QC49--;else if(mode==3)QC49--; END; END; end

				END


				chain=0;  end;
			IF(CHAIN>0 AND CHAIN<3) CANDIDATO=1;CHAIN=0; signal(type combo,s_kill); combo(chain+1,1,0);RESET(0,57,2);RESET(0,57,1);END // SI LA CHAIN ES CORTA (MENOS DE 3) SE RESETEA
		END

		frame;
	END
END

//****************************************************************
//***                  PROCESS PUNTERO                         ***
//****************************************************************

PROCESS PUNTERO();
PRIVATE
int linea,pulsador=0;
int  CONTADOR,change=0,prisa=-2,id_music=0,PULSADA=0, sonando=1;

BEGIN
	if(os_id==OS_GP2X_WIZ) ELSE GRAPH=200;SIZE=100;END

	LOOP
		if(!exists(type intro)) file=joyas; else file=splash; end

		int joy = joy_query( JOY_QUERY_FIRST_ATTACHED );

		IF ((KEY(_ESC) OR ( joy != -1 AND joy_query( joy, JOY_BUTTON_BACK ))) AND !EXISTS(TYPE INTRO))
			RELOJ=0;
		END

		IF ((KEY(_SPACE) OR ( joy != -1 AND joy_query( joy, JOY_BUTTON_RIGHTSHOULDER ))) AND !EXISTS(TYPE INTRO) AND !EXISTS(TYPE GAMEOVER));
			PAUSA();
		END

		IF ((KEY(_C_MINUS) OR ( joy != -1 AND joy_query( joy, JOY_AXIS_TRIGGERLEFT ) )))
			channel_set_volume(-1,SFX_VOL-=1);
			music_set_volume(MUS_VOL-=1);
			IF(SFX_VOL<0) SFX_VOL=0; END
			IF(MUS_VOL<0) MUS_VOL=0; END
		END

		IF ((KEY(_C_PLUS) OR ( joy != -1 AND joy_query( joy, JOY_AXIS_TRIGGERRIGHT ) )))
			channel_set_volume(-1,SFX_VOL+=1);
			music_set_volume(MUS_VOL+=1);
			IF(SFX_VOL>saved[1].option) SFX_VOL=saved[1].option; END
			IF(MUS_VOL>saved[2].option) MUS_VOL=saved[2].option; END
		END

		IF (KEY(_M) OR ( joy != -1 AND joy_query( joy, JOY_BUTTON_LEFTSHOULDER )))
			if(sonando==1) music_pause();frame(2000); sonando=0; elseif (sonando==0) music_resume();frame(2000);sonando=1; end
		end

		X=MOUSE.X;
		Y=MOUSE.Y;
		Z=-300;

		FRAME;
	END
END

//****************************************************************
//***               PROCESS LEVEL_QUOTA(NIVEL                  ***
//****************************************************************

process LEVEL_QUOTA(NIVEL);//--> QUOTA MODE
private int contador3,PROXYSCORE,PROXYTIME, ID_SCORE,ID_TIME;
BEGIN

	write_delete(all_text);
    clear_screen();
	put_screen(fondo,nivel);

	timer[0]=0;
	reloj+=25*NIVEL;
	if(firstime>0) FLYT(38,128,Z,25*NIVEL); end;
	firstime=1;
	refill=2;
	NUM_NIVEL=NIVEL+((NIVEL_GLOBAL-1)*5);
	ID_SCORE=write_var(numscore,38,152,4,SCORE);
	write_var(numlevel,58,40,4,NUM_NIVEL);

	fpg_unload(splash);
	joyas=fpg_load("graphics/magic.fpg");

	IF(NIVEL==6) NIVEL=1; NIVEL_GLOBAL++; END



	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;

	SWITCH (NIVEL_GLOBAL)
		CASE 1:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,4,49,168,0,30,0,0);
	XPUT(NUMEROSQUOTA,5,49,188,0,30,0,0);
	XPUT(NUMEROSQUOTA,6,49,208,0,30,0,0);
	XPUT(NUMEROSQUOTA,7,49,228,0,30,0,0);

	// ASIGNA VALORES CHAIN
	QC3=NIVEL*4;
	QC4=NIVEL*3;
	QC5=NIVEL*2;
	QC6=NIVEL;

	// COLOCA NUMEROS CHAIN
	write_var(numquota,66,168,4,QC3);
	write_var(numquota,66,188,4,QC4);
	write_var(numquota,66,208,4,QC5);
	write_var(numquota,66,228,4,QC6);


	END
		CASE 2:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,4,49,168,0,30,0,0);
	XPUT(NUMEROSQUOTA,5,49,188,0,30,0,0);
	XPUT(NUMEROSQUOTA,9,49,208,0,30,0,0);

	XPUT(NUMEROSQUOTA,2,45,228,0,25,0,0);
	XPUT(NUMEROSQUOTA,1,52,228,0,25,0,0);

	// ASIGNA VALORES CHAIN
	QC3=NIVEL*4;
	QC4=NIVEL*3;
	QC8=NIVEL*2;
	QC10=NIVEL;

	// COLOCA NUMEROS CHAIN
	write_var(numquota,66,168,4,QC3);
	write_var(numquota,66,188,4,QC4);
	write_var(numquota,66,208,4,QC8);
	write_var(numquota,66,228,4,QC10);


	END
	CASE 3:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,4,49,168,0,30,0,0);
	XPUT(NUMEROSQUOTA,5,49,188,0,30,0,0);

	XPUT(NUMEROSQUOTA,2,45,208,0,25,0,0);
	XPUT(NUMEROSQUOTA,1,52,208,0,25,0,0);

	XPUT(NUMEROSQUOTA,2,45,228,0,25,0,0);
	XPUT(NUMEROSQUOTA,6,52,228,0,25,0,0);

	// ASIGNA VALORES CHAIN
	QC4=NIVEL*4;
	QC5=NIVEL*3;
	QC10=NIVEL*2;
	QC15=NIVEL;
	// COLOCA NUMEROS CHAIN
	write_var(numquota,65,170,4,QC4);
	write_var(numquota,65,190,4,QC5);
	write_var(numquota,65,210,4,QC10);
	write_var(numquota,65,230,4,QC15);

		END

	CASE 4:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,5,49,168,0,30,0,0);
	XPUT(NUMEROSQUOTA,6,49,188,0,30,0,0);

	XPUT(NUMEROSQUOTA,2,45,208,0,25,0,0);
	XPUT(NUMEROSQUOTA,4,52,208,0,25,0,0);

	XPUT(NUMEROSQUOTA,2,45,228,0,25,0,0);
	XPUT(NUMEROSQUOTA,9,52,228,0,25,0,0);

	// ASIGNA VALORES CHAIN
	QC4=NIVEL*4;
	QC5=NIVEL*3;
	QC13=NIVEL*2;
	QC18=NIVEL;

	// COLOCA NUMEROS CHAIN
	write_var(numquota,65,170,4,QC4);
	write_var(numquota,65,190,4,QC5);
	write_var(numquota,65,210,4,QC13);
	write_var(numquota,65,230,4,QC18);

	END

	CASE 5:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2*NIVEL_GLOBAL)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,6,49,168,0,30,0,0);
	XPUT(NUMEROSQUOTA,7,49,188,0,30,0,0);

	XPUT(NUMEROSQUOTA,2,45,208,0,25,0,0);
	XPUT(NUMEROSQUOTA,6,52,208,0,25,0,0);

	XPUT(NUMEROSQUOTA,2,45,228,0,25,0,0);
	XPUT(NUMEROSQUOTA,10,52,228,0,25,0,0);

	// ASIGNA VALORES CHAIN
	QC5=NIVEL*4;
	QC6=NIVEL*3;
	QC15=NIVEL*2;
	QC19=NIVEL;

	// COLOCA NUMEROS CHAIN
	write_var(numquota,65,170,4,QC5);
	write_var(numquota,65,190,4,QC6);
	write_var(numquota,65,210,4,QC15);
	write_var(numquota,65,230,4,QC19);


		END

CASE 6..99:
	// COLOCA GRAFICOS JOYAS
	XPUT(JOYAS,2, 10,168,0,57,0,0);
	XPUT(JOYAS,11,10,188,0,57,0,0);
	XPUT(JOYAS,21,10,208,0,57,0,0);
	XPUT(JOYAS,31,10,228,0,57,0,0);

	// ASIGNA VALORES JOYAS
	QJ1=(NIVEL*2)*(1+NIVEL+NIVEL_GLOBAL);
	QJ2=(NIVEL*2)*(1+NIVEL+NIVEL_GLOBAL);
	QJ3=(NIVEL*2)*(1+NIVEL+NIVEL_GLOBAL);
	QJ4=(NIVEL*2)*(1+NIVEL+NIVEL_GLOBAL);

	// COLOCA NUMEROS JOYAS
	write_var(numquota,30,168,4,QJ1);
	write_var(numquota,30,188,4,QJ2);
	write_var(numquota,30,208,4,QJ3);
	write_var(numquota,30,228,4,QJ4);

	// COLOCA GRAFICOS CHAIN
	XPUT(NUMEROSQUOTA,6,49,168,0,30,0,0);

	XPUT(NUMEROSQUOTA,2,45,188,0,25,0,0);
	XPUT(NUMEROSQUOTA,1,52,188,0,25,0,0);

	XPUT(NUMEROSQUOTA,2,45,208,0,25,0,0);
	XPUT(NUMEROSQUOTA,6,52,208,0,25,0,0);

	XPUT(NUMEROSQUOTA,2,45,228,0,25,0,0);
	XPUT(NUMEROSQUOTA,10,52,228,0,25,0,0);

	// ASIGNA VALORES CHAIN
	QC5=NIVEL*4;
	QC10=NIVEL*3;
	QC15=NIVEL*2;
	QC19=NIVEL;

	// COLOCA NUMEROS CHAIN
	write_var(numquota,65,170,4,QC5);
	write_var(numquota,65,190,4,QC10);
	write_var(numquota,65,210,4,QC15);
	write_var(numquota,65,230,4,QC19);


		END
	END

	FRELOJ=fTIME("%M %S",RELOJ+GTM);
	ID_TIME=write_string(numtime,38,124,4,&Freloj);

	file=marcador;
	z=250;
	graph=1;
	x=160;y=120;
	loop

	IF(PROXYSCORE<>SCORE) write_delete(ID_SCORE); ID_SCORE=write_var(NUMBIGSCORE,38,152,4,SCORE);FRAME(200); PROXYSCORE=SCORE;write_delete(ID_SCORE); ID_SCORE=write_var(numscore,38,152,4,SCORE);END
	IF(PROXYTIME<RELOJ) write_delete(ID_TIME); ID_TIME=write_string(numlevel,38,124,4,&Freloj);FRAME(300); PROXYTIME=RELOJ; write_delete(ID_TIME); ID_TIME=write_string(numtime,38,124,4,&Freloj);END

	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;
	FRELOJ=fTIME("%M:%S",RELOJ+GTM);
	if(timer[0]=>100 and not exists(type gameover))reloj--;timer[0]=0;tiempo_juego++;end
	if(reloj==20 and timer[0]==1) SOUND(500); END //--> Hurry up
	if(QJ1<=0) QJ1=0;end; if(QJ2<=0) QJ2=0;end; if(QJ3<=0) QJ3=0;end; if(QJ4<=0) QJ4=0;end

	SWITCH (NIVEL_GLOBAL)
	CASE 1:
			if(QC3<=0) QC3=0;end; if(QC4<=0) QC4=0;end; if(QC5<=0) QC5=0;end; if(QC6<=0) QC6=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC3=<0 AND QC4=<0 AND QC5=<0 AND QC6=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0;
			SIGNAL(TYPE PUNTERO, S_SLEEP);frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound_stop(all_sound);
			sound(101);
			FRAME(12000);diferencia=RELOJ*10; IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	CASE 2:
			if(QC3<=0) QC3=0;end; if(QC4<=0) QC4=0;end; if(QC8<=0) QC8=0;end; if(QC10<=0) QC10=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC3=<0 AND QC4=<0 AND QC8=<0 AND QC10=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0; SIGNAL(TYPE PUNTERO, S_SLEEP);
			frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound(101);
			FRAME(12000);diferencia=RELOJ*10;IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	CASE 3:
			if(QC4<=0) QC4=0;end; if(QC5<=0) QC5=0;end; if(QC10<=0) QC10=0;end; if(QC15<=0) QC15=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC4=<0 AND QC5=<0 AND QC10=<0 AND QC15=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0;
			SIGNAL(TYPE PUNTERO, S_SLEEP);
			frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound(101);
			FRAME(12000);diferencia=RELOJ*10;IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	CASE 4:
			if(QC4<=0) QC4=0;end; if(QC5<=0) QC5=0;end; if(QC13<=0) QC13=0;end; if(QC18<=0) QC18=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC4=<0 AND QC5=<0 AND QC13=<0 AND QC18=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0;
			SIGNAL(TYPE PUNTERO, S_SLEEP);
			frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound(101);
			FRAME(12000);diferencia=RELOJ*10;IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	CASE 5:
			if(QC5<=0) QC5=0;end; if(QC6<=0) QC6=0;end; if(QC15<=0) QC15=0;end; if(QC19<=0) QC19=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC5=<0 AND QC6=<0 AND QC15=<0 AND QC19=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0;
			SIGNAL(TYPE PUNTERO, S_SLEEP);
			frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound(101);
			FRAME(12000);diferencia=RELOJ*10;IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	CASE 6..99:
			if(QC5<=0) QC5=0;end; if(QC10<=0) QC10=0;end; if(QC15<=0) QC15=0;end; if(QC19<=0) QC19=0;end
			IF(QJ1=<0 AND QJ2=<0 AND QJ3=<0 AND QJ4=<0 AND QC5=<0 AND QC6=<0 AND QC15=<0 AND QC19=<0  AND !exists(TYPE EXPLOSION) OR (puzsion==4 AND !exists(TYPE EXPLOSION)))
			IF(PUZSION==4) SOUND(200); END; puzsion=0;
			SIGNAL(TYPE PUNTERO, S_SLEEP);
			frame(5000);
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
			TABLERO[CONTADOR3].GLOW=1;
			END
			sound(101);
			FRAME(12000);diferencia=RELOJ*10;IF(DIFERENCIA>999) DIFERENCIA=999; END
			FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
			TABLERO[CONTADOR3].DESTRUIR=1;
			END
			RESET(0,57,1);
			FRAME(12000);
			SIGNAL(TYPE PUNTERO, S_WAKEUP);
			LEVEL_QUOTA(NIVEL+=1); FLY(38,152,Z,((10000*NIVEL)*NIVEL_GLOBAL)+RELOJ*1000,NUMBIGSCORE);
			BREAK;
			END
	END
	END // SWITCH


	if(SCORE<=0) SCORE=0; eND
	IF(RELOJ=<0 and not exists(type gameover)) GAMEOVER(1); END
	frame;
	end
END


//****************************************************************
//***                LEVEL_GLASS(GLASS_NIVEL)                  ***
//****************************************************************


process LEVEL_GLASS(GLASS_NIVEL);//--> GLASS MODE
PRIVATE
int CONTADOR,PROXYSCORE,PROXYTIME, ID_SCORE,ID_TIME;
BEGIN
	fpg_unload(splash);
	joyas=fpg_load("graphics/magic.fpg");
    write_delete(all_text);
	clear_screen();
    put_screen(fondo,GLASS_NIVEL);
	QUOTA_DESTROYED=GLASS_NIVEL;
	timer[0]=0;
	refill=1;
	NUM_NIVEL=GLASS_NIVEL+((NIVEL_GLOBAL-1)*10);
	ID_SCORE=write_var(numscore,38,152,4,SCORE);
	//write_var(numtime,10,10,4,fps);
	write_var(numlevel,57,39,4,NUM_NIVEL);
	//ID_TIME=write_var(numtime,38,124,4,reloj);
	FRELOJ=fTIME("%M %S",RELOJ+GTM);
	ID_TIME=write_string(numtime,38,124,4,&Freloj);

	IF(GLASS_NIVEL==11) GLASS_NIVEL=1; NIVEL_GLOBAL++; END

	SIGNAL(TYPE JEWEL,S_KILL);
	FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
			JEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,SORTEO[Rand(0,3)].NUMERO);
	END
	XPUT(JOYAS,1, 34,168,0,65,0,0);
	XPUT(JOYAS,11,34,188,0,65,0,0);
	XPUT(JOYAS,21,34,208,0,65,0,0);
	XPUT(JOYAS,31,34,228,0,65,0,0);

	write_var(numquota,55,168,4,QJ1);
	write_var(numquota,55,188,4,QJ2);
	write_var(numquota,55,208,4,QJ3);
	write_var(numquota,55,228,4,QJ4);


	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;
	file=marcador;
	z=250;
	graph=2;
	x=160;y=120;
	loop;

	IF(PROXYSCORE<>SCORE) write_delete(ID_SCORE); ID_SCORE=write_var(NUMBIGSCORE,38,152,4,SCORE);FRAME(200); PROXYSCORE=SCORE;write_delete(ID_SCORE); ID_SCORE=write_var(numscore,38,152,4,SCORE);END
	IF(PROXYTIME<RELOJ) write_delete(ID_TIME); ID_TIME=write_string(numlevel,38,124,4,&Freloj);FRAME(300); PROXYTIME=RELOJ; write_delete(ID_TIME); ID_TIME=write_string(numtime,38,124,4,&Freloj);END

	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;
	FRELOJ=fTIME("%M:%S",RELOJ+GTM);
	if(timer[0]=>100 and not exists(type gameover))reloj--;timer[0]=0;tiempo_juego++;end
	if(reloj==20 and timer[0]=<2) SOUND(500); END //--> Hurry up
	if(QJ1==-100) QJ1=0; fly(38,152,z,10000,NUMbigscore); RELOJ+=15; FLYT(38,128,Z,15);END
	if(QJ2==-100) QJ2=0; fly(38,152,z,10000,NUMbigscore); RELOJ+=15; FLYT(38,128,Z,15);END
	if(QJ3==-100) QJ3=0; fly(38,152,z,10000,NUMbigscore); RELOJ+=15; FLYT(38,128,Z,15);END
	if(QJ4==-100) QJ4=0; fly(38,152,z,10000,NUMbigscore); RELOJ+=15; FLYT(38,128,Z,15);END
	if(SCORE<0) SCORE=0; eND
	IF(RELOJ=<0 and not exists(type gameover)) GAMEOVER(2); END

	frame;

	end
END

//****************************************************************
//***                      LEVEL_RUSH()                        ***
//****************************************************************


process LEVEL_RUSH();//--> RUSH MODE
PRIVATE
int reloj2=0,hurrysound;
int ID_COMBO;
int RQC3,RQC4,RQC5,RQC6,RQC7,RQC8,RQC9,RQC10,RQC11,RQC12,RQC13,RQC14,RQC15,RQC16,RQC17,RQC18,RQC19,RQC20,RQC21,RQC22;
int RQC23,RQC24,RQC25,RQC26,RQC27,RQC28,RQC29,RQC30,RQC31,RQC32,RQC33,RQC34,RQC35,RQC36,RQC37,RQC38,RQC39,RQC40,RQC41,RQC42;
int RQC43,RQC44,RQC45,RQC46,RQC47,RQC48,RQC49;
int PROXYSCORE,PROXYTIME, ID_SCORE,ID_TIME;
BEGIN
	fpg_unload(splash);
	joyas=fpg_load("graphics/magic.fpg");
	RUSH_LEVEL=4;
    write_delete(all_text);
	clear_screen();
    put_screen(fondo,1);
	timer[0]=0;
	reloj=30;
	refill=2;
	ID_SCORE=write_var(numscore,38,162,4,SCORE);


	FRELOJ=fTIME("%M %S",RELOJ+GTM);
	ID_TIME=write_string(numtime,38,128,4,&Freloj);



	RQC3=QC3;
	COMBORUSH(RUSH_LEVEL,1);
	file=marcador;
	z=250;
	graph=3;
	x=160;y=120;
	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;
	loop;
	IF(PROXYSCORE<>SCORE) write_delete(ID_SCORE); ID_SCORE=write_var(NUMBIGSCORE,38,162,4,SCORE);FRAME(200); PROXYSCORE=SCORE;write_delete(ID_SCORE); ID_SCORE=write_var(numscore,38,162,4,SCORE);END
	IF(PROXYTIME<RELOJ) write_delete(ID_TIME); ID_TIME=write_string(numlevel,38,128,4,&Freloj);FRAME(300); PROXYTIME=RELOJ; write_delete(ID_TIME); ID_TIME=write_string(numtime,38,128,4,&Freloj);END
	PROXYSCORE=SCORE;
	PROXYTIME=RELOJ;
	FRELOJ=fTIME("%M:%S",RELOJ+GTM);
	if(timer[0]=>100 and not exists(type gameover))reloj--;timer[0]=0; tiempo_juego++; end
	if(reloj==20 and timer[0]=<2 and exists(hurrysound)==0); hurrysound=SOUND(500); END //--> Hurry up
	IF(NOT EXISTS(TYPE COMBORUSH)) COMBORUSH(RUSH_LEVEL,1);END
	IF(RUSH_LEVEL==4) iF(RQC3<>QC3 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);RQC4=QC4;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==5) iF(RQC4<>QC4 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);   RQC5=QC5;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==6) iF(RQC5<>QC5 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);   RQC6=QC6;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==7) iF(RQC6<>QC6 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);   RQC7=QC7;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==8) iF(RQC7<>QC7 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);   RQC8=QC8;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==9) iF(RQC8<>QC8 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);  RQC9=QC9;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==10) iF(RQC9<>QC9 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);   RQC10=QC10;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==11) iF(RQC10<>QC10 and !exists(TYPE EXPLOSION)) SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC11=QC11;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==12) iF(RQC11<>QC11 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC12=QC12;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==13) iF(RQC12<>QC12 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC13=QC13;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==14) iF(RQC13<>QC13 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC14=QC14;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==15) iF(RQC14<>QC14 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC15=QC15;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==16) iF(RQC15<>QC15 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC16=QC16;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==17) iF(RQC16<>QC16 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC17=QC17;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==18) iF(RQC17<>QC17 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC18=QC18;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==19) iF(RQC18<>QC18 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC19=QC19;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==20) iF(RQC19<>QC19 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC20=QC20;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==21) iF(RQC20<>QC20 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC21=QC21;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==22) iF(RQC21<>QC21 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC22=QC22;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==23) iF(RQC22<>QC22 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC23=QC23;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END

	IF(RUSH_LEVEL==24) iF(RQC23<>QC23 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC24=QC24;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==25) iF(RQC24<>QC24 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC25=QC25;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==26) iF(RQC25<>QC25 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC26=QC26;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==27) iF(RQC26<>QC26 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC27=QC27;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==28) iF(RQC27<>QC27 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC28=QC28;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==29) iF(RQC28<>QC28 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC29=QC29;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==30) iF(RQC29<>QC29 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC30=QC30;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==31) iF(RQC30<>QC30 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC31=QC31;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==32) iF(RQC31<>QC31 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC32=QC32;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==33) iF(RQC32<>QC32 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC33=QC33;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==34) iF(RQC33<>QC33 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC34=QC34;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==35) iF(RQC34<>QC34 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC35=QC35;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==36) iF(RQC35<>QC35 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC36=QC36;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==37) iF(RQC36<>QC36 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC37=QC37;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==38) iF(RQC37<>QC37 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC38=QC38;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==39) iF(RQC38<>QC38 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC39=QC39;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==40) iF(RQC39<>QC39 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC40=QC40;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==41) iF(RQC40<>QC40 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC41=QC41;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==42) iF(RQC41<>QC41 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC42=QC42;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==43) iF(RQC42<>QC42 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC43=QC43;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==44) iF(RQC43<>QC43 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC44=QC44;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==45) iF(RQC44<>QC44 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC45=QC45;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==46) iF(RQC45<>QC45 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC46=QC46;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==47) iF(RQC46<>QC46 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC47=QC47;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==48) iF(RQC47<>QC47 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC48=QC48;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END
	IF(RUSH_LEVEL==49) iF(RQC48<>QC48 and !exists(TYPE EXPLOSION))  SIGNAL(TYPE COMBORUSH,S_KILL);  combo(1,1,0); RQC49=QC49;RUSH_LEVEL++;COMBORUSH(RUSH_LEVEL,1);RELOJ+=25;FLYT(38,128,Z,25);FLY(38,164,Z,10000*(RUSH_LEVEL-2)+RELOJ*1000,NUMBIGSCORE); END;END

	IF(RELOJ=<0 and not exists(type gameover))

		GAMEOVER(3);
	END

	frame;
	end
END

//****************************************************************
//***                      PROCESS HIT(X,Y,Z)                  ***
//****************************************************************


PROCESS HIT(double X,Y, int Z);
private
int contador=0;
BEGIN
	file=GFX_HIT;
	graph=0;
	Z-=5;
	angle=rand(0,270000);
	for(contador=0;contador<9;contador++);
		GRAPH++;
		frame(800);
	end


	END

//****************************************************************
//***                       PROCESS PAUSA()                    ***
//****************************************************************

process PAUSA();
private
int CONTADOR=0;
Struct PAU[3] INT NUMERO; END=5,15,25,35;//,45;
Struct P[13] INT NUMP;END=16,17,18,19,20,23,27,30,31,32,33,34,37,44;
Struct A[15] INT NUMP;END=16,17,18,19,20,23,27,30,31,32,33,34,37,41,44,48;
Struct U[12] INT NUMP;END=16,20,23,27,30,34,37,41,44,45,46,47,48;
Struct S[16] INT NUMP;END=16,17,18,19,20,23,30,31,32,33,34,41,44,45,46,47,48;
Struct E[16] INT NUMP;END=16,17,18,19,20,23,30,31,32,33,34,37,44,45,46,47,48;

int AZAR;
int AZAR2;
int VUELTAS=0;
int LETRA=1;
BEGIN
	music_pause();
	sound(503);
	file=SPLASH;
	Z=-255;

	SIGNAL(ALL_PROCESS,S_FREEZE);
	FRAME(3000);
	SIGNAL(TYPE JEWEL,S_SLEEP);
	SIGNAL(TYPE PUNTERO,S_SLEEP);

	int joy = joy_query( JOY_QUERY_FIRST_ATTACHED );

	REPEAT
		AZAR=PAU[RAND(0,3)].NUMERO;
		REPEAT
		AZAR2=PAU[RAND(0,3)].NUMERO;

		UNTIL AZAR<>AZAR2;

		IF(NOT EXISTS(TYPE FAKEJEWEL) AND LETRA==1)
			FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				IF(CONTADOR==P[VUELTAS].NUMP);
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR2,1);
				VUELTAS++;
				ELSE
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR,0);
				END
			END
			VUELTAS=0;LETRA++;
		END

		IF(NOT EXISTS(TYPE FAKEJEWEL) AND LETRA==2)
			FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				IF(CONTADOR==A[VUELTAS].NUMP);
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR2,1);
				VUELTAS++;
				ELSE
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR,0);
				END
			END
			VUELTAS=0;LETRA++;
		END
		IF(NOT EXISTS(TYPE FAKEJEWEL) AND LETRA==3)
			FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				IF(CONTADOR==U[VUELTAS].NUMP);
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR2,1);
				VUELTAS++;
				ELSE
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR,0);
				END
			END
			VUELTAS=0;LETRA++;
		END
		IF(NOT EXISTS(TYPE FAKEJEWEL) AND LETRA==4)
			FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				IF(CONTADOR==S[VUELTAS].NUMP);
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR2,1);
				VUELTAS++;
				ELSE
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR,0);
				END
			END
			VUELTAS=0;LETRA++;
		END
		IF(NOT EXISTS(TYPE FAKEJEWEL) AND LETRA==5)
			FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				IF(CONTADOR==E[VUELTAS].NUMP);
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR2,1);
				VUELTAS++;
				ELSE
				FAKEJEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,AZAR,0);
				END
			END
			VUELTAS=0;LETRA=1;
		END

		FRAME;
	
		joy = joy_query( JOY_QUERY_FIRST_ATTACHED );

	UNTIL (KEY(_space) OR ( joy != -1 AND joy_query( joy, JOY_BUTTON_RIGHTSHOULDER ) ) )
	sound(210);
	FRAME(3000);
	SIGNAL(TYPE FAKEJEWEL,S_KILL);FRAME;
	SIGNAL(ALL_PROCESS,S_WAKEUP);
	music_resume();



END

//****************************************************************
//***                      PROCESS SOUND(SFX)                  ***
//****************************************************************

process sound(SFX);
private
int contador=0;
BEGIN
	If(SFX==0) sound_play(SFX_G1,0);end


	If(SFX==150) contador=rand(1,15); // diferente sonidos roturas
		if(contador==1) sound_play(sfx_fantastic,0);end
		if(contador==2) sound_play(sfx_GREAT,0);end
		if(contador==3) sound_play(sfx_COMBO,0);end
		if(contador==4) sound_play(sfx_AWESOME,0);end
		if(contador==5) sound_play(SFX_SWEET,0);end
		if(contador==6) sound_play(sfx_perfect,0);end
		if(contador==7) sound_play(sfx_marve,0);end
		if(contador==8) sound_play(sfx_outs,0);end
		if(contador==9) sound_play(sfx_increible,0);end
		if(contador==10) sound_play(SFX_superb,0);end
		if(contador==11) sound_play(SFX_excellent,0);end
		if(contador==12) sound_play(SFX_priceless,0);end
		if(contador==13) sound_play(SFX_brillant,0);end
		if(contador==14) sound_play(SFX_smooth,0);end
		if(contador==15) sound_play(SFX_killer,0);end

	END
	If(SFX==250) contador=rand(1,6); // diferente sonidos roturas
		if(contador==1) sound_play(sfx_extra,0);end
		if(contador==2) sound_play(SFX_super,0);end
		if(contador==3) sound_play(SFX_hyper,0);end
		if(contador==4) sound_play(SFX_ultra,0);end
		if(contador==5) sound_play(SFX_giga,0);end
		if(contador==6) sound_play(SFX_mega,0);end
		FRAME(4200);
		SOUND(150);
	END

	If(SFX==350) contador=rand(1,6); // diferente sonidos roturas
		if(contador==1) sound_play(sfx_extra,0);end
		if(contador==2) sound_play(SFX_super,0);end
		if(contador==3) sound_play(SFX_hyper,0);end
		if(contador==4) sound_play(SFX_ultra,0);end
		if(contador==5) sound_play(SFX_giga,0);end
		if(contador==6) sound_play(SFX_mega,0);end
		FRAME(4200);
		SOUND(250);
	END



	If(SFX==1) sound_play(SFX_J1,0);	END
	If(SFX==11) sound_play(SFX_J2,0);	END
	If(SFX==21) sound_play(SFX_J3,0);	END
	If(SFX==31) sound_play(SFX_J4,0);	END
	If(SFX==41) sound_play(SFX_J5,0);	END

	If(SFX==200) sound_play(SFX_puzsion,0);	END
	If(SFX==201) sound_play(SFX_qmode,0);	END
	If(SFX==202) sound_play(SFX_rmode,0);	END
	If(SFX==203) sound_play(SFX_gmode,0);	END
	If(SFX==204) sound_play(SFX_options,0);	END
	If(SFX==205) sound_play(sfx_hiscore,0);	END
	If(SFX==206) sound_play(sfx_exitg,0);	END
	If(SFX==207) sound_play(sfx_howto,0);	END
	If(SFX==208) sound_play(sfx_exit,0);	END
	If(SFX==101) sound_play(sfx_levelup,0);	END
	If(SFX==210) sound_play(sfx_start,0);	END

	If(SFX==500) sound_play(sfx_hurryup,0);	END
	If(SFX==501) sound_play(sfx_gameover,0);	END
	If(SFX==502) sound_play(sfx_entername,0);	END
	If(SFX==502) sound_play(sfx_entername,0);	END
	If(SFX==503) sound_play(sfx_pause,0);	END
	If(SFX==504) sound_play(sfx_nomoves,0);	END
	If(SFX==505) sound_play(sfx_destruction,0);	END



END


//****************************************************************
//***               PROCESS GAMEOVER(MODOJUEGO)                ***
//****************************************************************

process GAMEOVER(modojuego);
private
	int CONTADOR=0;
BEGIN
	while(exists(TYPE EXPLOSION))
		frame;
	end

	RESET(1,57,1);
	RESET(1,57,3);
	NOMBRE=0;
	music_stop();
	music_unload(music);
	music=music_load("music/musicgameover.mp3");
	music_play(music,-1);
	signal(type puntero,s_sleep);
	file=joyas;
	x=160;y=120;z=-255;

	frame(7000);
	sound_stop(all_sound);
	signal(type sound, s_kill);
	SOUND(501);
	graph=50;
	frame(15000);
	repeat
		RELOJ=0;
		frame;
	until mouse.left;
	SIGNAL(TYPE JEWEL, S_KILL);
	GRAPH=0;
	frame(5000);

	signal(type puntero,s_wakeup);
	fecha = ftime("%d/%m/%Y",time()); //--> se calcula la fecha

	switch(modojuego)

		case 1:		
			IF(SCORE>qhighscore[0].hscore)
				qhighscore[0].hscore=score;
				qhighscore[0].hfase=num_nivel;
				qhighscore[0].last=1;
				qhighscore[0].dia=fecha;
				qhighscore[0].tiempo=ftime("%H:%M:%S",tiempo_juego);

				SOUND(502);
				//---> ENTER YOUR NAME
				ENTERNAME(5,TABLERO[CONTADOR+8].X,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(14,TABLERO[CONTADOR+8].X+15,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(20,TABLERO[CONTADOR+8].X+30,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+45,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+60,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(25,TABLERO[CONTADOR+8].X+90,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(15,TABLERO[CONTADOR+8].X+105,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(21,TABLERO[CONTADOR+8].X+120,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+135,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(14,TABLERO[CONTADOR+8].X+165,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(1,TABLERO[CONTADOR+8].X+180,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(13,TABLERO[CONTADOR+8].X+195,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+210,TABLERO[CONTADOR+8].Y,4);
				//--> JOYAS BARRA SEPARADORA
				ENTERNAME(29,TABLERO[22].X,TABLERO[22].Y,5);
				ENTERNAME(31,TABLERO[23].X,TABLERO[23].Y,5);
				ENTERNAME(33,TABLERO[24].X,TABLERO[24].Y,5);
				ENTERNAME(35,TABLERO[25].X,TABLERO[25].Y,5);
				ENTERNAME(33,TABLERO[26].X,TABLERO[26].Y,5);
				ENTERNAME(31,TABLERO[27].X,TABLERO[27].Y,5);
				ENTERNAME(29,TABLERO[28].X,TABLERO[28].Y,5);

				FOR(CONTADOR=0; CONTADOR<28; CONTADOR++);
					ENTERNAME(CONTADOR,TABLERO[CONTADOR+28].X,TABLERO[CONTADOR+28].Y,1);
				END

				FILE=ENTERNAME_ID;
				GRAPH=CONTADOR;
				X=TABLERO[CONTADOR+28].X;
				Y=TABLERO[CONTADOR+28].Y;
				repeat
					frame;
				until MOUSE.LEFT AND collision(TYPE MOUSE);
				GRAPH+=50;
				sound(150);
				FRAME(1500);
				GRAPH-=50;
				frame;
				FOR(;NOMBRE<7;NOMBRE++);
					qhighscore[0].hname[NOMBRE]=" ";
				END
				SORT(qhighscore,10);
				save(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file",qhighscore);
			END //--> ENTER NAME
		end //--> case 1

		case 2:
			IF(SCORE>ghighscore[0].hscore)
				ghighscore[0].hscore=score;
				ghighscore[0].hfase=num_nivel;
				ghighscore[0].last=1;
				ghighscore[0].dia=fecha;
				ghighscore[0].tiempo=ftime("%H:%M:%S",tiempo_juego);

				SOUND(502);
				//---> ENTER YOUR NAME
				ENTERNAME(5,TABLERO[CONTADOR+8].X,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(14,TABLERO[CONTADOR+8].X+15,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(20,TABLERO[CONTADOR+8].X+30,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+45,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+60,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(25,TABLERO[CONTADOR+8].X+90,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(15,TABLERO[CONTADOR+8].X+105,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(21,TABLERO[CONTADOR+8].X+120,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+135,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(14,TABLERO[CONTADOR+8].X+165,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(1,TABLERO[CONTADOR+8].X+180,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(13,TABLERO[CONTADOR+8].X+195,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+210,TABLERO[CONTADOR+8].Y,4);
				//--> JOYAS BARRA SEPARADORA
				ENTERNAME(29,TABLERO[22].X,TABLERO[22].Y,5);
				ENTERNAME(31,TABLERO[23].X,TABLERO[23].Y,5);
				ENTERNAME(33,TABLERO[24].X,TABLERO[24].Y,5);
				ENTERNAME(35,TABLERO[25].X,TABLERO[25].Y,5);
				ENTERNAME(33,TABLERO[26].X,TABLERO[26].Y,5);
				ENTERNAME(31,TABLERO[27].X,TABLERO[27].Y,5);
				ENTERNAME(29,TABLERO[28].X,TABLERO[28].Y,5);

				FOR(CONTADOR=0; CONTADOR<28; CONTADOR++);
					ENTERNAME(CONTADOR,TABLERO[CONTADOR+28].X,TABLERO[CONTADOR+28].Y,2);
				END

				FILE=ENTERNAME_ID;
				GRAPH=CONTADOR;
				X=TABLERO[CONTADOR+28].X;
				Y=TABLERO[CONTADOR+28].Y;
				repeat
					frame;
				until MOUSE.LEFT AND collision(TYPE MOUSE);
				GRAPH+=50;
				sound(150);
				FRAME(1500);
				GRAPH-=50;
				frame;
				FOR(;NOMBRE<7;NOMBRE++);
					ghighscore[0].hname[NOMBRE]=" ";
				END
				SORT(ghighscore,10);
				save(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file",ghighscore);
			END //--> ENTER NAME
		end //--> case 2

		case 3:
			IF(SCORE>rhighscore[0].hscore)
				rhighscore[0].hscore=score;
				rhighscore[0].hfase=rush_level-2;
				rhighscore[0].last=1;
				rhighscore[0].dia=fecha;
				rhighscore[0].tiempo=ftime("%H:%M:%S",tiempo_juego);

				SOUND(502);
				//---> ENTER YOUR NAME
				ENTERNAME(5,TABLERO[CONTADOR+8].X,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(14,TABLERO[CONTADOR+8].X+15,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(20,TABLERO[CONTADOR+8].X+30,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+45,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+60,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(25,TABLERO[CONTADOR+8].X+90,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(15,TABLERO[CONTADOR+8].X+105,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(21,TABLERO[CONTADOR+8].X+120,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(18,TABLERO[CONTADOR+8].X+135,TABLERO[CONTADOR+8].Y,4);

				ENTERNAME(14,TABLERO[CONTADOR+8].X+165,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(1,TABLERO[CONTADOR+8].X+180,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(13,TABLERO[CONTADOR+8].X+195,TABLERO[CONTADOR+8].Y,4);
				ENTERNAME(5,TABLERO[CONTADOR+8].X+210,TABLERO[CONTADOR+8].Y,4);
				//--> JOYAS BARRA SEPARADORA
				ENTERNAME(29,TABLERO[22].X,TABLERO[22].Y,5);
				ENTERNAME(31,TABLERO[23].X,TABLERO[23].Y,5);
				ENTERNAME(33,TABLERO[24].X,TABLERO[24].Y,5);
				ENTERNAME(35,TABLERO[25].X,TABLERO[25].Y,5);
				ENTERNAME(33,TABLERO[26].X,TABLERO[26].Y,5);
				ENTERNAME(31,TABLERO[27].X,TABLERO[27].Y,5);
				ENTERNAME(29,TABLERO[28].X,TABLERO[28].Y,5);

				FOR(CONTADOR=0; CONTADOR<28; CONTADOR++);
					ENTERNAME(CONTADOR,TABLERO[CONTADOR+28].X,TABLERO[CONTADOR+28].Y,3);
				END

				FILE=ENTERNAME_ID;
				GRAPH=CONTADOR;
				X=TABLERO[CONTADOR+28].X;
				Y=TABLERO[CONTADOR+28].Y;
				repeat
					frame;
				until MOUSE.LEFT AND collision(TYPE MOUSE);
				GRAPH+=50;
				sound(150);
				FRAME(1500);
				GRAPH-=50;
				frame;
				FOR(;NOMBRE<7;NOMBRE++);
					rhighscore[0].hname[NOMBRE]=" ";
				END
				SORT(rhighscore,10);
				save(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file",rhighscore);
			END //--> ENTER NAME
		end //--> case 3
	END //--> SWITCH

	tipo_glow=1;
	RUSH_LEVEL=200;
	NUM_NIVEL=0;
	fade_off(1000);
	music_fade_OFF(1000);

	fpg_unload(joyas);
	splash=fpg_load("graphics/splash.fpg");

	FRAME(10000);

	music_unload(music);
	music=music_load("music/musicintro.mp3");

	music_stop();
	music_play(music,-1);
	SWITCH(MODE)
		CASE 1:INTRO(6);END
		CASE 2:INTRO(8);END
		CASE 3:INTRO(7);END
	END
END


//****************************************************************
//***                      PROCESS CHECK()                     ***
//****************************************************************


process CHECK(); //--> pROCESO QUE CHECKEA SI EXISTE UNA CADENA MINIMA DE 3 CHAINS O UNA FICHA +
PRIVATE;
	int CONTADOR=0,contador2=0,contador3=0;
BEGIN

	while(exists(type explosion))
		frame;
	end
	IF(WAITCHECK==1) WAITCHECK=0; FRAME(5000); END
	IF(MODE<>2) frame(4000); END;
	FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
	IF(TABLERO[CONTADOR].GRAPH==41 or TABLERO[CONTADOR].GRAPH==100);CHECK_SI=5; frame; RETURN; END
	END

	FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);

	  IF(CONTADOR>14) //--> PARA EVITAR EL CHEKEO DE LA "LINEA 0" REFILLER
		IF(TABLERO[TABLERO[CONTADOR].ARRIBA].GRAPH==TABLERO[CONTADOR].GRAPH);
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ARRIBA].ARRIBA].GRAPH==TABLERO[CONTADOR].GRAPH and CONTADOR>21); check_who=contador-7;CHECK_SI=1; frame; RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ARRIBA].IZQUIERDA].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=1;frame; RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ARRIBA].DERECHA].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=1;frame; RETURN; END
		END
	  END
		IF(TABLERO[TABLERO[CONTADOR].ABAJO].GRAPH==TABLERO[CONTADOR].GRAPH);
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ABAJO].ABAJO].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=2; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ABAJO].IZQUIERDA].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=2; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].ABAJO].DERECHA].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=2; frame;RETURN; END
		END
		IF(TABLERO[TABLERO[CONTADOR].IZQUIERDA].GRAPH==TABLERO[CONTADOR].GRAPH);
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].IZQUIERDA].ABAJO].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=3; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].IZQUIERDA].IZQUIERDA].GRAPH==TABLERO[CONTADOR].GRAPH);check_who=contador-7; CHECK_SI=3; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].IZQUIERDA].ARRIBA].GRAPH==TABLERO[CONTADOR].GRAPH  and CONTADOR>21); check_who=contador-7;CHECK_SI=3; frame;RETURN; END
		END

		IF(TABLERO[TABLERO[CONTADOR].DERECHA].GRAPH==TABLERO[CONTADOR].GRAPH);
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].DERECHA].ABAJO].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=4; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].DERECHA].DERECHA].GRAPH==TABLERO[CONTADOR].GRAPH); check_who=contador-7;CHECK_SI=4; frame;RETURN; END
			IF(TABLERO[TABLERO[TABLERO[CONTADOR].DERECHA].ARRIBA].GRAPH==TABLERO[CONTADOR].GRAPH  and CONTADOR>21); check_who=contador-7;CHECK_SI=4;frame; RETURN; END
		END
	END
	CHECK_SI=99;
	sound(504);

	if(mode==3 or mode==1 or mode==2)
		RELOJ-=reloj/3;FLYT(38,128,Z,-reloj/3);
		CONTADOR3=RAND(8,56);
		TABLERO[CONTADOR3].GRAPH=100;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y;FLY(X,Y,Z,-10000,NUMJOYAS);

	end;
	frame;
END



//****************************************************************
//***                     PROCESS CHECK_GLASS                  ***
//****************************************************************

PROCESS CHECK_GLASS(); //--> CHEKEA SI TODO EL PANEL ESTA COMPLETO

PRIVATE;
int CONTADOR=0,CONTADOR3,contador4=0;
BEGIN
	FRAME(5000);
	FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
	IF(TABLERO[CONTADOR].ILUM==0); IF(PUZSION==4) BREAK; END; RETURN; END
	END
ENCIENDEME=1;
IF(PUZSION==4) SOUND(200); ELSE sound(101); END; puzsion=0;

//-------------------------------------------
	//sound(101);
	FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE SELECCIONAN TODAS LA JOYAS
		TABLERO[CONTADOR3].GLOW=1;

		END
		FRAME(12000);
		FOR(CONTADOR3=8;CONTADOR3<57;CONTADOR3++); //--> SE ELIMINAN TODAS LAS JOYAS
		TABLERO[CONTADOR3].DESTRUIR=1;
		END

		RESET(0,57,1);

		FRAME(5000);
//-------------------------------------------
	ENCIENDEME=0;
	RESET(8,57,3);
GLASS_NIVEL++;
RELOJ+=60;
SIGNAL(TYPE FLYT, S_KILL);
FLYT(38,124,Z,60);
FLY(38,152,Z,(RELOJ*1000)*(GLASS_NIVEL-1),NUMBIGSCORE);
		repeat
				SIGNAL(TYPE JEWEL, S_KILL);
				FOR(CONTADOR=8;CONTADOR<57;CONTADOR++);
				JEWEL(CONTADOR,TABLERO[CONTADOR].X,TABLERO[CONTADOR].Y,SORTEO[Rand(0,3)].NUMERO);
				END
			FRAME;	contador4++;
		until contador4==10;

signal(type level_glass,s_kill);
level_glass(GLASS_NIVEL);
END


//****************************************************************
//***          PROCESS COMBO(GRAFICO,TIPO,INCREMENTO)          ***
//****************************************************************


process combo(grafico,TIPO,INCREMENTO); //--> PROCESO QUE MUESTRA EN GRANDE EL NUMERO DE CHAIN
PRIVATE
int FAKEX, FAKEY;
BEGIN


	if(grafico<11 AND TIPO==1) x=37;y=75; graph=grafico; End
	if(grafico<11 AND TIPO==2) x=18;y=75; graph=grafico; SIZE=100; End
	if(grafico<11 AND TIPO==3) x=52;y=75; graph=grafico; SIZE=100; End
	IF(GRAFICO>10 AND GRAFICO<21) COMBO(2,2,INCREMENTO);COMBO(GRAFICO-10,3,INCREMENTO);RETURN; END
	IF(GRAFICO>20 AND GRAFICO<31) COMBO(3,2,INCREMENTO);COMBO(GRAFICO-20,3,INCREMENTO);RETURN; END
	IF(GRAFICO>30 AND GRAFICO<41) COMBO(4,2,INCREMENTO);COMBO(GRAFICO-30,3,INCREMENTO);RETURN; END
	IF(GRAFICO>40 AND GRAFICO<51) COMBO(5,2,INCREMENTO);COMBO(GRAFICO-40,3,INCREMENTO);RETURN; END
	IF(MODE==3) Y-=11;END

	SWITCH(MODE)
	CASE 1: file=numeroSQUOTA;END
	CASE 2: file=numeroSGLASS;END
	CASE 3: file=numeroSRUSH;incremento+=5;END
	CASE 3: file=numeroSRUSH;incremento+=5;END
	END

	Y+=INCREMENTO;
	FAKEX=X;
	FAKEY=Y;


	LOOP
	IF(CANDIDATO==RUSH_LEVEL and mode==3) SHAKE=1; ELSE SHAKE=0; eND
	IF(SHAKE==1)
	X=FAKEX+RAND(-2,2);
	Y=FAKEY+RAND(-2,2);
	ELSE
	X=FAKEX;
	Y=FAKEY;
	END
	frame;
	END;

end



//****************************************************************
//***            PROCESS COMBORUSH(GRAFICO,TIPO)               ***
//****************************************************************

process comboRUSH(grafico,TIPO); //--> PROCESO QUE MUESTRA EN GRANDE EL NUMERO DE CHAIN
PRIVATE
int FAKEX,FAKEY,sonar=0;
BEGIN


	if(grafico<11 AND TIPO==1) x=37;y=205; graph=grafico; End
	if(grafico<11 AND TIPO==2) x=18;y=205; graph=grafico; SIZE=100; End
	if(grafico<11 AND TIPO==3) x=52;y=205; graph=grafico; SIZE=100; End
	IF(GRAFICO>10 AND GRAFICO<21) COMBORUSH(2,2);COMBORUSH(GRAFICO-10,3);RETURN; END
	IF(GRAFICO>20 AND GRAFICO<31) COMBORUSH(3,2);COMBORUSH(GRAFICO-20,3);RETURN; END
	IF(GRAFICO>30 AND GRAFICO<41) COMBORUSH(4,2);COMBORUSH(GRAFICO-30,3);RETURN; END
	IF(GRAFICO>40 AND GRAFICO<51) COMBORUSH(5,2);COMBORUSH(GRAFICO-40,3);RETURN; END

	file=numeroSRUSH;

	FAKEX=X;
	FAKEY=Y;


	LOOP
	IF(CANDIDATO==RUSH_LEVEL) SHAKE=1; ELSE SHAKE=0; eND
	IF(SHAKE==1)
	sonar++;

	X=FAKEX+RAND(-2,2);
	Y=FAKEY+RAND(-2,2);
	ELSE
	X=FAKEX;
	Y=FAKEY;
	sonar=0;
	END
	frame;
	END;

end


//****************************************************************
//***               PROCESS FLY(X,Y,Z,FLY_NUM,LOL)             ***
//****************************************************************

PROCESS FLY(double X,Y,int Z,FLY_NUM,LOL);
//process FLY(X,Y,Z,FLY_NUM,LOL);
PRIVATE
int CONTADOR=0,ID_PUNTUACION;
BEGIN

	file=JOYAS;
	Y-=20;
	Z-=1;
	SCORE+=FLY_NUM;
	if(FLY_NUM==500 and mode<>3) reloj+=5; FLYT(38,128,Z,5);end
	if(FLY_NUM==1000 and mode<>3) reloj+=5; FLYT(38,128,Z,15);end
	if(FLY_NUM==1500 and mode<>3) reloj+=25; FLYT(38,128,Z,25);end
	if(FLY_NUM==2000 and mode<>3) reloj+=50; FLYT(38,128,Z,50);end

	LOOP
    	if(lol==NUMJOYAS)
            ID_PUNTUACION=write_var(LOL,X,Y,4,FLY_NUM);
    	    Y-=2;
            CONTADOR++;
            frame;
        else
            FLYS(X,Y+20,Z,FLY_NUM) ;
            BREAK ;
        end

	write_delete(ID_PUNTUACION);
	IF(CONTADOR==25) BREAK; END
	END
END


//****************************************************************
//***                   PROCESS SPEAKER(NAME)                  ***
//****************************************************************


process SPEAKER(NAME);
PRIVATE
int CONTADOR=0;
BEGIN
//--> DESCARGA DE SFX
	sound_unload(SFX_G1); SFX_G1 = NULL;
	sound_unload(SFX_J1); SFX_J1 = NULL;
	sound_unload(SFX_J2); SFX_J2 = NULL;
	sound_unload(SFX_J3); SFX_J3 = NULL;
	sound_unload(SFX_J4); SFX_J4 = NULL;
	sound_unload(SFX_J5); SFX_J5 = NULL;
	sound_unload(SFX_J6); SFX_J6 = NULL;
	sound_unload(SFX_J7); SFX_J7 = NULL;

	sound_unload(sfx_puzsion);       sfx_puzsion = NULL;
	sound_unload(sfx_qmode);         sfx_qmode = NULL;
	sound_unload(sfx_rmode);         sfx_rmode = NULL;
	sound_unload(Sfx_gmode);         Sfx_gmode = NULL;
	sound_unload(sfx_options);       sfx_options = NULL;
	sound_unload(sfx_hiscore);       sfx_hiscore = NULL;
	sound_unload(sfx_exitg);         sfx_exitg = NULL;
	sound_unload(sfx_start);         sfx_start = NULL;
	sound_unload(sfx_howto);         sfx_howto = NULL;
	sound_unload(sfx_exit);          sfx_exit = NULL;
	sound_unload(sfx_levelup);       sfx_levelup = NULL;
	sound_unload(sfx_fantastic);     sfx_fantastic = NULL;
	sound_unload(sfx_great);         sfx_great = NULL;
	sound_unload(sfx_combo);         sfx_combo = NULL;
	sound_unload(sfx_awesome);       sfx_awesome = NULL;
	sound_unload(SFX_SWEET);         SFX_SWEET = NULL;
	sound_unload(sfx_perfect);       sfx_perfect = NULL;
	sound_unload(sfx_extra);         sfx_extra = NULL;
	sound_unload(sfx_marve);         sfx_marve = NULL;
	sound_unload(sfx_outs);          sfx_outs = NULL;
	sound_unload(sfx_increible);     sfx_increible = NULL;

	sound_unload(SFX_superb);        SFX_superb = NULL;
	sound_unload(SFX_excellent);     SFX_excellent = NULL;
	sound_unload(SFX_priceless);     SFX_priceless = NULL;
	sound_unload(SFX_brillant);      SFX_brillant = NULL;
	sound_unload(SFX_smooth);        SFX_smooth = NULL;
	sound_unload(SFX_killer);        SFX_killer = NULL;

	sound_unload(SFX_super);         SFX_super = NULL;
	sound_unload(SFX_hyper);         SFX_hyper = NULL;
	sound_unload(SFX_ultra);         SFX_ultra = NULL;
	sound_unload(SFX_giga);          SFX_giga = NULL;
	sound_unload(SFX_mega);          SFX_mega = NULL;

	sound_unload(sfx_hurryup);       sfx_hurryup = NULL;
	sound_unload(sfx_gameover);      sfx_gameover = NULL;
	sound_unload(sfx_entername);     sfx_entername = NULL;
	sound_unload(sfx_pause);         sfx_pause = NULL;
	sound_unload(sfx_nomoves);       sfx_nomoves = NULL;
	sound_unload(sfx_destruction);   sfx_destruction = NULL;

SWITCH(NAME)
 CASE 1: //MALE
	SFX_G1= sound_load("sfx/g1.wav");
	SFX_J1= sound_load("sfx/mj1.wav");
	SFX_J2= sound_load("sfx/mj2.wav");
	SFX_J3= sound_load("sfx/mj3.wav");
	SFX_J4= sound_load("sfx/mj4.wav");
	SFX_J5= sound_load("sfx/mj5.wav");
	SFX_J6= sound_load("sfx/mj6.wav");
	SFX_J7= sound_load("sfx/mj7.wav");

	sfx_puzsion= sound_load("sfx/mpuzsion.wav");
	sfx_qmode= sound_load("sfx/mqmode.wav");
	sfx_rmode= sound_load("sfx/mrmode.wav");
	sfx_gmode= sound_load("sfx/mgmode.wav");
	sfx_options= sound_load("sfx/moptions.wav");
	sfx_hiscore= sound_load("sfx/mhiscore.wav");
	sfx_exitg= sound_load("sfx/mexitg.wav");
	sfx_start= sound_load("sfx/mstart.wav");
	sfx_howto= sound_load("sfx/mhowto.wav");
	sfx_exit= sound_load("sfx/mexit.wav");
	sfx_levelup= sound_load("sfx/mlevelup.wav");
	sfx_fantastic= sound_load("sfx/mfantastic.wav");
	sfx_great= sound_load("sfx/mgreat.wav");
	sfx_combo= sound_load("sfx/mcombo.wav");
	sfx_awesome= sound_load("sfx/mawesome.wav");
	SFX_SWEET= sound_load("sfx/msweet.wav");
	sfx_perfect= sound_load("sfx/mperfect.wav");
	sfx_extra= sound_load("sfx/mextra.wav");
	sfx_marve=sound_load("sfx/mmarve.wav");
	sfx_outs= sound_load("sfx/mouts.wav");
	sfx_increible= sound_load("sfx/mincreible.wav");

	SFX_superb= sound_load("sfx/msuperb.wav");
	SFX_excellent= sound_load("sfx/mexcellent.wav");
	SFX_priceless= sound_load("sfx/mpriceless.wav");
	SFX_brillant= sound_load("sfx/mbrillant.wav");
	SFX_smooth= sound_load("sfx/msmooth.wav");
	SFX_killer= sound_load("sfx/mkiller.wav");

	SFX_super= sound_load("sfx/msuper.wav");
	SFX_hyper= sound_load("sfx/mhyper.wav");
	SFX_ultra= sound_load("sfx/multra.wav");
	SFX_giga= sound_load("sfx/mgiga.wav");
	SFX_mega= sound_load("sfx/mmega.wav");

	sfx_hurryup = sound_load("sfx/mhurryup.wav");;
	sfx_gameover = sound_load("sfx/mgameover.wav");;
	sfx_entername = sound_load("sfx/mentername.wav");
	sfx_pause  = sound_load("sfx/mpause.wav");
	sfx_nomoves = sound_load("sfx/mnomoves.wav");
	sfx_destruction = sound_load("sfx/mdestruction.wav");
 END
 CASE 3: //FEMALE
	SFX_G1= sound_load("sfx/g1.wav");
	SFX_J1= sound_load("sfx/j1.wav");
	SFX_J2= sound_load("sfx/j2.wav");
	SFX_J3= sound_load("sfx/j3.wav");
	SFX_J4= sound_load("sfx/j4.wav");
	SFX_J5= sound_load("sfx/j5.wav");
	SFX_J6= sound_load("sfx/j6.wav");
	SFX_J7= sound_load("sfx/j7.wav");

	sfx_puzsion= sound_load("sfx/puzsion.wav");
	sfx_qmode= sound_load("sfx/qmode.wav");
	sfx_rmode= sound_load("sfx/rmode.wav");
	sfx_gmode= sound_load("sfx/gmode.wav");
	sfx_options= sound_load("sfx/options.wav");
	sfx_hiscore= sound_load("sfx/hiscore.wav");
	sfx_exitg= sound_load("sfx/exitg.wav");
	sfx_start= sound_load("sfx/start.wav");
	sfx_howto= sound_load("sfx/howto.wav");
	sfx_exit= sound_load("sfx/exit.wav");
	sfx_levelup= sound_load("sfx/levelup.wav");
	sfx_fantastic= sound_load("sfx/fantastic.wav");
	sfx_great= sound_load("sfx/great.wav");
	sfx_combo= sound_load("sfx/combo.wav");
	sfx_awesome= sound_load("sfx/awesome.wav");
	SFX_SWEET= sound_load("sfx/sweet.wav");
	sfx_perfect= sound_load("sfx/perfect.wav");
	sfx_extra= sound_load("sfx/extra.wav");
	sfx_marve=sound_load("sfx/marve.wav");
	sfx_outs= sound_load("sfx/outs.wav");
	sfx_increible= sound_load("sfx/increible.wav");

	SFX_superb= sound_load("sfx/superb.wav");
	SFX_excellent= sound_load("sfx/excellent.wav");
	SFX_priceless= sound_load("sfx/priceless.wav");
	SFX_brillant= sound_load("sfx/brillant.wav");
	SFX_smooth= sound_load("sfx/smooth.wav");
	SFX_killer= sound_load("sfx/killer.wav");

	SFX_super= sound_load("sfx/super.wav");
	SFX_hyper= sound_load("sfx/hyper.wav");
	SFX_ultra= sound_load("sfx/ultra.wav");
	SFX_giga= sound_load("sfx/giga.wav");
	SFX_mega= sound_load("sfx/mega.wav");

	sfx_hurryup = sound_load("sfx/hurryup.wav");;
	sfx_gameover = sound_load("sfx/gameover.wav");;
	sfx_entername = sound_load("sfx/entername.wav");
	sfx_pause  = sound_load("sfx/pause.wav");
	sfx_nomoves = sound_load("sfx/nomoves.wav");
	sfx_destruction = sound_load("sfx/destruction.wav");
 END
 CASE 5: //NO VOICE
	SFX_G1= sound_load("sfx/g1.wav");
	SFX_J1= sound_load("sfx/j1.wav");
	SFX_J2= sound_load("sfx/j2.wav");
	SFX_J3= sound_load("sfx/j3.wav");
	SFX_J4= sound_load("sfx/j4.wav");
	SFX_J5= sound_load("sfx/j5.wav");
	SFX_J6= sound_load("sfx/j6.wav");
	SFX_J7= sound_load("sfx/j7.wav");

	sfx_puzsion= sound_load("sfx/j7.wav");
	sfx_qmode= sound_load("sfx/j7.wav");
	sfx_rmode= sound_load("sfx/j7.wav");
	sfx_gmode= sound_load("sfx/j7.wav");
	sfx_options= sound_load("sfx/j7.wav");
	sfx_hiscore= sound_load("sfx/j7.wav");
	sfx_exitg= sound_load("sfx/j7.wav");
	sfx_start= sound_load("sfx/j7.wav");
	sfx_howto= sound_load("sfx/j7.wav");
	sfx_exit= sound_load("sfx/j7.wav");
	sfx_levelup= sound_load("sfx/j7.wav");
	sfx_fantastic= sound_load("sfx/j7.wav");
	sfx_great= sound_load("sfx/j7.wav");
	sfx_combo= sound_load("sfx/j7.wav");
	sfx_awesome= sound_load("sfx/j7.wav");
	SFX_SWEET= sound_load("sfx/j7.wav");
	sfx_perfect= sound_load("sfx/j7.wav");
	sfx_extra= sound_load("sfx/j7.wav");
	sfx_marve=sound_load("sfx/j7.wav");
	sfx_outs= sound_load("sfx/j7.wav");
	sfx_increible= sound_load("sfx/j7.wav");

	SFX_superb= sound_load("sfx/j7.wav");
	SFX_excellent= sound_load("sfx/j7.wav");
	SFX_priceless= sound_load("sfx/j7.wav");
	SFX_brillant= sound_load("sfx/j7.wav");
	SFX_smooth= sound_load("sfx/j7.wav");
	SFX_killer= sound_load("sfx/j7.wav");

	SFX_super= sound_load("sfx/j7.wav");
	SFX_hyper= sound_load("sfx/j7.wav");
	SFX_ultra= sound_load("sfx/j7.wav");
	SFX_giga= sound_load("sfx/j7.wav");
	SFX_mega=sound_load("sfx/j7.wav");

	sfx_hurryup = sound_load("sfx/j7.wav");
	sfx_gameover = sound_load("sfx/j7.wav");
	sfx_entername = sound_load("sfx/j7.wav");
	sfx_pause  = sound_load("sfx/j7.wav");
	sfx_nomoves = sound_load("sfx/j7.wav");
	sfx_destruction = sound_load("sfx/j7.wav");
 END
END //SWITCH
END // PROCESS



//****************************************************************
//***               PROCESS FLYS(X,Y,Z,FLYSCORE)               ***
//****************************************************************

PROCESS FLYS(double X,Y,int Z,FLYSCORE);
PRIVATE
int CONTADOR=0,ID_FLYSCORE,contador2=0;
BEGIN

	file=JOYAS;
	IF(FLYSCORE>0) Y-=10; ELSE Y+=5; END
	Z-=1;

	LOOP
	ID_FLYSCORE=write_var(numBIGSCORE,X,Y,4,FLYSCORE);
	IF(FLYSCORE>0) Y-=2; ELSE Y+=2; END
	CONTADOR++;contador2++;
	if(contador2<5) x--; end
	if(contador2>5 AND contador2<11) x++; end
	if(contador2>11 AND contador2<13) x--; end
	IF(CONTADOR2=>13) CONTADOR2=0; END

	frame(200);
	write_delete(ID_FLYSCORE);
	IF(CONTADOR==40) BREAK; END
	END
END

//****************************************************************
//***               PROCESS FLYT(X,Y,Z,FLYTIME)                ***
//****************************************************************

PROCESS FLYT(double X,Y,int Z,FLYTIME);
PRIVATE
int CONTADOR=0,ID_FLYTIME,contador2=0;
BEGIN

	file=JOYAS;
	IF(FLYTIME>0) Y-=10; ELSE Y+=5; END
	Z-=1;

	LOOP
	ID_FLYTIME=write_var(numlevel,X,Y,4,FLYTIME);
	IF(FLYTIME>0) Y-=2; ELSE Y+=2; END
	CONTADOR++;contador2++;
	if(contador2<5) x++; end
	if(contador2>5 AND contador2<11) x--; end
	if(contador2>11 AND contador2<13) x++; end
	IF(CONTADOR2=>13) CONTADOR2=0; END

	frame(200);
	write_delete(ID_FLYTIME);
	IF(CONTADOR==40) BREAK; END
	END
END

//---------------------------------------> HIGHSCORES ON
//****************************************************************
//***                   PROCESS MERGEHIGHSCORES                 ***
//****************************************************************

process mergerhighscore()

private
int CONTADOR=0;

begin

	load(get_pref_path("bennugd.org", "puzsion") + "newrhighscore.file", mergerhighscore_dat);
	FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
		IF(mergerhighscore_dat[CONTADOR].hscore>rhighscore[0].hscore)
			rhighscore[0].hscore=mergerhighscore_dat[CONTADOR].hscore;
			rhighscore[0].hfase=mergerhighscore_dat[CONTADOR].hfase;
			rhighscore[0].last=0;
			rhighscore[0].dia=mergerhighscore_dat[CONTADOR].dia;
			rhighscore[0].tiempo=mergerhighscore_dat[CONTADOR].tiempo;

			rhighscore[0].hname[0]=mergerhighscore_dat[CONTADOR].hname[0];
			rhighscore[0].hname[1]=mergerhighscore_dat[CONTADOR].hname[1];
			rhighscore[0].hname[2]=mergerhighscore_dat[CONTADOR].hname[2];
			rhighscore[0].hname[3]=mergerhighscore_dat[CONTADOR].hname[3];
			rhighscore[0].hname[4]=mergerhighscore_dat[CONTADOR].hname[4];
			rhighscore[0].hname[5]=mergerhighscore_dat[CONTADOR].hname[5];
			rhighscore[0].hname[6]=mergerhighscore_dat[CONTADOR].hname[6];

			SORT(rhighscore,10);
		end
	end
	save(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file",rhighscore);
	rm(get_pref_path("bennugd.org", "puzsion") + "newrhighscore.file");
end

//****************************************************************
//***                   PROCESS MERGEHIGHSCORES                 ***
//****************************************************************

process mergeqhighscore()

private
int CONTADOR=0;

begin

	load(get_pref_path("bennugd.org", "puzsion") + "newqhighscore.file", mergeqhighscore_dat);
	FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
		IF(mergeqhighscore_dat[CONTADOR].hscore>qhighscore[0].hscore)
			qhighscore[0].hscore=mergeqhighscore_dat[CONTADOR].hscore;
			qhighscore[0].hfase=mergeqhighscore_dat[CONTADOR].hfase;
			qhighscore[0].last=0;
			qhighscore[0].dia=mergeqhighscore_dat[CONTADOR].dia;
			qhighscore[0].tiempo=mergeqhighscore_dat[CONTADOR].tiempo;

			qhighscore[0].hname[0]=mergeqhighscore_dat[CONTADOR].hname[0];
			qhighscore[0].hname[1]=mergeqhighscore_dat[CONTADOR].hname[1];
			qhighscore[0].hname[2]=mergeqhighscore_dat[CONTADOR].hname[2];
			qhighscore[0].hname[3]=mergeqhighscore_dat[CONTADOR].hname[3];
			qhighscore[0].hname[4]=mergeqhighscore_dat[CONTADOR].hname[4];
			qhighscore[0].hname[5]=mergeqhighscore_dat[CONTADOR].hname[5];
			qhighscore[0].hname[6]=mergeqhighscore_dat[CONTADOR].hname[6];

			SORT(qhighscore,10);
		end
	end
	save(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file",qhighscore);
	rm(get_pref_path("bennugd.org", "puzsion") + "newqhighscore.file");
end

//****************************************************************
//***                   PROCESS MERGEHIGHSCORES                 ***
//****************************************************************

process mergeghighscore()

private
int CONTADOR=0;

begin

load(get_pref_path("bennugd.org", "puzsion") + "newghighscore.file", mergeghighscore_dat);
	FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
		IF(mergeghighscore_dat[CONTADOR].hscore>ghighscore[0].hscore)
			ghighscore[0].hscore=mergeghighscore_dat[CONTADOR].hscore;
			ghighscore[0].hfase=mergeghighscore_dat[CONTADOR].hfase;
			ghighscore[0].last=0;
			ghighscore[0].dia=mergeghighscore_dat[CONTADOR].dia;
			ghighscore[0].tiempo=mergeghighscore_dat[CONTADOR].tiempo;

			ghighscore[0].hname[0]=mergeghighscore_dat[CONTADOR].hname[0];
			ghighscore[0].hname[1]=mergeghighscore_dat[CONTADOR].hname[1];
			ghighscore[0].hname[2]=mergeghighscore_dat[CONTADOR].hname[2];
			ghighscore[0].hname[3]=mergeghighscore_dat[CONTADOR].hname[3];
			ghighscore[0].hname[4]=mergeghighscore_dat[CONTADOR].hname[4];
			ghighscore[0].hname[5]=mergeghighscore_dat[CONTADOR].hname[5];
			ghighscore[0].hname[6]=mergeghighscore_dat[CONTADOR].hname[6];

			SORT(ghighscore,10);
		end
	end
	save(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file",ghighscore);
	rm(get_pref_path("bennugd.org", "puzsion") + "newghighscore.file");
end
//****************************************************************
//***                   PROCESS INITHIGHSCORES                 ***
//****************************************************************

process initHighscore()

private
int CONTADOR=0;
int i=0;

begin

if(fexists(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file"))

 //File exists and can be loaded:
 load(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file", rhighscore);

else

 //File does not exist, fill the highscore table with standard values:
 for(i = 0;i < 10; i++ )
   //Fill in a generated score(lower index means higher score)
   rhighscore[i].hscore = (10-i)*100000;
   rhighscore[i].hfase = 3;
   rhighscore[i].dia=fecha;
   rhighscore[i].tiempo=ftime("%H:%M:%S",GTM+raND(25,300));
 end
   FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
	rhighscore[CONTADOR].hname[0]="R";
	rhighscore[CONTADOR].hname[1]="U";
	rhighscore[CONTADOR].hname[2]="Z";
	rhighscore[CONTADOR].hname[3]="S";
	rhighscore[CONTADOR].hname[4]="I";
	rhighscore[CONTADOR].hname[5]="O";
	rhighscore[CONTADOR].hname[6]="N";
	END

   SORT(rhighscore,10);

end
//-----------------------------------------------
CONTADOR=0;
i=0;

if(fexists(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file"))

 //File exists and can be loaded:
 load(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file", ghighscore);

else

 //File does not exist, fill the highscore table with standard values:
 for(i = 0;i < 10; i++ )
   //Fill in a generated score(lower index means higher score)
   ghighscore[i].hscore = (10-i)*100000;
   ghighscore[i].hfase = 1;
   ghighscore[i].dia=fecha;
   ghighscore[i].tiempo=ftime("%H:%M:%S",GTM+raND(25,300));

 end
   FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
	ghighscore[CONTADOR].hname[0]="G";
	ghighscore[CONTADOR].hname[1]="U";
	ghighscore[CONTADOR].hname[2]="Z";
	ghighscore[CONTADOR].hname[3]="S";
	ghighscore[CONTADOR].hname[4]="I";
	ghighscore[CONTADOR].hname[5]="O";
	ghighscore[CONTADOR].hname[6]="N";
	END

   SORT(ghighscore,10);

end

//-----------------------------------------------
CONTADOR=0;
i=0;

if(fexists(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file"))

 //File exists and can be loaded:
 load(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file", qhighscore);

else

 //File does not exist, fill the highscore table with standard values:
 for(i = 0;i < 10; i++ )
   //Fill in a generated score(lower index means higher score)
   qhighscore[i].hscore = (10-i)*100000;
   qhighscore[i].hfase = 1;
   qhighscore[i].dia=fecha;
   qhighscore[i].tiempo=ftime("%H:%M:%S",GTM+raND(25,300));
 end
   FOR (CONTADOR=0;CONTADOR<10;CONTADOR++)
	qhighscore[CONTADOR].hname[0]="Q";
	qhighscore[CONTADOR].hname[1]="U";
	qhighscore[CONTADOR].hname[2]="Z";
	qhighscore[CONTADOR].hname[3]="S";
	qhighscore[CONTADOR].hname[4]="I";
	qhighscore[CONTADOR].hname[5]="O";
	qhighscore[CONTADOR].hname[6]="N";
	END

   SORT(qhighscore,10);

end

end
//-----------------------------------------------


//****************************************************************
//***                   PROCESS PRINTrhighscore                ***
//****************************************************************


process printrhighscore()

private
int i, contador=0;


begin

for( i = 0; i <10; i++ )



	if(rhighscore[i].last==0);
		if(i==9);
		write_int(whiscore,105,221-i*16,2,&rhighscore[i].hfase);
		write_int(whiscore, 85,221-i*16,2,&rhighscore[i].hscore);
		write_string(whiscore,235,221-i*16,2,&rhighscore[i].dia);
		write_string(whiscore,165,221-i*16,2,&rhighscore[i].TIEMPO);



			for( contador = 0;contador <7;contador++ )
				write_string(whiscore, 250+contador*8,221-i*16,1,&rhighscore[i].hname[contador]);
			end
		else
		write_int(rhiscore,105,221-i*16,2,&rhighscore[i].hfase);
		write_int(rhiscore, 85,221-i*16,2,&rhighscore[i].hscore);
		write_string(rhiscore,235,221-i*16,2,&rhighscore[i].dia);
		write_string(rhiscore,165,221-i*16,2,&rhighscore[i].TIEMPO);


			for( contador = 0;contador <7;contador++ )
				write_string(rhiscore, 250+contador*8,221-i*16,1,&rhighscore[i].hname[contador]);
			end
		end
	ELSE
		write_int(yhiscore,105,221-i*16,2,&rhighscore[i].hfase);
		write_int(yhiscore, 85,221-i*16,2,&rhighscore[i].hscore);
		write_string(yhiscore,235,221-i*16,2,&rhighscore[i].dia);
	write_string(yhiscore,165,221-i*16,2,&rhighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(yhiscore, 250+contador*8,221-i*16,1,&rhighscore[i].hname[contador]);
			end
		rhighscore[I].last=0;
		save(get_pref_path("bennugd.org", "puzsion") + "rhighscore.file",rhighscore);
	END

end

end


//****************************************************************
//***                   PROCESS PRINTghighscore                ***
//****************************************************************

process printghighscore()

private
int i, contador=0;

begin

for( i = 0; i <10; i++ )



	if(ghighscore[i].last==0);
	if(i==9);
		write_int(whiscore,105,221-i*16,2,&ghighscore[i].hfase);
		write_int(whiscore, 85,221-i*16,2,&ghighscore[i].hscore);
		write_string(whiscore,235,221-i*16,2,&ghighscore[i].dia);
		write_string(whiscore,165,221-i*16,2,&ghighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(whiscore, 250+contador*8,221-i*16,1,&ghighscore[i].hname[contador]);
			end
		else
		write_int(ghiscore,105,221-i*16,2,&ghighscore[i].hfase);
		write_int(ghiscore, 85,221-i*16,2,&ghighscore[i].hscore);
		write_string(ghiscore,235,221-i*16,2,&ghighscore[i].dia);
		write_string(ghiscore,165,221-i*16,2,&ghighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(ghiscore, 250+contador*8,221-i*16,1,&ghighscore[i].hname[contador]);
			end
		end
	ELSE
		write_int(yhiscore,105,221-i*16,2,&ghighscore[i].hfase);
		write_int(yhiscore, 85,221-i*16,2,&ghighscore[i].hscore);
		write_string(yhiscore,235,221-i*16,2,&ghighscore[i].dia);
		write_string(yhiscore,165,221-i*16,2,&ghighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(yhiscore, 250+contador*8,221-i*16,1,&ghighscore[i].hname[contador]);
			end
		ghighscore[I].last=0;
		save(get_pref_path("bennugd.org", "puzsion") + "ghighscore.file",ghighscore);
	END

end

end


//****************************************************************
//***                   PROCESS PRINTqhighscore                ***
//****************************************************************

process printqhighscore()

private
int i, contador=0;


begin

for( i = 0; i <10; i++ )

	if(qhighscore[i].last==0);
	if(i==9);
		write_int(whiscore,105,221-i*16,2,&qhighscore[i].hfase);
		write_int(whiscore, 85,221-i*16,2,&qhighscore[i].hscore);
		write_string(whiscore,235,221-i*16,2,&qhighscore[i].dia);
		write_string(whiscore,165,221-i*16,2,&qhighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(whiscore, 250+contador*8,221-i*16,1,&qhighscore[i].hname[contador]);

			end
		else
		write_int(qhiscore,105,221-i*16,2,&qhighscore[i].hfase);
		write_int(qhiscore, 85,221-i*16,2,&qhighscore[i].hscore);
		write_string(qhiscore,235,221-i*16,2,&qhighscore[i].dia);
		write_string(qhiscore,165,221-i*16,2,&qhighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(qhiscore, 250+contador*8,221-i*16,1,&qhighscore[i].hname[contador]);
			end
		end
	ELSE
		write_int(yhiscore,105,221-i*16,2,&qhighscore[i].hfase);
		write_int(yhiscore, 85,221-i*16,2,&qhighscore[i].hscore);
		write_string(yhiscore,235,221-i*16,2,&qhighscore[i].dia);
		write_string(yhiscore,165,221-i*16,2,&qhighscore[i].TIEMPO);

			for( contador = 0;contador <7;contador++ )
				write_string(yhiscore, 250+contador*8,221-i*16,1,&qhighscore[i].hname[contador]);
			end
		qhighscore[I].last=0;
		save(get_pref_path("bennugd.org", "puzsion") + "qhighscore.file",qhighscore);
	END

end

end






//---------------------------------------> HIGHSCORES Off

//****************************************************************
//***                      PROCESS RESET                       ***
//****************************************************************

PROCESS RESET(INICIO,FINAL,ELECCION);
PRIVATE
int CONTADOR=0;
BEGIN
	FOR(CONTADOR=INICIO;CONTADOR<FINAL;CONTADOR++);
	IF(ELECCION==1) TABLERO[CONTADOR].GLOW=0;END
	IF(ELECCION==2) LISTA[CONTADOR].ELIMINAR=0;END
	IF(ELECCION==3) TABLERO[CONTADOR].ILUM=0;END

	END

END

PROCESS EXPLOSION();
PRIVATE
int CONTADOR=0, contador2=0, CONTADOR3;
BEGIN
X=40;
Y=150;
Z=FATHER.Z-1;
	signal(type puntero,s_sleep);

	FOR(;CANDIDATO>0;CANDIDATO--);
		IF(TABLERO[LISTA[CANDIDATO].ELIMINAR].GLOW<>0)
			TABLERO[LISTA[CANDIDATO].ELIMINAR].DESTRUIR=1;
			FRAME(1500);

			TABLERO[LISTA[CANDIDATO].ELIMINAR].glow=0;
		END

	END
	WHILE(TABLERO[LISTA[CANDIDATO+1].ELIMINAR].DESTRUIR<>0)
	FRAME;
	END
	CANDIDATO=1;

	//--> CHAIN DE MAS DE 5 COMBO PUNTOS
	IF(PUZSION==1) PUZSION=0;
	if(mode==2) frame(6000); end
	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=41;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y;FLY(X,Y,Z,500,NUMJOYAS); FLY(38,152,Z,500,NUMBIGSCORE); if(mode<>3) reloj-=5; END; SCORE-=500;
	sound(150);HIT(X,Y,Z);
	END

	//--> CHAIN DE MAS DE 10 JOYAS CREAMOS LA JEWEL UNITE x 3
	IF(PUZSION==2) PUZSION=0;
	if(mode==2) frame(6000); end
	CONTADOR2=3;
	sound(250);
	REPEAT
	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=41;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y; FLY(X,Y,Z,1000,NUMJOYAS); FLY(38,152,Z,3000,NUMBIGSCORE); SCORE-=3000;
	HIT(X,Y,Z);
	CONTADOR2--;
	UNTIL CONTADOR2==0;

	END

	//--> CHAIN DE MAS DE 15 JOYAS CREAMOS LA JEWEL PUZSION
	IF(PUZSION==3) PUZSION=0; CONTADOR2=3;
	if(mode==2) frame(6000); end
	REPEAT
	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=41;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y; FLY(X,Y,Z,1000,NUMJOYAS);
	HIT(X,Y,Z);
	CONTADOR2--;
	UNTIL CONTADOR2==0;

	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=100;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y; FLY(X,Y,Z,2000,NUMJOYAS);  FLY(38,152,Z,5000,NUMBIGSCORE); SCORE-=5000;
	sound(350);HIT(X,Y,Z); END

	//--> CHAIN DE MAS DE 20 JOYAS=PETAMOS EL TABLERO
	IF(PUZSION==4 AND MODE==3) PUZSION=0; CONTADOR2=5;
	REPEAT
	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=41;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y; FLY(X,Y,Z,2000,NUMJOYAS);
	HIT(X,Y,Z);
	CONTADOR2--;
	UNTIL CONTADOR2==0;
	CONTADOR3=RAND(8,56);
	TABLERO[CONTADOR3].GRAPH=100;X=TABLERO[CONTADOR3].X;Y=TABLERO[CONTADOR3].Y; FLY(X,Y,Z,9999,NUMJOYAS);  FLY(38,152,Z,500,NUMBIGSCORE); SCORE-=9999;
	sound(200);HIT(X,Y,Z); END

	signal(type puntero,s_wakeup);
	IF(MODE==1 OR MODE==3) check(); END
	IF(MODE==2) frame(5000);check();check_glass(); END


	END
