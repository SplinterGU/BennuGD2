//****************************************
//*                                      *
//*               GALAXIAN               *
//*                  BY                  *
//*              Kim-elet-o              *
//*                                      *
//****************************************

import "mod_gfx";
import "mod_input";
import "mod_misc";
import "mod_sound";

#include "jkeys.lib"

CONST
                   // ***************** CONSTANTES DE INICIALIZACION DEL PROGRAMA ***************************

    Gal_ResolucionX       = 800;                 // Resolucion del modo grafico.
    Gal_ResolucionY       = 600;

    Gal_FramesSegundo     = 60;                  // Fija los frames por segundo y los frames que el programa
    Gal_FramesSalto       = 3;                   // puede saltarse por defecto.

    Gal_RegionJuego       = 1;                   // Identificador de la region del juego.
                                                 // Definicion de la region donde se desarrollara el juego, esta simulara la pantalla de video
    Gal_AnchoRegionJuego  = Gal_ResolucionY*7/8; // de la maquina recreativa, 7:8 es la relacion de aspecto de la pantalla de la maquina recreativa.
    Gal_AltoRegionJuego   = Gal_ResolucionY;
    Gal_RegionJuegoX      = Gal_ResolucionX/2-(Gal_AnchoRegionJuego/2);
    Gal_RegionJuegoY      = 0;

    Gal_AnchoFondoScroll = Gal_AnchoRegionJuego; // Ancho del grafico del fondo de scroll.
    Gal_AltoFondoScroll = Gal_AltoRegionJuego;   // alto del grafico del fondo de scroll.

                   // ************* CONSTANTES DEL ESCUADRON DE MARCIANOS Y DE LOS MARCIANOS ****************
    Gal_NFilas            = 6;                   // Numero de filas que posee el escuadron de marcianos.
    Gal_NColumnas         = 10;                  // Numero de Columnas que posee el escuadron de marcianos.

    Gal_EscuadronPosX     = Gal_RegionJuegoX+100;// Posicion inicial del escuadron de marcianos en pantalla.
    Gal_EscuadronPosY     = 60;

    Gal_MarcianoSeparacionX = 41;                // Separacion entre marcianos dentro del escuadron.
    Gal_MarcianoSeparacionY = 31;

    Gal_NFasesMarciano    = 3;                   // Numero de fases que tiene la animacion de los Marcianos.
    Gal_FramesFaseMarciano = 20;                 // Numero de frames que tarda en cambiar la fase de la animacion de los marcianos.

    Gal_MensajeMarcianoWait = 1;                 // Mensaje de espera del marciano.
    Gal_MensajeMarcianoAtackDerecha = 2;         // Mensaje de ataque del marciano girando a la derecha.
    Gal_MensajeMarcianoAtackIzquierda = 3;       // Mensaje de ataque del marciano girando a la izquierda.
    Gal_MensajeMarcianoKamikaze = 4;             // Mensaje de estado ataque kamikaze del marciano.
    Gal_MensajeMarcianoBack = 5;                 // Mensaje de regreso a formacion del marciano.

    Gal_DistanNoGiroMarciano = 135;              // Distancia a la cual el marciano no gira si esta realizando el ataque.
    Gal_AnguloGiroMarciano = 5000;               // Angulo de giro del marciano a la hora de iniciar el ataque.
    Gal_MaximaDesviacionAngulo = 15000;          // Maxima desviacion que sufrira el calculo del angulo entre la nave y el marciano.
    Gal_ModuloGiroMarciano = 3;                  // Modulo de giro del marciano a la hora de iniciar el ataque.
    Gal_FramesAjusteKamikaze = 30;               // Frames que tiene de tiempo el marciano para ajustar el angulo del ataque Kamikaze.
    Gal_IncrementoAnguloKamikaze = 2000;         // Angulo de incremento para el ajuste del angulo del ataque kamikaze.
    Gal_ModuloAtaqueKamikaze = 4;                // Modulo que usa durante el ataque kamikaze.

    Gal_IntervaloDisparoMarciano = 30;           // intervalo entre disparo y disparo del marciano.
    Gal_AjusteYDisparoMarciano = 12;             // ajuste de posicion Y del disparo del marciano.

    Gal_SentidoDerecha    = 1;                   // El escuadron se mueve a la derecha.
    Gal_SentidoIzquierda  = 2;                   // El escuadron se mueve a la Izquierda.
    Gal_PuntosMarciano1 = 20;                    // Puntos del marciano tipo 1, que suman en la puntuacion al ser este destruido.
    Gal_PuntosMarciano2 = 30;                    // Puntos del marciano tipo 2, que suman en la puntuacion al ser este destruido.
    Gal_PuntosMarciano3 = 50;                    // Puntos del marciano tipo 3, que suman en la puntuacion al ser este destruido.
    Gal_PuntosMarciano4 = 80;                    // Puntos del marciano tipo 4, que suman en la puntuacion al ser este destruido.
                                                 // Fija los limites de los movimientos del escuadron.
    Gal_LimiteMovimientoDerecha = Gal_RegionJuegoX+Gal_AnchoRegionJuego-18;
    Gal_LimiteMovimientoIzquierda = Gal_RegionJuegoX+15;

                   // ***************************** CONSTANTES DEL SCROLL ***********************************
    Gal_FondoScrollInicial = 1;                  // El primer fondo del scroll.
    Gal_FondoScrollFinal  = 3;                   // El ultimo fondo del scroll.
    Gal_FramesScroll      = 15;                  // Numero de frames que tarda en cambiar el fondo del scroll.
    Gal_VelocidadScroll   = 2;                   // Velocidad con que baja el scroll.

                   // ****************************** CONSTANTES DE LA NAVE **********************************
    Gal_NVidas            = 3;                   // Numero de vidas que tiene el jugador.
    Gal_NavePosicionX     = Gal_ResolucionX/2;   // Posicion Inicial de la nave del jugador.
    Gal_NavePosicionY     = 530;
                                                 // Fija los limites de los movimientos de la nave.
    Gal_LimiteNaveDerecha = Gal_RegionJuegoX+Gal_AnchoRegionJuego-20;
    Gal_LimiteNaveIzquierda = Gal_RegionJuegoX+19;
                   // *************************** CONSTANTES DE LOS DISPAROS ********************************
                                                 // Tipos de disparos.
    Gal_DisparoNave       = 1;                   // Disparo de la nave del jugador,
    Gal_DisparoMarciano   = 2;                   // Disparo de los marcianos.

                   // **************************** CONSTANTES DE LAS EXPLOSIONES ****************************
                                                 // Tipos de explosion.
    Gal_ExplosionNave     = 1;                   // Explosion de la nave.
    Gal_ExplosionMarciano = 2;                   // Explosion de los marcianos.

    Gal_NFasesExplosionNave = 4;                 // Numero de Fases que tiene la explosion de la nave del jugador.
    Gal_NFasesExplosionMarciano = 3;             // Numero De Fases que tiene la explosion de los marcianos.
    Gal_FramesFaseExplosion = 5;                 // Numero de frames que tarda a cambiar la fase de la animacion de las explosiones.

                   // ********************* CONSTANTES DEL PROCESO DE CONTROL *******************************
    Gal_PorcentajeAtack   = 25;                  // Porcentaje de que un marciano se le ordene atacar.
    Gal_FramesEsperaAtack = 30;                  // Son los frames que hay que esperar antes de decidir si lanzar otro ataque.
    Gal_FrameEsperaInicioAtack = Gal_FramesSegundo*3; // Espera 3 segundos antes de iniciar los ataques.
    Gal_FramesEsperaNivel = Gal_FramesSegundo*2; // Espera 2 segundos antes de iniciar el nuevo nivel.

    Gal_EstadoEsperaInicial = 1;                 // Estado espera inicial.
    Gal_EstadoAtack       = 2;                   // Estado ataque.

    Gal_MarcianoDerecha   = 1;                   // Flags de seleccion del lado del escuadron saldra un marciano a atacar.
    Gal_MarcianoIzquierda = 2;

                   // *************************** CONSTANTES DEL PROCESO MUESTRAVIDAS ***********************
    Gal_AnchoMapaMuesVidas = 240;                // Ancho del mapa grafico del proceso MuestraVidas.
    Gal_AltoMapaMuesVidas = 42;                  // Alto del mapa grafico del proceso MuestraVidas.
    Gal_AjusteXMapaMuesVidas = 120;              // Ajuste de la posicion X del mapa grafico del proceso MuestrasVidas.
    Gal_AjusteYMapaMuesVidas = 575;              // Ajuste de la posicion Y del mapa grafico del proceso MuestrasVidas.
    Gal_VidasMaximaMostrar = 5;                  // Numero maximo de vidas a mostrar.
    Gal_AjusteXGraficoVida = 17;                 // Ajuste de la posicion X del grafico de las vidas dentro el mapa grafico.
    Gal_AjusteYGraficoVida = 20;                 // Ajuste de la posicion Y del grafico de las vidas dentro el mapa grafico.
    Gal_SeparacionXGraficoVida = 40;             // Separacion entre los graficos de las vidas en la posicion X.

                   //**************************** CONSTANTES DEL PROCESO MUESTRANIVEL ***********************
    Gal_AnchoMapaMuesNivel = 188;                // Ancho del mapa grafico del proceso MuestraNivel.
    Gal_AltoMapaMuesNivel = 45;                  // Alto del mapa grafico del proceso MuestraNivel.
    Gal_AjusteXMapaMuesNivel = 430;              // Ajuste de la posicion X del mapa grafico del proceso MuestrasNivel.
    Gal_AjusteYMapaMuesNivel = 575;              // Ajuste de la posicion Y del mapa grafico del proceso MuestrasNivel.
    Gal_Banderin1Mostrar = 10;                   // Numero de conteo del banderin de 1 nivel.
    Gal_Banderin10Mostrar = 10;                  // Numero de conteo del banderin de 10 niveles.
    Gal_AjusteXGraficoBanderin1 = 11;            // Ajuste de la posicion X del grafico del banderin1 dentro el mapa grafico.
    Gal_AjusteYGraficoBanderin1 = 22;            // Ajuste de la posicion Y del grafico del banderin1 dentro el mapa grafico.
    Gal_AjusteXGraficoConteo1 = 53;              // Ajuste de la posicion X del grafico del conteo1 dentro el mapa grafico.
    Gal_AjusteYGraficoConteo1 = 21;              // Ajuste de la posicion Y del grafico del conteo1 dentro el mapa grafico.
    Gal_AjusteXGraficoBanderin10 = 107;          // Ajuste de la posicion X del grafico del banderin10 dentro el mapa grafico.
    Gal_AjusteYGraficoBanderin10 = 22;           // Ajuste de la posicion Y del grafico del banderin10 dentro el mapa grafico.
    Gal_AjusteXGraficoConteo10 = 163;            // Ajuste de la posicion X del grafico del conteo10 dentro el mapa grafico.
    Gal_AjusteYGraficoConteo10 = 21;             // Ajuste de la posicion Y del grafico del conteo10 dentro el mapa grafico.

                   // ****************************  CODIGOS DE LOS GRAFICOS *********************************
    Gal_GraficoMarciano1  = 10;                  // Grafico del marciano tipo 1.
    Gal_GraficoMarciano2  = 13;                  // Grafico del marciano tipo 2.
    Gal_GraficoMarciano3  = 16;                  // Grafico del marciano tipo 3.
    Gal_GraficoMarciano4  = 19;                  // Grafico del marciano tipo 4.

    Gal_GraficoNaveA      = 5;                   // Grafico de la nave del jugador con el disparo cargado.
    Gal_GraficoNaveB      = 6;                   // Grafico de la nave del jugador sin el disparo.

    Gal_GraficoDisparoNave = 7;                  // Grafico del disparo de la nave del jugador.
    Gal_GraficoDisparoMarciano = 22;             // Grafico del disparo de los marcianos.

    Gal_GraficoExplosionNave = 23;               // Grafico de la explosion de la nave del jugador.
    Gal_GraficoExplosionMarciano = 27;           // Grafico de la explosion de los marcianos.

    Gal_GraficoVida       = 4;                   // Grafico de la vidas del jugador.
    Gal_GraficoBanderin   = 8;                   // Grafico del banderin de 1 nivel.
    Gal_GraficoBanderin10 = 9;                   // Grafico del banderin de 10 niveles.
    Gal_GraficoConteo     = 30;                  // Grafico del conteo.

END


TYPE Gal_Forma                                   // Estructura que guarda la formacion del escuadron.
    Word Linea;
    Byte Tipo;
END


GLOBAL

    Int Gal_Graficos;                            // Libreria de graficos usados en el juego.
    Int Gal_Fuente;                              // Fuente de color rojo para los textos del juego.
    Int Gal_Fuente2;                             // Fuente de color blanco para los textos del juego.
    Int Gal_ScrollId;                            // Identificador del proceso ScrollGalaxian.
    Int Gal_Sentido;                             // Indica el sentido en el que se mueve el escuadron.
    Int Gal_ControlId;                           // Identificador del proceso de control.
    Int Gal_NaveId;                              // Identificador de la nave del jugador.
    Int Gal_MuestraVidasId;                      // Identificador del proceso MuestraVidas.
    Int Gal_MuestraNivelId;                      // Identificador del proceso MuestraNivel.
    Int Gal_Vidas;                               // Numero de vidas que posee el jugador.
    Int Gal_Score;                               // Puntuacion del jugador.
    Int Gal_TopScore;                            // Puntuacion maxima.
    Int Gal_Nivel;                               // Nivel en el cual se esta jugando.

                                                 // Se define la posicion de los marcianos dentro del escuadron de marcianos.
    Gal_Forma Escuadron_Gal[Gal_NFilas-1] = 0048h,4,00FCh,3,01FEh,2,03FFh,1,03FFh,1,03FFh,1;

                                                 // Forma del escuadron:
                                                 //
                                                 //       4     4
                                                 //     3 3 3 3 3 3
                                                 //   2 2 2 2 2 2 2 2
                                                 // 1 1 1 1 1 1 1 1 1 1
                                                 // 1 1 1 1 1 1 1 1 1 1
                                                 // 1 1 1 1 1 1 1 1 1 1

END


LOCAL

    Int Gal_Mensaje;                             // Variable de comunicacion de mensajes entre diferentes procesos.

END

CONST
    VERSION = "0.4";

    SCREEN_WIDTH = 640;
    SCREEN_HEIGHT = 480;
    SCREEN_DEPTH = 16;
    FPS_GAME = 30;

GLOBAL
    // GLOBALES DEL VOLUMEN DE LOS EFECTOS DE SONIDO
    int volumen_musica=50;         //0-128
    int volumen_efectos =128;       //0-128

//******************************************************************************************************
// CODIGO INICIAL DEL PROGRAMA
//******************************************************************************************************
BEGIN
    set_mode(Gal_ResolucionX,Gal_ResolucionY);
    jkeys_set_default_keys();
    signal( type jkeys_controller, s_kill ); jkeys_controller();
    Galaxian_main();
END

//***********************************************************************************************************
//                              SUBRUTINAS VARIAS DE INICIALIZACION
//***********************************************************************************************************

FUNCTION Int CargaPngsEnFpg(String Gal_Ruta);    // rutina para cargar los Pngs de un subdirectorio en una libreria grafica en memoria,
                                                 // la funcion recibe como parametro el subdirectorio donde se encuentran los Pngs,
                                                 // y la funcion devuelve el identificador de la libreria graficaen memoria donde se encuentran los
                                                 // graficos cargados, en el nombre del Png se indica el punto central del grafico, y el N�
                                                 // del grafico en el Fpg, el formato del nombre es:
    PRIVATE                                      // <X punto central>-<Y Punto central>-<N� Grafico>-nombre.Png, Ejemplo: 15-9-11-marciano2A.png
        String Gal_Archivo;                      // Nombre del archivo Png.
        String Gal_Parametros[2];                // Parametros del grafico a cargar en la libreria grafica.
        Int Gal_LibreriaId;                      // Identificador de la libreria grafica en memoria.
        Int Gal_GraficoId;                       // Identificador del grafico a cargar en la libreria grafica.
    BEGIN

        Gal_LibreriaId = Fpg_New();              // creamos una libreria grafica en memoria.

        Gal_Archivo = Glob(Gal_Ruta + "*.png");  // Obtenemos el nombre del primer png,
        WHILE (Gal_Archivo <> "")                // si no quedan mas pngs sal del bucle de busqueda,
            Gal_GraficoId = Map_Load(Gal_Ruta + Gal_Archivo); // cargamos el grafico del png en memoria
            Split("[-\.]",Gal_archivo,OFFSET Gal_Parametros,3); // obtenemos las cadenas de los parametros introducidos en el nombre del png,
            Point_Set(0,Gal_GraficoId,0,Atoi(Gal_Parametros[1]),Atoi(Gal_Parametros[2])); // ponemos el punto central al grafico,
            Fpg_Add(Gal_LibreriaId,Atoi(Gal_Parametros[0]),0,Gal_GraficoId); // introducimos el grafico en la libreria grafica,
            Gal_Archivo = Glob(Gal_Ruta + "*.png"); // obtenemos el nombre del siguiente png,
            Map_Unload(0,Gal_GraficoId);         // descargamos el grafico de la memoria,
        END                                      // bucle de busqueda,

        RETURN(Gal_LibreriaId);                  // devolvemos el identificador de la libreria.

    END


FUNCTION IniciaPrograma();                       // Inicializacion del programa.
    BEGIN
                                                 // Fijamos la region donde discurrira la accion del juego.
        Region_Define(Gal_RegionJuego,Gal_RegionJuegoX,Gal_RegionJuegoY,Gal_AnchoRegionJuego,Gal_AltoRegionJuego);
        Gal_Graficos = CargaPngsEnFpg("gfx/"); //"// Cargamos los graficos que usaremos en el juego.
        Gal_Fuente = Fnt_Load("fnt/galaxian.fnt");      // Cargamos la fuente de color rojo de los textos del juego.
        Gal_Fuente2 = Fnt_Load("fnt/galaxian2.fnt");    // Cargamos la fuente de color blanco de los textos del juego.
        Set_Fps(Gal_FramesSegundo,Gal_FramesSalto); // Configuramos los frames por segundo que el juego puede alcanzar.

    END

FUNCTION TextosInicioJuego();                    // Son los textos que utilizan al inicio del juego.
    BEGIN
        write(Gal_Fuente2,400,200,ALIGN_TOP,"Galaxian");
        write(Gal_Fuente2,400,440,ALIGN_TOP,"Press ENTER/START to start");
        write(Gal_Fuente2,400,480,ALIGN_TOP,"or ESCAPE/BACK to exit");
        Write(Gal_Fuente,200,0,ALIGN_TOP_LEFT,"1UP          HIGH SCORE");
        Write_Var(Gal_Fuente2,235,22,ALIGN_TOP,Gal_Score);
        Write_Var(Gal_Fuente2,355,22,ALIGN_TOP,Gal_TopScore);
    END


FUNCTION GeneraEscuadron();                     // Generacion e inicializacion del escuadron de marcianos.
    PRIVATE
        Int Gal_Fila;                           // Contador de filas.
        Int Gal_Columnas;                       // Contador de columnas.
        Int Gal_Fase;                           // Contador de fase de animacion con que comenzara el enemigo.
    BEGIN

        signal( type Marciano, s_kill );

        FROM Gal_Fila = 0 TO Gal_NFilas-1;      // Bucle de filas para recorrer las filas del mapa de bits del escuadron.
            Gal_Fase = 0;                       // Inicializamos el contador de fase de animacion.
            FROM Gal_Columnas = 0 TO Gal_NColumnas-1; // Bucle de columnas para recorrer las columnas del mapa de bits del escuadron.
                Gal_Fase++;                     // incrementamos una unidad al contador de animacion,
                IF (Gal_Fase == Gal_NFasesMarciano) // miramos si hemos contado todas la animaciones del enemigo,
                    Gal_Fase = 0;               // si es asi inicializamos el contador de fase de animacion,
                END                             // recorriendo el mapa de bits del escuadron, si el bit de posicion correspondiente
                IF (Escuadron_Gal[Gal_Fila].Linea BAND (1<<Gal_Columnas)) // a esa posicion esta activo,
                    Marciano(Gal_EscuadronPosX+Gal_MarcianoSeparacionX*Gal_Columnas,Gal_EscuadronPosY+Gal_MarcianoSeparacionY*Gal_Fila,Escuadron_Gal[Gal_Fila].Tipo,Gal_Fase);
                END                             // si es asi creamos un marciano en la posicion correcta en la pantalla,
            END                                 // teniendo en cuenta el tipo de marciano, y su fase de animacion.
        END

    END


//***********************************************************************************************************
//                                    PROCESO DE DISPARO
//***********************************************************************************************************


PROCESS Disparo(Int Gal_XD,Gal_YD, Byte Gal_Tipo); // Procesos de los disparos del juego.
    PRIVATE
        Int Gal_IdDiana;                         // Segun quien halla realizado el disparo, esta variable podra contener o bien el identificador del marciano,
    BEGIN                                        // o bien el identificador de la nave del jugador, siempre y cuando el disparo colisione.

        X = Gal_XD;                              // Inicializacion del proceso disparo.
        Y = Gal_YD;
        Region = Gal_RegionJuego;
        File = Gal_Graficos;

        cshape = SHAPE_CIRCLE;  // Collision Circle

        SWITCH (Gal_Tipo);                   // Miramos que tipo de disparo es.
            CASE Gal_DisparoNave:            // Tipo Nave,
                LOOP                                     // Bucle principal.
                    Graph = Gal_GraficoDisparoNave; // cargamos el grafico del disparo tipo nave,
                    Y -= 6;                      // decrementamos 6 pixeles hacia arriba,
                    Gal_IdDiana = Collision(Type Marciano); // obtenemos el identificador del marciano, si el disparo ha chocado contra uno,
                    IF (Gal_IdDiana)             // si se ha chocado contra un marciano,
                        Signal(Gal_IdDiana,S_KILL); // lo destruimos
                        BREAK;                   // y salimos del bucle pricipal, terminando asi el proceso disparo.
                    END
                    IF (Region_Out(Id,Gal_RegionJuego)) // si el disparo esta fuera de la region de juego,
                        BREAK;                   // salimos del bucle pricipal, terminando asi el proceso disparo.
                    END
                    FRAME;                               // Esperamos un frame.
                END
            END

            CASE Gal_DisparoMarciano:        // Tipo Marciano,
                LOOP                                     // Bucle principal.
                    Graph = Gal_GraficoDisparoMarciano; // cargamos el grafico del disparo tipo marciano,
                    Y += 5;                      // incrementamos 6 pixeles hacia abajo,
                    Gal_IdDiana = Collision(TYPE Nave); // miramos si ha colisionado con la nave del jugador,
                    IF (Gal_IdDiana)             // si ha chocado contra la nave,
                        Signal(Gal_IdDiana,S_KILL); // destruimos la nave,
                        BREAK;                    // y salimos del bucle pricipal, terminando asi el proceso disparo.
                    END
                    IF (Region_Out(Id,Gal_RegionJuego)) // si el disparo esta fuera de la region de juego,
                        BREAK;                    // salimos del bucle pricipal, terminando asi el proceso disparo.
                    END
                    FRAME;                               // Esperamos un frame.
                END
            END
        END
    END

//***********************************************************************************************************
//                                 PROCESO EXPLOSION
//***********************************************************************************************************

PROCESS Explosion(Int Gal_Xe,Gal_Ye, Byte Gal_Tipo); // Proceso de la explosiones del juego.
    PRIVATE
        Int ContadorFE_Gal;                      // Contador de los frames de la fases de las explosiones.
    BEGIN

        X = Gal_Xe;                               // Inicializacion del proceso explosion.
        Y = Gal_Ye;
        Region = Gal_RegionJuego;
        ContadorFE_Gal = 0;
        File = Gal_Graficos;

        SWITCH (Gal_Tipo)                         // Inicializacion de los graficos del proceso explosion.
            CASE Gal_ExplosionNave:               // Tipo nave.
                Graph = Gal_GraficoExplosionNave;
            END

            CASE Gal_ExplosionMarciano:          // Tipo Marciano.
                Graph = Gal_GraficoExplosionMarciano;
            END

        END

        LOOP                                     // Bucle principal.
            ContadorFE_Gal++;                    // Incrementamos el contador de frames.
            SWITCH (Gal_Tipo)                    // Miramos de que tipo es.
                CASE Gal_ExplosionNave:          // Tipo Nave,
                      IF (ContadorFE_Gal == Gal_FramesFaseExplosion) // Si han pasado todos los frames,
                          ContadorFE_Gal = 0;      // inicializamos el contador de frames,
                          Graph++;                 // incrementamos el contador de la animacion,
                          IF (Graph >= (Gal_GraficoExplosionNave+Gal_NFasesExplosionNave)) // si se ha llegado al final de la animacion,
                              BREAK;               // se rompe el bucle principal, terminando asi el proceso Explosion.
                          END
                      END
                END

                CASE Gal_ExplosionMarciano:      // Tipo Marciano,
                      IF (ContadorFE_Gal == Gal_FramesFaseExplosion) // Si han pasado todos los frames,
                          ContadorFE_Gal = 0;      // inicializamos el contador de frames,
                          Graph++;                 // incrementamos el contador de la animacion.
                          IF (Graph >= (Gal_GraficoExplosionMarciano+Gal_NFasesExplosionMarciano)) // si se ha llegado al final de la animacion,
                              BREAK;               // se rompe el bucle principal, terminando asi el proceso explosion.
                          END
                      END
                END

            END

            FRAME;                               // Esperamos un frame.
        END

    END

//***********************************************************************************************************
//                                 PROCESO MARCIANO
//***********************************************************************************************************


PROCESS Marciano(Int Gal_Xi,Gal_Yi, byte Gal_Tipo,Gal_Fase); // Proceso Marciano.
    PRIVATE
        Int Gal_MemoriaGraph;                    // Grafico base de la animacion del tipo de Marciano.
        Int Gal_ContadorFramesAnimacion;         // Contador de frames de la animacion del marciano.
        Int Gal_ContadorGeneral;                 // Contador de uso general.
        Int Gal_DesviacionAngulo;                // determina la desviacion que tendra el calculo del angulo entre la nave y el marciano.
        Int Gal_Angulo;                          // Angulo del vector del marciano.
        Int Gal_IntervaloDisparo;                // Contador del intervalo del disparo del marciano.
        Int Gal_Puntos;                          // Puntos que da el marciano a ser destruido.
    BEGIN

        cshape = SHAPE_CIRCLE;  // Collision Circle

        X = Gal_Xi;                              // Inicializacion del marciano.
        Y = Gal_Yi;
        Region = Gal_RegionJuego;
        File = Gal_Graficos;
        Gal_Mensaje = Gal_MensajeMarcianoWait;   // Inicializacion la variable mensaje con un mensaje de espera en formacion para el marciano.
        Gal_ContadorFramesAnimacion = 0;         // Inicializacion el contador de frames de la animacion del marciano.
        Gal_Angulo = 90000;                      // Inicializacion el angulo del vector del marciano.
        SWITCH (Gal_Tipo)                         // Inicializacion de los diferentes parametros del marciano segun su tipo.
            CASE 1:
                Gal_MemoriaGraph = Gal_GraficoMarciano1; // Inicializacion del grafico base.
                Gal_Puntos = Gal_PuntosMarciano1;        // Inicializacion de la puntuacion base.
            END

            CASE 2:
                Gal_MemoriaGraph = Gal_GraficoMarciano2;
                Gal_Puntos = Gal_PuntosMarciano2;
            END

            CASE 3:
                Gal_MemoriaGraph = Gal_GraficoMarciano3;
                Gal_Puntos = Gal_PuntosMarciano3;
            END

            CASE 4:
                Gal_MemoriaGraph = Gal_GraficoMarciano4;
                Gal_Puntos = Gal_PuntosMarciano4;
            END

        END
        Graph = Gal_MemoriaGraph+Gal_Fase;       // Calculo de la fase de animacion inicial del marciano.

                                                 // Bucle principal del marcianos
        LOOP                                     // Sistema de animacion del marciano.
            Gal_ContadorFramesAnimacion++;       // Se incrementa el contador de frames del sistema de animacion,
            IF (Gal_ContadorFramesAnimacion == Gal_FramesFaseMarciano) // comprueba si han contado todos los frames,
                Gal_ContadorFramesAnimacion = 0; // si es asi se inicializa el contador de frames,
                Gal_Fase++;                      // se incrementa el contador de fase de la animacion del marciano,
                IF (Gal_Fase==Gal_NFasesMarciano) // comprueba si se ha llegado al final de la animacion,
                    Gal_Fase = 0;                // si es asi se inicializa el contador de fase de la animacion del marciano,
                END
                Graph = Gal_MemoriaGraph+Gal_Fase; // y por ultimo, se cambia el grafico del marciano.
            END
                                                 // Control de posicion del marciano dentro de la formacion.
            SWITCH (Gal_Sentido)                 // Miramos el flag de sentido,
                CASE Gal_SentidoDerecha:         // si el sentido es a la derecha,
                    Gal_Xi++;                    // incrementamos un pixel a la derecha, miramos si hemos llegado al limite derecho,
                    IF ((Gal_Xi>Gal_LimiteMovimientoDerecha) AND (Gal_Mensaje == Gal_MensajeMarcianoWait)) // y si el marciano esta en modo "espera",
                        Gal_ControlId.Gal_Mensaje = Gal_SentidoIzquierda; // comunicamos al proceso Control que cambiamos de sentido.
                    END
                END

                CASE Gal_SentidoIzquierda:       // si el sentido es a la izquierda,
                    Gal_Xi--;                    // decrementamos un pixel a la izquierda, miramos si hemos llegado al limite izquierdo,
                    IF ((Gal_Xi<Gal_LimiteMovimientoIzquierda) AND (Gal_Mensaje == Gal_MensajeMarcianoWait)) // y si el marciano esta en modo "espera",
                        Gal_ControlId.Gal_Mensaje = Gal_SentidoDerecha; // comunicamos al proceso Control que cambiamos de sentido.
                    END
                END
            END

            SWITCH (Gal_Mensaje)                 // Control de la accion a realizar por el marciano.
                CASE Gal_MensajeMarcianoWait:    // modo en formacion esperando iniciar el ataque,
                      X = Gal_Xi;                  // actualizamos su posicion del marciano segun indique el control de posicion en formacion.
                END

               CASE Gal_MensajeMarcianoAtackDerecha,Gal_MensajeMarcianoAtackIzquierda: // Modo inicio de ataque girando por derecha o por izquierda:
                     IF (Gal_Mensaje == Gal_MensajeMarcianoAtackDerecha) // si gira por la derecha,
                         XAdvance(Gal_Angulo,Gal_ModuloGiroMarciano);    // avanza segun el vector formado por el angulo de giro y el modulo de giro,
                         Gal_Angulo -= Gal_AnguloGiroMarciano;           // decrementa el angulo de giro,
                         Angle -= Gal_AnguloGiroMarciano;                // decrementa la variable angle del proceso
                         IF (Gal_Angulo < -90000)                        // si se ha girado hasta un angulo menor de -90�,
                             Gal_ContadorGeneral = Gal_FramesAjusteKamikaze; // se carga el contador general con los frames de ajuste de direccion del modo Kamikaze,
                             Gal_Mensaje = Gal_MensajeMarcianoKamikaze;  // y se inicializa el mensaje del marciano con el modo kamikaze.
                         END
                     ELSE                                                // Si no gira por la derecha, lo hace por la izquierda,
                         XAdvance(Gal_Angulo,Gal_ModuloGiroMarciano);    // avanza segun el vector formado por el angulo de giro y el modulo de giro,
                         Gal_Angulo += Gal_AnguloGiroMarciano;           // incrementa el angulo de giro,
                         Angle += Gal_AnguloGiroMarciano;                // incrementa la variable angle del proceso
                         IF (Gal_Angulo > 270000)                        // si se ha girado hasta un angulo mayor de 270�,
                             Gal_ContadorGeneral = Gal_FramesAjusteKamikaze; // se carga el contador general con los frames de ajuste de direccion del modo Kamikaze,
                             Gal_Mensaje = Gal_MensajeMarcianoKamikaze; // y se inicializa el mensaje del marciano con el modo kamikaze,
                         END
                     END
                     Graph = Gal_MemoriaGraph;     // Se usa la primera fase de animacion del marciano para este modo de ataque.
               END

               CASE Gal_MensajeMarcianoKamikaze: // modo Kamikaze,
                     IF (Gal_ContadorGeneral > 0)  // si no se ha llegado al final de los frames de ajuste de direccion del modo <kamikaze,
                         Gal_Angulo = Near_Angle(Gal_Angulo,Get_Angle(Gal_NaveId),Gal_IncrementoAnguloKamikaze); // se va aproximando el angulo de giro al angulo que hay entre marciano y la nave del jugador,
                         Gal_ContadorGeneral--;    // decrementamos el contador de los frames de ajuste,
                         IF (Gal_ContadorGeneral == 0) // si el contador de los frames de ajuste ha llegado a cero,
                             Gal_DesviacionAngulo = Rand(Gal_MaximaDesviacionAngulo/2, Gal_MaximaDesviacionAngulo); // se obtiene por azar el angulo de desviacion del marciano,
                             IF (Gal_Angulo >= 270000) // Si el angulo de ataque es mayor o igual a 270�,
                                 Gal_Angulo += Gal_DesviacionAngulo; // se a�ade cierto desvio en el angulo de ataque obtenido.
                             ELSE                      // Si es menor a 270�.
                                 Gal_Angulo -= Gal_DesviacionAngulo; // se resta cierto desvio en el angulo de ataque obtenido.
                             END
                         END
                     ELSE                          // Si se ha llegado al final de los frames de ajuste,
                      Gal_IntervaloDisparo++;      // incrementamos el contador del intervalo entre dos disparo,
                      IF (Gal_InterValoDisparo == Gal_IntervaloDisparoMarciano) // si se ha llegado al final de intervalo,
                          Gal_IntervaloDisparo = 0;                             // se inicializa el contador,
                          Disparo(X,Y+Gal_AjusteYDisparoMarciano,Gal_DisparoMarciano); // y se realiza un disparo.
                      END
                     END
                     IF ((Get_Dist(Gal_NaveId) > Gal_DistanNoGiroMarciano) AND (Y < Gal_NavePosicionY)) // Si la distancia entre marciano y la nave, es mayor a la distacia de no giro, y el marciano no ha rebasado la nave aun,
                         Angle = Get_angle(Gal_NaveId)-90000; // Ajusta el angulo del grafico del marciano.
                     END
                     XAdvance(Gal_Angulo,Gal_ModuloAtaqueKamikaze); // Avanza el vector dado entre el angulo del ataque y modulo del ataque,
                     Graph = Gal_MemoriaGraph;                  // Se usa la primera fase de animacion del marciano para este modode ataque.
                     IF (Region_Out(Id,Gal_RegionJuego))        // Si el marciano sale fuera de la region del juego,
                         Gal_Mensaje = Gal_MensajeMarcianoBack; // se inicializa el mensaje del marciano con el modo vuelta a al escuadron,
                         Y = 0;                                 // se inicializa las coordenadas del marciano para que ete aparezca por arriba de la pantalla.
                         X = Gal_Xi;
                     END
               END

               CASE Gal_MensajeMarcianoBack:     // Modo vuelta a formacion.
                     X = Gal_Xi;                   // Sigue su posicion X en la formacion del escudron,
                     Y+=2;                         // mientras baja por la pantalla.
                     Graph = Gal_MemoriaGraph;     // usando la primera fase de animacion del marciano para este modo de accion,
                     IF ((Gal_Yi-Y)<60)            // si esta cerca de su posicion Y en la formacion del escuadron,
                         Angle -= 180000/(60/2);   // el marciano gira sobre si mismo,
                         IF (Abs(Gal_Yi-Y)<2)      // si ya esta lo suficientemente cerca, o esta ya esta dentro de la formacio del escuadron.
                             Y = Gal_Yi;           // este se coloca en su posicion en la formacion del escuadro,
                             Gal_Mensaje = Gal_MensajeMarcianoWait; // se inicializa el mensaje del marciano con el modo en formacion esperando iniciar el ataque,
                             Angle = 0;            // se inicializa el angulo grafico
                             Gal_Angulo = 90000;   // y el angulo de ataque,
                         END
                     ELSE
                         Angle = 180000;           // si aun no ha llegado a su posicion el angulo grafico vale 180�.
                     END
               END

            END

            IF (Collision(Gal_NaveId))           // Si el marciano choca con la nave del jugador,
                Signal(Gal_NaveId,S_KILL);       // destruye a esta,
                BREAK;                           // y el marciano muere en el acto.
            END

            FRAME;                               // esperar un frame.
        END

    ONEXIT                                       // Esto ocurrira cuando el marciano muera.

        IF (Gal_Vidas >= 0)                      // Si al jugador le quedan vidas,
            Explosion(X,Y,Gal_ExplosionMarciano); // se genera una explosion tipo marciano,
            IF (Gal_Mensaje == Gal_MensajeMarcianoWait) // si el marciano esta en modo de formacion esperando iniciar el ataque,
                Gal_Score += Gal_Puntos;         // a�ade a la puntuacion del jugador el valor base del enemigo,
            ELSE                                 // si no,
                Gal_Score += (Gal_Puntos*2);     // A�ade a la puntuacion del jugador el doble del valor base del enemigo.
            END
        END

    END


//***********************************************************************************************************
//              PROCESO DE CONTROL DE LA LOGICA DE LOS MARCIANOS Y FUNCIONES ASOCIADAS
//***********************************************************************************************************


PROCESS Control();                               // Proceso de control de la logica de los marcianos, digamos que es
    PRIVATE                                      // el general que da ordenes a sus tropas.
        Int Gal_Estado;                          // Guarda el estado del proceso de control.
        Int Gal_ContadorFramesControl;           // Contador de frames del proceso de control.
        Int Gal_Linea;                           // linea del escuadron de marcianos en pantalla.
        Int Gal_IdMarciano;                      // Identificador del marciano.
        Int Gal_FlagMuerteNave;                  // flag del control del proceso de la muerte de la nave del jugador.
        Int Gal_Attack[Gal_NFilas-1];
        Int Gal_Aux, Gal_Swap;
        Int Gal_ExistExplosion;
    BEGIN

        Gal_Mensaje = 0;                         // Inicializacion del proceso de control.
        Gal_Estado = Gal_EstadoEsperaInicial;
        Gal_ContadorFramesControl = Gal_FrameEsperaInicioAtack;
        Gal_FlagMuerteNave = FALSE;

        /* Creo array para seleccion de ataque */
        FOR ( Gal_Linea = 0; Gal_Linea < Gal_NFilas; Gal_Linea++ )
            Gal_Attack[ Gal_Linea ] = Gal_Linea;
        END

        LOOP
            SWITCH (Gal_Mensaje)                 // Sistema de recepcion de mensajes externos.
                CASE Gal_SentidoDerecha,Gal_SentidoIzquierda: // Si control ha recibido algun mensaje de cambio de sentido,
                      Gal_Sentido = Gal_Mensaje;   // actualiza la variable global de control del sentido con el mensaje correspondiente,
                      Gal_Mensaje = 0;             // borra el mensaje.
                END

                CASE Gal_EstadoEsperaInicial:    // Si control ha recibido un mensaje de puesta de modo de estado de espera inicial,
                      Gal_Estado = Gal_Mensaje;    // inicia el modo de estado de espera inicial,
                      Gal_Mensaje = 0;             // borra mensaje,
                      Gal_ContadorFramesControl = Gal_FrameEsperaInicioAtack; // inicializa el contador de frames de espera, que tiene que ejecutar control.
                END

            END

            SWITCH (Gal_Estado)                  // Sistema de control de los modos del proceso Control.
                CASE Gal_EstadoEsperaInicial:    // Modo de estado de espera inicial,
                    Gal_ContadorFramesControl--; // decrementa el contador de frames de espera,
                    IF (Gal_ContadorFramesControl == 0) // si se han ejecutado todos los frames de espera,
                        Gal_Estado = Gal_EstadoAtack;   // iniciamos el modo ataque,
                        Gal_ContadorFramesControl = Gal_FramesEsperaAtack; // e inicializamos el contador de frames.
                    END
                END

                CASE Gal_EstadoAtack:            // Modo ataque (coordina el ataque de los marcianos),
                      IF (NOT Gal_FlagMuerteNave)  // si la nave del jugador no esta muerta,
                        Gal_ContadorFramesControl--; // decrementamos el contador de frames,
                        IF (Gal_ContadorFramesControl == 0) // si se han ejecutado todos los frames,
                            Gal_ContadorFramesControl = Gal_FramesEsperaAtack; // inicializamos el contador de frames,
                            IF (Rand(1,100) <= Gal_PorcentajeAtack AND MarcianoConMensaje(Gal_MensajeMarcianoWait) > 0) // por azar elegimos atacar o no,
                                /* Mezclo array de seleccion de ataque */
                                FOR ( Gal_Linea = 0; Gal_Linea < Gal_NFilas; Gal_Linea++ )
                                    Gal_Swap = Rand(0,Gal_NFilas-1);
                                    Gal_Aux = Gal_Attack[ Gal_Linea ];
                                    Gal_Attack[ Gal_Linea ] = Gal_Attack[ Gal_Swap ];
                                    Gal_Attack[ Gal_Swap ] = Gal_Aux;
                                END
                                Gal_IdMarciano = 0;
                                FOR ( Gal_Aux = 0; Gal_Aux < Gal_NFilas AND Gal_IdMarciano == 0; Gal_Aux++ ) // Bucle de seleccion de marciano,
                                    Gal_Linea = Gal_Attack[ Gal_Aux ]; // por azar seleccionamos una fila de enemigos,
                                    Gal_Linea = Gal_EscuadronPosY+Gal_MarcianoSeparacionY*Gal_Linea; // calculamos la posicion de la fila en pantalla,
                                    IF (Gal_Sentido == Gal_SentidoIzquierda)                         // miramos en que sentido se esta moviendo el escudron, si es a la izquierda,
                                        Gal_IdMarciano = HallarMarcianoExtremo(Gal_Linea,Gal_MarcianoDerecha); // hallamos el marciano que se encuentre mas a la derecha, dentro del escuadron en la fila seleccionada,
                                        IF (Gal_IdMarciano <> 0) // si existe tal marciano,
                                            Gal_IdMarciano.Gal_Mensaje = Gal_MensajeMarcianoAtackDerecha; // le decimos que inicie un ataque a la derecha.
                                        END
                                    ELSE         // Si el escuadron se mueve a la derecha,
                                        Gal_IdMarciano = HallarMarcianoExtremo(Gal_Linea,Gal_MarcianoIzquierda); // hallamos el marciano que se encuentre mas a la izquierda, dentro del escuadron en la fila seleccionada,
                                        IF (Gal_IdMarciano <> 0) // si existe tal marciano,
                                            Gal_IdMarciano.Gal_Mensaje = Gal_MensajeMarcianoAtackIzquierda; // le decimos que inicie un ataque a la izquierda.
                                        END
                                    END
                                END // Si hemos encontrado un marciano apropiado, o
                            END                                                                                     // no hay ningun marciano en formacion dentro del escuadron,
                        END                                                                                         // el bucle de seleccion de marciano se rompe.
                    END
                END
            END

            Gal_ExistExplosion = Exists(TYPE Explosion);

            IF (NOT Exists(Gal_NaveId) AND Gal_ExistExplosion) // Si la nave del jugador no existe, y existe alguna explosion,
                Gal_FlagMuerteNave = TRUE;       // activamos el flag de muerte de la nave.
            END
                                                 // Si no hay ninguna explosion y todos los marcianos estan en formacion y la nave del jugador a muerto,
            IF (Gal_FlagMuerteNave AND NOT Gal_ExistExplosion AND MarcianosMismoMensaje(Gal_MensajeMarcianoWait))
                Gal_Vidas--;                     // Decrementa una vida al jugador,
                IF (Gal_Vidas < 0)               // si no le queda al jugador alguna vida,
                    Signal(TYPE Marciano,S_KILL); // mata a todos los marcianos,
                ELSE                             // si el jugador tiene vidas aun,
                    Gal_NaveId = Nave();         // crea una nueva nave,
                    Gal_FlagMuerteNave = FALSE;  // y borra el flag de muerte de la nave del jugador.
                END
            END

            IF (Gal_Score > Gal_TopScore)        // Si la puntuacion del jugador es mayor que la puntuacion record,
                Gal_TopScore = Gal_Score;        // se iguala la puntuacion record con la puntuacion del jugador.
            END

            IF (NOT Exists(TYPE Marciano))       // Si no queda ningun marciano,
                Gal_ContadorFramesControl = Gal_FrameEsperaInicioAtack; // inicializa el contador de frames,
                REPEAT                           // Bucle de espera, espera los frames indicado en el contador de frame,
                    FRAME;
                    Gal_ContadorFramesControl--;
                UNTIL(Gal_ContadorFramesControl == 0)
                Gal_ContadorFramesControl = Gal_FramesEsperaNivel;
                GeneraEscuadron();               // genera un nuevo escuadron,
                Gal_Nivel++;                     // aumenta un nivel o una fase del juego,
            END

            FRAME;                               // Espera un frame.
        END

    END
                //********************* SUBRUTINAS DE APOYO DEL PROCESO CONTROL ****************************

FUNCTION Int MarcianosMismoMensaje(Int Gal_TipoMensaje); // Si todos los marcianos, tienen el mismo mensaje que el introducido en la funcion, devuelve True,
    PRIVATE                                              // si alguno tiene un mensaje diferente devuelve False.
        Int Gal_IdMarciano;                      // Identificador del marciano.
    BEGIN

        Get_Id(0);                               // Inicializamos la funcion Get_Id, para que nos de los identificadores
                                                 // de todos los marcianos, en las proximas lecturas de Get_Id.
        REPEAT                                   // Bucle de busqueda,
            Gal_IdMarciano = Get_Id(TYPE Marciano); // se obtiene el identificador de un marciano,
        UNTIL((Gal_IdMarciano == 0) OR (Gal_IdMarciano.Gal_Mensaje <> Gal_TipoMensaje)) // Si no hay mas marcianos o el mensaje del marciano es diferente al introducido,
        IF (Gal_IdMarciano == 0)                 // se rompe el bucle, si no habia mas marcianos, es que todos los mensajes eran iguales,
            RETURN(TRUE);                        // por lo tanto el resultado es True,
        ELSE                                     // por lo contrario, si hay algun identificador de marciano, su mensaje no es el mismo que el introducido,
            RETURN(FALSE);                       // Por lo Tanto el resultado es False.
        END

    END


FUNCTION Int MarcianoConMensaje(Int Gal_TipoMensaje); // Busca el primer marciano que tenga el mismo mensaje que el introducido,
    PRIVATE                                           // en caso de no encontrar ninguno devuelve cero.
        Int Gal_IdMarciano;                      // Identificador del marciano.
    BEGIN

        Get_Id(0);                               // Inicializamos la funcion Get_Id, para que nos de los identificadores
                                                 // de todos los marcianos, en las proximas lecturas de Get_Id.

        REPEAT                                   // Bucle de busqueda,
            Gal_IdMarciano = Get_Id(TYPE Marciano); // se obtiene el identificador de un marciano,
        UNTIL((Gal_IdMarciano == 0) OR (Gal_IdMarciano.Gal_Mensaje == Gal_TipoMensaje)) // Si no hay mas marcianos o el mensaje del marciano es igual al introducido,

        RETURN(Gal_IdMarciano); // retorna el identificador del marciano con igualmensaje introducido, o en caso no haber devuelve cero.

    END


FUNCTION Int HallarMarcianoExtremo(Int Gal_Linea,Gal_Extremo); // Halla el marciano que este mas a la derecha, o mas a la izquierda de una linea  determinada de la formacion del escuadron,
    PRIVATE                                                    // el extremo de la linea y la linea donde buscar, se introducen en la funcion.
        Int Gal_IdMemoriaMarciano,Gal_IdMarciano;              // Memoria del identificador e identificador del marciano
    BEGIN

        Gal_IdMemoriaMarciano = 0;
        Get_Id(0);                               // Inicializamos la funcion Get_Id, para que nos de los identificadores
                                                 // de todos los marcianos, en las proximas lecturas de Get_Id.
        LOOP                                     // Bucle de busqueda,
            Gal_IdMarciano = Get_Id(TYPE Marciano); // obtencion del identificador del enemigo,
            IF (Gal_IdMarciano == 0)             // si no hay mas marcianos,
                BREAK;                           // rompe el bucle,
            END
            IF ((Gal_IdMarciano.Y == Gal_Linea) AND (Gal_IdMarciano.Gal_Mensaje == Gal_MensajeMarcianoWait)) // Si el marciano esta a la altura de la linea, y tiene el mensaje de espera en formacion,
                IF (Gal_IdMemoriaMarciano == 0)  // es un marciano valido, si la memoria del identificador esta vacia,
                    Gal_IdMemoriaMarciano = Gal_IdMarciano; // cargamos la memoria del identificador,
                ELSE                             // si no es asi, es que hay un identificador antiguo,
                    IF (Gal_Extremo == Gal_MarcianoDerecha) // si buscamos el marciano que este mas a la derecha,
                        IF (Gal_IdMarciano.X > Gal_IdMemoriaMarciano.X) // miramos que el ultimo marciano obtenido, este mas a la derecha, comparandolo con el marciano de la memoria
                            Gal_IdMemoriaMarciano = Gal_IdMarciano; // si es asi lo guardamos en la memoria del identificador.
                        END
                    ELSE                         // Si buscamos el marciano que este mas a la izquierda,
                        IF (Gal_IdMarciano.X < Gal_IdMemoriaMarciano.X) // miramos que el ultimo marciano obtenido, este mas a la Izquierda, comparandolo con el marciano de la memoria
                            Gal_IdMemoriaMarciano = Gal_IdMarciano;  // si es asi lo guardamos en la memoria del identificador.
                        END
                    END
                END
            END
        END
        RETURN(Gal_IdMemoriaMarciano);           // retornando asi, de la linea deseada, el marciano que este mas a la derecha,
                                                 // o mas a la izquierda, segun se halla pedido
    END


//***********************************************************************************************************
//                                 PROCESO DE LA NAVE DEL JUGADOR
//***********************************************************************************************************


PROCESS Nave();                                  // Proceso de la nave del jugador.
    PRIVATE
        Int Gal_IdDisparo;                       // Identificador del disparo.
    BEGIN

        X = Gal_NavePosicionX;                   // Inicializacion de la nave del jugador.
        Y = Gal_NavePosicionY;
        Region = Gal_RegionJuego;
        File = Gal_Graficos;

        LOOP                                     // Bucle principal.
            IF (jkeys_state[_JKEY_RIGHT]) // Si pulsamos la tecla de direccion a la derecha,
                X +=2;                           // incrementamos 2 pixeles a la derecha,
                IF (X>Gal_LimiteNaveDerecha)     // si se ha llegado al limite derecho,
                    X -=2;                       // se decrementa lo incrementado.
                END
            END

            IF (jkeys_state[_JKEY_LEFT]) // Si pulsamos la tecla de diereccion a la izquierda,
                X -=2;                           // decrementamos 2 pixeles a la izquierda,
                IF (X<Gal_LimiteNaveIzquierda)   // si se ha llegado al limite izquierdo,
                    X +=2;                       // se incrementa lo decrementado.
                END
            END

            IF (NOT Exists(Gal_IdDisparo))       // se mira si no existe el disparo,
                Graph = Gal_GraficoNaveA;        // si no existe, la nave aparece con el grafico de la nave con el disparo cargado,
                IF (jkeys_state[_JKEY_A]) // Si pulsamos la tecla del disparo,
                    Gal_IdDisparo = Disparo(X,Y-16,Gal_DisparoNave); // creamos un disparo tipo nave y guardamos su identificador,
                    Graph = Gal_GraficoNaveB;    // la nave aparece con el grafico de la nave sin el disparo.
                END
            END

        FRAME;                                   // Esperamos un frame.
    END

    ONEXIT                                       // Esto ocurrira cuando la nave muera.

        IF (Gal_Vidas >= 0)                      // Si al jugador le quedan vidas aun,
            Explosion(X,Y,Gal_ExplosionNave);    // genera una explosion tipo nave.
        END

    END


//***********************************************************************************************************
//                                 PROCESO SCROLL TIPO GALAXIAN
//***********************************************************************************************************


PROCESS ScrollGalaxian();                        // Proceso de control del scroll tipo Galaxian.
    PRIVATE
        Int Gal_FaseScroll;                      // Fase del scroll.
        Int Gal_ContadorFramesScroll;            // Contador de los frames de la fase del scroll.
        Int Gal_FondoId[Gal_FondoScrollFinal-1]; // Identificadores de los mapas graficos de los fondos del scroll.
    BEGIN
                                                 // **** Nota: utilizamos estas variables como controladoras de bucle
                                                 // de forma temporal, para ahorrar variables. ****
                                                 // Generacion de los fondos en memoria del scroll tipo galaxian.
        FROM Gal_ContadorFramesScroll = 0 TO Gal_FondoScrollFinal-1; // bucle para creacion de los fondos de scroll,
            Gal_FondoId[Gal_ContadorFramesScroll] = Map_New(Gal_AnchoFondoScroll,Gal_AltoFondoScroll); //se crea un fondo de scroll, y se guarda su codigo identificador,
//            Drawing_Map(0,Gal_FondoId[Gal_ContadorFramesScroll]); // Se escoge el fondo recien creado, para que se dibujen las primitivas graficas,
            FROM Gal_FaseScroll = 0 TO 99;        // bucle de dibujado de los circulos que simulan estrellas en los fondos,
//                Drawing_Color(Rand(32750,65535)); // se elige un color al azar,
//                Draw_FCircle(Rand(0,Gal_AnchoFondoScroll),Rand(0,Gal_AltoFondoScroll),2); // se dibuja un circulo relleno para simular una estrella.
            END
        END

        Gal_FaseScroll = Gal_FondoScrollInicial; // Inicializacion de variables.
        Gal_ContadorFramesScroll = 0;
        Scroll_Start(0,0,Gal_FondoId[Gal_FaseScroll-1],0,0,Gal_RegionJuego,2); // Inicia el Scroll por primera vez con el primer fondo,

        LOOP                                                  // entramos en el bucle de control del scroll
            Gal_ContadorFramesScroll++;                       // incrementa el contadorm de frames,
            IF (Gal_ContadorFramesScroll == Gal_FramesScroll) // mira si se ha contado todos los frames,
                Gal_ContadorFramesScroll = 0;                 // si es asi inicializamos el contador de frames,
                Gal_FaseScroll++;                             // incrementamos el contador de fases,
                IF (Gal_FaseScroll>Gal_FondoScrollFinal)      // miramos si hemos mostrado todas las fases del scroll,
                    Gal_FaseScroll = Gal_FondoScrollInicial;  // si es asi, inicializamos el contador de fases,
                END
                Scroll_Stop(0);                               // Paramos el scroll con el fondo anterior,
                Scroll_Start(0,0,Gal_FondoId[Gal_FaseScroll-1],0,0,Gal_RegionJuego,2); // y lo volvemos a reiniciar con el fondo nuevo.
            END
            Scroll[0].Y0 -=Gal_VelocidadScroll;  // Movemos el scroll.
            FRAME;                               // Esperamos que pase un frame.
        END

    ONEXIT                                       // Esto ocurrira cuando se destruya el proceso ScrollGalaxian.

        Scroll_Stop(0);                          // cuando se destruya el Scroll tipo Galaxian se para el scroll,
        Map_Unload(0,Gal_FondoId[0]);            // y se descargan los fondos creados en memoria.
        Map_Unload(0,Gal_FondoId[1]);
        Map_Unload(0,Gal_FondoId[2]);

    END


//********************************************************************************************************
//                                 PROCESO PARA MOSTRAR LAS VIDAS DEL JUGADOR
//********************************************************************************************************


PROCESS MuestraVidas();                          // proceso de mostrado de vidas
    PRIVATE
        Int Gal_MemoriaNVidas;                   // Memoria del numero de vidas.
        Int Gal_VidaId;                          // Identificador del mapa del grafico de la vida.
        Int Gal_MapaId;                          // Identificador del mapa del proceso MuestraVidas.
        Int Gal_ContadorGeneral;                 // Contador de uso general.
    BEGIN

        Gal_MemoriaNVidas = -1;                  // se inicializa la memoria de vidas.
        Gal_VidaId = Map_Clone(Gal_Graficos,Gal_GraficoVida); // Se clona el grafico de la vida de la libreria del juego a la libreria 0.
        Gal_MapaId = Map_New(Gal_AnchoMapaMuesVidas,Gal_AltoMapaMuesVidas); // Se crea el mapa grafico del proceso.
        Region = Gal_RegionJuego;                // Inicializacion del proceso MuestraVidas.
        X = Gal_RegionJuegoX+Gal_AjusteXMapaMuesVidas;
        Y = Gal_AjusteYMapaMuesVidas;
        File = Gal_Graficos;
        Graph = Gal_MapaId;

        LOOP                                    // Bucle principal
            IF (Gal_MemoriaNvidas<>Gal_Vidas)   // Si el numero de vidas que tiene el jugador a cambiado,
                Gal_MemoriaNVidas = Gal_Vidas;  // se actualiza la memoria de las vidas,
                Map_Clear(0,Gal_MapaId,0);      // se borra el mapa del proceso,
                IF (Gal_MemoriaNVidas < Gal_VidasMaximaMostrar) // si las vidas actuales son inferiores a la cantidad de vidas que se puede mostrar,
                    FROM Gal_Contadorgeneral = 0 TO Gal_MemoriaNVidas-1; // bucle de dibujado de vidas cuando la cantidad a dibujar es menor a la maxima,
                        Map_Put(0,Gal_MapaId,0,Gal_VidaId,Gal_AjusteXGraficoVida+Gal_ContadorGeneral*Gal_SeparacionXGraficoVida,Gal_AjusteYGraficoVida); //se dibuja la vida dentro del mapa del proceso
                    END
                ELSE                             // si las vidas actuales son iguales o mayores a la cantidad de vidas que se puede mostrar,
                    FROM Gal_Contadorgeneral = 0 TO Gal_VidasMaximaMostrar-1; // bucle de dibujado de vidas cuando la cantidad a dibujar es igual o mayor a la maxima,
                        Map_Put(0,Gal_MapaId,0,Gal_VidaId,Gal_AjusteXGraficoVida+Gal_ContadorGeneral*Gal_SeparacionXGraficoVida,Gal_AjusteYGraficoVida); //se dibuja la vida dentro del mapa del proceso
                    END
                END
            END
            FRAME;                               // Esperamos que pase un frame.
        END

    ONEXIT                                       // Esto ocurrira cuando se destruya el proceso MuestraVida.

        Map_Unload(0,Gal_VidaId);                // Descargar mapas graficos de la memoria
        Map_Unload(0,Gal_MapaId);

    END


//********************************************************************************************************
//                                 PROCESO GALAXIAN
//********************************************************************************************************


PROCESS MuestraNivel();                          // Proceso de mostrado del nivel.
    PRIVATE
        Int Gal_MemoriaNivel;                    // Memoria del numero de nivel.
        Int Gal_Banderin1Id;                     // Identificador del mapa grafico del banderin de unidades.
        Int Gal_Banderin10Id;                    // Identificador del mapa grafico del banderin de decenas.
        Int Gal_ConteoId;                        // Identificador del mapa temporal de los graficos del conteo.
        Int Gal_MapaId;                          // Identificador del mapa del proceso MuestraNivel.
        Int Gal_Resultado;                       // resultado de los calculos realizados en el proceso.
    BEGIN

        Gal_MemoriaNivel = -1;                   // se inicializa la memoria de nivel.
        Gal_Banderin1Id = Map_Clone(Gal_Graficos,Gal_GraficoBanderin); // Se clona el grafico del banderin de unidades de la libreria del juego a la libreria 0.
        Gal_Banderin10Id = Map_Clone(Gal_Graficos,Gal_GraficoBanderin10); // Se clona el grafico del banderin de decenas de la libreria del juego a la libreria 0.
        Gal_MapaId = Map_New(Gal_AnchoMapaMuesNivel,Gal_AltoMapaMuesNivel); // Se crea el mapa grafico del proceso.
        Region = Gal_RegionJuego;                // Inicializacion del proceso.
        X = Gal_RegionJuegoX+Gal_AjusteXMapaMuesNivel;
        Y = Gal_AjusteYMapaMuesNivel;
        File = Gal_Graficos;
        Graph = Gal_MapaId;

        LOOP                                     // Bucle principal.
            IF (Gal_MemoriaNivel <> Gal_Nivel)   // Si el numero de nivel a cambiado,
                Gal_MemoriaNivel = Gal_Nivel;    // actualiza la memoria de nivel,
                Map_Clear(0,Gal_MapaId,0);       // borra el mapa grafico del proceso,
                Gal_Resultado = Gal_MemoriaNivel MOD Gal_Banderin1Mostrar; // se calcula unidades del conteo,
                IF (Gal_Resultado <> 0)          // si el valor de las unidades es diferente a cero,
                    Map_Put(0,Gal_MapaId,0,Gal_Banderin1Id,Gal_AjusteXGraficoBanderin1,Gal_AjusteYGraficoBanderin1); // ponemos el banderin de unidades,
                    Gal_ConteoId = Map_Clone(Gal_Graficos,Gal_GraficoConteo+Gal_Resultado-1); // clonamos el grafico temporal del conteo necesario para representar las unidades,
                    Map_Put(0,Gal_MapaId,0,Gal_ConteoId,Gal_AjusteXGraficoConteo1,Gal_AjusteYGraficoConteo1); // ponemos el grafico del conteo en el mapa del proceso,
                    Map_Unload(0,Gal_ConteoId);  // descargamos el grafico temporal del conteo de la memoria.
                END
                Gal_Resultado = (Gal_MemoriaNivel/Gal_Banderin1Mostrar) MOD Gal_Banderin10Mostrar; // Se calcula las decenas del conteo,
                IF (Gal_Resultado <> 0)          // si el valor de las decenas es diferente a cero,
                    Map_Put(0,Gal_MapaId,0,Gal_Banderin10Id,Gal_AjusteXGraficoBanderin10,Gal_AjusteYGraficoBanderin10); // ponemos el banderin de las decenas,
                    Gal_ConteoId = Map_Clone(Gal_Graficos,Gal_GraficoConteo+Gal_Resultado-1); // clonamos el grafico temporal del conteo necesario para representar las decenas,
                    Map_Put(0,Gal_MapaId,0,Gal_ConteoId,Gal_AjusteXGraficoConteo10,Gal_AjusteYGraficoConteo10); // ponemos el grafico del conteo en el mapa del proceso,
                    Map_Unload(0,Gal_ConteoId); // descargamos el grafico temporal del conteo de la memoria.
                END
            END
            FRAME;                               // Esperamos que pase un frame.
        END

    END


//********************************************************************************************************
//                                 PROCESO GALAXIAN
//********************************************************************************************************


PROCESS Galaxian_main();                              // Proceso principal galaxian.
    BEGIN

        Write_Delete(ALL_TEXT);                   // Borramos todos los textos residuales del menu de seleccion de juegos.
        IniciaPrograma();                        // Inicializamos el juego.
        Gal_TopScore = 0;                        // Inicializamos el record de puntuacion.
        Gal_ScrollId = ScrollGalaxian();         // Iniciamos el scroll del galaxian.

        TextosInicioJuego();                     // Ponemos los textos de la presentacion del juego.

        WHILE (NOT(jkeys_state[_JKEY_MENU])) // Mientras no se pulse la tecla de escape no se saldra del bucle.
            IF (jkeys_state[_JKEY_SELECT])     // si pulsamo la tecla enter entramos en el juego,
                Write_Delete(ALL_TEXT);           // borramos todos los textos,
                Gal_Sentido = Gal_SentidoDerecha;// inicializamos las variables del juego,
                Gal_Vidas = Gal_NVidas-1;
                Gal_Score = 0;
                Gal_Nivel = 1;
                Write(Gal_Fuente,200,0,ALIGN_TOP_LEFT,"1UP          HIGH SCORE"); // ponemos los textos de la partida,
                Write_Var(Gal_Fuente2,235,22,ALIGN_TOP,Gal_Score);
                Write_Var(Gal_Fuente2,355,22,ALIGN_TOP,Gal_TopScore);
                Gal_MuestraVidasId = MuestraVidas(); // creamos el proceso MuestraVidas(),
                Gal_MuestraNivelId = MuestraNivel(); // creamos el proceso MuestraNivel(),
                GeneraEscuadron();                   // generamos el escuadron de marcianos,
                Gal_ControlId = Control();           // creamos el proceso de control,
                Gal_NaveId = Nave();                 // creamos la nave del jugador,

                WHILE (NOT(jkeys_state[_JKEY_MENU]) AND Gal_Vidas>=0) // si se pulsa la tecla de escape, o las vidas del jugador se agotan (se acaba la partida),
                    FRAME;                           // se rompe el bucle
                END

                Gal_Vidas = -1;                  // inicializamos las vidas del jugador con un valor negativo, para evitar que aparezcan las explosiones cuando matemos los procesos,
                Write_Delete(ALL_TEXT);           // borramos todos los textos,
                Signal(TYPE Marciano,S_KILL);    // matamos los procesos participantes en la partida,
                Signal(Gal_ControlId,S_Kill);
                Signal(Gal_NaveId,S_KILL);
                Signal(Gal_MuestraVidasId,S_KILL);
                Signal(Gal_MuestraNivelId,S_KILL);
                Signal(TYPE Disparo,S_KILL);
                Signal(TYPE Explosion,S_KILL);
                WHILE (jkeys_state[_JKEY_MENU]) // no se sale de este bucle, hasta que no se deje de pulsar la tecla de escape,
                    FRAME;                       // con esto se evita que al pulsar el escape para salir de la partida, se salga totalmente del juego,
                END
                TextosInicioJuego();             // Ponemos de nuevo los textos de la presentacion del juego,
            END
            FRAME;                               // esperamos que pase un frame.
        END

        While (jkeys_state[_JKEY_MENU])
          Frame;
        End

        let_me_alone();  // Nos aseguramos de que este sea el unico proceso funcionando
        Write_Delete(all_text); // borramos todos los textos

    ONEXIT                                       // Finalizacion del proceso principal

        Signal(Gal_ScrollId,S_KILL);             // Matamos el proceso ScrollGalaxian.
        //Write_Delete(ALL_TEXT);                   // Borramos todos los textos.
        Fpg_Unload(Gal_Graficos);                // descargamos los graficos del juego.

    END
