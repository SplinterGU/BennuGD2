 // Bomberlink (18-4-02)
// Version: 1.5
PROGRAM Joselkiller;
import "libmod_input";
import "libmod_gfx";
import "libmod_misc";
import "libmod_sound";

#define clear_screen()

declare PROCESS controles();
declare PROCESS controlador_principal();
declare PROCESS avion_destrozada( double x, y );
declare PROCESS aterrizaje_nave();
declare PROCESS josel_chafao( double x, double y );
declare PROCESS msn_engine( int graph, int tipo );
declare PROCESS gameover();
declare PROCESS titover();
declare PROCESS menu_del_juego();
declare PROCESS desplaza_menu( double x, double y, int graph, int flags );
declare PROCESS tabla_del_menu( int graph );
declare PROCESS usar_objetos();
declare PROCESS selecciona_objetos();
declare PROCESS congelar( int tipo );
declare PROCESS pausa_juego();
declare PROCESS titobjetivos( int graph );
declare PROCESS objetivos_mision( int tipo );
declare PROCESS objetivos_texto( int graph );
declare PROCESS fin_mision( int tipo );
declare PROCESS fin_fase();
declare PROCESS siguiente_mundo();
declare PROCESS titpausa();
declare PROCESS continuando();
declare PROCESS continuar_partida();
declare PROCESS reiniciar_partida();
declare PROCESS reinicia_partida();
declare PROCESS salir_partida();
declare PROCESS extermina_partida();
declare PROCESS arma_players( int ke_player );
declare PROCESS municion_players( int ke_player );
declare PROCESS vida_player1();
declare PROCESS tapa_scroll( double x, double y, int graph );
declare PROCESS tercer_scroll( double x, double y, int graph );
declare PROCESS fase1_desierto();
declare PROCESS coche_enemigo( double x, double y, int graph, int file );
declare PROCESS salta_barril( double x, double y, int graph, int file );
declare PROCESS coche( double x, double y, int tipo );
declare PROCESS fase1_a();
declare PROCESS Barril( double x, double y, int file, int graph, int tipo );
declare PROCESS fase1_b();
declare PROCESS prohibido_elpaso( double x, double y, int file, int graph, int tipo );
declare PROCESS rail_robot( double x, double y, int file, int graph, int tipo );
declare PROCESS robot( double x, double y, int file, int graph, int tipo );
declare PROCESS fase1_c();
declare PROCESS fase1_d();
declare PROCESS fase2_jungla();
declare PROCESS fase2_a();
declare PROCESS sal_pantalla( double x, double y, int graph );
declare PROCESS tapa_base( double x, double y );
declare PROCESS fase2_b();
declare PROCESS helicoptero3( double x, double y );
declare PROCESS descenso_camara();
declare PROCESS descenso_camara1();
declare PROCESS ascensOR ();
declare PROCESS fase2_c();
declare PROCESS fase2_d();
declare PROCESS fogon_programado();
declare PROCESS barril_sorpresa();
declare PROCESS fase3_nieve();
declare PROCESS nave( double x, double y );
declare PROCESS platillo();
declare PROCESS nave_enemiga( double y );
declare PROCESS disparo_nave( double x, double y );
declare PROCESS disparo_nave_enemiga( double x, double y );
declare PROCESS camara_nave();
declare PROCESS fase3_a();
declare PROCESS viga( double x, double y );
declare PROCESS fase3_b();
declare PROCESS Prepara_piedras();
declare PROCESS pedrazo( double x, double y );
declare PROCESS fase3_c();
declare PROCESS tapa_fondo( int graph );
declare PROCESS crea_nieve();
declare PROCESS copo_nieve( double x, double y, double y_total );
declare PROCESS pared_atras( double x, double y );
declare PROCESS parte_esc( double x, double y, int file, int graph, int tipo );
declare PROCESS plataforma( double x, double y, int file, int graph, int tipo );
declare PROCESS bola_pinchos( double x, double y );
declare PROCESS gusano( double x, double y, int graph, int incr_x );
declare PROCESS barco( double x, double y, int file, int graph, int tipo );
declare PROCESS agua( double x, double y, int file, int graph, int tipo );
declare PROCESS burbuja( double x, double y, double y_total );
declare PROCESS onda_agua( double x, double y, int tipo );
declare PROCESS salpicada( double x, double y, double size );
declare PROCESS agua_reflejo( double x, double y, int file, int graph, int tipo );
declare PROCESS espejado( double x, double y, int file, int graph, int tipo );
declare PROCESS abre_fase( int tipo );
declare PROCESS filtro_rapido( int tipo, double y );
declare PROCESS numero_pantalla( int graph );
declare PROCESS cartel_fin( double x, double y );
declare PROCESS blur_efect( double x, double y, int file, int graph, int angle );
declare PROCESS imita_bala();
declare PROCESS imita_2();
declare PROCESS imita_josel();
declare PROCESS sombras( double x, double y, int graph );
declare PROCESS brazo_josel( double x, double y );
declare PROCESS camara_juego();
declare PROCESS camara_vertical();
declare PROCESS camara_horizontal();
declare PROCESS camara_juego1();
declare PROCESS brazo_arma( double x, double y );
declare PROCESS arma_gastada( double x, double y, int graph, int flags );
declare PROCESS paracaidas( double x, double y, int tipo );
declare PROCESS jose_player1( double x, double y );
declare PROCESS onda_exp( double x, double y );
declare PROCESS items_info( double x, double y, int graph );
declare PROCESS items( double x, double y, int tipo );
declare PROCESS escopetazo( double x, double y, int estado );
declare PROCESS llamazo( double x, double y, int estado );
declare PROCESS fogon( double x, double y, int estado );
declare PROCESS torpedo( double x, double y );
declare PROCESS torpedo3( double x, double y );
declare PROCESS lancha( double x, double y );
declare PROCESS torpedo2( double x, double y, int tipo );
declare PROCESS trozo_torpedo( double x, double y, int tipo );
declare PROCESS bala( double x, double y, int graph, int sentido, int estado );
declare PROCESS barracada( double x, double y, int tipo );
declare PROCESS pestanea( double x, double y, int flags );
declare PROCESS vaya_ostia( double x, double y, int flags );
declare PROCESS resto_bala( double x, double y, int sentido );
declare PROCESS brillo_disparo( double x, double y, int sentido, int estado );
declare PROCESS brillo_cuerpo( double x, double y, int sentido, int player );
declare PROCESS vagoneta( double x, double y, int graph, int file );
declare PROCESS cacho_vagoneta( double x, double y );
declare PROCESS tanke( double x, double y, int graph, int file );
declare PROCESS rayazo();
declare PROCESS disparo_monstruo( double x, double y, int tipo );
declare PROCESS monstruo1( double x, double y );
declare PROCESS pincho_monstruo( double x, double y, int graph );
declare PROCESS canones( double x, double y, int tipo );
declare PROCESS monstruo2( double x, double y );
declare PROCESS rayo_monstruo3();
declare PROCESS asteroide();
declare PROCESS prepara_asteroides();
declare PROCESS nave_monstruo3();
declare PROCESS mini_nave( double x, double y, int tipo );
declare PROCESS nave_inversa();
declare PROCESS rayo_inverso();
declare PROCESS monstruo3( double x, double y );
declare PROCESS monstruo_encargado();
declare PROCESS final_juego();
declare PROCESS cuadro_final( int tipo );
declare PROCESS nostalgia();
declare PROCESS espera_monstruo();
declare PROCESS daldalboom( double x, double y, int estado );
declare PROCESS super_onda( double x, double y );
declare PROCESS mini_onda( double x, double y );
declare PROCESS movilatak( double x, double y, int estado );
declare PROCESS ultraboom( double x, double y, int estado );
declare PROCESS prepara_bombazos();
declare PROCESS bola_efecto( double x, double y, int graph, int file, int tipo );
declare PROCESS lluvia1();
declare PROCESS enemigo_atropellado( double x, double y, int graph, int file );
declare PROCESS enemigo_achicharrao( double x, double y );
declare PROCESS sangre_mix( int file, double x, double y, int flags, int tipo );
declare PROCESS enemigo_arabe( double x, double y, int tipo, int estado, int inteligencia );
declare PROCESS cacho_cuerpo_enemigo( double x, double y, int graph, int file, int tipo );
declare PROCESS enemigo_dispara( double x, double y, int file, int tipo );
declare PROCESS llamas_del_suelo();
declare PROCESS elicoptero1( double x, double y, int graph, int file, int tipo );
declare PROCESS elicoptero2( double x, double y, int graph, int file, int tipo );
declare PROCESS explosion( double x, double y, double size, int tipo );
declare PROCESS explosion3( double x, double y, double size, int tipo );
declare PROCESS explosion2( double x, double y );
declare PROCESS humo( double x, double y );
declare PROCESS cacho_heli( double x, double y, int graph, int file, int tipo );
declare PROCESS piedras( double x, double y, int graph, double size, int tipo ); // muy importante
declare PROCESS bombazos( double x, double y );
declare PROCESS luzjose();
declare PROCESS brillos( int graph, int tipo );
declare PROCESS secundario( double x, double y, int graph, int file, int tipo );
declare PROCESS personajes_animados( int graph, int file, int tipo );
declare PROCESS pinchos( double x, double y, int graph, int file );
declare PROCESS bala_enemigo( double x, double y, int graph, int file, int tipo, int sentido );
declare PROCESS primer_plano( double y, int graph, int file, double xrelativa );
declare PROCESS polvo( double x, double y, int graph, int tipo );
declare PROCESS trozo_suelo( double x, double y, int graph, int tipo );
declare PROCESS nube( double x, double y, int graph, int file, double size, int tipo );
declare PROCESS cachocasa( double x, double y, int graph, int file );
declare PROCESS hierba( double x, double y, int graph, int file, int tipo );
declare PROCESS palla( double x, double y, int graph, int file );
declare PROCESS piedra( double x, double y, int graph, int file, int flags );
declare PROCESS viento( double x, double y, int graph, int file, int tipo );
declare PROCESS elemento_destructible( double x, double y, int graph, int file, int tipo );
declare PROCESS carro( double x, double y, int graph, int file );
declare PROCESS copa_palmera( double x, double y, int graph, int file );
declare PROCESS presentacion();
declare PROCESS bordeizqarriba();
declare PROCESS bordederarriba();
declare PROCESS bordeizqabajo();
declare PROCESS bordederabajo();
declare PROCESS presionaenter();
declare PROCESS mirahora();
declare PROCESS creadorjuego();
declare PROCESS Joselpresentacion1();
declare PROCESS Microfono1();
declare PROCESS brazo1();
declare PROCESS exploss( double x, double y );
declare PROCESS pegotesangre( double x, double y, int graph, double size );
declare PROCESS objetojosel1( double x, double y, int graph, int destino );
declare PROCESS sangre_cara( double x, double y, int graph, double size, int tipo );
declare PROCESS Joselpresentacion();
declare PROCESS cachocalne( double x, double y, int graph, int destino );
declare PROCESS muevefondo();
declare PROCESS trasladaopciones();
declare PROCESS iconopciones();
declare PROCESS trasladahistoria();
declare PROCESS iconohistoria();
declare PROCESS titjuego();
declare PROCESS josecolmenu();
declare PROCESS tortabrillo();
declare PROCESS babajosel();
declare PROCESS saldeljuego();
declare PROCESS menuopciones();
declare PROCESS cuadrotrucos();
declare PROCESS menutrucos();
declare PROCESS titrucos();
declare PROCESS icoinvencible();
declare PROCESS sinvencible();
declare PROCESS noinvencible();
declare PROCESS icorecarga();
declare PROCESS sirecarga();
declare PROCESS norecarga();
declare PROCESS icoreaccion();
declare PROCESS ruedareaccion();
declare PROCESS icovida();
declare PROCESS ruedavida();
declare PROCESS icosaltrucos();
declare PROCESS salcuadropciones();
declare PROCESS menuopciones1();
declare PROCESS cuadropciones();
declare PROCESS icopciones();
declare PROCESS icodificultad();
declare PROCESS icomax();
declare PROCESS iconOR ();
declare PROCESS icomin();
declare PROCESS icosangre();
declare PROCESS icosangremax();
declare PROCESS icosangrenOR ();
declare PROCESS icosangremin();
declare PROCESS icovolumen();
declare PROCESS Barravolumen();
declare PROCESS ruedavolumen();
declare PROCESS icovolumenfx();
declare PROCESS Barravolumenfx();
declare PROCESS ruedavolumenfx();
declare PROCESS icosalopciones();
GLOBAL
    int dureza = 2;
    int modo_wiz;
    int msn_enemigos;
    int total_robots;
    int reiniciando;
    // De intros ..
    int aleatorio;
    int intro1acaba;
    int intro3acaba;
    int dejacargar;
    // sonidos
    int cambio_musica;
    int poing;
    int logo;
    int descarg_logo;
    int decide;
    int aceptar; // sonido para aceptar algo
    int canal1;
    int sound2; // musica del menu
    int s_fase1; // musica de la fase 1
    int bienvenio; // bienvenido a mi juego
    int quieto; // voz quieto
    int golpe1; // en to la cara
    int golpe2; // que golpeee
    int golpe3; // El ojo el ojo
    int golpe4; // en toa la mano
    int golpe5; // en tol brazo
    int golpe6; // tas pasao con tu fuelza
    int golpe7; // que dolor que dolor
    int golpe8; // la rodilla
    int golpe9; // aaaaaahhh
    int golpe10; // ayyy
    int golpe11; // en tol ombrol
    int golpe12; // mas dao aiii
    int golpe13; // sonido ostia
    int golpe14; // gun
    int golpe15; // daldalboom
    int golpe16; // cortacuerpos
    int ok; // ok
    int complete; // mision complete
    int torta; // vaya torta
    int au; // auuu
    int combo; // wuaaaaaaaaaaa
    int kalauka; // kalaukaaaaa
    int cunao; // cunaaaaooooooo
    int pantoja; // con la panTOja
    int triunfo; // igororopera cion triunfo
    int lesiden; // lesident evill
    int crugida; // de huesos
    int erupto; // erupto .. XD
    int desper; // el jose se levanta
    int canta; // el jose canta
    int dish; // golpe al jose
    int explo1; // sonido explosion
    int pistola; // sonido de una pistola
    int pistola1; // sonido de otra pistola
    int camion; // sonido de un camion
    int scohete; // sonido del vuelo de un cohete
    int elicop; // sonido elicop
    int caida; // sonido de una caida al suelo
    int smetal1; // cuando una bala da sobre un metal
    int explo_3; // sonido explosion creada por un enemigo
    int sangre; // sonido de un impacto de bala
    int lanza; // sonido de algo lanzado
    int metal1; // sonido de la vagoneta al caer al suelo
    int gameove2; // sonido de la muerte del jose
    int s_palmera; // sonido caida palmera
    int madera; // al golpear la madera
    int recarga; // al cambiar de arma o recargar
    int nobalas; // sonido cuando no hay municion
    int lanzalla; // sonido del lanzallamas
    int uzi; // sonido de la metralleta
    int laser6; // sonido del disparo laser
    int escop1; // sonido de la escopeta
    int bazoka; // sonido del lanzamisiles
    int casquete; // al caer los cartuchos hacen ruido
    int lluvia; // sonido de la lluvia
    int trueno; // babooooooom
    int splash; // si algo cae al agua, splash !!!
    int explo3; // super explosion
    int cristal2; // peacho sonido de petar cristales
    int kema; // deja mas negros a los josels
    int argh; // sonido diferente al aaaaargh
    // De presentacion ..
    int a_memoria; // para mesurar y optimizar la memoria
    int entrando;
    int marcaopcion = 1;
    int marcaopcion2 = 1;
    int seleccionaopcion;
    int seleccionaopcion2;
    int josexplota; // cuando explota el chinorris
    int objetoencara; // cuando le pegan los frutos
    // Calculos para sacar la hora actual
    int ano;
    int mes;
    int dia;
    int hora;
    int min;
    int seg;
    int mihandle;
    int hora_juego; // tiempo que juegas al juego
    int min_juego;
    int seg_juego;
    // Del menu
    int alterna_josel;
    int guarda_x1 = 501;
    int guarda_x2 = 442;
    int volfx = 96; // Volumen Fx
    int vol = 128; // Volumen musica
    int opseleccionada = 1;
    int opseleccionada1 = 1;
    int fueramenu;
    int cantidadsangre = 2; // Cantidad de sangre del juego
    int cantidadificultad = 3; // Dificultad
    int selectdif = 1;
    int selectsang = 1;
    int bloqueo = 1; // Bloqueo del modo historia
    // Del los controles
    int pulsa_izq;
    int pulsa_der;
    int pulsa_arriba;
    int pulsa_abajo;
    int pulsa_enter;
    int pulsa_x;
    int pulsa_c;
    int pulsa_v;
    int pulsa_esc;
    int boton_1 = 2; // X
    int boton_2 = 0; // C
    int boton_3 = 1; // X doble
    int boton_4 = 6; // esc
    int boton_5 = 7; // start
    int boton_6 = 3; // V
    STRING extrae;
    int guarda;
    int control_arriba1 = _up;
    int control_abajo1 = _down;
    int control_izq1 = _left;
    int control_der1 = _right;
    int control_recargar1 = _x;
    int control_disparar1 = _c;
    int control_cambarma1 = _v;
    int control_pausa1 = _enter;
    int tipo_control1 = 1; // Control 1 jugador teclado / mando / raton
    int tipo_control2 = 1; // Control 2 jugador teclado / mando / raton
    int controlselect = 1;
    int quecontrol = 1;
    int configurando1;
    // De la tienda
    int compramenu = 2; // comprar opcion
    // del puto programa ...
    int funcional;
    // programacion de enemigos en las fases
    int total_enemigos;
    int elicopteros;
    int enemigos_tierra;
    int tankes;
    int camiones;
    int granaderos;
    int existe_bala; // los enemigos saben si hay disparos o explosiones
    int existe_explosion;
    int clones_eliminados; // cuenta todos los enemigos que mueren en el juego
    // Todo toca superficies
    int ke_fase_es; // para el fichero cargado
    int ke_grafico_es; // para el grafico de durezas

    int ke_color_es = 0x8cfcf8ff; // 110 			para el tipo de color
    int color_pared_con_pinchos = 0x304830ff; // 195
    int color_pared_1 = 0x74a474ff; // 200
    int color_pared_2 = 0x84ac84ff; // 201

    int color_via_0 = 0xe07000ff; // 168
	int color_via_1 = 0x8cfcf8ff; // 110
	int color_via_2 = 0xfcdca8ff; // 30
	int color_via_3 = 0xd8fcb0ff; // 46
	int color_via_4 = 0xfcb8fcff; // 94
	int color_via_5 = 0xfcfc8cff; // 238
	int color_via_6 = 0xd4e0d4ff; // 206
	int color_via_7 = 0xfce8a8ff; // 174
	int color_via_8 = 0xd80000ff; // 21

	int color_arriba_objeto_no_piso = 0xfc2800ff; // 230
	int color_techo = 0x649064ff; // 199

    // Ficheros de las fases
    int camara_x;
    int rondas_fase;
    int bloquea;
    int juego_fpg;
    int velocidad_juego = 36; // velocidad del juego
    int fase_entreno;
    int fase1;
    int fase2;
    int fase3;
    int fase4;
    int fase5;
    int fase6;
    int fase7;
    int fase8;
    int fase9;
    int fase10;
    int haz_reflejos; // si hay agua , fuego o hielo habran reflejos
    int grafico_origen1; // permite destrozar el escenario
    int grafico_origen2;
    int vagoneta_x; // coordenadas de la vagoneta
    int scrolls; // optimizacion para ordenadores con 32 MB de RAM
    int limite_pantalla; // marca el limite permitido por la fase
    int musica_fase; // pone la musica de la fase
    int estado_fase; // muestra el estado actual de la fase
    int tiempo_actual = 1; // Utiliza el motor " Real time " para cambiar el tiempo en el juego
    STRING nombre_pantalla; // el nombre de la pantalla
    int mapa_pantalla; // mapa de la pantalla
    STRING objetivos_a; // estado de los objetivos
    STRING objetivos_b;
    STRING objetivos_c;
    STRING objetivos_d;
    // Datos del josel
    int vida_total = 8;
    int nivel_player1 = 1;
    int salto_player1 = 19;
    int velocidad_player1 = 5;
    int agilidad_player1 = 1;
    int punteria_player1 = 50;
    // Esto controla el motor del juego
    int copos_nieve; // cantidad maxima de copos de nieve k puede haber
    int imita_graf; // efecto gr fico que muestra en el agua al personaje
    int imita_flags;
    int imita_angle;
    int ke_nivel; // selecciona nivel a ejecutar
    int realiza_intro1;
    int limite_vida_p1 = 17; // Cantidad de vida del jugador 1
    int josel_tocao; // un enemigo golpea al josel
    int limite_vida_player1 = 25; // vida en modo historia del p1 y p2
    int juego_en_pausa; // comprueba si el juego esta en pausa
    int tengo;
    int llaves; // principal para abrir puertas
    int boss; // el mostruo de final de fase
    int gasolinas; // sin ella no se puede utilizar el avion
    int regalos;
    int puntuacion; // puntua los puntos
    int vida_robot; // si muere el robot, destruye la barrera que proteje
    int guarda_llaves;
    int guarda_gasolinas;
    int guarda_regalos;
    // numero de objetos
    STRING obj_nombre;
    STRING obj_linea1;
    STRING obj_linea2;
    STRING obj_linea3;
    int objetos_llave;
    int objetos_combustible;
    int objetos_muelle;
    int objetos_moneda;
    int objetos_chip;
    int objetos_teletransportador;
    int objetos_vasoagua;
    int objetos_reductor;
    int objetos_ampliador;
    int objetos_joselillo;
    int objetos_navecilla;
    int objetos_melon;
    int objetos_pirana;
    int objetos_joselxdye;
    int objetos_ovni;
    int objetos_regalo;
    int objetivos_a1;
    // para utilizar el arma he de comprarla antes
    int guarda_arma2; //si es 1, esta comprada
    int guarda_arma3;
    int guarda_arma4;
    int guarda_arma5;
    int guarda_arma6;
    int guarda_arma7;
    int guarda_arma8;
    int guarda_arma9;
    int guarda_arma10;
    // restaura todo lo que tenia antes si me matan  ****
    int guarda_objetos_llave;
    int guarda_objetos_combustible;
    int guarda_objetos_muelle;
    int guarda_nivel1;
    int guarda_puntuacion1;
    int guarda_salto1;
    int guarda_velocidad1;
    int guarda_agilidad1;
    int guarda_disparos1;
    int guarda_disparostotal1;
    int disparos_acertados = 1;
    int total_disparos = 1;
    int guarda_subenivel;
    int sube_nivel = 25000; // hara subir de nivel
    int guarda_clones;
    int guarda_vidatotal;
    // en modo historia se guarda la municion de cada arma
    int municion_arma2 = -1;
    int municion_arma3 = -1;
    int municion_arma4 = -1;
    int municion_arma5 = -1;
    int municion_arma6 = -1;
    int municion_arma7 = -1;
    int municion_arma8 = -1;
    int municion_arma9 = -1;
    int municion_arma10 = -1;
    // maximo de balas para cada arma
    int limite_arma2 = 300;
    int limite_arma3 = 100;
    int limite_arma4 = 75;
    int limite_arma5 = 60;
    int limite_arma6 = 10;
    int limite_arma7 = 8;
    int limite_arma8 = 5;
    int limite_arma9 = 5;
    int limite_arma10 = 3;
    int fuente; // carga la fuente
    int escrito; // se borra el texto que muestra la municion si existe
    int arma_seleccionada_p1 = 1; // el arma por defecto
    int arma_seleccionada_p2 = 1;
    int pausa_en_pantalla; // Si existe pausa activa
    int marca_pausa;
    int juegan_dos = 0; // Si esta en 1, juegan dos jugadores
    double x_p1; // guarda coordenadas
    double y_p1;
    double x_p2;
    double y_p2;
    int boom; // temblores de la camara del juego
    int vida_general; // del segundo monstruo
    // De los trucos
    int selectinvencible = 1;
    int invencible = 2; // Activar invencibilidad
    int selectrecarga = 1;
    int autorecarga = 2; // Activar municion infinita
    int altura_salto = -19; // Altura maxima del salto
    int vidaenemigo = 0; // vida del enemigo
    int incre_m1 = 120;
    int incre_m2 = 120;
    int fuecargada; // carga la configuracion y partida nada mas ponerse
    int i;
    int valor;
BEGIN
    set_fps( 24, 0 );
    window_set_title( "Joselkiller: El Ataque de los clones - v1.5" );
    screen.fullscreen = 0;
    set_mode( 640, 480 );
    fpg_load( "fpg/juego.fpg" );
    window_set_icon( 0, 21 );
    sound.channels = 32;
    reserve_channels( 24 );
    channel_set_volume( -1, volfx );

    // carga fuentes
    fuente = fnt_load( "fnt/fuente.fnt" );

    // CONFIGURACION POR TXT
    if ( fexists( "controls.txt" ))
        guarda = fopen( "controls.txt", o_read );
        for ( i = 1; i < 9; i++ )
            extrae = fgets( guarda );
            extrae = fgets( guarda );
            extrae = fgets( guarda );
            valor = -1;
            if ( extrae == "up" )
                valor = _up;
            end
            if ( extrae == "down" )
                valor = _down;
            end
            if ( extrae == "left" )
                valor = _left;
            end
            if ( extrae == "right" )
                valor = _right;
            end
            if ( extrae == "enter" )
                valor = _enter;
            end
            if ( extrae == "1" )
                valor = _1;
            end
            if ( extrae == "2" )
                valor = _2;
            end
            if ( extrae == "3" )
                valor = _3;
            end
            if ( extrae == "4" )
                valor = _4;
            end
            if ( extrae == "5" )
                valor = _5;
            end
            if ( extrae == "6" )
                valor = _6;
            end
            if ( extrae == "7" )
                valor = _7;
            end
            if ( extrae == "8" )
                valor = _8;
            end
            if ( extrae == "9" )
                valor = _9;
            end
            if ( extrae == "q" )
                valor = _q;
            end
            if ( extrae == "w" )
                valor = _w;
            end
            if ( extrae == "e" )
                valor = _e;
            end
            if ( extrae == "r" )
                valor = _r;
            end
            if ( extrae == "t" )
                valor = _t;
            end
            if ( extrae == "y" )
                valor = _y;
            end
            if ( extrae == "u" )
                valor = _u;
            end
            if ( extrae == "i" )
                valor = _i;
            end
            if ( extrae == "o" )
                valor = _o;
            end
            if ( extrae == "p" )
                valor = _p;
            end
            if ( extrae == "a" )
                valor = _a;
            end
            if ( extrae == "s" )
                valor = _s;
            end
            if ( extrae == "d" )
                valor = _d;
            end
            if ( extrae == "f" )
                valor = _f;
            end
            if ( extrae == "g" )
                valor = _g;
            end
            if ( extrae == "h" )
                valor = _h;
            end
            if ( extrae == "j" )
                valor = _j;
            end
            if ( extrae == "k" )
                valor = _k;
            end
            if ( extrae == "l" )
                valor = _l;
            end
            if ( extrae == "z" )
                valor = _z;
            end
            if ( extrae == "x" )
                valor = _x;
            end
            if ( extrae == "c" )
                valor = _c;
            end
            if ( extrae == "v" )
                valor = _v;
            end
            if ( extrae == "b" )
                valor = _b;
            end
            if ( extrae == "n" )
                valor = _n;
            end
            if ( extrae == "m" )
                valor = _m;
            end
            if ( valor != -1 )
                if ( i == 1 )
                    control_arriba1 = valor;
                end
                if ( i == 2 )
                    control_abajo1 = valor;
                end
                if ( i == 3 )
                    control_izq1 = valor;
                end
                if ( i == 4 )
                    control_der1 = valor;
                end
                if ( i == 5 )
                    control_pausa1 = valor;
                end
                if ( i == 6 )
                    control_recargar1 = valor;
                end
                if ( i == 7 )
                    control_disparar1 = valor;
                end
                if ( i == 8 )
                    control_cambarma1 = valor;
                end
            end
        end
        fclose( guarda );
    end

    write( fuente, 320, 240, ALIGN_CENTER, "Cargando... Por favor espere..." );

    frame;
    // Carga sonidos
    poing = sound_load( "sound/poing.wav" );
    logo = music_load( "sound/logo.mp3" );
    decide = sound_load( "sound/decide.wav" );
    bienvenio = sound_load( "sound/menu1.wav" );
    splash = sound_load( "sound/splash.wav" );
    cristal2 = sound_load( "sound/cristal2.wav" );
    sound2 = music_load( "sound/sound2.mp3" );
    // quieto=sound_load("sound/quieto.wav");
    // golpe1=sound_load("sound/golpe1.wav"); // GUA EN TODA LA CARA (AÑADIR)
    golpe2 = sound_load( "sound/golpe2.wav" );
    golpe3 = sound_load( "sound/golpe3.wav" );
    golpe4 = sound_load( "sound/golpe4.wav" );
    golpe5 = sound_load( "sound/golpe5.wav" );
    golpe6 = sound_load( "sound/golpe6.wav" );
    // golpe7=sound_load("sound/golpe7.wav");
    golpe8 = sound_load( "sound/golpe8.wav" );
    golpe9 = sound_load( "sound/golpe9.wav" );
    golpe10 = sound_load( "sound/golpe10.wav" );
    golpe11 = sound_load( "sound/golpe11.wav" );
    golpe12 = sound_load( "sound/golpe12.wav" );
    golpe13 = sound_load( "sound/golpe13.wav" );
    golpe14 = sound_load( "sound/golpe14.wav" );
    golpe15 = sound_load( "sound/golpe15.wav" );
    golpe16 = sound_load( "sound/cortacu.wav" );
    // ok=sound_load("sound/ok.wav");
    complete = music_load( "sound/complete.mp3" );
    torta = sound_load( "sound/torta.wav" );
    // au=sound_load("sound/au.wav");
    combo = sound_load( "sound/combo.wav" );
    kalauka = sound_load( "sound/carl.wav" );
    cunao = sound_load( "sound/cunao.wav" );
    pantoja = sound_load( "sound/pantoja.wav" );
    triunfo = sound_load( "sound/triunfo.wav" );
    lesiden = sound_load( "sound/lesiden.wav" );
    crugida = sound_load( "sound/crugida.wav" );
    canta = sound_load( "sound/canta.wav" );
    erupto = sound_load( "sound/erupto.wav" );
    dish = sound_load( "sound/dish2.wav" );
    scohete = sound_load( "sound/cohete.wav" );
    explo1 = sound_load( "sound/explo1.wav" );
    explo3 = sound_load( "sound/explo3.wav" );
    pistola = sound_load( "sound/pistola.wav" );
    pistola1 = sound_load( "sound/astra.wav" );
    aceptar = sound_load( "sound/aceptar.wav" ); // carga sonidos
    camion = sound_load( "sound/camion.wav" );
    elicop = sound_load( "sound/elicop.wav" );
    caida = sound_load( "sound/caida.wav" );
    smetal1 = sound_load( "sound/smetal1.wav" );
    sangre = sound_load( "sound/sangre.wav" );
    lanza = sound_load( "sound/lanza.wav" );
    argh = sound_load( "sound/argh.wav" );
    kema = sound_load( "sound/kema.wav" );
    metal1 = sound_load( "sound/metal1.wav" );
    gameove2 = music_load( "sound/gameove2.mp3" );
    s_palmera = sound_load( "sound/palmera.wav" );
    madera = sound_load( "sound/madera.wav" );
    recarga = sound_load( "sound/recarga.wav" );
    nobalas = sound_load( "sound/nobalas.wav" );
    lanzalla = sound_load( "sound/lanzalla.wav" );
    uzi = sound_load( "sound/uzi.wav" );
    laser6 = sound_load( "sound/laser6.wav" );
    escop1 = sound_load( "sound/escop1.wav" );
    bazoka = sound_load( "sound/bazoka.wav" );
    casquete = sound_load( "sound/casquete.wav" );
    lluvia = sound_load( "sound/lluvia.wav" );
    trueno = sound_load( "sound/trueno.wav" );

    write_delete( all_text );

    presentacion();
    frame;
end


PROCESS controles();
PRIVATE
    int POV1;
    int POVx1;
    int POVy1;
    int num_pad;
    int tipo;
    int POVx;
    int POVc;
    int POVv;
    int POVesc;
    int POVstart;
BEGIN
    priority = -200;
    if ( os_id == OS_GP2X_WIZ )
        boton_1 = 12; // X
        boton_2 = 14; // C
        boton_3 = 13; // X doble
        boton_4 = 8; // esc
        boton_5 = 9; // start
        boton_6 = 15; // V
        tipo = 1;
    end
    LOOP
        // cierra al pulsar cruz en la ventana
        if ( wm_info.exit_status == 1 )
            exit();
        end
        // cambia modo ventana o pantalla completa
        if ( key( _enter ) && key( _alt ))
            if ( screen.fullscreen == true )
                screen.fullscreen = false;
            else
                screen.fullscreen = true;
            end
            set_mode( 640, 480 );
        end
        POVy1 = 0;
        POVx1 = 0;
        POVx = 0;
        POVc = 0;
        POVv = 0;
        POVesc = 0;
        POVstart = 0;

        // TECLADO
        if ( key( control_arriba1 ))
            POVy1 = 1;
        end // arriba
        if ( key( control_abajo1 ))
            POVy1 = 2;
        end // abajo
        if ( key( control_izq1 ))
            POVx1 = 1;
        end // izq
        if ( key( control_der1 ))
            POVx1 = 2;
        end // der
        // TRANSFORMA
        if ( key( _esc ))
            POVesc = 1;
        end // esc
        if ( key( control_pausa1 ))
            POVstart = 1;
        end // enter
        if ( key( control_recargar1 ))
            POVx = 1;
        end // x
        if ( key( control_disparar1 ))
            POVc = 1;
        END // c
        if ( key( control_cambarma1 ))
            POVv = 1;
        END // v
        // anula enter si se pulsa alt+enter
        if ( key( _alt ))
            pulsa_enter = 0;
        end

        int joy = joy_query( JOY_QUERY_FIRST_ATTACHED );

        if ( joy != -1 )
            if ( joy_query( joy, JOY_BUTTON_DPAD_UP        ) || joy_query( joy, JOY_AXIS_LEFTY ) < -16384 )
            	POVy1 = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_DPAD_LEFT      ) || joy_query( joy, JOY_AXIS_LEFTX ) < -16384 )
            	POVx1 = 1;
           	end
            if ( joy_query( joy, JOY_BUTTON_DPAD_DOWN      ) || joy_query( joy, JOY_AXIS_LEFTY ) > 16383)
            	POVy1 = 2;
            end
            if ( joy_query( joy, JOY_BUTTON_DPAD_RIGHT     ) || joy_query( joy, JOY_AXIS_LEFTX ) > 16383 )
            	POVx1 = 2;
           	end
            if ( joy_query( joy, JOY_BUTTON_BACK           ) )
            	POVesc = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_START          ) )
            	POVstart = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_A              ) )
            	POVc = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_B              ) )
            	POVx = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_X              ) )
            	POVv = 1;
            end
            if ( joy_query( joy, JOY_BUTTON_Y              ) )
            	POVx = 1;
            end
        end

        // TRANSFORMA
        if ( POVx1 == 1 )
            if ( pulsa_izq == 0 )
                pulsa_izq = 1;
            else
                pulsa_izq = 2;
            end
        else
            pulsa_izq = 0;
        end // izquierda
        if ( POVx1 == 2 )
            if ( pulsa_der == 0 )
                pulsa_der = 1;
            else
                pulsa_der = 2;
            end
        else
            pulsa_der = 0;
        END // derecha
        if ( POVy1 == 1 )
            if ( pulsa_arriba == 0 )
                pulsa_arriba = 1;
            else
                pulsa_arriba = 2;
            end
        else
            pulsa_arriba = 0;
        END // arriba
        if ( POVy1 == 2 )
            if ( pulsa_abajo == 0 )
                pulsa_abajo = 1;
            else
                pulsa_abajo = 2;
            end
        else
            pulsa_abajo = 0;
        END // abajo
        if ( POVx == 1 )
            if ( pulsa_x == 0 )
                pulsa_x = 1;
            else
                pulsa_x = 2;
            end
        else
            pulsa_x = 0;
        end // x
        if ( POVc == 1 )
            if ( pulsa_c == 0 )
                pulsa_c = 1;
            else
                pulsa_c = 2;
            end
        else
            pulsa_c = 0;
        end // c
        if ( POVv == 1 )
            if ( pulsa_v == 0 )
                pulsa_v = 1;
            else
                pulsa_v = 2;
            end
        else
            pulsa_v = 0;
        end // v
        if ( POVstart == 1 )
            if ( pulsa_enter == 0 )
                pulsa_enter = 1;
            else
                pulsa_enter = 2;
            end
        else
            pulsa_enter = 0;
        end // start
        if ( POVesc == 1 )
            if ( pulsa_esc == 0 )
                pulsa_esc = 1;
            else
                pulsa_esc = 2;
            end
        else
            pulsa_esc = 0;
        end // esc
        frame;
    end
end

// CONTROLADOR PRINCIPAL, controla vidas, municion , personaje, hora

PROCESS controlador_principal();
PRIVATE
    int esperaterl;
    int estado_ilum;
    int msn_objetos;
    int msn_otros;
    int msn_monstruo;
    int tarda2 = 50;
    int tarda1 = 50;
    int tarda = 50;
BEGIN
    controles();
    if ( modo_wiz == 1 )
        haz_reflejos = 0;
    end
    // *** Evita errores ***
    regalos = 0;
    llaves = 0;
    gasolinas = 0;
    boss = 0;
    copos_nieve = 0;
    scroll.y0 = 0;
    scroll.y1 = 0;
    boom = 0;
    elicopteros = 0;
    enemigos_tierra = 0;
    existe_explosion = 0;
    rondas_fase = 0;
    bloquea = 0;
    msn_enemigos = 0;
    total_robots = 2;
    objetivos_c = "incompleto";
    objetivos_d = "-";
    guarda_llaves = objetos_llave;
    guarda_gasolinas = objetos_combustible;
    guarda_regalos = objetos_regalo;
    // guarda el numero de municion actual para luego restaurarlo
    guarda_arma2 = municion_arma2;
    guarda_arma3 = municion_arma3;
    guarda_arma4 = municion_arma4;
    guarda_arma5 = municion_arma5;
    guarda_arma6 = municion_arma6;
    guarda_arma7 = municion_arma7;
    guarda_arma8 = municion_arma8;
    guarda_arma9 = municion_arma9;
    guarda_arma10 = municion_arma10;
    // tambien perdera todos los objetos conseguidos , etc
    guarda_nivel1 = nivel_player1;
    guarda_agilidad1 = agilidad_player1;
    guarda_puntuacion1 = puntuacion;
    guarda_disparos1 = disparos_acertados;
    guarda_disparostotal1 = total_disparos;
    guarda_subenivel = sube_nivel;
    guarda_clones = clones_eliminados;
    guarda_vidatotal = vida_total;
    // Actualiza la hora del reloj
    mirahora();
    // *** Papel del controlador de juego ***
    if ( juegan_dos == 0 )
        // informacion de la fase
        if ( estado_fase == 1 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 100, 401 );
            nombre_pantalla = "1.1 Entrada al pueblo";
            mapa_pantalla = 417;
        end
        if ( estado_fase == 2 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 100, 430 );
            nombre_pantalla = "1.2 Pueblo Badulake";
            mapa_pantalla = 418;
        end
        if ( estado_fase == 3 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 85, 398 );
            nombre_pantalla = "1.3 Mina Skisme";
            mapa_pantalla = 419;
        end
        if ( estado_fase == 4 )
            vagoneta( 450, 221, 134, fase1 );
            nombre_pantalla = "1.4 Salida de la mina";
            mapa_pantalla = 420;
        end
        if ( estado_fase == 5 )
            vagoneta( 70, 369, 134, fase1 );
            nombre_pantalla = "1.5 Boss Ajabdala";
            mapa_pantalla = 421;
        end
        if ( estado_fase == 6 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 91, 328 );
            nombre_pantalla = "2.1 La jungla Ambparenn";
            mapa_pantalla = 422;
        end
        if ( estado_fase == 7 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 69, 383 );
            nombre_pantalla = "2.2 Infiltración en la base";
            mapa_pantalla = 576;
        end
        if ( estado_fase == 8 )
            jose_player1( 320, 0 );
            descenso_camara();
            nombre_pantalla = "2.3 Ascensor de la base";
            mapa_pantalla = 651;
        end
        if ( estado_fase == 9 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 60, 120 );
            nombre_pantalla = "2.4 Base abandonada";
            mapa_pantalla = 601;
        end
        if ( estado_fase == 10 )
            camara_juego();
            signal( type jose_player1, s_kill );
            jose_player1( 48, 340 );
            mapa_pantalla = 652;
            nombre_pantalla = "2.5 Boss Kisinouchiken";
        end
        if ( estado_fase == 22 )
            nave( 150, 320 );
            camara_nave();
            nombre_pantalla = "3.1 Vuelo en avión";
            mapa_pantalla = 0;
        end
        if ( estado_fase == 23 )
            camara_juego();
            aterrizaje_nave();
            nombre_pantalla = "3.2 Los Pirineos";
            mapa_pantalla = 649;
        end
        if ( estado_fase == 24 )
            camara_vertical();
            signal( type jose_player1, s_kill );
            jose_player1( 60, 3361 );
            nombre_pantalla = "3.3 Montaña peligrosa";
            mapa_pantalla = 650;
        end
        if ( estado_fase == 25 )
            camara_horizontal();
            signal( type jose_player1, s_kill );
            jose_player1( 320, 342 );
            nombre_pantalla = "3.4 Enfrentamiento final";
            mapa_pantalla = 653;
        end
        if ( estado_fase == 26 )
            nombre_pantalla = "3.5 Huida de la nave";
            mapa_pantalla = 0;
        end
        vida_player1();
        municion_players( 1 );
        arma_players( 1 );
        x = 152;
        y = 30;
        graph = 220;
    end
    // Establecer el " Real Time " del juego
    if ( mes == 1 OR mes == 2 OR mes == 3 )
        if ( hora => 8 AND hora < 17 )
            tiempo_actual = 12;
        end
        if ( hora => 17 AND hora < 19 )
            tiempo_actual = 2;
        end
        if ( hora => 19 AND hora < 8 )
            tiempo_actual = 1;
        end
    end
    if ( mes == 4 )
        if ( hora => 8 AND hora < 18 )
            tiempo_actual = 12;
        end
        if ( hora => 18 AND hora < 20 )
            tiempo_actual = 2;
        end
        if ( hora => 20 AND hora < 8 )
            tiempo_actual = 1;
        end
    end
    if ( mes == 6 OR mes == 7 OR mes == 5 OR mes == 8 OR mes == 9 )
        if ( hora => 8 AND hora < 19 )
            tiempo_actual = 12;
        end
        if ( hora => 19 AND hora < 21 )
            tiempo_actual = 2;
        end
        if ( hora => 21 AND hora < 8 )
            tiempo_actual = 1;
        end
    end
    if ( mes == 10 )
        if ( hora => 7 AND hora < 19 )
            tiempo_actual = 12;
        end
        if ( hora => 19 AND hora < 20 )
            tiempo_actual = 2;
        end
        if ( hora => 20 AND hora < 7 )
            tiempo_actual = 1;
        end
    end
    if ( mes == 11 OR mes == 12 )
        if ( hora => 8 AND hora < 16 )
            tiempo_actual = 12;
        end
        if ( hora => 16 AND hora < 18 )
            tiempo_actual = 2;
        end
        if ( hora => 18 AND hora < 8 )
            tiempo_actual = 1;
        end
    end
    z = -51;
    // escribe puntuacion y experiencia
    write_var( fuente, 90, 22, 4, puntuacion );
    LOOP
        // permite abrir menu y pausar en fase avion y vagoneta
        if ( estado_fase == 4 OR estado_fase == 22 )
            if ( limite_vida_player1 > 0 )
                if ( esperaterl > 0 AND juego_en_pausa == 0 AND pausa_en_pantalla == 0 )
                    esperaterl--;
                end
                if ( pulsa_esc == 1 AND juego_en_pausa == 0 AND esperaterl == 0 )
                    juego_en_pausa = 1;
                    esperaterl = 10;
                    pausa_juego();
                end
                if ( pulsa_enter == 1 and modo_wiz == 0 and pausa_en_pantalla == 0 AND esperaterl == 0 )
                    esperaterl = 10;
                    pausa_en_pantalla = 1;
                    menu_del_juego();
                end
            end
        end
        if ( elicopteros < 0 )
            elicopteros = 0;
        end
        // controla el tiempo que juegas al juego
        if ( timer[ 6 ] >= 100 )
            timer[ 6 ] = 0;
            seg_juego++;
        end
        if ( seg_juego >= 60 )
            seg_juego = 0;
            min_juego++;
        end
        if ( min_juego >= 60 )
            min_juego = 0;
            hora_juego++;
        end
        // controla las subidas de nivel
        if ( puntuacion => sube_nivel )
            nivel_player1++;
            items_info( x_p1, y_p1, 435 );
            sube_nivel += 25000;
            sound_play( aceptar, 0, 16 );
            // aumenta habilidades al subir de nivel
            vida_total++;
            limite_vida_player1++;
            agilidad_player1++;
        end
        if ( enemigos_tierra => 1 )
            tarda = 50;
        end
        if ( msn_enemigos == 0 )
            if ( elicopteros =< 0 AND enemigos_tierra =< 0 )
                tarda--;
                if ( tarda =< 0 )
                    msn_engine( 343, 1 );
                    msn_enemigos = 1;
                    objetivos_a = "completo";
                    objetivos_a1 = 1;
                    if ( estado_fase == 1 )
                        items( 224, 293, 14 );
                    end
                    if ( estado_fase == 2 AND rondas_fase == 3 )
                        items( 2271, 301, 15 );
                    end
                    if ( estado_fase == 3 AND rondas_fase == 3 )
                        items( 3037, 257, 16 );
                    end
                    if ( estado_fase == 4 )
                        items( 6698, 115, 17 );
                    end
                    if ( estado_fase == 6 )
                        items( 2097, 253, 18 );
                    end
                    if ( estado_fase == 7 )
                        items( 3063, 318, 19 );
                    end
                    if ( estado_fase == 8 )
                        items( 100, 380, 20 );
                    end
                    if ( estado_fase == 9 )
                        items( 5320, 86, 21 );
                    end
                    if ( estado_fase == 23 )
                        items( 4327, 224, 22 );
                    end
                end
            end
        end
        if ( msn_objetos == 0 AND estado_fase =< 3 )
            if ( llaves == 0 )
                tarda1--;
                if ( tarda1 =< 0 )
                    msn_engine( 378, 4 );
                    msn_objetos = 1;
                    objetivos_b = "completo";
                end
            end
        end
        if ( msn_objetos == 0 AND estado_fase == 23 )
            if ( regalos == 0 )
                tarda1--;
                if ( tarda1 =< 0 )
                    msn_engine( 658, 4 );
                    msn_objetos = 1;
                    objetivos_b = "completo";
                end
            end
        end
        if ( msn_otros == 0 AND estado_fase == 8 )
            tarda1--;
            if ( tarda1 =< 0 )
                msn_engine( 632, 4 );
                msn_otros = 1;
            end
        end
        if ( msn_objetos == 0 AND ( estado_fase == 7 OR estado_fase == 9 ))
            if ( gasolinas == 0 )
                tarda1--;
                if ( tarda1 =< 0 )
                    msn_engine( 603, 4 );
                    msn_objetos = 1;
                    objetivos_b = "completo";
                end
            end
        end
        frame;
    end
end


PROCESS avion_destrozada( double x, y );
PRIVATE
    int incr_y;
BEGIN
    file = fase1;
    ctype = c_scroll;
    y = -100;
    graph = 13;
    LOOP
        angle -= 10000;
        y += incr_y;
        x += 5;
        if ( incr_y < 15 )
            incr_y++;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            cacho_heli( x, y, 259, fase1, 1 );
            cacho_heli( x, y, 260, fase1, 2 );
            cacho_heli( x, y, 261, fase1, 3 );
            cacho_heli( x, y, 262, fase1, 4 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS aterrizaje_nave();
PRIVATE
    int rebotes = 5;
    int esperate;
    int incr_y = -20;
BEGIN
    file = fase1;
    ctype = c_scroll;
    x = -200;
    x_p1 = x;
    y = 10;
    graph = 13;
    josel_chafao( 2369, 402 );
    josel_chafao( 2399, 402 );
    josel_chafao( 2349, 402 );
    josel_chafao( 2330, 402 );
    josel_chafao( 1682, 269 );
    josel_chafao( 1725, 264 );
    josel_chafao( 1710, 263 );
    josel_chafao( 1744, 257 );
    josel_chafao( 1222, 280 );
    josel_chafao( 1243, 274 );
    josel_chafao( 1210, 277 );
    josel_chafao( 1181, 275 );
    josel_chafao( 454, 396 );
    josel_chafao( 480, 403 );
    josel_chafao( 494, 402 );
    josel_chafao( 520, 398 );
    josel_chafao( 546, 398 );
    josel_chafao( 603, 202 );
    josel_chafao( 620, 205 );
    josel_chafao( 642, 203 );
    josel_chafao( 654, 201 );
    josel_chafao( 677, 203 );
    LOOP
        x += 10;
        angle -= 10000;
        x_p1 = x;
        y += incr_y;
        if ( incr_y < 15 )
            incr_y++;
        end
        if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND ( esperate =< 0 ))
            incr_y = -25;
            rebotes--;
            esperate = 9;
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
        end
        if ( esperate > 0 )
            esperate--;
        end
        if ( rebotes =< 0 )
            incr_y = -50;
        end
        if ( y < -100 AND rebotes =< 0 )
            signal( type jose_player1, s_kill );
            jose_player1( x, -50 );
            avion_destrozada( x, -100 );
            if ( cantidadificultad == 1 ) // nivel facil
                elicopteros = 10;
                enemigos_tierra = 30;
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                elicoptero1( 3499, 84, 20, fase1, 1 );
            end
            if ( cantidadificultad == 2 ) // nivel normal
                elicopteros = 12;
                enemigos_tierra = 35;
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                elicoptero1( 3499, 84, 20, fase1, 1 );
                elicoptero1( 3499, 84, 20, fase1, 1 );
            end
            if ( cantidadificultad == 3 ) // nivel dificil
                elicopteros = 14;
                enemigos_tierra = 40;
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                enemigo_dispara( 3261, 289, fase1, 1 );
                elicoptero1( 3499, 84, 20, fase1, 1 );
                elicoptero1( 3499, 84, 20, fase1, 1 );
            end
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS josel_chafao( double x, double y );
BEGIN
    file = fase1;
    ctype = c_scroll;
    graph = 14;
    LOOP
        if ( collision( type explosion ))
            puntuacion += 50;
            sound_play( dish, 0 );
            piedras( x, y, 568, rand( 70, 95 ), 1 );
            piedras( x, y, 569, rand( 70, 95 ), 2 );
            piedras( x, y, 570, rand( 70, 95 ), 3 );
            piedras( x, y, 571, rand( 70, 95 ), 4 );
            piedras( x, y, 572, rand( 70, 95 ), 5 );
            piedras( x, y, 568, rand( 70, 95 ), 6 );
            piedras( x, y, 568, rand( 70, 95 ), 7 );
            piedras( x, y, 569, rand( 70, 95 ), 8 );
            piedras( x, y, 570, rand( 70, 95 ), 9 );
            piedras( x, y, 571, rand( 70, 95 ), 10 );
            piedras( x, y, 572, rand( 70, 95 ), 11 );
            piedras( x, y, 568, rand( 70, 95 ), 12 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS msn_engine( int graph, int tipo );
PRIVATE
    int estado = 1;
    int esperate = 50;
    int escribe_text;
    int escribe_text1;
    int escribe_text2;
    int escribe_text3;
    STRING que_es;
    int escribe_text4;
BEGIN
    z = -80;
    size = 10;
    x = 105;
    if ( tiempo_actual == 1 )
        que_es = "NOCHE";
    end
    if ( tiempo_actual == 2 )
        que_es = "TARDE";
    end
    if ( tiempo_actual == 12 )
        que_es = "DIA";
    end
    if ( tipo == 1 )
        y = 155;
    end
    if ( tipo == 2 )
        y = 130;
        size = 50;
    end
    if ( tipo == 4 )
        y = 180;
    end
    if ( tipo == 3 )
        if ( modo_wiz == 1 )
            return;
        end
        y = 180;
        size = 30;
        escribe_text = write( 0, 73, 180, 4, "FECHA :    -   -" );
        escribe_text1 = write_var( 0, 84, 180, 4, dia );
        escribe_text2 = write_var( 0, 108, 180, 4, mes );
        escribe_text3 = write_var( 0, 132, 180, 4, ano );
        escribe_text4 = write( 0, 165, 180, 4, que_es );
    end
    LOOP
        if ( estado == 1 )
            if ( size < 100 )
                size += 5;
            else
                size = 100;
                if ( tipo == 1 OR tipo == 4 )
                    sound_play( aceptar, 0, 16 );
                end
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( esperate > 0 )
                esperate--;
            end
            if ( esperate == 10 )
                alpha = 128;
            end
            if ( esperate =< 0 )
                if ( tipo == 3 )
                    write_delete( escribe_text );
                    write_delete( escribe_text1 );
                    write_delete( escribe_text2 );
                    write_delete( escribe_text3 );
                    write_delete( escribe_text4 );
                end
                signal( id, s_kill );
            end
        end
        if ( esperate < 11 )
            if ( alpha > 15 )
                alpha -= 15;
            end
        end
        frame;
    end
end


PROCESS gameover();
PRIVATE
    int estado = 1;
BEGIN
    x = 320;
    y = 240;
    graph = 325;
    size = 10;
    z = -100;
    alpha = 128;
    marca_pausa = 2; // fade(95,95,95,1);
    sound_stop( all_sound );
    music_stop();
    congelar( s_freeze );
    juego_en_pausa = 1;
    set_fps( velocidad_juego, 4 );
    LOOP
        if ( marca_pausa < 2 )
            marca_pausa = 3;
        end
        if ( marca_pausa > 3 )
            marca_pausa = 2;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            marca_pausa--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            marca_pausa++;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 5;
            else
                size = 100;
                titover();
                salir_partida();
                reiniciar_partida();
                music_play( gameove2, 0 ); // aqui !!
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS titover();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 342;
    x = 320;
    y = 185;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( estado == 6 )
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 1;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 1;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS menu_del_juego();
PRIVATE
    int estado = 1;
    int esperate = 1;
BEGIN
    if ( vol > 30 )
        music_set_volume( 30 );
    end
    x = 320;
    y = 240;
    graph = 325;
    size = 0;
    z = -100;
    alpha = 128;
    marca_pausa = 1; // fade(95,95,95,1);
    congelar( s_freeze );
    juego_en_pausa = 1;
    set_fps( velocidad_juego, 4 );
    LOOP
        if ( esperate > 1 )
            esperate--;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 20;
            else
                size = 100;
                tabla_del_menu( 413 );
                sound_play( aceptar, 0, 16 ); // aqui !!
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS desplaza_menu( double x, double y, int graph, int flags );
PRIVATE // datos privados del proceso
    int estado2 = 2;
BEGIN
    z = -104;
    if ( graph == 609 OR graph == 610 OR graph == 611 )
        z = -103;
    end
    if ( graph == mapa_pantalla )
        z = -140;
    end
    LOOP
        if ( NOT( graph == mapa_pantalla OR graph == 424 OR graph == 13 OR graph == 609 OR graph == 610 OR graph == 611 ))
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 1;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 1;
                else
                    estado2 = 2;
                end
            end
        end
        frame;
    end
end


PROCESS tabla_del_menu( int graph );
PRIVATE
    int estado;
    int estado2;
    int escribe_info;
    int guarda_z;
    int esperate;
BEGIN
    x = 320;
    y = 240;
    size = 5;
    guarda_z = text.z;
    estado = 1;
    z = -101;
    text.z = -102;
    if ( total_disparos > 0 )
        punteria_player1 = disparos_acertados * 100 / total_disparos * 100 / 100;
    else
        punteria_player1 = 100;
    end
    if ( punteria_player1 > 100 )
        punteria_player1 = 100;
    end
    if ( graph == 414 )
        desplaza_menu( 320, 233, mapa_pantalla, 0 );
        if ( tiempo_actual == 1 )
            desplaza_menu( 320, 218, 611, 0 );
        end
        if ( tiempo_actual == 2 )
            desplaza_menu( 320, 218, 610, 0 );
        end
        if ( tiempo_actual == 12 )
            desplaza_menu( 320, 218, 609, 0 );
        end
    end
    if ( graph == 416 )
        if ( municion_arma2 < 0 )
            desplaza_menu( 264, 196, 424, 0 );
        end
        if ( municion_arma3 < 0 )
            desplaza_menu( 264, 228, 424, 0 );
        end
        if ( municion_arma4 < 0 )
            desplaza_menu( 264, 260, 424, 0 );
        end
        if ( municion_arma5 < 0 )
            desplaza_menu( 264, 292, 424, 0 );
        end
        if ( municion_arma6 < 0 )
            desplaza_menu( 376, 165, 424, 0 );
        end
        if ( municion_arma7 < 0 )
            desplaza_menu( 376, 196, 424, 0 );
        end
        if ( municion_arma8 < 0 )
            desplaza_menu( 376, 228, 424, 0 );
        end
        if ( municion_arma9 < 0 )
            desplaza_menu( 376, 260, 424, 0 );
        end
        if ( municion_arma10 < 0 )
            desplaza_menu( 376, 292, 424, 0 );
        end
        if ( municion_arma2 == 0 )
            desplaza_menu( 264, 196, 424, 4 );
        end
        if ( municion_arma3 == 0 )
            desplaza_menu( 264, 228, 424, 4 );
        end
        if ( municion_arma4 == 0 )
            desplaza_menu( 264, 260, 424, 4 );
        end
        if ( municion_arma5 == 0 )
            desplaza_menu( 264, 292, 424, 4 );
        end
        if ( municion_arma6 == 0 )
            desplaza_menu( 376, 165, 424, 4 );
        end
        if ( municion_arma7 == 0 )
            desplaza_menu( 376, 196, 424, 4 );
        end
        if ( municion_arma8 == 0 )
            desplaza_menu( 376, 228, 424, 4 );
        end
        if ( municion_arma9 == 0 )
            desplaza_menu( 376, 260, 424, 4 );
        end
        if ( municion_arma10 == 0 )
            desplaza_menu( 376, 292, 424, 4 );
        end
    end
    if ( graph == 415 )
        selecciona_objetos();
        if ( objetos_llave == 0 )
            desplaza_menu( 227, 171, 13, 0 );
        end
        if ( objetos_combustible == 0 )
            desplaza_menu( 254, 171, 13, 0 );
        end
        if ( objetos_muelle == 0 )
            desplaza_menu( 281, 171, 13, 0 );
        end
        if ( objetos_moneda == 0 )
            desplaza_menu( 308, 171, 13, 0 );
        end
        if ( objetos_chip == 0 )
            desplaza_menu( 335, 171, 13, 0 );
        end
        if ( objetos_teletransportador == 0 )
            desplaza_menu( 362, 171, 13, 0 );
        end
        if ( objetos_vasoagua == 0 )
            desplaza_menu( 389, 171, 13, 0 );
        end
        if ( objetos_reductor == 0 )
            desplaza_menu( 416, 171, 13, 0 );
        end
        if ( objetos_ampliador == 0 )
            desplaza_menu( 227, 207, 13, 0 );
        end
        if ( objetos_joselillo == 0 )
            desplaza_menu( 254, 207, 13, 0 );
        end
        if ( objetos_navecilla == 0 )
            desplaza_menu( 281, 207, 13, 0 );
        end
        if ( objetos_melon == 0 )
            desplaza_menu( 308, 207, 13, 0 );
        end
        if ( objetos_pirana == 0 )
            desplaza_menu( 335, 207, 13, 0 );
        end
        if ( objetos_joselxdye == 0 )
            desplaza_menu( 362, 207, 13, 0 );
        end
        if ( objetos_ovni == 0 )
            desplaza_menu( 389, 207, 13, 0 );
        end
        if ( objetos_regalo == 0 )
            desplaza_menu( 416, 207, 13, 0 );
        end
    end
    LOOP
        if ( esperate < 5 )
            esperate++;
        end
        if ( estado == 6 )
            if ( esperate == 5 )
                if ( graph == 413 ) // del menu estado puedo ir al menu mapa
                    if ( tipo_control1 == 1 AND pulsa_der == 1 AND pulsa_izq == 0 )
                        write_delete( all_text );
                        signal( type msn_engine, s_kill );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        tabla_del_menu( 414 );
                        signal( id, s_kill );
                    end
                end
                if ( graph == 414 ) // del menu estado puedo ir al menu mapa
                    if ( tipo_control1 == 1 AND pulsa_der == 1 AND pulsa_izq == 0 )
                        write_delete( all_text );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        tabla_del_menu( 415 );
                        signal( id, s_kill );
                    end
                    if ( tipo_control1 == 1 AND pulsa_izq == 1 AND pulsa_der == 0 )
                        write_delete( all_text );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        tabla_del_menu( 413 );
                        signal( id, s_kill );
                    end
                end
                if ( graph == 415 ) // del menu estado puedo ir al menu mapa
                    if ( tipo_control1 == 1 AND pulsa_der == 1 AND pulsa_izq == 0 )
                        write_delete( all_text );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        signal( type usar_objetos, s_kill );
                        signal( type selecciona_objetos, s_kill );
                        tabla_del_menu( 416 );
                        signal( id, s_kill );
                    end
                    if ( tipo_control1 == 1 AND pulsa_izq == 1 AND pulsa_der == 0 )
                        write_delete( all_text );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        signal( type usar_objetos, s_kill );
                        signal( type selecciona_objetos, s_kill );
                        tabla_del_menu( 414 );
                        signal( id, s_kill );
                    end
                end
                if ( graph == 416 )
                    if ( tipo_control1 == 1 AND pulsa_izq == 1 AND pulsa_der == 0 )
                        write_delete( all_text );
                        sound_play( decide, 0, 16 );
                        signal( type desplaza_menu, s_kill );
                        signal( type usar_objetos, s_kill );
                        signal( type selecciona_objetos, s_kill );
                        tabla_del_menu( 415 );
                        signal( id, s_kill );
                    end
                end
            end
            if ( escribe_info == 0 )
                write_var( fuente, 90, 22, 4, puntuacion );
                if ( arma_seleccionada_p1 > 1 )
                    write_var( fuente, 261, 33, 4, tengo );
                end
            end
            if ( graph == 413 AND escribe_info == 0 )
                write_var( 0, 352, 170, 4, limite_vida_player1 );
                write( 0, 372, 170, 4, "/" );
                write_var( 0, 392, 170, 4, vida_total );
                write_var( 0, 372, 181, 4, nivel_player1 );
                write_var( 0, 342, 192, 4, puntuacion );
                write( 0, 372, 192, 4, "/" );
                write_var( 0, 402, 192, 4, sube_nivel );
                write_var( 0, 372, 203, 4, salto_player1 );
                write_var( 0, 372, 214, 4, velocidad_player1 );
                write_var( 0, 372, 225, 4, agilidad_player1 );
                write_var( 0, 362, 236, 4, punteria_player1 );
                write( 0, 383, 235, 4, "%" );
                write_var( 0, 368, 291, 4, hora_juego );
                write( 0, 380, 291, 4, ":" );
                write_var( 0, 392, 291, 4, min_juego );
                write( 0, 404, 291, 4, ":" );
                write_var( 0, 416, 291, 4, seg_juego );
                write_var( 0, 407, 257, 4, total_disparos );
                write_var( 0, 407, 268, 4, clones_eliminados );
                escribe_info = 1;
            end
            if ( graph == 414 AND escribe_info == 0 )
                write( 0, 343, 156, 4, nombre_pantalla );
                write( 0, 273, 293, 4, objetivos_a );
                write( 0, 383, 293, 4, objetivos_b );
                write( 0, 273, 304, 4, objetivos_c );
                write( 0, 383, 304, 4, objetivos_d );
                escribe_info = 1;
            end
            if ( graph == 415 AND escribe_info == 0 )
                escribe_info = 1;
                write( 0, 339, 241, 4, obj_nombre );
                write( 0, 320, 257, 4, obj_linea1 );
                write( 0, 320, 268, 4, obj_linea2 );
                write( 0, 320, 279, 4, obj_linea3 );
                write_var( 0, 234, 181, 4, objetos_llave );
                write_var( 0, 261, 181, 4, objetos_combustible );
                write_var( 0, 423, 217, 4, objetos_regalo );
            end
            if ( graph == 416 AND escribe_info == 0 )
                escribe_info = 1;
                write_var( 0, 283, 190, 4, municion_arma2 );
                write_var( 0, 283, 221, 4, municion_arma3 );
                write_var( 0, 283, 254, 4, municion_arma4 );
                write_var( 0, 283, 286, 4, municion_arma5 );
                write_var( 0, 393, 159, 4, municion_arma6 );
                write_var( 0, 393, 190, 4, municion_arma7 );
                write_var( 0, 393, 221, 4, municion_arma8 );
                write_var( 0, 393, 254, 4, municion_arma9 );
                write_var( 0, 393, 286, 4, municion_arma10 );
            end
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                write_delete( all_text );
                signal( type msn_engine, s_kill );
                signal( type desplaza_menu, s_kill );
                signal( type usar_objetos, s_kill );
                signal( type selecciona_objetos, s_kill );
                text.z = guarda_z;
                write_var( fuente, 90, 22, 4, puntuacion );
                if ( arma_seleccionada_p1 > 1 )
                    write_var( fuente, 261, 33, 4, tengo );
                end
                continuando();
            end
        end
        if ( estado == 1 )
            size = 100;
            estado = 6;
            estado2 = 2;
            if ( graph == 413 )
                desplaza_menu( 415, 321, 423, 0 );
            end
            if ( graph == 414 OR graph == 415 )
                desplaza_menu( 222, 321, 423, 1 );
                desplaza_menu( 415, 321, 423, 0 );
            end
            if ( graph == 416 )
                desplaza_menu( 222, 321, 423, 1 );
            end
        end
        frame;
    end
end


PROCESS usar_objetos();
PRIVATE
    int estado2 = 2;
BEGIN
    x = 315;
    y = 301;
    size = 100;
    z = -160;
    graph = 8;
    LOOP
        if ( estado2 == 2 )
            if ( size < 130 )
                size += 1;
            else
                estado2 = 3;
            end
        end
        if ( estado2 == 3 )
            if ( size > 100 )
                size -= 1;
            else
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS selecciona_objetos();
PRIVATE
    int estado;
    int posicion = 1;
    int esperate = 10;
    int haz_graficos;
BEGIN
    x = 227;
    y = 171;
    size = 5;
    estado = 1;
    z = -170;
    graph = 9;
    LOOP
        haz_graficos++;
        if ( esperate > 0 )
            esperate--;
        end
        if ( haz_graficos == 1 )
            graph = 9;
        end
        if ( haz_graficos == 3 )
            graph = 10;
        end
        if ( haz_graficos == 5 )
            graph = 11;
        end
        if ( haz_graficos == 7 )
            graph = 12;
        end
        if ( haz_graficos == 9 )
            graph = 12;
        end
        if ( haz_graficos == 11 )
            graph = 11;
        end
        if ( haz_graficos == 13 )
            graph = 10;
        end
        if ( haz_graficos == 15 )
            graph = 9;
        end
        if ( haz_graficos => 16 )
            haz_graficos = 0;
        end
        if ( posicion <= 1 )
            posicion = 1;
        end
        if ( posicion >= 16 )
            posicion = 16;
        end
        if ( estado == 6 AND esperate == 0 )
            if ( tipo_control1 == 1 AND pulsa_abajo == 1 AND posicion < 16 )
                posicion++;
                esperate = 5;
                sound_play( decide, 0, 16 );
            end
            if ( tipo_control1 == 1 AND pulsa_arriba == 1 AND posicion > 1 )
                posicion--;
                esperate = 5;
                sound_play( decide, 0, 16 );
            end
        end
        if ( estado == 6 )
            obj_nombre = "vacío       ";
            obj_linea1 = "---";
            obj_linea2 = "---";
            obj_linea3 = "---";
            if ( posicion == 3 )
                x = 281;
                y = 171;
                if ( objetos_muelle > 0 )
                    obj_nombre = "Un muelle rojo";
                end
            end
            if ( posicion == 4 )
                x = 308;
                y = 171;
                if ( objetos_moneda > 0 )
                    obj_nombre = "Una moneda de oro";
                end
            end
            if ( posicion == 5 )
                x = 335;
                y = 171;
                if ( objetos_chip > 0 )
                    obj_nombre = "Un chip programable";
                end
            end
            if ( posicion == 6 )
                x = 362;
                y = 171;
                if ( objetos_teletransportador > 0 )
                    obj_nombre = "Mando teletransportador";
                end
            end
            if ( posicion == 7 )
                x = 389;
                y = 171;
                if ( objetos_vasoagua > 0 )
                    obj_nombre = "Un vaso de agua";
                end
            end
            if ( posicion == 8 )
                x = 416;
                y = 171;
                if ( objetos_reductor > 0 )
                    obj_nombre = "Aparato reductor";
                end
            end
            if ( posicion == 9 )
                x = 227;
                y = 207;
                if ( objetos_ampliador > 0 )
                    obj_nombre = "Aparato ampliador";
                end
            end
            if ( posicion == 10 )
                x = 254;
                y = 207;
                if ( objetos_joselillo > 0 )
                    obj_nombre = "Joselkiller";
                end
            end
            if ( posicion == 11 )
                x = 281;
                y = 207;
                if ( objetos_navecilla > 0 )
                    obj_nombre = "Un Avión";
                end
            end
            if ( posicion == 12 )
                x = 308;
                y = 207;
                if ( objetos_melon > 0 )
                    obj_nombre = "Un melón";
                end
            end
            if ( posicion == 13 )
                x = 335;
                y = 207;
                if ( objetos_pirana > 0 )
                    obj_nombre = "Una piraña";
                end
            end
            if ( posicion == 14 )
                x = 362;
                y = 207;
                if ( objetos_joselxdye > 0 )
                    obj_nombre = "Cabeza Josel";
                end
            end
            if ( posicion == 15 )
                x = 389;
                y = 207;
                if ( objetos_ovni > 0 )
                    obj_nombre = "Un ovni";
                end
            end
            if ( obj_nombre != "vacío       " )
                obj_linea1 = "Objetos ocultos de colección:";
                obj_linea2 = "Aparecen tras eliminar";
                obj_linea3 = "a todos los enemigos";
            end
            if ( posicion == 1 )
                x = 227;
                y = 171;
                if ( objetos_llave > 0 )
                    obj_nombre = " Llave aparentemente mora";
                    obj_linea1 = "Esta llave no es la de mi casa";
                    obj_linea2 = "y es roja, como la";
                    obj_linea3 = "sangre de los enemigos";
                end
            end
            if ( posicion == 2 )
                x = 254;
                y = 171;
                if ( objetos_combustible > 0 )
                    obj_nombre = "Combustible";
                    obj_linea1 = "Un recipiente con un líquido negro?";
                    obj_linea2 = "parece salsa de esa del chino";
                    obj_linea3 = "pero sabe a combustible de avión";
                end
            end
            if ( posicion == 16 )
                x = 416;
                y = 207;
                if ( objetos_regalo > 0 )
                    obj_nombre = "Un paquete de regalo";
                    obj_linea1 = "Aparentemente parece un paquete";
                    obj_linea2 = "de regalo, pero dentro lleva una";
                    obj_linea3 = "bomba nuclear de aliento josel";
                end
            end
        end
        if ( estado == 6 AND esperate == 4 )
            write_delete( all_text );
            write( 0, 339, 241, 4, obj_nombre );
            write( 0, 320, 257, 4, obj_linea1 );
            write( 0, 320, 268, 4, obj_linea2 );
            write( 0, 320, 279, 4, obj_linea3 );
            write_var( 0, 234, 181, 4, objetos_llave );
            write_var( 0, 261, 181, 4, objetos_combustible );
            write_var( 0, 423, 217, 4, objetos_regalo );
            write_var( fuente, 90, 22, 4, puntuacion );
            if ( arma_seleccionada_p1 > 1 )
                write_var( fuente, 261, 33, 4, tengo );
            end
        end
        if ( estado == 1 )
            size = 100;
            estado = 6;
        end
        frame;
    end
end


PROCESS congelar( int tipo );
BEGIN
    signal( type salta_barril, tipo );
    signal( type tanke, tipo );
    signal( type camara_juego1, tipo );
    signal( type cacho_vagoneta, tipo );
    signal( type onda_exp, tipo );
    signal( type secundario, tipo );
    signal( type onda_agua, tipo );
    signal( type torpedo3, tipo );
    signal( type camara_vertical, tipo );
    signal( type platillo, tipo );
    signal( type coche_enemigo, tipo );
    signal( type coche, tipo );
    signal( type bola_pinchos, tipo );
    signal( type imita_bala, tipo );
    signal( type imita_2, tipo );
    signal( type imita_josel, tipo );
    signal( type jose_player1, tipo );
    signal( type enemigo_arabe, tipo );
    signal( type elicoptero1, tipo );
    signal( type bala, tipo );
    signal( type bala_enemigo, tipo );
    signal( type explosion, tipo );
    signal( type carro, tipo );
    signal( type monstruo1, tipo );
    signal( type copo_nieve, tipo );
    signal( type crea_nieve, tipo );
    signal( type humo, tipo );
    signal( type monstruo_encargado, tipo );
    signal( type fogon, tipo );
    signal( type disparo_monstruo, tipo );
    signal( type vagoneta, tipo );
    signal( type copa_palmera, tipo );
    signal( type trozo_suelo, tipo );
    signal( type polvo, tipo );
    signal( type prepara_asteroides, tipo );
    signal( type llamas_del_suelo, tipo );
    signal( type resto_bala, tipo );
    signal( type prepara_bombazos, tipo );
    signal( type bombazos, tipo );
    signal( type enemigo_achicharrao, tipo );
    signal( type pinchos, tipo );
    signal( type piedras, tipo );
    signal( type asteroide, tipo );
    signal( type mini_nave, tipo );
    signal( type pedrazo, tipo );
    signal( type llamas_del_suelo, tipo );
    signal( type camara_horizontal, tipo );
    signal( type prepara_piedras, tipo );
    signal( type descenso_camara, tipo );
    signal( type explosion2, tipo );
    signal( type explosion3, tipo );
    signal( type torpedo, tipo );
    signal( type elicoptero2, tipo );
    signal( type humo, tipo );
    signal( type monstruo1, tipo );
    signal( type monstruo2, tipo );
    signal( type monstruo3, tipo );
    signal( type nave, tipo );
    signal( type gusano, tipo );
    signal( type nave_enemiga, tipo );
    signal( type disparo_nave, tipo );
    signal( type disparo_nave_enemiga, tipo );
    signal( type camara_nave, tipo );
    signal( type fogon, tipo );
    signal( type fogon_programado, tipo );
    signal( type helicoptero3, tipo );
    signal( type paracaidas, tipo );
    signal( type prepara_bombazos, tipo );
    signal( type llamazo, tipo );
    signal( type escopetazo, tipo );
    signal( type monstruo_encargado, tipo );
    signal( type movilatak, tipo );
    signal( type daldalboom, tipo );
    signal( type ultraboom, tipo );
    signal( type mini_onda, tipo );
    signal( type nave_inversa, tipo );
    signal( type nave_monstruo3, tipo );
    signal( type rayo_inverso, tipo );
    signal( type rayo_monstruo3, tipo );
    signal( type pincho_monstruo, tipo );
    signal( type canones, tipo );
    signal( type items, tipo );
    signal( type items_info, tipo );
    signal( type disparo_monstruo, tipo );
    signal( type espera_monstruo, tipo );
    signal( type super_onda, tipo );
    signal( type avion_destrozada, tipo );
    signal( type lluvia1, tipo );
    signal( type llamazo, tipo );
    signal( type cacho_heli, tipo );
    signal( type explosion2, tipo );
    signal( type lancha, tipo );
    signal( type trozo_torpedo, tipo );
    signal( type torpedo2, tipo );
    signal( type sangre_mix, tipo );
    signal( type escopetazo, tipo );
    signal( type cartel_fin, tipo );
    signal( type elemento_destructible, tipo );
    signal( type viento, tipo );
    signal( type nube, tipo );
    signal( type cacho_cuerpo_enemigo, tipo );
    signal( type espejado, tipo );
    signal( type agua, tipo );
    signal( type blur_efect, tipo );
    signal( type imita_josel, tipo );
    signal( type imita_bala, tipo );
    signal( type agua_reflejo, tipo );
    signal( type personajes_animados, tipo );
    signal( type enemigo_dispara, tipo );
    signal( type cacho_heli, tipo );
    signal( type piedras, tipo );
    signal( type robot, tipo );
    signal( type rail_robot, tipo );
    signal( type barco, tipo );
    signal( type agua, tipo );
    signal( type agua_reflejo, tipo );
    signal( type burbuja, tipo );
    signal( type salpicada, tipo );
    signal( type camara_juego, tipo );
end


PROCESS pausa_juego();
PRIVATE
    int estado = 1;
BEGIN
    if ( vol > 30 )
        music_set_volume( 30 );
    end
    x = 320;
    y = 240;
    graph = 325;
    size = 0;
    z = -100;
    alpha = 128;
    marca_pausa = 1; // fade(95,95,95,1);
    congelar( s_freeze );
    juego_en_pausa = 1;
    set_fps( velocidad_juego, 4 );
    LOOP
        if ( marca_pausa < 1 )
            marca_pausa = 3;
        end
        if ( marca_pausa > 3 )
            marca_pausa = 1;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            marca_pausa--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            marca_pausa++;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 20;
            else
                size = 100;
                titpausa();
                salir_partida();
                continuar_partida();
                reiniciar_partida();
                sound_play( poing, 0 ); // aqui !!
                estado = 2;
            end
        end
        frame;
    end
end

// objetivos fases

PROCESS titobjetivos( int graph );
PRIVATE
    int estado;
    int estado2;
BEGIN
    x = 318;
    y = 141;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( estado == 6 )
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 1;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 1;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS objetivos_mision( int tipo );
PRIVATE
    int estado = 1;
    int esperate = 1;
    int ke_graf;
    int ke_text;
BEGIN
    if ( vol > 30 )
        music_set_volume( 30 );
    end
    x = 320;
    y = 240;
    graph = 325;
    size = 10;
    z = -99;
    alpha = 128;
    marca_pausa = 1; // fade(95,95,95,1);
    if ( tipo == 1 )
        ke_graf = 222;
        ke_text = 366;
    end
    if ( tipo == 2 )
        ke_graf = 223;
        ke_text = 591;
    end
    if ( tipo == 3 )
        ke_graf = 224;
        ke_text = 662;
    end
    if ( tipo == 4 )
        ke_graf = 225;
    end
    if ( tipo == 5 )
        ke_graf = 226;
    end
    if ( tipo == 6 )
        ke_graf = 227;
    end
    if ( tipo == 7 )
        ke_graf = 228;
    end
    if ( tipo == 8 )
        ke_graf = 229;
    end
    if ( tipo == 9 )
        ke_graf = 230;
    end
    if ( tipo == 10 )
        ke_graf = 231;
    end
    signal( type jose_player1, s_wakeup );
    music_set_volume( vol );
    // fade(100,100,100,1);
    fade_on( 0 );
    juego_en_pausa = 0;
    pausa_en_pantalla = 0;
    // VOLVER!
    return;
    frame;
end


PROCESS objetivos_texto( int graph );
PRIVATE
    int estado;
    int estado2;
BEGIN
    x = 320;
    y = 240;
    size = 5;
    estado = 1;
    z = -100;
    LOOP
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end

// fin demo

PROCESS fin_mision( int tipo );
PRIVATE
    int estado = 1;
    int esperate = 1;
    int ke_graf;
    int ke_text;
BEGIN
    x = 320;
    y = 240;
    graph = 325;
    size = 10;
    z = -99;
    alpha = 128;
    marca_pausa = 1; // fade(95,95,95,1);
    if ( tipo == 1 )
        ke_graf = 222;
        ke_text = 366;
    end
    if ( tipo == 2 )
        ke_graf = 223;
    end
    if ( tipo == 3 )
        ke_graf = 224;
    end
    if ( tipo == 4 )
        ke_graf = 225;
    end
    if ( tipo == 5 )
        ke_graf = 226;
    end
    if ( tipo == 6 )
        ke_graf = 227;
    end
    if ( tipo == 7 )
        ke_graf = 228;
    end
    if ( tipo == 8 )
        ke_graf = 229;
    end
    if ( tipo == 9 )
        ke_graf = 230;
    end
    if ( tipo == 10 )
        ke_graf = 231;
    end
    congelar( s_freeze );
    LOOP
        if ( estado == 1 )
            if ( size < 100 )
                size += 5;
            else
                size = 100;
                titobjetivos( ke_graf );
                fin_fase();
                if ( modo_wiz == 0 )
                    objetivos_texto( 380 );
                end
                sound_play( poing, 0 ); // aqui !!
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS fin_fase();
PRIVATE
    int estado;
    int estado2;
    int una_vez;
    int escribe_uno;
BEGIN
    x = 320;
    y = 340;
    size = 5;
    graph = 31;
    x_p1 = 0;
    existe_explosion = 0;
    estado = 1;
    z = -101;
    text.z = -102;
    music_play( complete, 0 );
    LOOP
        if ( estado == 6 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                if ( una_vez == 0 )
                    sound_play( aceptar, 0, 16 );
                    abre_fase( 3 );
                    abre_fase( 4 ); // efecto ke oscurece la pantalla
                    signal( type pausa_juego, s_freeze );
                    signal( type camara_juego, s_freeze );
                    signal( type titpausa, s_freeze );
                    signal( type salir_partida, s_freeze );
                    signal( type continuar_partida, s_freeze );
                    signal( type reiniciar_partida, s_freeze );
                    signal( type jose_player1, s_freeze );
                    siguiente_mundo();
                    una_vez = 1; // proceso que iniciara el menu
                end // carga la siguiente escena
            end
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 1;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 1;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
                if ( modo_wiz == 0 )
                    write_var( 0, 407, 229, 4, total_disparos );
                    write_var( 0, 407, 240, 4, clones_eliminados );
                    // pondra ranking AA es lo maximo
                    if ( punteria_player1 => 75 AND limite_vida_player1 => vida_total -3 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "AA rank" );
                    end
                    if ( punteria_player1 => 65 AND limite_vida_player1 => vida_total -5 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "A rank" );
                    end
                    if ( punteria_player1 => 55 AND limite_vida_player1 => vida_total -7 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "B rank" );
                    end
                    if ( punteria_player1 => 45 AND limite_vida_player1 => vida_total -9 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "C rank" );
                    end
                    if ( punteria_player1 => 35 AND limite_vida_player1 => vida_total -11 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "D rank" );
                    end
                    if ( punteria_player1 => 25 AND limite_vida_player1 => vida_total -13 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "E rank" );
                    end
                    if ( punteria_player1 => 15 AND limite_vida_player1 => vida_total -15 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "F rank" );
                    end
                    if ( punteria_player1 => 10 AND limite_vida_player1 => vida_total -17 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "G rank" );
                    end
                    if ( punteria_player1 <= 9 AND limite_vida_player1 =< vida_total -18 AND escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "H rank" );
                    end
                    if ( escribe_uno == 0 )
                        escribe_uno = 1;
                        write( 0, 354, 218, 4, "C rank" );
                    end
                    write( 0, 369, 262, 4, objetivos_a );
                    write( 0, 369, 273, 4, objetivos_b );
                    write( 0, 369, 284, 4, objetivos_c );
                    write( 0, 369, 295, 4, objetivos_d );
                end
            end
        end
        frame;
    end
end


PROCESS siguiente_mundo();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            juego_en_pausa = 0;
            pausa_en_pantalla = 0;
            entrando = 0;
            fueramenu = 0;
            fade_off( 0 );
            scroll_stop( 0 );
            write_delete( all_text );
            let_me_alone();
            clear_screen();
            sound_stop( all_sound );
            music_stop();
            fpg_unload( fase1 );
            fpg_unload( scrolls );
            music_unload( s_fase1 );
            fade_on( 0 );
            // paso al siguiente mundo
            if ( estado_fase == 5 ) // por misiones
                fase2_jungla();
            end
            if ( estado_fase == 10 ) // por misiones
                fase3_nieve();
            end
            if ( estado_fase == 25 )
                final_juego();
            end
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            signal( id, s_kill );
        end
        frame;
    end
end

// menu de las pausas

PROCESS titpausa();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 322;
    x = 318;
    y = 141;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( estado == 6 )
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 1;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 1;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS continuando();
BEGIN
    sound_play( aceptar, 0, 16 );
    music_set_volume( vol );
    juego_en_pausa = 0;
    pausa_en_pantalla = 0;
    congelar( s_wakeup );
    signal( type pausa_juego, s_kill );
    signal( type titpausa, s_kill );
    signal( type salir_partida, s_kill );
    signal( type continuar_partida, s_kill );
    signal( type reiniciar_partida, s_kill );
    signal( type menu_del_juego, s_kill );
    signal( type tabla_del_menu, s_kill );
    signal( id, s_kill );
    LOOP
        frame;
    end
end


PROCESS continuar_partida();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 323;
    x = 320;
    y = 215;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( pulsa_esc == 1 OR pulsa_c == 1 )
            continuando();
        end
        if ( marca_pausa != 1 AND estado == 6 )
            size = 100;
        end
        if ( estado == 6 AND marca_pausa == 1 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                continuando();
            end
            if ( estado2 == 2 )
                if ( size < 120 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS reiniciar_partida();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 324;
    x = 321;
    y = 268;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( marca_pausa != 2 AND estado == 6 )
            size = 100;
        end
        if ( estado == 6 AND marca_pausa == 2 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                sound_play( aceptar, 0, 16 );
                signal( type camara_juego, s_kill ); // elimina todos los procesos de ese scroll
                cambio_musica = 0;
                abre_fase( 3 );
                abre_fase( 4 ); // efecto ke oscurece la pantalla
                signal( type pausa_juego, s_freeze );
                signal( type titpausa, s_freeze );
                signal( type salir_partida, s_freeze );
                signal( type continuar_partida, s_freeze );
                x_p1 = 0;
                existe_explosion = 0;
                reinicia_partida(); // proceso que iniciara el menu
                signal( id, s_freeze );
            end
            if ( estado2 == 2 )
                if ( size < 120 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS reinicia_partida();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            signal( type nave, s_kill );
            juego_en_pausa = 0;
            pausa_en_pantalla = 0;
            entrando = 0;
            fueramenu = 0;
            vida_robot = 0;
            fade_off( 0 );
            scroll_stop( 0 );
            write_delete( all_text );
            let_me_alone();
            sound_stop( all_sound );
            music_stop();
            if ( estado_fase == 1 OR estado_fase == 6 OR estado_fase == 22 )
                fpg_unload( fase1 );
                fpg_unload( scrolls );
                music_unload( s_fase1 );
            end
            // say("guarda: "+guarda_llaves);
            // say(objetos_llave);
            objetos_llave = guarda_llaves;
            objetos_combustible = guarda_gasolinas;
            objetos_regalo = guarda_regalos;
            // si reinicia el nivel, perdera todas las armas que consiga en el mismo
            municion_arma2 = guarda_arma2;
            municion_arma3 = guarda_arma3;
            municion_arma4 = guarda_arma4;
            municion_arma5 = guarda_arma5;
            municion_arma6 = guarda_arma6;
            municion_arma7 = guarda_arma7;
            municion_arma8 = guarda_arma8;
            municion_arma9 = guarda_arma9;
            municion_arma10 = guarda_arma10;
            // tambien perdera todos los objetos conseguidos , etc
            nivel_player1 = guarda_nivel1;
            agilidad_player1 = guarda_agilidad1;
            puntuacion = guarda_puntuacion1;
            disparos_acertados = guarda_disparos1;
            total_disparos = guarda_disparostotal1;
            sube_nivel = guarda_subenivel;
            clones_eliminados = guarda_clones;
            vida_total = guarda_vidatotal;
            limite_vida_player1 = vida_total;
            boss = 0;
            llaves = 0;
            regalos = 0;
            gasolinas = 0;
            arma_seleccionada_p1 = 1;
            realiza_intro1 = 0;
            music_set_volume( vol );
            scroll.y0 = 0;
            scroll.y1 = 0;
            // estoy en la pantalla que voy a repetir
            reiniciando = 1;
            if ( estado_fase == 1 )
                reiniciando = 0;
                fase1_desierto();
            end
            if ( estado_fase == 2 )
                fase1_a();
            end
            if ( estado_fase == 3 )
                fase1_b();
            end
            if ( estado_fase == 4 )
                fase1_c();
            end
            if ( estado_fase == 5 )
                fase1_d();
            end
            if ( estado_fase == 6 )
                reiniciando = 0;
                fase2_jungla();
            end
            if ( estado_fase == 7 )
                fase2_a();
            end
            if ( estado_fase == 8 )
                fase2_b();
            end
            if ( estado_fase == 9 )
                fase2_c();
            end
            if ( estado_fase == 10 )
                fase2_d();
            end
            if ( estado_fase == 22 )
                reiniciando = 0;
                fase3_nieve();
            end
            if ( estado_fase == 23 )
                fase3_a();
            end
            if ( estado_fase == 24 )
                fase3_b();
            end
            if ( estado_fase == 25 )
                fase3_c();
            end
            fade_on( 0 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS salir_partida();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 40;
    x = 315;
    y = 321;
    size = 5;
    estado = 1;
    z = -101;
    LOOP
        if ( marca_pausa != 3 AND estado == 6 )
            size = 100;
        end
        if ( estado == 6 AND marca_pausa == 3 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                sound_play( aceptar, 0, 16 );
                signal( type camara_juego, s_kill ); // elimina todos los procesos de ese scroll
                cambio_musica = 0;
                abre_fase( 3 );
                abre_fase( 4 ); // efecto ke oscurece la pantalla
                signal( type pausa_juego, s_freeze );
                signal( type titpausa, s_freeze );
                signal( type continuar_partida, s_freeze );
                signal( type reiniciar_partida, s_freeze );
                extermina_partida(); // proceso que iniciara el menu
                x_p1 = 0;
                existe_explosion = 0;
                signal( id, s_freeze );
            end
            if ( estado2 == 2 )
                if ( size < 120 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS extermina_partida();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            juego_en_pausa = 0;
            pausa_en_pantalla = 0;
            entrando = 0;
            boss = 0;
            fueramenu = 0;
            josexplota = 0;
            objetoencara = 0;
            fade_off( 0 );
            scroll_stop( 0 );
            write_delete( all_text );
            let_me_alone();
            clear_screen();
            set_fps( 24, 0 );
            sound_stop( all_sound );
            music_stop();
            fpg_unload( fase1 );
            fpg_unload( scrolls );
            music_unload( s_fase1 );
            sound2 = music_load( "sound/sound2.mp3" );
            limite_vida_player1 = 25;
            // si no acaba bien el nivel, perdera todas las armas que consiga en el mismo
            if ( autorecarga != 1 )
                municion_arma2 = -1;
                municion_arma3 = -1;
                municion_arma4 = -1;
                municion_arma5 = -1;
                municion_arma6 = -1;
                municion_arma7 = -1;
                municion_arma8 = -1;
                municion_arma9 = -1;
                municion_arma10 = -1;
            end
            // tambi‚n perdera todos los objetos conseguidos , etc
            nivel_player1 = 1;
            puntuacion = 0;
            agilidad_player1 = 1;
            disparos_acertados = 1;
            total_disparos = 1;
            sube_nivel = 25000;
            clones_eliminados = 0;
            vida_total = 8;
            if ( cantidadificultad == 1 )
                vida_total = 15;
            end
            if ( cantidadificultad == 2 )
                vida_total = 12;
            end
            punteria_player1 = 0;
            objetos_llave = 0;
            objetos_combustible = 0;
            objetos_muelle = 0;
            objetos_moneda = 0;
            objetos_chip = 0;
            objetos_teletransportador = 0;
            objetos_vasoagua = 0;
            // del tiempo de juego
            seg_juego = 0;
            min_juego = 0;
            hora_juego = 0;
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            scroll.y0 = 0;
            scroll.y1 = 0;
            presentacion();
            fade_on( 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS arma_players( int ke_player );
PRIVATE
    int ke_municion;
    int arma_l2 = 2;
    int arma_l3 = 2;
    int arma_l4 = 2;
    int arma_l5 = 2;
    int arma_l6 = 2;
    int arma_l7 = 2;
    int arma_l8 = 2;
    int arma_l9 = 2;
BEGIN
    escrito = 1;
    if ( ke_player == 1 )
        x = 194;
        y = 29;
    end
    if ( ke_player == 2 )
        x = 373;
        y = 29;
    end
    z = -51;
    text.z = -90;
    LOOP
        if ( ke_player == 1 )
            if ( arma_seleccionada_p1 == 2 AND municion_arma2 =< 0 )
                arma_seleccionada_p1 = 3;
                if ( arma_l2 == 5 )
                    arma_l2 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 3 AND municion_arma3 =< 0 )
                arma_seleccionada_p1 = 4;
                if ( arma_l3 == 5 )
                    arma_l3 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 4 AND municion_arma4 =< 0 )
                arma_seleccionada_p1 = 5;
                if ( arma_l4 == 5 )
                    arma_l4 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 5 AND municion_arma5 =< 0 )
                arma_seleccionada_p1 = 6;
                if ( arma_l5 == 5 )
                    arma_l5 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 6 AND municion_arma6 =< 0 )
                arma_seleccionada_p1 = 7;
                if ( arma_l6 == 5 )
                    arma_l6 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 7 AND municion_arma7 =< 0 )
                arma_seleccionada_p1 = 8;
                if ( arma_l7 == 5 )
                    arma_l7 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 8 AND municion_arma8 =< 0 )
                arma_seleccionada_p1 = 9;
                if ( arma_l8 == 5 )
                    arma_l8 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 9 AND municion_arma9 =< 0 )
                arma_seleccionada_p1 = 10;
                if ( arma_l9 == 5 )
                    arma_l9 = 1;
                    arma_seleccionada_p1 = 1;
                    ke_municion = 1;
                end
            end
            if ( arma_seleccionada_p1 == 10 AND municion_arma10 =< 0 )
                arma_seleccionada_p1 = 1;
                ke_municion = 1;
            end
            if ( arma_l2 == 1 )
                arma_gastada( x_p1, y_p1, 335, imita_flags );
                arma_l2 = 2;
            end
            if ( arma_l3 == 1 )
                arma_gastada( x_p1, y_p1, 334, imita_flags );
                arma_l3 = 2;
            end
            if ( arma_l4 == 1 )
                arma_gastada( x_p1, y_p1, 321, imita_flags );
                arma_l4 = 2;
            end
            if ( arma_l5 == 1 )
                arma_gastada( x_p1, y_p1, 336, imita_flags );
                arma_l5 = 2;
            end
            if ( arma_l6 == 1 )
                arma_gastada( x_p1, y_p1, 333, imita_flags );
                arma_l6 = 2;
            end
            if ( arma_seleccionada_p1 == 1 )
                graph = 237;
                x = 194;
                y = 29;
                if ( ke_municion == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    ke_municion = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 344, 2 );
                end
            end
            if ( arma_seleccionada_p1 == 2 AND municion_arma2 > 0 )
                graph = 335;
                arma_l2 = 5;
                x = 196;
                y = 30;
                tengo = municion_arma2;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 345, 2 );
                    sound_play( golpe2, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 3 AND municion_arma3 > 0 )
                graph = 334;
                arma_l3 = 5;
                x = 193;
                y = 32;
                tengo = municion_arma3;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 346, 2 );
                    sound_play( golpe3, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 4 AND municion_arma4 > 0 )
                graph = 321;
                arma_l4 = 5;
                x = 200;
                y = 29;
                tengo = municion_arma4;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 347, 2 );
                    sound_play( golpe4, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 5 AND municion_arma5 > 0 )
                graph = 336;
                arma_l5 = 5;
                x = 199;
                y = 33;
                tengo = municion_arma5;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 348, 2 );
                    sound_play( golpe5, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 6 AND municion_arma6 > 0 )
                graph = 333;
                arma_l6 = 5;
                x = 194;
                y = 30;
                tengo = municion_arma6;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 349, 2 );
                    sound_play( golpe8, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 7 AND municion_arma7 > 0 )
                graph = 627;
                arma_l7 = 5;
                x = 193;
                y = 32;
                tengo = municion_arma7;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 411, 2 );
                    sound_play( golpe11, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 8 AND municion_arma8 > 0 )
                graph = 634;
                arma_l8 = 5;
                x = 200;
                y = 29;
                tengo = municion_arma8;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 409, 2 );
                    sound_play( golpe16, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 9 AND municion_arma9 > 0 )
                graph = 625;
                arma_l9 = 5;
                x = 193;
                y = 32;
                tengo = municion_arma9;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 410, 2 );
                    sound_play( golpe15, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 == 10 AND municion_arma10 > 0 )
                graph = 626;
                tengo = municion_arma10;
                if ( escrito == 1 )
                    write_delete( all_text );
                    write_var( fuente, 90, 22, 4, puntuacion );
                    write_var( fuente, 261, 33, 4, tengo );
                    escrito = 0;
                    signal( type msn_engine, s_kill );
                    msn_engine( 412, 2 );
                    sound_play( golpe12, 0, 13 );
                end
            end
            if ( arma_seleccionada_p1 => 11 )
                arma_seleccionada_p1 = 1;
                ke_municion = 1;
                sound_play( golpe14, 0, 13 );
            end
        end
        frame;
    end
end


PROCESS municion_players( int ke_player );
BEGIN
    if ( ke_player == 1 )
        x = 262;
        y = 34;
    end
    if ( ke_player == 2 )
        x = 442;
        y = 34;
    end
    z = -51;
    LOOP
        if ( ke_player == 1 )
            if ( arma_seleccionada_p1 == 1 )
                graph = 313;
            end
            if ( arma_seleccionada_p1 != 1 )
                graph = 197;
            end
        end
        if ( ke_player == 2 )
            if ( arma_seleccionada_p2 == 1 )
                graph = 313;
            end
            if ( arma_seleccionada_p2 != 1 )
                graph = 197;
            end
        end
        frame;
    end
end


PROCESS vida_player1();
PRIVATE
    int porciento;
BEGIN
    x = 89;
    y = 41;
    z = -51;
    size = 76;
    LOOP
        porciento = limite_vida_player1 * 100 / vida_total * 100 / 100;
        if ( porciento > 100 )
            porciento = 100;
        end
        if ( limite_vida_player1 > vida_total )
            limite_vida_player1 = vida_total;
        end
        if ( invencible == 1 )
            limite_vida_player1 = 9999;
            vida_total = 9999;
        end
        if ( porciento > 95 AND porciento <= 100 )
            graph = 219;
        end
        if ( porciento > 90 AND porciento <= 95 )
            graph = 218;
        end
        if ( porciento > 85 AND porciento <= 90 )
            graph = 217;
        end
        if ( porciento > 80 AND porciento <= 85 )
            graph = 216;
        end
        if ( porciento > 75 AND porciento <= 80 )
            graph = 215;
        end
        if ( porciento > 70 AND porciento <= 75 )
            graph = 214;
        end
        if ( porciento > 65 AND porciento <= 70 )
            graph = 213;
        end
        if ( porciento > 60 AND porciento <= 65 )
            graph = 212;
        end
        if ( porciento > 55 AND porciento <= 60 )
            graph = 211;
        end
        if ( porciento > 50 AND porciento <= 55 )
            graph = 210;
        end
        if ( porciento > 45 AND porciento <= 50 )
            graph = 209;
        end
        if ( porciento > 40 AND porciento <= 45 )
            graph = 208;
        end
        if ( porciento > 35 AND porciento <= 40 )
            graph = 207;
        end
        if ( porciento > 30 AND porciento <= 35 )
            graph = 206;
        end
        if ( porciento > 25 AND porciento <= 30 )
            graph = 205;
        end
        if ( porciento > 20 AND porciento <= 25 )
            graph = 204;
        end
        if ( porciento > 15 AND porciento <= 20 )
            graph = 203;
        end
        if ( porciento > 10 AND porciento <= 15 )
            graph = 202;
        end
        if ( porciento > 5 AND porciento <= 10 )
            graph = 201;
        end
        if ( porciento => 0 AND porciento <= 5 )
            graph = 200;
        end
        frame;
    end
end

// FASES DEL JUEGO !! ESCENARIOS !! MUSICAS !!
// 1 escenario del juego !! " Entrada al pueblo moro "

PROCESS tapa_scroll( double x, double y, int graph );
BEGIN
    file = scrolls;
    ctype = c_scroll;
    z = 7;
    LOOP
        frame;
    end
end


PROCESS tercer_scroll( double x, double y, int graph );
BEGIN
    file = scrolls;
    ctype = c_scroll;
    z = 10;
    priority = 1000;
    LOOP
        x = scroll.x0 / 2; // Halla la posicion relativa al scroll
        frame;
    end
end


PROCESS fase1_desierto();
PRIVATE
    int una_vez;
BEGIN
    // mato y kallo a too , pongo nueva velocidad
    let_me_alone();
    scroll_stop( 0 );
    clear_screen();
    fade_off( 0 );
    sound_stop( all_sound );
    music_stop();
    write_delete( all_text );
    set_fps( velocidad_juego, 4 );
    // carga escenario
    scrolls = fpg_load( "fpg/scroll1.fpg" );
    fase1 = fpg_load( "fpg/fase1.fpg" );
    // Carga cancion del escenario
    s_fase1 = music_load( "sound/fase1.mp3" );
    // Actualiza la hora del reloj
    // Hace reflejos de todo lo que ve
    x_p1 = 0;
    haz_reflejos = 0;
    // Restaurar los valores iniciales
    limite_vida_player1 = vida_total;
    arma_seleccionada_p1 = 1;
    arma_seleccionada_p2 = 1;
    realiza_intro1 = 0;
    llaves = 0;
    // restaura todos los objetos recolectados
    objetos_muelle = 0;
    objetos_moneda = 0;
    objetos_chip = 0;
    objetos_teletransportador = 0;
    objetos_vasoagua = 0;
    objetos_reductor = 0;
    objetos_ampliador = 0;
    objetos_joselillo = 0;
    objetos_navecilla = 0;
    objetos_melon = 0;
    objetos_pirana = 0;
    objetos_joselxdye = 0;
    objetos_ovni = 0;
    llaves = 0;
    regalos = 0;
    gasolinas = 0;
    objetos_combustible = 0;
    objetos_llave = 0;
    // efectos al empezar
    abre_fase( 1 );
    abre_fase( 2 );
    numero_pantalla( 222 );
    juego_en_pausa = 1;
    pausa_en_pantalla = 1;
    // Inicia controladores de juego
    estado_fase = 1;
    limite_pantalla = 2995;
    controlador_principal();
    // inicia los 2 scroll
    // tiempo actual 1= noche, 2= tarde, 12= dia,
    if ( modo_wiz == 1 )
        tiempo_actual = 12;
    end
    scroll_start( 0, scrolls, 3, scrolls, tiempo_actual, 0, 6 );
    scroll.z = 50;
    scroll.ratio = 400;
    if ( modo_wiz == 0 )
        if ( tiempo_actual == 2 OR tiempo_actual == 12 )
            tercer_scroll( 320, 364, 51 );
        end
        if ( tiempo_actual == 1 )
            tercer_scroll( 320, 364, 52 );
        end // x, y , graph,
        tapa_scroll( 1500, 362, 50 ); // x , y , graph
    end
    // Indica a todos los objetos k suelo deben tocar
    ke_fase_es = scrolls;
    ke_grafico_es = 23;
    //ke_color_es = 0x8cfcf8ff; // 110;
    // permite que las explosiones destrocen el escenario
    grafico_origen1 = 3;
    grafico_origen2 = 23;
    // objetivos de la fase
    objetivos_a = "incompleto";
    objetivos_a1 = 0;
    objetivos_b = "incompleto";
    // situacion de la fase, numero enemigos
    if ( cantidadificultad == 1 ) // nivel f cil
        elicopteros = 0;
        enemigos_tierra = 20;
        tankes = 1;
        camiones = 1;
        coche( 1250, 426, 1 );
    end
    if ( cantidadificultad == 2 ) // nivel normal
        elicopteros = 0;
        enemigos_tierra = 30;
        tankes = 1;
        camiones = 1;
        coche( 1250, 426, 1 );
    end
    if ( cantidadificultad == 3 ) // nivel dif¡cil
        elicopteros = 0;
        enemigos_tierra = 40;
        tankes = 1;
        camiones = 1;
        coche( 1250, 426, 1 );
    end
    // Fin fase
    cartel_fin( 2672, 384 );
    // enemigos de la pantalla (si hay)
    // Elementos de la pantalla
    if ( tiempo_actual == 2 OR tiempo_actual == 12 ) // segun la hora del dia, habra gente en la calle
        secundario( 397, 325, 113, fase1, 1 );
        secundario( 947, 354, 113, fase1, 1 );
        secundario( 2673, 300, 113, fase1, 1 );
    end
    if ( tiempo_actual == 1 ) // la noche ..
        secundario( 1471, 326, 121, fase1, 2 );
    end
    items( 1180, 199, 10 );
    items( 1816, 92, 6 );
    items( 1893, 188, 5 );
    items( 914, 187, 5 );
    items( 1481, 227, 5 );
    items( 2661, 341, 5 );
    viento( rand( -100, -1000 ), 365, 11, fase1, 1 );
    elemento_destructible( 227, 172, 28, fase1, 1 ); // cachos de casa, etc, pueden destruirse
    elemento_destructible( 267, 287, 29, fase1, 1 );
    elemento_destructible( 130, 328, 30, fase1, 1 );
    elemento_destructible( 1190, 313, 31, fase1, 1 );
    if ( tiempo_actual == 12 )
        nube( -50, 38, 327, 0, 80, 1 );
        nube( -50, 90, 10, fase1, 80, 2 );
        nube( -50, 125, 10, fase1, 60, 3 );
        nube( 600, 38, 327, 0, 80, 1 );
        nube( 600, 90, 10, fase1, 80, 2 );
        nube( 600, 125, 10, fase1, 60, 3 );
    end
    if ( tiempo_actual == 1 OR tiempo_actual == 2 )
        nube( -50, 38, 326, 0, 100, 1 );
        nube( -50, 90, 326, 0, 80, 2 );
        nube( -50, 125, 326, 0, 60, 3 );
        nube( 600, 38, 326, 0, 100, 1 );
        nube( 600, 90, 326, 0, 80, 2 );
        nube( 600, 125, 326, 0, 60, 3 );
    end
    primer_plano( 350, 15, fase1, 573 ); // graficos por delante del scroll
    primer_plano( 350, 15, fase1, 1504 ); // palmeras
    primer_plano( 350, 15, fase1, 2511 );
    primer_plano( 350, 15, fase1, 318 );
    primer_plano( 334, 74, fase1, 444 ); // savanas colgaas
    primer_plano( 463, 71, fase1, 2403 ); // butaca con savana
    primer_plano( 441, 73, fase1, 1374 ); // savana colgaa en un palo
    carro( 957, 367, 8, fase1 );
    carro( 2135, 339, 8, fase1 );
    cachocasa( 1182, 88, 27, fase1 );
    copa_palmera( 324, 237, 16, fase1 );
    copa_palmera( 640, 271, 16, fase1 );
    copa_palmera( 2208, 221, 16, fase1 );
    copa_palmera( 1600, 249, 16, fase1 );
    hierba( 59, 401, 6, fase1, 1 );
    hierba( 259, 400, 6, fase1, 1 );
    hierba( 1519, 444, 6, fase1, 1 );
    hierba( 1773, 456, 6, fase1, 1 );
    hierba( 2210, 446, 6, fase1, 1 );
    palla( 475, 446, 7, fase1 );
    palla( 1039, 450, 7, fase1 );
    palla( 1812, 468, 7, fase1 );
    palla( 1857, 459, 7, fase1 );
    piedra( 378, 437, 22, fase1, 1 );
    piedra( 762, 437, 22, fase1, 0 );
    piedra( 1584, 437, 22, fase1, 1 );
    piedra( 1968, 437, 22, fase1, 0 );
    piedra( 2790, 437, 22, fase1, 1 );
    // Enciende pantalla cuando cargue
    fade_on( 0 );
    musica_fase = music_play( s_fase1, -1 );
    music_set_volume( vol );
    // signal(type jose_player1,s_freeze);
    LOOP
        if ( x_p1 => limite_pantalla -5 )
            fase1_a(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS coche_enemigo( double x, double y, int graph, int file );
PRIVATE
    int estado;
    int estado2 = -1;
    int sonido_camion;
BEGIN
    ctype = c_scroll;
    z = -12;
    flags = 0;
    estado = 0;
    size = 137;
    sonido_camion = sound_play( camion, -1 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        // si colisiona con los barriles explota
        if ( collision( type salta_barril ))
            signal( type salta_barril, s_kill );
            sound_stop( sonido_camion );
            explosion( x + 140, y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            piedras( x, y, 425, 100, 3 );
            piedras( x, y, 426, 100, 9 );
            piedras( x, y, 428, 100, 15 );
            piedras( x, y, 427, 100, 21 );
            piedras( x, y, 427, 100, 1 );
            piedras( x, y, 427, 100, 5 );
            piedras( x, y, 427, 100, 18 );
            piedras( x, y, 429, rand( 70, 95 ), 1 );
            piedras( x, y, 429, rand( 70, 95 ), 2 );
            piedras( x, y, 429, rand( 70, 95 ), 3 );
            piedras( x, y, 429, rand( 70, 95 ), 4 );
            piedras( x, y, 429, rand( 70, 95 ), 5 );
            piedras( x, y, 429, rand( 70, 95 ), 6 );
            piedras( x, y, 429, rand( 70, 95 ), 7 );
            piedras( x, y, 429, rand( 70, 95 ), 8 );
            piedras( x, y, 429, rand( 70, 95 ), 9 );
            piedras( x, y, 429, rand( 70, 95 ), 10 );
            piedras( x, y, 429, rand( 70, 95 ), 11 );
            piedras( x, y, 429, rand( 70, 95 ), 12 );
            piedras( x, y, 429, rand( 70, 95 ), 13 );
            piedras( x, y, 429, rand( 70, 95 ), 14 );
            piedras( x, y, 429, rand( 70, 95 ), 15 );
            piedras( x, y, 429, rand( 70, 95 ), 16 );
            piedras( x, y, 429, rand( 70, 95 ), 17 );
            piedras( x, y, 429, rand( 70, 95 ), 18 );
            piedras( x, y, 429, rand( 70, 95 ), 19 );
            piedras( x, y, 429, rand( 70, 95 ), 20 );
            piedras( x, y, 429, rand( 70, 95 ), 21 );
            piedras( x, y, 429, rand( 70, 95 ), 22 );
            piedras( x, y, 429, rand( 70, 95 ), 23 );
            piedras( x, y, 429, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        // el coche tiembla
        estado2++;
        if ( estado2 < 3 )
            y++;
        end
        if ( estado2 => 3 )
            y--;
        end
        if ( estado2 == 5 )
            estado2 = -1;
        end
        // Persigue al personaje
        if ( estado == 1 AND flags == 1 AND x_p1 < x + 1000 )
            x -= 12;
        end
        if ( estado == 1 AND flags == 1 AND x_p1 > x + 1000 )
            estado = 0;
            flags = 0;
        end
        if ( estado == 0 AND flags == 0 AND x_p1 > x -1000 )
            x += 12;
        end
        if ( estado == 0 AND flags == 0 AND x_p1 < x -1000 )
            estado = 1;
            flags = 1;
        end
        frame;
    end
end


PROCESS salta_barril( double x, double y, int graph, int file );
PRIVATE
    double y_incr = -17;
    int estado;
    int rotacion;
    int cuenta;
BEGIN
    z = -10;
    ctype = c_scroll;
    rotacion = rand( 10000, 20000 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( estado == 0 )
            angle += rotacion;
            y_incr++;
            y += y_incr;
        end
        if ( y > 470 )
            estado = 1;
            angle = 0;
        end
        frame;
    end
end


PROCESS coche( double x, double y, int tipo );
PRIVATE
    int estado;
    int incr_x;
    int estado2 = -1;
    int crea_enemigos;
    int suena_ahora;
    int sonido_camion;
    int incr_s = 256;
    int canal_s = 255;
    int una_vez;
BEGIN
    // tipo de vehiculo
    if ( tipo == 1 )
        file = fase1;
        z = -12;
        graph = 21;
        flags = 1;
        size = 137;
    end
    ctype = c_scroll;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( limite_vida_player1 =< 0 AND una_vez == 0 )
            if ( sound_is_playing( sonido_camion ))
                sound_stop( sonido_camion );
                una_vez = 1;
            end
        end
        if ( suena_ahora == 0 AND x_p1 > x -500 )
            suena_ahora = 1;
            sonido_camion = sound_play( camion, -1 );
        end
        if ( suena_ahora == 1 AND x_p1 < x -500 )
            sound_stop( sonido_camion );
            suena_ahora = 0;
        end
        if ( canal_s < 5 )
            canal_s = 5;
        end
        if ( incr_s > 350 )
            incr_s = 350;
        end
        if ( tipo == 1 )
            estado2++;
            if ( estado2 < 3 )
                y++;
            end
            if ( estado2 => 3 )
                y--;
            end
            if ( estado2 == 5 )
                estado2 = -1;
            end
            if ( crea_enemigos == 0 AND estado == 1 )
                if ( cantidadificultad == 1 ) // nivel f cil
                    enemigo_arabe( 1296, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_dispara( 1276, 440, fase1, 0 );
                    salta_barril( x, y, 258, fase1 );
                    salta_barril( x + 60, y, 258, fase1 );
                    salta_barril( x + 30, y -10, 258, fase1 );
                end
                if ( cantidadificultad == 2 ) // nivel normal
                    enemigo_arabe( 1296, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_dispara( 1276, 440, fase1, 0 );
                    salta_barril( x, y, 258, fase1 );
                    salta_barril( x + 60, y, 258, fase1 );
                    salta_barril( x + 30, y -10, 258, fase1 );
                end
                if ( cantidadificultad == 3 ) // nivel dificil
                    enemigo_arabe( 1296, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_arabe( 1276, 420, 1, 1, 1 );
                    enemigo_dispara( 1276, 440, fase1, 0 );
                    salta_barril( x, y, 258, fase1 );
                    salta_barril( x + 60, y, 258, fase1 );
                    salta_barril( x + 30, y -10, 258, fase1 );
                end
                crea_enemigos = 1;
            end
            if ( x_p1 > 1000 )
                estado = 1;
            end
            if ( estado == 1 )
                x -= incr_x;
                incr_x++;
                incr_s += 3;
                canal_s -= 4;
                if ( region_out( id, 0 ))
                    coche_enemigo( x, y, graph, file );
                    sound_stop( sonido_camion );
                    signal( id, s_kill );
                end
            end
        end
        frame;
    end
end

// 2 escenario de la fase 1, " el pueblo "

PROCESS fase1_a();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
    int una_vez;
    int comprueba;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            let_me_alone();
            clear_screen();
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll2.fpg" );
            // Inicia controladores de juego
            estado_fase = 2;
            limite_pantalla = 4995;
            controlador_principal();
            // inicia los 2 scroll
            if ( modo_wiz == 1 )
                tiempo_actual = 2;
            end
            scroll_start( 0, scrolls, 94, scrolls, tiempo_actual, 0, 6 );
            scroll.z = 50;
            scroll.ratio = 400;
            if ( modo_wiz == 0 )
                if ( tiempo_actual == 2 OR tiempo_actual == 12 )
                    tercer_scroll( 1200, 409, 51 );
                end
                if ( tiempo_actual == 1 )
                    tercer_scroll( 1200, 409, 52 );
                end // x, y , graph,
                tapa_scroll( 2500, 412, 50 ); // x , y , graph
            end
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 99;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "incompleto";
            // situacion de la fase, numero enemigos
            if ( cantidadificultad == 1 ) // en el nivel f cil
                elicopteros = 2;
                enemigos_tierra = 10;
                tankes = 1;
                camiones = 1;
            end
            // enemigos
            elicoptero1( 2499, 84, 20, fase1, 1 );
            enemigo_arabe( 1286, 420, 1, 1, 1 );
            enemigo_arabe( 1286, 420, 1, 1, 1 );
            enemigo_arabe( 1286, 420, 1, 1, 1 );
            enemigo_dispara( 1286, 289, fase1, 1 );
            if ( cantidadificultad == 2 ) // en el nivel normal
                elicopteros = 3;
                enemigos_tierra = 12;
                tankes = 1;
                camiones = 1;
                enemigo_arabe( 1286, 420, 1, 1, 1 );
            end
            if ( cantidadificultad == 3 ) // en el nivel dificil
                elicopteros = 3;
                enemigos_tierra = 15;
                tankes = 2;
                camiones = 1;
                elicoptero1( 2099, 84, 20, fase1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
            end
            // Fin de la fase
            cartel_fin( 4812, 190 );
            barracada( 4949, 424, 1 );
            barracada( 1892, 460, 2 );
            // obstaculos , pinchos, etc
            pinchos( 1762, 438, 101, fase1 );
            pinchos( 1844, 438, 101, fase1 );
            pinchos( 1980, 439, 101, fase1 );
            pinchos( 2051, 440, 101, fase1 );
            pinchos( 2199, 441, 101, fase1 );
            pinchos( 2299, 447, 101, fase1 );
            items( 2374, 368, 1 );
            items( 4694, 19, 10 );
            items( 1209, 188, 3 );
            items( 3679, 266, 13 );
            // objetos , escenario
            if ( tiempo_actual == 2 OR tiempo_actual == 12 ) // segun la hora del dia, habra gente en la calle
                secundario( 3331, 382, 113, fase1, 1 );
                secundario( 4909, 248, 113, fase1, 1 );
                secundario( 2729, 395, 113, fase1, 1 );
                secundario( 867, 402, 113, fase1, 1 );
            end
            if ( tiempo_actual == 1 ) // la noche ..
                secundario( 1485, 366, 121, fase1, 2 );
            end
            viento( rand( -100, -1000 ), 365, 11, fase1, 1 );
            cachocasa( 3325, 138, 27, fase1 );
            cachocasa( 4038, 133, 103, fase1 );
            cachocasa( 4589, 62, 114, fase1 );
            cachocasa( 4929, 86, 120, fase1 );
            carro( 1463, 397, 8, fase1 );
            carro( 534, 408, 107, fase1 );
            hierba( 1628, 410, 108, fase1, 1 );
            hierba( 2472, 423, 108, fase1, 1 );
            hierba( 332, 429, 108, fase1, 1 );
            if ( tiempo_actual == 12 )
                nube( -50, 38, 327, 0, 80, 1 );
                nube( -50, 90, 10, fase1, 80, 2 );
                nube( -50, 125, 10, fase1, 60, 3 );
                nube( 600, 38, 327, 0, 80, 1 );
                nube( 600, 90, 10, fase1, 80, 2 );
                nube( 600, 125, 10, fase1, 60, 3 );
            end
            if ( tiempo_actual == 1 OR tiempo_actual == 2 )
                nube( -50, 38, 326, 0, 100, 1 );
                nube( -50, 90, 326, 0, 80, 2 );
                nube( -50, 125, 326, 0, 60, 3 );
                nube( 600, 38, 326, 0, 100, 1 );
                nube( 600, 90, 326, 0, 80, 2 );
                nube( 600, 125, 326, 0, 60, 3 );
            end
            items( 435, 260, 5 );
            items( 25, 240, 5 );
            items( 663, 240, 5 );
            items( 1404, 244, 5 );
            items( 1917, 204, 5 );
            items( 3266, 251, 5 );
            items( 4021, 12, 5 );
            primer_plano( 350, 15, fase1, 87 ); // graficos por delante del scroll
            primer_plano( 350, 15, fase1, 983 );
            primer_plano( 350, 15, fase1, 1811 );
            primer_plano( 350, 15, fase1, 3037 );
            primer_plano( 350, 15, fase1, 3327 );
            primer_plano( 350, 15, fase1, 3983 );
            primer_plano( 350, 15, fase1, 4826 );
            primer_plano( 464, 15, fase1, 2745 );
            palla( 2626, 455, 7, fase1 );
            copa_palmera( 1470, 301, 16, fase1 );
            barril( 147, 279, fase1, 249, 1 );
            barril( 940, 274, fase1, 249, 1 );
            barril( 4205, 271, fase1, 249, 1 );
            barril( 128, 237, fase1, 251, 2 );
            barril( 945, 236, fase1, 250, 2 );
            barril( 4193, 232, fase1, 252, 2 );
            // efectos al empezar
            if ( NOT music_is_playing())
                musica_fase = music_play( s_fase1, -1 );
            end
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
        end
        if ( x_p1 => limite_pantalla -5 AND carga_todo == 0 )
            fase1_b();
            return; // carga la siguiente escena
        end
        frame;
    end
end

// Barriles destructivos

PROCESS Barril( double x, double y, int file, int graph, int tipo );
PRIVATE
    int grafico = 408;
    int estado;
BEGIN
    ctype = c_scroll;
    if ( graph == 3 )
        grafico = 451;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( tipo == 1 )
            z = -1;
            if ( collision( type bala ) OR collision( type explosion ) OR collision( type bala_enemigo ) OR collision( type escopetazo ) OR collision( type llamazo ) OR collision( type explosion3 ))
                estado = 1;
            end
        end
        if ( estado == 1 )
            puntuacion += 200;
            explosion3( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            signal( id, s_kill );
        end
        if ( tipo == 2 AND collision( type explosion3 ))
            estado = 2;
        end
        if ( estado == 2 )
            puntuacion += 200;
            piedras( x, y, grafico, rand( 70, 95 ), 1 );
            piedras( x, y, grafico, rand( 70, 95 ), 2 );
            piedras( x, y, grafico, rand( 70, 95 ), 3 );
            piedras( x, y, grafico, rand( 70, 95 ), 4 );
            piedras( x, y, grafico, rand( 70, 95 ), 5 );
            piedras( x, y, grafico, rand( 70, 95 ), 6 );
            piedras( x, y, grafico, rand( 70, 95 ), 7 );
            piedras( x, y, grafico, rand( 70, 95 ), 8 );
            piedras( x, y, grafico, rand( 70, 95 ), 9 );
            piedras( x, y, grafico, rand( 70, 95 ), 10 );
            piedras( x, y, grafico, rand( 70, 95 ), 11 );
            piedras( x, y, grafico, rand( 70, 95 ), 12 );
            piedras( x, y, grafico, rand( 70, 95 ), 13 );
            piedras( x, y, grafico, rand( 70, 95 ), 14 );
            piedras( x, y, grafico, rand( 70, 95 ), 15 );
            piedras( x, y, grafico, rand( 70, 95 ), 16 );
            piedras( x, y, grafico, rand( 70, 95 ), 17 );
            piedras( x, y, grafico, rand( 70, 95 ), 18 );
            piedras( x, y, grafico, rand( 70, 95 ), 19 );
            piedras( x, y, grafico, rand( 70, 95 ), 20 );
            piedras( x, y, grafico, rand( 70, 95 ), 21 );
            piedras( x, y, grafico, rand( 70, 95 ), 22 );
            piedras( x, y, grafico, rand( 70, 95 ), 23 );
            piedras( x, y, grafico, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        frame;
    end
end

// 3 escenario de la fase 1, "la mina"

PROCESS fase1_b();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
    int una_vez;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll3.fpg" );
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // Inicia controladores de juego
            estado_fase = 3;
            limite_pantalla = 4995;
            controlador_principal();
            // inicia los 2 scroll
            scroll_start( 0, scrolls, 126, scrolls, 130, 0, 6 );
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 127;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 126;
            grafico_origen2 = 127;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/mina.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "incompleto";
            enemigo_dispara( 600, 420, fase1, 1 );
            enemigo_dispara( 640, 420, fase1, 1 );
            enemigo_dispara( 700, 420, fase1, 1 );
            msn_enemigos = 1;
            llamas_del_suelo();
            // Fin de la fase
            cartel_fin( 4503, 233 );
            barracada( 4265, 384, 3 );
            barracada( 4454, 380, 3 );
            barracada( 4656, 373, 3 );
            // obstaculos , pinchos, etc
            pinchos( 4160, 366, 101, fase1 );
            pinchos( 4229, 366, 101, fase1 );
            pinchos( 4348, 366, 101, fase1 );
            pinchos( 4420, 366, 101, fase1 );
            pinchos( 4540, 366, 101, fase1 );
            pinchos( 4614, 366, 101, fase1 );
            // objetos , escenario
            items( 2912, 166, 11 );
            items( 207, 287, 7 );
            items( 60, 221, 5 );
            items( 630, 159, 5 );
            items( 982, 169, 5 );
            items( 1752, 226, 5 );
            items( 2516, 185, 5 );
            items( 630, 346, 5 );
            items( 3280, 142, 5 );
            items( 2934, 261, 5 );
            items( 3787, 160, 5 );
            items( 4212, 208, 5 );
            items( 4582, 290, 5 );
            secundario( 624, 355, 132, fase1, 2 );
            primer_plano( 389, 128, fase1, 109 ); // graficos por delante del scroll
            primer_plano( 332, 128, fase1, 1400 );
            primer_plano( 402, 128, fase1, 2272 );
            primer_plano( 375, 128, fase1, 3352 );
            primer_plano( 409, 128, fase1, 4508 );
            primer_plano( 122, 129, fase1, 746 );
            primer_plano( 122, 129, fase1, 1950 );
            primer_plano( 122, 129, fase1, 2900 );
            primer_plano( 122, 129, fase1, 4100 );
            primer_plano( 240, 131, fase1, 350 );
            primer_plano( 240, 131, fase1, 1100 );
            primer_plano( 240, 131, fase1, 1650 );
            primer_plano( 240, 131, fase1, 2550 );
            primer_plano( 240, 131, fase1, 3750 );
            primer_plano( 240, 131, fase1, 4350 );
            vida_robot = 15;
            prohibido_elpaso( 1226, 340, fase1, 95, 1 );
            prohibido_elpaso( 1226, 261, fase1, 95, 2 );
            prohibido_elpaso( 1226, 182, fase1, 95, 3 );
            robot( 1145, 157, fase1, 241, 1 );
            rail_robot( 1154, 264, fase1, 245, 4 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        // Esto hace un cambio de pantalla al llegar a su limite
        if ( x_p1 => limite_pantalla -5 AND carga_todo == 0 )
            fase1_c(); // carga la siguiente escena
            return;
        end
        frame;
    end
end

// prohibido el paso , unica en 3 nivel

PROCESS prohibido_elpaso( double x, double y, int file, int graph, int tipo );
PRIVATE
    int para_barreras;
    double y_incr;
BEGIN
    ctype = c_scroll;
    y_incr = -5;
    if ( tipo == 1 )
        z = -2;
    end
    if ( tipo == 2 )
        z = -3;
    end
    if ( tipo == 3 )
        z = -4;
    end
    if ( tipo == 4 )
        z = -5;
    end
    if ( tipo == 50 )
        z = -6;
    end
    if ( tipo == 100 )
        z = -4;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( vida_robot == 0 )
            para_barreras = 1;
        end
        if ( tipo < 4 AND para_barreras == 1 )
            piedras( x, y, 406, rand( 70, 95 ), 1 );
            piedras( x, y, 406, rand( 70, 95 ), 2 );
            piedras( x, y, 406, rand( 70, 95 ), 7 );
            piedras( x, y, 406, rand( 70, 95 ), 8 );
            piedras( x, y, 406, rand( 70, 95 ), 13 );
            piedras( x, y, 406, rand( 70, 95 ), 14 );
            piedras( x, y, 406, rand( 70, 95 ), 19 );
            piedras( x, y, 406, rand( 70, 95 ), 20 );
            signal( id, s_kill );
        end
        if ( tipo == 5 AND collision( type explosion ))
            piedras( x, y, 406, rand( 70, 95 ), 1 );
            piedras( x, y, 406, rand( 70, 95 ), 2 );
            piedras( x, y, 406, rand( 70, 95 ), 3 );
            piedras( x, y, 406, rand( 70, 95 ), 4 );
            piedras( x, y, 406, rand( 70, 95 ), 5 );
            piedras( x, y, 406, rand( 70, 95 ), 6 );
            piedras( x, y, 406, rand( 70, 95 ), 7 );
            piedras( x, y, 406, rand( 70, 95 ), 8 );
            piedras( x, y, 406, rand( 70, 95 ), 9 );
            piedras( x, y, 406, rand( 70, 95 ), 10 );
            piedras( x, y, 406, rand( 70, 95 ), 11 );
            piedras( x, y, 406, rand( 70, 95 ), 12 );
            piedras( x, y, 406, rand( 70, 95 ), 13 );
            piedras( x, y, 406, rand( 70, 95 ), 14 );
            piedras( x, y, 406, rand( 70, 95 ), 15 );
            piedras( x, y, 406, rand( 70, 95 ), 16 );
            piedras( x, y, 406, rand( 70, 95 ), 17 );
            piedras( x, y, 406, rand( 70, 95 ), 18 );
            piedras( x, y, 406, rand( 70, 95 ), 19 );
            piedras( x, y, 406, rand( 70, 95 ), 20 );
            piedras( x, y, 406, rand( 70, 95 ), 21 );
            piedras( x, y, 406, rand( 70, 95 ), 22 );
            piedras( x, y, 406, rand( 70, 95 ), 23 );
            piedras( x, y, 406, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        if ( tipo == 100 AND para_barreras == 1 )
            piedras( x, y, 406, rand( 70, 95 ), 1 );
            piedras( x, y, 406, rand( 70, 95 ), 2 );
            piedras( x, y, 406, rand( 70, 95 ), 7 );
            piedras( x, y, 406, rand( 70, 95 ), 8 );
            piedras( x, y, 406, rand( 70, 95 ), 13 );
            piedras( x, y, 406, rand( 70, 95 ), 14 );
            piedras( x, y, 406, rand( 70, 95 ), 19 );
            piedras( x, y, 406, rand( 70, 95 ), 20 );
            if ( cantidadificultad == 1 ) // en el nivel f cil
                enemigos_tierra = 40;
                elicopteros = 0;
                tankes = 1;
                camiones = 1;
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_dispara( 1286, 289, fase1, 1 );
            end
            if ( cantidadificultad == 2 ) // en el nivel normal
                enemigos_tierra = 50;
                tankes = 1;
                camiones = 1;
                elicopteros = 0;
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_dispara( 1286, 289, fase1, 1 ); // ****
                enemigo_dispara( 1286, 289, fase1, 1 );
            end
            if ( cantidadificultad == 3 ) // en el nivel dificil
                enemigos_tierra = 60;
                tankes = 2;
                camiones = 1;
                elicopteros = 0;
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_arabe( 1286, 420, 1, 1, 1 );
                enemigo_dispara( 1286, 289, fase1, 1 ); // ****
                enemigo_dispara( 1286, 289, fase1, 1 );
            end
            signal( id, s_kill );
        end
        if ( tipo == 6 AND collision( type bala ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS rail_robot( double x, double y, int file, int graph, int tipo );
PRIVATE
    int para_barreras;
    double y_incr;
BEGIN
    ctype = c_scroll;
    y_incr = -5;
    if ( tipo == 1 )
        z = -2;
    end
    if ( tipo == 2 )
        z = -3;
    end
    if ( tipo == 3 )
        z = -4;
    end
    if ( tipo == 4 )
        z = -5;
    end
    LOOP
        if ( vida_robot == 0 )
            para_barreras = 1;
        end
        if ( tipo == 4 AND para_barreras == 1 )
            angle -= 5000;
            x += 5;
            y_incr++;
            y += y_incr;
            if ( region_out( id, 0 ))
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS robot( double x, double y, int file, int graph, int tipo );
PRIVATE
    int esperate;
    int esperate2;
    int estado;
    int disparo_aleatorio;
    int recorre_disparo;
    int calcula_posicion;
    int vida_robot1 = 30;
BEGIN
    ctype = c_scroll;
    z = -6;
    disparo_aleatorio = rand( 30, 100 );
    size = 115;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    total_robots--;
    LOOP
        vida_robot = vida_robot1;
        if ( vida_robot1 < 0 )
            vida_robot1 = 0;
        end
        if ( esperate2 > 0 )
            esperate2--;
        end
        if ( tipo == 1 )
            esperate++;
            recorre_disparo++; // robot montado en una pared que dispara en un tiempo aleatorio
            if ( recorre_disparo == disparo_aleatorio )
                if ( !region_out( id, 0 ))
                    bala_enemigo( x -5, y, 350, 0, 20, 1 );
                    sound_play( laser6, 0, 14 );
                    disparo_aleatorio = rand( 30, 100 );
                    recorre_disparo = 0;
                else
                    recorre_disparo = 0;
                end
            end
            if ( vida_robot1 == 0 AND esperate2 == 0 )
                puntuacion += 1000;
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                robot( 2301, 101, fase1, 241, 2 );
                rail_robot( 2311, 211, fase1, 245, 4 );
                prohibido_elpaso( 2389, 290, fase1, 95, 1 );
                prohibido_elpaso( 2389, 214, fase1, 95, 2 );
                prohibido_elpaso( 2389, 134, fase1, 95, 3 );
                signal( id, s_kill );
            end
            if ( esperate == 1 )
                graph = 241;
            end
            if ( esperate == 2 )
                graph = 242;
            end
            if ( esperate == 3 )
                graph = 243;
            end
            if ( esperate == 4 )
                graph = 242;
            end
            if ( esperate => 5 )
                esperate = 0;
            end
            if ( collision( type bala ) OR collision( type explosion ) OR collision( type escopetazo ) OR collision( type llamazo ))
                if ( esperate2 == 0 )
                    graph = 244;
                    vida_robot1--;
                    esperate2 = 3;
                    sound_play( smetal1, 0, 21 );
                    explosion2( x, y );
                end
            end
            if ( estado == 0 AND y < 367 )
                y += 3;
                if ( y >= 367 )
                    estado = 1;
                end
            end
            if ( estado == 1 AND y > 157 )
                y -= 3;
                if ( y <= 157 )
                    estado = 0;
                end
            end
        end
        if ( tipo == 2 )
            esperate++;
            recorre_disparo++; // robot montado en una pared que dispara en un tiempo aleatorio
            if ( recorre_disparo == disparo_aleatorio )
                if ( !region_out( id, 0 ))
                    bala_enemigo( x -5, y, 350, 0, 20, 1 );
                    sound_play( laser6, 0, 14 );
                    disparo_aleatorio = rand( 30, 100 );
                    recorre_disparo = 0;
                else
                    recorre_disparo = 0;
                end
            end
            if ( esperate == 1 )
                graph = 241;
            end
            if ( esperate == 2 )
                graph = 242;
            end
            if ( esperate == 3 )
                graph = 243;
            end
            if ( esperate == 4 )
                graph = 242;
            end
            if ( esperate => 5 )
                esperate = 0;
            end
            if ( collision( type bala ) OR collision( type explosion ) OR collision( type escopetazo ) OR collision( type llamazo ))
                if ( esperate2 == 0 )
                    graph = 244;
                    vida_robot1--;
                    esperate2 = 3;
                    sound_play( smetal1, 0, 21 );
                    explosion2( x, y );
                end
            end
            if ( estado == 0 AND y < 316 )
                y += 3;
                if ( y >= 316 )
                    estado = 1;
                end
            end
            if ( estado == 1 AND y > 101 )
                y -= 3;
                if ( y <= 101 )
                    estado = 0;
                end
            end
            if ( vida_robot1 == 0 )
                vida_robot = 0;
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                signal( id, s_kill );
            end
        end
        frame;
    end
end

// 4 escenario de la fase 1, " la vagoneta "

PROCESS fase1_c();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=1; restore_type=-1;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll4.fpg" );
            // Inicia controladores de juego
            estado_fase = 4; // *** cambiar
            limite_pantalla = 6995; // *** cambiar
            controlador_principal();
            // inicia los 2 scroll
            scroll_start( 0, scrolls, 135, scrolls, 130, 0, 4 ); // *** cambiar
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 137; // *** cambiar
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 135;
            grafico_origen2 = 137;
            objetivos_b = "-";
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/fase6.mp3" );
            // objetos , escenario
            items( 1272, 207, 1 );
            items( 497, 69, 2 );
            items( 5308, 419, 2 );
            items( 1955, 99, 3 );
            items( 2495, 282, 3 );
            items( 6775, 203, 3 );
            primer_plano( 389, 128, fase1, 109 ); // graficos por delante del scroll
            primer_plano( 332, 128, fase1, 1400 );
            primer_plano( 402, 128, fase1, 2272 );
            primer_plano( 375, 128, fase1, 3352 );
            primer_plano( 409, 128, fase1, 4508 );
            primer_plano( 122, 129, fase1, 746 );
            primer_plano( 122, 129, fase1, 1950 );
            primer_plano( 122, 129, fase1, 2900 );
            primer_plano( 122, 129, fase1, 4100 );
            primer_plano( 122, 129, fase1, 5000 );
            primer_plano( 122, 129, fase1, 5900 );
            primer_plano( 122, 129, fase1, 6800 );
            primer_plano( 240, 131, fase1, 350 );
            primer_plano( 240, 131, fase1, 1000 );
            primer_plano( 240, 131, fase1, 1650 );
            primer_plano( 240, 131, fase1, 2550 );
            primer_plano( 240, 131, fase1, 3750 );
            primer_plano( 240, 131, fase1, 4350 );
            primer_plano( 240, 131, fase1, 5350 );
            primer_plano( 240, 131, fase1, 6350 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        if ( x_p1 => limite_pantalla -50 AND carga_todo == 0 )
            fase1_d(); // carga la siguiente escena
            return;
        end
        frame;
    end
end

// escenario 5 de la fase1, " el mercaillo "

PROCESS fase1_d();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=1; restore_type=-1;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            fade_on( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll5.fpg" );
            // Inicia controladores de juego
            estado_fase = 5; // *** cambiar
            limite_pantalla = 2995; // *** cambiar
            controlador_principal();
            // inicia los 2 scroll
            if ( modo_wiz == 1 )
                tiempo_actual = 1;
            end
            scroll_start( 0, scrolls, 142, scrolls, tiempo_actual, 0, 6 ); // *** cambiar
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 143; // *** cambiar
            //ke_color_es = 0x8cfcf8ff; // 110;
            enemigos_tierra = 1;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 142;
            grafico_origen2 = 143;
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/monstro.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "-";
            // situacion de la fase, numero enemigos
            enemigo_atropellado( 283, 379, 52, fase1 );
            enemigo_atropellado( 303, 379, 52, fase1 );
            enemigo_atropellado( 320, 379, 52, fase1 );
            enemigo_atropellado( 333, 379, 52, fase1 );
            enemigo_atropellado( 350, 386, 52, fase1 );
            enemigo_atropellado( 380, 379, 52, fase1 );
            enemigo_atropellado( 403, 379, 52, fase1 );
            enemigo_atropellado( 426, 376, 52, fase1 );
            enemigo_atropellado( 503, 379, 52, fase1 );
            enemigo_atropellado( 543, 379, 52, fase1 );
            enemigo_atropellado( 584, 458, 52, fase1 );
            enemigo_atropellado( 572, 410, 52, fase1 );
            enemigo_atropellado( 610, 379, 52, fase1 );
            enemigo_atropellado( 633, 379, 52, fase1 );
            enemigo_atropellado( 661, 379, 52, fase1 );
            enemigo_atropellado( 683, 379, 52, fase1 );
            enemigo_atropellado( 710, 379, 52, fase1 );
            enemigo_atropellado( 756, 398, 52, fase1 );
            enemigo_atropellado( 773, 379, 52, fase1 );
            enemigo_atropellado( 800, 379, 52, fase1 );
            enemigo_atropellado( 843, 359, 52, fase1 );
            enemigo_atropellado( 883, 359, 52, fase1 );
            enemigo_atropellado( 918, 365, 52, fase1 );
            enemigo_atropellado( 953, 359, 52, fase1 );
            enemigo_atropellado( 993, 353, 52, fase1 );
            enemigo_atropellado( 1069, 366, 52, fase1 );
            enemigo_atropellado( 1100, 366, 52, fase1 );
            enemigo_atropellado( 1125, 359, 52, fase1 );
            enemigo_atropellado( 1158, 362, 52, fase1 );
            enemigo_atropellado( 1178, 359, 52, fase1 );
            enemigo_atropellado( 1200, 366, 52, fase1 );
            enemigo_atropellado( 1241, 389, 52, fase1 );
            enemigo_atropellado( 1289, 366, 52, fase1 );
            enemigo_atropellado( 1300, 359, 52, fase1 );
            enemigo_atropellado( 1337, 368, 52, fase1 );
            enemigo_atropellado( 1369, 366, 52, fase1 );
            enemigo_atropellado( 1383, 359, 52, fase1 );
            enemigo_atropellado( 1415, 368, 52, fase1 );
            enemigo_atropellado( 1483, 403, 52, fase1 );
            enemigo_atropellado( 1519, 366, 52, fase1 );
            enemigo_atropellado( 1539, 359, 52, fase1 );
            enemigo_atropellado( 1569, 365, 52, fase1 );
            enemigo_atropellado( 1600, 359, 52, fase1 );
            enemigo_atropellado( 1630, 359, 52, fase1 );
            enemigo_atropellado( 1657, 368, 52, fase1 );
            enemigo_atropellado( 1680, 359, 52, fase1 );
            enemigo_atropellado( 1703, 359, 52, fase1 );
            enemigo_atropellado( 1773, 359, 52, fase1 );
            enemigo_atropellado( 1748, 378, 52, fase1 );
            enemigo_atropellado( 1773, 359, 52, fase1 );
            enemigo_atropellado( 1800, 366, 52, fase1 );
            enemigo_atropellado( 1833, 359, 52, fase1 );
            enemigo_atropellado( 1856, 368, 52, fase1 );
            enemigo_atropellado( 1883, 359, 52, fase1 );
            enemigo_atropellado( 1939, 379, 52, fase1 );
            enemigo_atropellado( 1973, 359, 52, fase1 );
            enemigo_atropellado( 2012, 384, 52, fase1 );
            enemigo_atropellado( 2053, 359, 52, fase1 );
            enemigo_atropellado( 2097, 374, 52, fase1 );
            cachocasa( 2811, 272, 93, fase1 );
            tanke( 2916, 374, 32, fase1 );
            // obstaculos , pinchos, etc
            enemigos_tierra = 70;
            // objetos , escenario
            if ( tiempo_actual == 12 )
                nube( -50, 38, 327, 0, 80, 1 );
                nube( -50, 90, 10, fase1, 80, 2 );
                nube( -50, 125, 10, fase1, 60, 3 );
                nube( 600, 38, 327, 0, 80, 1 );
                nube( 600, 90, 10, fase1, 80, 2 );
                nube( 600, 125, 10, fase1, 60, 3 );
            end
            if ( tiempo_actual == 1 OR tiempo_actual == 2 )
                nube( -50, 38, 326, 0, 100, 1 );
                nube( -50, 90, 326, 0, 80, 2 );
                nube( -50, 125, 326, 0, 60, 3 );
                nube( 600, 38, 326, 0, 100, 1 );
                nube( 600, 90, 326, 0, 80, 2 );
                nube( 600, 125, 326, 0, 60, 3 );
            end
            primer_plano( 350, 15, fase1, 573 ); // graficos por delante del scroll
            primer_plano( 350, 15, fase1, 1504 ); // palmeras
            primer_plano( 350, 15, fase1, 318 );
            primer_plano( 441, 73, fase1, 1374 );
            // efectos al empezar
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        frame;
    end
end

// 6 escenario del juego "La jungla"

PROCESS fase2_jungla();
PRIVATE
    int una_vez;
BEGIN
    // mato y kallo a too , pongo nueva velocidad
    let_me_alone();
    clear_screen();
    fade_off( 0 );
    sound_stop( all_sound );
    music_stop();
    // dump_type=0; restore_type=0;
    write_delete( all_text );
    scroll_stop( 0 );
    set_fps( velocidad_juego, 4 );
    // carga escenario
    scrolls = fpg_load( "fpg/scroll6.fpg" );
    fase1 = fpg_load( "fpg/fase2.fpg" );
    // Carga cancion del escenario
    s_fase1 = music_load( "sound/fase2.mp3" );
    // Actualiza la hora del reloj
    // Hace reflejos de todo lo que ve
    x_p1 = 0;
    haz_reflejos = 1; // aqui va un 1
    // Restaurar los valores iniciales
    limite_vida_player1 = vida_total;
    arma_seleccionada_p1 = 1;
    arma_seleccionada_p2 = 1;
    realiza_intro1 = 0;
    // efectos al empezar
    abre_fase( 1 );
    abre_fase( 2 );
    numero_pantalla( 223 );
    juego_en_pausa = 1;
    pausa_en_pantalla = 1;
    // Inicia controladores de juego
    estado_fase = 6;
    limite_pantalla = 3995;
    controlador_principal();
    // inicia los 2 scroll
    // tiempo actual 1= noche, 2= tarde, 12= dia,
    if ( modo_wiz == 1 )
        tiempo_actual = 12;
    end
    scroll_start( 0, scrolls, 5, scrolls, tiempo_actual, 0, 6 );
    scroll.z = 50;
    // Indica a todos los objetos k suelo deben tocar
    ke_fase_es = scrolls;
    ke_grafico_es = 6;
    //ke_color_es = 0x8cfcf8ff; // 110;
    // permite que las explosiones destrocen el escenario
    grafico_origen1 = 5;
    grafico_origen2 = 6;
    // objetivos de la fase
    objetivos_a = "incompleto";
    objetivos_a1 = 0;
    objetivos_b = "-";
    // situacion de la fase, numero enemigos
    if ( cantidadificultad == 1 ) // nivel f cil
        elicopteros = 3;
        enemigos_tierra = 30;
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_dispara( 1286, 289, fase1, 1 );
        elicoptero2( 2000, 100, 200, fase1, 1 );
    end
    if ( cantidadificultad == 2 ) // nivel normal
        elicopteros = 4;
        enemigos_tierra = 40;
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_dispara( 1261, 289, fase1, 1 );
        enemigo_dispara( 1261, 289, fase1, 1 );
        elicoptero2( 2000, 100, 200, fase1, 1 );
    end
    if ( cantidadificultad == 3 ) // nivel dif¡cil
        elicopteros = 5;
        enemigos_tierra = 50;
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_arabe( 1761, 420, 1, 1, 1 );
        enemigo_dispara( 1261, 289, fase1, 1 );
        enemigo_dispara( 1261, 289, fase1, 1 );
        elicoptero2( 2000, 100, 200, fase1, 1 );
    end
    // Fin fase
    cartel_fin( 3699, 330 );
    // enemigos de la pantalla (si hay)
    bola_pinchos( 518, 4 );
    bola_pinchos( 834, 4 );
    bola_pinchos( 1189, 5 );
    bola_pinchos( 1513, 4 );
    bola_pinchos( 1870, 5 );
    bola_pinchos( 2239, 4 );
    bola_pinchos( 2629, 4 );
    bola_pinchos( 3727, 4 );
    pinchos( 1847, 450, 185, fase1 );
    pinchos( 1891, 450, 185, fase1 );
    pinchos( 1935, 450, 185, fase1 );
    pinchos( 2047, 450, 185, fase1 );
    pinchos( 2091, 450, 185, fase1 );
    pinchos( 2135, 450, 185, fase1 );
    pinchos( 2258, 450, 185, fase1 );
    pinchos( 2303, 450, 185, fase1 );
    pinchos( 2349, 450, 185, fase1 );
    // Elementos de la pantalla
    items( 3490, 288, 2 );
    items( 1438, 138, 8 );
    items( 543, 178, 4 );
    barril( 1201, 106, fase1, 3, 2 );
    barril( 1285, 185, fase1, 249, 1 );
    hierba( 528, 184, 189, fase1, 10 );
    hierba( 1445, 172, 189, fase1, 1 );
    agua( 320, 435, fase1, 15, 1 );
    agua_reflejo( x_p1, 440, fase1, 23, 2 );
    if ( modo_wiz == 0 )
        espejado( 931, 449, fase1, 16, 3 );
        espejado( 2200, 424, fase1, 18, 3 );
        espejado( 1989, 426, fase1, 19, 3 );
        espejado( 2651, 467, fase1, 20, 3 );
        espejado( 3365, 438, fase1, 21, 3 );
    end
    primer_plano( 40, 196, fase1, 190 );
    primer_plano( 40, 196, fase1, 1052 );
    primer_plano( 40, 196, fase1, 2318 );
    primer_plano( 40, 196, fase1, 2800 );
    primer_plano( 40, 196, fase1, 3400 );
    primer_plano( 357, 14, fase1, 266 );
    primer_plano( 444, 26, fase1, 66 );
    primer_plano( 458, 27, fase1, 800 );
    primer_plano( 444, 26, fase1, 1500 );
    primer_plano( 458, 27, fase1, 2000 );
    primer_plano( 444, 26, fase1, 3000 );
    primer_plano( 355, 14, fase1, 1216 );
    primer_plano( 353, 14, fase1, 1894 );
    primer_plano( 356, 14, fase1, 2377 );
    primer_plano( 361, 14, fase1, 2542 );
    primer_plano( 356, 14, fase1, 3505 );
    // Enciende pantalla cuando cargue
    fade_on( 0 );
    musica_fase = music_play( s_fase1, -1 );
    music_set_volume( vol );
    // signal(type imita_bala,s_freeze);
    // signal(type imita_2,s_freeze);
    // signal(type imita_josel,s_freeze);
    // signal(type jose_player1,s_freeze);
    LOOP
        if ( x_p1 => limite_pantalla -5 )
            fase2_a(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS fase2_a();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
    int una_vez;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll7.fpg" );
            // Cambia cancion si hace falta
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 1; // aki va un 1
            // Inicia controladores de juego
            estado_fase = 7;
            limite_pantalla = 5995;
            controlador_principal();
            // inicia los 2 scroll
            if ( modo_wiz == 1 )
                tiempo_actual = 2;
            end
            scroll_start( 0, scrolls, 3, scrolls, tiempo_actual, 0, 6 );
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "incompleto";
            // situacion de la fase, numero enemigos
            enemigos_tierra = 10;
            helicoptero3( 800, 120 );
            prohibido_elpaso( 640, 223, fase1, 2, 100 );
            prohibido_elpaso( 640, 223, fase1, 2, 3 );
            // enemigos de la pantalla (si hay)
            // objetos , escenario
            tapa_base( 5874, 276 );
            items( 440, 12, 9 );
            items( 2282, 59, 9 );
            items( 4674, -10, 9 );
            items( 5313, 64, 9 );
            items( 1265, 230, 9 );
            items( 555, 327, 1 );
            items( 3067, 96, 11 );
            items( 142, 96, 10 );
            cartel_fin( 5400, 302 );
            pinchos( 4659, 382, 185, fase1 );
            pinchos( 4854, 382, 185, fase1 );
            pinchos( 4883, 382, 185, fase1 );
            pinchos( 4909, 382, 185, fase1 );
            pinchos( 5079, 382, 185, fase1 );
            pinchos( 5220, 382, 185, fase1 );
            pinchos( 5310, 382, 185, fase1 );
            sal_pantalla( 5883, 285, 6 );
            agua( 320, 435, fase1, 15, 1 );
            agua_reflejo( x_p1, 440, fase1, 23, 2 );
            espejado( 383, 428, fase1, 25, 3 );
            espejado( 1377, 439, fase1, 28, 3 );
            espejado( 2417, 438, fase1, 241, 3 );
            espejado( 3792, 435, fase1, 242, 3 );
            espejado( 5367, 430, fase1, 243, 3 );
            primer_plano( 40, 196, fase1, 190 );
            primer_plano( 40, 196, fase1, 1052 );
            primer_plano( 40, 196, fase1, 2318 );
            primer_plano( 40, 196, fase1, 2800 );
            primer_plano( 40, 196, fase1, 3400 );
            primer_plano( 40, 196, fase1, 4000 );
            primer_plano( 40, 196, fase1, 4500 );
            primer_plano( 40, 196, fase1, 5000 );
            primer_plano( 40, 196, fase1, 5700 );
            primer_plano( 357, 14, fase1, 566 );
            primer_plano( 444, 26, fase1, 66 );
            primer_plano( 458, 27, fase1, 800 );
            primer_plano( 444, 26, fase1, 1500 );
            primer_plano( 458, 27, fase1, 2000 );
            primer_plano( 444, 26, fase1, 3000 );
            primer_plano( 444, 26, fase1, 4300 );
            primer_plano( 444, 26, fase1, 4700 );
            primer_plano( 444, 26, fase1, 5500 );
            primer_plano( 355, 14, fase1, 1216 );
            primer_plano( 353, 14, fase1, 1894 );
            primer_plano( 356, 14, fase1, 2377 );
            primer_plano( 361, 14, fase1, 2542 );
            primer_plano( 356, 14, fase1, 3505 );
            // efectos al empezar
            if ( NOT music_is_playing())
                musica_fase = music_play( s_fase1, -1 );
            end
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
        end
        frame;
    end
end


PROCESS sal_pantalla( double x, double y, int graph );
BEGIN
    file = scrolls;
    ctype = c_scroll;
    z = 2;
    LOOP
        frame;
    end
end


PROCESS tapa_base( double x, double y );
BEGIN
    file = scrolls;
    ctype = c_scroll;
    z = -7;
    graph = 4;
    LOOP
        frame;
    end
end


PROCESS fase2_b();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=1; restore_type=-1;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll8.fpg" );
            // Cambia cancion si hace falta
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // Inicia controladores de juego
            estado_fase = 8;
            limite_pantalla = 635;
            controlador_principal();
            // inicia los 2 scroll
            scroll.camera = 0;
            scroll_start( 0, scrolls, 3, scrolls, 0, 0, 0 );
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/fase6.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "-";
            // situacion de la fase, numero enemigos
            if ( cantidadificultad == 1 ) // en el nivel f cil
                enemigos_tierra = 40;
                elicopteros = 3;
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 2 ) // en el nivel normal
                enemigos_tierra = 50;
                elicopteros = 3;
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 3 ) // en el nivel dificil
                enemigos_tierra = 60;
                elicopteros = 3;
                tankes = 2;
                camiones = 1;
            end
            // enemigos de la pantalla (si hay)
            helicoptero3( 800, 120 );
            helicoptero3( 1500, 120 );
            helicoptero3( 2047, 120 );
            items( 320, 370, 1 );
            items( 30, 249, 12 );
            items( 623, 249, 13 );
            // objetos , escenario
            ascensOR ();
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        if ( x_p1 => 634 AND carga_todo == 0 )
            fase2_c(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS helicoptero3( double x, double y );
PRIVATE
    int estado;
    int grafica;
    int estado2;
    int haz_graficos;
    int haz_graficos1;
    int tocao;
    int esperate;
    int vida = 30;
BEGIN
    ctype = c_scroll;
    file = fase1;
    graph = 265;
    z = -8;
    vida_robot++;
    LOOP
        estado2++;
        if ( estado2 <= 5 )
            y--;
        end
        if ( estado2 > 5 )
            y++;
        end
        if ( estado2 => 10 )
            estado2 = 0;
        end
        if ( esperate > 0 )
            esperate--;
        end
        if ( tocao == 1 )
            haz_graficos1++;
            if ( haz_graficos1 == 1 )
                if ( graph == 265 )
                    graph = 268;
                end
                if ( graph == 266 )
                    graph = 269;
                end
                if ( graph == 267 )
                    graph = 270;
                end
            end
            if ( haz_graficos1 == 3 )
                if ( graph == 268 )
                    graph = 265;
                end
                if ( graph == 269 )
                    graph = 266;
                end
                if ( graph == 270 )
                    graph = 267;
                end
            end
            if ( haz_graficos1 == 5 )
                if ( graph == 265 )
                    graph = 268;
                end
                if ( graph == 266 )
                    graph = 269;
                end
                if ( graph == 267 )
                    graph = 270;
                end
            end
            if ( haz_graficos1 == 7 )
                if ( graph == 268 )
                    graph = 265;
                end
                if ( graph == 269 )
                    graph = 266;
                end
                if ( graph == 270 )
                    graph = 267;
                end
            end
            if ( haz_graficos1 => 9 )
                tocao = 0;
                haz_graficos1 = 0;
            end
        end
        if ( vida =< 0 )
            sound_play( cristal2, 0 );
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            cacho_heli( x, y, 271, fase1, 1 );
            cacho_heli( x, y, 272, fase1, 2 );
            cacho_heli( x, y, 273, fase1, 3 );
            cacho_heli( x, y, 274, fase1, 4 );
            elicopteros--;
            vida_robot--;
            signal( id, s_kill );
        end
        if (( collision( type bala ) OR collision( type explosion ) OR collision( type escopetazo ) OR collision( type llamazo )) AND esperate == 0 )
            tocao = 1;
            esperate = 4;
            vida--;
            sound_play( smetal1, 0, 21 );
            explosion2( x + rand( -20, 20 ), y + rand( -20, 20 ));
        end
        if ( grafica == 1 )
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 265;
            end
            if ( haz_graficos == 4 )
                graph = 266;
            end
            if ( haz_graficos == 8 )
                graph = 267;
                sound_play( pistola, 0, 17 );
                bala_enemigo( x -21, y + 54, 341, 0, 50, 1 );
                bala_enemigo( x + 39, y + 54, 341, 0, 50, 1 );
                if ( estado == 6 )
                    flags = 0;
                end
                if ( estado == 0 )
                    flags = 1;
                end
            end
            if ( haz_graficos == 14 )
                graph = 266;
            end
            if ( haz_graficos == 18 )
                graph = 265;
                haz_graficos = 0;
                grafica = 0;
                estado++;
            end
        end
        if ( estado == 0 AND grafica == 0 )
            x -= 6;
            if ( x < 50 )
                grafica = 1;
            end
        end
        if ( estado == 1 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 100 )
                grafica = 1;
            end
        end
        if ( estado == 2 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 200 )
                grafica = 1;
            end
        end
        if ( estado == 3 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 300 )
                grafica = 1;
            end
        end
        if ( estado == 4 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 400 )
                grafica = 1;
            end
        end
        if ( estado == 5 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 500 )
                grafica = 1;
            end
        end
        if ( estado == 6 AND grafica == 0 )
            flags = 1;
            x += 6;
            if ( x > 600 )
                grafica = 1;
            end
        end
        if ( estado => 7 )
            flags = 0;
            estado = 0;
        end
        frame;
    end
end


PROCESS descenso_camara();
PRIVATE
    int salida_enemigos;
    int esperate = 0;
BEGIN
    file = scrolls;
    graph = 2;
    x = 320;
    y = 0;
    z = 90;
    descenso_camara1();
    LOOP
        /*
if (boom==1) esperate++;
if (esperate==1) scroll.y0=10; end if (esperate==2) scroll.y0=20; end
if (esperate==3) scroll.y0=40; end if (esperate==4) scroll.y0=20; end
if (esperate==5) scroll.y0=0;end if (esperate==6) scroll.y0=10; end
if (esperate==7) scroll.y0=20; end if (esperate==8) scroll.y0=10; end
if (esperate==9) scroll.y0=0; end if (esperate=>10) boom=0; esperate=0; end
end
*/
        if ( y > -3350 )
            y -= 2;
        end
        if ( y <= -3350 AND salida_enemigos == 4 )
            if ( enemigos_tierra > 0 )
                enemigos_tierra = 1;
            end
            salida_enemigos = 5;
        end
        if ( y < -1000 AND salida_enemigos == 0 )
            sound_play( cristal2, 0 );
            sound_play( cristal2, 0 );
            piedras( 1, 200, 624, rand( 70, 95 ), 1 );
            piedras( 1, 200, 624, rand( 70, 95 ), 2 );
            piedras( 1, 200, 624, rand( 70, 95 ), 3 );
            piedras( 1, 200, 624, rand( 70, 95 ), 4 );
            piedras( 1, 200, 624, rand( 70, 95 ), 5 );
            piedras( 1, 200, 624, rand( 70, 95 ), 6 );
            piedras( 1, 200, 624, rand( 70, 95 ), 13 );
            piedras( 1, 200, 624, rand( 70, 95 ), 14 );
            piedras( 1, 200, 624, rand( 70, 95 ), 15 );
            piedras( 1, 200, 624, rand( 70, 95 ), 16 );
            piedras( 1, 200, 624, rand( 70, 95 ), 17 );
            piedras( 1, 200, 624, rand( 70, 95 ), 18 );
            piedras( 639, 200, 624, rand( 70, 95 ), 7 );
            piedras( 639, 200, 624, rand( 70, 95 ), 8 );
            piedras( 639, y, 624, rand( 70, 95 ), 9 );
            piedras( 639, 200, 624, rand( 70, 95 ), 10 );
            piedras( 639, 200, 624, rand( 70, 95 ), 11 );
            piedras( 639, 200, 624, rand( 70, 95 ), 12 );
            piedras( 639, 200, 624, rand( 70, 95 ), 19 );
            piedras( 639, 200, 624, rand( 70, 95 ), 20 );
            piedras( 639, 200, 624, rand( 70, 95 ), 21 );
            piedras( 639, 200, 624, rand( 70, 95 ), 22 );
            piedras( 639, 200, 624, rand( 70, 95 ), 23 );
            piedras( 639, 200, 624, rand( 70, 95 ), 24 );
            salida_enemigos = 1;
            enemigo_arabe( 16, 200, 1, 1, 5 );
            enemigo_arabe( 625, 200, 1, 1, 5 );
        end
        if ( y < -1500 AND salida_enemigos == 1 )
            sound_play( cristal2, 0 );
            piedras( 1, 200, 624, rand( 70, 95 ), 1 );
            piedras( 1, 200, 624, rand( 70, 95 ), 2 );
            piedras( 1, 200, 624, rand( 70, 95 ), 3 );
            piedras( 1, 200, 624, rand( 70, 95 ), 4 );
            piedras( 1, 200, 624, rand( 70, 95 ), 5 );
            piedras( 1, 200, 624, rand( 70, 95 ), 6 );
            piedras( 1, 200, 624, rand( 70, 95 ), 13 );
            piedras( 1, 200, 624, rand( 70, 95 ), 14 );
            piedras( 1, 200, 624, rand( 70, 95 ), 15 );
            piedras( 1, 200, 624, rand( 70, 95 ), 16 );
            piedras( 1, 200, 624, rand( 70, 95 ), 17 );
            piedras( 1, 200, 624, rand( 70, 95 ), 18 );
            salida_enemigos = 2;
            enemigo_arabe( 16, 200, 1, 1, 5 );
        end
        if ( y < -2000 AND salida_enemigos == 2 )
            sound_play( cristal2, 0 );
            piedras( 639, 200, 624, rand( 70, 95 ), 7 );
            piedras( 639, 200, 624, rand( 70, 95 ), 8 );
            piedras( 639, y, 624, rand( 70, 95 ), 9 );
            piedras( 639, 200, 624, rand( 70, 95 ), 10 );
            piedras( 639, 200, 624, rand( 70, 95 ), 11 );
            piedras( 639, 200, 624, rand( 70, 95 ), 12 );
            piedras( 639, 200, 624, rand( 70, 95 ), 19 );
            piedras( 639, 200, 624, rand( 70, 95 ), 20 );
            piedras( 639, 200, 624, rand( 70, 95 ), 21 );
            piedras( 639, 200, 624, rand( 70, 95 ), 22 );
            piedras( 639, 200, 624, rand( 70, 95 ), 23 );
            piedras( 639, 200, 624, rand( 70, 95 ), 24 );
            salida_enemigos = 3;
            enemigo_arabe( 625, 200, 1, 1, 5 );
        end
        if ( y < -3000 AND salida_enemigos == 3 )
            sound_play( cristal2, 0 );
            sound_play( cristal2, 0 );
            piedras( 1, 200, 624, rand( 70, 95 ), 1 );
            piedras( 1, 200, 624, rand( 70, 95 ), 2 );
            piedras( 1, 200, 624, rand( 70, 95 ), 3 );
            piedras( 1, 200, 624, rand( 70, 95 ), 4 );
            piedras( 1, 200, 624, rand( 70, 95 ), 5 );
            piedras( 1, 200, 624, rand( 70, 95 ), 6 );
            piedras( 1, 200, 624, rand( 70, 95 ), 13 );
            piedras( 1, 200, 624, rand( 70, 95 ), 14 );
            piedras( 1, 200, 624, rand( 70, 95 ), 15 );
            piedras( 1, 200, 624, rand( 70, 95 ), 16 );
            piedras( 1, 200, 624, rand( 70, 95 ), 17 );
            piedras( 1, 200, 624, rand( 70, 95 ), 18 );
            piedras( 639, 200, 624, rand( 70, 95 ), 7 );
            piedras( 639, 200, 624, rand( 70, 95 ), 8 );
            piedras( 639, y, 624, rand( 70, 95 ), 9 );
            piedras( 639, 200, 624, rand( 70, 95 ), 10 );
            piedras( 639, 200, 624, rand( 70, 95 ), 11 );
            piedras( 639, 200, 624, rand( 70, 95 ), 12 );
            piedras( 639, 200, 624, rand( 70, 95 ), 19 );
            piedras( 639, 200, 624, rand( 70, 95 ), 20 );
            piedras( 639, 200, 624, rand( 70, 95 ), 21 );
            piedras( 639, 200, 624, rand( 70, 95 ), 22 );
            piedras( 639, 200, 624, rand( 70, 95 ), 23 );
            piedras( 639, 200, 624, rand( 70, 95 ), 24 );
            salida_enemigos = 4;
            enemigo_arabe( 16, 200, 1, 1, 5 );
            enemigo_arabe( 625, 200, 1, 1, 5 );
        end
        frame;
    end
end


PROCESS descenso_camara1();
BEGIN
    file = scrolls;
    graph = 4;
    x = 320;
    y = 0;
    z = 100;
    flags = 128;
    LOOP
        if ( !exists( father ))
            return;
        end
        y = father.y / 2;
        frame;
    end
end


PROCESS ascensOR ();
BEGIN
    ctype = c_scroll;
    file = scrolls;
    graph = 10;
    x = 320;
    y = 446;
    z = -4;
    LOOP
        frame;
    end
end


PROCESS fase2_c();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
    int una_vez;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=1; restore_type=-1;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( x_p1 => limite_pantalla -5 AND una_vez == 0 )
            enemigos_tierra = 0;
            una_vez = 1;
        end
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            cambio_musica = 1;
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll9.fpg" );
            // Cambia cancion si hace falta
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 1; // aki va un 1
            // Inicia controladores de juego
            estado_fase = 9;
            limite_pantalla = 5995;
            controlador_principal();
            // inicia los 2 scroll
            scroll_start( 0, scrolls, 3, scrolls, 1, 0, 6 );
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/fase3.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "incompleto";
            // situacion de la fase, numero enemigos
            if ( cantidadificultad == 1 ) // en el nivel f cil
                elicopteros = 6;
                elicoptero2( 2000, 100, 200, fase1, 1 );
                lancha( 1000, 355 );
                enemigos_tierra = 20;
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_dispara( 1286, 289, fase1, 1 );
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 2 ) // en el nivel normal
                elicopteros = 8;
                elicoptero2( 2000, 100, 200, fase1, 1 );
                lancha( 1000, 355 );
                enemigos_tierra = 30;
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_dispara( 1261, 289, fase1, 1 );
                enemigo_dispara( 1261, 289, fase1, 1 );
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 3 ) // en el nivel dificil
                elicopteros = 10;
                elicoptero2( 2000, 100, 200, fase1, 1 );
                enemigos_tierra = 40;
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_arabe( 1761, 420, 1, 1, 1 );
                enemigo_dispara( 1261, 289, fase1, 1 );
                enemigo_dispara( 1261, 289, fase1, 1 );
                lancha( 1000, 355 );
                tankes = 2;
                camiones = 1;
            end
            // enemigos de la pantalla (si hay)
            // objetos , escenario
            items( 184, 122, 1 );
            items( 36, 302, 9 );
            items( 622, 95, 9 );
            items( 1326, 56, 9 );
            items( 2090, 91, 9 );
            items( 4734, 316, 9 );
            items( 4299, 59, 9 );
            items( 5111, 307, 9 );
            items( 5552, 77, 9 );
            items( 2274, 135, 7 );
            items( 4710, 298, 6 );
            items( 5686, 310, 9 );
            cartel_fin( 5400, 302 );
            fogon_programado();
            espejado( 3000, 440, fase1, 250, 3 );
            agua( 320, 435, fase1, 15, 1 );
            agua_reflejo( x_p1, 440, fase1, 23, 2 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        if ( x_p1 => limite_pantalla -5 AND carga_todo == 0 )
            fase2_d(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS fase2_d();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=0; restore_type=0;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll10.fpg" );
            // Cambia cancion si hace falta
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 1; // aki va un uno
            // Inicia controladores de juego
            estado_fase = 10;
            limite_pantalla = 635;
            controlador_principal();
            // inicia los 2 scroll
            scroll_start( 0, scrolls, 3, scrolls, 1, 0, 6 );
            scroll.z = 50;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/monstro.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "-";
            // situacion de la fase, numero enemigos
            if ( cantidadificultad == 1 ) // en el nivel f cil
                enemigos_tierra = 0;
                elicopteros = 0;
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 2 ) // en el nivel normal
                enemigos_tierra = 0;
                elicopteros = 0;
                tankes = 1;
                camiones = 1;
            end
            if ( cantidadificultad == 3 ) // en el nivel dificil
                enemigos_tierra = 0;
                elicopteros = 0;
                tankes = 2;
                camiones = 1;
            end
            // enemigos de la pantalla (si hay)
            // objetos , escenario
            items( 47, 186, 1 );
            pared_atras( 320, 167 );
            tapa_fondo( 3 );
            barril_sorpresa();
            espejado( 319, 440, fase1, 251, 3 );
            agua( 320, 435, fase1, 15, 1 );
            agua_reflejo( x_p1, 440, fase1, 23, 2 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        frame;
    end
end


PROCESS fogon_programado();
PRIVATE
    int fogon1 = 50;
    int p_fogon1;
    int fogon2 = 60;
    int p_fogon2;
    int fogon3 = 50;
    int p_fogon3;
    int fogon4 = 80;
    int p_fogon4;
    int fogon5 = 70;
    int p_fogon5;
BEGIN
    ctype = c_scroll;
    file = fase1;
    graph = 276;
    x = 4624;
    y = 270;
    LOOP
        if ( x_p1 + 500 > 4624 )
            if ( x_p1 < 4924 )
                p_fogon1++;
            end
        end
        if ( x_p1 + 500 > 4674 )
            if ( x_p1 < 4974 )
                p_fogon2++;
            end
        end
        if ( x_p1 + 500 > 5297 )
            if ( x_p1 < 5597 )
                p_fogon3++;
            end
        end
        if ( x_p1 + 500 > 5455 )
            if ( x_p1 < 5755 )
                p_fogon4++;
            end
        end
        if ( x_p1 + 500 > 5855 )
            if ( x_p1 < 6055 )
                p_fogon5++;
            end
        end
        if ( p_fogon1 => fogon1 )
            p_fogon1 = 0;
            fogon( 4604, 270, 1 );
            sound_play( lanzalla, 0 );
        end
        if ( p_fogon2 => fogon2 )
            p_fogon2 = 0;
            fogon( 4684, 104, 0 );
            sound_play( lanzalla, 0 );
        end
        if ( p_fogon3 => fogon3 )
            p_fogon3 = 0;
            fogon( 5287, 267, 1 );
            sound_play( lanzalla, 0 );
        end
        if ( p_fogon4 => fogon4 )
            p_fogon4 = 0;
            fogon( 5445, 25, 1 );
            sound_play( lanzalla, 0 );
        end
        if ( p_fogon5 => fogon5 )
            p_fogon5 = 0;
            fogon( 5845, 221, 1 );
            sound_play( lanzalla, 0 );
        end
        frame;
    end
end


PROCESS barril_sorpresa();
BEGIN
    file = fase1;
    ctype = c_scroll;
    x = 320;
    y = 220;
    graph = 275;
    z = -4;
    LOOP
        if ( collision( type bala ) OR collision( type escopetazo ) OR collision( type llamazo ) OR collision( type explosion ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            monstruo2( 320, 200 );
            piedras( x -200, y -100, 623, rand( 70, 95 ), 1 );
            piedras( x -100, y -100, 623, rand( 70, 95 ), 2 );
            piedras( x, y -100, 623, rand( 70, 95 ), 3 );
            piedras( x + 100, y -100, 623, rand( 70, 95 ), 4 );
            piedras( x + 200, y -100, 623, rand( 70, 95 ), 5 );
            piedras( x -200, y, 623, rand( 70, 95 ), 6 );
            piedras( x -100, y, 623, rand( 70, 95 ), 7 );
            piedras( x, y, 623, rand( 70, 95 ), 8 );
            piedras( x + 100, y, 623, rand( 70, 95 ), 9 );
            piedras( x + 200, y, 623, rand( 70, 95 ), 10 );
            piedras( x -200, y + 100, 623, rand( 70, 95 ), 11 );
            piedras( x -100, y + 100, 623, rand( 70, 95 ), 12 );
            piedras( x, y + 100, 623, rand( 70, 95 ), 13 );
            piedras( x + 100, y + 100, 623, rand( 70, 95 ), 14 );
            piedras( x + 200, y + 100, 623, rand( 70, 95 ), 15 );
            piedras( x, y, 623, rand( 70, 95 ), 16 );
            piedras( x, y, 623, rand( 70, 95 ), 17 );
            piedras( x, y, 623, rand( 70, 95 ), 18 );
            piedras( x, y, 623, rand( 70, 95 ), 19 );
            piedras( x, y, 623, rand( 70, 95 ), 20 );
            piedras( x, y, 623, rand( 70, 95 ), 21 );
            piedras( x, y, 623, rand( 70, 95 ), 22 );
            piedras( x, y, 623, rand( 70, 95 ), 23 );
            piedras( x, y, 623, rand( 70, 95 ), 24 );
            signal( type pared_atras, s_kill );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS fase3_nieve();
PRIVATE
    int una_vez;
    int k_hora = 3;
BEGIN
    // mato y kallo a too , pongo nueva velocidad
    let_me_alone();
    clear_screen();
    fade_off( 0 );
    sound_stop( all_sound );
    music_stop();
    write_delete( all_text );
    scroll_stop( 0 );
    // dump_type=1; restore_type=-1;
    set_fps( velocidad_juego, 4 );
    cambio_musica = 1;
    // carga escenario
    scrolls = fpg_load( "fpg/scroll22.fpg" );
    fase1 = fpg_load( "fpg/fase6.fpg" );
    // Carga cancion del escenario
    s_fase1 = music_load( "sound/fase6.mp3" );
    // Enciende pantalla cuando cargue
    fade_on( 0 );
    // Hace reflejos de todo lo que ve
    x_p1 = 0;
    haz_reflejos = 0;
    // Restaurar los valores iniciales
    limite_vida_player1 = vida_total;
    arma_seleccionada_p1 = 1;
    arma_seleccionada_p2 = 1;
    realiza_intro1 = 0;
    // efectos al empezar
    abre_fase( 1 );
    abre_fase( 2 );
    numero_pantalla( 224 );
    juego_en_pausa = 1;
    pausa_en_pantalla = 1;
    // Inicia controladores de juego
    estado_fase = 22;
    limite_pantalla = 24995;
    controlador_principal();
    // inicia los 2 scroll
    // tiempo actual 1= noche, 2= tarde, 12= dia,
    if ( modo_wiz == 1 )
        tiempo_actual = 12;
    end
    if ( tiempo_actual == 1 )
        k_hora = 5;
    end
    scroll_start( 0, scrolls, k_hora, scrolls, tiempo_actual, 0, 5 );
    scroll.z = 50;
    scroll.ratio = 400;
    // Indica a todos los objetos k suelo deben tocar
    ke_fase_es = scrolls;
    ke_grafico_es = 4;
    //ke_color_es = 0x8cfcf8ff; // 110;
    // permite que las explosiones destrocen el escenario
    grafico_origen1 = 5;
    grafico_origen2 = 6;
    // objetivos de la fase
    objetivos_a = "-";
    objetivos_a1 = 0;
    objetivos_b = "-";
    objetivos_c = "-";
    objetos_regalo = 0;
    regalos = 0;
    // situacion de la fase, numero enemigos
    enemigos_tierra = 1;
    elicopteros = 0;
    // Fin fase
    // enemigos de la pantalla (si hay)
    // Elementos de la pantalla
    musica_fase = music_play( s_fase1, -1 );
    music_set_volume( vol );
    // signal(type nave,s_freeze);
    LOOP
        frame;
    end
end


PROCESS nave( double x, double y );
PRIVATE
    int esperate;
    int esperate2;
    int estado4;
BEGIN
    file = fase1;
    graph = 13;
    z = -5;
    LOOP
        // brillo al disparar
        if ( esperate2 == 3 )
            graph = 17;
        end
        if ( esperate2 == 1 )
            graph = 13;
        end
        if ( limite_vida_player1 =< 0 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            gameover();
            signal( type camara_nave, s_freeze );
            signal( id, s_kill );
        end
        if ( estado4 == 0 )
            if ( collision( type disparo_nave_enemiga ) OR collision( type explosion ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                limite_vida_player1--;
                sound_play( smetal1, 0, 21 );
            end
        end
        // al ser tocado
        if ( estado4 > 0 )
            estado4++;
        end
        if ( estado4 == 1 )
            graph = 16;
        end
        if ( estado4 == 2 )
            graph = 13;
        end
        if ( estado4 == 3 )
            graph = 16;
        end
        if ( estado4 == 4 )
            graph = 13;
            estado4 = 0;
        end
        // no deja slir de pantalla
        if ( esperate > 0 )
            esperate--;
        end
        if ( esperate2 > 0 )
            esperate2--;
        end
        // si llevas teclado
        if ( tipo_control1 == 1 )
            if ( pulsa_arriba > 0 )
                y -= 6;
            end
            if ( pulsa_abajo > 0 )
                y += 6;
            end
            if ( pulsa_izq > 0 )
                x -= 6;
            end
            if ( pulsa_der > 0 )
                x += 6;
            end
            if ( pulsa_c > 0 AND esperate == 0 )
                disparo_nave( x + 45, y + 20 );
                sound_play( pistola1, 0 );
                esperate = 5;
                esperate2 = 3;
            end
        end
        if ( x < 77 )
            x = 77;
        end
        if ( x > 558 )
            x = 558;
        end
        if ( y < 70 )
            y = 70;
        end
        if ( y > 429 )
            y = 429;
        end
        frame;
    end
end


PROCESS platillo();
BEGIN
    x = -300;
    y = 480;
    file = fase1;
    graph = 241;
    size = 70;
    LOOP
        x += 15;
        y -= 10;
        if ( x > 900 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS nave_enemiga( double y );
PRIVATE
    int si_ono;
    int estado4;
    int vida_nave = 5;
    int dispara;
    int cuando_dispara = 10;
    int tipo_enemigo;
BEGIN
    x = 750;
    file = fase1;
    graph = 303;
    tipo_enemigo = rand( 1, 3 );
    si_ono = rand( 1, 2 );
    if ( si_ono == 1 )
        y = rand( 70, 380 );
    end
    LOOP
        x -= 6;
        dispara++;
        if ( tipo_enemigo == 2 )
            y--;
        end
        if ( tipo_enemigo == 1 )
            y += 2;
        end
        if ( vida_nave =< 0 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            sound_play( explo3, 0, 23 );
            cacho_heli( x, y, 304, fase1, 1 );
            cacho_heli( x, y, 305, fase1, 2 );
            cacho_heli( x, y, 306, fase1, 3 );
            cacho_heli( x, y, 307, fase1, 4 );
            puntuacion += 300;
            clones_eliminados++;
            signal( id, s_kill );
        end
        if ( dispara => cuando_dispara )
            disparo_nave_enemiga( x -40, y + 50 );
            sound_play( pistola, 0, 17 );
            dispara = 0;
            cuando_dispara = rand( 40, 80 );
        end
        if ( estado4 == 0 )
            if ( collision( type disparo_nave ) OR collision( type explosion ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_nave--;
                sound_play( smetal1, 0, 21 );
            end
        end
        // al ser tocado
        if ( estado4 > 0 )
            estado4++;
        end
        if ( estado4 == 1 )
            graph = 308;
        end
        if ( estado4 == 2 )
            graph = 303;
        end
        if ( estado4 == 3 )
            graph = 308;
        end
        if ( estado4 == 4 )
            graph = 303;
            estado4 = 0;
        end
        if ( x < -200 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS disparo_nave( double x, double y );
PRIVATE
    int balafuera;
BEGIN
    z = -6;
    graph = 257;
    LOOP
        x += 14;
        if ( balafuera == 1 )
            signal( id, s_kill );
        end
        if ( collision( type nave_enemiga ))
            balafuera = 1;
        end
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS disparo_nave_enemiga( double x, double y );
PRIVATE
    int balafuera;
BEGIN
    z = -6;
    graph = 341;
    LOOP
        x -= 14;
        if ( balafuera == 1 )
            signal( id, s_kill );
        end
        if ( collision( type nave ))
            balafuera = 1;
        end
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS camara_nave();
PRIVATE
    int esperate;
    int programa_vuelo;
    int repite;
    int crea_naves = -50;
    int cuando_crea = 50;
    int ves_contando;
BEGIN
    y = 240;
    x = 320;
    graph = 1;
    ctype = c_scroll;
    scroll.camera = 0;
    LOOP
        scroll.x1++;
        programa_vuelo++;
        scroll.x0 += 2;
        // dump_type=1; restore_type=-1;
        if ( programa_vuelo => 15 AND repite < 80 )
            scroll.y0 += 2;
            scroll.y1++;
            programa_vuelo = 0;
            repite++;
        end
        if ( programa_vuelo => 15 AND repite => 80 AND repite < 140 )
            scroll.y0 -= 2;
            scroll.y1--;
            programa_vuelo = 0;
            repite++;
        end
        if ( programa_vuelo => 15 AND repite => 140 AND repite < 200 )
            scroll.y0 += 2;
            scroll.y1++;
            programa_vuelo = 0;
            repite++;
        end
        // va creando enemigos
        if ( repite < 170 )
            crea_naves++;
            ves_contando++;
            if ( ves_contando == 1000 )
                platillo();
            end
            if ( crea_naves => cuando_crea )
                nave_enemiga( y );
                crea_naves = 0;
                cuando_crea = rand( 30, 70 );
            end
        end
        if ( repite => 182 )
            fase3_a(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS fase3_a();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
    int una_vez;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    // dump_type=1; restore_type=-1;
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        /*
if (x_p1>limite_pantalla-640 AND una_vez==0)
if (cantidadificultad==1)   // en el nivel f cil
enemigos_tierra=15;
elicopteros=4;
end
if (cantidadificultad==2)   // en el nivel normal
enemigos_tierra=20;
elicopteros=4;
end
if (cantidadificultad==3)   // en el nivel dificil
enemigos_tierra=25;
elicopteros=4;
end
una_vez=1; end
*/
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            cambio_musica = 1;
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll23.fpg" );
            // Cambia cancion si hace falta
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // Inicia controladores de juego
            estado_fase = 23;
            limite_pantalla = 4995;
            controlador_principal();
            // inicia los 2 scroll
            if ( modo_wiz == 1 )
                tiempo_actual = 2;
            end
            scroll_start( 0, scrolls, 3, scrolls, tiempo_actual, 0, 6 );
            scroll.z = 50;
            scroll.ratio = 400;
            if ( modo_wiz == 0 )
                tercer_scroll( 320, 357, 51 );
                tapa_scroll( 2500, 358, 50 ); // x , y , graph
            end
            crea_nieve();
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/fase4.mp3" );
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "incompleto";
            // situacion de la fase, numero enemigos
            enemigos_tierra = 10;
            // enemigos de la pantalla (si hay)
            items( 2453, 39, 1 );
            cartel_fin( 4699, 420 );
            items( 2744, 291, 11 );
            items( 4801, 23, 4 );
            items( 4022, 265, 8 );
            items( 2471, 268, 27 );
            items( 3069, 109, 27 );
            items( 3583, 235, 27 );
            items( 3864, 236, 27 );
            items( 3709, 324, 27 );
            items( 3711, 135, 27 );
            items( 4556, 153, 27 );
            items( 4556, 183, 27 );
            items( 4556, 215, 27 );
            items( 4556, 252, 27 );
            items( 4556, 286, 27 );
            items( 4603, 164, 27 );
            items( 4603, 273, 27 );
            items( 4641, 203, 27 );
            items( 4641, 241, 27 );
            items( 4720, 145, 27 );
            items( 4720, 186, 27 );
            items( 4720, 219, 27 );
            items( 4720, 253, 27 );
            items( 4720, 283, 27 );
            items( 4775, 149, 27 );
            items( 4894, 149, 27 );
            items( 4790, 187, 27 );
            items( 4872, 187, 27 );
            items( 4803, 232, 27 );
            items( 4854, 232, 27 );
            items( 4827, 282, 27 );
            items( 464, 218, 27 );
            items( 764, 222, 27 );
            items( 1002, 194, 27 );
            items( 160, 260, 12 );
            // viga(2248,190);
            // objetos , escenario
            primer_plano( 292, 1, fase1, 87 ); // graficos por delante del scroll
            primer_plano( 292, 1, fase1, 983 );
            primer_plano( 292, 1, fase1, 1511 );
            primer_plano( 412, 2, fase1, 2037 );
            primer_plano( 292, 1, fase1, 2527 );
            primer_plano( 292, 1, fase1, 3083 );
            primer_plano( 412, 2, fase1, 3526 );
            primer_plano( 292, 1, fase1, 4545 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        if ( x_p1 => limite_pantalla -5 AND carga_todo == 0 )
            fase3_b(); // carga la siguiente escena
            return;
        end
        frame;
    end
end


PROCESS viga( double x, double y );
BEGIN
    ctype = c_scroll;
    z = -4;
    file = fase1;
    graph = 15;
    LOOP
        frame;
    end
end


PROCESS fase3_b();
PRIVATE
    int Esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    abre_fase( 3 );
    abre_fase( 4 );
    if ( reiniciando == 1 )
        esperate = 0;
        reiniciando = 0;
    end
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll24.fpg" );
            // Cambia cancion si hace falta
            // Inicia controladores de juego
            estado_fase = 24;
            limite_pantalla = 635;
            controlador_principal();
            // inicia los 2 scroll
            if ( modo_wiz == 1 )
                tiempo_actual = 1;
            end
            scroll_start( 0, scrolls, 3, scrolls, tiempo_actual, 0, 0 );
            scroll.z = 50;
            scroll.ratio = 400;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/monstro.mp3" );
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "-";
            // situacion de la fase, numero enemigos
            // enemigos
            enemigos_tierra = 70;
            prepara_piedras();
            // Fin de la fase
            // obstaculos , pinchos, etc
            // objetos , escenario
            items( 62, 297, 1 );
            items( 590, 297, 6 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        frame;
    end
end


PROCESS Prepara_piedras();
PRIVATE
    int espera;
    int cuanto_espera;
BEGIN
    cuanto_espera = rand( 50, 100 );
    LOOP
        espera++;
        if ( espera => cuanto_espera )
            cuanto_espera = rand( 20, 60 );
            espera = 0;
            sound_play( bazoka, 0 );
            pedrazo( rand( 50, 600 ), y_p1 -500 );
        end
        if ( y_p1 < 440 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS pedrazo( double x, double y );
PRIVATE
    double y_incr;
BEGIN
    ctype = c_scroll;
    file = fase1;
    graph = 249;
    LOOP
        y += y_incr;
        angle += 10000;
        if ( y_incr < 15 )
            y_incr++;
        end
        if ( collision( type bala ) OR collision( type llamazo ) OR collision( type escopetazo ) OR collision( type explosion ))
            sound_play( dish, 0 );
            piedras( x, y, 656, rand( 70, 95 ), 1 );
            piedras( x, y, 656, rand( 70, 95 ), 2 );
            piedras( x, y, 656, rand( 70, 95 ), 3 );
            piedras( x, y, 656, rand( 70, 95 ), 4 );
            piedras( x, y, 656, rand( 70, 95 ), 5 );
            piedras( x, y, 656, rand( 70, 95 ), 6 );
            piedras( x, y, 656, rand( 70, 95 ), 7 );
            piedras( x, y, 656, rand( 70, 95 ), 8 );
            piedras( x, y, 656, rand( 70, 95 ), 9 );
            piedras( x, y, 656, rand( 70, 95 ), 10 );
            piedras( x, y, 656, rand( 70, 95 ), 11 );
            piedras( x, y, 656, rand( 70, 95 ), 12 );
            piedras( x, y, 656, rand( 70, 95 ), 13 );
            piedras( x, y, 656, rand( 70, 95 ), 14 );
            piedras( x, y, 656, rand( 70, 95 ), 15 );
            piedras( x, y, 656, rand( 70, 95 ), 16 );
            piedras( x, y, 656, rand( 70, 95 ), 17 );
            piedras( x, y, 656, rand( 70, 95 ), 18 );
            piedras( x, y, 656, rand( 70, 95 ), 19 );
            piedras( x, y, 656, rand( 70, 95 ), 20 );
            piedras( x, y, 656, rand( 70, 95 ), 21 );
            piedras( x, y, 656, rand( 70, 95 ), 22 );
            piedras( x, y, 656, rand( 70, 95 ), 23 );
            piedras( x, y, 656, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y > y_p1 )
            sound_play( dish, 0 );
            piedras( x, y, 656, rand( 70, 95 ), 1 );
            piedras( x, y, 656, rand( 70, 95 ), 2 );
            piedras( x, y, 656, rand( 70, 95 ), 3 );
            piedras( x, y, 656, rand( 70, 95 ), 4 );
            piedras( x, y, 656, rand( 70, 95 ), 5 );
            piedras( x, y, 656, rand( 70, 95 ), 6 );
            piedras( x, y, 656, rand( 70, 95 ), 7 );
            piedras( x, y, 656, rand( 70, 95 ), 8 );
            piedras( x, y, 656, rand( 70, 95 ), 9 );
            piedras( x, y, 656, rand( 70, 95 ), 10 );
            piedras( x, y, 656, rand( 70, 95 ), 11 );
            piedras( x, y, 656, rand( 70, 95 ), 12 );
            piedras( x, y, 656, rand( 70, 95 ), 13 );
            piedras( x, y, 656, rand( 70, 95 ), 14 );
            piedras( x, y, 656, rand( 70, 95 ), 15 );
            piedras( x, y, 656, rand( 70, 95 ), 16 );
            piedras( x, y, 656, rand( 70, 95 ), 17 );
            piedras( x, y, 656, rand( 70, 95 ), 18 );
            piedras( x, y, 656, rand( 70, 95 ), 19 );
            piedras( x, y, 656, rand( 70, 95 ), 20 );
            piedras( x, y, 656, rand( 70, 95 ), 21 );
            piedras( x, y, 656, rand( 70, 95 ), 22 );
            piedras( x, y, 656, rand( 70, 95 ), 23 );
            piedras( x, y, 656, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        if ( collision( type jose_player1 ))
            josel_tocao = 1;
            sound_play( dish, 0 );
            piedras( x, y, 656, rand( 70, 95 ), 1 );
            piedras( x, y, 656, rand( 70, 95 ), 2 );
            piedras( x, y, 656, rand( 70, 95 ), 3 );
            piedras( x, y, 656, rand( 70, 95 ), 4 );
            piedras( x, y, 656, rand( 70, 95 ), 5 );
            piedras( x, y, 656, rand( 70, 95 ), 6 );
            piedras( x, y, 656, rand( 70, 95 ), 7 );
            piedras( x, y, 656, rand( 70, 95 ), 8 );
            piedras( x, y, 656, rand( 70, 95 ), 9 );
            piedras( x, y, 656, rand( 70, 95 ), 10 );
            piedras( x, y, 656, rand( 70, 95 ), 11 );
            piedras( x, y, 656, rand( 70, 95 ), 12 );
            piedras( x, y, 656, rand( 70, 95 ), 13 );
            piedras( x, y, 656, rand( 70, 95 ), 14 );
            piedras( x, y, 656, rand( 70, 95 ), 15 );
            piedras( x, y, 656, rand( 70, 95 ), 16 );
            piedras( x, y, 656, rand( 70, 95 ), 17 );
            piedras( x, y, 656, rand( 70, 95 ), 18 );
            piedras( x, y, 656, rand( 70, 95 ), 19 );
            piedras( x, y, 656, rand( 70, 95 ), 20 );
            piedras( x, y, 656, rand( 70, 95 ), 21 );
            piedras( x, y, 656, rand( 70, 95 ), 22 );
            piedras( x, y, 656, rand( 70, 95 ), 23 );
            piedras( x, y, 656, rand( 70, 95 ), 24 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS fase3_c();
PRIVATE
    int Esperate = 2; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    // abre_fase(3); abre_fase(4);
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // Optimizando juego, descargando y cargando nuevos scroll
            fpg_unload( scrolls );
            scrolls = fpg_load( "fpg/scroll27.fpg" );
            // Cambia cancion si hace falta
            // Inicia controladores de juego
            estado_fase = 25;
            limite_pantalla = 635;
            controlador_principal();
            // inicia los 2 scroll
            scroll_start( 0, scrolls, 3, scrolls, 1, 0, 4 );
            scroll.z = 50;
            scroll.ratio = 400;
            // Indica a todos los objetos k suelo deben tocar
            ke_fase_es = scrolls;
            ke_grafico_es = 5;
            //ke_color_es = 0x8cfcf8ff; // 110;
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            music_unload( s_fase1 );
            s_fase1 = music_load( "sound/fase7.mp3" );
            // permite que las explosiones destrocen el escenario
            grafico_origen1 = 94;
            grafico_origen2 = 99;
            // Hace reflejos de todo lo que ve
            x_p1 = 0;
            haz_reflejos = 0;
            // objetivos de la fase
            objetivos_a = "incompleto";
            objetivos_a1 = 0;
            objetivos_b = "-";
            // situacion de la fase, numero enemigos
            // enemigos
            nave_inversa();
            enemigos_tierra = 1;
            // Fin de la fase
            // obstaculos , pinchos, etc
            // objetos , escenario
            tapa_fondo( 3 );
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            if ( reiniciando == 1 )
                abre_fase( 8 );
                abre_fase( 9 );
            end
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        frame;
    end
end


PROCESS tapa_fondo( int graph );
BEGIN
    ctype = c_scroll;
    file = scrolls;
    z = -1;
    x = 320;
    y = 240;
    LOOP
        frame;
    end
end


PROCESS crea_nieve();
PRIVATE
    int cuenta_alea;
    int alea;
BEGIN
    ctype = c_scroll;
    graph = 1;
    alea = rand( 10, 20 );
    LOOP
        cuenta_alea++;
        if ( cuenta_alea => alea AND copos_nieve < 50 )
            cuenta_alea = 0;
            alea = rand( 10, 20 );
            copo_nieve( x_p1 + rand( -500, 500 ), -10, 500 );
            copo_nieve( x_p1 + rand( -500, 500 ), -10, 500 );
        end
        frame;
    end
end


PROCESS copo_nieve( double x, double y, double y_total );
PRIVATE
    int estado;
    int subida;
BEGIN
    graph = 608;
    z = -6;
    ctype = c_scroll;
    size = rand( 60, 95 );
    subida = rand( 1, 2 );
    copos_nieve++;
    LOOP
        y += subida;
        estado++;
        if ( y >= y_total )
            copos_nieve--;
            signal( id, s_kill );
        end
        if ( estado <= 5 )
            x--;
        end
        if ( estado > 5 )
            x++;
        end
        if ( estado => 10 )
            estado = 0;
        end
        frame;
    end
end


PROCESS pared_atras( double x, double y );
BEGIN
    ctype = c_scroll;
    file = scrolls;
    z = -2;
    graph = 6;
    LOOP
        frame;
    end
end


PROCESS parte_esc( double x, double y, int file, int graph, int tipo );
BEGIN
    ctype = c_scroll;
    if ( tipo == 2 )
        flags = 1;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        frame;
    end
end


PROCESS plataforma( double x, double y, int file, int graph, int tipo );
PRIVATE
    int estado = 1;
BEGIN
    ctype = c_scroll;
    flags = 1;
    if ( tipo == 1 )
        z = -12;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( estado == 1 AND y < 319 )
            y++;
            angle -= 250;
        end
        if ( estado == 2 AND y > 309 )
            y--;
            angle += 250;
        end
        if ( y => 319 )
            estado = 2;
        end
        if ( y =< 309 )
            estado = 1;
        end
        frame;
    end
end

// bola que cae y deja un gusano

PROCESS bola_pinchos( double x, double y );
PRIVATE
    int incr_y;
    int estado;
BEGIN
    ctype = c_scroll;
    z = -7;
    graph = 185;
    file = fase1;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( x_p1 + 35 > x )
            estado = 1;
        end
        if ( estado == 1 )
            incr_y++;
            y += incr_y;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es OR y > 480 )
            gusano( x, y, 190, 4 );
            gusano( x, y, 190, -4 );
            gusano( x, y, 191, 0 );
            sound_play( dish, 0 );
            signal( id, s_kill );
        end
        if ( collision( type jose_player1 ))
            josel_tocao = 1;
            gusano( x, y, 190, 4 );
            gusano( x, y, 190, -4 );
            gusano( x, y, 191, 0 );
            sound_play( dish, 0 );
            signal( id, s_kill );
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS gusano( double x, double y, int graph, int incr_x );
PRIVATE
    int incr_y = -8;
BEGIN
    ctype = c_scroll;
    z = -7;
    file = fase1;
    if ( graph == 191 )
        size = 80;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        incr_y++;
        y += incr_y;
        x += incr_x;
        angle += 20000;
        if ( y > 480 )
            signal( id, s_kill );
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS barco( double x, double y, int file, int graph, int tipo );
PRIVATE
    int estado = 1;
BEGIN
    ctype = c_scroll;
    flags = 1;
    z = -4;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( estado == 1 AND y < 319 )
            y++;
            angle -= 250;
        end
        if ( estado == 2 AND y > 309 )
            y--;
            angle += 250;
        end
        if ( y => 319 )
            estado = 2;
        end
        if ( y =< 309 )
            estado = 1;
        end
        frame;
    end
end


PROCESS agua( double x, double y, int file, int graph, int tipo );
PRIVATE
    int estado = 1;
    int alea;
    int cuenta_alea;
    int agua_lenta;
BEGIN
    ctype = c_scroll;
    alpha = 128;
    z = -7;
    alea = rand( 25, 75 );
    LOOP
        x = x_p1;
        cuenta_alea++;
        agua_lenta++;
        if ( cuenta_alea == alea )
            cuenta_alea = 0;
            alea = rand( 25, 75 );
            burbuja( x_p1 + rand( -320, 320 ), 490, 390 );
            burbuja( x_p1 + rand( -320, 320 ), 500, 390 );
        end
        if ( agua_lenta == 5 )
            if ( estado == 1 AND y < 445 )
                y++;
                agua_lenta = 0;
            end
            if ( estado == 2 AND y > 435 )
                y--;
                agua_lenta = 0;
            end
        end
        if ( y => 445 )
            estado = 2;
        end
        if ( y =< 435 )
            estado = 1;
        end
        frame;
    end
end


PROCESS burbuja( double x, double y, double y_total );
PRIVATE
    int estado;
    int subida;
BEGIN
    graph = 404;
    alpha = 128;
    z = -6;
    ctype = c_scroll;
    size = 90;
    subida = rand( 1, 3 );
    LOOP
        y -= subida;
        estado++;
        if ( y <= y_total )
            signal( id, s_kill );
        end
        if ( estado <= 5 )
            x--;
        end
        if ( estado > 5 )
            x++;
        end
        if ( estado => 10 )
            estado = 0;
        end
        frame;
    end
end


PROCESS onda_agua( double x, double y, int tipo );
BEGIN
    y = 373;
    graph = 399;
    size = 29;
    alpha = 128;
    ctype = c_scroll;
    z = -6;
    y += 20;
    if ( tipo > 140 )
        y += 20;
    end
    LOOP
        if ( size <= 100 )
            size += 3;
        else
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS salpicada( double x, double y, double size );
PRIVATE
    int haz_graficos;
BEGIN
    y = 395;
    graph = 400;
    alpha = 128;
    ctype = c_scroll;
    z = -7;
    LOOP
        haz_graficos++;
        if ( haz_graficos == 0 )
            graph = 400;
            onda_agua( x, y, size );
        end
        if ( haz_graficos == 2 )
            graph = 401;
        end
        if ( haz_graficos == 4 )
            graph = 402;
        end
        if ( haz_graficos == 6 )
            graph = 403;
        end
        if ( haz_graficos == 8 )
            graph = 402;
        end
        if ( haz_graficos == 10 )
            graph = 401;
        end
        if ( haz_graficos == 12 )
            graph = 400;
            onda_agua( x, y, size );
        end
        if ( haz_graficos == 14 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS agua_reflejo( double x, double y, int file, int graph, int tipo );
BEGIN
    ctype = c_scroll;
    alpha = 128;
    z = -6;
    LOOP
        if ( tipo == 1 )
            x -= 2;
        end
        if ( tipo == 2 )
            x -= 3;
        end
        if ( x < x_p1 -640 )
            x = x_p1 + rand( 640, 1500 );
        end
        frame;
    end
end


PROCESS espejado( double x, double y, int file, int graph, int tipo );
BEGIN
    ctype = c_scroll;
    if ( tipo == 1 )
        z = -9;
    end
    if ( tipo == 2 )
        flags = 7;
        z = -8;
    end
    if ( tipo == 3 )
        flags = 6;
        z = -8;
    end
    LOOP
        frame;
    end
end

// Efecto al entrar a la fase

PROCESS abre_fase( int tipo );
PRIVATE
    int sonido_asciende;
    int tiempo_barra;
BEGIN
    if ( tipo == 10 )
        x = 320;
        y = 16;
        abre_fase( 12 );
    end
    if ( tipo == 11 )
        x = 320;
        y = 464;
    end
    if ( tipo == 1 OR tipo == 8 )
        return;
    end
    if ( tipo == 2 OR tipo == 9 )
        filtro_rapido( 4, 120 );
        filtro_rapido( 4, 360 );
        return;
    end
    if ( tipo == 3 OR tipo == 5 )
        return;
    end
    if ( tipo == 4 OR tipo == 6 )
        filtro_rapido( 3, 120 );
        filtro_rapido( 3, 360 );
        return;
    end
    graph = 314;
    z = -501;
    if ( tipo == 12 )
        x = 320;
        y = 463;
        graph = 376;
        z = -502;
    end
    LOOP
        tiempo_barra++;
        if ( tiempo_barra => 50 AND tipo == 10 )
            y -= 5;
            if ( region_out( id, 0 ))
                signal( type abre_fase, s_kill );
            end
        end
        if ( tiempo_barra => 50 AND ( tipo == 11 OR tipo == 12 ))
            y += 5;
            if ( region_out( id, 0 ))
                signal( id, s_kill );
            end
        end
        if ( tipo == 1 OR tipo == 2 )
            sonido_asciende += 10;
            if ( sonido_asciende => vol )
                sonido_asciende = vol;
            end
        end
        if ( tipo == 5 OR tipo == 6 )
            sonido_asciende -= 10;
            if ( sonido_asciende =< vol )
                sonido_asciende = vol;
            end
        end
        if ( tipo == 1 )
            y -= 3;
        end
        if ( tipo == 2 )
            y += 3;
        end
        if ( tipo == 3 )
            y += 13;
        end
        if ( tipo == 4 )
            y -= 13;
        end
        // cierra imagen con graduado de sonido rapidamente
        if ( tipo == 5 )
            y += 13;
        end
        if ( tipo == 6 )
            y -= 13;
        end
        // visualiza nueva imagen rapidamente
        if ( tipo == 8 )
            y -= 10;
        end
        if ( tipo == 9 )
            y += 10;
        end
        if ( tipo == 3 OR tipo == 5 )
            if ( y > 118 )
                signal( id, s_kill );
            end
        end
        if ( tipo == 4 OR tipo == 6 )
            if ( y < 361 )
                signal( id, s_kill );
            end
        end
        if ( region_out( id, 0 ))
            if ( tipo == 1 OR tipo == 2 OR tipo == 8 OR tipo == 9 )
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS filtro_rapido( int tipo, double y );
PRIVATE
    int estado;
    int reduce_vol;
BEGIN
    graph = 5;
    z = -715;
    if ( tipo == 4 )
        x = 280;
    else
        x = 990;
    end
    if ( tipo == 5 )
        x = 320;
        graph = 314;
        z = -499;
        size_y = 0;
    end
    reduce_vol = vol;
    LOOP
        if ( tipo == 4 )
            x += 36;
            if ( x > 2000 )
                if ( !exists( type numero_pantalla ))
                    juego_en_pausa = 0;
                    pausa_en_pantalla = 0;
                end
                return;
            end
        end
        IF ( tipo == 3 )
            juego_en_pausa = 1;
            pausa_en_pantalla = 1;
            IF ( x > 230 )
                x -= 36;
            end
            // si hay cambio de musica
            if ( cambio_musica == 1 )
                if ( reduce_vol > 8 )
                    reduce_vol -= 8;
                else
                    reduce_vol = 0;
                    cambio_musica = 0;
                end
                music_set_volume( reduce_vol );
            end
        end
        if ( tipo == 5 )
            if ( estado == 0 )
                if ( size_y < 100 )
                    size_y += 5;
                else
                    size_y = 100;
                    FRAME( 2200 );
                    estado = 1;
                end
            else
                if ( size_y > 10 )
                    size_y -= 5;
                else
                    return;
                end
            end
        end
        frame;
    end
end


PROCESS numero_pantalla( int graph );
PRIVATE
    int estado;
    int aceleracion = -30;
BEGIN
    x = 750;
    y = 240;
    z = -500;
    FRAME( 1500 );
    filtro_rapido( 5, 240 );
    juego_en_pausa = 1;
    pausa_en_pantalla = 1;
    LOOP
        if ( estado == 0 )
            aceleracion++;
            x += aceleracion;
            if ( aceleracion == 0 )
                FRAME( 1000 );
                estado = 1;
            end
        end
        if ( estado == 1 )
            aceleracion += 2;
            x -= aceleracion;
        end
        if ( x < -200 )
            msn_engine( 363, 3 );
            if ( estado_fase == 1 )
                objetivos_mision( 1 );
            end
            if ( estado_fase == 6 )
                objetivos_mision( 2 );
            end
            if ( estado_fase == 22 )
                objetivos_mision( 3 );
            end
            juego_en_pausa = 0;
            pausa_en_pantalla = 0;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS cartel_fin( double x, double y );
PRIVATE
    int estado;
    int estado2 = 1;
BEGIN
    z = -10;
    ctype = c_scroll;
    graph = 320;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( estado == 0 )
            estado2 = 1;
            angle = 0;
        end
        if ( estado > 0 )
            estado--;
        end
        if ( collision( type jose_player1 ))
            estado = 10;
        end
        if ( estado > 0 )
            if ( estado2 == 1 )
                angle -= 1000;
                if ( angle < -10000 )
                    estado2 = 2;
                end
            end
            if ( estado2 == 2 )
                angle += 1000;
                if ( angle => 10000 )
                    estado2 = 1;
                end
            end
        end
        frame;
    end
end

// FIN DE LAS FASES DEL JUEGO

PROCESS blur_efect( double x, double y, int file, int graph, int angle );
PRIVATE
    int cuenta_tiempo = 7;
BEGIN
    ctype = c_scroll;
    z = -4;
    LOOP
        cuenta_tiempo--;
        if ( alpha > 15 )
            alpha -= 15;
        end
        if ( alpha =< 15 )
            signal( id, s_kill );
        end
        frame;
    end
end

// PERSONAJE JOSEL !!

PROCESS imita_bala();
PRIVATE
    int calculos;
    int calculos2;
    int keflags;
BEGIN
    ctype = c_scroll;
    z = -9;
    file = father.file;
    priority = -120;
    LOOP
        if ( !exists( father ))
            return;
        end
        file = father.file;
        x = father.x;
        graph = father.graph;
        size = father.size;
        keflags = father.flags;
        angle = father.angle;
        calculos = 385 - father.y;
        calculos2 = 385 + calculos;
        if ( calculos2 => 385 )
            y = calculos2;
        end
        if ( calculos2 < 385 )
            y = 550;
        end
        if ( keflags == 0 )
            flags = 6;
        end
        if ( keflags == 1 )
            flags = 7;
        end
        if ( keflags == 4 )
            alpha = 128;
        end
        if ( keflags == 5 )
            flags = 5;
        end
        if ( keflags == 16 )
            flags = 16;
        end
        if ( x > x_p1 + 640 )
            signal( id, s_kill );
        end
        if ( x < x_p1 -640 )
            signal( id, s_kill );
        end
        if ( y > 600 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS imita_2();
PRIVATE
    int calculos;
    int calculos2;
    int keflags;
BEGIN
    ctype = c_scroll;
    z = -9;
    file = father.file;
    priority = -120;
    LOOP
        if ( !exists( father ))
            return;
        end
        file = father.file;
        x = father.x;
        graph = father.graph;
        size = father.size;
        keflags = father.flags;
        angle = father.angle;
        calculos = 385 - father.y;
        calculos2 = 385 + calculos;
        if ( calculos2 => 385 )
            y = calculos2;
        end
        if ( calculos2 < 385 )
            y = 550;
        end
        if ( keflags == 0 )
            flags = 6;
        end
        if ( keflags == 1 )
            flags = 7;
        end
        if ( keflags == 4 )
            alpha = 128;
        end
        if ( keflags == 5 )
            flags = 5;
        end
        if ( keflags == 16 )
            flags = 16;
        end
        frame;
    end
end


PROCESS imita_josel();
PRIVATE
    int calculos;
    int calculos2;
BEGIN
    ctype = c_scroll;
    z = -9;
    flags = 6;
    y = 400;
    priority = -120;
    LOOP
        calculos = 385 - y_p1;
        calculos2 = 385 + calculos;
        if ( calculos2 => 385 )
            y = calculos2;
        end
        if ( calculos2 < 385 )
            y = 550;
        end
        x = x_p1;
        graph = imita_graf;
        angle = imita_angle;
        if ( imita_flags == 0 )
            flags = 6;
        end
        if ( imita_flags == 1 )
            flags = 7;
        end
        frame;
    end
end


PROCESS sombras( double x, double y, int graph );
BEGIN
    ctype = c_scroll;
    z = -4;
    alpha = 128;
    priority = -120;
    LOOP
        if ( !exists( father ))
            return;
        end
        x = father.x;
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) != ke_color_es )
            y += 4;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            y -= 4;
        end
        frame;
    end
end


PROCESS brazo_josel( double x, double y );
BEGIN
    ctype = c_scroll;
    graph = 236;
    z = -6;
    LOOP
        x = x_p1;
        y = y_p1;
        flags = imita_flags;
        frame;
    end
end


PROCESS camara_juego();
PRIVATE
    int esperate;
    int guarda_x;
    int intentos;
BEGIN
    y = 240;
    x = 320;
    graph = 1;
    ctype = c_scroll;
    scroll.camera = id;
    x_p1 = 0;
    priority = 100;
    LOOP
        if ( bloquea == 0 OR bloquea == 3 )
            x = x_p1;
        end
        if ( bloquea == 2 )
            if ( x < x_p1 )
                x += 8;
            else
                x -= 8;
            end
            if ( x => x_p1 -8 AND x <= x_p1 + 8 )
                bloquea = 0;
                x = x_p1;
            end
        end
        // EVENTO EXPLOSION
        if ( estado_fase == 1 )
            if ( rondas_fase == 0 )
                if ( x_p1 > 1000 )
                    x = 1000;
                    bloquea = 1;
                end
                if ( bloquea == 1 )
                    if ( !exists( type coche ) AND !exists( type salta_barril ))
                        rondas_fase = 1;
                        bloquea = 2;
                    end
                end
            end
        end
        // RONDAS DE ENEMIGOS
        if ( estado_fase == 2 )
            if ( rondas_fase == 0 )
                if ( x_p1 > 1244 )
                    x = 1244;
                    bloquea = 3;
                end
                if ( bloquea == 3 OR x_p1 > 320 )
                    if ( elicopteros < 1 AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            rondas_fase = 1;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
            if ( rondas_fase == 1 )
                if ( x_p1 > 2148 )
                    x = 2148;
                    if ( bloquea == 0 )
                        bloquea = 1;
                        objetivos_a = "incompleto";
                        objetivos_a1 = 0;
                        elicopteros = 2;
                        elicoptero1( 2600, 84, 20, fase1, 1 );
                        if ( cantidadificultad == 3 )
                            elicoptero1( 1600, 84, 20, fase1, 1 );
                        end
                    end
                end
                if ( bloquea == 1 )
                    if ( elicopteros < 1 AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            rondas_fase = 2;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
            if ( rondas_fase == 2 )
                objetivos_a = "incompleto";
                objetivos_a1 = 0;
                msn_enemigos = 0;
                enemigos_tierra = 25;
                elicopteros = 4;
                elicoptero1( 3600, 84, 20, fase1, 1 );
                enemigo_arabe( 2500, 420, 1, 1, 1 );
                enemigo_arabe( 2500, 420, 1, 1, 1 );
                enemigo_arabe( 2500, 420, 1, 1, 1 );
                enemigo_dispara( 2800, 289, fase1, 1 );
                if ( cantidadificultad == 3 )
                    elicoptero1( 1000, 84, 20, fase1, 1 );
                    enemigo_arabe( 3000, 420, 1, 1, 1 );
                end
                rondas_fase = 3;
            end
        end
        // LOS ROBOTS
        if ( estado_fase == 3 )
            if ( rondas_fase == 0 )
                if ( x_p1 > 973 )
                    x = 973;
                    bloquea = 3;
                end
                if ( bloquea == 3 )
                    if ( total_robots == 0 AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            rondas_fase = 1;
                            bloquea = 2;
                            intentos = 0;
                            enemigo_arabe( 1900, 420, 1, 1, 1 );
                            enemigo_arabe( 1900, 420, 1, 1, 1 );
                            enemigo_arabe( 1900, 420, 1, 1, 1 );
                            enemigo_dispara( 1900, 289, fase1, 1 );
                            if ( cantidadificultad == 3 )
                                enemigo_arabe( 1900, 420, 1, 1, 1 );
                            end
                        end
                    else
                        intentos = 0;
                    end
                end
            end
            if ( rondas_fase == 1 )
                if ( x_p1 > 2140 )
                    x = 2140;
                    if ( bloquea == 0 )
                        bloquea = 1;
                        objetivos_a = "incompleto";
                        objetivos_a1 = 0;
                        enemigos_tierra = 10;
                        enemigo_arabe( 1600, 420, 1, 1, 1 );
                        enemigo_arabe( 1600, 420, 1, 1, 1 );
                        enemigo_arabe( 1600, 420, 1, 1, 1 );
                        enemigo_dispara( 1600, 289, fase1, 1 );
                        if ( cantidadificultad == 3 )
                            enemigo_arabe( 1600, 420, 1, 1, 1 );
                        end
                    end
                end
                if ( bloquea == 1 )
                    if ( !exists( type robot ) AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            enemigos_tierra = 1;
                            rondas_fase = 2;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
            if ( rondas_fase == 2 )
                if ( x_p1 > 3058 )
                    x = 3058;
                    if ( bloquea == 0 )
                        bloquea = 3;
                        objetivos_a = "incompleto";
                        objetivos_a1 = 0;
                        msn_enemigos = 0;
                        enemigos_tierra = 25;
                        enemigo_arabe( 2500, 420, 1, 1, 1 );
                        enemigo_arabe( 3500, 420, 1, 1, 1 );
                        enemigo_arabe( 3500, 420, 1, 1, 1 );
                        enemigo_arabe( 2500, 420, 1, 1, 1 );
                        enemigo_dispara( 3500, 420, fase1, 1 );
                        if ( cantidadificultad == 2 )
                            enemigo_arabe( 2500, 420, 1, 1, 1 );
                            enemigo_dispara( 3500, 420, fase1, 1 );
                        end
                        if ( cantidadificultad == 3 )
                            enemigo_arabe( 2500, 420, 1, 1, 1 );
                            enemigo_dispara( 3500, 420, fase1, 1 );
                            enemigo_dispara( 2500, 420, fase1, 1 );
                        end
                    end
                end
                if ( bloquea == 3 OR x_p1 > 320 )
                    if ( enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            msn_enemigos = 0;
                            elicopteros = 0;
                            rondas_fase = 3;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
        end
        // BLOQUEA BARRERA AL PRINCIPIO Y ANTES DE LA FILTRACION
        if ( estado_fase == 7 )
            if ( rondas_fase == 0 )
                if ( x_p1 > 320 )
                    x = 320;
                    bloquea = 1;
                end
                if ( bloquea == 1 )
                    if ( !exists( type helicoptero3 ))
                        rondas_fase = 1;
                        bloquea = 2;
                    end
                end
            end
            if ( rondas_fase == 1 )
                if ( x_p1 > 4900 )
                    x = 4900;
                    bloquea = 3;
                end
                if ( bloquea == 3 OR x_p1 > 1000 )
                    if ( elicopteros < 1 AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            rondas_fase = 2;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
        end
        // NO DEJA LLEGAR A LOS FOGONES
        if ( estado_fase == 9 )
            if ( rondas_fase == 0 )
                if ( x_p1 > 3416 )
                    x = 3416;
                    bloquea = 3;
                end
                if ( bloquea == 3 OR x_p1 > 1000 )
                    if ( elicopteros < 1 AND enemigos_tierra < 1 )
                        intentos++;
                        if ( intentos > 5 )
                            rondas_fase = 1;
                            bloquea = 2;
                            intentos = 0;
                        end
                    else
                        intentos = 0;
                    end
                end
            end
        end
        // say(enemigos_tierra); // say("x_p1: "+x_p1); // say("rondas: "+rondas_fase); say("heli: "+elicopteros); say("ene: "+enemigos_tierra);
        if ( boom == 1 )
            esperate++;
            if ( esperate == 1 )
                y = 250;
            end
            if ( esperate == 2 )
                y = 260;
            end
            if ( esperate == 3 )
                y = 270;
            end
            if ( esperate == 4 )
                y = 260;
            end
            if ( esperate == 5 )
                y = 240;
            end
            if ( esperate == 6 )
                y = 250;
            end
            if ( esperate == 7 )
                y = 260;
            end
            if ( esperate == 8 )
                y = 250;
            end
            if ( esperate == 9 )
                y = 240;
            end
            if ( esperate => 10 )
                boom = 0;
                esperate = 0;
            end
        end
        // say(enemigos_tierra);
        // say(elicopteros);
        if ( x != guarda_x OR boom == 1 )
            // dump_type=1; restore_type=-1;
            guarda_x = x;
        else
            //dump_type=0; restore_type=0;
        end
        camara_x = x;
        frame;
    end
end


PROCESS camara_vertical();
PRIVATE
    int esperate;
    int accion;
    int guarda_y;
BEGIN
    y = 3361;
    y_p1 = 3361;
    x = 320;
    graph = 1;
    ctype = c_scroll;
    scroll.camera = id;
    LOOP
        if ( y_p1 < 423 AND accion == 0 )
            crea_nieve();
            monstruo3( 120, -250 );
            accion = 1;
        end
        if ( accion == 0 )
            y = y_p1;
        end
        if ( accion == 1 )
            y -= 2;
        end
        if ( y < 240 AND accion == 1 )
            accion = 2;
        end
        if ( boom == 1 AND accion == 2 )
            esperate++;
            if ( esperate == 1 )
                y = 250;
            end
            if ( esperate == 2 )
                y = 260;
            end
            if ( esperate == 3 )
                y = 270;
            end
            if ( esperate == 4 )
                y = 260;
            end
            if ( esperate == 5 )
                y = 240;
            end
            if ( esperate == 6 )
                y = 250;
            end
            if ( esperate == 7 )
                y = 260;
            end
            if ( esperate == 8 )
                y = 250;
            end
            if ( esperate == 9 )
                y = 240;
            end
            if ( esperate => 10 )
                boom = 0;
                esperate = 0;
            end
        end
        if ( y != guarda_y )
            // dump_type=1; restore_type=-1;
            guarda_y = y;
        else
            // dump_type=0; restore_type=0;
        end
        frame;
    end
end


PROCESS camara_horizontal();
PRIVATE
    int esperate;
BEGIN
    y = 240;
    x = 320;
    graph = 1;
    ctype = c_scroll;
    scroll.camera = 0;
    LOOP
        scroll.x1++;
        // dump_type=1; restore_type=-1;
        if ( boom == 1 )
            esperate++;
            if ( esperate == 1 )
                y = 250;
            end
            if ( esperate == 2 )
                y = 260;
            end
            if ( esperate == 3 )
                y = 270;
            end
            if ( esperate == 4 )
                y = 260;
            end
            if ( esperate == 5 )
                y = 240;
            end
            if ( esperate == 6 )
                y = 250;
            end
            if ( esperate == 7 )
                y = 260;
            end
            if ( esperate == 8 )
                y = 250;
            end
            if ( esperate == 9 )
                y = 240;
            end
            if ( esperate => 10 )
                boom = 0;
                esperate = 0;
            end
        end
        frame;
    end
end


PROCESS camara_juego1();
PRIVATE
    int esperate;
BEGIN
    x = vagoneta_x;
    graph = 1;
    ctype = c_scroll;
    scroll.camera = id;
    y = 240;
    LOOP
        x = vagoneta_x;
        // dump_type=1; restore_type=-1;
        if ( boom == 1 )
            esperate++;
            if ( esperate == 1 )
                y = 250;
            end
            if ( esperate == 2 )
                y = 260;
            end
            if ( esperate == 3 )
                y = 270;
            end
            if ( esperate == 4 )
                y = 260;
            end
            if ( esperate == 5 )
                y = 240;
            end
            if ( esperate == 6 )
                y = 250;
            end
            if ( esperate == 7 )
                y = 260;
            end
            if ( esperate == 8 )
                y = 250;
            end
            if ( esperate == 9 )
                y = 240;
            end
            if ( esperate => 10 )
                boom = 0;
                esperate = 0;
            end
        end
        frame;
    end
end


PROCESS brazo_arma( double x, double y );
BEGIN
    ctype = c_scroll;
    z = -6;
    graph = 599;
    LOOP
        if ( arma_seleccionada_p1 == 1 )
            graph = 599;
        end
        if ( arma_seleccionada_p1 == 2 )
            graph = 595;
        end
        if ( arma_seleccionada_p1 == 3 )
            graph = 594;
        end
        if ( arma_seleccionada_p1 == 4 )
            graph = 596;
        end
        if ( arma_seleccionada_p1 == 5 )
            graph = 598;
        end
        if ( arma_seleccionada_p1 == 6 )
            graph = 597;
        end
        if ( imita_flags == 0 )
            x = x_p1 -10;
            flags = 0;
        end
        if ( imita_flags == 1 )
            x = x_p1 + 10;
            flags = 1;
        end
        y = y_p1 -35;
        frame;
    end
end


PROCESS arma_gastada( double x, double y, int graph, int flags );
PRIVATE
    double y_incr = -13;
    int rebotes;
BEGIN
    ctype = c_scroll;
    z = -7;
    LOOP
        if ( y_incr > 15 )
            y_incr = 15;
        end
        angle += 20000;
        y_incr++;
        y += y_incr;
        if ( flags == 0 )
            x -= 3;
        end
        if ( flags == 1 )
            x += 3;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND rebotes < 2 )
            y_incr = -7;
            rebotes++;
        end
        if ( y > 500 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS paracaidas( double x, double y, int tipo );
PRIVATE
    double y_incr = -13;
    int estado;
    int angulos;
    int cae;
    int guarda_y;
    int caida;
    int tiempo;
    int semaforo;
BEGIN
    // no en las fases tapadas
    if ( estado_fase == 3 OR estado_fase == 8 OR estado_fase == 9 )
        return;
    end
    ctype = c_scroll;
    z = -4;
    graph = 600;
    y = y -40;
    caida = rand( 2, 4 );
    LOOP
        guarda_y = y;
        if ( estado == 0 )
            if ( tipo == 1 )
                x = x_p1 -100;
            end
            if ( tipo == 2 )
                x = x_p1 + 100;
            end
            y += 3;
        end
        if ( y > 50 )
            estado = 1;
        end
        if ( estado == 1 )
            angulos++;
            if ( angulos <= 5 )
                angle += 500;
            end
            if ( angulos > 5 )
                angle -= 500;
            end
            if ( angulos == 10 )
                angulos = 0;
            end
            if ( tipo == 1 )
                x -= 5;
                y += caida;
            end
            if ( tipo == 2 )
                x += 5;
                y += caida;
            end
        end
        if ( y_incr > 15 )
            y_incr = 15;
        end
        if ( x < -50 OR y > 550 OR x > limite_pantalla + 50 )
            signal( id, s_kill );
        end
        if ( cae == 1 )
            tiempo++;
            if ( tiempo > 25 )
                return;
            end
            semaforo++;
            if ( semaforo == 3 )
                if ( graph == 600 )
                    graph = 0;
                else
                    graph = 600;
                end
                semaforo = 0;
            end
        end
        if ( cae == 0 )
            if ( y > 240 )
                cae = 1;
            end
            if ( !exists( father ))
                cae = 1;
            end
            if ( cae == 0 )
                y = father.y;
                x = father.x;
            end
            if ( guarda_y == y AND y > 0 )
                cae = 1;
            end
        end
        frame;
    end
end


PROCESS jose_player1( double x, double y );
PRIVATE
    int color;
    int ca; // canal de sonido, sonidos del personaje
    double x_pos;
    double y_pos; // guarda la x y la y
    int s_disparo1; // sonido disparo
    int esperate; // pekena pausa
    int estado; // estado de los graficos
    int saltando = 0; // estado del salto
    double y_salto; // altura del salto
    int salto_pulso; // detecta la tecla pulsada
    int salto_rapido; // salto giratorio
    int reconoce_sonido; // evita ke se confundan sonidos
    int disp_pulso;
    int movizq_pulso;
    int movder_pulso;
    int movabajo_pulso;
    int movarriba_pulso;
    int sentido; // si mira a la derecha o a la izquierda
    int grafico_usado; // mas animaciones para cada estado
    int calidad_sentandose;
    int calidad_alaire;
    int calidad_kieto;
    int caigo_del_aire; // cae del aire, ejem.. ejem ...
    int cuando_tedan;
    int incremento_x;
    int esperaterl = 5;
    int en_el_aire;
    int haz_graficos; // se encarga de hacer los movimientos
    int haz_graficos2;
    int control;
    int ix;
    int muestra_aviso;
    int cuenta_aviso;
    int limite_mision;
    int salpica; // salpica al caer al agua (solo un poco)
    double y_incr_muerto; // si esta en el aire cuando muere, caera d todas formas
    int graf_muerte;
    int salto_giratorio; // hace un salto k t cagas
    int estoy_en_agua; // si me mojo salpico y no puedo disparar
    int actualiza_fps;
    int pon_fps;
    int estado_cual;
    int corregir;
BEGIN
    y_salto = altura_salto;
    graph = 240;
    z = -5;
    josel_tocao = 0;
    ctype = c_scroll;
    limite_mision = limite_pantalla -15;
    control = tipo_control1;
    if ( haz_reflejos == 1 )
        signal( type imita_josel, s_kill );
        imita_josel();
    end
    priority = 100;
    actualiza_fps = velocidad_juego;
    LOOP
        estado_cual = estado;
        imita_graf = graph;
        imita_flags = flags;
        imita_angle = angle; // deja que todos los procesos puedan ver sus graficos
        // pilla coordenadas del propio proceso
        if ( color != color_pared_1 AND color != color_pared_2 AND color != ke_color_es )
            x_pos = x;
            y_pos = y;
        end
        // si no tienes vida mueres
        if ( y_incr_muerto > 15 )
            y_incr_muerto = 15;
        end
        if ( limite_vida_player1 <= 0 AND control < 3 )
            control = 3;
            saltando = 0;
        end
        if ( control == 3 )
            y_incr_muerto++;
            y += y_incr_muerto;
            angle = 0;
            // al morir caera encima de algo, plataforma o suelo
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                polvo( x -16, y -3, 264, 1 );
                polvo( x, y -6, 264, 3 );
                polvo( x + 16, y -2, 264, 2 );
                sound_play( caida, 0, 15 );
                control = 4;
            end
        end
        if ( control == 4 )
            graf_muerte++;
            if ( graf_muerte == 1 )
                graph = 284;
            end
            if ( graf_muerte == 4 )
                graph = 285;
            end
            if ( graf_muerte == 8 )
                graph = 286;
            end
            if ( graf_muerte == 12 )
                graph = 287;
            end
            if ( graf_muerte == 16 )
                graph = 288;
            end
            if ( graf_muerte => 20 )
                graph = 289;
                graf_muerte = 20;
                gameover();
            end
        end
        if ( limite_vida_player1 > 0 ) // Cumple todas las condiciones si keda vida
            if ( esperaterl > 0 )
                esperaterl--;
            end
            if ( pulsa_esc == 1 AND juego_en_pausa == 0 AND esperaterl == 0 )
                esperaterl = 10;
                pausa_juego();
            end
            if ( y_salto > 10 )
                y_salto = 10;
            end // deja incrementar la velocidad de caida, pero hasta un limite
            if ( cuando_tedan > 0 )
                cuando_tedan--;
                josel_tocao = 0;
            end
            x_p1 = x;
            y_p1 = y; // todos saben las coordenadas del personaje
            grafico_usado = 0; // esto hace ke al personaje se le asigne un grafico y no se repita
            // Marca limite de pantalla monstruo
            if ( estado_fase == 23 )
                if ( caigo_del_aire == 0 )
                    saltando = 1;
                    estado = 2;
                    graph = 259;
                    calidad_kieto = 0;
                    caigo_del_aire = 1;
                end
            end
            if ( estado_fase == 8 )
                if ( caigo_del_aire == 0 )
                    saltando = 1;
                    estado = 2;
                    graph = 259;
                    calidad_kieto = 0;
                    caigo_del_aire = 1;
                end
            end
            if ( estado_fase == 5 )
                if ( caigo_del_aire == 0 )
                    saltando = 1;
                    estado = 2;
                    graph = 259;
                    calidad_kieto = 0;
                    caigo_del_aire = 1;
                end
                if ( x < 2373 )
                    x = 2373;
                end
                if ( x > 2993 )
                    x = 2993;
                end
            end
            // Marca el limite de la fase
            if ( x > limite_pantalla )
                x -= 5;
                estado_cual = 2;
            end
            if ( x < 15 )
                x = 15;
                estado_cual = 1;
            end
            if ( elicopteros == 0 AND enemigos_tierra == 0 AND boss == 0 )
                if ( collision( type sal_pantalla ))
                    fase2_b(); // carga la siguiente escena
                    signal( id, s_freeze );
                end
            end
            if ( elicopteros > 0 OR enemigos_tierra > 0 OR boss > 0 ) // objetivos_b1==0
                if ( x => limite_mision )
                    x = limite_mision;
                    if ( estado_fase != 5 AND estado_fase != 8 AND estado_fase != 10 AND estado_fase != 24 AND estado_fase != 25 )
                        muestra_aviso++;
                    end
                    estado_cual = 2;
                end
                if ( collision( type sal_pantalla ))
                    muestra_aviso++;
                    estado_cual = 2;
                end
            end
            if ( muestra_aviso == 1 )
                abre_fase( 10 );
                abre_fase( 11 );
                muestra_aviso = 2;
            end
            if ( muestra_aviso > 0 )
                cuenta_aviso++;
                if ( cuenta_aviso == 150 )
                    muestra_aviso = 0;
                    cuenta_aviso = 0;
                end
            end
            // salpica al sumergirse en el agua
            if ( estoy_en_agua == 0 AND collision( type agua ))
                salpicada( x, y, 170 );
                estoy_en_agua = 1;
                trozo_suelo( x, y -3, 377, 10 );
                trozo_suelo( x, y -3, 377, 20 );
                trozo_suelo( x, y -3, 377, 10 );
                sound_play( splash, 0 );
            end
            if ( estoy_en_agua == 1 AND ( NOT( collision( type agua ))) AND y_salto == 1 )
                estoy_en_agua = 0;
            end
            // Los enemigos que te dan o colisionas con pinchos , etc
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_pared_con_pinchos OR collision( type pinchos )
                OR collision( type explosion ))
                josel_tocao = 1;
            end
            if ( cuando_tedan == 0 ) // deja tiempo sin k te den
                if ( collision( type bala_enemigo ) OR josel_tocao == 1 )
                    signal( type vaya_ostia, s_kill );
                    graph = 316;
                    haz_graficos2 = 2;
                    limite_vida_player1--;
                    brillos( 311, 2 );
                    ca = sound_play( golpe10, 0, 13 );
                    cuando_tedan = 15;
                    josel_tocao = 0;
                end
            end
            if ( estado == 0 )
                haz_graficos2++;
                if ( pulsa_c == 1 )
                    graph = 317;
                    haz_graficos2 = 5;
                end
                if ( haz_graficos2 == 1 )
                    graph = 329;
                end
                if ( haz_graficos == 10 )
                    graph = 330;
                end
                if ( haz_graficos2 == 20 )
                    graph = 331;
                end
                if ( haz_graficos == 30 )
                    graph = 332;
                end
                if ( haz_graficos2 == 39 )
                    haz_graficos2 = 0;
                end
                calidad_kieto++; // el personaje respira, etc
                if ( calidad_kieto == 40 )
                    calidad_kieto = 0;
                end
            end // estado quieto
            if ( grafico_usado == 1 )
                graph = 265;
            end // anda apuntando inclinado
            if ( estado == 1 )
                haz_graficos2 = 0;
                if ( graph > 253 )
                    graph = 241;
                end
            end // al caminar
            if ( estado == 2 )
                haz_graficos2 = 0;
                if ( graph > 262 )
                    graph = 262;
                end
            end // al saltar
            // si no hay ninguna accion vuelve el estado 0
            if ( saltando == 0 AND movarriba_pulso == 1 AND movabajo_pulso == 1 )
                estado = 0;
                incremento_x = 0;
                haz_graficos = 1;
            end
            if ( saltando == 0 AND movarriba_pulso == 0 AND movabajo_pulso == 0 AND movizq_pulso == 0
                AND movder_pulso == 0 )
                estado = 0;
                incremento_x = 0;
                haz_graficos = 1;
            end
            if ( estado == 3 )
                if ( graph > 277 )
                    graph = 265;
                end
            end // anda apuntando inclinado
        end
        // activa o desactiva un estado del personaje, segun si la tecla este pulsada
        if ( control == 1 )
            if ( pulsa_arriba == 0 )
                movarriba_pulso = 0;
                calidad_alaire = 0;
            end
            if ( pulsa_arriba > 0 )
                movarriba_pulso = 1;
                calidad_kieto = 0;
                haz_graficos2 = 0;
            end
            if ( pulsa_abajo == 0 )
                movabajo_pulso = 0;
                calidad_sentandose = 0;
            end
            if ( pulsa_abajo > 0 )
                movabajo_pulso = 1;
                calidad_kieto = 0;
                haz_graficos2 = 0;
            end
            if ( pulsa_izq == 0 )
                movizq_pulso = 0;
            end
            if ( pulsa_izq > 0 )
                movizq_pulso = 1;
                calidad_kieto = 0;
                haz_graficos2 = 0;
            end
            if ( pulsa_der == 0 )
                movder_pulso = 0;
            end
            if ( pulsa_der > 0 )
                movder_pulso = 1;
                calidad_kieto = 0;
                haz_graficos2 = 0;
            end
            if ( pulsa_c == 0 )
                disp_pulso = 0;
            end
            if ( pulsa_x == 0 )
                salto_pulso = 0;
            end
            if ( pulsa_izq > 0 AND pulsa_der > 0 )
                estado = 0;
                calidad_kieto = 0;
                incremento_x = 0;
            end
        end
        if ( control == 2 )
            control = 1;
        end
        // Accion de saltar, se mueve
        if ( salto_giratorio == 1 )
            graph = 283;
            y_salto++;
            y += y_salto;
            en_el_aire = 1;
            haz_graficos2 = 0;
            angle -= 25000;
            if ( modo_wiz == 0 )
                if ( y_salto == -16 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == -13 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == -10 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == -7 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == -3 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == 1 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == 4 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == 7 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == 10 )
                    blur_efect( x, y, 0, 312, angle );
                end
                if ( y_salto == 13 )
                    blur_efect( x, y, 0, 312, angle );
                end
            end
            if ( movarriba_pulso == 0 AND movabajo_pulso == 0 )
                estado = 1;
            end
            if ( movarriba_pulso == 0 AND movabajo_pulso == 1 )
                estado = 6;
            end
            if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movarriba_pulso == 1 )
                estado = 3;
            end
            if ( movarriba_pulso == 1 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                estado = 5;
            end
            if ( movarriba_pulso == 0 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                estado = 1;
            end
            if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movabajo_pulso == 1 )
                estado = 7;
            end
        end
        if ( saltando == 1 AND salto_giratorio == 0 )
            y_salto++;
            y += y_salto;
            en_el_aire = 1;
            haz_graficos2 = 0;
            if ( salto_rapido == 0 )
                if ( y_salto < -5 )
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 )
                        graph = 259;
                        estado = 1;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 1 )
                        graph = 302;
                        estado = 6;
                    end
                    if ( movarriba_pulso == 1 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        graph = 299;
                        estado = 5;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movarriba_pulso == 1 )
                        graph = 279;
                        estado = 3;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movabajo_pulso == 1 )
                        graph = 381;
                        estado = 7;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        estado = 1;
                    end
                end
                if ( y_salto => -5 AND y_salto < 5 )
                    if ( control == 1 AND salto_pulso == 0 )
                        if ( pulsa_x == 1 )
                            salto_giratorio = 1;
                            y_salto = -18;
                        end
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 )
                        graph = 260;
                        estado = 1;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 1 )
                        graph = 303;
                        estado = 6;
                    end
                    if ( movarriba_pulso == 1 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        graph = 300;
                        estado = 5;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movarriba_pulso == 1 )
                        graph = 280;
                        estado = 3;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movabajo_pulso == 1 )
                        graph = 382;
                        estado = 7;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        estado = 1;
                    end
                end
                if ( y_salto => 0 AND y_salto < 5 )
                    if ( control == 1 AND salto_pulso == 0 )
                        if ( pulsa_x == 1 )
                            salto_giratorio = 1;
                            y_salto = -18;
                        end
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 )
                        graph = 261;
                        estado = 1;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 1 )
                        graph = 304;
                        estado = 6;
                    end
                    if ( movarriba_pulso == 1 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        graph = 301;
                        estado = 5;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movarriba_pulso == 1 )
                        graph = 281;
                        estado = 3;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movabajo_pulso == 1 )
                        graph = 383;
                        estado = 7;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        estado = 1;
                    end
                end
                if ( y_salto >= 5 )
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 )
                        graph = 260;
                        estado = 1;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 1 )
                        graph = 303;
                        estado = 6;
                    end
                    if ( movarriba_pulso == 1 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        graph = 300;
                        estado = 5;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movarriba_pulso == 1 )
                        graph = 280;
                        estado = 3;
                    end
                    if (( movder_pulso == 1 OR movizq_pulso == 1 ) AND movabajo_pulso == 1 )
                        graph = 382;
                        estado = 7;
                    end
                    if ( movarriba_pulso == 0 AND movabajo_pulso == 0 AND movder_pulso == 0 AND movizq_pulso == 0 )
                        estado = 1;
                    end
                end
            end
            if ( salto_rapido == 1 )
                graph = 283;
                if ( sentido == 0 )
                    angle -= 14000;
                end
                if ( sentido == 1 )
                    angle -= 14000;
                end
                if ( angle == 360000 )
                    angle = 0;
                    salto_rapido = 0;
                end
            end
        end
        // Controles pa hacer las acciones
        if ( esperate > 0 )
            esperate--;
        end
        // CONTROLES PAL TECLAO
        if ( control == 1 )
            if ( pulsa_arriba > 0 AND movabajo_pulso == 0 AND pulsa_izq == 0 AND pulsa_der == 0 AND pulsa_abajo == 0 AND saltando == 0 )
                estado = 5;
                calidad_alaire++;
                if ( calidad_alaire == 1 )
                    graph = 305;
                end
                if ( calidad_alaire == 2 )
                    graph = 306;
                end
                if ( calidad_alaire == 3 )
                    graph = 307;
                end
                if ( calidad_alaire > 4 )
                    graph = 297;
                end
            end
            if ( pulsa_abajo > 0 AND movarriba_pulso == 0 AND pulsa_arriba == 0 AND saltando == 0 )
                estado = 4;
                calidad_sentandose++;
                if ( calidad_sentandose == 1 )
                    graph = 284;
                end
                if ( calidad_sentandose == 2 )
                    graph = 285;
                end
                if ( calidad_sentandose == 3 )
                    graph = 286;
                end
                if ( calidad_sentandose > 4 )
                    graph = 290;
                end
            end
            // Se mueve hacia la derecha
            if ( pulsa_der > 0 AND pulsa_izq == 0 )
                if ( incremento_x < 0 )
                    incremento_x = 0;
                end
                if ( incremento_x < 4 )
                    incremento_x++;
                end
                if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) != color_pared_1 )
                    x += incremento_x;
                    if ( pulsa_x == 1 )
                        corregir = 1;
                    end
                end
                flags = 0;
                sentido = 0;
                if ( movabajo_pulso == 1 AND saltando == 0 )
                    haz_graficos++;
                    estado = 7;
                    if ( haz_graficos == 1 )
                        graph = 384;
                    end
                    if ( haz_graficos == 2 )
                        graph = 385;
                    end
                    if ( haz_graficos == 3 )
                        graph = 386;
                    end
                    if ( haz_graficos == 4 )
                        graph = 387;
                    end
                    if ( haz_graficos == 5 )
                        graph = 388;
                    end
                    if ( haz_graficos == 6 )
                        graph = 389;
                    end
                    if ( haz_graficos == 7 )
                        graph = 390;
                    end
                    if ( haz_graficos == 8 )
                        graph = 391;
                    end
                    if ( haz_graficos == 9 )
                        graph = 392;
                    end
                    if ( haz_graficos == 10 )
                        graph = 393;
                    end
                    if ( haz_graficos == 11 )
                        graph = 394;
                    end
                    if ( haz_graficos == 12 )
                        graph = 395;
                    end
                    if ( haz_graficos => 13 )
                        graph = 396;
                        haz_graficos = 1;
                    end
                end
                if ( pulsa_arriba > 0 AND saltando == 0 )
                    if ( estado == 0 OR estado == 1 OR estado => 5 OR graph == 297 )
                        estado = 3;
                        if ( graph != 241 AND graph != 242 AND graph != 243 AND graph != 244 AND graph != 245 AND graph != 246
                            AND graph != 247 AND graph != 248 AND graph != 249 AND graph != 250 AND graph != 251 AND graph != 252
                            AND graph != 253 AND graph != 254 )
                            graph = 265;
                        end
                        if ( graph == 241 )
                            graph = 265;
                        end
                        if ( graph == 242 )
                            graph = 266;
                        end
                        if ( graph == 243 )
                            graph = 267;
                        end
                        if ( graph == 244 )
                            graph = 268;
                        end
                        if ( graph == 245 )
                            graph = 269;
                        end
                        if ( graph == 246 )
                            graph = 270;
                        end
                        if ( graph == 247 )
                            graph = 271;
                        end
                        if ( graph == 248 )
                            graph = 272;
                        end
                        if ( graph == 249 )
                            graph = 273;
                        end
                        if ( graph == 250 )
                            graph = 274;
                        end
                        if ( graph == 251 )
                            graph = 275;
                        end
                        if ( graph == 252 )
                            graph = 276;
                        end
                        if ( graph == 253 )
                            graph = 277;
                        end
                        if ( graph == 254 )
                            graph = 265;
                        end
                    end
                    estado = 3;
                    haz_graficos++;
                    if ( haz_graficos == 1 )
                        graph = 265;
                    end
                    if ( haz_graficos == 2 )
                        graph = 266;
                    end
                    if ( haz_graficos == 3 )
                        graph = 267;
                    end
                    if ( haz_graficos == 4 )
                        graph = 268;
                    end
                    if ( haz_graficos == 5 )
                        graph = 269;
                    end
                    if ( haz_graficos == 6 )
                        graph = 270;
                    end
                    if ( haz_graficos == 7 )
                        graph = 271;
                    end
                    if ( haz_graficos == 8 )
                        graph = 272;
                    end
                    if ( haz_graficos == 9 )
                        graph = 273;
                    end
                    if ( haz_graficos == 10 )
                        graph = 274;
                    end
                    if ( haz_graficos == 11 )
                        graph = 275;
                    end
                    if ( haz_graficos == 12 )
                        graph = 276;
                    end
                    if ( haz_graficos => 13 )
                        graph = 277;
                        haz_graficos = 1;
                    end
                end
                if ( saltando == 0 AND pulsa_arriba == 0 AND movabajo_pulso == 0 )
                    if ( estado => 3 )
                        estado = 1;
                        if ( graph != 265 AND graph != 266 AND graph != 267 AND graph != 268 AND graph != 269 AND graph != 270
                            AND graph != 271 AND graph != 272 AND graph != 273 AND graph != 274 AND graph != 275 AND graph != 276
                            AND graph != 277 AND graph != 278 )
                            graph = 241;
                        end
                        if ( graph == 265 )
                            graph = 241;
                        end
                        if ( graph == 266 )
                            graph = 242;
                        end
                        if ( graph == 267 )
                            graph = 243;
                        end
                        if ( graph == 268 )
                            graph = 244;
                        end
                        if ( graph == 269 )
                            graph = 245;
                        end
                        if ( graph == 270 )
                            graph = 246;
                        end
                        if ( graph == 271 )
                            graph = 247;
                        end
                        if ( graph == 272 )
                            graph = 248;
                        end
                        if ( graph == 273 )
                            graph = 249;
                        end
                        if ( graph == 274 )
                            graph = 250;
                        end
                        if ( graph == 275 )
                            graph = 251;
                        end
                        if ( graph == 276 )
                            graph = 252;
                        end
                        if ( graph == 277 )
                            graph = 253;
                        end
                        if ( graph == 278 )
                            graph = 241;
                        end
                    end
                    estado = 1;
                    haz_graficos++;
                    if ( haz_graficos == 1 )
                        graph = 241;
                    end
                    if ( haz_graficos == 2 )
                        graph = 242;
                    end
                    if ( haz_graficos == 3 )
                        graph = 243;
                    end
                    if ( haz_graficos == 4 )
                        graph = 244;
                    end
                    if ( haz_graficos == 5 )
                        graph = 245;
                    end
                    if ( haz_graficos == 6 )
                        graph = 246;
                    end
                    if ( haz_graficos == 7 )
                        graph = 247;
                    end
                    if ( haz_graficos == 8 )
                        graph = 248;
                    end
                    if ( haz_graficos == 9 )
                        graph = 249;
                    end
                    if ( haz_graficos == 10 )
                        graph = 250;
                    end
                    if ( haz_graficos == 11 )
                        graph = 251;
                    end
                    if ( haz_graficos == 12 )
                        graph = 252;
                    end
                    if ( haz_graficos => 13 )
                        graph = 253;
                        haz_graficos = 1;
                    end
                end
            end
            // se mueve hacia la izquierda
            if ( pulsa_izq > 0 AND pulsa_der == 0 )
                if ( incremento_x > 0 )
                    incremento_x = 0;
                end
                if ( incremento_x > -4 )
                    incremento_x--;
                end
                if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) != color_pared_2 )
                    x += incremento_x;
                    if ( pulsa_x == 1 )
                        corregir = 1;
                    end
                end
                flags = 1;
                sentido = 1;
                if ( movabajo_pulso == 1 AND saltando == 0 )
                    haz_graficos++;
                    estado = 7;
                    if ( haz_graficos == 1 )
                        graph = 384;
                    end
                    if ( haz_graficos == 2 )
                        graph = 385;
                    end
                    if ( haz_graficos == 3 )
                        graph = 386;
                    end
                    if ( haz_graficos == 4 )
                        graph = 387;
                    end
                    if ( haz_graficos == 5 )
                        graph = 388;
                    end
                    if ( haz_graficos == 6 )
                        graph = 389;
                    end
                    if ( haz_graficos == 7 )
                        graph = 390;
                    end
                    if ( haz_graficos == 8 )
                        graph = 391;
                    end
                    if ( haz_graficos == 9 )
                        graph = 392;
                    end
                    if ( haz_graficos == 10 )
                        graph = 393;
                    end
                    if ( haz_graficos == 11 )
                        graph = 394;
                    end
                    if ( haz_graficos == 12 )
                        graph = 395;
                    end
                    if ( haz_graficos => 13 )
                        graph = 396;
                        haz_graficos = 1;
                    end
                end
                if ( pulsa_arriba > 0 AND saltando == 0 )
                    if ( estado == 0 OR estado == 1 OR estado => 5 OR graph == 297 )
                        estado = 3;
                        if ( graph != 241 AND graph != 242 AND graph != 243 AND graph != 244 AND graph != 245 AND graph != 246
                            AND graph != 247 AND graph != 248 AND graph != 249 AND graph != 250 AND graph != 251 AND graph != 252
                            AND graph != 253 AND graph != 254 )
                            graph = 265;
                        end
                        if ( graph == 241 )
                            graph = 265;
                        end
                        if ( graph == 242 )
                            graph = 266;
                        end
                        if ( graph == 243 )
                            graph = 267;
                        end
                        if ( graph == 244 )
                            graph = 268;
                        end
                        if ( graph == 245 )
                            graph = 269;
                        end
                        if ( graph == 246 )
                            graph = 270;
                        end
                        if ( graph == 247 )
                            graph = 271;
                        end
                        if ( graph == 248 )
                            graph = 272;
                        end
                        if ( graph == 249 )
                            graph = 273;
                        end
                        if ( graph == 250 )
                            graph = 274;
                        end
                        if ( graph == 251 )
                            graph = 275;
                        end
                        if ( graph == 252 )
                            graph = 276;
                        end
                        if ( graph == 253 )
                            graph = 277;
                        end
                        if ( graph == 254 )
                            graph = 265;
                        end
                    end
                    estado = 3;
                    haz_graficos++;
                    if ( haz_graficos == 1 )
                        graph = 265;
                    end
                    if ( haz_graficos == 2 )
                        graph = 266;
                    end
                    if ( haz_graficos == 3 )
                        graph = 267;
                    end
                    if ( haz_graficos == 4 )
                        graph = 268;
                    end
                    if ( haz_graficos == 5 )
                        graph = 269;
                    end
                    if ( haz_graficos == 6 )
                        graph = 270;
                    end
                    if ( haz_graficos == 7 )
                        graph = 271;
                    end
                    if ( haz_graficos == 8 )
                        graph = 272;
                    end
                    if ( haz_graficos == 9 )
                        graph = 273;
                    end
                    if ( haz_graficos == 10 )
                        graph = 274;
                    end
                    if ( haz_graficos == 11 )
                        graph = 275;
                    end
                    if ( haz_graficos == 12 )
                        graph = 276;
                    end
                    if ( haz_graficos => 13 )
                        graph = 277;
                        haz_graficos = 1;
                    end
                end
                if ( saltando == 0 AND pulsa_arriba == 0 AND movabajo_pulso == 0 )
                    if ( estado => 3 )
                        estado = 1;
                        if ( graph != 265 AND graph != 266 AND graph != 267 AND graph != 268 AND graph != 269 AND graph != 270
                            AND graph != 271 AND graph != 272 AND graph != 273 AND graph != 274 AND graph != 275 AND graph != 276
                            AND graph != 277 AND graph != 278 )
                            graph = 241;
                        end
                        if ( graph == 265 )
                            graph = 241;
                        end
                        if ( graph == 266 )
                            graph = 242;
                        end
                        if ( graph == 267 )
                            graph = 243;
                        end
                        if ( graph == 268 )
                            graph = 244;
                        end
                        if ( graph == 269 )
                            graph = 245;
                        end
                        if ( graph == 270 )
                            graph = 246;
                        end
                        if ( graph == 271 )
                            graph = 247;
                        end
                        if ( graph == 272 )
                            graph = 248;
                        end
                        if ( graph == 273 )
                            graph = 249;
                        end
                        if ( graph == 274 )
                            graph = 250;
                        end
                        if ( graph == 275 )
                            graph = 251;
                        end
                        if ( graph == 276 )
                            graph = 252;
                        end
                        if ( graph == 277 )
                            graph = 253;
                        end
                        if ( graph == 278 )
                            graph = 241;
                        end
                    end
                    estado = 1;
                    haz_graficos++;
                    if ( haz_graficos == 1 )
                        graph = 241;
                    end
                    if ( haz_graficos == 2 )
                        graph = 242;
                    end
                    if ( haz_graficos == 3 )
                        graph = 243;
                    end
                    if ( haz_graficos == 4 )
                        graph = 244;
                    end
                    if ( haz_graficos == 5 )
                        graph = 245;
                    end
                    if ( haz_graficos == 6 )
                        graph = 246;
                    end
                    if ( haz_graficos == 7 )
                        graph = 247;
                    end
                    if ( haz_graficos == 8 )
                        graph = 248;
                    end
                    if ( haz_graficos == 9 )
                        graph = 249;
                    end
                    if ( haz_graficos == 10 )
                        graph = 250;
                    end
                    if ( haz_graficos == 11 )
                        graph = 251;
                    end
                    if ( haz_graficos == 12 )
                        graph = 252;
                    end
                    if ( haz_graficos => 13 )
                        graph = 253;
                        haz_graficos = 1;
                    end
                end
            end
            // Pega un salto
            if ( pulsa_x == 1 AND saltando == 0 AND salto_pulso == 0 )
                saltando = 1;
                salto_pulso = 1;
                estado = 2;
                graph = 259;
                calidad_kieto = 0;
                // if ((key(control_izq1)) OR (key(control_der1)) AND (saltando==0)) salto_rapido=1; end
            end
            // Provoca un disparo
            if ( pulsa_c > 0 AND esperate == 0 AND disp_pulso == 0 )
                calidad_kieto = 0;
                // Armas seleccionadas
                // se gasta la municion al disparar
                if ( autorecarga != 1 )
                    if ( arma_seleccionada_p1 == 2 AND municion_arma2 > 0 )
                        if ( municion_arma2 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma2--;
                    end
                    if ( arma_seleccionada_p1 == 3 AND municion_arma3 > 0 )
                        if ( municion_arma3 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma3--;
                    end
                    if ( arma_seleccionada_p1 == 4 AND municion_arma4 > 0 )
                        if ( municion_arma4 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma4--;
                    end
                    if ( arma_seleccionada_p1 == 5 AND municion_arma5 > 0 )
                        if ( municion_arma5 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma5--;
                    end
                    if ( arma_seleccionada_p1 == 6 AND municion_arma6 > 0 )
                        if ( municion_arma6 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma6--;
                    end
                    if ( arma_seleccionada_p1 == 7 AND municion_arma7 > 0 )
                        if ( municion_arma7 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma7--;
                    end
                    if ( arma_seleccionada_p1 == 8 AND municion_arma8 > 0 )
                        if ( municion_arma8 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma8--;
                    end
                    if ( arma_seleccionada_p1 == 9 AND municion_arma9 > 0 )
                        if ( municion_arma9 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma9--;
                    end
                    if ( arma_seleccionada_p1 == 10 AND municion_arma10 > 0 )
                        if ( municion_arma10 == 1 )
                            sound_play( recarga, 0 );
                            escrito = 1;
                        end
                        municion_arma10--;
                    end
                end
                // sonido que hace cada una y su disparo
                if ( arma_seleccionada_p1 == 2 )
                    esperate = 10;
                    disp_pulso = 0;
                    sound_play( uzi, 0, 18 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x + 62, y -27, 361, sentido, 1 );
                            bala( x + 62, y -27, 361, sentido, 8 );
                            bala( x + 62, y -27, 361, sentido, 7 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x + 62, y -27, 361, sentido, 1 );
                            bala( x + 62, y -27, 361, sentido, 8 );
                            bala( x + 62, y -27, 361, sentido, 7 );
                        end
                        if ( estado == 3 )
                            bala( x + 53, y -69, 361, sentido, estado );
                            bala( x + 53, y -69, 361, sentido, 9 );
                            bala( x + 53, y -69, 361, sentido, 10 );
                        end
                        if ( estado == 4 )
                            bala( x + 59, y -15, 361, sentido, 1 );
                            bala( x + 59, y -15, 361, sentido, 7 );
                            bala( x + 59, y -15, 361, sentido, 8 );
                        end
                        if ( estado == 5 )
                            bala( x + 5, y -98, 361, sentido, 5 );
                            bala( x + 5, y -98, 361, sentido, 11 );
                            bala( x + 5, y -98, 361, sentido, 12 );
                        end
                        if ( estado == 6 )
                            bala( x -10, y + 32, 361, sentido, 6 );
                            bala( x -10, y + 32, 361, sentido, 13 );
                            bala( x -10, y + 32, 361, sentido, 14 );
                        end
                        if ( estado == 7 )
                            bala( x + 49, y -2, 361, sentido, 15 );
                            bala( x + 49, y -2, 361, sentido, 17 );
                            bala( x + 49, y -2, 361, sentido, 18 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x -62, y -27, 361, sentido, 1 );
                            bala( x -62, y -27, 361, sentido, 7 );
                            bala( x -62, y -27, 361, sentido, 8 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x -62, y -27, 361, sentido, 1 );
                            bala( x -62, y -27, 361, sentido, 7 );
                            bala( x -62, y -27, 361, sentido, 8 );
                        end
                        if ( estado == 3 )
                            bala( x -53, y -69, 361, sentido, estado );
                            bala( x -53, y -69, 361, sentido, 9 );
                            bala( x -53, y -69, 361, sentido, 10 );
                        end
                        if ( estado == 4 )
                            bala( x -59, y -15, 361, sentido, 1 );
                            bala( x -59, y -15, 361, sentido, 7 );
                            bala( x -59, y -15, 361, sentido, 8 );
                        end
                        if ( estado == 5 )
                            bala( x -5, y -98, 361, sentido, 5 );
                            bala( x -5, y -98, 361, sentido, 11 );
                            bala( x -5, y -98, 361, sentido, 12 );
                        end
                        if ( estado == 6 )
                            bala( x + 10, y + 32, 361, sentido, 6 );
                            bala( x + 10, y + 32, 361, sentido, 13 );
                            bala( x + 10, y + 32, 361, sentido, 14 );
                        end
                        if ( estado == 7 )
                            bala( x -49, y -2, 361, sentido, 15 );
                            bala( x -49, y -2, 361, sentido, 17 );
                            bala( x -49, y -2, 361, sentido, 18 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 3 )
                    esperate = 10;
                    disp_pulso = 1;
                    sound_play( escop1, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            escopetazo( x + 62, y -27, 2 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            escopetazo( x + 62, y -27, 2 );
                        end
                        if ( estado == 3 )
                            escopetazo( x + 53, y -69, 6 );
                        end
                        if ( estado == 4 )
                            escopetazo( x + 59, y -15, 2 );
                        end
                        if ( estado == 5 )
                            escopetazo( x + 5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            escopetazo( x -10, y + 32, 4 );
                        end
                        if ( estado == 7 )
                            escopetazo( x + 49, y -2, 8 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            escopetazo( x -62, y -27, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            escopetazo( x -62, y -27, 1 );
                        end
                        if ( estado == 3 )
                            escopetazo( x -53, y -69, 5 );
                        end
                        if ( estado == 4 )
                            escopetazo( x -59, y -15, 1 );
                        end
                        if ( estado == 5 )
                            escopetazo( x -5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            escopetazo( x + 10, y + 32, 4 );
                        end
                        if ( estado == 7 )
                            escopetazo( x -49, y -2, 7 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 4 )
                    esperate = 13;
                    disp_pulso = 0;
                    sound_play( laser6, 0, 14 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x + 62, y -27, 350, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x + 62, y -27, 350, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x + 53, y -69, 350, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x + 59, y -15, 350, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x + 5, y -98, 354, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x -10, y + 32, 354, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x + 49, y -2, 350, sentido, 15 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x -62, y -27, 350, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x -62, y -27, 350, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x -53, y -69, 350, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x -59, y -15, 350, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x -5, y -98, 354, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x + 10, y + 32, 354, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x -49, y -2, 350, sentido, 15 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 5 )
                    esperate = 15;
                    disp_pulso = 0;
                    sound_play( lanzalla, 0 );
                    total_disparos++;
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            llamazo( x + 62, y -27, 2 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            llamazo( x + 62, y -27, 2 );
                        end
                        if ( estado == 3 )
                            llamazo( x + 53, y -69, 6 );
                        end
                        if ( estado == 4 )
                            llamazo( x + 59, y -15, 2 );
                        end
                        if ( estado == 5 )
                            llamazo( x + 5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            llamazo( x -10, y + 32, 4 );
                        end
                        if ( estado == 7 )
                            llamazo( x + 49, y -2, 8 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            llamazo( x -62, y -27, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            llamazo( x -62, y -27, 1 );
                        end
                        if ( estado == 3 )
                            llamazo( x -53, y -69, 5 );
                        end
                        if ( estado == 4 )
                            llamazo( x -59, y -15, 1 );
                        end
                        if ( estado == 5 )
                            llamazo( x -5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            llamazo( x + 10, y + 32, 4 );
                        end
                        if ( estado == 7 )
                            llamazo( x -49, y -2, 7 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 6 )
                    esperate = 20;
                    disp_pulso = 1;
                    sound_play( bazoka, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x + 62, y -27, 358, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x + 62, y -27, 358, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x + 53, y -69, 358, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x + 59, y -15, 358, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x + 5, y -98, 359, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x -10, y + 32, 360, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x + 49, y -2, 358, sentido, 15 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x -62, y -27, 358, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x -62, y -27, 358, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x -53, y -69, 358, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x -59, y -15, 358, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x -5, y -98, 359, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x + 10, y + 32, 360, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x -49, y -2, 358, sentido, 15 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 7 )
                    esperate = 15;
                    disp_pulso = 1;
                    sound_play( lanzalla, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            movilatak( x + 62, y -27, 0 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            movilatak( x + 62, y -27, 0 );
                        end
                        if ( estado == 3 )
                            movilatak( x + 53, y -69, 4 );
                        end
                        if ( estado == 4 )
                            movilatak( x + 59, y -15, 0 );
                        end
                        if ( estado == 5 )
                            movilatak( x + 5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            movilatak( x -10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            movilatak( x + 49, y -2, 0 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            movilatak( x -62, y -27, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            movilatak( x -62, y -27, 1 );
                        end
                        if ( estado == 3 )
                            movilatak( x -53, y -69, 5 );
                        end
                        if ( estado == 4 )
                            movilatak( x -59, y -15, 1 );
                        end
                        if ( estado == 5 )
                            movilatak( x -5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            movilatak( x + 10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            movilatak( x -49, y -2, 1 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 8 )
                    esperate = 3;
                    disp_pulso = 0;
                    s_disparo1 = sound_play( pistola1, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x + 62, y -27, 639, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x + 62, y -27, 639, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x + 53, y -69, 639, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x + 59, y -15, 639, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x + 5, y -98, 640, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x -10, y + 32, 640, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x + 49, y -2, 639, sentido, 15 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x -62, y -27, 639, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x -62, y -27, 639, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x -53, y -69, 639, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x -59, y -15, 639, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x -5, y -98, 640, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x + 10, y + 32, 640, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x -49, y -2, 639, sentido, 15 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 9 )
                    esperate = 20;
                    disp_pulso = 1;
                    sound_play( lanzalla, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            daldalboom( x + 62, y -27, 0 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            daldalboom( x + 62, y -27, 0 );
                        end
                        if ( estado == 3 )
                            daldalboom( x + 53, y -69, 4 );
                        end
                        if ( estado == 4 )
                            daldalboom( x + 59, y -15, 0 );
                        end
                        if ( estado == 5 )
                            daldalboom( x + 5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            daldalboom( x -10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            daldalboom( x + 49, y -2, 0 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            daldalboom( x -62, y -27, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            daldalboom( x -62, y -27, 1 );
                        end
                        if ( estado == 3 )
                            daldalboom( x -53, y -69, 5 );
                        end
                        if ( estado == 4 )
                            daldalboom( x -59, y -15, 1 );
                        end
                        if ( estado == 5 )
                            daldalboom( x -5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            daldalboom( x + 10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            daldalboom( x -49, y -2, 1 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 10 )
                    esperate = 20;
                    disp_pulso = 1;
                    sound_play( lanzalla, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            ultraboom( x + 62, y -27, 0 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            ultraboom( x + 62, y -27, 0 );
                        end
                        if ( estado == 3 )
                            ultraboom( x + 53, y -69, 4 );
                        end
                        if ( estado == 4 )
                            ultraboom( x + 59, y -15, 0 );
                        end
                        if ( estado == 5 )
                            ultraboom( x + 5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            ultraboom( x -10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            ultraboom( x + 49, y -2, 0 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            ultraboom( x -62, y -27, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            ultraboom( x -62, y -27, 1 );
                        end
                        if ( estado == 3 )
                            ultraboom( x -53, y -69, 5 );
                        end
                        if ( estado == 4 )
                            ultraboom( x -59, y -15, 1 );
                        end
                        if ( estado == 5 )
                            ultraboom( x -5, y -98, 3 );
                        end
                        if ( estado == 6 )
                            ultraboom( x + 10, y + 32, 6 );
                        end
                        if ( estado == 7 )
                            ultraboom( x -49, y -2, 1 );
                        end
                    end
                end
                if ( arma_seleccionada_p1 == 1 )
                    esperate = 5;
                    disp_pulso = 1;
                    s_disparo1 = sound_play( pistola1, 0 );
                    if ( sentido == 0 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x + 62, y -27, 257, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x + 62, y -27, 257, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x + 53, y -69, 257, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x + 59, y -15, 257, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x + 5, y -98, 298, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x -10, y + 32, 298, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x + 49, y -2, 257, sentido, 15 );
                        end
                    end
                    if ( sentido == 1 )
                        if ( estado == 0 OR estado == 1 )
                            bala( x -62, y -27, 257, sentido, 1 );
                        end
                        if ( estado == 2 AND saltando == 1 AND movarriba_pulso == 0 )
                            bala( x -62, y -27, 257, sentido, 1 );
                        end
                        if ( estado == 3 )
                            bala( x -53, y -69, 257, sentido, estado );
                        end
                        if ( estado == 4 )
                            bala( x -59, y - 15, 257, sentido, 1 );
                        end
                        if ( estado == 5 )
                            bala( x -5, y - 98, 298, sentido, 5 );
                        end
                        if ( estado == 6 )
                            bala( x + 10, y + 32, 298, sentido, 6 );
                        end
                        if ( estado == 7 )
                            bala( x -49, y - 2, 257, sentido, 15 );
                        end
                    end
                end
                // casquetes que dejan al disparar, etc
                if ( sentido == 0 )
                    resto_bala( x + 26, y - 37, sentido );
                    if ( estado == 0 OR estado == 1 OR estado == 2 )
                        brillo_disparo( x + 55, y - 29, sentido, estado );
                    end
                    if ( estado == 3 )
                        brillo_disparo( x + 46, y - 64, sentido, estado );
                    end
                    if ( estado != 4 AND salto_giratorio == 0 )
                        brillo_cuerpo( x + 5, y - 44, sentido, 1 );
                    end
                    if ( estado == 4 AND salto_giratorio == 0 )
                        brillo_cuerpo( x + 5, y - 30, sentido, 1 );
                    end
                end
                if ( sentido == 1 )
                    resto_bala( x -26, y - 37, sentido );
                    if ( estado == 0 OR estado == 1 OR estado == 2 )
                        brillo_disparo( x - 55, y - 29, sentido, estado );
                    end
                    if ( estado == 3 )
                        brillo_disparo( x - 46, y - 64, sentido, estado );
                    end
                    if ( estado != 4 AND salto_giratorio == 0 )
                        brillo_cuerpo( x - 5, y - 44, sentido, 1 );
                    end
                    if ( estado == 4 AND salto_giratorio == 0 )
                        brillo_cuerpo( x - 5, y - 30, sentido, 1 );
                    end
                end
            end
            if ( pulsa_v == 1 )
                write_delete( all_text );
                sound_play( recarga, 0 );
                arma_seleccionada_p1++;
                escrito = 1;
                esperate = 7;
            end
            if ( pulsa_enter == 1 and modo_wiz == 0 AND pausa_en_pantalla == 0 AND esperaterl == 0 )
                esperaterl = 10;
                pausa_en_pantalla = 1;
                menu_del_juego();
            end
        end
        // NO DEJA AVANZAR SI LA CAMARA NO SE MUEVE
        if ( bloquea != 0 )
            if ( x > camara_x + 310 )
                x = camara_x + 310;
            end
            if ( x < camara_x -310 )
                x = camara_x -310;
            end
        end
        // MAPA DE DUREZAS
        // Colisiona con todo tipo de objetos y marca limites
        if ( collision( type prohibido_elpaso ))
            x = x_pos;
            estado_cual = 2;
            if ( collision( type prohibido_elpaso ))
                if ( flags == 0 )
                    x -= 5;
                else
                    x += 5;
                end
            end
        end
        if ( x > limite_pantalla )
            x -= 5;
            estado_cual = 2;
        end
        if ( x < 15 )
            x = 15;
            estado_cual = 1;
        end
        // hace gravedad, hace caer al personaje k esta subido a un objeto que no es el suelo
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_arriba_objeto_no_piso AND ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )) AND saltando == 0 )
            saltando = 1;
            estado = 2;
            graph = 259;
            calidad_kieto = 0;
            if ( y_salto < 0 )
                y_salto = 0;
            end
        end
        // anda por zonas onduladas
        // Sube y baja por las montanas , toca superficies, hace gravedad en diferentes pantalla
        if ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ))
            if ( corregir == 0 )
                y += 4;
            end
        end
        // en el suelo y en una plataforma
        if ( y > 5000 OR map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            if ( saltando == 0 and corregir == 0 )
                y -= 4;
                saltando = 0;
                salto_giratorio = 0;
                y_salto = altura_salto;
            end
        end
        // al saltar cae bien y no la lies
        // toco el techo
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_techo AND y_salto < 0 )
            y_salto = 0;
        end
        // en el suelo
        if ( saltando == 1 AND map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y_salto > 0 )
            if ( estoy_en_agua == 0 )
                polvo( x -16, y -3, 264, 1 );
                polvo( x, y -6, 264, 3 );
                polvo( x + 16, y -2, 264, 2 );
                sound_play( caida, 0, 15 );
            end
            saltando = 0;
            salto_giratorio = 0;
            salto_rapido = 0;
            angle = 0;
            en_el_aire = 0;
            y_salto = altura_salto;
            if ( estado == 1 )
                graph = 241;
            end
            if ( estado == 2 OR estado == 0 )
                graph = 240;
            end
        end
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        // choca con las paredes
        if ( color == color_pared_1 )
            x = x_pos;
            estado_cual = 2;
        end
        if ( color == color_pared_2 )
            x = x_pos;
            estado_cual = 1;
        end
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        if ( color == ke_color_es OR color == color_pared_1 OR color == color_pared_2 )
            if ( saltando == 0 )
                for ( ix = 1; ix < 16; ix++ )
                    if ( color == ke_color_es OR color == color_pared_1 OR color == color_pared_2 ) // say(y);
                        if ( color == ke_color_es AND saltando == 0 )
                            y--;
                        end
                        if ( color == color_pared_1 )
                            x--;
                            x_pos = x;
                            estado_cual = 2;
                        end
                        if ( color == color_pared_2 )
                            x++;
                            x_pos = x;
                            estado_cual = 1;
                        end
                        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
                    else
                        break;
                    end
                end
            end
        end
        corregir = 0;
        // say(x+","+graph);
        frame;
    end
end


PROCESS onda_exp( double x, double y );
PRIVATE
    int haz_graficos;
BEGIN
    size = 20;
    graph = 372;
    ctype = c_scroll;
    z = -50;
    flags = 16;
    LOOP
        haz_graficos++;
        if ( haz_graficos == 1 )
            graph = 372;
        end
        if ( haz_graficos == 2 )
            graph = 373;
        end
        if ( haz_graficos == 3 )
            graph = 374;
        end
        if ( haz_graficos == 4 )
            graph = 375;
        end
        if ( haz_graficos == 5 )
            graph = 374;
        end
        if ( haz_graficos == 6 )
            graph = 373;
        end
        if ( haz_graficos == 7 )
            graph = 372;
        end
        if ( haz_graficos => 8 )
            haz_graficos = 0;
        end
        size += 10;
        if ( size => 180 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS items_info( double x, double y, int graph );
PRIVATE
    int estado;
BEGIN
    ctype = c_scroll;
    z = -7;
    LOOP
        y -= 2;
        estado++;
        if ( alpha > 15 )
            alpha -= 15;
        end
        if ( alpha <= 15 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS items( double x, double y, int tipo );
PRIVATE
    int estado;
    int estado2 = 2;
BEGIN
    z = -6;
    ctype = c_scroll;
    size = 10;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    if ( tipo == 1 )
        graph = 340;
    end // salud
    if ( tipo == 2 )
        graph = 333;
    end // municion bazooka
    if ( tipo == 3 )
        graph = 335;
    end // municion metra
    if ( tipo == 4 )
        graph = 321;
    end // municion laser
    if ( tipo == 5 )
        graph = 339;
        llaves++;
    end // llaves
    if ( tipo == 6 )
        graph = 334;
    end // municion escopeta
    if ( tipo == 7 )
        graph = 336;
    end // municion lanzallamas
    if ( tipo == 8 )
        graph = 6;
    end // sube de golpe un nivel
    if ( tipo == 9 )
        graph = 7;
        gasolinas++;
    end // gasolina
    if ( tipo == 10 )
        graph = 627;
    end // municion movilatak
    if ( tipo == 11 )
        graph = 634;
    end // municion cortacuerpos
    if ( tipo == 12 )
        graph = 625;
    end // municion daldalboom
    if ( tipo == 13 )
        graph = 626;
    end // municion ultraboom
    if ( tipo == 14 )
        graph = 642;
    end // objeto muelle
    if ( tipo == 15 )
        graph = 643;
    end // objeto moneda
    if ( tipo == 16 )
        graph = 644;
    end // objeto circuito
    if ( tipo == 17 )
        graph = 645;
    end // objeto mando
    if ( tipo == 18 )
        graph = 646;
    end // objeto vaso agua
    if ( tipo == 19 )
        graph = 647;
    end // objeto reductor
    if ( tipo == 20 )
        graph = 648;
    end // objeto agrandador
    if ( tipo == 21 )
        graph = 654;
    end // objeto joselillo
    if ( tipo == 22 )
        graph = 655;
    end // objeto navecilla
    if ( tipo == 23 )
        graph = 659;
    end // objeto melon
    if ( tipo == 24 )
        graph = 660;
    end // objeto pirana
    if ( tipo == 25 )
        graph = 661;
    end // objeto joselxdye
    if ( tipo == 26 )
        graph = 641;
    end // objeto ovni
    if ( tipo == 27 )
        graph = 657;
        regalos++;
    end // regalos
    LOOP
        if ( size < 5 )
            signal( id, s_kill );
        end
        if ( estado == 1 )
            size -= 10;
        end
        if ( estado == 0 )
            if ( estado2 == 2 )
                if ( size < 120 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
            if ( collision( type jose_player1 ) OR collision( type vagoneta ))
                if ( tipo == 1 )
                    limite_vida_player1 += 5;
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    items_info( x, y, 431 );
                end
                if ( tipo == 2 )
                    if ( municion_arma6 < 0 )
                        municion_arma6 = 0;
                    end
                    municion_arma6 += 5;
                    if ( municion_arma6 > 9999 )
                        municion_arma6 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe8, 0, 13 );
                    items_info( x, y, 432 );
                end
                if ( tipo == 3 )
                    if ( municion_arma2 < 0 )
                        municion_arma2 = 0;
                    end
                    municion_arma2 += 50;
                    if ( municion_arma2 > 9999 )
                        municion_arma2 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe2, 0, 13 );
                    items_info( x, y, 434 );
                end
                if ( tipo == 4 )
                    if ( municion_arma4 < 0 )
                        municion_arma4 = 0;
                    end
                    municion_arma4 += 25;
                    if ( municion_arma4 > 9999 )
                        municion_arma4 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe4, 0, 13 );
                    items_info( x, y, 433 );
                end
                if ( tipo == 5 )
                    llaves--;
                    alpha = 128;
                    estado = 1;
                    puntuacion += 100;
                    sound_play( decide, 0, 16 );
                    objetos_llave++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 6 )
                    if ( municion_arma3 < 0 )
                        municion_arma3 = 0;
                    end
                    municion_arma3 += 25;
                    if ( municion_arma3 > 9999 )
                        municion_arma3 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe3, 0, 13 );
                    items_info( x, y, 433 );
                end
                if ( tipo == 7 )
                    if ( municion_arma5 < 0 )
                        municion_arma5 = 0;
                    end
                    municion_arma5 += 25;
                    if ( municion_arma5 > 9999 )
                        municion_arma5 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe5, 0, 13 );
                    items_info( x, y, 433 );
                end
                if ( tipo == 8 )
                    puntuacion = sube_nivel;
                    alpha = 128;
                    estado = 1;
                end
                if ( tipo == 9 )
                    gasolinas--;
                    puntuacion += 300;
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_combustible++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 10 )
                    if ( municion_arma7 < 0 )
                        municion_arma7 = 0;
                    end
                    municion_arma7 += 5;
                    if ( municion_arma7 > 9999 )
                        municion_arma7 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe11, 0, 13 );
                    items_info( x, y, 432 );
                end
                if ( tipo == 11 )
                    if ( municion_arma8 < 0 )
                        municion_arma8 = 0;
                    end
                    municion_arma8 += 50;
                    if ( municion_arma8 > 9999 )
                        municion_arma8 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe16, 0, 13 );
                    items_info( x, y, 434 );
                end
                if ( tipo == 12 )
                    if ( municion_arma9 < 0 )
                        municion_arma9 = 0;
                    end
                    municion_arma9 += 5;
                    if ( municion_arma9 > 9999 )
                        municion_arma9 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe15, 0, 13 );
                    items_info( x, y, 432 );
                end
                if ( tipo == 13 )
                    if ( municion_arma10 < 0 )
                        municion_arma10 = 0;
                    end
                    municion_arma10 += 5;
                    if ( municion_arma10 > 9999 )
                        municion_arma10 = 9999;
                    end
                    alpha = 128;
                    estado = 1;
                    sound_play( recarga, 0 );
                    sound_play( golpe12, 0, 13 );
                    items_info( x, y, 432 );
                end
                if ( tipo > 13 AND tipo < 27 )
                    objetivos_c = "completo";
                end
                if ( tipo == 14 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_muelle++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 15 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_moneda++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 16 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_chip++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 17 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_teletransportador++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 18 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_vasoagua++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 19 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_reductor++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 20 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_ampliador++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 21 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_joselillo++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 22 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_navecilla++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 23 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_melon++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 24 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_pirana++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 25 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_joselxdye++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 26 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    objetos_ovni++;
                    items_info( x, y, 430 );
                end
                if ( tipo == 27 )
                    alpha = 128;
                    estado = 1;
                    sound_play( decide, 0, 16 );
                    puntuacion += 300;
                    regalos--;
                    objetos_regalo++;
                    items_info( x, y, 430 );
                end
            end
        end
        frame;
    end
end

// Complementos del personaje
// escopetazo del personaje, por separado de las armas

PROCESS escopetazo( double x, double y, int estado );
PRIVATE
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    size = 200;
    luzjose();
    z = -9;
    existe_bala++;
    total_disparos++;
    if ( estado == 1 )
        flags = 1;
    end
    if ( estado == 5 )
        flags = 1;
        angle = 30000;
    end
    if ( estado == 6 )
        angle = 30000;
    end
    if ( estado == 7 )
        flags = 1;
        angle = 330000;
    end
    if ( estado == 8 )
        angle = 330000;
    end
    if ( estado == 4 )
        flags = 2;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        haz_graficos++;
        if ( estado != 3 AND estado != 4 )
            if ( haz_graficos == 16 )
                existe_bala--;
                signal( id, s_kill );
            else
                graph = 451 + haz_graficos;
            end
        end
        if ( estado == 3 OR estado == 4 )
            if ( haz_graficos == 16 )
                existe_bala--;
                signal( id, s_kill );
            else
                graph = 466 + haz_graficos;
            end
        end
        if ( collision( type agua ) AND haz_graficos == 2 )
            if ( estado == 1 OR estado == 7 )
                salpicada( x -50, y, 200 );
                sangre_mix( fase1, x -100, 395, 4, 0 );
            end
            if ( estado == 2 OR estado == 8 )
                salpicada( x + 50, y, 200 );
                sangre_mix( fase1, x + 100, 395, 5, 0 );
            end
            if ( estado == 4 )
                salpicada( x, y, 200 );
                sangre_mix( fase1, x, 395, 4, 0 );
            end
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS llamazo( double x, double y, int estado );
PRIVATE
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    size = 200;
    z = -9;
    existe_bala++;
    if ( estado == 1 )
        flags = 1;
    end
    if ( estado == 3 )
        angle = 90000;
    end
    if ( estado == 5 )
        flags = 1;
        angle = 30000;
    end
    if ( estado == 6 )
        angle = 30000;
    end
    if ( estado == 7 )
        flags = 1;
        angle = 330000;
    end
    if ( estado == 8 )
        angle = 330000;
    end
    if ( estado == 4 )
        angle = 260000;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        haz_graficos++;
        if ( estado == 1 )
            x -= 6;
        end
        if ( estado == 2 )
            x += 6;
        end
        if ( estado == 3 )
            y -= 6;
        end
        if ( estado == 4 )
            y += 6;
        end
        if ( estado == 5 )
            x -= 6;
            y -= 6;
        end
        if ( estado == 6 )
            x += 6;
            y -= 6;
        end
        if ( estado == 7 )
            x -= 6;
            y += 6;
        end
        if ( estado == 8 )
            x += 6;
            y += 6;
        end
        if ( haz_graficos == 26 )
            existe_bala--;
            signal( id, s_kill );
        else
            graph = 481 + haz_graficos;
        end
        frame;
    end
end


PROCESS fogon( double x, double y, int estado );
PRIVATE
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    size = 200;
    z = -9;
    if ( estado == 1 )
        flags = 1;
    end // izquierda
    if ( estado == 3 )
        angle = 90000;
    end // Arriba
    if ( estado == 5 )
        flags = 1;
        angle = 30000;
    end
    if ( estado == 6 )
        angle = 30000;
    end // derecha arriba
    if ( estado == 7 )
        flags = 1;
        angle = 330000;
    end // izquierda abajo
    if ( estado == 8 )
        angle = 330000;
    end // derecha abajo
    if ( estado == 4 )
        angle = 260000;
    end // abajo
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        haz_graficos++;
        if ( estado == 0 )
            x += 6;
        end // derecha
        if ( estado == 1 )
            x -= 6;
        end
        if ( estado == 2 )
            x += 6;
        end
        if ( estado == 3 )
            y -= 6;
        end
        if ( estado == 4 )
            y += 6;
        end
        if ( estado == 5 )
            x -= 6;
            y -= 6;
        end
        if ( estado == 6 )
            x += 6;
            y -= 6;
        end
        if ( estado == 7 )
            x -= 6;
            y += 6;
        end
        if ( estado == 8 )
            x += 6;
            y += 6;
        end
        if ( haz_graficos == 1 )
            graph = 482;
        end
        if ( haz_graficos == 2 )
            graph = 483;
        end
        if ( haz_graficos == 3 )
            graph = 484;
        end
        if ( haz_graficos == 4 )
            graph = 485;
        end
        if ( haz_graficos == 5 )
            graph = 486;
        end
        if ( haz_graficos == 6 )
            graph = 487;
        end
        if ( haz_graficos == 7 )
            graph = 488;
        end
        if ( haz_graficos == 8 )
            graph = 489;
        end
        if ( haz_graficos == 9 )
            graph = 490;
        end
        if ( haz_graficos == 10 )
            graph = 491;
        end
        if ( haz_graficos == 11 )
            graph = 492;
        end
        if ( haz_graficos == 12 )
            graph = 493;
        end
        if ( haz_graficos == 13 )
            graph = 494;
        end
        if ( haz_graficos == 14 )
            graph = 495;
        end
        if ( haz_graficos == 15 )
            graph = 496;
        end
        if ( haz_graficos == 16 )
            graph = 497;
        end
        if ( haz_graficos == 17 )
            graph = 498;
        end
        if ( haz_graficos == 18 )
            graph = 499;
        end
        if ( haz_graficos == 19 )
            graph = 500;
        end
        if ( haz_graficos == 20 )
            graph = 501;
        end
        if ( haz_graficos == 21 )
            graph = 502;
        end
        if ( haz_graficos == 22 )
            graph = 503;
        end
        if ( haz_graficos == 23 )
            graph = 504;
        end
        if ( haz_graficos == 24 )
            graph = 505;
        end
        if ( haz_graficos == 25 )
            graph = 506;
        end
        if ( haz_graficos == 26 )
            existe_bala--;
            signal( id, s_kill );
        end
        if ( collision( type jose_player1 ))
            josel_tocao = 1;
        end
        frame;
    end
end


PROCESS torpedo( double x, double y );
PRIVATE
    int incr_y;
BEGIN
    ctype = c_scroll;
    z = -7;
    graph = 575;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        incr_y++;
        y += incr_y;
        if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y > 280 ) OR y > 480 )
            explosion3( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            signal( id, s_kill );
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS torpedo3( double x, double y );
PRIVATE
    int incr_y;
    int tiempokekae;
    int secuenciador;
BEGIN
    ctype = c_scroll;
    z = -7;
    graph = 641;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        incr_y++;
        y += incr_y;
        if ( secuenciador == 1 )
            tiempokekae++;
            if ( tiempokekae == 1 )
                fogon( x, 425, 3 );
                sound_play( lanzalla, 0 );
            end
            if ( tiempokekae == 10 )
                fogon( x -50, 425, 3 );
                fogon( x + 50, 425, 3 );
                sound_play( lanzalla, 0 );
            end
            if ( tiempokekae == 20 )
                fogon( x -100, 425, 3 );
                fogon( x + 100, 425, 3 );
                sound_play( lanzalla, 0 );
            end
            if ( tiempokekae > 20 )
                signal( id, s_kill );
            end
        end
        if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y > 280 ) OR y > 480 )
            secuenciador = 1;
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS lancha( double x, double y );
PRIVATE
    int rutina;
    int avance;
    int disparo_aleatorio;
    int cuenta_tiempo;
    int vida_lancha = 20;
    int estado4;
    int de_onde_salgo;
    int ponmelo;
    int agua_lenta;
    int estado;
    int salpicadas;
BEGIN
    ctype = c_scroll;
    file = fase1;
    graph = 244;
    disparo_aleatorio = rand( 50, 175 );
    z = -12;
    de_onde_salgo = rand( 1, 2 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        agua_lenta++;
        salpicadas++;
        if ( elicopteros <= 0 )
            elicopteros = 1;
        end
        if ( salpicadas == 10 )
            if ( flags == 0 )
                sangre_mix( fase1, x -60, y + 70, 4, 0 );
            end
            if ( flags == 1 )
                sangre_mix( fase1, x + 60, y + 70, 5, 0 );
            end
            salpicadas = 0;
        end
        if ( agua_lenta == 5 )
            if ( estado == 1 AND y < 350 )
                y++;
                agua_lenta = 0;
            end
            if ( estado == 2 AND y > 340 )
                y--;
                agua_lenta = 0;
            end
        end
        if ( y => 350 )
            estado = 2;
        end
        if ( y =< 340 )
            estado = 1;
        end
        // colisiona con las balas
        if ( vida_lancha <= 0 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            cacho_heli( x, y, 293, fase1, 1 );
            cacho_heli( x, y, 294, fase1, 2 );
            cacho_heli( x, y, 295, fase1, 3 );
            cacho_heli( x, y, 296, fase1, 4 );
            signal( type trozo_torpedo, s_kill );
            puntuacion += 300;
            elicopteros--;
            clones_eliminados++;
            if ( elicopteros > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -640;
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 640;
                end
                elicoptero2( ponmelo, -100, 24, fase1, 1 );
            end
            signal( id, s_kill );
        end
        if ( estado4 == 0 )
            if ( collision( type bala ) OR collision( type explosion ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_lancha--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
            end
            if ( collision( type llamazo ) OR collision( type escopetazo ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_lancha--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
                disparos_acertados++;
            end
        end
        // al ser tocado
        if ( estado4 > 0 )
            estado4++;
        end
        if ( estado4 == 1 )
            graph = 292;
        end
        if ( estado4 == 2 )
            graph = 244;
        end
        if ( estado4 == 3 )
            graph = 292;
        end
        if ( estado4 == 4 )
            graph = 244;
            estado4 = 0;
        end
        if ( collision( type trozo_torpedo ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            cacho_heli( x, y, 293, fase1, 1 );
            cacho_heli( x, y, 294, fase1, 2 );
            cacho_heli( x, y, 295, fase1, 3 );
            cacho_heli( x, y, 296, fase1, 4 );
            signal( type trozo_torpedo, s_kill );
            puntuacion += 300;
            elicopteros--;
            clones_eliminados++;
            if ( elicopteros > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -640;
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 640;
                end
                elicoptero2( ponmelo, -100, 24, fase1, 1 );
            end
            signal( id, s_kill );
        end
        if ( rutina == 1 )
            cuenta_tiempo++;
        end
        if ( cuenta_tiempo == disparo_aleatorio )
            if ( flags == 0 )
                torpedo2( x, y, 1 );
                sound_play( lanzalla, 0 );
            end
            if ( flags == 1 )
                torpedo2( x, y, 2 );
                sound_play( lanzalla, 0 );
            end
            cuenta_tiempo = 0;
            disparo_aleatorio = rand( 50, 175 );
        end
        if ( rutina == 1 ) // rutina de la I.A  ** Mision, atacar
            if ( x < x_p1 -320 AND vida_lancha > 0 )
                avance = 1;
                flags = 0;
            end
            if ( x > x_p1 + 320 AND vida_lancha > 0 )
                avance = 2;
                flags = 1;
            end
            if ( avance == 1 )
                x += 5;
                if ( region_out( id, 0 ) AND vida_lancha > 0 )
                    avance = 2;
                    flags = 1;
                end
            end
            if ( avance == 2 )
                x -= 5;
                if ( region_out( id, 0 ) AND vida_lancha > 0 )
                    avance = 1;
                    flags = 0;
                end
            end
        end
        if ( rutina == 0 ) // rutina de la I.A  ** Mision, buscar objetivo
            if ( x < x_p1 )
                x += 6;
                flags = 0;
                if ( x > x_p1 -100 AND x < x_p1 + 100 )
                    rutina = 1;
                    avance = 2;
                end
            end
            if ( x > x_p1 )
                x -= 6;
                flags = 1;
                rutina = 1;
                avance = 1;
            end
        end
        frame;
    end
end


PROCESS torpedo2( double x, double y, int tipo );
PRIVATE
    int haz_girar;
    int tiempo_torpedo;
    double x_incr;
    int humos;
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    z = -13;
    graph = 575;
    if ( tipo == 1 )
        angle = 90000;
    end
    if ( tipo == 2 )
        angle = 270000;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        humos++;
        haz_graficos++;
        if ( haz_graficos == 3 )
            graph = 590;
            haz_graficos = 0;
        end
        if ( haz_graficos == 1 )
            graph = 575;
        end
        if ( humos == 40 )
            humo( x, y );
            humos = 0;
        end
        if ( tipo == 1 AND haz_girar == 0 )
            x += 5;
            y -= 1;
            tiempo_torpedo++;
            if ( tiempo_torpedo == 10 )
                angle = 100000;
            end
            if ( tiempo_torpedo == 15 )
                angle = 110000;
            end
            if ( tiempo_torpedo == 20 )
                angle = 120000;
            end
            if ( tiempo_torpedo == 25 )
                angle = 130000;
            end
            if ( y < y_p1 )
                haz_girar = 1;
                tiempo_torpedo = 0;
            end
        end
        if ( haz_girar == 1 )
            tiempo_torpedo++;
            angle += 35000;
            if ( tiempo_torpedo => 30 )
                if ( x < x_p1 )
                    haz_girar = 4;
                    sound_play( scohete, 0 );
                end
                if ( x > x_p1 )
                    haz_girar = 3;
                    sound_play( scohete, 0 );
                end
            end
        end
        if ( haz_girar == 3 )
            if ( x_incr < 15 )
                x_incr++;
            end
            x -= x_incr;
            y -= 2;
            angle = 270000;
            if ( collision( type bala ) OR collision( type escopetazo ) OR collision( type llamazo ))
                trozo_torpedo( x, y, 2 );
                sound_play( smetal1, 0, 21 );
                signal( id, s_kill );
            end
            if ( collision( type jose_player1 ))
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                signal( id, s_kill );
            end
            if ( x < 0 OR x > limite_pantalla )
                signal( id, s_kill );
            end
        end
        if ( tipo == 2 AND haz_girar == 0 )
            x -= 5;
            y -= 1;
            tiempo_torpedo++;
            if ( tiempo_torpedo == 10 )
                angle = 260000;
            end
            if ( tiempo_torpedo == 15 )
                angle = 250000;
            end
            if ( tiempo_torpedo == 20 )
                angle = 240000;
            end
            if ( tiempo_torpedo == 25 )
                angle = 230000;
            end
            if ( y < y_p1 )
                haz_girar = 2;
                tiempo_torpedo = 0;
            end
        end
        if ( haz_girar == 2 )
            tiempo_torpedo++;
            angle -= 35000;
            if ( tiempo_torpedo => 30 )
                if ( x < x_p1 )
                    haz_girar = 4;
                    sound_play( scohete, 0 );
                end
                if ( x > x_p1 )
                    haz_girar = 3;
                    sound_play( scohete, 0 );
                end
            end
        end
        if ( haz_girar == 4 )
            if ( x_incr < 15 )
                x_incr++;
            end
            x += x_incr;
            y -= 2;
            angle = 90000;
            if ( collision( type bala ) OR collision( type escopetazo ) OR collision( type llamazo ))
                trozo_torpedo( x, y, 1 );
                sound_play( smetal1, 0, 21 );
                signal( id, s_kill );
            end
            if ( collision( type jose_player1 ))
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                signal( id, s_kill );
            end
            if ( x < 0 OR x > limite_pantalla )
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS trozo_torpedo( double x, double y, int tipo );
PRIVATE
    double y_incr = -14;
    int incr_x;
BEGIN
    ctype = c_scroll;
    graph = 575;
    z = -11;
    if ( tipo == 1 )
        incr_x = -4;
    end
    if ( tipo == 2 )
        incr_x = 4;
    end
    LOOP
        angle += 32000;
        y_incr++;
        y += y_incr;
        x += incr_x;
        if ( y > 480 )
            signal( id, s_kill );
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            sound_play( splash, 0 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS bala( double x, double y, int graph, int sentido, int estado );
PRIVATE
    int balafuera;
    int haz_graficos;
    int graf;
    int k_select;
    int id_e;
BEGIN
    total_disparos++;
    if ( estado == 3 )
        if ( sentido == 0 )
            angle = 45000;
        end
        if ( sentido == 1 )
            angle = 315000;
        end
    end
    if ( estado == 15 )
        if ( sentido == 0 )
            if ( arma_seleccionada_p1 == 4 OR arma_seleccionada_p1 == 3 )
                angle = 337000;
            end
            if ( arma_seleccionada_p1 != 4 OR arma_seleccionada_p1 == 3 )
                angle = 45000;
            end
        end
        if ( sentido == 1 )
            if ( arma_seleccionada_p1 == 4 OR arma_seleccionada_p1 == 3 )
                angle = 26000;
            end
            if ( arma_seleccionada_p1 != 4 OR arma_seleccionada_p1 == 3 )
                angle = 315000;
            end
        end
    end
    if ( arma_seleccionada_p1 == 4 )
        graf = 1;
    end
    if ( arma_seleccionada_p1 == 6 )
        size = 110;
    end
    k_select = arma_seleccionada_p1;
    ctype = c_scroll;
    z = -6;
    existe_bala++;
    priority = -1;
    if ( sentido == 1 )
        priority = -2;
    end
    if ( haz_reflejos == 1 )
        imita_bala();
    end
    LOOP
        if ( graf == 1 )
            haz_graficos++;
            if ( haz_graficos == 7 )
                haz_graficos = 0;
            end
            if ( estado != 5 AND estado != 6 )
                if ( haz_graficos == 1 )
                    graph = 350;
                end
                if ( haz_graficos == 2 )
                    graph = 351;
                end
                if ( haz_graficos == 3 )
                    graph = 352;
                end
                if ( haz_graficos == 4 )
                    graph = 353;
                end
                if ( haz_graficos == 5 )
                    graph = 352;
                end
                if ( haz_graficos == 6 )
                    graph = 351;
                end
            ELSE
                if ( haz_graficos == 1 )
                    graph = 354;
                end
                if ( haz_graficos == 2 )
                    graph = 355;
                end
                if ( haz_graficos == 3 )
                    graph = 356;
                end
                if ( haz_graficos == 4 )
                    graph = 357;
                end
                if ( haz_graficos == 5 )
                    graph = 356;
                end
                if ( haz_graficos == 6 )
                    graph = 355;
                end
            end
        end
        // disparo triple de la metralleta
        if ( estado == 7 ) // disparo uzi desviado hacia arriba , recto
            if ( sentido == 0 )
                x += 15;
                y -= 5;
            end
            if ( sentido == 1 )
                x -= 15;
                y -= 5;
            end
        end
        if ( estado == 8 ) // disparo uzi desviado hacia abajo , recto
            if ( sentido == 0 )
                x += 15;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 15;
                y += 5;
            end
        end
        if ( estado == 9 ) // disparo uzi desviado hacia arriba, vertical
            if ( sentido == 0 )
                x += 15;
                y -= 25;
                angle = 45000;
            end
            if ( sentido == 1 )
                x -= 15;
                y -= 25;
                angle = 315000;
            end
        end
        if ( estado == 10 ) // disparo uzi desviado hacia abajo, vertical
            if ( sentido == 0 )
                x += 20;
                y -= 5;
                angle = 45000;
            end
            if ( sentido == 1 )
                x -= 20;
                y -= 5;
                angle = 315000;
            end
        end
        if ( estado == 11 ) // disparo uzi desviado hacia izq, arriba
            if ( sentido == 0 )
                x -= 5;
                y -= 15;
            end
            if ( sentido == 1 )
                x += 5;
                y -= 15;
            end
        end
        if ( estado == 12 ) // disparo uzi desviado hacia der, arriba
            if ( sentido == 0 )
                x += 5;
                y -= 15;
            end
            if ( sentido == 1 )
                x -= 5;
                y -= 15;
            end
        end
        if ( estado == 13 ) // disparo uzi desviado hacia izq, abajo
            if ( sentido == 0 )
                x -= 5;
                y += 15;
            end
            if ( sentido == 1 )
                x += 5;
                y += 15;
            end
        end
        if ( estado == 14 ) // disparo uzi desviado hacia der, abajo
            if ( sentido == 0 )
                x += 5;
                y += 15;
            end
            if ( sentido == 1 )
                x -= 5;
                y += 15;
            end
        end
        if ( estado == 16 ) // disparo uzi desviado hacia izq,ver abajo
            if ( sentido == 0 )
                x -= 20;
                y += 15;
            end
            if ( sentido == 1 )
                x += 20;
                y += 15;
            end
        end
        if ( estado == 17 ) // disparo uzi desviado hacia der,ver abajo
            if ( sentido == 0 )
                x += 20;
                y += 15;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 15;
            end
        end
        if ( estado == 18 ) // disparo uzi desviado hacia izq,ver arriba
            if ( sentido == 0 )
                x += 20;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 5;
            end
        end
        if ( estado == 19 ) // disparo uzi desviado hacia der,ver arriba
            if ( sentido == 0 )
                x += 20;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 5;
            end
        end
        // disparos unicos
        if ( estado == 5 )
            y -= 15;
            angle = 0;
        end
        if ( estado == 6 )
            y += 15;
            angle = 0;
        end
        if ( estado == 3 )
            if ( sentido == 0 )
                x += 20;
                y -= 15;
                angle = 45000;
            end
            if ( sentido == 1 )
                x -= 20;
                y -= 15;
                angle = 315000;
            end
        end
        if ( estado == 15 )
            if ( sentido == 0 )
                x += 20;
                y += 10;
                if ( arma_seleccionada_p1 != 4 )
                    angle = 315000;
                end
                if ( arma_seleccionada_p1 == 4 )
                    angle = 337000;
                end
            end
            if ( sentido == 1 )
                x -= 20;
                y += 10;
                if ( arma_seleccionada_p1 != 4 )
                    angle = 45000;
                end
                if ( arma_seleccionada_p1 == 4 )
                    angle = 26000;
                end
            end
        end
        if ( estado == 1 )
            if ( sentido == 0 )
                x += 15;
            end
            if ( sentido == 1 )
                x -= 15;
            end
        end
        // colisiona contra el agua
        if ( collision( type agua ))
            salpicada( x, y, 100 );
            trozo_suelo( x, y -3, 377, 10 );
            trozo_suelo( x, y -3, 377, 20 );
            trozo_suelo( x, y -3, 377, 10 );
            existe_bala--;
            if ( estado != 10 AND estado != 8 AND estado != 13 AND estado != 14 AND estado != 6 AND estado != 17 )
                sound_play( splash, 0 );
            end
            signal( id, s_kill );
        end
        // colision con los enemigos de la pantalla
        if ( balafuera == 2 )
            if ( arma_seleccionada_p1 != 4 AND arma_seleccionada_p1 != 3 )
                if ( k_select == 6 )
                    explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                    onda_exp( x, y );
                end
                existe_bala--;
                disparos_acertados++;
                signal( id, s_kill );
            end
        end
        if ( id_e = collision( type enemigo_arabe ))
            if ( exists( id_e ))
                if ( id_e.priority != 1 )
                    balafuera = 2;
                end
            end
        end
        if ( id_e = collision( type enemigo_dispara ))
            if ( exists( id_e ))
                if ( id_e.priority != 1 )
                    balafuera = 2;
                end
            end
        end
        if ( collision( type elicoptero1 ) OR collision( type elicoptero2 ) OR collision( type helicoptero3 ) OR collision( type tanke ) OR collision( type robot ) OR collision( type monstruo1 ) OR collision( type canones ) OR collision( type lancha ) OR collision( type nave_inversa ) OR collision( type mini_nave ) OR collision( type monstruo3 ))
            balafuera = 2;
            trozo_suelo( x, y, 294, 1 );
            trozo_suelo( x, y, 294, 2 );
            trozo_suelo( x, y, 294, 3 );
        end
        // colision con los elementos de la pantalla
        if ( balafuera == 1 )
            if ( k_select != 4 AND k_select != 3 )
                if ( k_select == 6 )
                    explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                    onda_exp( x, y );
                end
                existe_bala--;
                polvo( x + rand( -6, 6 ), y - rand( -3, 3 ), 264, 3 );
                signal( id, s_kill );
            end
        end
        if ( collision( type carro ) OR collision( type elemento_destructible ) OR collision( type prohibido_elpaso ))
            trozo_suelo( x, y -3, 291, 1 );
            trozo_suelo( x, y -3, 291, 2 );
            trozo_suelo( x, y -3, 291, 1 );
            balafuera = 1;
        end
        if ( collision( type copa_palmera ))
            trozo_suelo( x, y -3, 292, 1 );
            trozo_suelo( x, y -3, 292, 2 );
            trozo_suelo( x, y -3, 292, 1 );
            balafuera = 1;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_pared_1 OR map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_pared_2 )
            trozo_suelo( x, y -3, 291, 1 );
            trozo_suelo( x, y -3, 291, 2 );
            trozo_suelo( x, y -3, 291, 1 );
            balafuera = 1;
        end
        // colisiona con el suelo
        if ( estado != 5 AND estado != 3 AND estado != 7 AND estado != 9 AND estado != 10 AND estado != 11 AND estado != 12
            AND map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y > 255 )
            if ( k_select == 6 )
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                onda_exp( x, y );
            end
            existe_bala--;
            polvo( x + rand( -6, 6 ), y - rand( -3, 3 ), 264, 3 );
            trozo_suelo( x, y -3, 282, 1 );
            trozo_suelo( x, y -3, 282, 2 );
            trozo_suelo( x, y -3, 282, 1 );
            signal( id, s_kill );
        end
        if ( region_out( id, 0 ) AND y > 0 )
            existe_bala--;
            signal( id, s_kill );
        end
        if ( y < -100 )
            existe_bala--;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS barracada( double x, double y, int tipo );
BEGIN
    z = 8;
    ctype = c_scroll;
    if ( tipo == 1 )
        file = fase1;
        graph = 236;
    end
    if ( tipo == 2 )
        file = fase1;
        graph = 234;
    end
    if ( tipo == 3 )
        file = fase1;
        graph = 235;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        frame;
    end
end


PROCESS pestanea( double x, double y, int flags );
PRIVATE
    int hazlo;
BEGIN
    graph = 317;
    ctype = c_scroll;
    z = -6;
    LOOP
        x = x_p1;
        y = y_p1;
        hazlo++;
        if ( hazlo == 1 )
            graph = 317;
        end
        if ( hazlo == 3 )
            graph = 318;
        end
        if ( hazlo == 5 )
            graph = 319;
        end
        if ( hazlo == 7 )
            graph = 318;
        end
        if ( hazlo == 9 )
            graph = 317;
        end
        if ( hazlo > 9 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS vaya_ostia( double x, double y, int flags );
PRIVATE
    int hazlo;
BEGIN
    graph = 316;
    ctype = c_scroll;
    z = -6;
    LOOP
        x = x_p1;
        y = y_p1;
        hazlo++;
        signal( type pestanea, s_kill );
        if ( hazlo > 2 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS resto_bala( double x, double y, int sentido );
PRIVATE
    int incr_y = -6;
    int rebote_resto;
    int alea_x;
    int una_vez;
BEGIN
    if ( modo_wiz == 1 )
        return;
    end
    ctype = c_scroll;
    z = -6;
    graph = 255;
    alea_x = rand( 2, 4 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        angle += 15000;
        incr_y++;
        y += incr_y;
        if ( collision( type agua ))
            salpicada( x, y, 50 );
            signal( id, s_kill );
        end
        if ( sentido == 0 )
            x -= alea_x;
        end
        if ( sentido == 1 )
            x += alea_x;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            incr_y = -3;
            alpha = 128;
            rebote_resto++;
            if ( una_vez == 0 )
                sound_play( casquete, 0 );
                una_vez = 1;
            end
        end
        if ( rebote_resto == 4 OR region_out( id, 0 ))
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS brillo_disparo( double x, double y, int sentido, int estado );
PRIVATE
    int esperate = 2;
BEGIN
    ctype = c_scroll;
    graph = 256;
    if ( estado == 3 )
        if ( sentido == 0 )
            angle = 45000;
        end
        if ( sentido == 1 )
            angle = 315000;
        end
    end
    if ( sentido == 0 )
        alpha = 128;
    end
    if ( sentido == 1 )
        flags = 5;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( esperate > 0 )
            esperate--;
        end
        if ( esperate == 0 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS brillo_cuerpo( double x, double y, int sentido, int player );
PRIVATE
    int esperate = 2;
BEGIN
    ctype = c_scroll;
    graph = 258;
    z = -6;
    if ( sentido == 0 )
        alpha = 128;
    end
    if ( sentido == 1 )
        flags = 5;
    end
    LOOP
        if ( esperate > 0 )
            esperate--;
        end
        if ( esperate == 0 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS vagoneta( double x, double y, int graph, int file );
PRIVATE
    double x_incr = 3;
    double y_incr = 5;
    double y_incr2 = -23;
    double y_incr3 = -45;
    int estado;
    int no_repitas;
    int explotas; // explota una vez al colisionar con piedras
    int sonido_vagon;
    int suena_ahora = 1;
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    z = -7;
    if ( estado_fase != 5 )
        scroll.camera = id;
    end
    if ( estado_fase == 5 )
        camara_juego1();
    end
    graph = 139;
    priority = 100;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        vagoneta_x = x;
        if ( explotas != 2 )
            x_p1 = x;
        end
        haz_graficos++;
        if ( haz_graficos == 1 )
            graph = 139;
        end
        if ( haz_graficos == 3 )
            graph = 144;
        end
        if ( haz_graficos == 5 )
            graph = 145;
        end
        if ( haz_graficos == 7 )
            graph = 146;
        end
        if ( haz_graficos == 9 )
            graph = 147;
        end
        if ( haz_graficos == 11 )
            graph = 148;
            haz_graficos = 1;
        end
        // en la pantalla de monstruo
        if ( estado_fase == 5 )
            if ( x > 2173 AND explotas == 0 )
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                explotas = 1;
            end
        end
        if ( explotas == 1 )
            y_incr3++;
            y += y_incr3;
            angle -= 20000;
            if ( y_incr3 > 0 )
                explotas = 2;
                signal( type jose_player1, s_kill );
                jose_player1( 2680, y );
                cacho_vagoneta( 2853, y );
            end
        end
        if ( explotas == 2 )
            if ( x > 2686 )
                x = 2686;
            end
            if ( y > -120 )
                y = -120;
            end
        end
        // en la mina
        if ( suena_ahora == 1 )
            suena_ahora = 2;
        end
        if ( no_repitas == 0 )
            x += x_incr;
        end
        if ( x > 7000 )
            x = 7000;
            y = 200;
        end
        if ( tipo_control1 == 1 AND estado_fase != 5 )
            if ( pulsa_x == 1 AND estado == 0 )
                estado = 1;
            end
        end
        if ( estado == 1 ) // vagoneta salta
            y_incr2++;
            y += y_incr2;
            if ( y_incr2 > 15 )
                y_incr2 = 15;
            end
        end
        if ( estado == 2 )
            limite_vida_player1--;
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            if ( limite_vida_player1 =< 0 )
                gameover();
                signal( id, s_freeze );
            end
            if ( limite_vida_player1 > 0 )
                estado = 3;
                fade_off( 0 );
                x = 466;
                y = 207;
                estado = 0;
                no_repitas = 0;
                angle = 0;
                fade_on( 0 );
            end
        end
        // colisiona con la via de la mina
        if (( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_1 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_2 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_3 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_4 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_5 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_6 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_7 )) AND
            ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_8 )))
            y += y_incr;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_0 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            y_incr = 4;
            x_incr = 4;
            y -= y_incr;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_1 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 0;
            y_incr = 8;
            x_incr = 8;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_2 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 18000;
            y_incr = 5;
            x_incr = 5;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_3 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 328000;
            y_incr = 12;
            x_incr = 12;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_4 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 352000;
            y_incr = 8;
            x_incr = 8;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_5 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 12000;
            y_incr = 9;
            x_incr = 9;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_6 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 357000;
            y_incr = 10;
            x_incr = 10;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_7 )
            if ( estado == 1 OR estado == 4 )
                sound_play( metal1, 0 );
                estado = 0;
                trozo_suelo( x, y + 20, 328, 1 );
                trozo_suelo( x, y + 20, 328, 2 );
                trozo_suelo( x, y + 20, 328, 3 );
                graph = 149;
            end
            angle = 9000;
            y_incr = 6;
            x_incr = 6;
            y -= y_incr;
            estado = 0;
            y_incr2 = -23;
        end
        // cae de la via
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_via_8 )
            if ( estado == 0 )
                sound_play( torta, 0, 13 );
            end
            estado = 4;
            y_incr = 15;
            y += 10;
        end
        if ( y > 625 )
            y = 625;
            if ( no_repitas == 0 )
                estado = 2;
                no_repitas = 1;
            end
        end
        frame;
    end
end


PROCESS cacho_vagoneta( double x, double y );
PRIVATE
    int incr_y;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = -6;
    graph = 134;
    LOOP
        x += 2;
        if ( incr_y < 15 )
            incr_y++;
        end
        y += incr_y;
        if ( y > 390 )
            signal( id, s_kill );
        end
        frame;
    end
end

// ENEMIGOS DE LA FASE 1 !!!!

PROCESS tanke( double x, double y, int graph, int file );
PRIVATE
    int dispara_ahora;
    int estado2 = -1;
    int vida_tanke = 10;
    int espera;
BEGIN
    // tipo de vehiculo
    z = -6;
    vida_tanke = vida_tanke + vidaenemigo;
    size = 120;
    ctype = c_scroll;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        estado2++;
        if ( estado2 < 3 )
            y++;
        end
        if ( estado2 => 3 )
            y--;
        end
        if ( estado2 == 5 )
            estado2 = -1;
        end
        if ( collision( type cacho_vagoneta ))
            vida_tanke = 0;
        end
        if ( espera > 0 )
            espera--;
        end
        if ( espera == 6 )
            graph = 32;
        end
        if ( espera == 4 )
            graph = 4;
        end
        if ( espera == 2 )
            graph = 32;
        end
        if ( vida_tanke =< 0 )
            puntuacion += 500;
            realiza_intro1++;
            prepara_bombazos();
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS rayazo();
PRIVATE
    int esperate;
BEGIN
    x = 320;
    y = 240;
    graph = 379;
    size = 200;
    alpha = 128;
    LOOP
        esperate++;
        if ( esperate == 8 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS disparo_monstruo( double x, double y, int tipo );
PRIVATE
    int incr_y = -10;
    int parpadeo;
BEGIN
    ctype = c_scroll;
    z = -10;
    graph = 622;
    sound_play( pistola, 0, 17 );
    LOOP
        incr_y++;
        y += incr_y;
        parpadeo++;
        if ( incr_y > 15 )
            incr_y = 15;
        end
        if ( angle != 128000 )
            angle += 8000;
        end
        if ( tipo == 1 )
            x -= 6;
        end
        if ( tipo == 2 )
            x -= 3;
        end
        if ( tipo == 3 )
            x += 2;
        end
        if ( tipo == 4 )
            x += 5;
        end
        if ( parpadeo == 0 )
            if ( tipo == 1 OR tipo == 2 )
                flags = 0;
            end
            if ( tipo == 3 OR tipo == 4 )
                flags = 1;
            end
        end
        if ( parpadeo == 2 )
            if ( tipo == 1 OR tipo == 2 )
                alpha = 128;
            end
            if ( tipo == 3 OR tipo == 4 )
                flags = 5;
            end
        end
        if ( parpadeo > 4 )
            parpadeo = -1;
        end
        if ( y > 520 )
            signal( id, s_kill );
        end
        if ( collision( type jose_player1 ))
            josel_tocao = 1;
        end
        frame;
    end
end


PROCESS monstruo1( double x, double y );
PRIVATE
    int estado2;
    int solo_una;
    int esperate_denuevo;
    int a_saco_paco;
    int rapidez = 1;
    int estado4;
    int disparando;
    int fogonazo;
    int cuando_fogon = 30;
    int estado3;
    int tiempo_y;
    int esperate;
    int vida_monstruo1 = 200;
    int estado;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = -3;
    graph = 264;
    monstruo_encargado();
    LOOP
        boss = 1;
        // hace disparos a lo loco cuando le quede poca vida
        if ( vida_monstruo1 == 120 )
            rapidez = 2;
        end
        if ( vida_monstruo1 < 10 AND solo_una == 0 )
            estado3 = 2;
            estado = 5;
            rapidez = 3;
            if ( estado3 == 2 )
                graph = 270;
                angle += 10000;
                esperate_denuevo++;
                disparando = 10;
                if ( esperate_denuevo == 5 )
                    disparo_monstruo( x, y -100, 1 );
                end
                if ( esperate_denuevo == 10 )
                    disparo_monstruo( x, y -100, 2 );
                end
                if ( esperate_denuevo == 15 )
                    disparo_monstruo( x, y -100, 1 );
                end
                if ( esperate_denuevo == 20 )
                    disparo_monstruo( x, y -100, 4 );
                end
                if ( esperate_denuevo == 25 )
                    disparo_monstruo( x, y -100, 3 );
                end
                if ( angle => 360000 )
                    estado = 0;
                    estado3 = 1;
                    graph = 264;
                    esperate_denuevo = 0;
                    solo_una = 1;
                    angle = 0;
                end
            end
        end
        if ( disparando == 1 )
            disparo_monstruo( x, 0, rand( 2, 3 ));
        end
        if ( disparando == 20 )
            disparo_monstruo( x -227, y -48, 1 );
        end
        if ( disparando == 30 )
            disparo_monstruo( x -171, y -59, 2 );
        end
        if ( disparando == 40 )
            disparo_monstruo( x + 172, y -59, 3 );
        end
        if ( disparando == 50 )
            disparo_monstruo( x + 228, y -48, 4 );
        end
        if ( disparando => 100 )
            disparando = 0;
        end
        // hace bajar y tambalea
        if ( estado3 == 0 AND y < 129 )
            y += 4;
            if ( y > 129 )
                estado3 = 1;
            end
        end
        if ( estado3 == 1 )
            estado2++;
            estado4++;
            disparando++;
            fogonazo++;
            if ( fogonazo => cuando_fogon )
                fogonazo = 0;
                cuando_fogon = rand( 30, 70 );
                sound_play( lanzalla, 0 );
                fogon( x -186, y + 98, 4 );
                fogon( x + 184, y + 98, 4 );
            end
            if ( estado2 <= 10 )
                y -= rapidez;
            end
            if ( estado2 > 10 )
                y += rapidez;
            end
            if ( estado4 <= 100 )
                x += rapidez;
            end
            if ( estado4 > 100 )
                x -= rapidez;
            end
            if ( estado4 => 200 )
                estado4 = 0;
            end
            if ( estado2 => 20 )
                estado2 = 0;
            end
        end
        // colisiona con las balas
        if ( estado == 0 )
            if ( collision( type bala ) OR collision( type explosion ))
                estado = 1;
                explosion2( rand( x -200, x + 200 ), rand( y -100, y + 100 ));
                vida_monstruo1--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
            end
            if ( collision( type llamazo ) OR collision( type escopetazo ))
                estado = 1;
                explosion2( rand( x -200, x + 200 ), rand( y -100, y + 100 ));
                vida_monstruo1--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
                disparos_acertados++;
            end
        end
        // al ser tocado
        if ( estado > 0 )
            estado++;
        end
        if ( estado == 1 )
            graph = 265;
        end
        if ( estado == 2 )
            graph = 264;
        end
        if ( estado == 3 )
            graph = 265;
        end
        if ( estado == 4 )
            graph = 264;
            estado = 0;
        end
        // muere cuando no tiene vida
        if ( vida_monstruo1 <= 0 )
            set_fps( 20, 0 );
            items( x_p1, 350, 23 );
            sound_play( explo3, 0, 23 );
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            puntuacion += 10000;
            cacho_heli( x, y, 266, fase1, 1 );
            cacho_heli( x, y, 267, fase1, 2 );
            cacho_heli( x, y, 268, fase1, 3 );
            cacho_heli( x, y, 266, fase1, 4 );
            cacho_heli( x - 30, y, 266, fase1, 1 );
            cacho_heli( x + 15, y, 269, fase1, 2 );
            cacho_heli( x + 30, y, 269, fase1, 3 );
            espera_monstruo();
            signal( type monstruo_encargado, s_kill );
            enemigos_tierra = 0;
            boss = 2;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS pincho_monstruo( double x, double y, int graph );
PRIVATE
    int balancea;
    int kierosaber;
BEGIN
    ctype = c_scroll;
    z = -4;
    file = fase1;
    size = 10;
    kierosaber = 90000;
    angle = 90000;
    LOOP
        if ( !exists( father ))
            return;
        end
        x = father.x;
        y = father.y + 120;
        if ( kierosaber >= 270000 )
            balancea = 1;
        end
        if ( kierosaber <= 90000 )
            balancea = 0;
        end
        if ( balancea == 0 )
            angle -= 5000;
            kierosaber += 5000;
        end
        if ( balancea == 1 )
            angle += 5000;
            kierosaber -= 5000;
        end
        if ( size < 100 )
            size += 10;
        end
        if ( collision( type jose_player1 ) AND graph == 302 )
            josel_tocao = 1;
        end
        frame;
    end
end


PROCESS canones( double x, double y, int tipo );
PRIVATE
    int estado;
    int vida_canones = 50;
    int dispara;
BEGIN
    ctype = c_scroll;
    file = fase1;
    if ( tipo == 1 )
        graph = 279;
    end
    if ( tipo == 2 )
        graph = 281;
    end
    if ( tipo == 3 )
        graph = 283;
    end
    if ( tipo == 4 )
        graph = 285;
    end
    if ( tipo == 5 )
        graph = 287;
    end
    if ( tipo == 6 )
        graph = 289;
    end
    priority = -120;
    LOOP
        dispara++;
        if ( !exists( father ))
            return;
        end
        if ( vida_canones =< 0 )
            cacho_heli( x, y, 297, fase1, 1 );
            cacho_heli( x, y, 298, fase1, 2 );
            cacho_heli( x, y, 299, fase1, 3 );
            cacho_heli( x, y, 300, fase1, 4 );
            puntuacion += 1000;
            vida_general--;
            signal( id, s_kill );
        end
        if ( tipo == 1 )
            x = 459;
            y = father.y -50;
            if ( dispara => 30 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 280;
            end
            if ( estado == 2 )
                graph = 279;
            end
            if ( estado == 3 )
                graph = 280;
            end
            if ( estado == 4 )
                graph = 279;
                estado = 0;
            end
        end
        if ( tipo == 2 )
            x = 405;
            y = father.y -88;
            if ( dispara => 35 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 282;
            end
            if ( estado == 2 )
                graph = 281;
            end
            if ( estado == 3 )
                graph = 282;
            end
            if ( estado == 4 )
                graph = 281;
                estado = 0;
            end
        end
        if ( tipo == 3 )
            x = 316;
            y = father.y + 45;
            if ( dispara => 40 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 284;
            end
            if ( estado == 2 )
                graph = 283;
            end
            if ( estado == 3 )
                graph = 284;
            end
            if ( estado == 4 )
                graph = 283;
                estado = 0;
            end
        end
        if ( tipo == 4 )
            x = 247;
            y = father.y + 68;
            if ( dispara => 45 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 286;
            end
            if ( estado == 2 )
                graph = 285;
            end
            if ( estado == 3 )
                graph = 286;
            end
            if ( estado == 4 )
                graph = 285;
                estado = 0;
            end
        end
        if ( tipo == 5 )
            x = 191;
            y = father.y + 64;
            if ( dispara => 50 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 288;
            end
            if ( estado == 2 )
                graph = 287;
            end
            if ( estado == 3 )
                graph = 288;
            end
            if ( estado == 4 )
                graph = 287;
                estado = 0;
            end
        end
        if ( tipo == 6 )
            x = 218;
            y = father.y -13;
            if ( dispara => 55 )
                dispara = 0;
                disparo_monstruo( x, y -10, rand( 1, 4 ));
            end
            if ( estado == 0 )
                if ( collision( type bala ) OR collision( type explosion ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                end
                if ( collision( type llamazo ) OR collision( type escopetazo ))
                    estado = 1;
                    explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                    vida_canones--;
                    sound_play( smetal1, 0, 21 );
                    puntuacion += 50;
                    disparos_acertados++;
                end
            end
            // al ser tocado
            if ( estado > 0 )
                estado++;
            end
            if ( estado == 1 )
                graph = 290;
            end
            if ( estado == 2 )
                graph = 289;
            end
            if ( estado == 3 )
                graph = 290;
            end
            if ( estado == 4 )
                graph = 289;
                estado = 0;
            end
        end
        frame;
    end
end


PROCESS monstruo2( double x, double y );
PRIVATE
    int estado2;
    int solo_una;
    int bombons;
    int esperate_denuevo;
    int a_saco_paco;
    int rapidez = 4;
    int estado4;
    int disparando;
    int fogonazo;
    int cuando_fogon = 30;
    int estado3 = 1;
    int tiempo_y;
    int esperate;
    int vida_monstruo1 = 250;
    int estado;
    int programa_atake;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = 1;
    graph = 278;
    vida_general = 6;
    enemigo_arabe( 625, 200, 1, 1, 5 );
    enemigo_arabe( 625, 200, 1, 1, 5 );
    canones( x, y, 1 );
    canones( x, y, 2 );
    canones( x, y, 3 );
    canones( x, y, 4 );
    canones( x, y, 5 );
    canones( x, y, 6 );
    LOOP
        boss = 1;
        // hace bajar y tambalea
        if ( estado3 == 1 )
            estado2++;
            estado4++;
            disparando++;
            if ( estado2 <= 10 )
                y--;
            end
            if ( estado2 > 10 )
                y++;
            end
            if ( estado4 => 200 )
                estado4 = 0;
            end
            if ( estado2 => 20 )
                estado2 = 0;
            end
        end
        // muere cuando no tiene vida
        if ( vida_general <= 0 )
            set_fps( 20, 0 );
            items( x_p1, 350, 24 );
            sound_play( explo3, 0, 23 );
            signal( type pincho_monstruo, s_kill );
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            cacho_heli( x, y, 301, fase1, 1 );
            cacho_heli( x, y, 302, fase1, 2 );
            cacho_heli( x, y, 303, fase1, 3 );
            cacho_heli( x, y, 304, fase1, 4 );
            cacho_heli( x - 30, y, 301, fase1, 1 );
            cacho_heli( x + 15, y, 302, fase1, 2 );
            cacho_heli( x + 30, y, 303, fase1, 3 );
            espera_monstruo();
            enemigos_tierra = 0;
            boss = 2;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS rayo_monstruo3();
PRIVATE
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    x = 339;
    z = -4;
    y = 141;
    file = fase1;
    graph = 252;
    flags = 16;
    LOOP
        haz_graficos++;
        size += 4;
        if ( haz_graficos == 1 )
            graph = 252;
        end
        if ( haz_graficos == 3 )
            graph = 253;
        end
        if ( haz_graficos == 5 )
            graph = 254;
        end
        if ( haz_graficos == 7 )
            graph = 255;
        end
        if ( haz_graficos == 9 )
            graph = 256;
        end
        if ( haz_graficos == 11 )
            graph = 257;
        end
        if ( haz_graficos == 13 )
            graph = 258;
            rayazo();
        end
        if ( haz_graficos => 17 )
            fase3_c(); // carga la siguiente escena
            signal( id, s_freeze );
        end
        frame;
    end
end


PROCESS asteroide();
PRIVATE
    int velocidad;
BEGIN
    ctype = c_scroll;
    velocidad = rand( 3, 5 );
    file = fase1;
    graph = 263;
    x = 750;
    y = rand( 30, 440 );
    size = rand( 40, 100 );
    LOOP
        x -= velocidad;
        if ( graph < 280 )
            graph++;
        end
        if ( graph => 280 )
            graph = 263;
        end
        if ( x < -100 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS prepara_asteroides();
PRIVATE
    int espera;
    int cuanto_espera;
BEGIN
    ctype = c_scroll;
    cuanto_espera = rand( 20, 40 );
    LOOP
        espera++;
        if ( espera => cuanto_espera )
            asteroide();
            espera = 0;
            cuanto_espera = rand( 20, 40 );
        end
        frame;
    end
end


PROCESS nave_monstruo3();
PRIVATE
    int hazlo;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = -3;
    graph = 251;
    x = 320;
    y = -150;
    LOOP
        if ( limite_vida_player1 <= 0 )
            signal( id, s_freeze );
        end
        if ( y < 100 )
            y += 2;
        end
        if ( y => 100 AND hazlo == 0 )
            hazlo = 1;
            rayo_monstruo3();
        end
        frame;
    end
end


PROCESS mini_nave( double x, double y, int tipo );
PRIVATE
    int dispara;
    int cuando_dispara = 45;
    int disparando;
    int estados;
    int vida_mini_nave = 3;
    int estado4;
BEGIN
    ctype = c_scroll;
    file = fase1;
    graph = 250;
    z = -4;
    size = 10;
    LOOP
        if ( size > 50 )
            dispara++;
        end
        if ( dispara => cuando_dispara )
            if ( flags == 0 )
                bala_enemigo( x, y + 50, 361, 0, 20, flags );
                sound_play( laser6, 0, 14 );
            end
            if ( flags == 1 )
                bala_enemigo( x, y + 50, 361, 0, 21, flags );
                sound_play( laser6, 0, 14 );
            end
            dispara = 0;
            cuando_dispara = rand( 20, 40 );
        end
        if ( size < 100 )
            size++;
        end
        if ( x < -200 )
            signal( id, s_kill );
        end
        if ( x > 800 )
            signal( id, s_kill );
        end
        if ( y > 550 )
            signal( id, s_kill );
        end
        if ( vida_mini_nave =< 0 OR boss == 2 )
            sound_play( dish, 0 );
            cacho_heli( x, y, 315, fase1, 1 );
            cacho_heli( x, y, 316, fase1, 2 );
            cacho_heli( x, y, 317, fase1, 3 );
            cacho_heli( x, y, 318, fase1, 4 );
            signal( id, s_kill );
        end
        if ( estado4 == 0 AND size > 50 )
            if ( collision( type bala ) OR collision( type explosion ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_mini_nave--;
                puntuacion += 50;
                disparos_acertados++;
            end
            if ( collision( type llamazo ) OR collision( type escopetazo ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_mini_nave--;
                puntuacion += 50;
            end
        end
        // al ser tocado
        if ( estado4 > 0 )
            estado4++;
        end
        if ( estado4 == 1 )
            graph = 314;
        end
        if ( estado4 == 2 )
            graph = 250;
        end
        if ( estado4 == 3 )
            graph = 314;
        end
        if ( estado4 == 4 )
            graph = 250;
            estado4 = 0;
        end
        if ( tipo == 1 AND disparando == 0 )
            x -= 3;
            y++;
            flags = 1;
        end
        if ( tipo == 2 AND disparando == 0 )
            x += 3;
            y++;
        end
        if ( tipo == 3 AND disparando == 0 )
            x -= 6;
            y += 2;
            flags = 1;
        end
        if ( tipo == 4 AND disparando == 0 )
            x += 6;
            y += 2;
        end
        if ( tipo == 5 AND disparando == 0 )
            if ( x > 100 AND estados == 0 )
                x -= 5;
                flags = 1;
            end
            if ( x < 100 )
                estados = 1;
            end
            if ( estados == 1 )
                y += 2;
            end
        end
        if ( tipo == 6 AND disparando == 0 )
            if ( x < 580 AND estados == 0 )
                x += 5;
            end
            if ( x > 580 )
                estados = 1;
            end
            if ( estados == 1 )
                y += 2;
            end
        end
        frame;
    end
end


PROCESS nave_inversa();
PRIVATE
    int hazlo;
    int estado4;
    int cuando_lohace = 30;
    int vida_monstruo_final = 300;
    int estado2;
    int k_tira;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = -3;
    graph = 251;
    x = 320;
    y = 100;
    rayo_inverso();
    prepara_asteroides();
    LOOP
        hazlo++;
        estado2++;
        if ( hazlo => cuando_lohace )
            k_tira = rand( 1, 6 );
            mini_nave( 334, 147, k_tira );
            hazlo = 0;
            cuando_lohace = rand( 20, 50 );
        end
        if ( estado2 <= 10 )
            y--;
        end
        if ( estado2 > 10 )
            y++;
        end
        if ( estado2 => 20 )
            estado2 = 0;
        end
        if ( estado4 == 0 )
            if ( collision( type bala ) OR collision( type explosion ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_monstruo_final--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
                disparos_acertados++;
            end
            if ( collision( type llamazo ) OR collision( type escopetazo ))
                estado4 = 1;
                explosion2( rand( x -50, x + 50 ), rand( y -25, y + 25 ));
                vida_monstruo_final--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
            end
        end
        // al ser tocado
        if ( estado4 > 0 )
            estado4++;
        end
        if ( estado4 == 1 )
            graph = 309;
        end
        if ( estado4 == 2 )
            graph = 251;
        end
        if ( estado4 == 3 )
            graph = 309;
        end
        if ( estado4 == 4 )
            graph = 251;
            estado4 = 0;
        end
        if ( vida_monstruo_final =< 0 )
            set_fps( 20, 0 );
            items( x_p1, 350, 25 );
            sound_play( explo3, 0, 23 );
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( 100, y );
            super_onda( 320, y );
            super_onda( 600, y );
            puntuacion += 20000;
            cacho_heli( x, y, 310, fase1, 1 );
            cacho_heli( x, y, 311, fase1, 2 );
            cacho_heli( x, y, 312, fase1, 3 );
            cacho_heli( x, y, 313, fase1, 4 );
            cacho_heli( x - 30, y, 313, fase1, 1 );
            cacho_heli( x + 15, y, 312, fase1, 2 );
            cacho_heli( x + 30, y, 311, fase1, 3 );
            espera_monstruo();
            signal( type monstruo_encargado, s_kill );
            enemigos_tierra = 0;
            boss = 2;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS rayo_inverso();
PRIVATE
    int haz_graficos;
BEGIN
    FRAME( 1000 );
    ctype = c_scroll;
    x = 339;
    z = -4;
    y = 141;
    file = fase1;
    graph = 258;
    size = 150;
    flags = 16;
    LOOP
        haz_graficos++;
        size -= 4;
        if ( haz_graficos == 13 )
            graph = 252;
        end
        if ( haz_graficos == 11 )
            graph = 253;
        end
        if ( haz_graficos == 9 )
            graph = 254;
        end
        if ( haz_graficos == 7 )
            graph = 255;
        end
        if ( haz_graficos == 5 )
            graph = 256;
        end
        if ( haz_graficos == 3 )
            graph = 257;
        end
        if ( haz_graficos == 1 )
            graph = 258;
            rayazo();
        end
        if ( haz_graficos => 15 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS monstruo3( double x, double y );
PRIVATE
    int estado2;
    int solo_una;
    int bombons;
    int estado3 = 0;
    int esperate_denuevo;
    int a_saco_paco;
    int rapidez = 4;
    int estado4;
    int disparando;
    int fogonazo;
    int cuando_fogon = 30;
    int tiempo_y;
    int esperate;
    int vida_monstruo1 = 250;
    int estado;
    int programa_atake;
BEGIN
    file = fase1;
    ctype = c_scroll;
    z = -3;
    graph = 241;
    LOOP
        boss = 1;
        if ( programa_atake == 2 )
            pincho_monstruo( x, y, 247 );
            pincho_monstruo( x, y, 302 );
            programa_atake = 3;
        end
        if ( programa_atake == 0 )
            bombons++;
            if ( bombons => 100 )
                torpedo3( x, y + 110 );
                sound_play( lanzalla, 0 );
                bombons = 0;
            end
        end
        if ( vida_monstruo1 < 150 AND programa_atake == 0 )
            programa_atake = 1;
        end
        if ( vida_monstruo1 < 75 AND programa_atake == 1 )
            programa_atake = 2;
        end
        if ( programa_atake == 1 )
            fogonazo++;
            if ( fogonazo => cuando_fogon )
                fogonazo = 0;
                cuando_fogon = rand( 30, 70 );
                sound_play( lanzalla, 0 );
                torpedo( x, y + 110 );
            end
        end
        if ( disparando == 1 )
            disparo_monstruo( x, 0, rand( 2, 3 ));
        end
        if ( disparando == 20 )
            disparo_monstruo( x, 0, 1 );
        end
        if ( disparando == 30 )
            disparo_monstruo( x, 0, 2 );
        end
        if ( disparando == 40 )
            disparo_monstruo( x, 0, 3 );
        end
        if ( disparando == 50 )
            disparo_monstruo( x, 0, 4 );
        end
        if ( disparando => 100 )
            disparando = 0;
        end
        // hace bajar y tambalea
        if ( estado3 == 0 AND y < 129 )
            y += 4;
            if ( y > 129 )
                estado3 = 1;
            end
        end
        if ( estado3 == 1 )
            estado2++;
            estado4++;
            disparando++;
            if ( estado2 <= 10 )
                y -= 2;
            end
            if ( estado2 > 10 )
                y += 2;
            end
            if ( estado4 <= 100 )
                x += rapidez;
            end
            if ( estado4 > 100 )
                x -= rapidez;
            end
            if ( estado4 => 200 )
                estado4 = 0;
            end
            if ( estado2 => 20 )
                estado2 = 0;
            end
        end
        // colisiona con las balas
        if ( estado == 0 )
            if ( collision( type bala ) OR collision( type explosion ))
                estado = 1;
                explosion2( rand( x -200, x + 200 ), rand( y -100, y + 100 ));
                vida_monstruo1--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
            end
            if ( collision( type llamazo ) OR collision( type escopetazo ))
                estado = 1;
                explosion2( rand( x -200, x + 200 ), rand( y -100, y + 100 ));
                vida_monstruo1--;
                sound_play( smetal1, 0, 21 );
                puntuacion += 50;
                disparos_acertados++;
            end
        end
        // al ser tocado
        if ( estado > 0 )
            estado++;
        end
        if ( estado == 1 )
            graph = 242;
        end
        if ( estado == 2 )
            graph = 241;
        end
        if ( estado == 3 )
            graph = 242;
        end
        if ( estado == 4 )
            graph = 241;
            estado = 0;
        end
        // muere cuando no tiene vida
        if ( vida_monstruo1 <= 0 )
            items( x_p1, 300, 26 );
            set_fps( 20, 0 );
            sound_play( explo3, 0 );
            sound_play( explo3, 0 );
            signal( type pincho_monstruo, s_kill );
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            cacho_heli( x, y, 243, fase1, 1 );
            cacho_heli( x, y, 244, fase1, 2 );
            cacho_heli( x, y, 245, fase1, 3 );
            cacho_heli( x, y, 246, fase1, 4 );
            cacho_heli( x -30, y, 243, fase1, 1 );
            cacho_heli( x + 15, y, 244, fase1, 2 );
            cacho_heli( x + 30, y, 245, fase1, 3 );
            espera_monstruo();
            enemigos_tierra = 0;
            nave_monstruo3();
            boss = 2;
            puntuacion += 10000;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS monstruo_encargado();
PRIVATE
    int rayo;
    int rayo_cuando = 100;
    int tira_gente;
    int tira_cuando = 10;
BEGIN
    LOOP
        rayo++;
        if ( rayo == rayo_cuando )
            rayo = 0;
            rayo_cuando = rand( 200, 500 );
            sound_play( trueno, 0 );
            rayazo();
        end
        frame;
    end
end


PROCESS final_juego();
PRIVATE
    int esperate = 30; // espera a ke la pantalla se oscurezca del todo
    int carga_todo = 1;
BEGIN
    // dump_type=1; restore_type=1;
    abre_fase( 3 );
    abre_fase( 4 );
    LOOP
        if ( carga_todo == 1 )
            esperate--;
        end
        if ( esperate =< 0 AND carga_todo == 1 )
            // kitando el escenario anterior
            fade_off( 0 );
            let_me_alone();
            clear_screen();
            write_delete( all_text );
            scroll_stop( 0 );
            // carga escenario
            scrolls = fpg_load( "fpg/scroll22.fpg" );
            fase1 = fpg_load( "fpg/fase6.fpg" );
            // Optimizando juego, descargando y cargando nuevos scroll
            estado_fase = 26;
            controles();
            // Cambia cancion si hace falta
            sound_stop( all_sound );
            music_stop();
            haz_reflejos = 0;
            if ( objetivos_d != "completo" )
                s_fase1 = music_load( "sound/fase5.mp3" );
                cuadro_final( 1 );
            else
                s_fase1 = music_load( "sound/bongomix.mp3" );
                cuadro_final( 2 );
            end
            // objetos , escenario
            // efectos al empezar
            fade_on( 0 );
            signal( type abre_fase, s_kill );
            abre_fase( 8 );
            abre_fase( 9 );
            // evita ke se repitan las mismas operaciones
            carga_todo = 0;
            esperate = 5;
            musica_fase = music_play( s_fase1, -1 );
            music_set_volume( vol );
        end
        frame;
    end
end


PROCESS cuadro_final( int tipo );
BEGIN
    file = fase1;
    x = 320;
    y = 240;
    z = -8;
    if ( tipo == 1 )
        graph = 319;
    end
    if ( tipo == 2 )
        graph = 320;
        nostalgia();
    end
    LOOP
        if ( pulsa_enter == 1 OR pulsa_c == 1 )
            sound_play( aceptar, 0, 16 );
            music_stop();
            abre_fase( 3 );
            abre_fase( 4 ); // efecto ke oscurece la pantalla
            extermina_partida(); // proceso que iniciara el menu
            x_p1 = 0;
            existe_explosion = 0;
            signal( id, s_freeze );
        end
        frame;
    end
end


PROCESS nostalgia();
PRIVATE
    int tiempo;
    int estado;
BEGIN
    file = fase1;
    graph = 321;
    x = -1000;
    y = 240;
    z = -10;
    LOOP
        if ( estado == 0 )
            x += 20;
            if ( x > 320 )
                x = 320;
                frame( 10000 );
                estado = 1;
            end
        end
        if ( estado == 1 )
            x -= 20;
            if ( x < -320 )
                x = -320;
                graph++;
                if ( graph < 323 )
                    flags = 128;
                else
                    flags = 0;
                end
                estado = 0;
                if ( graph > 329 )
                    graph = 321;
                end
            end
        end
        frame;
    end
end


PROCESS espera_monstruo();
PRIVATE
    int esperate;
BEGIN
    LOOP
        if ( esperate < 45 )
            boom = 1;
        end
        esperate++;
        if ( limite_vida_player1 =< 0 )
            signal( id, s_freeze );
        end
        if ( esperate > 60 AND boom == 0 )
            objetivos_a = "completo";
            objetivos_a1 = 1;
            if ( estado_fase != 24 )
                objetivos_b = "incompleto";
                objetivos_c = "incompleto";
                objetivos_d = "-";
            end
            set_fps( velocidad_juego, 4 );
            if ( estado_fase == 5 )
                if ( objetos_llave > 21 )
                    objetivos_b = "completo";
                end
                if ( objetos_muelle > 0 AND objetos_moneda > 0 AND objetos_chip > 0 AND objetos_teletransportador > 0 AND objetos_melon > 0 )
                    objetivos_c = "completo";
                end
                fin_mision( 1 );
                boss = 0;
                signal( id, s_kill );
            end
            if ( estado_fase == 10 )
                if ( objetos_combustible > 13 )
                    objetivos_b = "completo";
                end
                if ( objetos_vasoagua > 0 AND objetos_reductor > 0 AND objetos_ampliador > 0 AND objetos_joselillo > 0 AND objetos_pirana > 0 )
                    objetivos_c = "completo";
                end
                fin_mision( 2 );
                boss = 0;
                signal( id, s_kill );
            end
            if ( estado_fase == 24 )
                boss = 0;
                signal( id, s_kill );
            end
            if ( estado_fase == 25 )
                if ( objetos_regalo > 29 )
                    objetivos_b = "completo";
                end
                if ( objetos_joselxdye > 0 AND objetos_navecilla > 0 AND objetos_ovni > 0 )
                    objetivos_c = "completo";
                end
                if ( objetos_muelle > 0 AND objetos_moneda > 0 AND objetos_chip > 0 AND objetos_teletransportador > 0 AND objetos_vasoagua > 0 AND objetos_reductor > 0 AND objetos_ampliador > 0
                    AND objetos_joselillo > 0 AND objetos_navecilla > 0 AND objetos_melon > 0 AND objetos_pirana > 0 AND objetos_joselxdye > 0 AND objetos_ovni > 0 AND objetos_llave > 21 AND objetos_combustible > 13 AND objetos_regalo > 29 )
                    objetivos_d = "completo";
                else
                    objetivos_d = "incompleto";
                end
                fin_mision( 3 );
                boss = 0;
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS daldalboom( double x, double y, int estado );
PRIVATE
    double y_incr;
BEGIN
    ctype = c_scroll;
    z = -6;
    graph = 625;
    total_disparos++;
    if ( estado == 3 OR estado == 4 OR estado == 5 )
        y_incr = -18;
    end
    LOOP
        if ( y_incr < 15 )
            y_incr++;
        end
        y += y_incr;
        // derecha
        if ( estado == 0 )
            angle -= 10000;
            x += 10;
        end
        if ( estado == 4 )
            angle -= 10000;
            x += 10;
        end
        // izquierda
        if ( estado == 1 )
            angle += 10000;
            x -= 10;
        end
        if ( estado == 5 )
            angle += 10000;
            x -= 10;
        end
        if ( y > 480 AND estado_fase != 24 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            signal( id, s_kill );
        end
        if ( y > 5000 AND estado_fase == 24 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            signal( id, s_kill );
        end
        if ( collision( type prohibido_elpaso ) OR collision( type elicoptero1 ) OR collision( type elicoptero2 ) OR collision( type helicoptero3 ) OR collision( type monstruo1 ) OR collision( type nave_inversa ) OR collision( type monstruo3 ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            signal( id, s_kill );
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y_incr > 5 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS super_onda( double x, double y );
PRIVATE
    int haz_graficos;
    int guarda_boss;
BEGIN
    size = 20;
    graph = 628;
    ctype = c_scroll;
    z = -50;
    alpha = 128;
    guarda_boss = boss;
    boss = 2;
    flags = 16;
    priority = 5;
    LOOP
        haz_graficos++;
        boom = 1;
        if ( haz_graficos == 1 )
            graph = 628;
            boss = guarda_boss;
        end
        if ( haz_graficos == 2 )
            graph = 629;
        end
        if ( haz_graficos == 3 )
            graph = 630;
        end
        if ( haz_graficos == 4 )
            graph = 631;
        end
        if ( haz_graficos == 5 )
            graph = 630;
        end
        if ( haz_graficos == 6 )
            graph = 629;
        end
        if ( haz_graficos == 7 )
            graph = 628;
        end
        if ( haz_graficos => 8 )
            haz_graficos = 0;
        end
        size += 10;
        if ( size => 200 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS mini_onda( double x, double y );
PRIVATE
    int haz_graficos;
    int guarda_boss;
BEGIN
    size = 100;
    graph = 635;
    ctype = c_scroll;
    z = -50;
    alpha = 128;
    sound_play( decide, 0, 16 );
    LOOP
        haz_graficos++;
        if ( haz_graficos == 1 )
            graph = 635;
        end
        if ( haz_graficos == 2 )
            graph = 636;
        end
        if ( haz_graficos == 3 )
            graph = 637;
        end
        if ( haz_graficos == 4 )
            graph = 638;
        end
        if ( haz_graficos == 5 )
            graph = 637;
        end
        if ( haz_graficos == 6 )
            graph = 636;
        end
        if ( haz_graficos == 7 )
            graph = 635;
        end
        if ( haz_graficos => 8 )
            haz_graficos = 0;
        end
        size += 10;
        if ( collision( type enemigo_arabe ) OR collision( type enemigo_dispara ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            sound_play( explo3, 0, 23 );
            signal( id, s_kill );
        end
        if ( size => 200 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS movilatak( double x, double y, int estado );
PRIVATE
    double y_incr;
    int crea_minionda = 50;
    int cuenta_minionda;
    int rebotes = 2;
BEGIN
    ctype = c_scroll;
    z = -4;
    graph = 627;
    total_disparos++;
    if ( estado == 3 OR estado == 4 OR estado == 5 )
        y_incr = -15;
    end
    LOOP
        if ( y_incr < 15 )
            y_incr++;
        end
        if ( rebotes > 0 )
            y += y_incr;
            if ( estado == 0 )
                angle -= 10000;
                x += 10;
            end
            if ( estado == 4 )
                angle -= 10000;
                x += 10;
            end
            // izquierda
            if ( estado == 1 )
                angle += 10000;
                x -= 10;
            end
            if ( estado == 5 )
                angle += 10000;
                x -= 10;
            end
        end
        if ( rebotes == 0 )
            cuenta_minionda++;
            if ( cuenta_minionda => crea_minionda )
                cuenta_minionda = 0;
                mini_onda( x, y );
            end
        end
        // derecha
        if ( collision( type explosion ))
            signal( id, s_kill );
        end
        if ( y > 480 AND estado_fase != 24 )
            signal( id, s_kill );
        end
        if ( y > 5000 AND estado_fase == 24 )
            signal( id, s_kill );
        end
        if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND ( y_incr > 5 ) AND ( rebotes > 0 ))
            y_incr = -5;
            rebotes--;
        end
        frame;
    end
end


PROCESS ultraboom( double x, double y, int estado );
PRIVATE
    double y_incr;
BEGIN
    ctype = c_scroll;
    z = -6;
    graph = 626;
    total_disparos++;
    if ( estado == 3 OR estado == 4 OR estado == 5 )
        y_incr = -15;
    end
    LOOP
        if ( y_incr < 15 )
            y_incr++;
        end
        y += y_incr;
        // derecha
        if ( estado == 0 )
            angle -= 10000;
            x += 10;
        end
        if ( estado == 4 )
            angle -= 10000;
            x += 10;
        end
        // izquierda
        if ( estado == 1 )
            angle += 10000;
            x -= 10;
        end
        if ( estado == 5 )
            angle += 10000;
            x -= 10;
        end
        if ( y > 480 AND estado_fase != 24 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            llamazo( x, y, 1 );
            llamazo( x, y, 2 );
            llamazo( x, y, 3 );
            llamazo( x, y, 4 );
            signal( id, s_kill );
        end
        if ( y > 5000 AND estado_fase == 24 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            llamazo( x, y, 1 );
            llamazo( x, y, 2 );
            llamazo( x, y, 3 );
            llamazo( x, y, 4 );
            signal( id, s_kill );
        end
        if ( collision( type prohibido_elpaso ) OR collision( type elicoptero1 ) OR collision( type elicoptero2 ) OR collision( type nave_inversa ) OR collision( type helicoptero3 ) OR collision( type monstruo1 ) OR collision( type monstruo3 ))
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            llamazo( x, y, 1 );
            llamazo( x, y, 2 );
            llamazo( x, y, 3 );
            llamazo( x, y, 4 );
            signal( id, s_kill );
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y_incr > 5 )
            explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
            super_onda( x, y );
            sound_play( explo3, 0, 23 );
            llamazo( x, y, 1 );
            llamazo( x, y, 2 );
            llamazo( x, y, 3 );
            llamazo( x, y, 4 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS prepara_bombazos();
PRIVATE
    int una;
    int espera = 15;
    int rayo_cuando;
    int rayo;
    int espera1 = 15;
BEGIN
    rayo_cuando = 100;
    LOOP
        una++;
        if ( realiza_intro1 => 18 AND una == 170 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
            signal( id, s_kill );
        end
        if ( realiza_intro1 == 17 AND una == 160 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 16 AND una == 150 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 15 AND una == 140 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 14 AND una == 130 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 13 AND una == 120 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 12 AND una == 110 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            sound_play( lluvia, -1 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 11 AND una == 100 )
            lluvia1();
            lluvia1();
            lluvia1();
            lluvia1();
            realiza_intro1++;
        end
        if ( realiza_intro1 == 10 AND una == 90 ) // fade(88,88,88,3);
            realiza_intro1++;
            monstruo1( 2637, -250 );
        end
        if ( realiza_intro1 == 9 AND una == 80 )
            bombazos( 2851, 332 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 8 AND una == 70 )
            bombazos( 2707, 348 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 7 AND una == 60 )
            bombazos( 2968, 290 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 6 AND una == 50 )
            bombazos( 2865, 237 );
            enemigo_arabe( 2995, 14, 1, 1, 2 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 5 AND una == 40 )
            bombazos( 2689, 228 );
            enemigo_arabe( 2995, 14, 1, 1, 2 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 4 AND una == 30 )
            bombazos( 2618, 299 );
            enemigo_arabe( 2995, 14, 1, 1, 2 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 3 AND una == 20 )
            bombazos( 2793, 166 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 2 AND una == 10 )
            bombazos( 2931, 174 );
            realiza_intro1++;
        end
        if ( realiza_intro1 == 1 AND una == 1 )
            bombazos( 2645, 164 );
            realiza_intro1++;
        end
        frame;
    end
end


PROCESS bola_efecto( double x, double y, int graph, int file, int tipo );
PRIVATE
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    z = 1;
    alpha = 128;
    size = 80;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        haz_graficos++;
        if ( tipo == 2 )
            angle += 5000;
        end
        if ( haz_graficos == 1 )
            graph = 237;
        end
        if ( haz_graficos == 2 )
            graph = 238;
        end
        if ( haz_graficos == 3 )
            graph = 239;
        end
        if ( haz_graficos == 4 )
            graph = 240;
        end
        if ( haz_graficos => 5 )
            haz_graficos = 0;
        end
        frame;
    end
end


PROCESS lluvia1();
BEGIN
    x = rand( 100, 900 );
    y = -10;
    z = -50;
    graph = 377;
    alpha = 128;
    LOOP
        x -= 15;
        y += 15;
        if ( y > 640 )
            lluvia1();
            signal( id, s_kill );
        end
        if ( collision( type agua ))
            salpicada( x, y, 30 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS enemigo_atropellado( double x, double y, int graph, int file );
PRIVATE
    int grita; // cuando salga atropellada gritar 
    double x_incr;
    double y_incr;
    int estado;
    int tipo_muerte;
BEGIN
    ctype = c_scroll;
    z = -5;
    flags = 1;
    tipo_muerte = rand( 1, 2 );
    LOOP
        if ( y_incr > 20 )
            y_incr = 20;
        end
        if ( y > 640 )
            clones_eliminados++;
            puntuacion += 50;
            signal( id, s_kill );
        end
        if ( collision( type vagoneta ))
            estado = tipo_muerte;
            x_incr = rand( -25, 25 );
            y_incr = rand( -15, -30 );
            grita++;
        end
        if ( grita == 1 )
            sound_play( golpe13, 0 );
            grita = 2;
        end
        if ( estado == 1 )
            x += x_incr;
            y_incr++;
            y += y_incr;
            angle += 10000;
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 54, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 55, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 56, fase1, 1 );
                puntuacion += 50;
                clones_eliminados++;
                signal( id, s_kill );
            end
        end
        if ( estado == 2 )
            piedras( x, y, 568, rand( 70, 95 ), 1 );
            piedras( x, y, 569, rand( 70, 95 ), 2 );
            piedras( x, y, 570, rand( 70, 95 ), 3 );
            piedras( x, y, 571, rand( 70, 95 ), 4 );
            piedras( x, y, 572, rand( 70, 95 ), 5 );
            piedras( x, y, 568, rand( 70, 95 ), 6 );
            piedras( x, y, 568, rand( 70, 95 ), 7 );
            piedras( x, y, 569, rand( 70, 95 ), 8 );
            piedras( x, y, 570, rand( 70, 95 ), 9 );
            piedras( x, y, 571, rand( 70, 95 ), 10 );
            piedras( x, y, 572, rand( 70, 95 ), 11 );
            piedras( x, y, 568, rand( 70, 95 ), 12 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS enemigo_achicharrao( double x, double y );
PRIVATE
    double y_incr;
    double x_incr;
    int haz_graficos;
    int grito_aleatorio;
BEGIN
    grito_aleatorio = rand( 1, 3 );
    if ( grito_aleatorio == 1 )
        sound_play( golpe9, 0 );
    end
    if ( grito_aleatorio == 2 )
        sound_play( argh, 0 );
    end
    if ( grito_aleatorio == 3 )
        sound_play( kema, 0 );
    end
    ctype = c_scroll;
    z = -5;
    size = 200;
    graph = 507;
    y_incr = -14;
    x_incr = rand( -4, 4 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        y_incr++;
        y += y_incr;
        x += x_incr;
        angle += 10000;
        // salpica al sumergirse en el agua
        if ( y_incr > 5 )
            if ( collision( type agua ))
                salpicada( x, y, 170 );
                trozo_suelo( x, y -3, 377, 10 );
                trozo_suelo( x, y -3, 377, 20 );
                trozo_suelo( x, y -3, 377, 10 );
                sound_play( splash, 0 );
                signal( id, s_kill );
            end
        end
        if ( y > 800 )
            signal( id, s_kill );
        end
        if ( haz_graficos < 24 )
            haz_graficos++;
            graph = 506 + haz_graficos;
        end
        frame;
    end
end


PROCESS sangre_mix( int file, double x, double y, int flags, int tipo );
PRIVATE
    int haz_graficos;
    int select;
    int semaforo;
BEGIN
    ctype = c_scroll;
    z = -7;
    size = 200;
    y -= 40;
    if ( flags == 0 AND select == 0 )
        select = 1;
        flags = 1;
        x += 20;
    end
    if ( flags == 1 AND select == 0 )
        select = 1;
        flags = 0;
        x -= 20;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    alpha = 180;
    LOOP
        // sangre normal
        if ( tipo == 0 )
            haz_graficos++;
            graph = 530 + haz_graficos;
            if ( haz_graficos == 17 )
                signal( id, s_kill );
            end
        end
        // explosion de sangre
        if ( tipo == 1 )
            if ( semaforo == 0 )
                haz_graficos++;
                semaforo = 1;
            else
                semaforo = 0;
            end
            graph = 669 + haz_graficos;
            if ( haz_graficos == 9 )
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS enemigo_arabe( double x, double y, int tipo, int estado, int inteligencia );
PRIVATE
    int id_e;
    int id_flags;
    int color;
    int vida_enemigo_arabe; // depende de la dificultad
    int rutina; // inteligencia artificial
    int avance; // a onde va
    int haz_graficos; // po eso
    double x_pos; // guarda coordenadas
    double x_incr;
    double y_incr;
    int guarda_x; // no deja k se bloquee
    int tiempo_guarda;
    int incr_y = -14;
    int incr_y2 = -21;
    int velocidad_x;
    int rebotes;
    int no_repitas; // no repite estado
    int no_repitas1;
    int no_repitas2;
    int anda_un_rato; // pega unos pasos...
    int destino; // .. hacia un destino ..
    int destino_aleatorio; // .. sin conocer
    int destino_cobarde; // .. por cobard¡a
    int se_sorprende; // y cuando ve al protagonista
    int ataca_ya; // suelta un espadazo
    int grafico_ataca; // moviendo la espada ..XDD
    int pensando;
    int tiempo_eskiva; // tiempo en el suelo
    int reaccion; // la reaccion al ver una bala
    int rascando = -1; // una mierda
    int cobarde; // si el personaje es miedica le asustan las bombas
    int una_vez;
    int rebotes1;
    int rebotando;
    int kiero_este; // ataca al jugador 1 o al 2
    int no_lo_repitas = 1;
    int muere_de_estres; // si esta mucho rato con miedo y locura muere
    int recuerda_accion; // recuerda la ultima accion que a realizado
    int tiene_espada = 1; // si no tiene espada o la pierde, se vuelve cobarde
    int de_onde_salgo;
    int ponmelo;
    int ponmelo_y = 289;
    int quehara = 1;
    int altura_p1;
    int direccion;
    int debilidad; // explotara en lugar de salir volando si es flojo
    int grita;
    int giratorio; // gira como la batidora
    int esperate;
    int grito_aleatorio;
    int limite_pantalla2;
    int calculos_mix;
    int piensa_barranco; // algunos retrocederan en un barranco otros intentaran saltarlo
    int tiempo_para_pensar; // cada enemigo tiene por naturaleza + o - inteligencia
    int accion; // 0 nada 1 andar , 2 atacar, 3 saltar, 4 tirarse en plancha, 7 miedo, 9 sale volando
    int doble_tiempo;
    int doble_intento;
BEGIN
    if ( estado_fase == 3 )
        if ( rondas_fase == 1 )
            if ( bloquea == 1 )
                x = 1600;
            end
        end
    end
    grito_aleatorio = rand( 1, 2 );
    limite_pantalla2 = limite_pantalla -10;
    vida_enemigo_arabe = 2 + vidaenemigo;
    giratorio = 20000;
    debilidad = rand( 1, 2 );
    file = fase1;
    graph = 36;
    ctype = c_scroll;
    flags = 1;
    z = -6;
    y_incr = rand( -25, -35 );
    x_incr = rand( -7, 7 );
    rebotes = rand( 1, 3 );
    velocidad_x = rand( 2, 7 );
    tiempo_para_pensar = rand( 5, 20 );
    rebotes1 = 1;
    cobarde = rand( 1, 8 );
    destino_cobarde = rand( 1, 2 );
    kiero_este = x_p1;
    de_onde_salgo = rand( 1, 2 );
    if ( inteligencia == 2 )
        accion = 3;
    end
    if ( inteligencia == 5 )
        rutina = 1;
        accion = 5;
    end
    if ( inteligencia == 3 )
        accion = 3;
        if ( x < 1000 )
            paracaidas( x, y, 1 );
        end
        if ( x => 1000 )
            paracaidas( x, y, 2 );
        end
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        kiero_este = x_p1;
        tiempo_guarda++;
        if ( color != color_pared_1 AND color != color_pared_2 AND color != ke_color_es )
            x_pos = x;
        end
        if ( x < x_p1 -800 )
            x = x_p1 -800;
            calculos_mix = x + 1600;
            if ( calculos_mix < limite_pantalla )
                x = calculos_mix;
            end
            y = 289;
        end
        if ( boss == 2 )
            accion = 13;
            grita = 1;
            disparos_acertados++;
        end
        if ( inteligencia == 2 AND x < 2358 )
            x = 2358;
        end
        if ( esperate > 0 )
            esperate--;
        end
        if ( enemigos_tierra <= 0 )
            enemigos_tierra = 1;
        end
        // no deja ke cojan mucha velocidad
        if ( y_incr > 15 )
            y_incr = 15;
        end
        if ( incr_y > 15 )
            incr_y = 15;
        end
        if ( incr_y2 > 15 )
            incr_y2 = 15;
        end
        if ( existe_explosion < 0 )
            existe_explosion = 0;
        end
        if ( existe_explosion > 0 AND cobarde < 4 AND accion != 3 OR tiene_espada == 0 )
            if (( NOT( region_out( id, 0 ))) AND no_lo_repitas == 1 AND accion < 3 )
                accion = 7;
                haz_graficos = 0;
                no_lo_repitas = 0;
            end
        end
        // no deja k el enemigo se bloquee
        if ( tiempo_guarda == 50 AND accion < 3 )
            if ( x == guarda_x )
                rascando = -1;
                accion = rand( 2, 4 );
                haz_graficos = 0;
                recuerda_accion = accion;
            end
            tiempo_guarda = 0;
            guarda_x = x;
        end
        if ( anda_un_rato > 0 )
            anda_un_rato--;
        end
        if ( ataca_ya < 0 )
            ataca_ya++;
        end
        if ( region_out( id, 0 ) AND accion != 3 AND accion != 5 )
            rutina = 0;
        end // si no esta en vision te buscar 
        if ( y > 5640 )
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = 15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = limite_pantalla -15;
                        quehara = 3;
                        ponmelo_y = -15;
                    end
                end
                enemigos_tierra--;
                clones_eliminados++;
                enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // rutinas, busca objetivos
        if ( NOT(( x > kiero_este -350 ) AND ( x < kiero_este + 350 )) AND accion < 3 )
            rutina = 0;
            accion = 1;
        end
        // toca con las balas del personaje
        id_flags = 2;
        if ( collision( type llamazo ) AND grita != 1 )
            accion = 14;
            grita = 1;
            disparos_acertados++;
        end
        if ( id_e = collision( type escopetazo ))
            if ( grita != 1 )
                if ( exists( id_e ))
                    if ( id_e.x < x )
                        id_flags = 1;
                        if ( x < x_p1 )
                            id_flags = 0;
                        end
                    else
                        id_flags = 0;
                        if ( x > x_p1 )
                            id_flags = 1;
                        end
                    end
                end
                accion = 13;
                grita = 1;
                disparos_acertados++;
                sangre_mix( 0, x, y, id_flags, 1 );
            end
        end
        if ( collision( type bala_enemigo ) AND accion < 8 )
            vida_enemigo_arabe--;
            haz_graficos = 0;
            sound_play( sangre, 0, 20 );
        end
        if ( id_e = collision( type bala ))
            if ( accion < 8 )
                if ( exists( id_e ))
                    if ( id_e.priority == -1 )
                        id_flags = 1;
                        if ( x < x_p1 )
                            id_flags = 0;
                        end
                    else
                        id_flags = 0;
                        if ( x > x_p1 )
                            id_flags = 1;
                        end
                    end
                end
                vida_enemigo_arabe--;
                haz_graficos = 0;
                esperate = 3;
                sound_play( sangre, 0, 20 );
                sangre_mix( 0, x, y, id_flags, 0 );
            end
        end
        if ( id_e = collision( type explosion ))
            if ( grita != 1 )
                if ( exists( id_e ))
                    if ( id_e.x < x )
                        id_flags = 1;
                    else
                        id_flags = 0;
                    end
                end
                sangre_mix( 0, x, y, id_flags, 1 );
                if ( debilidad == 2 )
                    grita = 1;
                    accion = 9;
                    if ( grito_aleatorio == 1 )
                        sound_play( golpe9, 0 );
                    end
                    if ( grito_aleatorio == 2 )
                        sound_play( argh, 0 );
                    end
                    if ( id_flags == 1 )
                        x_incr = rand( 4, 10 );
                    else
                        x_incr = rand( -4, -10 );
                    end
                end
                if ( debilidad == 1 )
                    accion = 10;
                    grita = 1;
                end
            end
        end
        if ( vida_enemigo_arabe =< 0 )
            accion = 11;
            priority = 1;
        end
        // hace la gravedad del enemigo  y no deja salir del scroll
        if ( x < 15 )
            x = 15;
        end
        if ( x > limite_pantalla2 )
            x = limite_pantalla2;
            destino = 2;
        end
        // ACCIONES DEL ENEMIGO
        // Accion 0 se keda kieto
        if ( accion == 0 )
            se_sorprende++;
            angle = 0;
            no_repitas2 = 0;
            if ( se_sorprende == 1 )
                graph = 51;
            end
            if ( se_sorprende > 10 )
                graph = 36;
            end
            if ( se_sorprende > 20 )
                accion = 2;
            end
        end
        // Accion 2 atacar
        if ( accion == 2 )
            angle = 0;
            // si tiene una inteligencia lenta, ser  poco preciso
            if ( tiempo_para_pensar => 10 )
                if ( anda_un_rato == 0 AND ataca_ya =< 0 )
                    destino_aleatorio = rand( -4, 5 );
                    anda_un_rato = rand( 5, 70 );
                end
            end
            // si esta dotado de mucha inteligencia ser  m s preciso
            if ( tiempo_para_pensar < 10 AND ataca_ya =< 0 )
                if ( x < kiero_este AND destino == 0 )
                    destino = 1;
                end
                if ( x > kiero_este AND destino == 0 )
                    destino = 2;
                end
                if ( destino == 1 AND anda_un_rato == 0 )
                    destino_aleatorio = 4;
                    anda_un_rato = 2;
                end
                if ( destino == 2 AND anda_un_rato == 0 )
                    destino_aleatorio = -4;
                    anda_un_rato = 2;
                end
                if ( x < kiero_este -330 AND destino == 2 )
                    destino = 1;
                end
                if ( x > kiero_este + 330 AND destino == 1 )
                    destino = 2;
                end
            end
            // Cuando este cerca del objetivo atacar 
            if ( ataca_ya == 0 )
                if ( NOT(( y < y_p1 -100 ) OR y > y_p1 + 100 ))
                    if ( x > kiero_este + 20 AND x < kiero_este + 50 AND flags == 1 )
                        anda_un_rato = 0;
                        grafico_ataca = 0;
                        ataca_ya = 1;
                        destino = 0;
                    end
                    if ( x > kiero_este -50 AND x < kiero_este -20 AND flags == 0 )
                        anda_un_rato = 0;
                        grafico_ataca = 0;
                        ataca_ya = 1;
                        destino = 0;
                    end
                end
            end
            if ( ataca_ya == 1 )
                grafico_ataca++;
                if ( grafico_ataca == 1 )
                    graph = 75;
                    sound_play( lanza, 0 );
                end
                if ( grafico_ataca == 2 )
                    graph = 76;
                end
                if ( grafico_ataca == 3 )
                    graph = 77;
                end
                if ( grafico_ataca == 4 )
                    graph = 76;
                end
                if ( grafico_ataca == 5 )
                    graph = 75;
                end
                if ( grafico_ataca == 6 )
                    graph = 64;
                end
                if ( grafico_ataca == 7 )
                    graph = 65;
                end
                if ( grafico_ataca == 8 )
                    graph = 66;
                end
                if ( grafico_ataca == 9 )
                    graph = 67;
                    if ( collision( type jose_player1 ))
                        josel_tocao = 1;
                    end
                end
                if ( grafico_ataca == 12 )
                    graph = 66;
                end
                if ( grafico_ataca == 13 )
                    graph = 65;
                end
                if ( grafico_ataca => 14 )
                    grafico_ataca = 0;
                    graph = 64;
                    ataca_ya = rand( -30, -90 );
                end
            end
            if ( destino_aleatorio == 0 )
                graph = 36;
            end
            if ( anda_un_rato > 0 AND destino_aleatorio != 0 AND ataca_ya =< 0 )
                haz_graficos++;
                if ( destino_aleatorio < 0 )
                    x += destino_aleatorio;
                    flags = 1;
                end
                if ( destino_aleatorio > 0 )
                    x += destino_aleatorio;
                    flags = 0;
                end
                if ( haz_graficos == 1 )
                    graph = 37;
                end
                if ( haz_graficos == 2 )
                    graph = 38;
                end
                if ( haz_graficos == 3 )
                    graph = 39;
                end
                if ( haz_graficos == 4 )
                    graph = 40;
                end
                if ( haz_graficos == 5 )
                    graph = 41;
                end
                if ( haz_graficos == 6 )
                    graph = 42;
                end
                if ( haz_graficos == 7 )
                    graph = 43;
                end
                if ( haz_graficos == 8 )
                    graph = 44;
                end
                if ( haz_graficos == 9 )
                    graph = 45;
                end
                if ( haz_graficos == 10 )
                    graph = 46;
                end
                if ( haz_graficos == 11 )
                    graph = 47;
                end
                if ( haz_graficos == 12 )
                    graph = 48;
                end
                if ( haz_graficos == 13 )
                    graph = 49;
                end
                if ( haz_graficos => 14 )
                    graph = 50;
                    haz_graficos = 0;
                end
            end
        end
        // Accion 3 salta
        if ( accion == 3 )
            incr_y2++;
            y += incr_y2;
            destino = 0;
            ataca_ya = 0;
            angle = 0;
            if ( incr_y2 < -5 )
                graph = 261;
            end
            if ( incr_y2 => -5 AND incr_y2 < 0 )
                graph = 262;
            end
            if ( incr_y2 => 0 AND incr_y2 < 5 )
                graph = 263;
            end
            if ( incr_y2 >= 5 )
                graph = 262;
            end
            if ( rebotando == rebotes1 )
                rebotando = 0;
                accion = 2;
                recuerda_accion = accion;
                no_repitas2 = 0;
                angle = 0;
            end
            if ( no_repitas2 == 0 )
                if ( x < kiero_este )
                    direccion = 1;
                end
                if ( x > kiero_este )
                    direccion = 2;
                end
                no_repitas2 = 1;
            end
            if ( direccion == 1 )
                x += velocidad_x;
                flags = 0;
            end
            if ( direccion == 2 )
                x -= velocidad_x;
                flags = 1;
            end
            if ( incr_y2 > 0 )
                if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                    incr_y2 = -20;
                    rebotando++;
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 3 );
                    sound_play( caida, 0, 15 );
                end
            end
        end
        // Accion 4 se tira en plancha
        if ( accion == 4 )
            haz_graficos++;
            tiempo_eskiva++;
            ataca_ya = 0;
            destino = 0;
            if ( rascando == -1 )
                incr_y2 = -12;
                rascando = 0;
                angle = 0;
            end
            if ( rascando == 0 )
                incr_y2++;
                y += incr_y2;
                if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND incr_y2 > 0 )
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 3 );
                    incr_y2 = -20;
                    rascando = 1;
                end
            end
            if ( tiempo_eskiva => 30 )
                haz_graficos = 19;
                graph = 62;
                angle -= 22000;
                incr_y2++;
                y += incr_y2;
                if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND incr_y2 > 0 )
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 3 );
                    accion = rand( 2, 4 );
                    recuerda_accion = accion;
                    incr_y2 = -20;
                    tiempo_eskiva = 0;
                    haz_graficos = 0;
                    angle = 0;
                    rascando = -1;
                end
            end
            if ( flags == 0 )
                x += 5;
            end
            if ( flags == 1 )
                x -= 5;
            end
            if ( haz_graficos == 1 )
                graph = 78;
            end
            if ( haz_graficos == 4 )
                graph = 79;
            end
            if ( haz_graficos == 7 )
                graph = 80;
                if ( collision( type jose_player1 ))
                    josel_tocao = 1;
                end
            end
            if ( haz_graficos == 10 )
                graph = 80;
                if ( collision( type jose_player1 ))
                    josel_tocao = 1;
                end
            end
            if ( haz_graficos == 18 )
                graph = 81;
            end
        end
        // Accion 5 salta bordillo
        if ( accion == 5 )
            graph = 62;
            angle -= 22000;
            incr_y2++;
            y += incr_y2;
            destino = 0;
            ataca_ya = 0;
            if ( flags == 0 )
                x += 4;
            end
            if ( flags == 1 )
                x -= 4;
            end
            if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND incr_y2 > 0 )
                incr_y2 = -20;
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 3 );
                sound_play( caida, 0, 15 );
                accion = 2;
            end
        end
        // Accion 7 tiene miedo y sale de la pantalla
        if ( accion == 7 AND vida_enemigo_arabe > 0 )
            haz_graficos++;
            angle = 0;
            muere_de_estres++;
            debilidad = 1;
            if ( destino_cobarde == 1 )
                x += 4;
                flags = 0;
            end
            if ( destino_cobarde == 2 )
                x -= 4;
                flags = 1;
            end
            color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
            if ( color == color_pared_1 OR color == color_pared_2 OR color == color_arriba_objeto_no_piso OR x < 16 OR x => limite_pantalla2 )
                muere_de_estres = 250;
            end
            if ( muere_de_estres == 250 )
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 54, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 55, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 56, fase1, 1 );
                clones_eliminados++;
                enemigos_tierra--;
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = 15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = limite_pantalla -15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end
            if ( tiene_espada == 1 )
                cacho_cuerpo_enemigo( x, y -20, 92, fase1, 10 );
                tiene_espada = 0;
            end
            if ( haz_graficos == 1 )
                graph = 82;
            end
            if ( haz_graficos == 2 )
                graph = 83;
            end
            if ( haz_graficos == 3 )
                graph = 84;
            end
            if ( haz_graficos == 4 )
                graph = 85;
            end
            if ( haz_graficos == 5 )
                graph = 86;
            end
            if ( haz_graficos == 6 )
                graph = 87;
            end
            if ( haz_graficos == 7 )
                graph = 88;
            end
            if ( haz_graficos == 8 )
                graph = 89;
            end
            if ( haz_graficos == 9 )
                graph = 90;
            end
            if ( haz_graficos => 10 )
                graph = 91;
                haz_graficos = 0;
            end
            if ( region_out( id, 0 ))
                clones_eliminados++;
                enemigos_tierra--;
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = 15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = limite_pantalla -15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end
        end
        // sale volando si le da una explosion
        if ( accion == 9 )
            graph = 52;
            angle -= giratorio;
            y_incr++;
            y += y_incr;
            x += x_incr;
            if ( rebotes =< 0 )
                accion = 50;
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 54, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 55, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 56, fase1, 1 );
                puntuacion += 200;
                clones_eliminados++;
                enemigos_tierra--;
                // si kedan enemigos salid
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = 15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = limite_pantalla -15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end // rebota contra el suelo
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND y_incr > 0 )
                y_incr = -10;
                rebotes--;
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
            end
        end
        // o si la explosion le da de lleno se parte en pedacitos
        if ( accion == 10 )
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            sound_play( dish, 0 );
            piedras( x, y, 568, rand( 70, 95 ), 1 );
            piedras( x, y, 569, rand( 70, 95 ), 2 );
            piedras( x, y, 570, rand( 70, 95 ), 3 );
            piedras( x, y, 571, rand( 70, 95 ), 4 );
            piedras( x, y, 572, rand( 70, 95 ), 5 );
            piedras( x, y, 568, rand( 70, 95 ), 6 );
            piedras( x, y, 568, rand( 70, 95 ), 7 );
            piedras( x, y, 569, rand( 70, 95 ), 8 );
            piedras( x, y, 570, rand( 70, 95 ), 9 );
            piedras( x, y, 571, rand( 70, 95 ), 10 );
            piedras( x, y, 572, rand( 70, 95 ), 11 );
            piedras( x, y, 568, rand( 70, 95 ), 12 );
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = 15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = limite_pantalla -15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // Muere tras ser disparado
        if ( accion == 11 )
            graph = 52;
            angle += 10000;
            incr_y++;
            y += incr_y;
            if ( id_flags != 2 )
                flags = id_flags;
            end
            if ( flags == 0 )
                x -= 3;
            end
            if ( flags == 1 )
                x += 3;
            end
            if ( angle => 90000 )
                angle = 90000;
            end
            if ( angle =< -90000 )
                angle = -90000;
            end
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 54, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 55, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 56, fase1, 1 );
                sound_play( caida, 0, 15 );
                enemigos_tierra--;
                puntuacion += 100;
                clones_eliminados++;
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = 15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = limite_pantalla -15;
                            quehara = 3;
                            ponmelo_y = -20;
                        end
                    end
                    enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end
        end
        // muere con un super escopetazo
        if ( accion == 13 )
            sound_play( dish, 0 );
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            if ( id_flags == 1 )
                piedras( x, y, 568, rand( 70, 95 ), 1 );
                piedras( x, y, 569, rand( 70, 95 ), 2 );
                piedras( x, y, 570, rand( 70, 95 ), 3 );
                piedras( x, y, 571, rand( 70, 95 ), 4 );
                piedras( x, y, 572, rand( 70, 95 ), 5 );
                piedras( x, y, 568, rand( 70, 95 ), 6 );
                piedras( x, y, 569, rand( 70, 95 ), 13 );
                piedras( x, y, 570, rand( 70, 95 ), 14 );
                piedras( x, y, 568, rand( 70, 95 ), 15 );
                piedras( x, y, 569, rand( 70, 95 ), 16 );
                piedras( x, y, 570, rand( 70, 95 ), 17 );
                piedras( x, y, 568, rand( 70, 95 ), 18 );
            else
                piedras( x, y, 568, rand( 70, 95 ), 7 );
                piedras( x, y, 569, rand( 70, 95 ), 8 );
                piedras( x, y, 570, rand( 70, 95 ), 9 );
                piedras( x, y, 571, rand( 70, 95 ), 10 );
                piedras( x, y, 572, rand( 70, 95 ), 11 );
                piedras( x, y, 568, rand( 70, 95 ), 12 );
                piedras( x, y, 569, rand( 70, 95 ), 19 );
                piedras( x, y, 570, rand( 70, 95 ), 20 );
                piedras( x, y, 568, rand( 70, 95 ), 21 );
                piedras( x, y, 569, rand( 70, 95 ), 22 );
                piedras( x, y, 570, rand( 70, 95 ), 23 );
                piedras( x, y, 568, rand( 70, 95 ), 24 );
            end
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = 15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = limite_pantalla -15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // achicharrando a la pena
        if ( accion == 14 )
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            enemigo_achicharrao( x, y );
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = 15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = limite_pantalla -15;
                        quehara = 3;
                        ponmelo_y = -20;
                    end
                end
                enemigo_arabe( ponmelo, ponmelo_y, 1, 1, quehara );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        if ( rutina == 1 AND accion < 7 AND accion != 5 ) // rutina de la I.A  ** Mision, atacar o crear accion
            pensando++;
            if ( x > x_p1 -140 AND x < x_p1 + 140 )
                pensando = tiempo_para_pensar;
            end
            if ( pensando == tiempo_para_pensar -5 AND accion == 0 )
                graph = 51;
            end
            if ( pensando => tiempo_para_pensar )
                if ( no_repitas1 == 0 AND accion < 3 )
                    accion = rand( 2, 4 );
                    rascando = -1;
                    haz_graficos = 0;
                    recuerda_accion = accion;
                    no_repitas1++;
                end
                pensando = 0;
            end
        end
        if ( rutina == 0 AND accion < 7 AND accion != 5 ) // rutina de la I.A  ** Mision, buscar objetivo
            // graficos de andar
            if ( accion == 1 )
                haz_graficos++;
                no_repitas1 = 0;
                no_repitas2 = 0;
                angle = 0;
                se_sorprende = 0;
                destino = 0;
                ataca_ya = 0;
                if ( haz_graficos == 1 )
                    graph = 37;
                end
                if ( haz_graficos == 2 )
                    graph = 38;
                end
                if ( haz_graficos == 3 )
                    graph = 39;
                end
                if ( haz_graficos == 4 )
                    graph = 40;
                end
                if ( haz_graficos == 5 )
                    graph = 41;
                end
                if ( haz_graficos == 6 )
                    graph = 42;
                end
                if ( haz_graficos == 7 )
                    graph = 43;
                end
                if ( haz_graficos == 8 )
                    graph = 44;
                end
                if ( haz_graficos == 9 )
                    graph = 45;
                end
                if ( haz_graficos == 10 )
                    graph = 46;
                end
                if ( haz_graficos == 11 )
                    graph = 47;
                end
                if ( haz_graficos == 12 )
                    graph = 48;
                end
                if ( haz_graficos == 13 )
                    graph = 49;
                end
                if ( haz_graficos => 14 )
                    graph = 50;
                    haz_graficos = 0;
                end
                if ( x < kiero_este )
                    x += velocidad_x;
                    flags = 0;
                end
                if ( x > kiero_este )
                    x -= velocidad_x;
                    flags = 1;
                end
            end
            if ( x > kiero_este -280 AND x < kiero_este + 280 AND accion < 3 )
                rutina = 1;
                accion = 0;
            end
        end
        // MAPAS DE DUREZA
        // Colisiona con todo tipo de objetos y marca limites
        if ( collision( type prohibido_elpaso ))
            x = x_pos;
            if ( flags == 0 )
                destino = 2;
            end
            if ( collision( type prohibido_elpaso ))
                if ( flags == 0 )
                    x -= 5;
                else
                    x += 5;
                end
            end
        end
        if ( x > limite_pantalla2 )
            x = limite_pantalla2;
            if ( accion == 2 )
                destino = 2;
                destino_aleatorio = -3;
                flags = 0;
            end
        end
        if ( x < 15 )
            x = 15;
            if ( accion == 2 )
                destino = 1;
                flags = 1;
                destino_aleatorio = 3;
            end
        end
        // gravedad
        if (( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )) AND accion != 9 )
            y += 4;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND accion != 9 )
            y -= 4;
        end
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        // se listo y salta cualquier barranco
        if ( color == color_arriba_objeto_no_piso AND accion < 3 )
            accion = 5;
        end
        // choca con paredes y con pinchos
        if ( color == color_pared_con_pinchos OR collision( type pinchos ))
            vida_enemigo_arabe = 0;
        end
        if ( color == color_pared_1 OR color == color_pared_2 )
            x = x_pos;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    angle = 0;
                    rascando = -1;
                    haz_graficos = 0;
                    accion = rand( 2, 4 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        if ( color == color_pared_1 )
            x -= 5;
            x_pos = x;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    angle = 0;
                    rascando = -1;
                    haz_graficos = 0;
                    accion = rand( 2, 4 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        if ( color == color_pared_2 )
            x += 5;
            x_pos = x;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    angle = 0;
                    rascando = -1;
                    haz_graficos = 0;
                    accion = rand( 2, 4 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        if ( doble_intento > 0 )
            if ( doble_tiempo > 0 )
                doble_tiempo--;
            else
                doble_intento = 0;
            end
            if ( accion < 3 )
                if ( doble_tiempo > 1 )
                    accion = 2;
                end
                if ( doble_tiempo == 1 )
                    if ( flags == 0 )
                        destino_aleatorio = 3;
                        flags = 1;
                    else
                        destino_aleatorio = -3;
                        flags = 0;
                    end
                    rascando = -1;
                    haz_graficos = 0;
                    accion = rand( 3, 4 );
                    anda_un_rato = rand( 5, 70 );
                end
            end
        end
        frame;
    end
end


PROCESS cacho_cuerpo_enemigo( double x, double y, int graph, int file, int tipo );
PRIVATE
    double y_incr;
    double y_incr2;
    double x_incr2;
BEGIN
    z = -5;
    ctype = c_scroll;
    y_incr = rand( -10, -15 );
    y_incr2 = rand( -15, -29 );
    x_incr2 = rand( -5, 5 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        y_incr++;
        y += y_incr;
        if ( collision( type agua ))
            salpicada( x, y, 120 );
            signal( id, s_kill );
        end
        if ( tipo == 1 )
            x -= 2;
            if ( modo_wiz == 0 )
                angle += 5000;
            end
        end
        if ( tipo == 2 )
            x += 2;
            if ( modo_wiz == 0 )
                angle -= 5000;
            end
        end
        if ( tipo == 10 )
            x += x_incr2;
            y += y_incr2;
            y_incr++;
            if ( modo_wiz == 0 )
                angle -= 20000;
            end
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es OR region_out( id, 0 ))
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS enemigo_dispara( double x, double y, int file, int tipo );
PRIVATE
    int color;
    int id_e;
    int id_flags;
    int mis_disparos = 500;
    int vida_enemigo_dispara;
    int cuando_disparo; // tiempo de espera entre disparo
    int pasando_tiempo; // el tiempo que espera
    int pasando_tiempo2;
    int haz_graficos;
    int haz_graficos2;
    int he_disparao;
    int accion; // accion : 1 disparar , 2 andar
    int tiempo_andar;
    int tiempo_disparo;
    int cuenta_tiempo;
    double x_incr;
    double x_pos;
    double y_incr;
    int incr_y = -14;
    double y_salto = -19;
    int diferencia_altura;
    int diferencia_distancia;
    int debilidad; // si es muy flojo explotara y no saldra volando
    int tocao;
    int ponmelo;
    int rebotes;
    int giratorio; // si si, este enemigo tambien gira
    int de_onde_salgo;
    int estoy_en_agua;
    int grita;
    int doble_intento;
    int doble_tiempo;
    int ke_arma_cojo; // segun avances en el juego, los enemigos tmb podr n coger armas m s potentes
BEGIN
    if ( estado_fase == 3 )
        if ( rondas_fase == 1 )
            if ( bloquea == 1 )
                x = 1600;
            end
        end
    end
    // sistema ke permite seleccionar arma segun el avance del juego
    if ( estado_fase <= 5 )
        ke_arma_cojo = 1;
    end
    if ( estado_fase => 6 AND estado_fase =< 10 )
        ke_arma_cojo = rand( 1, 2 );
    end
    if ( estado_fase => 23 AND estado_fase =< 24 )
        ke_arma_cojo = rand( 1, 3 );
    end
    debilidad = rand( 1, 2 );
    vida_enemigo_dispara = 3 + vidaenemigo;
    giratorio = 20000;
    ctype = c_scroll;
    graph = 152;
    z = -5;
    cuando_disparo = rand( 5, 70 );
    accion = rand( 1, 2 );
    tiempo_andar = rand( 20, 100 );
    tiempo_disparo = rand( 10, 50 );
    de_onde_salgo = rand( 1, 2 );
    rebotes = rand( 1, 3 );
    x_incr = rand( 3, 6 );
    y_incr = rand( -30, -35 );
    if ( tipo == 2 )
        accion = 3;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( color != color_pared_1 AND color != color_pared_2 AND color != ke_color_es )
            x_pos = x;
        end
        if ( boss == 2 )
            accion = 13;
            grita = 1;
            disparos_acertados++;
        end
        if ( tipo == 2 AND x < 2358 )
            x = 2358;
        end
        if ( enemigos_tierra <= 0 )
            enemigos_tierra = 1;
        end
        if ( x < x_p1 -800 )
            x = x_p1 -800;
            y = 289;
        end
        // explota en mil pedazos , weno solo 24 pedazos
        if ( collision( type explosion ) AND grita != 1 AND debilidad == 1 )
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            sound_play( dish, 0 );
            piedras( x, y, 568, rand( 70, 95 ), 1 );
            piedras( x, y, 569, rand( 70, 95 ), 2 );
            piedras( x, y, 570, rand( 70, 95 ), 3 );
            piedras( x, y, 574, rand( 70, 95 ), 5 );
            piedras( x, y, 568, rand( 70, 95 ), 6 );
            piedras( x, y, 568, rand( 70, 95 ), 7 );
            piedras( x, y, 569, rand( 70, 95 ), 8 );
            piedras( x, y, 570, rand( 70, 95 ), 9 );
            piedras( x, y, 574, rand( 70, 95 ), 11 );
            piedras( x, y, 568, rand( 70, 95 ), 12 );
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = x_p1 + 740;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = x_p1 -740;
                    end
                end
                enemigo_dispara( ponmelo, 289, fase1, 1 );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // salpica si cae al agua
        if ( estoy_en_agua == 0 AND collision( type agua ))
            salpicada( x, y, 170 );
            estoy_en_agua = 1;
            trozo_suelo( x, y -3, 377, 10 );
            trozo_suelo( x, y -3, 377, 20 );
            trozo_suelo( x, y -3, 377, 10 );
            sound_play( splash, 0 );
        end
        if ( estoy_en_agua == 1 AND ( NOT( collision( type agua ))) AND y_salto == 1 )
            estoy_en_agua = 0;
        end
        // detecta paredes y salta barrancos
        // deja caer al barranco hasta un punto en que muere
        if ( y > 5000 )
            enemigos_tierra--;
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = x_p1 + 740;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = x_p1 -740;
                    end
                end
                enemigo_dispara( ponmelo, 289, fase1, 1 );
                clones_eliminados++;
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        if ( tocao > 0 )
            tocao--;
        end
        id_flags = 2;
        if ( collision( type llamazo ) AND grita != 1 )
            accion = 14;
            grita = 1;
            disparos_acertados++;
        end
        if ( id_e = collision( type escopetazo ))
            if ( exists( id_e ))
                if ( id_e.x < x )
                    id_flags = 1;
                    if ( x < x_p1 )
                        id_flags = 0;
                    end
                else
                    id_flags = 0;
                    if ( x > x_p1 )
                        id_flags = 1;
                    end
                end
            end
            if ( grita != 1 )
                accion = 13;
                grita = 1;
                sangre_mix( 0, x, y, id_flags, 1 );
                disparos_acertados++;
            end
        end
        if ( id_e = collision( type bala ))
            if ( exists( id_e ))
                if ( id_e.priority == -1 )
                    id_flags = 1;
                    if ( x < x_p1 )
                        id_flags = 0;
                    end
                else
                    id_flags = 0;
                    if ( x > x_p1 )
                        id_flags = 1;
                    end
                end
            end
            if ( tocao == 0 AND vida_enemigo_dispara > 0 )
                vida_enemigo_dispara--;
                sound_play( sangre, 0, 20 );
                tocao = 4;
                sangre_mix( 0, x, y, id_flags, 0 );
            end
            if ( vida_enemigo_dispara <= 0 )
                priority = 1;
                accion = 4;
            end
        end
        if ( id_e = collision( type explosion ))
            if ( grita != 1 AND debilidad == 2 )
                grita = 1;
                accion = 5;
                sound_play( golpe9, 0 );
                if ( exists( id_e ))
                    if ( id_e.x < x )
                        id_flags = 1;
                    else
                        id_flags = 0;
                    end
                end
                sangre_mix( 0, x, y, id_flags, 1 );
                if ( id_flags == 1 )
                    x_incr = rand( 2, 8 );
                else
                    x_incr = rand( -2, -8 );
                end
            end
        end
        if ( accion == 5 ) // sale volando en una explosion
            graph = 178;
            angle -= giratorio;
            y_incr++;
            y += y_incr;
            x += x_incr;
            if ( rebotes =< 0 )
                accion = 50;
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 179, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 180, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 181, fase1, 1 );
                clones_eliminados++;
                puntuacion += 400;
                enemigos_tierra--;
                // si kedan enemigos salid
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = x_p1 + 740;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = x_p1 -740;
                        end
                    end
                    enemigo_dispara( ponmelo, 289, fase1, 1 );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end // rebota contra el suelo
            if ( y_incr > 0 )
                if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                    y_incr = -10;
                    rebotes--;
                    polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                end
            end
        end
        if ( accion == 4 ) // muere cuando le disparan
            if ( id_flags != 2 )
                flags = id_flags;
            end
            graph = 178;
            angle += 10000;
            incr_y++;
            y += incr_y;
            if ( flags == 0 )
                x -= 3;
            end
            if ( flags == 1 )
                x += 3;
            end
            if ( angle => 90000 )
                angle = 90000;
            end
            if ( angle =< -90000 )
                angle = -90000;
            end
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                if ( estoy_en_agua == 0 )
                    polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                    polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                    sound_play( caida, 0, 15 );
                end
                cacho_cuerpo_enemigo( x, y, 53, fase1, 1 );
                cacho_cuerpo_enemigo( x, y, 179, fase1, 2 );
                cacho_cuerpo_enemigo( x, y, 180, fase1, 3 );
                cacho_cuerpo_enemigo( x, y, 181, fase1, 1 );
                clones_eliminados++;
                enemigos_tierra--;
                puntuacion += 200;
                if ( enemigos_tierra > 0 )
                    if ( de_onde_salgo == 1 )
                        ponmelo = x_p1 -740;
                        if ( ponmelo < 15 )
                            ponmelo = x_p1 + 740;
                        end
                    end
                    if ( de_onde_salgo == 2 )
                        ponmelo = x_p1 + 740;
                        if ( ponmelo > limite_pantalla )
                            ponmelo = x_p1 -740;
                        end
                    end
                    enemigo_dispara( ponmelo, 289, fase1, 1 );
                    signal( id, s_kill );
                end
                signal( id, s_kill );
            end
        end
        if ( accion == 3 ) // salta y a veces dispara
            y_salto++;
            y += y_salto;
            haz_graficos2 = 0;
            if ( y < y_p1 + 80 AND y > y_p1 -80 )
                pasando_tiempo++;
                he_disparao = 0;
            end
            if ( flags == 0 )
                x += x_incr;
            end
            if ( flags == 1 )
                x -= x_incr;
            end
            if (( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) AND y_salto > 0 )
                accion = rand( 1, 2 );
                y_salto = -19;
                haz_graficos = 27;
                if ( estoy_en_agua == 0 )
                    polvo( x -16, y -3, 264, 1 );
                    polvo( x, y -6, 264, 3 );
                    polvo( x + 16, y -2, 264, 2 );
                    sound_play( caida, 0, 15 );
                end
                graph = 152;
            end
            if ( y_salto < -5 )
                graph = 171;
            end
            if ( y_salto => -5 AND y_salto < 0 )
                graph = 172;
            end
            if ( y_salto => 0 AND y_salto < 5 )
                graph = 173;
            end
            if ( y_salto >= 5 )
                graph = 172;
            end
        end
        // recibe un escopetazo
        if ( accion == 13 )
            sound_play( dish, 0 );
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            if ( id_flags == 1 )
                piedras( x, y, 568, rand( 70, 95 ), 1 );
                piedras( x, y, 569, rand( 70, 95 ), 2 );
                piedras( x, y, 570, rand( 70, 95 ), 3 );
                piedras( x, y, 574, rand( 70, 95 ), 5 );
                piedras( x, y, 568, rand( 70, 95 ), 6 );
                piedras( x, y, 569, rand( 70, 95 ), 13 );
                piedras( x, y, 570, rand( 70, 95 ), 14 );
                piedras( x, y, 568, rand( 70, 95 ), 15 );
                piedras( x, y, 569, rand( 70, 95 ), 16 );
                piedras( x, y, 570, rand( 70, 95 ), 17 );
                piedras( x, y, 568, rand( 70, 95 ), 18 );
            else
                piedras( x, y, 568, rand( 70, 95 ), 7 );
                piedras( x, y, 569, rand( 70, 95 ), 8 );
                piedras( x, y, 570, rand( 70, 95 ), 9 );
                piedras( x, y, 574, rand( 70, 95 ), 11 );
                piedras( x, y, 568, rand( 70, 95 ), 12 );
                piedras( x, y, 569, rand( 70, 95 ), 19 );
                piedras( x, y, 570, rand( 70, 95 ), 20 );
                piedras( x, y, 568, rand( 70, 95 ), 21 );
                piedras( x, y, 569, rand( 70, 95 ), 22 );
                piedras( x, y, 570, rand( 70, 95 ), 23 );
                piedras( x, y, 568, rand( 70, 95 ), 24 );
            end
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = x_p1 + 740;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = x_p1 -740;
                    end
                end
                enemigo_dispara( ponmelo, 289, fase1, 1 );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // achicharrando a la pena
        if ( accion == 14 )
            puntuacion += 200;
            clones_eliminados++;
            enemigos_tierra--;
            enemigo_achicharrao( x, y );
            // si kedan enemigos salid
            if ( enemigos_tierra > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -740;
                    if ( ponmelo < 15 )
                        ponmelo = x_p1 + 740;
                    end
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 740;
                    if ( ponmelo > limite_pantalla )
                        ponmelo = x_p1 -740;
                    end
                end
                enemigo_dispara( ponmelo, 289, fase1, 1 );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        // pone la fisica al enemigo
        if ( x < 15 )
            x = 15;
        end
        if ( x > limite_pantalla )
            x = limite_pantalla -2;
        end
        if ( accion == 2 ) // anda
            haz_graficos2++;
            cuenta_tiempo++;
            if ( haz_graficos2 == 1 )
                graph = 157;
            end
            if ( haz_graficos2 == 2 )
                graph = 158;
            end
            if ( haz_graficos2 == 3 )
                graph = 159;
            end
            if ( haz_graficos2 == 4 )
                graph = 160;
            end
            if ( haz_graficos2 == 5 )
                graph = 161;
            end
            if ( haz_graficos2 == 6 )
                graph = 162;
            end
            if ( haz_graficos2 == 7 )
                graph = 163;
            end
            if ( haz_graficos2 == 8 )
                graph = 164;
            end
            if ( haz_graficos2 == 9 )
                graph = 165;
            end
            if ( haz_graficos2 == 10 )
                graph = 166;
            end
            if ( haz_graficos2 == 11 )
                graph = 167;
            end
            if ( haz_graficos2 == 12 )
                graph = 168;
            end
            if ( haz_graficos2 == 13 )
                graph = 169;
            end
            if ( haz_graficos2 => 14 )
                graph = 170;
                haz_graficos2 = 0;
            end
            if ( flags == 0 )
                x += x_incr;
            end
            if ( flags == 1 )
                x -= x_incr;
            end
            if ( cuenta_tiempo => tiempo_andar )
                cuenta_tiempo = 0;
                tiempo_andar = rand( 10, 50 );
                tiempo_disparo = rand( 20, 50 );
                x_incr = rand( 2, 5 );
                accion = rand( 1, 3 );
                graph = 152;
            end
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_arriba_objeto_no_piso )
                accion = 3;
            end
        end
        // hace que el personaje respire
        if ( accion == 1 )
            haz_graficos++;
            cuenta_tiempo++;
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_arriba_objeto_no_piso )
                accion = 3;
            end
            if ( cuenta_tiempo => tiempo_disparo )
                cuenta_tiempo = 0;
                accion = rand( 1, 2 );
                tiempo_andar = rand( 10, 50 );
                tiempo_disparo = rand( 20, 50 );
                x_incr = rand( 2, 5 );
            end
            if ( haz_graficos == 1 )
                graph = 152;
            end
            if ( haz_graficos == 30 )
                graph = 156;
            end
            if ( haz_graficos == 59 )
                haz_graficos = 0;
            end
            // en el disparo se tira hacia atr s
            if ( he_disparao > 0 )
                he_disparao--;
            end
            if ( he_disparao == 8 )
                graph = 153;
            end
            if ( he_disparao == 6 )
                graph = 154;
            end
            if ( he_disparao == 4 )
                graph = 155;
            end
            if ( he_disparao == 2 )
                graph = 152;
            end
            // rutina de la I.A
            if (( NOT( region_out( id, 0 ))) AND mis_disparos > 0 )
                if ( y < y_p1 + 80 AND y > y_p1 -80 )
                    pasando_tiempo++;
                    pasando_tiempo2 = 0;
                end
                diferencia_altura = y_p1 - y;
                diferencia_distancia = x_p1 - x;
                if ( diferencia_altura < -100 )
                    if ( diferencia_distancia < -120 OR diferencia_distancia > 120 )
                        pasando_tiempo2++;
                        pasando_tiempo = 0;
                        graph = 176;
                    end
                end
            end
            if ( x_p1 < x -60 )
                flags = 1;
            end
            if ( x_p1 > x + 60 )
                flags = 0;
            end
        end
        if ( pasando_tiempo2 => cuando_disparo AND mis_disparos > 0 )
            pasando_tiempo2 = 0;
            cuando_disparo = rand( 5, 100 );
            mis_disparos--;
            if ( ke_arma_cojo == 1 )
                if ( flags == 1 )
                    bala_enemigo( x -53, y -69, 295, 0, 4, flags );
                    sound_play( pistola, 0, 17 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                end
                if ( flags == 0 )
                    bala_enemigo( x + 53, y -69, 295, 0, 4, flags );
                    sound_play( pistola, 0, 17 );
                    brillo_cuerpo( x + 5, y -44, flags, 1 );
                    resto_bala( x + 26, y -37, flags );
                end
            end
            if ( ke_arma_cojo == 2 )
                if ( flags == 1 )
                    sound_play( uzi, 0, 18 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x -53, y -69, 361, 0, 4, flags );
                    bala_enemigo( x -53, y -69, 361, 0, 9, flags );
                    bala_enemigo( x -53, y -69, 361, 0, 10, flags );
                end
                if ( flags == 0 )
                    sound_play( uzi, 0, 18 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x + 53, y -69, 361, 0, 4, flags );
                    bala_enemigo( x + 53, y -69, 361, 0, 9, flags );
                    bala_enemigo( x + 53, y -69, 361, 0, 10, flags );
                end
            end
            if ( ke_arma_cojo == 3 )
                if ( flags == 1 )
                    sound_play( laser6, 0, 14 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x -53, y -69, 361, 0, 22, flags );
                end
                if ( flags == 0 )
                    sound_play( laser6, 0, 14 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x + 53, y -69, 361, 0, 23, flags );
                end
            end
        end
        if ( pasando_tiempo => cuando_disparo AND mis_disparos > 0 )
            pasando_tiempo = 0;
            he_disparao = 10;
            cuando_disparo = rand( 5, 100 );
            mis_disparos--;
            if ( ke_arma_cojo == 1 )
                if ( flags == 1 )
                    bala_enemigo( x -62, y -27, 341, 0, 3, flags );
                    sound_play( pistola, 0, 17 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                end
                if ( flags == 0 )
                    bala_enemigo( x + 62, y -27, 341, 0, 3, flags );
                    sound_play( pistola, 0, 17 );
                    brillo_cuerpo( x + 5, y -44, flags, 1 );
                    resto_bala( x + 26, y -37, flags );
                end
            end
            if ( ke_arma_cojo == 2 )
                if ( flags == 1 )
                    sound_play( uzi, 0, 18 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x -62, y -27, 361, 0, 3, flags );
                    bala_enemigo( x -62, y -27, 361, 0, 8, flags );
                    bala_enemigo( x -62, y -27, 361, 0, 7, flags );
                end
                if ( flags == 0 )
                    sound_play( uzi, 0, 18 );
                    brillo_cuerpo( x + 5, y -44, flags, 1 );
                    resto_bala( x + 26, y -37, flags );
                    bala_enemigo( x + 62, y -27, 361, 0, 3, flags );
                    bala_enemigo( x + 62, y -27, 361, 0, 8, flags );
                    bala_enemigo( x + 62, y -27, 361, 0, 7, flags );
                end
            end
            if ( ke_arma_cojo == 3 )
                if ( flags == 1 )
                    sound_play( laser6, 0, 14 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x -53, y -30, 361, 0, 20, flags );
                end
                if ( flags == 0 )
                    sound_play( laser6, 0, 14 );
                    brillo_cuerpo( x -5, y -44, flags, 1 );
                    resto_bala( x -26, y -37, flags );
                    bala_enemigo( x + 53, y -30, 361, 0, 21, flags );
                end
            end
        end
        // MAPAS DE DUREZA
        // Colisiona con todo tipo de objetos y marca limites
        if ( collision( type prohibido_elpaso ))
            x = x_pos;
            if ( collision( type prohibido_elpaso ))
                if ( flags == 0 )
                    x -= 5;
                else
                    x += 5;
                end
            end
        end
        if ( x < 15 )
            x = 15;
            if ( accion == 2 )
                flags = 0;
            end
        end
        if ( x > limite_pantalla )
            x = limite_pantalla -2;
            if ( accion == 2 )
                flags = 1;
            end
        end
        if ( NOT( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ))
            y += 4;
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            y -= 4;
        end
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        // muere por pinchos
        if ( color == color_pared_con_pinchos OR collision( type pinchos ))
            accion = 4;
        end
        // paredes
        if ( color == color_pared_1 )
            x = x_pos;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    accion = rand( 2, 3 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        if ( color == color_pared_2 )
            x = x_pos;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    accion = rand( 2, 3 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        // paredes revision 2
        color = map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza );
        if ( color == color_pared_1 )
            x -= 5;
            x_pos = x;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    accion = rand( 2, 3 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        if ( color == color_pared_2 )
            x += 5;
            x_pos = x;
            if ( accion < 3 )
                if ( doble_intento == 0 )
                    accion = rand( 2, 3 );
                    doble_intento = 1;
                    doble_tiempo = 20;
                end
            end
        end
        if ( doble_intento > 0 )
            if ( doble_tiempo > 0 )
                doble_tiempo--;
            else
                doble_intento = 0;
            end
            if ( accion < 3 )
                accion = 2;
                if ( doble_tiempo > 1 )
                    accion = 2;
                end
                if ( doble_tiempo == 1 )
                    if ( flags == 0 )
                        flags = 1;
                    else
                        flags = 0;
                    end
                    accion = 3;
                end
            end
        end
        frame;
    end
end


PROCESS llamas_del_suelo();
PRIVATE
    int activar;
    int temporizador;
BEGIN
    ctype = c_scroll;
    x = 3446;
    y = 337;
    graph = 1;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( camara_x > 3150 AND camara_x < 3850 )
            activar = 1;
        end
        if ( camara_x > 4000 OR camara_x < 3150 )
            activar = 0;
            temporizador = 0;
        end
        if ( activar == 1 )
            temporizador++;
            if ( temporizador == 5 )
                explosion3( 3446, 337, 200, rand( 0, 1 ));
            end
            if ( temporizador == 20 )
                explosion3( 3529, 316, 200, rand( 0, 1 ));
            end
            if ( temporizador == 35 )
                explosion3( 3603, 296, 200, rand( 0, 1 ));
            end
            if ( temporizador == 50 )
                explosion3( 3670, 276, 200, rand( 0, 1 ));
            end
            if ( temporizador == 65 )
                explosion3( 3725, 262, 200, rand( 0, 1 ));
            end
            if ( temporizador == 80 )
                explosion3( 3785, 247, 200, rand( 0, 1 ));
            end
            if ( temporizador > 90 )
                temporizador = 0;
            end
        end
        frame;
    end
end


PROCESS elicoptero1( double x, double y, int graph, int file, int tipo );
PRIVATE
    int incr_x;
    int semaforo;
    int s_elicop; // sonido de disparo del elicoptero
    int s_elice; // sonido de las helices del helicoptero
    int no_jodas; // cuando hay ke solucionar problemas se hace cualkier intento
    int vida_elicoptero1;
    int si_dispara; // depende de la dificultad
    int rutina;
    int avance;
    int rato;
    int alea;
    double y_incr;
    int incr_y = -6;
    int lo_ve;
    // esto hace k el enemigo sea cada vez + inteligente
    int de_onde_salgo; // al morir el siguiente helicoptero sale fuera de pantalla acercandose
    int ponmelo;
    int tiempo_de_vida;
    int cada_cuando = 60;
    int rabia;
    int guarda_pos;
    int graficos;
    int esperate;
    int angulo;
    int rebotes;
    int esperate1;
    int una_vez;
    int explotao;
BEGIN
    vida_elicoptero1 = 10 + vidaenemigo;
    ctype = c_scroll;
    z = -11;
    alea = rand( 15, 70 );
    rebotes = 1;
    s_elice = sound_play( elicop, -1, 19 );
    de_onde_salgo = rand( 1, 2 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        rato++;
        tiempo_de_vida++;
        if ( collision( type escopetazo ))
            vida_elicoptero1 = 0;
            rebotes = 0;
        end
        if ( collision( type explosion ) OR collision( type llamazo ) OR collision( type explosion3 ))
            vida_elicoptero1--;
            graph = 20;
            explosion2( x, y );
        end
        if ( elicopteros <= 0 )
            elicopteros = 1;
        end
        // si josel muere tambi‚n para el sonido
        if ( limite_vida_player1 =< 0 AND una_vez == 0 )
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
                una_vez = 1;
            end
        end
        // para el sonido al salir de la pantalla onde esta
        if ( x_p1 > 2990 AND estado_fase == 1 )
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
            end
        end
        if ( no_jodas > 0 )
            no_jodas--;
        end
        if ( esperate1 > 0 )
            esperate1--;
        end
        // esto hace que el enemigo se cabree
        if ( rabia == 1 AND x > x_p1 -10 AND x < x_p1 + 10 )
            torpedo( x, y );
            s_elicop = sound_play( lanzalla, 0 );
            esperate1 = 20;
            rabia = 0;
        end
        if ( tiempo_de_vida == 195 )
            if ( guarda_pos == x_p1 )
                rabia = 1;
            end
            if ( guarda_pos != x_p1 )
                rabia = 0;
            end
        end
        // hazlo m s salvaje
        if ( tiempo_de_vida => 200 )
            cada_cuando--;
            tiempo_de_vida = 0;
            guarda_pos = x_p1;
        end
        if ( cada_cuando < 8 )
            cada_cuando = 8;
        end
        if ( lo_ve == 1 )
            si_dispara = rand( 0, cada_cuando );
        end
        if ( si_dispara == 5 AND vida_elicoptero1 > 0 AND esperate1 == 0 )
            s_elicop = sound_play( pistola, 0, 17 );
            esperate1 = 10;
            if ( flags == 1 )
                bala_enemigo( x + 74, y + 49, 295, 0, 1, 1 );
            end
            if ( flags == 0 )
                bala_enemigo( x -74, y + 49, 295, 0, 1, 2 );
            end
        end
        if ( vida_elicoptero1 > 0 )
            if ( rutina == 1 )
                angulo = 5000;
            end
        end
        if (( flags == 0 AND avance == 1 ) OR ( flags == 1 AND avance == 2 ))
            angulo = 0;
        end
        if ( vida_elicoptero1 > 0 )
            y += y_incr;
        end
        // El elicoptero muere, si kedan mas elicopteros sale otro
        if ( rebotes =< 0 OR y > 480 )
            puntuacion += 300;
            elicopteros--;
            clones_eliminados++;
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
            end
            if ( explotao == 0 )
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                explotao = 1;
            end
            cacho_heli( x, y, 237, fase1, 1 );
            cacho_heli( x, y, 238, fase1, 2 );
            cacho_heli( x, y, 239, fase1, 3 );
            cacho_heli( x, y, 240, fase1, 4 );
            if ( elicopteros > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -640;
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 640;
                end
                elicoptero1( ponmelo, -100, 24, fase1, 1 );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        if ( x > x_p1 -300 AND x < x_p1 + 300 )
            lo_ve = 1;
        end
        if ( vida_elicoptero1 <= 0 )
            angle += angulo;
            incr_y++;
            y += incr_y;
            if ( incr_x < 4 )
                incr_x = 4;
            end
            if ( flags == 0 )
                avance = 2;
            else
                avance = 1;
            end
            if ( x < 6 )
                x = 6;
            end
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                incr_y = -10;
                rebotes--;
                if ( x > limite_pantalla )
                    x = limite_pantalla;
                end
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                if ( explotao == 0 )
                    explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                    explotao = 1;
                end
                cacho_heli( x, y, 237, fase1, 1 );
                cacho_heli( x, y, 238, fase1, 2 );
                cacho_heli( x, y, 239, fase1, 3 );
                cacho_heli( x, y, 240, fase1, 4 );
            end
        end
        if ( esperate > 0 )
            esperate--;
        end
        // colision con otros objetos
        if ( no_jodas == 0 AND esperate == 0 AND collision( type bala ))
            graph = 20;
            esperate = 3;
            vida_elicoptero1--;
            sound_play( smetal1, 0, 21 );
            no_jodas = 5;
            explosion2( x, y );
        end
        if ( collision( type carro ))
            graph = 20;
            esperate = 3;
            vida_elicoptero1--;
            y -= 12;
            y_incr = -8;
        end
        if ( tipo == 1 AND esperate == 0 )
            graficos++;
            if ( graficos == 1 )
                graph = 24;
            end
            if ( graficos == 2 )
                graph = 25;
            end
            if ( graficos == 3 )
                graph = 26;
            end
            if ( graficos == 4 )
                graficos = 0;
            end
        end
        if ( rato == alea )
            y_incr = rand( -4, 5 );
            rato = 0;
            alea = rand( 15, 70 );
        end
        if ( y > 250 AND vida_elicoptero1 > 0 )
            y = 250;
        end
        if ( y < 10 AND vida_elicoptero1 > 0 )
            y = 10;
        end
        if ( rutina == 1 ) // rutina de la I.A  ** Mision, atacar
            if ( x < x_p1 -320 AND vida_elicoptero1 > 0 AND avance == 2 )
                avance = 1;
                flags = 1;
                incr_x = 0;
            end
            if ( x > x_p1 + 320 AND vida_elicoptero1 > 0 AND avance == 1 )
                avance = 2;
                flags = 0;
                incr_x = 0;
            end
            if ( avance == 1 )
                x += incr_x;
            end
            if ( avance == 2 )
                x -= incr_x;
            end
        end
        if ( rutina == 0 ) // rutina de la I.A  ** Mision, buscar objetivo
            if ( x < x_p1 )
                x += incr_x;
                flags = 1;
                if ( x > x_p1 -100 AND x < x_p1 + 100 )
                    rutina = 1;
                    avance = 2;
                    incr_x = 0;
                end
            end
            if ( x > x_p1 )
                x -= incr_x;
                flags = 0;
                if ( x > x_p1 -100 AND x < x_p1 + 100 )
                    rutina = 1;
                    avance = 1;
                    incr_x = 0;
                end
            end
        end
        if ( semaforo == 0 )
            if ( incr_x < 6 )
                incr_x++;
            end
            semaforo = 1;
        else
            semaforo = 0;
        end
        frame;
    end
end


PROCESS elicoptero2( double x, double y, int graph, int file, int tipo );
PRIVATE
    int incr_x;
    int semaforo;
    int s_elicop; // sonido de disparo del elicoptero
    int s_elice; // sonido de las helices del helicoptero
    int no_jodas; // cuando hay ke solucionar problemas se hace cualkier intento
    int vida_elicoptero1;
    int si_dispara; // depende de la dificultad
    int rutina;
    int avance;
    int rato;
    int alea;
    double y_incr;
    int incr_y = -6;
    int lo_ve;
    // esto hace k el enemigo sea cada vez + inteligente
    int de_onde_salgo; // al morir el siguiente helicoptero sale fuera de pantalla acercandose
    int ponmelo;
    int tiempo_de_vida;
    int cada_cuando = 60;
    int rabia;
    int guarda_pos;
    int graficos;
    int esperate;
    int angulo;
    int rebotes;
    int esperate1;
    int una_vez;
    int espera;
    int explotao;
BEGIN
    vida_elicoptero1 = 10 + vidaenemigo;
    ctype = c_scroll;
    z = -11;
    alea = rand( 15, 70 );
    rebotes = 1;
    s_elice = sound_play( elicop, -1, 19 );
    de_onde_salgo = rand( 1, 2 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        rato++;
        tiempo_de_vida++;
        if ( tipo == 1 AND esperate == 0 )
            graficos++;
            if ( graficos == 1 )
                graph = 200;
            end
            if ( graficos == 2 )
                graph = 201;
            end
            if ( graficos == 3 )
                graph = 202;
            end
            if ( graficos == 4 )
                graficos = 0;
            end
        end
        if ( collision( type escopetazo ))
            vida_elicoptero1 = 0;
            rebotes = 0;
        end
        if ( collision( type explosion ) OR collision( type llamazo ) OR collision( type explosion3 ))
            vida_elicoptero1--;
            graph = 199;
            explosion2( x, y );
        end
        if ( elicopteros <= 0 )
            elicopteros = 1;
        end
        // si josel muere tambi‚n para el sonido
        if ( limite_vida_player1 =< 0 AND una_vez == 0 )
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
                una_vez = 1;
            end
        end
        // para el sonido al salir de la pantalla onde esta
        if ( x_p1 > 2990 AND estado_fase == 1 )
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
            end
        end
        if ( no_jodas > 0 )
            no_jodas--;
        end
        if ( esperate1 > 0 )
            esperate1--;
        end
        // esto hace que el enemigo se cabree
        if ( rabia == 1 AND x > x_p1 -10 AND x < x_p1 + 10 )
            torpedo( x, y );
            s_elicop = sound_play( lanzalla, 0 );
            esperate1 = 20;
            rabia = 0;
        end
        if ( tiempo_de_vida == 195 )
            if ( guarda_pos == x_p1 )
                rabia = 1;
            end
            if ( guarda_pos != x_p1 )
                rabia = 0;
            end
        end
        // hazlo m s salvaje
        if ( tiempo_de_vida => 200 )
            cada_cuando--;
            tiempo_de_vida = 0;
            guarda_pos = x_p1;
        end
        if ( cada_cuando < 8 )
            cada_cuando = 8;
        end
        if ( lo_ve == 1 )
            si_dispara = rand( 0, cada_cuando );
        end
        if ( si_dispara == 5 AND vida_elicoptero1 > 0 AND esperate1 == 0 )
            if ( avance == 1 )
                torpedo( x, y );
                s_elicop = sound_play( lanzalla, 0 );
                esperate1 = 10;
            end
            if ( avance == 2 )
                torpedo( x, y );
                s_elicop = sound_play( lanzalla, 0 );
                esperate1 = 10;
            end
        end
        if ( vida_elicoptero1 > 0 )
            if ( rutina == 1 )
                angulo = 5000;
            end
        end
        if (( flags == 0 AND avance == 1 ) OR ( flags == 1 AND avance == 2 ))
            angulo = 0;
        end
        if ( vida_elicoptero1 > 0 )
            y += y_incr;
        end
        // El elicoptero muere, si kedan mas elicopteros sale otro
        if ( rebotes =< 0 OR y > 375 )
            puntuacion += 300;
            elicopteros--;
            clones_eliminados++;
            if ( sound_is_playing( s_elice ))
                sound_stop( s_elice );
            end
            if ( explotao == 0 )
                explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                explotao = 1;
            end
            cacho_heli( x, y, 237, fase1, 1 );
            cacho_heli( x, y, 238, fase1, 2 );
            cacho_heli( x, y, 239, fase1, 3 );
            cacho_heli( x, y, 240, fase1, 4 );
            if ( elicopteros > 0 )
                if ( de_onde_salgo == 1 )
                    ponmelo = x_p1 -640;
                end
                if ( de_onde_salgo == 2 )
                    ponmelo = x_p1 + 640;
                end
                lancha( ponmelo, 355 );
                signal( id, s_kill );
            end
            signal( id, s_kill );
        end
        if ( x > x_p1 -300 AND x < x_p1 + 300 )
            lo_ve = 1;
        end
        if ( vida_elicoptero1 <= 0 )
            angle += angulo;
            incr_y++;
            y += incr_y;
            if ( incr_x < 4 )
                incr_x = 4;
            end
            if ( flags == 0 )
                avance = 2;
            else
                avance = 1;
            end
            if ( x < 6 )
                x = 6;
            end
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                incr_y = -10;
                rebotes--;
                if ( x > limite_pantalla )
                    x = limite_pantalla;
                end
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                if ( explotao == 0 )
                    explosion( x + rand( -10, 10 ), y + rand( -10, 10 ), rand( 150, 250 ), rand( 0, 1 ));
                    explotao = 1;
                end
                cacho_heli( x, y, 237, fase1, 1 );
                cacho_heli( x, y, 238, fase1, 2 );
                cacho_heli( x, y, 239, fase1, 3 );
                cacho_heli( x, y, 240, fase1, 4 );
            end
        end
        if ( esperate > 0 )
            esperate--;
        end
        // colision con otros objetos
        if ( no_jodas == 0 AND esperate == 0 AND collision( type bala ))
            graph = 199;
            esperate = 3;
            vida_elicoptero1--;
            sound_play( smetal1, 0, 21 );
            no_jodas = 5;
            explosion2( x, y );
        end
        if ( collision( type carro ))
            graph = 199;
            esperate = 3;
            vida_elicoptero1--;
            y -= 12;
            y_incr = -8;
        end
        if ( rato == alea )
            y_incr = rand( -4, 5 );
            rato = 0;
            alea = rand( 15, 70 );
        end
        if ( y > 50 AND vida_elicoptero1 > 0 )
            y = 50;
        end
        if ( y < 10 AND vida_elicoptero1 > 0 )
            y = 10;
        end
        if ( rutina == 1 ) // rutina de la I.A  ** Mision, atacar
            if ( x < x_p1 -320 AND vida_elicoptero1 > 0 AND avance == 2 )
                avance = 1;
                flags = 1;
                incr_x = 0;
            end
            if ( x > x_p1 + 320 AND vida_elicoptero1 > 0 AND avance == 1 )
                avance = 2;
                flags = 0;
                incr_x = 0;
            end
            if ( avance == 1 )
                x += incr_x;
            end
            if ( avance == 2 )
                x -= incr_x;
            end
        end
        if ( rutina == 0 ) // rutina de la I.A  ** Mision, buscar objetivo
            if ( x < x_p1 )
                x += incr_x;
                flags = 1;
                if ( x > x_p1 -100 AND x < x_p1 + 100 )
                    rutina = 1;
                    avance = 2;
                    incr_x = 0;
                end
            end
            if ( x > x_p1 )
                x -= incr_x;
                flags = 0;
                if ( x > x_p1 -100 AND x < x_p1 + 100 )
                    rutina = 1;
                    avance = 1;
                    incr_x = 0;
                end
            end
        end
        if ( semaforo == 0 )
            if ( incr_x < 6 )
                incr_x++;
            end
            semaforo = 1;
        else
            semaforo = 0;
        end
        frame;
    end
end

// EFECTOS ESPECIALES

PROCESS explosion( double x, double y, double size, int tipo );
PRIVATE
    int s_explo;
    int una_vez;
    int dos_veces;
BEGIN
    graph = 155;
    flags = 16;
    if ( estado_fase != 22 )
        ctype = c_scroll;
    end
    z = -14;
    existe_explosion++;
    boom = 1;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    if ( tipo == 1 OR tipo == 0 )
        luzjose();
    end
    if ( size > 200 )
        size = 200;
    end
    sound_play( explo1, 0, 22 );
    if ( estado_fase != 22 )
        piedras( x, y, 398, rand( 70, 95 ), 1 );
        piedras( x, y, 398, rand( 70, 95 ), 2 );
        piedras( x, y, 398, rand( 70, 95 ), 3 );
        piedras( x, y, 398, rand( 70, 95 ), 4 );
        piedras( x, y, 398, rand( 70, 95 ), 5 );
        piedras( x, y, 398, rand( 70, 95 ), 6 );
        piedras( x, y, 398, rand( 70, 95 ), 7 );
        piedras( x, y, 398, rand( 70, 95 ), 8 );
        piedras( x, y, 398, rand( 70, 95 ), 9 );
        piedras( x, y, 398, rand( 70, 95 ), 10 );
        piedras( x, y, 398, rand( 70, 95 ), 11 );
        piedras( x, y, 398, rand( 70, 95 ), 12 );
        piedras( x, y, 398, rand( 70, 95 ), 13 );
        piedras( x, y, 398, rand( 70, 95 ), 14 );
        piedras( x, y, 398, rand( 70, 95 ), 15 );
        piedras( x, y, 398, rand( 70, 95 ), 16 );
        piedras( x, y, 398, rand( 70, 95 ), 17 );
        piedras( x, y, 398, rand( 70, 95 ), 18 );
        piedras( x, y, 398, rand( 70, 95 ), 19 );
        piedras( x, y, 398, rand( 70, 95 ), 20 );
        piedras( x, y, 398, rand( 70, 95 ), 21 );
        piedras( x, y, 398, rand( 70, 95 ), 22 );
        piedras( x, y, 398, rand( 70, 95 ), 23 );
        piedras( x, y, 398, rand( 70, 95 ), 24 );
    end
    LOOP
        if ( graph == 157 AND dos_veces == 0 )
            explosion2( x -83, y -72 );
            explosion2( x + 89, y + 63 );
            explosion2( x -83, y + 63 );
            explosion2( x + 89, y -72 );
            dos_veces = 1;
        end
        if ( graph == 160 AND dos_veces == 1 )
            explosion2( x -141, y -108 );
            explosion2( x + 136, y + 104 );
            explosion2( x -141, y + 104 );
            explosion2( x + 136, y -108 );
            dos_veces = 2;
        end
        if ( graph == 189 )
            existe_explosion--;
            signal( id, s_kill );
        end
        if ( graph < 189 )
            graph++;
        end
        frame;
    end
end


PROCESS explosion3( double x, double y, double size, int tipo );
PRIVATE
    int s_explo;
    int una_vez;
BEGIN
    size = 200;
    graph = 547;
    ctype = c_scroll;
    z = -12;
    existe_explosion++;
    boom = 1;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    if ( tipo == 1 OR tipo == 0 )
        luzjose();
    end
    if ( estado_fase != 3 )
        sound_play( explo1, 0, 22 );
    end
    if ( estado_fase == 3 )
        sound_play( lanzalla, 0 );
    end
    LOOP
        if ( collision( type jose_player1 ))
            josel_tocao = 1;
        end
        if ( graph == 566 )
            existe_explosion--;
            signal( id, s_kill );
        end
        if ( graph < 566 )
            graph++;
        end
        frame;
    end
end


PROCESS explosion2( double x, double y );
PRIVATE
    int s_explo;
    int una_vez;
BEGIN
    graph = 436;
    if ( estado_fase != 22 )
        ctype = c_scroll;
    end
    z = -13;
    size = 200;
    x += rand( -25, 25 );
    y += rand( -25, 25 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( graph == 449 )
            signal( id, s_kill );
        end
        if ( graph < 449 )
            graph++;
        end
        frame;
    end
end


PROCESS humo( double x, double y );
PRIVATE
    int s_explo;
    int una_vez;
BEGIN
    graph = 577;
    if ( estado_fase != 22 )
        ctype = c_scroll;
    end
    z = -12;
    size = 180;
    x += rand( -5, 5 );
    y += rand( -5, 5 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    if ( graph == 293 )
        alpha = 100;
    end
    LOOP
        if ( graph == 588 )
            signal( id, s_kill );
        end
        if ( graph < 588 )
            graph++;
        end
        frame;
    end
end


PROCESS cacho_heli( double x, double y, int graph, int file, int tipo );
PRIVATE
    double y_incr;
    int humos;
BEGIN
    if ( estado_fase != 22 )
        ctype = c_scroll;
    end
    z = -6;
    y_incr = rand( -15, -20 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        humos++;
        if ( humos == 15 )
            humo( x, y );
            humos = 0;
        end
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        if ( y_incr > 20 )
            y_incr = 15;
        end
        angle += 50000;
        if ( tipo == 1 )
            x += 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 2 )
            x += 10;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 3 )
            x -= 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 4 )
            x -= 10;
            y_incr++;
            y += y_incr;
        end
        if ( collision( type agua ))
            salpicada( x, y, 170 );
            trozo_suelo( x, y + 15, 377, 10 );
            trozo_suelo( x, y + 15, 377, 20 );
            trozo_suelo( x, y + 15, 377, 10 );
            existe_bala--;
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS piedras( double x, double y, int graph, double size, int tipo ); // muy importante
PRIVATE
    double y_incr;
    int rebotes = 2;
    int anula_angle;
BEGIN
    z = -6;
    if ( modo_wiz == 1 )
        if ( graph == 398 )
            return;
        end
    end
    if ( estado_fase != 22 )
        ctype = c_scroll;
    end
    if ( tipo =< 12 )
        y_incr = rand( -15, -25 );
    end
    if ( tipo => 13 )
        y_incr = rand( 5, 15 );
    end
    if ( haz_reflejos == 1 AND modo_wiz == 0 )
        imita_2();
    end
    if ( modo_wiz == 1 )
        anula_angle = 1;
        if ( tipo == 1 OR tipo == 2 OR tipo == 7 OR tipo == 8 OR tipo == 13 OR tipo == 14 OR tipo == 19 OR tipo == 20 )
            anula_angle = 0;
        end
    end
    LOOP
        if ( anula_angle == 0 )
            angle += 30000;
        end
        if ( collision( type agua ))
            salpicada( x, y, 100 );
            signal( id, s_kill );
        end
        if ( y_incr > 0 AND rebotes > 0 AND map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
            y_incr = -10;
            rebotes--;
            if ( graph == 568 OR graph == 569 OR graph == 570 OR graph == 571 OR graph == 572 OR graph == 573 OR graph == 574 )
                if ( modo_wiz == 0 OR rebotes < 1 )
                    sangre_mix( 0, x, y, flags, 0 );
                end
            end
        end
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        if ( y_incr > 20 )
            y_incr = 15;
        end
        if ( tipo == 1 )
            x += 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 2 )
            x += 10;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 3 )
            x += 15;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 4 )
            x += 20;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 5 )
            x += 25;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 6 )
            x += 30;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 7 )
            x -= 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 8 )
            x -= 10;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 9 )
            x -= 15;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 10 )
            x -= 20;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 11 )
            x -= 25;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 12 )
            x -= 30;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 13 )
            x += 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 14 )
            x += 10;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 15 )
            x += 15;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 16 )
            x += 20;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 17 )
            x += 25;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 18 )
            x += 30;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 19 )
            x -= 5;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 20 )
            x -= 10;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 21 )
            x -= 15;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 22 )
            x -= 20;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 23 )
            x -= 25;
            y_incr++;
            y += y_incr;
        end
        if ( tipo == 24 )
            x -= 30;
            y_incr++;
            y += y_incr;
        end
        frame;
    end
end


PROCESS bombazos( double x, double y );
PRIVATE
    int s_explo;
    int una_vez;
    int tipo;
    int espera = 5;
    int una;
BEGIN
    graph = 155;
    ctype = c_scroll;
    tipo = rand( 0, 1 );
    flags = 16;
    z = -8;
    boom = 1;
    size = rand( 150, 250 );
    if ( tipo == 1 )
        luzjose();
    end
    sound_play( explo1, 0, 22 );
    onda_exp( x, y );
    piedras( x, y, 398, rand( 70, 95 ), 1 );
    piedras( x, y, 398, rand( 70, 95 ), 2 );
    piedras( x, y, 398, rand( 70, 95 ), 3 );
    piedras( x, y, 398, rand( 70, 95 ), 4 );
    piedras( x, y, 398, rand( 70, 95 ), 5 );
    piedras( x, y, 398, rand( 70, 95 ), 6 );
    piedras( x, y, 398, rand( 70, 95 ), 7 );
    piedras( x, y, 398, rand( 70, 95 ), 8 );
    piedras( x, y, 398, rand( 70, 95 ), 9 );
    piedras( x, y, 398, rand( 70, 95 ), 10 );
    piedras( x, y, 398, rand( 70, 95 ), 11 );
    piedras( x, y, 398, rand( 70, 95 ), 12 );
    piedras( x, y, 398, rand( 70, 95 ), 13 );
    piedras( x, y, 398, rand( 70, 95 ), 14 );
    piedras( x, y, 398, rand( 70, 95 ), 15 );
    piedras( x, y, 398, rand( 70, 95 ), 16 );
    piedras( x, y, 398, rand( 70, 95 ), 17 );
    piedras( x, y, 398, rand( 70, 95 ), 18 );
    piedras( x, y, 398, rand( 70, 95 ), 19 );
    piedras( x, y, 398, rand( 70, 95 ), 20 );
    piedras( x, y, 398, rand( 70, 95 ), 21 );
    piedras( x, y, 398, rand( 70, 95 ), 22 );
    piedras( x, y, 398, rand( 70, 95 ), 23 );
    piedras( x, y, 398, rand( 70, 95 ), 24 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( graph == 189 )
            signal( id, s_kill );
        end
        if ( graph < 189 )
            graph++;
        end
        frame;
    end
end


PROCESS luzjose();
PRIVATE
    int esperate;
BEGIN
    x = 320;
    y = 240;
    graph = 14;
    size = 200;
    z = -5;
    alpha = 128;
    LOOP
        esperate++;
        if ( esperate == 3 )
            signal( type luzjose, s_kill );
        end
        frame;
    end
end


PROCESS brillos( int graph, int tipo );
PRIVATE
    int esperate;
BEGIN
    x = 320;
    y = 240;
    size = 200;
    z = -10;
    alpha = 128;
    LOOP
        esperate++;
        if ( esperate == tipo )
            signal( id, s_kill );
        end
        frame;
    end
end

// PERSONAJES SECUNDARIOS

PROCESS secundario( double x, double y, int graph, int file, int tipo );
PRIVATE
    int kiero_este; // mira al player 1 o 2
BEGIN
    ctype = c_scroll;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        kiero_este = x_p1;
        if ( tipo == 1 ) // los moros
            if ( x < kiero_este -50 )
                flags = 1;
            end
            if ( x > kiero_este + 50 )
                flags = 0;
            end
        end
        if ( tipo == 2 )
            alpha = 128;
            z = -8;
        end
        frame;
    end
end


PROCESS personajes_animados( int graph, int file, int tipo );
PRIVATE
    int estado2 = -1;
    int kiero_este;
    int haz_graficos;
BEGIN
    ctype = c_scroll;
    size = 120;
    z = -100;
    kiero_este = x_p1;
    if ( tipo == 1 )
        x = x_p1 + 700;
    end
    if ( tipo == 2 )
        x = x_p1 + 800;
    end
    if ( tipo == 3 )
        x = x_p1 -700;
    end
    if ( tipo == 4 )
        x = x_p1 -800;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( tipo == 1 ) // el camello con joroba
            estado2++;
            x -= 8;
            y = 479;
            flags = 1;
            if ( estado2 < 3 )
                y++;
            end
            if ( estado2 => 3 )
                y--;
            end
            if ( estado2 == 5 )
                estado2 = -1;
            end
            if ( x < x_p1 -700 )
                signal( id, s_kill );
            end
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 115;
            end
            if ( haz_graficos == 2 )
                graph = 116;
            end
            if ( haz_graficos == 3 )
                graph = 117;
            end
            if ( haz_graficos == 4 )
                graph = 118;
            end
            if ( haz_graficos == 5 )
                graph = 117;
            end
            if ( haz_graficos == 6 )
                graph = 116;
            end
            if ( haz_graficos => 7 )
                haz_graficos = 0;
            end
        end
        if ( tipo == 3 ) // el camello con joroba
            estado2++;
            x += 8;
            y = 479;
            flags = 0;
            if ( estado2 < 3 )
                y++;
            end
            if ( estado2 => 3 )
                y--;
            end
            if ( estado2 == 5 )
                estado2 = -1;
            end
            if ( x > x_p1 + 700 )
                signal( id, s_kill );
            end
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 115;
            end
            if ( haz_graficos == 2 )
                graph = 116;
            end
            if ( haz_graficos == 3 )
                graph = 117;
            end
            if ( haz_graficos == 4 )
                graph = 118;
            end
            if ( haz_graficos == 5 )
                graph = 117;
            end
            if ( haz_graficos == 6 )
                graph = 116;
            end
            if ( haz_graficos => 7 )
                haz_graficos = 0;
            end
        end
        if ( tipo == 2 )
            if ( x < x_p1 -700 )
                signal( id, s_kill );
            end
            x -= 7;
            y = 509;
            flags = 1;
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 37;
            end
            if ( haz_graficos == 2 )
                graph = 38;
            end
            if ( haz_graficos == 3 )
                graph = 39;
            end
            if ( haz_graficos == 4 )
                graph = 40;
            end
            if ( haz_graficos == 5 )
                graph = 41;
            end
            if ( haz_graficos == 6 )
                graph = 42;
            end
            if ( haz_graficos == 7 )
                graph = 43;
            end
            if ( haz_graficos == 8 )
                graph = 44;
            end
            if ( haz_graficos == 9 )
                graph = 45;
            end
            if ( haz_graficos == 10 )
                graph = 46;
            end
            if ( haz_graficos == 11 )
                graph = 47;
            end
            if ( haz_graficos == 12 )
                graph = 48;
            end
            if ( haz_graficos == 13 )
                graph = 49;
            end
            if ( haz_graficos => 14 )
                graph = 50;
                haz_graficos = 0;
            end
        end
        if ( tipo == 4 )
            if ( x > x_p1 + 700 )
                signal( id, s_kill );
            end
            x += 7;
            y = 509;
            flags = 0;
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 37;
            end
            if ( haz_graficos == 2 )
                graph = 38;
            end
            if ( haz_graficos == 3 )
                graph = 39;
            end
            if ( haz_graficos == 4 )
                graph = 40;
            end
            if ( haz_graficos == 5 )
                graph = 41;
            end
            if ( haz_graficos == 6 )
                graph = 42;
            end
            if ( haz_graficos == 7 )
                graph = 43;
            end
            if ( haz_graficos == 8 )
                graph = 44;
            end
            if ( haz_graficos == 9 )
                graph = 45;
            end
            if ( haz_graficos == 10 )
                graph = 46;
            end
            if ( haz_graficos == 11 )
                graph = 47;
            end
            if ( haz_graficos == 12 )
                graph = 48;
            end
            if ( haz_graficos == 13 )
                graph = 49;
            end
            if ( haz_graficos => 14 )
                graph = 50;
                haz_graficos = 0;
            end
        end
        frame;
    end
end

// OBJETOS DE LAS FASES !!

PROCESS pinchos( double x, double y, int graph, int file );
PRIVATE
    int estado;
    int haz_graficos;
BEGIN
    size = 50;
    ctype = c_scroll;
    if ( graph == 185 )
        estado = 1;
        size = 100;
    end
    LOOP
        if ( estado == 1 )
            haz_graficos++;
            if ( haz_graficos == 1 )
                graph = 186;
            end
            if ( haz_graficos == 3 )
                graph = 187;
            end
            if ( haz_graficos == 5 )
                graph = 188;
            end
            if ( haz_graficos == 7 )
                graph = 187;
            end
            if ( haz_graficos == 9 )
                graph = 186;
            end
            if ( haz_graficos => 10 )
                haz_graficos = 0;
            end
        end
        frame;
    end
end


PROCESS bala_enemigo( double x, double y, int graph, int file, int tipo, int sentido );
PRIVATE
    int balafuera;
    int haz_graficos;
BEGIN
    z = -2;
    ctype = c_scroll;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        // disparos robot
        if ( tipo == 20 )
            haz_graficos++;
            x -= 10;
            if ( haz_graficos == 1 )
                graph = 350;
            end
            if ( haz_graficos == 2 )
                graph = 351;
            end
            if ( haz_graficos == 3 )
                graph = 352;
            end
            if ( haz_graficos == 4 )
                graph = 353;
            end
            if ( haz_graficos == 5 )
                graph = 352;
            end
            if ( haz_graficos == 6 )
                graph = 351;
            end
            if ( haz_graficos == 7 )
                haz_graficos = 0;
            end
        end
        if ( tipo == 21 )
            haz_graficos++;
            x += 10;
            if ( haz_graficos == 1 )
                graph = 350;
            end
            if ( haz_graficos == 2 )
                graph = 351;
            end
            if ( haz_graficos == 3 )
                graph = 352;
            end
            if ( haz_graficos == 4 )
                graph = 353;
            end
            if ( haz_graficos == 5 )
                graph = 352;
            end
            if ( haz_graficos == 6 )
                graph = 351;
            end
            if ( haz_graficos == 7 )
                haz_graficos = 0;
            end
        end
        if ( tipo == 22 )
            haz_graficos++;
            angle = 321000;
            x -= 16;
            y -= 12;
            if ( haz_graficos == 1 )
                graph = 350;
            end
            if ( haz_graficos == 2 )
                graph = 351;
            end
            if ( haz_graficos == 3 )
                graph = 352;
            end
            if ( haz_graficos == 4 )
                graph = 353;
            end
            if ( haz_graficos == 5 )
                graph = 352;
            end
            if ( haz_graficos == 6 )
                graph = 351;
            end
            if ( haz_graficos == 7 )
                haz_graficos = 0;
            end
        end
        if ( tipo == 23 )
            haz_graficos++;
            angle = 225000;
            x += 16;
            y -= 12;
            if ( haz_graficos == 1 )
                graph = 350;
            end
            if ( haz_graficos == 2 )
                graph = 351;
            end
            if ( haz_graficos == 3 )
                graph = 352;
            end
            if ( haz_graficos == 4 )
                graph = 353;
            end
            if ( haz_graficos == 5 )
                graph = 352;
            end
            if ( haz_graficos == 6 )
                graph = 351;
            end
            if ( haz_graficos == 7 )
                haz_graficos = 0;
            end
        end
        // disparos helicoptero
        if ( tipo == 50 )
            y += 10;
            angle = 270000;
        end
        if ( tipo == 1 )
            if ( sentido == 1 )
                x += 10;
                y += 6;
                flags = 0;
            end
            if ( sentido == 2 )
                x -= 10;
                y += 6;
                flags = 1;
            end
        end
        if ( tipo == 2 )
            y += 7;
        end
        if ( balafuera == 2 )
            signal( id, s_kill );
        end
        // disparos enemigos de tierra
        // disparo triple de la metralleta
        if ( tipo == 7 ) // disparo uzi desviado hacia arriba , recto
            if ( sentido == 0 )
                x += 15;
                y -= 5;
            end
            if ( sentido == 1 )
                x -= 15;
                y -= 5;
            end
        end
        if ( tipo == 8 ) // disparo uzi desviado hacia abajo , recto
            if ( sentido == 0 )
                x += 15;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 15;
                y += 5;
            end
        end
        if ( tipo == 9 ) // disparo uzi desviado hacia arriba, vertical
            if ( sentido == 0 )
                x += 15;
                y -= 25;
                angle = 45000;
            end
            if ( sentido == 1 )
                x -= 15;
                y -= 25;
                angle = 315000;
            end
        end
        if ( tipo == 10 ) // disparo uzi desviado hacia abajo, vertical
            if ( sentido == 0 )
                x += 20;
                y -= 5;
                angle = 45000;
            end
            if ( sentido == 1 )
                x -= 20;
                y -= 5;
                angle = 315000;
            end
        end
        if ( tipo == 11 ) // disparo uzi desviado hacia izq, arriba
            if ( sentido == 0 )
                x -= 5;
                y -= 15;
            end
            if ( sentido == 1 )
                x += 5;
                y -= 15;
            end
        end
        if ( tipo == 12 ) // disparo uzi desviado hacia der, arriba
            if ( sentido == 0 )
                x += 5;
                y -= 15;
            end
            if ( sentido == 1 )
                x -= 5;
                y -= 15;
            end
        end
        if ( tipo == 13 ) // disparo uzi desviado hacia izq, abajo
            if ( sentido == 0 )
                x -= 5;
                y += 15;
            end
            if ( sentido == 1 )
                x += 5;
                y += 15;
            end
        end
        if ( tipo == 14 ) // disparo uzi desviado hacia der, abajo
            if ( sentido == 0 )
                x += 5;
                y += 15;
            end
            if ( sentido == 1 )
                x -= 5;
                y += 15;
            end
        end
        if ( tipo == 16 ) // disparo uzi desviado hacia izq,ver abajo
            if ( sentido == 0 )
                x -= 20;
                y += 15;
            end
            if ( sentido == 1 )
                x += 20;
                y += 15;
            end
        end
        if ( tipo == 17 ) // disparo uzi desviado hacia der,ver abajo
            if ( sentido == 0 )
                x += 20;
                y += 15;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 15;
            end
        end
        if ( tipo == 18 ) // disparo uzi desviado hacia izq,ver arriba
            if ( sentido == 0 )
                x += 20;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 5;
            end
        end
        if ( tipo == 19 ) // disparo uzi desviado hacia der,ver arriba
            if ( sentido == 0 )
                x += 20;
                y += 5;
            end
            if ( sentido == 1 )
                x -= 20;
                y += 5;
            end
        end
        if ( tipo == 3 )
            if ( sentido == 1 )
                x -= 10;
                flags = 1;
                if ( graph == 361 )
                    x -= 5;
                end
            end
            if ( sentido == 0 )
                x += 10;
                flags = 0;
                if ( graph == 361 )
                    x += 5;
                end
            end
        end
        if ( tipo == 4 )
            if ( sentido == 0 )
                x += 16;
                y -= 12;
                flags = 1;
            end
            if ( sentido == 1 )
                x -= 16;
                y -= 12;
                flags = 0;
            end
        end
        // colisiona con el agua
        if ( collision( type agua ))
            salpicada( x, y, 100 );
            signal( id, s_kill );
        end
        if ( collision( type jose_player1 ) OR collision( type enemigo_arabe ))
            balafuera = 2;
            trozo_suelo( x, y -3, 293, 1 );
            trozo_suelo( x, y -3, 293, 2 );
            trozo_suelo( x, y -3, 293, 3 );
        end
        // colision con los elementos de la pantalla
        if ( balafuera == 1 )
            polvo( x + rand( -6, 6 ), y - rand( -3, 3 ), 264, 3 );
            signal( id, s_kill );
        end
        if ( collision( type carro ) OR collision( type elemento_destructible ) OR collision( type prohibido_elpaso ))
            trozo_suelo( x, y -3, 291, 1 );
            trozo_suelo( x, y -3, 291, 2 );
            trozo_suelo( x, y -3, 291, 1 );
            balafuera = 1;
        end
        if ( collision( type copa_palmera ))
            trozo_suelo( x, y -3, 292, 1 );
            trozo_suelo( x, y -3, 292, 2 );
            trozo_suelo( x, y -3, 292, 1 );
            balafuera = 1;
        end
        // colision con el suelo
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        if (( y > 350 ) AND ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es ) OR
            ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_pared_1 ) OR ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == color_pared_2 ))
            polvo( x + rand( -6, 6 ), y - rand( -3, 3 ), 264, 3 );
            trozo_suelo( x, y -3, 282, 1 );
            trozo_suelo( x, y -3, 282, 2 );
            trozo_suelo( x, y -3, 282, 1 );
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS primer_plano( double y, int graph, int file, double xrelativa );
BEGIN
    z = -9;
    priority = 1000;
    LOOP
        // y=400-scroll.y0*2;   esto halla la posicion relativa en un ascenso vertical
        x = ( xrelativa - scroll.x0 ) * 1.5; // Halla la posicion relativa al scroll
        frame;
    end
end


PROCESS polvo( double x, double y, int graph, int tipo );
PRIVATE
    int estado = 1;
    int grande = 100;
BEGIN
    ctype = c_scroll;
    size = 0;
    alpha = 128;
    z = -6;
    alpha = 180;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( modo_wiz == 0 )
            angle += 3000;
        end
        if ( alpha > 90 )
            alpha -= 10;
        end
        if ( estado == 1 )
            size += 20;
        end
        if ( estado == 1 AND size >= grande )
            estado = 2;
        end
        if ( estado == 2 )
            size -= 5;
        end
        if ( estado == 2 AND size < 10 )
            signal( id, s_kill );
        end
        if ( tipo == 1 )
            x--;
        end
        if ( tipo == 2 )
            x++;
        end
        if ( tipo == 5 )
            grande = 220;
        end
        if ( tipo == 6 )
            x -= 3;
            grande = 150;
        end
        if ( tipo == 7 )
            x += 3;
            grande = 150;
        end
        frame;
    end
end


PROCESS trozo_suelo( double x, double y, int graph, int tipo );
PRIVATE
    double y_incr = -6;
    double x_incr;
BEGIN
    if ( modo_wiz == 1 )
        return;
    end
    x_incr = rand( 2, 6 );
    ctype = c_scroll;
    z = -6;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    if ( graph == 293 )
        alpha = 180;
    end
    LOOP
        y_incr++;
        y += y_incr;
        angle += 10000;
        if ( tipo == 1 )
            x -= x_incr;
        end
        if ( tipo == 2 )
            x += x_incr;
        end
        if ( tipo == 10 )
            x -= x_incr;
            alpha = 128;
        end
        if ( tipo == 20 )
            x += x_incr;
            alpha = 128;
        end
        if ( tipo == 30 )
            alpha = 128;
        end
        if ( collision( type agua ))
            salpicada( x, y, 50 );
            signal( id, s_kill );
        end
        if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es AND graph != 328 )
            signal( id, s_kill );
        end
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS nube( double x, double y, int graph, int file, double size, int tipo );
PRIVATE
    int resultao;
BEGIN
    z = 3;
    ctype = c_scroll;
    if ( tiempo_actual == 12 )
        flags = 0;
    end
    if ( tiempo_actual == 1 OR tiempo_actual == 2 )
        alpha = 128;
    end
    LOOP
        resultao = x - x_p1;
        if ( tipo == 1 )
            x -= 3;
        end
        if ( tipo == 2 )
            x -= 2;
        end
        if ( tipo == 3 )
            x -= 1;
        end
        if ( resultao < -640 )
            x = x_p1 + 640;
        end
        frame;
    end
end


PROCESS cachocasa( double x, double y, int graph, int file );
BEGIN
    ctype = c_scroll;
    z = 2;
    LOOP
        if ( realiza_intro1 == 2 )
            graph = 225;
        end
        if ( realiza_intro1 == 3 )
            graph = 226;
        end
        if ( realiza_intro1 == 4 )
            graph = 227;
        end
        if ( realiza_intro1 == 5 )
            graph = 228;
        end
        if ( realiza_intro1 == 6 )
            graph = 229;
        end
        if ( realiza_intro1 == 7 )
            graph = 230;
        end
        if ( realiza_intro1 == 8 )
            graph = 231;
        end
        if ( realiza_intro1 == 9 )
            graph = 232;
        end
        if ( realiza_intro1 == 10 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS hierba( double x, double y, int graph, int file, int tipo );
BEGIN
    ctype = c_scroll;
    z = -9;
    if ( tipo == 10 )
        flags = 1;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        frame;
    end
end


PROCESS palla( double x, double y, int graph, int file );
BEGIN
    ctype = c_scroll;
    z = -10;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        frame;
    end
end


PROCESS piedra( double x, double y, int graph, int file, int flags );
BEGIN
    ctype = c_scroll;
    z = -8;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        frame;
    end
end


PROCESS viento( double x, double y, int graph, int file, int tipo );
PRIVATE
    int escena_camello; // este proceso ser  el encargao de hacer salir al cAmello
    int escena_cuando;
    int escena_lado = 1;
    int cabrones;
BEGIN
    alpha = 128;
    z = 1;
    escena_cuando = rand( 200, 1000 );
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        escena_camello++;
        if ( tipo == 1 )
            x += 10;
        end
        if ( x > 690 )
            x = rand( -1000, -10000 );
        end
        if ( escena_camello == escena_cuando ) // esto realiza la escena de persecucion
            if ( escena_lado == 2 )
                personajes_animados( 115, fase1, 3 );
                personajes_animados( 36, fase1, 4 );
                escena_camello = 0;
                escena_cuando = rand( 1000, 1500 );
                escena_lado = 1;
                cabrones = 1;
            end
            if ( escena_lado == 1 AND cabrones == 0 )
                personajes_animados( 115, fase1, 1 );
                personajes_animados( 36, fase1, 2 );
                escena_camello = 0;
                escena_cuando = rand( 1000, 1500 );
                escena_lado = 2;
            end
            cabrones = 0;
        end
        frame;
    end
end


PROCESS elemento_destructible( double x, double y, int graph, int file, int tipo );
PRIVATE
    int vida_elemento = 5;
    double y_incr = -5;
    double y_pos;
    int estado = -1;
    int s_tiempo;
BEGIN
    ctype = c_scroll;
    z = 1;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        y_pos = y;
        if ( collision( type explosion ) OR collision( type escopetazo ) OR collision( type llamazo ))
            vida_elemento = 0;
        end
        if ( s_tiempo > 0 )
            s_tiempo--;
        end
        if ( estado == -1 )
            if ( collision( type bala ) OR collision( type bala_enemigo ))
                vida_elemento--;
                if ( s_tiempo == 0 )
                    sound_play( madera, 0 );
                    s_tiempo = 5;
                end
            end
        end
        if ( tipo == 1 )
            if ( vida_elemento =< 0 )
                if ( estado == -1 )
                    estado = 0;
                end
            end
            if ( estado == 0 )
                y_incr++;
                y += y_incr;
                angle += 4600;
            end
            if ( estado == 0 )
                if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                    y = y_pos;
                    estado = 1;
                end
            end
            if ( estado == 1 )
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 5 );
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 5 );
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 5 );
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 1 );
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 2 );
                polvo( x + rand( -20, 20 ), y - rand( -15, 15 ), 264, 5 );
                puntuacion += 25;
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS carro( double x, double y, int graph, int file );
PRIVATE
    int vida_carro = 6;
    double y_incr = -20;
    int estado = -1;
    double x_incr;
    int angulo;
    int rebotes = 3;
    int s_tiempo;
BEGIN
    ctype = c_scroll;
    z = -1;
    x_incr = rand( -4, 4 );
    if ( x_incr < 0 )
        angulo = 5000;
    end
    if ( x_incr > -1 )
        angulo = -5000;
    end
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        if ( s_tiempo > 0 )
            s_tiempo--;
        end
        if ( collision( type escopetazo ) AND vida_carro > 0 )
            vida_carro = 0;
            y_incr = -15;
            angulo = 30000;
            if ( imita_flags == 0 )
                x_incr = 10;
            end
            if ( imita_flags == 1 )
                x_incr = -10;
            end
        end
        if ( collision( type explosion ) OR collision( type llamazo ))
            vida_carro = 0;
        end
        if ( collision( type bala ) OR collision( type bala_enemigo ))
            vida_carro--;
            if ( s_tiempo == 0 )
                sound_play( madera, 0 );
                s_tiempo = 5;
            end
        end
        if ( estado == -1 )
            if ( vida_carro =< 0 )
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                estado = 0;
            end
        end
        if ( rebotes =< 0 )
            puntuacion += 25;
            signal( id, s_kill );
        end
        if ( estado == 0 )
            angle += angulo;
            y_incr++;
            y += y_incr;
            x += x_incr;
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                y_incr = -8;
                rebotes--;
                polvo( x + rand( -50, 50 ), y - rand( -30, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 1 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 2 );
                polvo( x + rand( -50, 50 ), y - rand( -10, 30 ), 264, 5 );
            end
        end
        frame;
    end
end


PROCESS copa_palmera( double x, double y, int graph, int file );
PRIVATE
    int vida_palmera = 6;
    double y_incr = -5;
    double y_pos;
    int estado = -1;
    int cruje;
    int s_cruje;
    int s_tiempo;
BEGIN
    ctype = c_scroll;
    if ( haz_reflejos == 1 )
        imita_2();
    end
    LOOP
        y_pos = y;
        if ( collision( type explosion ) OR collision( type escopetazo ) OR collision( type llamazo ))
            vida_palmera = 0;
        end
        if ( s_tiempo > 0 )
            s_tiempo--;
        end
        if ( estado == -1 )
            if ( collision( type bala ) OR collision( type bala_enemigo ))
                vida_palmera--;
                if ( s_tiempo == 0 )
                    sound_play( madera, 0 );
                    s_tiempo = 5;
                end
            end
        end
        if ( vida_palmera =< 0 AND estado == -1 )
            estado = 0;
        end
        if ( estado == 0 )
            y_incr++;
            y += y_incr;
            angle += 4600;
            if ( cruje == 0 )
                s_cruje = sound_play( s_palmera, 0 );
                cruje = 1;
            end
        end
        if ( estado == 0 )
            if ( map_get_pixel( ke_fase_es, ke_grafico_es, x / dureza, y / dureza ) == ke_color_es )
                y = y_pos;
                estado = 1;
            end
        end
        if ( estado == 1 )
            if ( cruje == 1 )
                sound_stop( s_cruje );
                cruje = 2;
                sound_play( madera, 0 );
            end
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 1 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 2 );
            polvo( x + rand( -90, 10 ), y - rand( -35, 15 ), 264, 5 );
            puntuacion += 25;
            signal( id, s_kill );
        end
        frame;
    end
end

// FIN DE LOS OBJETOS DE LAS FASES
// Proceso de la pantalla de presentacion

PROCESS presentacion();
PRIVATE
    int Estado;
    int kiensale;
BEGIN
    // dump_type=1; restore_type=-1;
    x = 314;
    y = 125;
    graph = 16;
    size = 200;
    estado = 1;
    z = -1;
    sound_stop( all_sound );
    music_stop();
    if ( descarg_logo == 0 )
        music_unload( logo );
        descarg_logo = 1;
    end
    canal1 = music_play( sound2, -1 );
    music_set_volume( vol );
    if ( alterna_josel == 0 )
        kiensale = 1;
        alterna_josel = 1;
    else
        kiensale = 2;
        alterna_josel = 0;
    end
    LOOP
        if ( estado == 1 )
            if ( size > 100 )
                size -= 10;
            end
            if ( size =< 100 )
                size = 100;
                bordeizqarriba();
                bordeizqabajo();
                if ( kiensale == 1 )
                    joselpresentacion();
                end
                if ( kiensale == 2 )
                    joselpresentacion1();
                end
                bordederarriba();
                bordederabajo();
                presionaenter();
                creadorjuego();
                muevefondo();
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size < 100 )
                size++;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size > 90 )
                size--;
            else
                estado = 2;
            end
        end
        if ( entrando == 2 )
            estado = 4;
            size = 100;
            angle += 30000;
            y -= 10;
            if ( y < -100 )
                signal( type presentacion, s_kill );
            end
        end
        frame;
    end
end


PROCESS bordeizqarriba();
PRIVATE
    int Estado;
BEGIN
    x = 82;
    y = 4;
    graph = 17;
    z = -1;
    estado = 1;
    LOOP
        if ( estado == 1 )
            if ( y < 54 )
                y += 5;
            end
            if ( y => 54 )
                y = 54;
                estado = 2;
            end
        end
        if ( fueramenu > 1 )
            y -= 15;
        end
        if ( y < -30 )
            signal( type bordeizqarriba, s_kill );
        end
        frame;
    end
end


PROCESS bordederarriba();
PRIVATE
    int Estado;
BEGIN
    x = 555;
    y = 4;
    graph = 17;
    z = -1;
    estado = 1;
    flags = 1;
    LOOP
        if ( estado == 1 )
            if ( y < 54 )
                y += 5;
            end
            if ( y => 54 )
                y = 54;
                estado = 2;
            end
        end
        if ( fueramenu > 1 )
            y -= 15;
        end
        if ( y < -30 )
            signal( type bordederarriba, s_kill );
        end
        frame;
    end
end


PROCESS bordeizqabajo();
PRIVATE
    int Estado;
BEGIN
    x = 82;
    y = 473;
    graph = 17;
    z = -1;
    estado = 1;
    flags = 2;
    LOOP
        if ( estado == 1 )
            if ( y > 425 )
                y -= 5;
            end
            if ( y =< 425 )
                y = 425;
                estado = 2;
            end
        end
        if ( fueramenu > 1 )
            y += 15;
        end
        if ( y > 510 )
            signal( type bordeizqabajo, s_kill );
        end
        frame;
    end
end


PROCESS bordederabajo();
PRIVATE
    int Estado;
    int esperate;
BEGIN
    x = 555;
    y = 473;
    graph = 17;
    z = -1;
    estado = 1;
    flags = 3;
    esperate = 1;
    LOOP
        if ( estado == 1 )
            if ( y > 425 )
                y -= 5;
            end
            if ( y =< 425 )
                y = 425;
                estado = 2;
            end
        end
        if ( fueramenu > 1 )
            y += 15;
        end
        if ( y > 510 )
            signal( type bordederabajo, s_kill );
        end
        frame;
    end
end


PROCESS presionaenter();
PRIVATE
    int esperate;
    int pirate;
BEGIN
    x = 313;
    y = 379;
    graph = 18;
    angle = 10000;
    Pirate = 1;
    LOOP
        esperate++;
        if ( entrando != 2 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
            seleccionaopcion = 0;
            entrando = 2;
            sound_play( aceptar, 0, 16 );
        end
        if ( esperate == 21 )
            esperate = 0;
        end
        if ( esperate < 10 )
            angle -= 2000;
        end
        if ( esperate > 10 )
            angle += 2000;
        end
        if ( entrando == 2 )
            if ( pirate == 1 )
                if ( size < 220 )
                    size += 10;
                else
                    pirate = 2;
                end
            end
        end
        if ( pirate == 2 )
            if ( size > 10 )
                size -= 15;
            else
                sound_play( bienvenio, 0, 13 );
                trasladaopciones();
                trasladahistoria();
                saldeljuego();
                signal( type presionaenter, s_kill );
            end
        end
        frame;
    end
end

// mira la hora actual

PROCESS mirahora();
BEGIN
    ano = ftime( "%y", time());
    mes = ftime( "%m", time());
    dia = ftime( "%d", time());
    hora = ftime( "%k", time());
    min = ftime( "%M", time());
    seg = ftime( "%S", time());
    LOOP
        if ( timer[ 1 ] > 99 )
            timer[ 1 ] = 0;
            seg++;
        end
        if ( seg >= 60 )
            seg = 0;
            min++;
        end
        if ( min >= 60 )
            min = 0;
            hora++;
        end
        if ( hora >= 24 )
            hora = 0;
            dia++;
        end
        if ( dia >= 31 )
            dia = 0;
        end
        frame;
    end
end


PROCESS creadorjuego();
BEGIN
    x = 320;
    y = 455;
    graph = 117;
    alpha = 128;
    z = -1;
    LOOP
        if ( entrando == 2 )
            y += 10;
            if ( y > 700 )
                signal( type creadorjuego, s_kill );
            end
        end
        frame;
    end
end


PROCESS Joselpresentacion1();
PRIVATE
    int esperate;
    int accion;
    int joselcanta;
    int esperate2;
    int noboca;
    int otromas;
    int creatrucos;
    int keja;
BEGIN
    x = 437;
    y = 352;
    graph = 107;
    microfono1();
    brazo1();
    z = 3;
    joselcanta = sound_play( canta, 0, 13 );
    LOOP
        if ( graph != 116 )
            if ( key( _P ) AND creatrucos == 0 )
                creatrucos++;
            end
            if ( key( _L ) AND creatrucos == 1 )
                creatrucos++;
            end
            if ( key( _A ) AND creatrucos == 2 )
                creatrucos++;
            end
            if ( key( _N ) AND creatrucos == 3 )
                creatrucos++;
            end
            if ( key( _O ) AND creatrucos == 4 )
                creatrucos++;
            end
            if ( key( _L ) AND creatrucos == 5 )
                creatrucos++;
            end
            if ( modo_wiz == 1 )
                if ( pulsa_c == 2 && pulsa_esc == 2 )
                    creatrucos = 6;
                end
            end
        end
        if ( creatrucos == 6 )
            sound_play( aceptar, 0, 16 );
            compramenu = 4;
            autorecarga = 1;
            municion_arma2 = 9999;
            municion_arma3 = 9999;
            municion_arma4 = 9999;
            municion_arma5 = 9999;
            municion_arma6 = 9999;
            municion_arma7 = 9999;
            municion_arma8 = 9999;
            municion_arma9 = 9999;
            municion_arma10 = 9999;
            invencible = 1;
            seleccionaopcion = 0;
            entrando = 2;
            sound_stop( joselcanta );
            signal( type presionaenter, s_kill );
            signal( type brazo1, s_kill );
            signal( type presentacion, s_kill );
            menutrucos();
            accion = 132;
            creatrucos++;
            signal( id, s_kill );
        end
        if ( sound_is_playing( joselcanta ))
            esperate++;
        end
        if ( objetoencara == 0 AND ( NOT( sound_is_playing( joselcanta ))))
            noboca = 1;
            otromas = 1;
        end
        if ( otromas == 1 )
            accion++;
        end
        if (( pulsa_enter == 1 OR pulsa_c == 1 ) AND accion < 132 )
            sound_stop( joselcanta );
            accion = 132;
        end
        if ( objetoencara == 1 )
            if ( accion < 132 )
                graph = 153;
            end
            esperate2 = 8;
            objetoencara = 0;
        end
        if ( objetoencara < 3 AND esperate2 > 0 )
            esperate2--;
            if ( accion < 132 )
                graph = 153;
            end
        end
        if ( objetoencara < 3 AND esperate2 == 0 )
            if ( accion < 132 )
                graph = 107;
            end
        end
        if ( esperate2 == 6 )
            pegotesangre( rand( 394, 472 ), rand( 157, 339 ), rand( 63, 64 ), rand( 80, 130 ));
        end
        if ( esperate == 10 )
            esperate = 0;
        end
        if ( fueramenu == 2 )
            signal( id, s_kill );
        end
        if ( esperate < 5 AND noboca == 0 )
            if ( accion < 132 )
                graph = 107;
            end
        end
        if ( esperate > 5 AND noboca == 0 )
            if ( accion < 132 )
                graph = 110;
            end
        end
        if ( accion == 2 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 15 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 29 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 42 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 53 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 63 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 70 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 78 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 85 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 90 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 95 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 100 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 105 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 107 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 109 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 111 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 113 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 115 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 117 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 119 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 121 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 123 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 124 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 125 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 126 )
            objetojosel1( 181, 446, rand( 102, 105 ), 1 );
        end
        if ( accion == 127 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 128 )
            objetojosel1( 5, 310, rand( 102, 105 ), 5 );
        end
        if ( accion == 129 )
            objetojosel1( 591, 446, rand( 102, 105 ), 2 );
        end
        if ( accion == 132 )
            sound_play( crugida, 0 );
            signal( type pegotesangre, s_kill );
            cachocalne( x -10, y -20, 111, 1 );
            cachocalne( x -10, y -10, 112, 2 );
            cachocalne( x -10, y, 113, 3 );
            cachocalne( x, y, 64, 6 );
            cachocalne( x, y, 64, 7 );
            cachocalne( x, y, 64, 8 );
            cachocalne( x, y, 64, 9 );
            cachocalne( x + 10, y -20, 114, 4 );
            cachocalne( x + 10, y -10, 115, 5 );
            exploss( 428, 272 );
            accion++;
        end
        if ( accion => 133 )
            y = 466;
            graph = 116;
        end
        frame;
    end
end


PROCESS Microfono1();
BEGIN
    x = 267;
    y = 478;
    graph = 149;
    z = 2;
    LOOP
        if ( fueramenu > 1 )
            y += 10;
            if ( y => 520 )
                signal( id, s_kill );
            end
        end
        frame;
    end
end


PROCESS brazo1();
BEGIN
    x = 432;
    y = 310;
    graph = 154;
    z = 1;
    LOOP
        frame;
    end
end


PROCESS exploss( double x, double y );
BEGIN
    graph = 155;
    size = 350;
    luzjose();
    sound_play( explo1, 0, 22 );
    LOOP
        if ( graph == 189 )
            signal( type pegotesangre, s_kill );
            signal( type brazo1, s_kill );
            signal( id, s_kill );
        end
        if ( graph < 189 )
            graph++;
        end
        frame;
    end
end


PROCESS pegotesangre( double x, double y, int graph, double size );
BEGIN
    alpha = 128;
    LOOP
        frame;
    end
end


PROCESS objetojosel1( double x, double y, int graph, int destino );
PRIVATE
    int incr_y;
    int cana;
BEGIN
    LOOP
        if ( size > 40 )
            size -= 10;
        end
        if ( destino == 1 )
            y -= 30;
            x += 35;
            if ( collision( type brazo1 ))
                sound_stop( cana );
                cana = sound_play( dish, 0 );
                objetoencara = 1;
                destino = 3;
                sangre_cara( x, y, 63, rand( 70, 95 ), 1 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 2 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 3 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 4 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 5 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 6 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 7 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 8 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 9 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 10 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 11 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 12 );
            end
        end
        if ( destino == 2 )
            y -= 30;
            x -= 25;
            if ( collision( type brazo1 ))
                sound_stop( cana );
                cana = sound_play( dish, 0 );
                objetoencara = 1;
                destino = 4;
                sangre_cara( x, y, 63, rand( 70, 95 ), 1 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 2 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 3 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 4 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 5 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 6 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 13 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 14 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 15 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 16 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 17 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 18 );
            end
        end
        if ( destino == 3 )
            incr_y++;
            y += 8 + incr_y;
            x -= 20;
            if ( size < 100 )
                size += 5;
            end
            angle -= 5000;
        end
        if ( destino == 4 )
            incr_y++;
            y += 8 + incr_y;
            x += 20;
            if ( size < 100 )
                size += 5;
            end
            angle += 5000;
        end
        if ( y > 520 )
            signal( id, s_kill );
        end
        if ( destino == 5 )
            x += 45;
            if ( collision( type brazo1 ))
                sound_stop( cana );
                cana = sound_play( dish, 0 );
                objetoencara = 1;
                destino = 3;
                sangre_cara( x, y, 64, rand( 70, 95 ), 7 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 8 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 9 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 10 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 11 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 12 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 19 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 20 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 21 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 22 );
                sangre_cara( x, y, 63, rand( 70, 95 ), 23 );
                sangre_cara( x, y, 64, rand( 70, 95 ), 24 );
            end
        end
        frame;
    end
end


PROCESS sangre_cara( double x, double y, int graph, double size, int tipo );
PRIVATE
    double y_incr;
BEGIN
    z = -2;
    alpha = 128;
    if ( tipo =< 12 )
        y_incr = rand( -5, -20 );
    end
    if ( tipo => 13 )
        y_incr = rand( 5, 15 );
    end
    LOOP
        if ( region_out( id, 0 ))
            signal( id, s_kill );
        end
        if ( y_incr > 20 )
            y_incr = 15;
        end
        if ( tipo == 1 )
            x += 5;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 2 )
            x += 10;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 3 )
            x += 15;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 4 )
            x += 20;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 5 )
            x += 25;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 6 )
            x += 30;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 7 )
            x -= 5;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 8 )
            x -= 10;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 9 )
            x -= 15;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 10 )
            x -= 20;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 11 )
            x -= 25;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 12 )
            x -= 30;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 13 )
            x += 5;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 14 )
            x += 10;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 15 )
            x += 15;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 16 )
            x += 20;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 17 )
            x += 25;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 18 )
            x += 30;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 19 )
            x -= 5;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 20 )
            x -= 10;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 21 )
            x -= 15;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 22 )
            x -= 20;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 23 )
            x -= 25;
            y_incr += 2;
            y += y_incr;
        end
        if ( tipo == 24 )
            x -= 30;
            y_incr += 2;
            y += y_incr;
        end
        frame;
    end
end


PROCESS Joselpresentacion();
PRIVATE
    int recuerda_x;
    int balance;
    int estado_x = 1;
    int estado_y = 1;
    int paranoias;
    int estado;
    int quedigo;
    int cana;
    int estado2;
    int can;
    int tiempovida;
    int creatrucos;
BEGIN
    x = 510;
    y = 357;
    graph = 107;
    z = 1;
    LOOP
        if ( key( _P ) AND creatrucos == 0 )
            creatrucos++;
        end
        if ( key( _L ) AND creatrucos == 1 )
            creatrucos++;
        end
        if ( key( _A ) AND creatrucos == 2 )
            creatrucos++;
        end
        if ( key( _N ) AND creatrucos == 3 )
            creatrucos++;
        end
        if ( key( _O ) AND creatrucos == 4 )
            creatrucos++;
        end
        if ( key( _L ) AND creatrucos == 5 )
            creatrucos++;
        end
        if ( modo_wiz == 1 )
            if ( pulsa_c == 2 && pulsa_esc == 2 )
                creatrucos = 6;
            end
        end
        if ( creatrucos == 6 )
            sound_play( aceptar, 0, 16 );
            compramenu = 4;
            creatrucos++;
            autorecarga = 1;
            municion_arma2 = 9999;
            municion_arma3 = 9999;
            municion_arma4 = 9999;
            municion_arma5 = 9999;
            municion_arma6 = 9999;
            municion_arma7 = 9999;
            municion_arma8 = 9999;
            municion_arma9 = 9999;
            municion_arma10 = 9999;
            invencible = 1;
            seleccionaopcion = 0;
            entrando = 2;
            signal( type presionaenter, s_kill );
            signal( type presentacion, s_kill );
            menutrucos();
            signal( id, s_kill );
        end
        tiempovida++;
        recuerda_x = x;
        if ( tiempovida == 2600 )
            josexplota = 3;
            graph = 110;
            cana = sound_play( combo, 0 );
        end
        if ( tiempovida > 2600 AND tiempovida < 2650 )
            graph = 110;
        end
        if ( tiempovida == 2650 )
            josexplota = 1;
        end
        if ( josexplota == 1 AND estado2 == 0 )
            graph = 116;
            sound_play( crugida, 0 );
            cachocalne( x -10, y -20, 111, 1 );
            cachocalne( x -10, y -10, 112, 2 );
            cachocalne( x -10, y, 113, 3 );
            cachocalne( x, y, 64, 6 );
            cachocalne( x, y, 64, 7 );
            cachocalne( x, y, 64, 8 );
            cachocalne( x, y, 64, 9 );
            cachocalne( x + 10, y -20, 114, 4 );
            cachocalne( x + 10, y -10, 115, 5 );
            exploss( x, y );
            josexplota = 2;
        end
        if ( josexplota == 2 )
            graph = 116;
            y += 10;
        end
        if ( y > 700 )
            signal( type joselpresentacion, s_kill );
        end
        if ( x < -200 )
            signal( type joselpresentacion, s_kill );
        end
        if ( collision( type trasladaopciones ))
            estado2 = 1;
            sound_stop( can );
            can = sound_play( golpe13, 0 );
        end
        if ( estado == 0 AND estado2 == 0 AND josexplota == 0 )
            paranoias = rand( 1, 10 );
        end
        if ( paranoias == 5 )
            estado++;
        end
        if ( estado == 1 AND estado2 != 1 )
            graph = 110;
            quedigo = rand( 1, 8 );
            sound_stop( cana );
            if ( quedigo == 1 )
                cana = sound_play( kalauka, 0, 13 );
            end
            if ( quedigo == 2 )
                cana = sound_play( cunao, 0, 13 );
            end
            if ( quedigo == 3 )
                cana = sound_play( pantoja, 0, 13 );
            end
            if ( quedigo == 4 )
                cana = sound_play( triunfo, 0, 13 );
            end
            if ( quedigo == 5 )
                cana = sound_play( lesiden, 0, 13 );
            end
            if ( quedigo == 6 )
                cana = sound_play( golpe10, 0, 13 );
            end
            if ( quedigo == 7 )
                cana = sound_play( golpe6, 0, 13 );
            end
            if ( quedigo == 8 )
                cana = sound_play( erupto, 0 );
            end
        end
        if ( estado == 3 )
            graph = 107;
        end
        if ( estado == 6 )
            graph = 110;
        end
        if ( estado == 9 )
            graph = 107;
        end
        if ( estado == 12 )
            graph = 110;
        end
        if ( estado == 25 )
            graph = 107;
            estado = 0;
            paranoias = 0;
        end
        if ( y =< 357 )
            estado_y = 1;
        end
        if ( y => 370 )
            estado_y = 2;
        end
        if ( josexplota == 0 )
            if ( estado2 == 0 )
                if ( y < 370 AND estado_y == 1 )
                    y += 2;
                end
            end
            if ( estado2 == 0 )
                if ( y => 357 AND estado_y == 2 )
                    y -= 2;
                end
            end
            if ( estado2 == 0 )
                if ( x =< 0 )
                    x = 0;
                    estado_x = 2;
                    flags = 1;
                end
            end
            if ( estado2 == 0 )
                if ( x => 640 )
                    x = 640;
                    estado_x = 1;
                    flags = 0;
                end
            end
            if ( estado2 == 0 )
                if ( x > 0 AND estado_x == 1 )
                    x -= 3;
                end
            end
            if ( estado2 == 0 )
                if ( x < 640 AND estado_x == 2 )
                    x += 3;
                end
            end
        end
        if ( estado2 == 1 )
            graph = 108;
            if ( flags == 0 )
                angle += 1000;
            end
            if ( flags == 1 )
                angle -= 1000;
            end
            x -= 30;
        end
        frame;
    end
end


PROCESS cachocalne( double x, double y, int graph, int destino );
BEGIN
    LOOP
        if ( x < -200 )
            signal( id, s_kill );
        end
        if ( x > 800 )
            signal( id, s_kill );
        end
        if ( destino == 1 )
            x -= 25;
            y -= 25;
            angle += 9000;
        end
        if ( destino == 2 )
            x -= 35;
            angle += 12000;
        end
        if ( destino == 3 )
            x -= 25;
            y += 15;
            angle += 9000;
        end
        if ( destino == 4 )
            x += 25;
            y -= 25;
            angle -= 9000;
        end
        if ( destino == 5 )
            x += 35;
            angle -= 12000;
        end
        if ( destino == 6 )
            alpha = 128;
            x -= 20;
            y += 25;
        end
        if ( destino == 7 )
            alpha = 128;
            x += 20;
            y += 25;
        end
        if ( destino == 8 )
            alpha = 128;
            x -= 20;
            y -= 5;
        end
        if ( destino == 9 )
            alpha = 128;
            x += 20;
            y += 5;
        end
        frame;
    end
end


PROCESS muevefondo();
BEGIN
    scroll.camera = 0;
    scroll_start( 0, 0, rand( 53, 54 ), 0, 0, 0, 3 );
    controles();
    LOOP
        scroll.x0 += 2;
        scroll.y0 += 2;
        vida_total = 8;
        if ( cantidadificultad == 1 )
            vida_total = 15;
        end
        if ( cantidadificultad == 2 )
            vida_total = 12;
        end
        frame;
    end
end

// Proceso del menu de juego

PROCESS trasladaopciones();
BEGIN
    x = 873;
    y = 314;
    graph = 22;
    seleccionaopcion = 0;
    cambio_musica = 0;
    LOOP
        if ( x > 373 )
            x -= 40;
        end
        if ( x =< 373 )
            x = 373;
            iconopciones();
            signal( type trasladaopciones, s_kill );
        end
        frame;
    end
end


PROCESS iconopciones();
PRIVATE
    int estado2;
BEGIN
    x = 373;
    y = 314;
    graph = 22;
    seleccionaopcion = 0;
    estado2 = 2;
    LOOP
        if ( seleccionaopcion == 1 )
            x += 25;
            if ( x > 850 )
                signal( type iconopciones, s_kill );
            end
        end
        if ( seleccionaopcion == 2 )
            marcaopcion = 2;
            angle += 20000;
            y -= 15;
            if ( y < -70 )
                fueramenu = 0;
                menuopciones1();
                signal( type iconopciones, s_kill );
            end
        end
        if ( marcaopcion != 2 )
            if ( size < 100 )
                size += 8;
            else
                size -= 8;
            end
            if ( size > 91 && size < 109 )
                size = 100;
            end
        end
        if ( marcaopcion == 2 )
            if ( seleccionaopcion != 2 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                seleccionaopcion = 2;
                sound_play( aceptar, 0, 16 );
            end
            if ( seleccionaopcion != 2 )
                if ( estado2 == 2 )
                    if ( size < 130 )
                        size += 4;
                    else
                        estado2 = 3;
                    end
                end
                if ( estado2 == 3 )
                    if ( size > 100 )
                        size -= 4;
                    else
                        estado2 = 2;
                    end
                end
            end
        end
        frame;
    end
end


PROCESS trasladahistoria();
BEGIN
    x = 958;
    y = 152;
    graph = 20;
    seleccionaopcion = 0;
    marcaopcion = 1;
    LOOP
        if ( x > 258 )
            x -= 40;
        end
        if ( x =< 258 )
            x = 258;
            iconohistoria();
            signal( type trasladahistoria, s_kill );
        end
        frame;
    end
end


PROCESS iconohistoria();
PRIVATE
    int estado2;
BEGIN
    x = 258;
    y = 152;
    graph = 20;
    marcaopcion = 1;
    estado2 = 2;
    josexplota = 1;
    LOOP
        //si marco un numero seleccionare el nivel
        if ( seleccionaopcion == 2 )
            x -= 25;
            if ( x < -200 )
                signal( type iconohistoria, s_kill );
            end
        end
        if ( seleccionaopcion == 1 )
            marcaopcion = 1;
            angle += 20000;
            y -= 15;
            if ( y < -100 )
                if ( ke_nivel == 0 )
                    // mato y kallo a too , pongo nueva velocidad
                    let_me_alone();
                    clear_screen();
                    fade_off( 0 );
                    sound_stop( all_sound );
                    music_stop();
                    write_delete( all_text );
                    scroll_stop( 0 );
                    scroll.y0 = 0;
                    scroll.y1 = 0;
                    // descarga lo que pueda del menu
                    music_unload( sound2 );
                    fase1_desierto();
                    // fase2_jungla();
                    // fase2_b();
                    // fase3_nieve(); // VOLVER!
                    // final_juego();
                end
                fueramenu = 0;
                signal( id, s_kill );
            end
        end
        if ( marcaopcion != 1 )
            if ( size < 100 )
                size += 8;
            else
                size -= 8;
            end
            if ( size > 91 && size < 109 )
                size = 100;
            end
        end
        if ( marcaopcion == 1 )
            if ( seleccionaopcion != 1 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                seleccionaopcion = 1;
                sound_play( aceptar, 0, 16 ); // fade(0,0,0,5);
            end
            if ( seleccionaopcion != 1 )
                if ( estado2 == 2 )
                    if ( size < 130 )
                        size += 4;
                    else
                        estado2 = 3;
                    end
                end
                if ( estado2 == 3 )
                    if ( size > 100 )
                        size -= 4;
                    else
                        estado2 = 2;
                    end
                end
            end
        end
        frame;
    end
end


PROCESS titjuego();
PRIVATE // datos privados del proceso
    int estado2;
BEGIN
    graph = 62;
    x = 360;
    y = 1;
    LOOP
        if ( estado2 == 2 )
            if ( x < 360 )
                x += 4;
            else
                x = 360;
                estado2 = 3;
            end
        end
        if ( estado2 == 3 )
            x -= 4;
            if ( collision( type josecolmenu ))
                estado2 = 2;
            end
        end
        if ( seleccionaopcion == 1 OR seleccionaopcion == 2 OR seleccionaopcion == 3 )
            if ( y > -60 )
                y -= 4;
            end
            if ( y =< -60 )
                signal( type titjuego, s_kill );
            end
        else
            if ( y < 25 )
                y += 4;
            else
                if ( estado2 == 0 )
                    estado2 = 2;
                end
            end
        end
        frame;
    end
end


PROCESS josecolmenu();
PRIVATE // datos privados del proceso
    int estado2;
    int esperate;
BEGIN
    graph = 15;
    x = 192;
    y = 7;
    size = 50;
    esperate = 1;
    LOOP
        if ( esperate == 1 AND collision( type titjuego ))
            graph = 19;
            tortabrillo();
            if ( cantidadsangre == 1 )
                babajosel();
            end
            if ( cantidadsangre == 2 )
                babajosel();
                babajosel();
                babajosel();
            end
            if ( cantidadsangre == 3 )
                babajosel();
                babajosel();
                babajosel();
                babajosel();
                babajosel();
            end
            estado2 = 2;
            esperate = 5;
        end
        if ( esperate > 1 )
            esperate--;
        end
        if ( estado2 == 2 AND esperate > 1 )
            angle += 5000;
        end
        if ( esperate == 1 )
            graph = 15;
            angle = 0;
        end
        if ( seleccionaopcion == 1 OR seleccionaopcion == 2 OR seleccionaopcion == 3 )
            if ( y > -60 )
                y -= 4;
            end
            if ( y =< -60 )
                signal( type josecolmenu, s_kill );
            end
        else
            if ( y < 31 )
                y += 4;
            end
        end
        frame;
    end
end


PROCESS tortabrillo();
BEGIN
    x = rand( 205, 210 );
    y = rand( 24, 32 );
    graph = 65;
    size = 5;
    z = -1;
    alpha = 128;
    LOOP
        if ( size < 100 )
            size += 40;
        else
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS babajosel();
PRIVATE
    int inc_y;
    int esto_x;
    int gira;
BEGIN
    x = 167;
    y = 46;
    z = 2;
    graph = rand( 63, 64 );
    alpha = 128;
    size = rand( 40, 80 );
    esto_x = rand( 4, 15 );
    gira = rand( 1, 10 );
    LOOP
        x -= esto_x;
        inc_y++;
        if ( gira == 1 )
            angle += 5000;
        end
        if ( gira == 5 )
            angle -= 5000;
        end
        if ( y < 500 )
            y += 2 + inc_y;
        end
        if ( y => 500 )
            signal( id, s_kill );
        end
        frame;
    end
end


PROCESS saldeljuego();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int saliendo;
    int tiempo_salida;
BEGIN
    x = 308;
    y = 500;
    graph = 40;
    estado = 1;
    estado2 = 2;
    fueramenu = 0;
    josecolmenu();
    titjuego();
    LOOP
        if ( saliendo == 1 )
            tiempo_salida++;
        end
        if ( tiempo_salida > 15 )
            exit();
        end
        if ( marcaopcion > 3 )
            marcaopcion = 1;
        end
        if ( marcaopcion < 1 )
            marcaopcion = 3;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            marcaopcion--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            marcaopcion++;
        end
        if ( estado == 1 )
            if ( y > 460 )
                y -= 5;
            end
            if ( y =< 460 )
                y = 460;
                estado = 2;
                estado2 = 2;
            end
        end
        if ( seleccionaopcion == 1 OR seleccionaopcion == 2 OR seleccionaopcion == 3 )
            y += 2;
            if ( y > 500 )
                signal( type saldeljuego, s_kill );
            end
        end
        if ( marcaopcion != 3 )
            size = 100;
        end
        if ( marcaopcion == 3 )
            if (( pulsa_enter == 1 OR pulsa_c == 1 ) AND seleccionaopcion != 4 )
                altura_salto = -19;
                vidaenemigo = 0;
                incre_m1 = 120;
                incre_m2 = 120;
                fuecargada = 0;
                seleccionaopcion = 4;
                sound_stop( all_sound );
                music_stop(); // fade(0,0,0,5);
                saliendo = 1;
            end
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 4;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 4;
                else
                    estado2 = 2;
                end
            end
        end
        frame;
    end
end

// Fin del proceso de menu del juego
// MENU OPCIONES !!

PROCESS menuopciones();
PRIVATE
    int Estado;
BEGIN
    x = 310;
    y = 251;
    graph = 26;
    size = 10;
    alpha = 128;
    estado = 1;
    selectdif = 1;
    selectsang = 1;
    seleccionaopcion2 = 0;
    marcaopcion2 = 1;
    LOOP
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type menuopciones, s_kill );
            end
        end
        if ( opseleccionada > 5 )
            opseleccionada = 1;
        end
        if ( opseleccionada < 1 )
            opseleccionada = 5;
        end
        if ( selectdif > 3 )
            selectdif = 1;
        end
        if ( selectdif < 1 )
            selectdif = 3;
        end
        if ( selectsang > 3 )
            selectsang = 1;
        end
        if ( selectsang < 1 )
            selectsang = 3;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectdif--;
        end
        if ( pulsa_der == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectdif++;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectsang--;
        end
        if ( pulsa_der == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectsang++;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada++;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 10;
            else
                size = 100;
                if ( compramenu == 2 )
                    cuadropciones();
                    salcuadropciones();
                    signal( type menuopciones, s_kill );
                end
                if ( compramenu == 4 )
                    cuadropciones();
                    cuadrotrucos();
                    salcuadropciones();
                    signal( type menuopciones, s_kill );
                end
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS cuadrotrucos();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 59;
    x = 373;
    y = 364;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( seleccionaopcion2 == 1 )
            x += 25;
            if ( x > 850 )
                signal( type cuadrotrucos, s_kill );
            end
        end
        if ( seleccionaopcion2 == 2 )
            x += 25;
            if ( x > 850 )
                signal( type cuadrotrucos, s_kill );
            end
        end
        if ( seleccionaopcion2 == 3 )
            marcaopcion2 = 3;
            angle += 20000;
            y -= 15;
            if ( y < -70 )
                menutrucos();
                fueramenu = 0;
                signal( type cuadrotrucos, s_kill );
            end
        end
        if ( seleccionaopcion2 == 4 )
            x += 25;
            if ( x > 850 )
                signal( type cuadrotrucos, s_kill );
            end
        end
        if ( marcaopcion2 != 3 )
            if ( estado2 == 2 OR estado2 == 3 )
                size = 100;
            end
        end
        if ( marcaopcion2 == 3 )
            if ( seleccionaopcion2 != 3 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                seleccionaopcion2 = 3;
                sound_play( aceptar, 0, 16 );
            end
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 4;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 4;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end

// Menu de los trucos

PROCESS menutrucos();
PRIVATE
    int Estado;
BEGIN
    x = 310;
    y = 251;
    graph = 26;
    size = 10;
    alpha = 128;
    estado = 1;
    fueramenu = 0;
    opseleccionada = 1;
    seleccionaopcion2 = 0;
    marcaopcion2 = 1;
    LOOP
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type menutrucos, s_kill );
            end
        end
        if ( opseleccionada > 5 )
            opseleccionada = 1;
        end
        if ( opseleccionada < 1 )
            opseleccionada = 5;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectinvencible = 1;
        end
        if ( pulsa_der == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectinvencible = 2;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectrecarga = 1;
        end
        if ( pulsa_der == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectrecarga = 2;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada++;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 10;
            else
                size = 100;
                barravolumen();
                barravolumenfx();
                titrucos();
                icoinvencible();
                icorecarga();
                icoreaccion();
                icovida();
                icosaltrucos();
                ruedareaccion();
                ruedavida();
                sinvencible();
                noinvencible();
                sirecarga();
                norecarga();
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS titrucos();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 61;
    x = 308;
    y = 37;
    size = 5;
    estado = 1;
    LOOP
        if ( fueramenu == 1 )
            if ( y > -60 )
                y -= 10;
            else
                signal( type titrucos, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
                sound_play( poing, 0 );
                sound_play( poing, 0 );
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
            end
        end
        frame;
    end
end


PROCESS icoinvencible();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 75;
    x = 200;
    y = 94;
    size = 5;
    estado = 1;
    opseleccionada = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x < 860 )
                x += 30;
                angle += 15000;
            end
            if ( x => 860 )
                trasladaopciones();
                trasladahistoria();
                saldeljuego();
                signal( type icoinvencible, s_kill );
            end
        end
        if ( opseleccionada != 1 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 204 )
                x -= 20;
            end
            if ( x =< 204 )
                x = 204;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 1 AND fueramenu == 0 )
            if ( x < 315 )
                x += 10;
            end
            if ( x => 315 )
                x = 315;
            end
        end
        if ( x == 315 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS sinvencible();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 237;
    y = 140;
    graph = 97;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( invencible == 1 )
            graph = 95;
        end
        if ( invencible != 1 )
            graph = 97;
        end
        if ( opseleccionada == 1 AND selectinvencible == 1 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                invencible = 1;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type sinvencible, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS noinvencible();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 383;
    y = 140;
    graph = 98;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( invencible == 2 )
            graph = 96;
        end
        if ( invencible != 2 )
            graph = 98;
        end
        if ( opseleccionada == 1 AND selectinvencible == 2 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                invencible = 2;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type noinvencible, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icorecarga();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 76;
    x = 170;
    y = 184;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x > -200 )
                x -= 30;
                angle -= 15000;
            end
            if ( x =< -200 )
                signal( type icorecarga, s_kill );
            end
        end
        if ( opseleccionada != 2 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 309 )
                x -= 20;
            else
                x = 309;
            end
        end
        if ( opseleccionada == 2 AND x == 309 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS sirecarga();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 237;
    y = 227;
    graph = 97;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( autorecarga == 1 )
            graph = 95;
        end
        if ( autorecarga != 1 )
            graph = 97;
        end
        if ( opseleccionada == 2 AND selectrecarga == 1 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                autorecarga = 1;
                municion_arma2 = 9999;
                municion_arma3 = 9999;
                municion_arma4 = 9999;
                municion_arma5 = 9999;
                municion_arma6 = 9999;
                municion_arma7 = 9999;
                municion_arma8 = 9999;
                municion_arma9 = 9999;
                municion_arma10 = 9999;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type sirecarga, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS norecarga();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 383;
    y = 227;
    graph = 98;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( autorecarga == 2 )
            graph = 96;
        end
        if ( autorecarga != 2 )
            graph = 98;
        end
        if ( opseleccionada == 2 AND selectrecarga == 2 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                municion_arma2 = 0;
                municion_arma3 = 0;
                municion_arma4 = 0;
                municion_arma5 = 0;
                municion_arma6 = 0;
                municion_arma7 = 0;
                municion_arma8 = 0;
                municion_arma9 = 0;
                municion_arma10 = 0;
                autorecarga = 2;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type norecarga, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icoreaccion();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 77;
    x = 180;
    y = 272;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x < 860 )
                x += 40;
                angle += 15000;
            end
            if ( x => 860 )
                signal( type icoreaccion, s_kill );
            end
        end
        if ( opseleccionada != 3 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 309 )
                x -= 20;
            else
                x = 309;
            end
        end
        if ( opseleccionada == 3 AND x == 309 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS ruedareaccion();
BEGIN
    x = incre_m1;
    y = 311;
    graph = 32;
    z = -2;
    signal( type ruedavolumen, s_kill );
    signal( type ruedavolumenfx, s_kill );
    LOOP
        incre_m1 = x;
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type ruedareaccion, s_kill );
            end
        end
        if ( x > 501 )
            x = 501;
            angle = 0;
        end
        if ( x < 120 )
            x = 120;
            angle = 0;
        end
        if ( opseleccionada == 3 )
            if ( pulsa_izq > 0 )
                x -= 5;
                angle += 25000;
            end
            if ( pulsa_der > 0 )
                x += 5;
                angle -= 25000;
            end
        end
        if ( opseleccionada != 3 )
            if ( size > 100 )
                size -= 20;
            end
        end
        if ( opseleccionada == 3 )
            if ( size < 116 )
                size += 20;
            end
        end
        if ( x > 120 AND x < 159 )
            altura_salto = -19;
        end
        if ( x > 160 AND x < 199 )
            altura_salto = -20;
        end
        if ( x > 200 AND x < 239 )
            altura_salto = -20;
        end
        if ( x > 240 AND x < 279 )
            altura_salto = -21;
        end
        if ( x > 280 AND x < 319 )
            altura_salto = -22;
        end
        if ( x > 320 AND x < 359 )
            altura_salto = -23;
        end
        if ( x > 360 AND x < 399 )
            altura_salto = -24;
        end
        if ( x > 400 AND x < 439 )
            altura_salto = -25;
        end
        if ( x > 440 AND x < 479 )
            altura_salto = -26;
        end
        if ( x > 480 AND x < 501 )
            altura_salto = -28;
        end
        frame;
    end
end


PROCESS icovida();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 78;
    x = 215;
    y = 353;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x > -200 )
                x -= 30;
                angle -= 15000;
            end
            if ( x =< -200 )
                signal( type icovida, s_kill );
            end
        end
        if ( opseleccionada != 4 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 269 )
                x -= 20;
            else
                x = 269;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 4 AND fueramenu == 0 )
            if ( x < 309 )
                x += 10;
            end
            if ( x => 309 )
                x = 309;
            end
        end
        if ( x == 309 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS ruedavida();
BEGIN
    x = incre_m2;
    y = 402;
    graph = 32;
    z = -2;
    LOOP
        incre_m2 = x;
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type ruedavida, s_kill );
            end
        end
        if ( x > 501 )
            x = 501;
            angle = 0;
        end
        if ( x < 120 )
            x = 120;
            angle = 0;
        end
        if ( opseleccionada == 4 )
            if ( pulsa_izq > 0 )
                x -= 5;
                angle += 25000;
            end
            if ( pulsa_der > 0 )
                x += 5;
                angle -= 25000;
            end
        end
        if ( opseleccionada != 4 )
            if ( size > 100 )
                size -= 20;
            end
        end
        if ( opseleccionada == 4 )
            if ( size < 116 )
                size += 20;
            end
        end
        if ( x > 120 AND x < 159 )
            vidaenemigo = 0;
        end
        if ( x > 160 AND x < 199 )
            vidaenemigo = 1;
        end
        if ( x > 200 AND x < 239 )
            vidaenemigo = 2;
        end
        if ( x > 240 AND x < 279 )
            vidaenemigo = 3;
        end
        if ( x > 280 AND x < 319 )
            vidaenemigo = 4;
        end
        if ( x > 320 AND x < 359 )
            vidaenemigo = 5;
        end
        if ( x > 360 AND x < 399 )
            vidaenemigo = 6;
        end
        if ( x > 400 AND x < 439 )
            vidaenemigo = 7;
        end
        if ( x > 440 AND x < 479 )
            vidaenemigo = 8;
        end
        if ( x > 480 AND x < 501 )
            vidaenemigo = 9;
        end
        frame;
    end
end


PROCESS icosaltrucos();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 31;
    x = 308;
    y = 460;
    size = 5;
    estado = 1;
    LOOP
        if ( opseleccionada == 5 )
            if ( fueramenu != 1 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                fueramenu = 1;
                sound_play( aceptar, 0, 16 );
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icosaltrucos, s_kill );
            end
        end
        if ( opseleccionada != 5 AND estado2 == 2 )
            if ( size > 100 )
                size -= 4;
            else
                size = 100;
            end
        end
        if ( opseleccionada == 5 )
            if ( estado2 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end

// fin del menu de los trucos

PROCESS salcuadropciones();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 79;
    x = 308;
    y = 460;
    size = 5;
    estado = 1;
    LOOP
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
                sound_play( poing, 0 );
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        if ( seleccionaopcion2 == 1 OR seleccionaopcion2 == 2 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type salcuadropciones, s_kill );
            end
        end
        if ( seleccionaopcion2 == 3 OR seleccionaopcion2 == 4 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type salcuadropciones, s_kill );
            end
        end
        if ( seleccionaopcion2 == 0 AND marcaopcion2 != 4 AND estado2 == 2 )
            if ( size > 100 )
                size -= 4;
            else
                size = 100;
            end
        end
        if ( marcaopcion2 == 4 AND seleccionaopcion2 == 0 )
            if ( seleccionaopcion2 != 4 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                seleccionaopcion2 = 4;
                sound_play( aceptar, 0, 16 );
            end
            if ( estado2 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        frame;
    end
end


PROCESS menuopciones1();
PRIVATE
    int Estado;
BEGIN
    x = 310;
    y = 251;
    graph = 26;
    size = 10;
    alpha = 128;
    estado = 1;
    selectdif = 1;
    selectsang = 1;
    seleccionaopcion2 = 0;
    marcaopcion2 = 1;
    signal( type salcuadropciones, s_kill );
    LOOP
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type menuopciones1, s_kill );
            end
        end
        if ( opseleccionada > 5 )
            opseleccionada = 1;
        end
        if ( opseleccionada < 1 )
            opseleccionada = 5;
        end
        if ( selectdif > 3 )
            selectdif = 1;
        end
        if ( selectdif < 1 )
            selectdif = 3;
        end
        if ( selectsang > 3 )
            selectsang = 1;
        end
        if ( selectsang < 1 )
            selectsang = 3;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectdif--;
        end
        if ( pulsa_der == 1 AND opseleccionada == 1 )
            sound_play( decide, 0, 16 );
            selectdif++;
        end
        if ( pulsa_izq == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectsang--;
        end
        if ( pulsa_der == 1 AND opseleccionada == 2 )
            sound_play( decide, 0, 16 );
            selectsang++;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada--;
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            opseleccionada++;
        end
        if ( estado == 1 )
            if ( size < 100 )
                size += 10;
            else
                size = 100;
                icodificultad();
                icosangre();
                icovolumen();
                icovolumenfx();
                icosalopciones();
                icopciones();
                icomax();
                iconOR ();
                icomin();
                icosangremax();
                icosangrenOR ();
                icosangremin();
                estado = 2;
            end
        end
        frame;
    end
end


PROCESS cuadropciones();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int esperate;
BEGIN
    graph = 22;
    x = 312;
    y = 234;
    size = 5;
    estado = 1;
    z = -1;
    esperate = 1;
    if ( compramenu == 2 )
        marcaopcion2 = 1;
    end
    if ( compramenu == 3 )
        marcaopcion2 = 2;
    end
    if ( compramenu == 4 )
        marcaopcion2 = 1;
    end
    LOOP
        if ( esperate > 1 )
            esperate--;
        end
        if ( marcaopcion2 > 4 )
            if ( compramenu != 3 )
                marcaopcion2 = 1;
            end
            if ( compramenu == 3 )
                marcaopcion2 = 2;
            end
        end
        if ( marcaopcion2 < 1 )
            marcaopcion2 = 4;
        end
        if ( pulsa_arriba == 1 )
            sound_play( decide, 0, 16 );
            marcaopcion2--;
            if (( compramenu == 2 ) AND ( marcaopcion2 == 3 ))
                marcaopcion2--;
            end
            if ( compramenu == 3 AND marcaopcion2 == 1 )
                marcaopcion2--;
            end
        end
        if ( pulsa_abajo == 1 )
            sound_play( decide, 0, 16 );
            marcaopcion2++;
            if (( compramenu == 3 ) AND ( marcaopcion2 == 1 ))
                marcaopcion2 = 2;
            end
            if ( compramenu == 2 AND marcaopcion2 == 3 )
                marcaopcion2++;
            end
        end
        if ( seleccionaopcion2 == 1 )
            x -= 25;
            if ( x < -200 )
                signal( type cuadropciones, s_kill );
            end
        end
        if ( seleccionaopcion2 == 2 )
            marcaopcion2 = 2;
            angle += 20000;
            y -= 15;
            if ( y < -100 )
                fueramenu = 0;
                menuopciones1();
                signal( type cuadropciones, s_kill );
            end
        end
        if ( seleccionaopcion2 == 3 )
            x += 25;
            if ( x > 850 )
                signal( type cuadropciones, s_kill );
            end
        end
        if ( seleccionaopcion2 == 4 )
            marcaopcion2 = 4;
            x += 25;
            if ( x > 850 )
                trasladaopciones();
                trasladahistoria();
                saldeljuego();
                signal( type cuadropciones, s_kill );
            end
        end
        if ( marcaopcion2 != 2 )
            if ( estado2 == 2 OR estado2 == 3 )
                size = 100;
            end
        end
        if ( marcaopcion2 == 2 )
            if ( seleccionaopcion2 != 2 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                seleccionaopcion2 = 2;
                sound_play( aceptar, 0, 16 );
            end
            if ( estado2 == 2 )
                if ( size < 130 )
                    size += 4;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 4;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end


PROCESS icopciones();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 25;
    x = 308;
    y = 37;
    size = 5;
    estado = 1;
    LOOP
        if ( fueramenu == 1 )
            if ( y > -60 )
                y -= 10;
            else
                signal( type icopciones, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
                sound_play( poing, 0 );
                sound_play( poing, 0 );
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
            end
        end
        frame;
    end
end


PROCESS icodificultad();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 27;
    x = 200;
    y = 94;
    size = 5;
    estado = 1;
    opseleccionada = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x < 860 )
                x += 30;
                angle += 15000;
            end
            if ( x => 860 )
                trasladaopciones();
                trasladahistoria();
                saldeljuego();
                signal( type icodificultad, s_kill );
            end
        end
        if ( opseleccionada != 1 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 200 )
                x -= 20;
            end
            if ( x < 200 )
                x = 200;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 1 AND fueramenu == 0 )
            if ( x < 324 )
                x += 10;
            end
            if ( x => 324 )
                x = 324;
            end
        end
        if ( x == 324 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icomax();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 455;
    y = 140;
    graph = 34;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadificultad == 3 )
            graph = 37;
        end
        if ( cantidadificultad != 3 )
            graph = 34;
        end
        if ( opseleccionada == 1 AND selectdif == 3 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadificultad = 3;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icomax, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS iconOR ();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 320;
    y = 140;
    graph = 35;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadificultad == 2 )
            graph = 38;
        end
        if ( cantidadificultad != 2 )
            graph = 35;
        end
        if ( opseleccionada == 1 AND selectdif == 2 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadificultad = 2;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type iconor, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icomin();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 181;
    y = 140;
    graph = 36;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadificultad == 1 )
            graph = 39;
        end
        if ( cantidadificultad != 1 )
            graph = 36;
        end
        if ( opseleccionada == 1 AND selectdif == 1 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadificultad = 1;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icomin, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icosangre();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 28;
    x = 170;
    y = 184;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x > -200 )
                x -= 30;
                angle -= 15000;
            end
            if ( x =< -200 )
                signal( type icosangre, s_kill );
            end
        end
        if ( opseleccionada != 2 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 185 )
                x -= 20;
            end
            if ( x < 185 )
                x = 185;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 2 AND fueramenu == 0 )
            if ( x < 315 )
                x += 10;
            end
            if ( x => 315 )
                x = 315;
            end
        end
        if ( x == 315 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icosangremax();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 455;
    y = 227;
    graph = 34;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadsangre == 3 )
            graph = 37;
        end
        if ( cantidadsangre != 3 )
            graph = 34;
        end
        if ( opseleccionada == 2 AND selectsang == 3 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadsangre = 3;
                velocidad_juego = 38;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icosangremax, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icosangrenOR ();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 320;
    y = 227;
    graph = 35;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadsangre == 2 )
            graph = 38;
        end
        if ( cantidadsangre != 2 )
            graph = 35;
        end
        if ( opseleccionada == 2 AND selectsang == 2 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadsangre = 2;
                velocidad_juego = 36;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icosangrenor, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icosangremin();
PRIVATE
    int estado;
    int estado3;
BEGIN
    x = 181;
    y = 227;
    graph = 36;
    z = -1;
    size = 5;
    estado = 1;
    LOOP
        if ( cantidadsangre == 1 )
            graph = 39;
        end
        if ( cantidadsangre != 1 )
            graph = 36;
        end
        if ( opseleccionada == 2 AND selectsang == 1 AND fueramenu == 0 )
            if ( pulsa_enter == 1 OR pulsa_c == 1 )
                cantidadsangre = 1;
                velocidad_juego = 32;
            end
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icosangremin, s_kill );
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado3 = 2;
            end
        end
        frame;
    end
end


PROCESS icovolumen();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 29;
    x = 180;
    y = 272;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x < 860 )
                x += 40;
                angle += 15000;
            end
            if ( x => 860 )
                signal( type icovolumen, s_kill );
            end
        end
        if ( opseleccionada != 3 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 180 )
                x -= 20;
            end
            if ( x < 180 )
                x = 180;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 3 AND fueramenu == 0 )
            if ( x < 318 )
                x += 10;
            else
                x = 318;
            end
        end
        if ( x == 318 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
                barravolumen();
                ruedavolumen();
            end
        end
        frame;
    end
end


PROCESS Barravolumen();
BEGIN
    x = 310;
    y = 316;
    graph = 33;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type barravolumen, s_kill );
            end
        end
        frame;
    end
end


PROCESS ruedavolumen();
BEGIN
    x = guarda_x1;
    y = 311;
    graph = 32;
    z = -2;
    LOOP
        guarda_x1 = x;
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type ruedavolumen, s_kill );
            end
        end
        if ( x > 501 )
            x = 501;
            angle = 0;
        end
        if ( x < 120 )
            x = 120;
            angle = 0;
        end
        if ( opseleccionada == 3 )
            if ( pulsa_izq > 0 )
                x -= 5;
                angle += 25000;
            end
            if ( pulsa_der > 0 )
                x += 5;
                angle -= 25000;
            end
        end
        if ( opseleccionada != 3 )
            if ( size > 100 )
                size -= 20;
            end
        end
        if ( opseleccionada == 3 )
            if ( size < 116 )
                size += 20;
            end
        end
        if ( x > 120 AND x < 159 )
            vol = 0;
        end
        if ( x > 160 AND x < 199 )
            vol = 12;
        end
        if ( x > 200 AND x < 239 )
            vol = 24;
        end
        if ( x > 240 AND x < 279 )
            vol = 36;
        end
        if ( x > 280 AND x < 319 )
            vol = 48;
        end
        if ( x > 320 AND x < 359 )
            vol = 60;
        end
        if ( x > 360 AND x < 399 )
            vol = 72;
        end
        if ( x > 400 AND x < 439 )
            vol = 84;
        end
        if ( x > 440 AND x < 479 )
            vol = 96;
        end
        if ( x > 480 AND x < 501 )
            vol = 128;
        end
        music_set_volume( vol );
        frame;
    end
end


PROCESS icovolumenfx();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
    int estado3;
BEGIN
    graph = 30;
    x = 215;
    y = 353;
    size = 5;
    estado = 1;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( x > -200 )
                x -= 30;
                angle -= 15000;
            end
            if ( x =< -200 )
                signal( type icovolumenfx, s_kill );
            end
        end
        if ( opseleccionada != 4 AND fueramenu == 0 )
            if ( estado == 6 )
                size = 100;
            end
            if ( x > 215 )
                x -= 20;
            end
            if ( x < 215 )
                x = 215;
            end
        end
        if ( estado2 == 1 AND opseleccionada == 4 AND fueramenu == 0 )
            if ( x < 318 )
                x += 10;
            end
            if ( x > 318 )
                x = 318;
            end
        end
        if ( x == 318 AND fueramenu == 0 )
            if ( estado3 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado3 = 3;
                end
            end
            if ( estado3 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado3 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 1;
                estado3 = 2;
                barravolumenfx();
                ruedavolumenfx();
            end
        end
        frame;
    end
end


PROCESS Barravolumenfx();
BEGIN
    x = 310;
    y = 407;
    graph = 33;
    z = -1;
    LOOP
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type barravolumenfx, s_kill );
            end
        end
        frame;
    end
end


PROCESS ruedavolumenfx();
BEGIN
    x = guarda_x2;
    y = 402;
    graph = 32;
    z = -2;
    LOOP
        guarda_x2 = x;
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type ruedavolumenfx, s_kill );
            end
        end
        if ( x > 501 )
            x = 501;
            angle = 0;
        end
        if ( x < 120 )
            x = 120;
            angle = 0;
        end
        if ( opseleccionada == 4 )
            if ( pulsa_izq > 0 )
                x -= 5;
                angle += 25000;
            end
            if ( pulsa_der > 0 )
                x += 5;
                angle -= 25000;
            end
        end
        if ( opseleccionada != 4 )
            if ( size > 100 )
                size -= 20;
            end
        end
        if ( opseleccionada == 4 )
            if ( size < 116 )
                size += 20;
            end
        end
        if ( x > 120 AND x < 159 )
            volfx = 0;
        end
        if ( x > 160 AND x < 199 )
            volfx = 12;
        end
        if ( x > 200 AND x < 239 )
            volfx = 24;
        end
        if ( x > 240 AND x < 279 )
            volfx = 36;
        end
        if ( x > 280 AND x < 319 )
            volfx = 48;
        end
        if ( x > 320 AND x < 359 )
            volfx = 60;
        end
        if ( x > 360 AND x < 399 )
            volfx = 72;
        end
        if ( x > 400 AND x < 439 )
            volfx = 84;
        end
        if ( x > 440 AND x < 479 )
            volfx = 96;
        end
        if ( x > 480 AND x < 501 )
            volfx = 128;
        end
        channel_set_volume( -1, volfx );
        frame;
    end
end


PROCESS icosalopciones();
PRIVATE // datos privados del proceso
    int estado;
    int estado2;
BEGIN
    graph = 31;
    x = 308;
    y = 460;
    size = 5;
    estado = 1;
    LOOP
        if ( opseleccionada == 5 )
            if ( fueramenu != 1 AND ( pulsa_enter == 1 OR pulsa_c == 1 ))
                fueramenu = 1;
                sound_play( aceptar, 0, 16 );
            end
        end
        if ( fueramenu == 1 )
            if ( size > 10 )
                size -= 10;
            else
                signal( type icosalopciones, s_kill );
            end
        end
        if ( opseleccionada != 5 AND estado2 == 2 )
            if ( size > 100 )
                size -= 4;
            else
                size = 100;
            end
        end
        if ( opseleccionada == 5 )
            if ( estado2 == 2 )
                if ( size < 110 )
                    size += 2;
                else
                    estado2 = 3;
                end
            end
            if ( estado2 == 3 )
                if ( size > 100 )
                    size -= 2;
                else
                    estado2 = 2;
                end
            end
        end
        if ( estado == 1 )
            if ( size < 102 )
                size += 32;
            else
                estado = 2;
            end
        end
        if ( estado == 2 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 3;
            end
        end
        if ( estado == 3 )
            if ( size < 102 )
                size += 64;
            else
                estado = 4;
            end
        end
        if ( estado == 4 )
            if ( size > 30 )
                size -= 48;
            else
                estado = 5;
            end
        end
        if ( estado == 5 )
            if ( size < 102 )
                size += 32;
            else
                size = 100;
                estado = 6;
                estado2 = 2;
            end
        end
        frame;
    end
end

// fin del menu de opciones
// FIN DEL JUE
