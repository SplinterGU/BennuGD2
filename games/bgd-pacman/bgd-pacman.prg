//-------------------------------------------------------------------
// PROYECTO: Classic Pack Games For Bennu.
// AUTORES: DCelso, TYCO, Kim-ele-to, PiXeL, PRG
// FECHA INICIO: 07/11/2008.
// FECHA FINALIZACION: xx/xx/xxxx.
// REVISION: 13/03/2009.
// VERSION PARA: Bennu WIP-15c2 y superiores.
//-------------------------------------------------------------------

PROGRAM apagame4be; // clAssic PAck GAMEs for BEnnugd

import "mod_gfx";
import "mod_input";
import "mod_misc";
import "mod_sound";

#include "jkeys.lib"

CONST
    VERSION = "0.4";

    SCREEN_WIDTH = 640;
    SCREEN_HEIGHT = 480;
    FPS_GAME = 30;
//    FPS_GAME = 0;

//******************************************************************************************************
// CODIGO INICIAL DEL PROGRAMA
//******************************************************************************************************

BEGIN
//    screen.scale_resolution = 03200240;
    PACMAN_main();
END


function put(int file, graph, double x, y, int angle = 0, double size = 100, int flags = B_NOCOLORKEY )
begin
    map_put( background.file, background.graph, file, graph, x, y, angle, size, size, flags, 255, 255, 255, 255, 0 );
end

function get_index_color( int color )
begin
    switch( color )
        CASE 0xfcfc54ff/*14*/:
            return 14;
        END
        CASE 0x54fc54ff/*10*/:
            return 10;
        END
        CASE 0xfc5454ff/*12*/:
            return 12;
        END
        CASE 0x5454fcff/*9*/: 
            return 9;
        END
        CASE 0x54fcfcff/*11*/:
            return 11;
        END
        CASE 0xff,0x0:
            return 16;
        END
    end
    return 0;
end

//******************************************************************************************************

DECLARE PROCESS PACMAN_fantasma(x, y, tipo)
    Public
        int estado=0;    // Estado de fantasma (0=normal >0 Contador de modo panico)
        int modelo;      // Tipo de grafico de fantasma
    End
END

GLOBAL
    int PACMAN_fpg;                    // ID del FPG
    int PACMAN_fnt;                    // ID de la FNT

    int PACMAN_snd_inicio;             // IDs de Sonidos
    int PACMAN_snd_come_fantasma;
    int PACMAN_snd_come_fruta;
    int PACMAN_snd_come_punto;
    int PACMAN_snd_come_puntogr;
    int PACMAN_snd_empieza;
    int PACMAN_snd_muerte;

    int PACMAN_puntuacion_antigua=0;   // Indicador de la ultima puntuacion
    int PACMAN_puntuacion=0;           // Puntuacion del jugador
    int PACMAN_nivel;                  // Nivel actual
    int PACMAN_puntos=0;               // Puntos comidos (cocos)

    int PACMAN_vidas[10];              // Identificadores para las vidas del marcador
    int PACMAN_ividas=0;               // Contador de vidas en el marcador
    int PACMAN_puntuacion_max=10000;   // Maxima puntuacion por defecto
    int PACMAN_idpacman;               // Identificador para pacman

    int i,w,h,d,gr,gr2,p1,p2;

PROCESS PACMAN_main()
BEGIN

    // RESETEAMOS LA SEMILLA DE RAND PARA VARIAR LA SENSACION DE ALEATORIO
    rand_seed(get_timer()+(time()%1000)*1000);

    // SETEAMOS LAS TECLAS POR DEFECTO DE LOS JUGADORES
    jkeys_set_default_keys();

    set_mode(SCREEN_WIDTH, SCREEN_HEIGHT);     // Selecciona modo de video
    set_fps(FPS_GAME,0);

    // screen emulation
    background.graph = map_new(SCREEN_WIDTH,SCREEN_HEIGHT);

    // Carga sonidos
    PACMAN_snd_come_fantasma=sound_load("sfx/comefant.wav");
    PACMAN_snd_come_fruta=sound_load("sfx/comefrut.wav");
    PACMAN_snd_come_punto=sound_load("sfx/comecoco.wav");
    PACMAN_snd_come_puntogr=sound_load("sfx/comegcoc.wav");
    PACMAN_snd_empieza=sound_load("sfx/comienzo.wav");
    PACMAN_snd_muerte=sound_load("sfx/muerto.wav");
    PACMAN_snd_inicio=sound_load("sfx/tambor2.wav");

    // Carga la maxima puntuacion
    load(get_pref_path("bennugd.org", "pacman") + "pacman.dat",PACMAN_puntuacion_max);

    // Define la region 1 que sera la que se use como de impresion
    region_define(1,105,0,(640-209),480);

    PACMAN_fpg=fpg_load("gfx/pacman.fpg"); // Carga los graficos
    PACMAN_fnt=fnt_load("fnt/pacman.fnt"); // Carga la fuente
    glyph_set(PACMAN_fnt,32,0,map_new(12,8)); // Fix char 32 width
//    load_pal("dat\pacman\pacman.fpg"); // Carga la paleta de 8bits
//    pal_load("gfx/pacman.fpg"); // Carga la paleta de 8bits

    frame;
    PACMAN_menu_juego();
END


PROCESS PACMAN_FinJuego()
BEGIN
    let_me_alone(); // Elimina todos los procesos que hubiera
    // Fin de Juego
    write_delete(all_text);  // Borra todos los textos que hubiera
    write(PACMAN_fnt,320,197,ALIGN_TOP,"GAME OVER");
    FRAME(10000);            // Espera un poco en la ultima imagen
    fade_off(200);             // Apaga la pantalla
    write_delete(all_text);  // Borra todos los textos que hubiera

    // Nueva maxima puntuacion
    IF (PACMAN_puntuacion > PACMAN_puntuacion_max)
        PACMAN_puntuacion_max=PACMAN_puntuacion;  // Cambia la variable que guarda el valor de maxima puntuacion
        // Y graba la maxima puntuacion en disco
        save(get_pref_path("bennugd.org", "pacman") + "pacman.dat",PACMAN_puntuacion_max);
    END

    // Resetea todas las variables para empezar de nuevo
    PACMAN_puntuacion_antigua=0;
    PACMAN_puntuacion=0;
    PACMAN_nivel=0;
    PACMAN_puntos=0;
    PACMAN_idpacman=0;

    PACMAN_menu_juego();
END

//------------------------------------------------------------------------------
// Proceso PacMan
//------------------------------------------------------------------------------

PROCESS PACMAN_PacMan()
PRIVATE
    PACMAN_fantasma contador;           // Un contador general
    PACMAN_fantasma comido;             // Identificador de fantasma

    int imagen;             // Contador de imagenes
    int velocidad_pacman=2; // Velocidad de pacman
    int incr_x=0;           // Coordenadas relativas
    int incr_y=0;
    int grafico_viejo;      // Contenedor temporal de graficos
    int pasos[]=0,1,2,1;    // Tabla de animacion
    int paso=0;             // Contador de animacion
    int grafico_inicial=3;  // Grafico inicial
    int puntos_por_comido=200;  // Contador de fantasmas comidos

    // Tabla con el tiempo que dura el efecto de poder comer
    int tiempo_capsulas[]=0,300,240,180,140,120,100,80,60,40,0;

    int direccion_pendiente=1;
BEGIN
    file=PACMAN_fpg;
    PACMAN_idpacman=id;        // Coge el identificador del proceso
    region=1;           // Hace que el proceso aparezca dentro de la region 1
    graph=3;            // Asigna el grafico y las coordenadas
    x=320;
    y=364;

    LOOP
        // Lee teclas
        IF (jkeys_state[_JKEY_A])    // Cambia velocidad si se pulsa la barra espaciadora
            velocidad_pacman=4;
        ELSE
            velocidad_pacman=2;
        END


        // Comprueba si se pulso el cursor de la derecha y puede ir en esa direccion
        IF (jkeys_state[_JKEY_RIGHT] || direccion_pendiente == 2)
            IF (PACMAN_camino_pacman(x+2,y))
                grafico_inicial=3;  // Elige el grafico inicial
                flags=0;
                incr_x=2;           // Pone los incrementos vertical y horizontal
                incr_y=0;
                direccion_pendiente = 0;
            ELSE
                direccion_pendiente = 2;
            END
        END

        // Comprueba si se pulso el cursor de la izquierda y puede moverse en esa direccion
        IF (jkeys_state[_JKEY_LEFT] || direccion_pendiente == 1)
            IF (PACMAN_camino_pacman(x-2,y))
                grafico_inicial=3;
                flags=1;
                incr_x=-2;
                incr_y=0;
                direccion_pendiente = 0;
            ELSE
                direccion_pendiente = 1;
            END
        END

        // Comprueba si se pulso el cursor de abajo y puede avanzar
        IF (jkeys_state[_JKEY_DOWN] || direccion_pendiente == 3)
            IF (PACMAN_camino_pacman(x,y+2))
                grafico_inicial=6;
                flags=0;
                incr_y=2;
                incr_x=0;
                direccion_pendiente = 0;
            ELSE
                direccion_pendiente = 3;
            END
        END

        // Comprueba si se pulso el cursor de arriba y puede avanzar
        IF (jkeys_state[_JKEY_UP] || direccion_pendiente == 4)
            IF (PACMAN_camino_pacman(x,y-2))
                grafico_inicial=6;
                flags=2;
                incr_y=-2;
                incr_x=0;
                direccion_pendiente = 0;
            ELSE
                direccion_pendiente = 4;
            END
        END

        // Comprueba caminos en la direccion horizontal
        IF (PACMAN_camino_pacman(x+incr_x,y))
            x+=incr_x;  // Si se pude mover se mueve
        ELSE
            incr_x=0;   // Detiene el movimiento
            paso=1;     // Y reinicia la animacion
        END

        // Comprueba caminos en la direccion horizontal
        IF (PACMAN_camino_pacman(x,y+incr_y))
            y+=incr_y;  // Si se pude mover se mueve
        ELSE
            incr_y=0;   // Detiene el movimiento
            paso=1;     // Y reinicia la animacion
        END

        // Selecciona graficos
        graph=grafico_inicial+pasos[paso];

        // Unicamente muestra la pantalla a la velocidad de PacMan
        IF (imagen => velocidad_pacman)
            FRAME;  // Muestra los graficos
            imagen=0;
            // Comprueba si ha colisionado con un fantasma
            comido=collision(TYPE PACMAN_fantasma);
            IF (comido)
                // Mira si realmente el fantasma esta cerca
                IF (abs(x-comido.x) > 10 OR abs(y-comido.y) > 10)
                    comido=0;   // Hace como si no se hubieran tocado
                END
            END
            // Actualiza la animacion del grafico
            IF (incr_x <> 0 OR incr_y <> 0)
                paso=(paso+1) MOD 4;
            END
        END
        imagen++;   // Incrementa el contador de imagenes

        // Verifica si pacman ha salido por los lados
        IF (x <= 95)
            x+=450; // Hace que aparezca por el otro lado
        END

        IF (x => 546)
            x-=450;
        END

        // Pacman ha comido un punto
        IF (PACMAN_punto(x,y) == 14)
            PACMAN_puntos++;       // Incrementa el contador de puntos comidos
            PACMAN_puntuacion+=10; // Incrementa la puntuacion
            put(PACMAN_fpg,9,x,y);   // Borra el punto del mapa de durezas
            imagen++;       // Incrementa el contador imagenes
            sound_play(PACMAN_snd_come_punto,0);    // Hace el sonido
        END

        // Pacman ha comido un punto grande
        IF (PACMAN_punto(x,y) == 10)
            sound_play(PACMAN_snd_come_puntogr,0);  // Realiza el sonido
            PACMAN_puntos++;                       // Incrementa el numero de puntos comidos
            PACMAN_puntuacion+=50;                 // Incrementa la puntuacion
            puntos_por_comido=0;            // Reinicia la variable que guarda
                                            // los puntos que se dan cuando se come a un fantasma
            put(PACMAN_fpg,10,x,y);                  // Quita el punto del mapa de durezas

            // Pone a todos los fantasmas en estado de ser comidos
            contador=get_id(TYPE PACMAN_fantasma);
            WHILE (contador)
                contador.estado=tiempo_capsulas[PACMAN_nivel];
                contador=get_id(TYPE PACMAN_fantasma);
            END
        END

        // Comprueba si ha tocado un fantasma
        IF (comido)
            IF (comido.estado > 0)      // Pacman se come al fantasma
                sound_play(PACMAN_snd_come_fantasma,0);         // Hace el sonido

                // Congela a todos los fantasmas
                contador=get_id(TYPE PACMAN_fantasma);
                WHILE (contador)
                    signal(contador,s_freeze);
                    contador=get_id(TYPE PACMAN_fantasma);
                END

                grafico_viejo=graph;                    // Guarda el grafico que tenia
                graph=0;                                // Quita el grafico
                comido.graph=42+puntos_por_comido;      // Cambia el grafico del fantasma
                                                        // por el de la puntuacion obtenida
                FRAME(1500);                            // Espera a que se den 15 imagenes

                // Vuelve a todos los fantasma al estado en que estaban
                contador=get_id(TYPE PACMAN_fantasma);
                WHILE (contador)
                    signal(contador,s_wakeup);
                    contador=get_id(TYPE PACMAN_fantasma);
                END
                graph=grafico_viejo;                           // Recupera el grafico de pacman
                PACMAN_ojos(comido.x,comido.y,comido.modelo);  // Crea unos ojos de fantasma
                signal(comido,s_kill);                         // Elimina el fantasma comido
                SWITCH (puntos_por_comido);                    // Da la puntuacion segun los fantasmas comidos
                    CASE 0: PACMAN_puntuacion+=200; END
                    CASE 1: PACMAN_puntuacion+=400; END
                    CASE 2: PACMAN_puntuacion+=800; END
                    CASE 3: PACMAN_puntuacion+=1600; END
                    CASE 4: PACMAN_puntuacion+=3200; END
                END
                puntos_por_comido++;                           // Incrementa para la proxima vez que coma
            ELSE                    // Fantasma se come a pacman

                // Congela los procesos de todos los fantasmas
                contador=get_id(TYPE PACMAN_fantasma);
                WHILE (contador)
                    signal(contador,s_freeze);
                    contador=get_id(TYPE PACMAN_fantasma);
                END
                FRAME(1500);        // Espera a que se den 15 imagenes

                // Elimina los procesos de fantasmas, ojos y frutas
                signal(TYPE PACMAN_fantasma,s_kill);
                signal(TYPE PACMAN_ojos,s_kill);
                signal(TYPE PACMAN_fruta,s_kill);
                flags=0;                    // Coloca el grafico sin ningun espejado
                sound_play(PACMAN_snd_muerte,0);     // Realiza el sonido
                FROM contador=34 TO 41;    // Hace la animacion de ser comido
                    graph=contador;
                    FRAME(400);
                END
                FRAME(800);                // Espera 8 imagenes de pantalla
                IF (PACMAN_ividas <> 0)             // Comprueba si le quedan vidas
                    PACMAN_ividas--;              // Quita una vida
                    signal(PACMAN_vidas[PACMAN_ividas],s_kill);
                    PACMAN_inicio_juego(); // Reinicia el juego
                ELSE
                    PACMAN_FinJuego();   // Final del juego
                END
                signal(id,s_kill);
                FRAME;
            END
            comido=0;               // Borra cualquier colision detectada que hubiera
        END
    END
END

//------------------------------------------------------------------------------
// Proceso camino_pacman
// Controla por donde puede ir pacman
//------------------------------------------------------------------------------

PROCESS PACMAN_camino_pacman(double x, y)
PRIVATE
    int numero_color;   // Numero de color en el mapa de durezas
BEGIN
    numero_color=PACMAN_color(x,y);     // Coge el color del mapa de durezas
    RETURN(numero_color == 10 OR
           numero_color == 12 OR
           numero_color == 14);           // Retorna TRUE si puede avanzar
END

//------------------------------------------------------------------------------
// Proceso punto
// Coge un color del mapa de durezas
//------------------------------------------------------------------------------

PROCESS PACMAN_punto(double x, y)
BEGIN
    int color_punto = get_index_color(map_get_pixel(0, background.graph, x, y));
    IF ( color_punto == 16 ) // Comprueba que el color no sea incorrecto
        RETURN(0);          // Devuelve FALSE (= 0 = falso) porque ese color es valido
    ELSE
        RETURN(PACMAN_color(x,y)); // Retorna el codigo de color
    END
END

//------------------------------------------------------------------------------
// Proceso camino_fantasma
// Controla por donde ira el fantasma
//------------------------------------------------------------------------------

PROCESS PACMAN_camino_fantasma(double x, y, int dir)
PRIVATE
    int n_color;  // Color 10=Punto grande 11=Casa de fantasma 12=Camino 14=Punto
BEGIN
    // Comprueba la direccion
    SWITCH (dir)
        CASE 0: n_color=PACMAN_color(x-2,y); END
        CASE 1: n_color=PACMAN_color(x+2,y); END
        CASE 2: n_color=PACMAN_color(x,y+2); END
        CASE 3: n_color=PACMAN_color(x,y-2); END
    END

    // El fantasma no vuelve a entrar en su casa
    IF (n_color == 11 AND dir == 2 AND PACMAN_color(x,y) == 12)
        n_color=0;
    END

    // Retorna verdadero (TRUE) si es un camino correcto
    RETURN(n_color == 11 OR n_color == 10 OR n_color == 12 OR n_color == 14);
END

//------------------------------------------------------------------------------
// Proceso color
// Coge un color del mapa de durezas
//------------------------------------------------------------------------------

function PACMAN_color(double x, y)
BEGIN
    // Comprueba si son los lados de la pantalla
    IF ((x < 105 OR x > 534) AND (y == 225 OR y == 226))
        RETURN(12); // Devuelve un color de camino
    END
    return get_index_color( map_get_pixel(PACMAN_fpg,2,(x-105)/2,(y-1)/2) );
END

//------------------------------------------------------------------------------
// Proceso fantasma
//------------------------------------------------------------------------------

PROCESS PACMAN_fantasma(double x, y, int tipo)
PRIVATE
    int imagen;         // Contador de graficos
    int num_imagenes;   // Numero de imagenes en que da en pantalla
    int dir=3;          // Direccion 0=izq. 1=der. 2=abajo 3=arriba
BEGIN
    modelo=tipo;
    region=1;       // Lo introduce dentro de la region definida
    file=PACMAN_fpg;
    LOOP
        // Si el fantasma esta en casa entonces da mas imagenes
        IF (PACMAN_color(x,y) == 11 OR estado > 0)
            num_imagenes=1;
        ELSE
            num_imagenes=2;
        END

        // Si el fantasma esta en un cruce selecciona un camino
        IF (PACMAN_cuenta_dir(x,y) > 2)
            dir=PACMAN_selecciona_dir(x,y,dir); // Devuelve una direccion correcta
        ELSE
            // Si no tiene ningun camino por donde avanza
            IF (NOT PACMAN_camino_fantasma(x,y,dir))
                dir=PACMAN_selecciona_dir(x,y,dir);    // Elige otro camino
            END

            // Aleatoriamente cambia la direccion si se puede
            IF (rand(0,1000) < 2)
                dir=PACMAN_selecciona_dir(x,y,dir BXOR 1);
            END
        END

        SWITCH (dir)    // Mueve al fantasma
            CASE 0: x-=2; END
            CASE 1: x+=2; END
            CASE 2: y+=2; END
            CASE 3: y-=2; END
        END

        // Comprueba si se sale por los lados de la pantalla
        IF (x <= 95) x+=450; END
        IF (x => 546) x-=450; END

        // Comprueba el estado del fantasma
        IF (estado == 0)
            graph=modelo+dir;   // Elige un grafico de la direccion en estado normal
        ELSE
            // El fantasma puede ser comido y parpadea cambiando graficos
            IF (estado < 70)
                IF (estado MOD 7 == 0)
                    IF (graph == 11)
                        graph=28;
                    ELSE
                        graph=11;
                    END
                END
            ELSE
                graph=11;
            END
            estado--;           // Decrementa el contador de estado del fantasma
        END

        // Unicamente muestra la imagen a la velocidad del fantasma
        IF (imagen => num_imagenes)
            FRAME;
            imagen=0;
        END
        imagen++;               // Incrementa el contador de imagenes
    END
END

//------------------------------------------------------------------------------
// Proceso selecciona_dir
// Controla por donde ira el fantasma
//------------------------------------------------------------------------------

PROCESS PACMAN_selecciona_dir(double x, y, int old_dir)
PRIVATE
    PACMAN_fantasma padre;

    int contador;     // Un contador general
    int dir[3];       // Tabla de posibles direcciones
    int num_dir=0;    // Numero de direcciones
    int dir1;         // Direcciones temporales
    int dir2;

    // Tabla de dificultad
    int inteligencia[]=0,10,30,50,65,75,85,90,95,100,100;
BEGIN
    padre=father;

    // Contando direcciones
    FROM contador=0 TO 3;
        // Comprueba si se puede avanzar en la direccion de contador
        IF (PACMAN_camino_fantasma(x,y,contador) AND old_dir <> (contador BXOR 1))
            dir[num_dir]=contador; // Si se puede se guarda
            num_dir++;              // Y se incrementa el contador de direcciones posibles
        END
    END

    // Cambia la direccion si no hay ninguna otra
    IF (num_dir == 0)
        dir[num_dir]=old_dir BXOR 1;
        num_dir++;
    END

    // Selecciona la direccion de acuerdo con el nivel
    contador=dir[rand(0,num_dir-1)];
    // Aleatoriamente y segun el nivel elige una direccion u otra
    IF (rand(0,100) < inteligencia[PACMAN_nivel])
        IF (padre.estado == 0) // Mira en que estado estaba el fantasma
            IF (PACMAN_idpacman)     // Comprueba si pacman existe
                // Es el estado de poder comer y va hacia pacman
                // Mira que distancia es menor, la horizontal o la vertical
                IF (abs(PACMAN_idpacman.x-father.x) > abs(PACMAN_idpacman.y-father.y))
                    // Mira si a la derecha o a la izquierda
                    IF (PACMAN_idpacman.x > father.x)
                        dir1=1; // Guarda la primera direccion posible
                    ELSE
                        dir1=0;
                    END
                    IF (PACMAN_idpacman.y > father.y)
                        dir2=2; // Guarda una segunda direccion posible
                    ELSE
                        dir2=3;
                    END
                ELSE            // La diferencia vertical es mayor
                    IF (PACMAN_idpacman.y > father.y)
                        dir1=2; // Guarda la primera direccion posible
                    ELSE
                        dir1=3;
                    END
                    IF (PACMAN_idpacman.x > father.x)
                        dir2=1; // Guarda una segunda direccion posible
                    ELSE
                        dir2=0;
                    END
                END
            END
        ELSE
            // Es el estado de ser comido, se aleja de pacman
            // Se comprueba que diferencia es mayor, la horizontal o la vertical
            IF (abs(PACMAN_idpacman.x-father.x) < abs(PACMAN_idpacman.y-father.y))
                // Y comprueba si es a la izquierda o a la derecha
                IF (PACMAN_idpacman.x > father.x)
                    dir1=0;     // Guarda la primera direccion posible
                ELSE
                    dir1=1;
                END
                IF (PACMAN_idpacman.y > father.y)
                    dir2=3;     // Guarda una segunda direccion posible
                ELSE
                    dir2=2;
                END
            ELSE
                // Mira si hacia arriba o hacia abajo
                IF (PACMAN_idpacman.y > father.y)
                    dir1=3;    // Guarda la primera direccion posible
                ELSE
                    dir1=2;
                END
                IF (PACMAN_idpacman.x > father.x)
                    dir2=0;    // Guarda una segunda direccion posible
                ELSE
                    dir2=1;
                END
            END
        END

        // Si se puede avanzar en la primera direccion posible, la devuelve
        IF (old_dir == dir1 AND PACMAN_camino_fantasma(x,y,dir1))
            RETURN(dir1);
        ELSE    // Si no, si se puede avanzar, devuelve la segunda posible direccion
            IF (NOT PACMAN_camino_fantasma(x,y,dir1) AND old_dir == dir2 AND PACMAN_camino_fantasma(x,y,dir2))
                RETURN(dir2);
            END
        END
        // Si no devuelve la posicion en la que sea posible avanzar
        IF (PACMAN_camino_fantasma(x,y,dir1) AND old_dir <> (dir1 BXOR 1))
            contador=dir1;
        ELSE
            IF (PACMAN_camino_fantasma(x,y,dir2) AND old_dir <> (dir2 BXOR 1))
                contador=dir2;
            END
        END
    END
    RETURN(contador);
END

//------------------------------------------------------------------------------
// Proceso cuenta_dir
// Cuenta las posibles direcciones del fantasma
//------------------------------------------------------------------------------

PROCESS PACMAN_cuenta_dir(double x, y)
PRIVATE
    int contador;       // Un contador general
    int dir=0;          // Numero de direcciones
BEGIN
    REPEAT          // Va mirando por todas la direcciones
        // Si el camino es posible incrementa el contador
        IF (PACMAN_camino_fantasma(x,y,contador))
            dir++;  // Contando salidas
        END
    UNTIL (contador++ == 3)
    RETURN(dir);    // Devuelve el numero de direcciones posibles
END

//------------------------------------------------------------------------------
// Proceso ojos
// Maneja los ojos de los fantasmas (cuando son comidos)
//------------------------------------------------------------------------------

PROCESS PACMAN_ojos(double x, y, int modelo)
PRIVATE
    int imagen;  // Numero de grafico
BEGIN
    file=PACMAN_fpg;
    LOOP
        // Comprueba los lados de la pantalla
        IF (x<105) x=105; END
        IF (x>554) x=554; END

        // Selecciona la direccion y el grafico de acuerdo al color del mapa de caminos
        SWITCH (get_index_color(map_get_pixel(PACMAN_fpg,33,(x-105)/2,(y-1)/2)))
            CASE 14: x-=2; graph=29; END
            CASE 10: x+=2; graph=30; END
            CASE 12: y+=2; graph=31; END
            CASE 9:  y-=2; graph=32; END
            // Si es el color 11 es que ha llegado a casa, quita los ojos y pone un fantasma
            CASE 11: imagen=0; PACMAN_fantasma(x,y,modelo); signal(id,s_kill); END
        END
        // Solo muestra los graficos cada cuatro imagens
        IF ((imagen BAND 3) == 0) FRAME; END
        imagen++;
    END
END

//------------------------------------------------------------------------------
// Proceso vida
// Imprime las vidas del marcador
//------------------------------------------------------------------------------

PROCESS PACMAN_vida(double x)
BEGIN
    file=PACMAN_fpg;
    y=14;       // Elige la coordenada vertical
    graph=4;    // Selecciona grafico
    LOOP
        FRAME;
    END
END

//------------------------------------------------------------------------------
// Proceso parpadeante
// Imprime los puntos grandes
//------------------------------------------------------------------------------

PROCESS PACMAN_parpadeante(double x, y)
BEGIN
    file=PACMAN_fpg;
    z=10;
    LOOP
        graph=10;   // Imprime el grafico
        FRAME(300); // Espera
        graph=0;    // No pone ningun grafico
        FRAME(300); // Espera
    END
END

//------------------------------------------------------------------------------
// Proceso fruta
// Maneja las frutas
//------------------------------------------------------------------------------

PROCESS PACMAN_fruta()
PRIVATE
    int tiempofruta=100;        // Contador de tiempo

    // Tabla con los graficos de la puntuacion de la fruta
    int g_valor_frutas[]=0,54,55,56,56,57,57,58,58,59,59;

    // Tabla con los graficos de la fruta
    int g_frutas[]=0,48,49,50,50,51,51,52,52,53,53;

    // Tabla con la puntuacion de las frutas
    int valor_frutas[]=0,100,300,500,500,700,700,1000,1000,2000,2000;
BEGIN
    file=PACMAN_fpg;
    x=320;                  // Asigna las coordenadas y el grafico
    y=270;
    graph=g_frutas[PACMAN_nivel];
    z=10;                   // Selecciona el grafico

    // Repite hasta que tiempofruto sea 0
    WHILE (tiempofruta > 0)
        tiempofruta--;      // Decrementa tiempo de fruta
        // Fruta comida
        IF (collision(TYPE PACMAN_pacman))
            sound_play(PACMAN_snd_come_fruta,0);        // Hace el sonido
            tiempofruta=-20;                    // Pone otra pausa
            PACMAN_puntuacion+=valor_frutas[PACMAN_nivel];    // Suma puntuacion
            graph=g_valor_frutas[PACMAN_nivel];        // Pon grafico con puntos
        END
        FRAME;
    END

    // Espera el valor que se puso negativo en tiempofruta
    WHILE (tiempofruta < 0)
        tiempofruta++;
        FRAME;
    END
END

//------------------------------------------------------------------------------
// Proceso titulo
// Imprime y mueve los texto
//------------------------------------------------------------------------------

PROCESS PACMAN_titulo(int txt)
PRIVATE
    int contador;           // Un contador general

    // Coordenadas verticales de titulo
    int tabla_grafico[]=50,52,53,54,54,53,52,50,48,47,46,46,47,48;
BEGIN
    file=PACMAN_fpg;
    graph=47;       // Selecciona grafico de titulo
    x=320;          // Pone coordenada horizontal
    z=-10;

    LOOP
        contador=(contador+1) MOD 14;

        // Imprimelo dentro de pantalla
        IF (contador == 0) write_move(txt,320,320); END

        // Imprimelo fuera de pantalla lo que hace al texto intermitente
        IF (contador == 7) write_move(txt,320,640); END
        // Mueve el titulo
        y=tabla_grafico[contador];    // Mueve el titulo
        FRAME;
    END
END

//------------------------------------------------------------------------------
// Proceso inicio_juego
// Reinicia el juego
//------------------------------------------------------------------------------

PROCESS PACMAN_inicio_juego()
PRIVATE
    int texto1;     // Identificadores de texto
    int texto2;
BEGIN
    // Para cualquier sonido que estuviera sonando
    sound_stop(all_sound);
    fade_on(200);  // Encender la pantalla

    signal( type jkeys_controller, s_kill ); jkeys_controller();

    // Inicia el sonido de entrada e imprime los texto necesarios
    sound_play(PACMAN_snd_empieza,0);
    texto1=write(PACMAN_fnt,320,243,ALIGN_TOP,"READY!");
    texto2=write(PACMAN_fnt,320,152,ALIGN_TOP,"Level " + PACMAN_nivel);

    FRAME(9000);        // Espera

    // Borra los textos
    write_delete(texto1);
    write_delete(texto2);

    signal( type jkeys_controller, s_kill ); jkeys_controller();

    // Crea a pacman y a los fantasma
    PACMAN_pacman();
    PACMAN_fantasma(320,177,12);
    PACMAN_fantasma(290,223,16);
    PACMAN_fantasma(320,223,20);
    PACMAN_fantasma(352,223,24);
END

PROCESS PACMAN_menu_juego()
PRIVATE
    int snd_channel_inicio;     // Identificador para el canal del sonido s_inicio
BEGIN
    PACMAN_parpadeante(128,56);  // Pone los puntos grandes
    PACMAN_parpadeante(512,56);
    PACMAN_parpadeante(128,364);
    PACMAN_parpadeante(512,364);

    signal( type jkeys_controller, s_kill ); jkeys_controller();

    // Pone la pantalla de inicio
    LOOP
        // Ejecuta sonido
        snd_channel_inicio=sound_play(PACMAN_snd_inicio,0);

        PACMAN_nivel=1;    // Primer PACMAN_nivel

        // Imprime pantalla de fondo
        put(PACMAN_fpg,1,320,240,0,100,4);

        // Pone textos explicativos
        PACMAN_titulo(write(PACMAN_fnt,320,320,ALIGN_TOP,"Press START"));
        write(PACMAN_fnt,320,90,ALIGN_TOP,"RECORD");
        write_var(PACMAN_fnt,320,120,ALIGN_TOP,PACMAN_puntuacion_max);
        write(PACMAN_fnt,320,430,ALIGN_TOP,"1996 PACMAN (DIV) - Port TYCO");

        PACMAN_fantasma(320,177,12);       // Crea un fantasma
        son.flags=4;                       // Lo hace transparente
        PACMAN_fantasma(290,223,16);       // Crea otro fantasma
        son.flags=4;                       // Lo hace transparente
        PACMAN_fantasma(320,223,20);       // Crea otro fantasma
        son.flags=4;                       // Lo hace transparente
        PACMAN_fantasma(352,223,24);       // Crea otro fantasma
        son.flags=4;                       // Lo hace transparente

        fade_on(200);  // Enciende la pantalla

        // Espera a que se pulse START
        WHILE (NOT jkeys_state[_JKEY_SELECT])
            IF (jkeys_state[_JKEY_MENU])      // Sale del programa cuando se pulsa escape
                WHILE (jkeys_state[_JKEY_MENU])
                    FRAME;
                END
                let_me_alone();
                sound_stop(all_sound);
                fpg_unload(PACMAN_fpg);
                fnt_unload(PACMAN_fnt);
                sound_unload(PACMAN_snd_come_fantasma);
                sound_unload(PACMAN_snd_come_fruta);
                sound_unload(PACMAN_snd_come_punto);
                sound_unload(PACMAN_snd_come_puntogr);
                sound_unload(PACMAN_snd_empieza);
                sound_unload(PACMAN_snd_muerte);
                sound_unload(PACMAN_snd_inicio);
                set_mode(SCREEN_WIDTH,SCREEN_HEIGHT);     // Deja el modo de video como estaba para volver al Menu de Juegos
                set_fps(FPS_GAME,0);
                RETURN;
            END
            FRAME;
        END

        // Detiene el sonido inicial del menu
        sound_stop(snd_channel_inicio);

        // Apaga la pantalla
        fade_off(200);
        let_me_alone();         // Elimina todos los procesos que hubiera
        write_delete(all_text);  // Elimina tambien todos los textos

        signal( type jkeys_controller, s_kill ); jkeys_controller();

        PACMAN_parpadeante(128,56);    // Crea los puntos gordos
        PACMAN_parpadeante(512,56);
        PACMAN_parpadeante(128,364);
        PACMAN_parpadeante(512,364);

//        clear_screen();         // Borra la pantalla
        // Y pone el fondo nuevo
        put(PACMAN_fpg, 1, 320, 240);

        // Pone los dos logos de los laterales
        put(PACMAN_fpg,47,56,260,pi/2,100,0);
        put(PACMAN_fpg,47,585,260,-pi/2,100,0);

        // Escribe la puntuacion
        write_var(PACMAN_fnt,100,0,ALIGN_TOP_RIGHT,PACMAN_puntuacion);

        // Crea los procesos que contabilizan las vidas
        PACMAN_ividas=0;
        PACMAN_vidas[PACMAN_ividas]=PACMAN_vida((PACMAN_ividas*26)+552);
        PACMAN_ividas++;
        PACMAN_vidas[PACMAN_ividas]=PACMAN_vida((PACMAN_ividas*26)+552);
        PACMAN_ividas++;

        // Reinicializa el numero de puntos comidos y la puntuacion
        PACMAN_puntos=0;
        PACMAN_puntuacion=0;
        // Llama a un proceso que reinicia el juego
        PACMAN_inicio_juego();

        // Empieza el juego
        // Repite hasta que no tenga vidas o se pulse la tecla escape
        WHILE (PACMAN_ividas => 0 AND NOT jkeys_state[_JKEY_MENU])
            IF (PACMAN_puntos == 246)        // Siguiente nivel
                PACMAN_nivel++;              // Incremente el nivel (fase)
                IF (PACMAN_nivel > 10)
                    let_me_alone(); // Elimina todos los procesos que hubiera
                    write(PACMAN_fnt,320,197,ALIGN_TOP,"THE END");
                    write(PACMAN_fnt,320,230,ALIGN_TOP,"CONGRATULATIONS!");
                    FRAME(10000);            // Espera un poco en la ultima imagen
                    fade_off(200);             // Apaga la pantalla
                    write_delete(all_text);  // Borra todos los textos que hubiera
//                    clear_screen();         // Borra la pantalla

                    // Nueva maxima puntuacion
                    IF (PACMAN_puntuacion > PACMAN_puntuacion_max)
                        PACMAN_puntuacion_max=PACMAN_puntuacion;  // Cambia la variable que guarda el valor de maxima puntuacion
                        // Y graba la maxima puntuacion en disco
                        save(get_pref_path("bennugd.org", "pacman") + "pacman.dat",PACMAN_puntuacion_max);
                    END

                    // Resetea todas las variables para empezar de nuevo
                    PACMAN_puntuacion_antigua=0;
                    PACMAN_puntuacion=0;
                    PACMAN_nivel=0;
                    PACMAN_puntos=0;
                    PACMAN_idpacman=0;

                    PACMAN_menu_juego();
                    RETURN;
                END

                fade_off(200);           // Apaga la pantalla
                let_me_alone();       // Borra todos los procesos que hubiera
                // Crea los procesos que contabilizan las vidas
                PACMAN_ividas=0;
                PACMAN_vidas[PACMAN_ividas]=PACMAN_vida((PACMAN_ividas*26)+552);
                PACMAN_ividas++;
                PACMAN_vidas[PACMAN_ividas]=PACMAN_vida((PACMAN_ividas*26)+552);
                PACMAN_ividas++;
                // Crea otros 4 nuevos puntos parpadeantes
                PACMAN_parpadeante(128,56);
                PACMAN_parpadeante(512,56);
                PACMAN_parpadeante(128,364);
                PACMAN_parpadeante(512,364);
                // Pone otro fondo de pantalla
                put(PACMAN_fpg,1,320,240);

                // Reinicia el numero de puntos comidos y llama a un proceso que reinicia el juego
                PACMAN_puntos=0;
                PACMAN_inicio_juego();
            END

            // Pone las frutas cada vez se comen 100 o 200 puntos (cocos)
            IF (PACMAN_puntos == 100 OR PACMAN_puntos == 200)
                PACMAN_fruta();
                PACMAN_puntos++;
            END

            // Da una vida a los 10000 puntos
            IF (PACMAN_puntuacion => 10000 AND PACMAN_puntuacion_antigua < 10000)
                PACMAN_vidas[PACMAN_ividas]=PACMAN_vida(PACMAN_ividas*26+552);
                PACMAN_ividas++;
            END

            // Da otra vida a los 50000 puntos
            IF (PACMAN_puntuacion => 50000 AND PACMAN_puntuacion_antigua < 50000)
                PACMAN_vidas[PACMAN_ividas]=PACMAN_vida(PACMAN_ividas*26+552);
                PACMAN_ividas++;
            END
            // Actualiza puntuacion antigua, necesaria para dar vidas solo una vez
            PACMAN_puntuacion_antigua=PACMAN_puntuacion;

            FRAME;
        END
        WHILE (jkeys_state[_JKEY_MENU]) FRAME; END
        PACMAN_FinJuego();
    END
END
