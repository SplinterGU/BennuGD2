 /*
  programme:  Mrbike + editeur
  Auteur:     Dereumaux H
  Groupe:     Artificial Dreams
  Version:    2.0
  Date:       24/10/2001
*/
program editeur;


import "mod_gfx";
import "mod_input";
import "mod_sound";
import "mod_misc";
import "mod_debug";

declare process static( double x, y, int graph );
declare process selection( double x, y, int idd );
declare process feu( double x, y, int graph );

const
    max = 2500; // longeur parcour
    screen_x = 640;
    screen_y = 480;

#define init_bgd1_background_emulation() background.file = 0; background.graph = map_new( screen_x, screen_y )
#define put_screen( f, g ) map_clear( 0, background.graph ); map_put( 0, background.graph, f, g, screen_x / 2, screen_y / 2 )
#define clear_screen() map_clear( 0, background.graph )
#define put( f, g, x, y ) map_put( 0, background.graph, f, g, x, y )
#define map_xput( fdst, gdst, gsrc, x, y, angle, size, flags ) map_put( fdst, gdst, fdst, gsrc, x, y, angle, size, size, flags, 255, 255, 255, 255 )
#define xput(f,g,x,y,angle,size,flags,region) map_put(0, background.graph, f, g, x, y, angle, size, size, flags, 255, 255, 255, 255)
#define set_text_color(color) rgba_get(color, &text.color_r,  &text.color_g,  &text.color_b, &text.alpha);
#define get_pixel(x,y) map_get_pixel( 0, background.graph, x, y )

#define input_new(x)    ""
#define input_delete(t) if ( t <> "" ) t = substr( t, 0, strlen( t ) - 1 ); end
#define input_add(t,a)  t = t + chr(a)

global
    int id_menu; // id du menu editeur
    int id_menu_a; // id du menu script
    int id_level; // id du fpg level
    int id_editeur; // id du fpg editeur
    int id_mbike; // id du fpg des sprites
    int id_fntA; // font du temps
    int id_fntB; // font du score
    int id_fntc; // font de la vitesse
    int id_fntD; // font des titres et scores
    int id_col; // test des collisions 0 non /1 oui
    int status; // status des boutons
    int select; // selection d'une case a cochet
    int valeur_step[ 5 ] = 1, 5, 10, 50, 100, 250; // valeur pour step
    int index_step; // index du tableau step
    int lenght; // longeur de la portion de route
    int courbe; // valeurs de la courbe
    int niveau_sol; // valeurs du niveau sol
    int id_txt[ 10 ]; // liste des id texte
    // 0 = step
    // 1 = lenght
    // 2 = courbe
    // 3 = niveau
    // 4 = position
    // 5 = road largeur route
    // 6 = graphique decor
    // 7 = step graphique
    // 8 = longeur graphique
    // 9 = position X graphique
    //10 = world
    struct level;
        int32 monde;
        struct circuit[ max ]; // longeur du circuit
            int32 pos_x; // virage
            int32 pos_Y; // montee
            int32 graf[ 3 ]; // graphique
            int32 dX[ 3 ]; // position
        end
    end
    struct data_base; // base de donnees
        struct race[ 4 ]; // liste des cources
            int32 level[ 4 ]; // niveau
        end
        struct track[ 49 ]; // liste des parcour
            int32 nom[ 8 ]; // leur noms
        end
    end
    struct regen[ 112 ]; // Tableau contenant les coords route
        int pos_x; // position X
        int pos_Y; // position Y
        int taille; // taille du block
    end
    struct icon[ 34 ];
        int gx;
        int gy;
        int graf;
    end =
        0, 0, 0,
        41, -77, 38, // 1 quitter
        0, -40, 2, // 2 Step -
        39, -40, 5, // 3 Step +
        39, -23, 5, // 4 lenght -
        0, -23, 2, // 5 lenght +
        9, -3, 14, // 6 up
        -9, -3, 17, // 7 down
        -19, 17, 8, // 8 right
        -37, 17, 11, // 9 left
        0, 17, 2, //10 left/right --
        39, 17, 5, //11 left/right ++
        -19, 34, 23, //12 niveau moin
        -37, 34, 20, //13 niveau plus
        0, 34, 2, //14 niveau --
        39, 34, 5, //15 niveau ++
        7, 181, 35, //16 new
        -37, 181, 26, //17 disk operation
        -49, -85, 41, //18 move menu
        -37, -3, 44, //19 depart
        39, -3, 47, //20 arriver
        0, 54, 2, //21 road -
        39, 54, 5, //22 road +
        0, 74, 2, //23 graph -
        39, 74, 5, //24 graph +
        0, 108, 2, //25 step -
        39, 108, 5, //26 step +
        0, 125, 2, //27 lenght -
        39, 125, 5, //28 lenght +
        0, 142, 2, //29 delta_x -
        39, 142, 5, //30 delta_x +
        -15, 159, 50, //31 put graph
        18, 159, 53, //32 del graph
        0, -59, 2, //33 world -
        39, -59, 5; //34 world +
    string nom[ 50 ] =
        "LEV-00.mbk", "LEV-01.mbk", "LEV-02.mbk", "LEV-03.mbk",
        "LEV-04.mbk", "LEV-05.mbk", "LEV-06.mbk", "LEV-07.mbk",
        "LEV-08.mbk", "LEV-09.mbk", "LEV-10.mbk", "LEV-11.mbk",
        "LEV-12.mbk", "LEV-13.mbk", "LEV-14.mbk", "LEV-15.mbk",
        "LEV-16.mbk", "LEV-17.mbk", "LEV-18.mbk", "LEV-19.mbk",
        "LEV-20.mbk", "LEV-21.mbk", "LEV-22.mbk", "LEV-23.mbk",
        "LEV-24.mbk", "LEV-25.mbk", "LEV-26.mbk", "LEV-27.mbk",
        "LEV-28.mbk", "LEV-29.mbk", "LEV-30.mbk", "LEV-31.mbk",
        "LEV-32.mbk", "LEV-33.mbk", "LEV-34.mbk", "LEV-35.mbk",
        "LEV-36.mbk", "LEV-37.mbk", "LEV-38.mbk", "LEV-39.mbk",
        "LEV-40.mbk", "LEV-41.mbk", "LEV-42.mbk", "LEV-43.mbk",
        "LEV-44.mbk", "LEV-45.mbk", "LEV-46.mbk", "LEV-47.mbk",
        "LEV-48.mbk", "LEV-49.mbk", "LEV-50.MBK";
    string nom_script[ 8 ] =
        "RACE A", "RACE B", "RACE C", "RACE D", "RACE E",
        "MUSIC A", "MUSIC B", "MUSIC C", "MUSIC D";
    int position; // position sur la route
    int road; // largeur de la route 0 - 300 pas de 5
    int graphique; // decor selectionner pour le decor
    int step_B; // decalage des decors
    int lenght_B; // longeur des decors
    int delta_x; // decalage des decors
    int droite; // index si decor a droite ou a gauche
    int niveau; // nombre de niveau
    int world; // graphique arriere plan
    struct moto[ 10 ]; // Tableau pour les concurrents
        int pos_x; // Position X sur la route
        int km; // km parcouru
        int image; // image du motard
        int d_angle; // angle du zigzag
    end
    struct bike[ 7 ]; // listes des concurrents
        int image[ 8 ]; // images correspondantes
    end =
        34, 33, 32, 31, 30, 31, 32, 33, 34,
        39, 38, 37, 36, 35, 36, 37, 38, 39,
        44, 43, 42, 41, 40, 41, 42, 43, 44,
        49, 48, 47, 46, 45, 46, 47, 48, 49,
        54, 53, 52, 51, 50, 51, 52, 53, 54,
        59, 58, 57, 56, 55, 56, 57, 58, 59,
        64, 63, 62, 61, 60, 61, 62, 63, 64,
        69, 68, 67, 66, 65, 66, 67, 68, 69;
    int heros_x; // position X du joueur
    int heros_status; // status du joueur
    int pos_sol; // position Y du sol
    int index_sol; // index du sol dans la table regen
    int mode; // parametre pour type de PC 0-slow 5-fast
    int back_x; // position X du decor en arriere plan
    int back_x1; // position X du decor en arriere plan
    int back_y; // position Y de l'arriere plan (montee/descente)
    int score; // le score du joueur
    int temps; // temps restant
    int vitesse; // vitesse du joueur
    int n_script; // numero du script selectionner
    int p_script; // position dans le script
    int id_sounda;
    int id_soundb;
    int id_soundc;
    int id_soundd;
    int id_sounde;
    int id_soundf;
    int channela;
    int channelb;
    int channelc;
    int channeld;
    int channele;
    int id_music;
    string list_music[ 3 ] =
                            "pcm/music1.wav",
                            "pcm/music2.wav",
                            "pcm/music3.wav",
                            "pcm/music4.wav";
    int volume_music;
    int volume_effect;
    struct top_score[ 50 ];
        int32 nom[ 7 ];
        int32 valeur;
    end
    int global_id; // variable pour le scrolling des score

local
    int px;
    int py;
    int i;
    int j;
    int k;

private
    int id_f1;
    int id_f2;
    int id_f3; // id des feux rouges
    string texte = "";
    int id_texte; // entrer du nom dans le score
begin
    set_mode( 640, 480 );

    init_bgd1_background_emulation();

    id_fntA = fnt_load( "fnt/temps.fnt" ); // charger fnt
    id_fntB = fnt_load( "fnt/score.fnt" ); // charger fnt
    id_fntC = fnt_load( "fnt/vites.fnt" ); // charger fnt
    id_fntD = fnt_load( "fnt/titre.fnt" ); // charger fnt
    id_mbike = fpg_load( "fpg/mbike.fpg" );
    id_editeur = fpg_load( "fpg/editeur.fpg" );
    id_level = fpg_load( "fpg/graphic.fpg" );

    sound.channels=32;
    soundsys_init();

    id_sounda = sound_load( "pcm/beep.wav" );
    id_soundb = sound_load( "pcm/moto.wav" );
    id_soundd = sound_load( "pcm/car2.wav" );
    id_sounde = sound_load( "pcm/coche6.wav" );
    id_soundf = sound_load( "pcm/crash.wav" );

    load( "script.mbk", data_base );
    load( "score.mbk", top_score );
    //--------------------------------------------
/*
from i= 1 to 50;
  top_score[i].valeur=15300-(i*300);
end
save("score.mbk",offset top_score,sizeof (top_score));
*/
    //--------------------------------
    // preparation des tailles pour le tableau regen
    from i = 0 to 112;
        size = 5 + ( i * size / 100 );
        regen[ i ].taille = size;
    end
    from i = 0 to max;
        if ( position < 15 )
            level.circuit[ i ].graf[ 0 ] = 20;
        else
            level.circuit[ i ].graf[ 0 ] = 21;
        end
        position++;
        if ( position >= 30 )
            position = 0;
        end
    end
/*
  -----------------------------------------------
  Introduction
  -----------------------------------------------
*/
    file = id_mbike;
    volume_music = 256;
    volume_effect = 200;
    loop
        set_fps( 40, 0 ); // frame / sec
        write( ID_fntb, 320, 100, 1, "ARTIFICIAL DREAMS" );
        write( id_fntb, 320, 150, 1, "PRESENTS" );
        write( 0, 320, 440, 1, "Graphiques: Dereumaux H" );
        write( 0, 320, 430, 1, "Musique:    Doct. Awesome" );
        write( 0, 320, 420, 1, "Programmation: Dereumaux H." );
        write( 0, 320, 460, 1, "E-Mail:DEREUMAUX_HERVE@HOTMAIL.COM" );
        status = 0;
        lettre( 980, 185 );
        repeat;
            frame;
            if ( key( _esc ) )
                let_me_alone();
                exit();
            end
        until ( status == 6 );
        write( id_fntb, 320, 280, 1, "FULL VERSION" );
/*
  -----------------------------------------------
  phase 0: Selectionner le mode
  -----------------------------------------------
*/
        static( 100, 360, 990 );
        static( 540, 360, 991 );
        from i = 0 to 4;
            selection( 200 + ( 50 * i ), 360, i );
        end
        timer[ 0 ] = 0;
        repeat // boucle d'attente
            frame; // affichage
            if ( key( _esc ) )
                let_me_alone();
                exit();
            end
            if ( key( _left ) and mode > 0 )
                mode--;
                REPEAT;
                    FRAME;
                until ( keyboard.scan_code == 0 );
            end
            if ( key( _right ) and mode < 4 )
                mode++;
                REPEAT;
                    FRAME;
                until ( keyboard.scan_code == 0 );
            end
            if ( timer[ 0 ] > 1000 ) break;
            end // si temp trop long
        until ( key( _enter ) or key( _f1 ) );
        switch ( mode )
            case 0:
                set_fps( 50, 0 );
            end
            case 1:
                set_fps( 25, 0 );
            end
            case 1:
                set_fps( 12, 0 );
            end
            case 3:
                set_fps( 50, 0 );
            end
            case 4:
                set_fps( 25, 0 );
            end
        end
        write_delete( all_text ); // effacer les options
        let_me_alone();
        position = 1;
        // -------------------------------------------
        // l'editeur de jeu
        // -------------------------------------------
        if ( key( _f1 ))
            mouse.file = id_editeur;
            mouse.graph = 999;
            mouse.size = 100;
            id_menu = menu();
            from i = 1 to 34;
                button( i, icon[ i ].gx, icon[ i ].gy, icon[ i ].graf );
            end
            button_b( -1, -11, 91 );
            button_b( 1, 39, 91 );
            position = 0;
            index_step = 0;
            courbe = 1;
            niveau_sol = 1;
            lenght = 1;
            graphique = 22;
            file = id_level;
            x = 320;
            y = 475;
            graph = graphique;
            size = 50;
            step_b = 1;
            lenght_b = 1;
            droite = 1;
            world = 100;
            level.monde = world;
            back_y = screen_y / 2;
            heros_x = 0;
            redraw();
            loop
                repeat;
                    frame;
                    if ( key( _up ))
                        status = 6;
                    end
                    if ( key( _down ))
                        status = 7;
                    end
                    if ( key( _left ))
                        heros_x -= 5;
                        redraw();
                    end
                    if ( key( _right ))
                        heros_x += 5;
                        redraw();
                    end
                until ( status <> 0 );
                repeat;
                    frame;
                until ( mouse.left == false );
                //-----------------------------------------------
                switch ( status );
                    //-----------------------------------------------
                    case 1: // quitter l'editeur
                        write_delete( all_text );
                        let_me_alone();
                        clear_screen();
                        mouse.size = 0;
                        break;
                    end
                    //-----------------------------------------------
                    case 2:
                        if ( index_step > 0 )
                            index_step--;
                        end
                        write_delete( id_txt[ 0 ] );
                        id_txt[ 0 ] = write_var( 0, id_menu.x + 19, id_menu.y -40, 4, valeur_step[ index_step ] );
                    end
                    //-----------------------------------------------
                    case 3:
                        if ( index_step < 5 )
                            index_step++;
                        end
                        write_delete( id_txt[ 0 ] );
                        id_txt[ 0 ] = write_var( 0, id_menu.x + 19, id_menu.y -40, 4, valeur_step[ index_step ] );
                    end
                    //-----------------------------------------------
                    case 5:
                        if ( lenght > valeur_step[ index_step ] )
                            lenght -= valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 4:
                        if ( lenght + valeur_step[ index_step ] < max )
                            lenght += valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 6:
                        if ( position + lenght <= max )
                            position += lenght;
                            redraw();
                        end
                    end
                    //-----------------------------------------------
                    case 7:
                        if ( position - lenght >= 0 )
                            position -= lenght;
                            redraw();
                        end
                    end
                    //-----------------------------------------------
                    case 8:
                        i = position;
                        j = 0;
                        repeat
                            level.circuit[ i ].pos_x = courbe;
                            i++;
                            j++;
                        until ( j == lenght or i == max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 9:
                        i = position;
                        j = 0;
                        repeat
                            level.circuit[ i ].pos_x = - courbe;
                            i++;
                            j++;
                        until ( j == lenght or i == max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 10:
                        if ( courbe > 0 )
                            courbe--;
                        end
                    end
                    //-----------------------------------------------
                    case 11:
                        if ( courbe < 4 )
                            courbe++;
                        end
                    end
                    //-----------------------------------------------
                    case 12:
                        i = position;
                        j = 0;
                        repeat
                            level.circuit[ i ].pos_y = niveau_sol;
                            i++;
                            j++;
                        until ( j == lenght or i == max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 13:
                        i = position;
                        j = 0;
                        repeat
                            level.circuit[ i ].pos_y = - niveau_sol;
                            i++;
                            j++;
                        until ( j == lenght or i == max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 14:
                        if ( niveau_sol > 0 )
                            niveau_sol--;
                        end
                    end
                    //-----------------------------------------------
                    case 15:
                        if ( niveau_sol < 2 )
                            niveau_sol++;
                        end
                    end
                    //-----------------------------------------------
                    case 16:
                        from i = 0 to max;
                            level.circuit[ i ].pos_x = 0;
                            level.circuit[ i ].pos_y = 0;
                            from j = 2 to 3;
                                level.circuit[ i ].graf[ j ] = 0;
                                level.circuit[ i ].dx[ j ] = 0;
                            end
                        end
                        position = 0;
                        redraw();
                    end
                    //-----------------------------------------------
                    case 17:
                        id_menu = Operation_disk();
                        repeat;
                            frame;
                        until ( status == -1 );
                        position = 0;
                        redraw();
                    end
                    //-----------------------------------------------
                    case 18:
                        repeat
                            id_menu.x = mouse.x;
                            id_menu.y = mouse.y + 80;
                            frame;
                        until ( mouse.left );
                    end
                    //-----------------------------------------------
                    case 19:
                        position = 0;
                        redraw();
                    end
                    //-----------------------------------------------
                    case 20:
                        position = max;
                        redraw();
                    end
                    //-----------------------------------------------
                    case 21:
                        if ( road >= 5 )
                            road -= 5;
                            delta_x -= 5;
                            i = position;
                            j = 0;
                            repeat
                                level.circuit[ i ].dx[ 0 ] = - road;
                                level.circuit[ i ].dx[ 1 ] = road;
                                level.circuit[ i ].graf[ 1 ] = level.circuit[ i ].graf[ 0 ];
                                i++;
                                j++;
                            until ( j == lenght or i == max );
                            redraw();
                        end
                    end
                    //-----------------------------------------------
                    case 22:
                        if ( road < 300 )
                            road += 5;
                            delta_x += 5;
                            i = position;
                            j = 0;
                            repeat
                                level.circuit[ i ].dx[ 0 ] = - road;
                                level.circuit[ i ].dx[ 1 ] = road;
                                level.circuit[ i ].graf[ 1 ] = level.circuit[ i ].graf[ 0 ];
                                i++;
                                j++;
                            until ( j == lenght or i == max );
                            redraw();
                        end
                    end
                    //-----------------------------------------------
                    case 23:
                        if ( graphique > 1 )
                            graphique--;
                        end
                        graph = graphique;
                    end
                    //-----------------------------------------------
                    case 24:
                        if ( graphique < 999 )
                            graphique++;
                        end
                        graph = graphique;
                    end
                    //-----------------------------------------------
                    case 25:
                        if ( step_b > 1 )
                            step_b--;
                        end
                    end
                    //-----------------------------------------------
                    case 26:
                        if ( step_b < 20 )
                            step_b++;
                        end
                    end
                    //-----------------------------------------------
                    case 27:
                        if ( lenght_b >= 5 )
                            lenght_b -= valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 28:
                        if ( lenght_b < max )
                            lenght_b += valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 29:
                        if ( delta_x > -1000 )
                            delta_x -= valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 30:
                        if ( delta_x < 1000 )
                            delta_x += valeur_step[ index_step ];
                        end
                    end
                    //-----------------------------------------------
                    case 31:
                        i = position;
                        j = 0;
                        repeat
                            if ( droite == -1 )
                                level.circuit[ i ].graf[ 2 ] = graphique;
                                level.circuit[ i ].dx[ 2 ] = - delta_x;
                            end
                            if ( droite == 1 )
                                level.circuit[ i ].graf[ 3 ] = - graphique;
                                level.circuit[ i ].dx[ 3 ] = delta_x;
                            end
                            i += step_b;
                            j += step_b;
                        until ( j >= lenght_b or i >= max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 32:
                        i = position;
                        j = 0;
                        repeat
                            if ( droite == -1 )
                                level.circuit[ i ].graf[ 2 ] = 0;
                                level.circuit[ i ].dx[ 2 ] = 0;
                            end
                            if ( droite == 1 )
                                level.circuit[ i ].graf[ 3 ] = 0;
                                level.circuit[ i ].dx[ 3 ] = 0;
                            end
                            i += step_b;
                            j += step_b;
                        until ( j >= lenght_b or i >= max );
                        redraw();
                    end
                    //-----------------------------------------------
                    case 33:
                        if ( world > 100 )
                            world -= 5;
                        end
                        level.monde = world;
                        redraw();
                    end
                    //-----------------------------------------------
                    case 34:
                        if ( world < 999 )
                            world += 5;
                        end
                        level.monde = world;
                        redraw();
                    end
                end
                repeat;
                    frame;
                until ( mouse.left == false );
                status = 0;
            end
            x = 320;
            y = -640;
            graph = 952;
        end
        // -------------------------------------------------
        // le jeu
        // -------------------------------------------------
        // selection d'une course
        //-----------------------------------------------
        if ( key( _enter ))
            file = id_mbike;
            graph = 992;
            size = 100;
            write( id_fntB, 320, 20, 4, "SELECT A RACE" );
            x = 320;
            y = 64;
            j = 0;
            from i = 0 to 4;
                if ( data_base.race[ i ].level[ 0 ] <> -1 )
                    write( id_fntb, 320, y, 4, nom_script[ i ] );
                    y += 32;
                    j++;
                else
                    break;
                end
            end
            status = 0;
            repeat;
                frame;
            until ( key( _enter ) == false );
            x = 200;
            y = 64;
            status = 0;
            repeat
                frame;
                if ( key( _up ) and status > 0 )
                    y -= 32;
                    status--;
                    repeat;
                        frame;
                    until ( key( _up ) == false );
                end
                if ( key( _down ) and status < j -1 )
                    y += 32;
                    status++;
                    repeat;
                        frame;
                    until ( key( _down ) == false );
                end
            until ( key( _enter ));
            n_script = status;
            p_script = 0;
            position = data_base.race[ n_script ].level[ p_script ];
            write_delete( all_text );

            load( nom[ position ], level );
            // selection d'une musique
            //-----------------------------------------------
            graph = 992;
            size = 100;
            write( id_fntB, 320, 20, 4, "SELECT A MUSIC" );
            x = 320;
            y = 64;
            from i = 5 to 8;
                write( id_fntb, 320, y, 4, nom_script[ i ] );
                y += 32;
            end
            status = 0;
            repeat;
                frame;
            until ( key( _enter ) == false );
            x = 200;
            y = 64;
            status = 0;

            id_music = sound_load( list_music[ status ] );
            channelC = sound_play( id_music, -1 );

            repeat
                frame;
                if ( key( _up ) and status > 0 )
                    if ( id_music <> 0 )
                        sound_unload( id_music );
                    end

                    y -= 32;
                    status--;

                    id_music = sound_load( list_music[ status ] );
                    channelC = sound_play( id_music, -1 );

                    repeat;
                        frame;
                    until ( key( _up ) == false );
                end
                if ( key( _down ) and status < 3 )
                    if ( id_music <> 0 )
                        sound_unload( id_music );
                    end

                    y += 32;
                    status++;

                    id_music = sound_load( list_music[ status ] );
                    channelc = sound_play( id_music, -1 );

                    repeat;
                        frame;
                    until ( key( _down ) == false );
                end
            until ( key( _enter ));

            sound_stop( channelc );
            channelD = sound_play( id_soundd, -1 );

            write_delete( all_text );
            heros_status = 1;
            vitesse = 0;
            position = 0;
            heros_x = 0;
            x = -150;
            y = 0;
            from i = 0 to 10;
                moto[ i ].pos_x = x;
                moto[ i ].km = y;
                moto[ i ].image = rand( 0, 7 );
                moto[ i ].d_angle = rand( 0, 90000 );
                x += 50;
                if ( i == 2 )
                    x += 50;
                end
                if ( i >= 4 )
                    x = 0;
                    y = 500;
                end
            end
            moto_heros();
            compteur();
            redraw();
/*
  -----------------------------------------------
  phase le depart
  -----------------------------------------------
*/
            graph = 953;
            x = 320;
            y = 180;
            timer[ 5 ] = 0;
            repeat
                frame;
                i = rand( 0, 3 );

                if ( i == 0 )
                    sound_play( id_soundb );
                end

            until ( timer[ 5 ] >= 150 );
            id_f1 = feu( 287, 180, 954 );

            sound_play( id_sounda );

            timer[ 5 ] = 0;
            repeat
                frame;
                i = rand( 0, 3 );

                if ( i == 0 )
                    sound_play( id_soundb );
                end

            until ( timer[ 5 ] >= 150 );
            id_f2 = feu( 320, 180, 954 );

            sound_play( id_sounda );

            timer[ 5 ] = 0;
            repeat
                frame;
                i = rand( 0, 3 );

                if ( i == 0 )
                    sound_play( id_soundb );
                end

            until ( timer[ 5 ] >= 150 );
            id_f3 = feu( 353, 180, 955 );

            sound_play( id_sounda );

/*
  -----------------------------------------------
  C'est parti
  -----------------------------------------------
*/
            y = -640;
            signal( id_f1, s_kill );
            signal( id_f2, s_kill );
            signal( id_f3, s_kill );
            temps = 70;
            timer[ 0 ] = 0;
            heros_status = 0;
            mad();
            curseur();

            channelc = sound_play( id_music );
            channel_set_volume( channelc, volume_music );
/*
  -----------------------------------------------
  main process
  -----------------------------------------------
*/
            repeat
                redraw();
                frame;

                if ( key( _1 ))
                    volume_music -= 5;
                    channel_set_volume( channelc, volume_music );
                end
                if ( key( _2 ))
                    volume_music += 5;
                    channel_set_volume( channelc, volume_music );
                end
                if ( key( _3 ))
                    volume_effect -= 5;
                    channel_set_volume( channeld, volume_effect );
//                    change_sound( channeld, volume_effect, 256 + ( vitesse * 2 ));
                end
                if ( key( _4 ))
                    volume_effect += 5;
                    channel_set_volume( channeld, volume_effect );
//                    change_sound( channeld, volume_effect, 256 + ( vitesse * 2 ));
                end

                if ( p_script == 5 )
                    sound_stop( channeld );
                    heros_status = 3;
                    write( 2, 320, 160, 1, "CONGRATUTION" );
                    repeat
                        score += 50;
                        temps--;
                        frame;
                    until ( temps == 0 );
                end
            until ( key( _esc ) or temps <= 0 );
/*
  -----------------------------------------------
  fin de la course
  -----------------------------------------------
*/
            sound_stop( channeld );
            x = 320;
            y = 240;
            graph = 952;
            heros_status = 1;
            timer[ 5 ] = 0;
            repeat
                frame;
                channel_set_volume( channelc, volume_music );
                volume_music = -5;
                if ( volume_music <= 0 )
                    i = 0;
                end
            until ( timer[ 5 ] > 200 );
            sound_stop( channelc );
            write_delete( all_text );
            let_me_alone();
            clear_screen();
            y = -640;
/*
  -----------------------------------------------
  memorisation du score
  -----------------------------------------------
*/
            sound_unload( id_music );

            id_music = sound_load( "pcm/music0.wav" );
            channelc = sound_play( id_music, -1 );

            i = 0;
            repeat
                i++;
            until ( score > top_score[ i ].valeur or i >= 50 );
            if ( i <= 50 )
                write( id_fntB, 320, 20, 4, "PLEASE ENTER YOUR NAME:" );
                texte = input_new( 7 );
                id_texte = write_var( id_fntd, 320, 240, 4, texte );
                position = 0;
                from j = 0 to 7;
                    top_score[ i ].nom[ j ] = 0;
                end
                repeat
                    IF ( keyboard.scan_code <> _enter )
                        IF ( keyboard.scan_code == _backspace and position > 0 )
                            input_delete( texte );
                            top_score[ i ].nom[ position ] = 0;
                            position--;
                            keyboard.ascii = 0;
                        ELSE
                            IF ( keyboard.ascii <> 0 )
                                input_add( texte, keyboard.ascii );
                                top_score[ i ].nom[ position ] = keyboard.ascii;
                                position++;
                                keyboard.ascii = 0;
                            END
                        END
                    ELSE
                        BREAK;
                    END
                    FRAME;
                until ( position >= 7 );
                write_delete( all_text );
                top_score[ i ].valeur = score;
                position = i;
                save( "score.mbk", top_score );
            end
        end // fin du test de selection
/*
  -----------------------------------------------
  defillement du score
  -----------------------------------------------
*/

        if ( id_music == 0 )
            id_music = sound_load( "pcm/music0.wav" );
            channelc = sound_play( id_music, -1 );
        end

        file = id_mbike;
        x = 320;
        y = 550;
        graph = 960;
        size = 100;
        z = -300;
        global_id = 1;
        repeat;
            y--;
            if ( y == 470 )
                global_id = 0;
                scroll_position( 50 );
                scroll_txt( 50 );
                scroll_score( 50 );
            end
            if ( y <= 26 )
                y = 26;
            end
            frame;
        until ( global_id == 0 or keyboard.scan_code <> 0 );
        let_me_alone();
        write_delete( all_text );
/* commented by SplinterGU
        sound_stop( id_music );
        sound_unload( id_music );
*/
        y = -600;
    end // fin de la boucle du jeu
end
 // fin du main process
/*
-----------------------------------------------
  menu de l'editeur
-----------------------------------------------
*/

process menu();
begin
    file = id_editeur;
    x = 320;
    y = 240;
    graph = 1;
    px = x;
    py = y;
    id_txt[ 0 ] = write_var( 0, x + 19, y -40, 4, valeur_step[ index_step ] );
    id_txt[ 1 ] = write_var( 0, x + 19, y -23, 4, lenght );
    id_txt[ 2 ] = write_var( 0, x + 19, y + 17, 4, courbe );
    id_txt[ 3 ] = write_var( 0, x + 19, y + 34, 4, niveau_sol );
    id_txt[ 4 ] = write_var( 0, x + 1, y -77, 5, position );
    id_txt[ 5 ] = write_var( 0, x + 19, y + 54, 4, road );
    id_txt[ 6 ] = write_var( 0, x + 19, y + 74, 4, graphique );
    id_txt[ 7 ] = write_var( 0, x + 19, y + 108, 4, step_b );
    id_txt[ 8 ] = write_var( 0, x + 19, y + 125, 4, lenght_b );
    id_txt[ 9 ] = write_var( 0, x + 19, y + 142, 4, delta_x );
    id_txt[ 10 ] = write_var( 0, x + 19, y -59, 4, world );
    repeat
        if ( px <> x or py <> y )
            from i = 0 to 10;
                write_delete( id_txt[ i ] );
            end
            id_txt[ 0 ] = write_var( 0, x + 19, y -40, 4, valeur_step[ index_step ] );
            id_txt[ 1 ] = write_var( 0, x + 19, y -23, 4, lenght );
            id_txt[ 2 ] = write_var( 0, x + 19, y + 17, 4, courbe );
            id_txt[ 3 ] = write_var( 0, x + 19, y + 34, 4, niveau_sol );
            id_txt[ 4 ] = write_var( 0, x + 1, y -77, 5, position );
            id_txt[ 5 ] = write_var( 0, x + 19, y + 54, 4, road );
            id_txt[ 6 ] = write_var( 0, x + 19, y + 74, 4, graphique );
            id_txt[ 7 ] = write_var( 0, x + 19, y + 108, 4, step_b );
            id_txt[ 8 ] = write_var( 0, x + 19, y + 125, 4, lenght_b );
            id_txt[ 9 ] = write_var( 0, x + 19, y + 142, 4, delta_x );
            id_txt[ 10 ] = write_var( 0, x + 19, y -59, 4, world );
            px = x;
            py = y;
        end
        frame;
    until ( status == -3 );
    from i = 0 to 10;
        write_delete( id_txt[ i ] );
    end
end

/*
-----------------------------------------------
  bouton a cliquer de l'editeur
-----------------------------------------------
*/

process button( N, px, py, graf );
begin
    file = id_editeur;
    graph = graf;
    repeat
        x = id_menu.x + px;
        y = id_menu.y + py;
        frame;
        if ( collision( type mouse ))
            graph = graf + 1;
            if ( mouse.left )
                graph = graf + 2;
                status = n;
            end
        else
            graph = graf;
        end
    until ( status == -3 );
end

/*
-----------------------------------------------
  bouton pour selection gauche ou droite
-----------------------------------------------
*/

process button_b( N, px, py );
begin
    file = id_editeur;
    repeat
        x = id_menu.x + px;
        y = id_menu.y + py;
        if ( droite == N )
            graph = 57;
        else
            graph = 56;
        end
        frame;
        if ( collision( type mouse ))
            if ( mouse.left )
                droite = N;
            end
        end
    until ( status == -3 );
end

/*-----------------------------------------------
le principal process, c'est ici tout le moteur du jeu
-----------------------------------------------
*/

process redraw();
    private
        int tab; // position de la route sur l'horizon
        int dd; // position d'un obstacle en fonction du milieu de la route
        int gg; // pour regenerer de la route (graphique)
        int cxx;
        int cyy; // variables pour le calcul des courbes de la route
        int flag; // flags de l'object
        int flagy; // index couleur bord de la route
        int fg; // numero image des biker's
        int ff; // flag axe Y des biker's
begin
    tab = position;
    cxx = 0;
    cyy = 0;
    pos_sol = 480;
    index_sol = 112;
    //les calculs
    from i = 112 to 0 step -1;
        if ( tab < 0 )
            tab += max;
        end
        if ( tab > max )
            tab -= max;
        end
        cxx += level.circuit[ tab ].pos_x;
        cyy += level.circuit[ tab ].pos_Y;
        regen[ i ].pos_x = ( screen_x / 2 ) + cxx + ( heros_x * regen[ i ].taille / 100 );
        regen[ i ].pos_Y = ( screen_y / 2 ) + cYY + regen[ i ].taille;
        if ( regen[ i ].pos_Y < pos_sol )
            pos_sol = regen[ i ].pos_Y;
            index_sol = i;
        end
        tab++;
    end
    // gestion du scrolling arriere plan
    if ( heros_status == 0 )
        if ( regen[ 0 ].pos_y < 245 ) back_y++;
        end
        if ( regen[ 0 ].pos_y > 245 ) back_y--;
        end
        back_x1 -= ( level.circuit[ position ].pos_x ) / 2;
        back_x -= level.circuit[ position ].pos_x;
    end
    if ( back_x < -320 )
        back_x += 320;
    end
    if ( back_x > 320 )
        back_x += 320;
    end
    if ( back_x1 < -320 )
        back_x1 += 320;
    end
    if ( back_x1 > 320 )
        back_x1 += 320;
    end
    if ( back_y < ( screen_y / 2 ) -100 )
        back_y = ( screen_y / 2 ) -100;
    end
    if ( back_y > ( screen_y / 2 ))
        back_y = screen_y / 2;
    end
    // Afficher le fond
    put( id_level, level.monde, back_x1, back_y );
    put( id_level, level.monde, back_x1 + 640, back_y );
    put( id_level, level.monde + 1, back_x, back_y );
    put( id_level, level.monde + 1, back_x + 640, back_y );
    // dessinner a partir de la ligne d'horizon
    tab = position + 112;
    from i = 0 to 112;
        if ( tab < 0 )
            tab += max;
        end
        if ( tab > max )
            tab -= max;
        end
        // pour pc tres lents: un seul block pour le sol
        if ( i == index_sol and mode < 2 )
            put( id_level, level.monde + 4, ( screen_x / 2 ), regen[ i ].pos_y );
        end
        // pour les autres plusieur sections
        if ( flag <> level.circuit[ tab ].graf[ 0 ] and mode >= 2 )
            xput(
                id_level,
                level.monde + level.circuit[ tab ].graf[ 0 ] -18,
                320,
                regen[ i ].pos_Y,
                0,
                100,
                0,
                0
                );
            flag = level.circuit[ tab ].graf[ 0 ];
        end
        // on dessinne la route et les decors
        from j = 0 to 3;
            if ( level.circuit[ tab ].graf[ j ] <> 0 )
                if ( j == 0 or j == 1 )
                    dd = level.circuit[ tab ].dX[ j ] * regen[ i ].taille / 100;
                end
                if ( j == 2 )
                    dd = ( level.circuit[ tab ].dX[ 0 ] + level.circuit[ tab ].dx[ 2 ] ) * regen[ i ].taille / 100;
                end
                if ( j == 3 )
                    dd = ( level.circuit[ tab ].dX[ 1 ] + level.circuit[ tab ].dx[ 3 ] ) * regen[ i ].taille / 100;
                end
                if ( level.circuit[ tab ].graf[ j ] < 0 ) flag = 1;
                else
                    flag = 0;
                end
                xput( // Afficher la route
                    id_level,
                    abs( level.circuit[ tab ].graf[ j ] ),
                    regen[ i ].pos_x + dd,
                    regen[ i ].pos_Y,
                    0,
                    regen[ i ].taille,
                    flag,
                    0
                    );
            end
        end
        // Afficher les autres vehicules
        from j = 0 to 10;
            if ( moto[ j ].km == tab )
                dd = moto[ j ].pos_x * regen[ i ].taille / 100;
                fg = level.circuit[ tab ].pos_x;
                if ( fg < 0 )
                    ff = 1;
                else
                    ff = 0;
                end
                fg = abs( fg ) + 4;
                xput( id_mbike, bike[ moto[ j ].image ].image[ fg ], regen[ i ].pos_x + dd, regen[ i ].pos_Y, 0, regen[ i ].taille / 2, ff, 0 );
            end
        end
        tab--;
    end
end

/*
--------------------------------------
  les lettres animmees de l'intro
--------------------------------------
*/

process lettre( graph, dx );
    private
        int dangle;
        int flag;
begin
    file = id_mbike;
    size = 2;
    x = 60;
    y = 60;
    z = graph;

    channela = sound_play( id_sounde );
    channel_set_volume( channela, volume_effect );

    repeat
        y++;
        frame;
        dangle = ( y -60 ) * 1000;
        x = 60 - get_disty( dangle, 600 );
        if ( size < 100 )
            size++;
        end;
        if ( x - dx >= 60 and flag == 0 )
            lettre( graph + 1, x );
            flag = 1;
        end
    until ( X >= 590 );
    repeat
        y++;
        frame;
        dangle = ( y -60 ) * 1000;
        x = dx - get_disty( dangle, 640 - dx );
        if ( size < 100 )
            size++;
        end;
        if ( x - dx >= 60 and flag == 0 )
            lettre( graph + 1, x );
            flag = 1;
        end
    until ( y == 240 );
    status++;
    loop
        frame;
    end;
end

/*
--------------------------------------
  la tortues et le lievre
--------------------------------------
*/

process static( double x, y, int graph );
begin
    file = id_mbike;
    loop
        frame;
    end
end

/*
--------------------------------------
  les poix rouge et le PC
--------------------------------------
*/

process selection( double x, y, int idd )
begin
    file = id_mbike;
    loop
        frame;
        if ( idd == mode )
            graph = 993;
        else
            graph = 992;
        end
    end
end

/*
--------------------------------------
  les feux rouge
--------------------------------------
*/

process feu( double x, y, int graph );
begin
    file = id_mbike;
    loop
        frame;
    end
end

/*
--------------------------------------
  le curseur indiquant les KM
--------------------------------------
*/

process curseur();
    private
        int dpos;
        int flag;
        int pcx;
begin
    file = id_mbike;
    x = 5;
    y = 5;
    graph = 956;
    dpos = 10;
    from i = 1 to 162;
        from j = 2 to 13;
            map_put_pixel( id_mbike, 956, i, j, 0 );
        end
    end
    pcx = 2;
    loop
        frame;
        dpos = position / 78;
        if ( dpos <> flag )
            map_put( 0, 956, 0, 957, pcx, 8 );
            pcx++;
            flag = dpos;
        end
    end
end

/*
--------------------------------------
  le cadre du compteur kilometrique
--------------------------------------
*/

process compteur();
begin
    file = id_mbike;
    graph = 950;
    x = 560;
    y = 400;
    speed();
    loop
        frame;
    end
end

/*
--------------------------------------
  le calcul de la vitesse et gestion des collisions
--------------------------------------
*/

process speed();
    private
        int col;
        int col0;
        int couleur;
        int dvit;
        int new_level;
        int calcul;
        int dx;
begin
    file = id_mbike;
    graph = 951;
    x = father.x;
    y = father.y;
    angle = 225000;
    write_var( id_fntB, 630, 10, 2, score );
    write_var( id_fntA, 320, 48, 4, temps );
    write_var( id_fntC, 560, 440, 4, vitesse );
    // recherche des couleurs de references
    col = map_get_pixel( id_level, 20, 200, 1 ); // couleur de la route0
    col0 = map_get_pixel( id_level, 21, 200, 1 ); // couleur de la route1
    loop
        // si le heros est libre
        if ( heros_status == 0 )
            if ( key( _up ))
                vitesse += 5;
            else
                vitesse--;
            end // accelerer
            if ( key( _down ))
                vitesse -= 15;
            end // freiner

//            change_sound( channeld, volume_effect, 256 + ( vitesse * 2 ));
            channel_set_volume( channeld, volume_effect );
        end
        // detection du sol
        couleur = get_pixel( 320, 479 );
        if ( couleur <> col and couleur <> col0 )
            vitesse -= 10;
            id_col = 1;
        else
            id_col = 0;
        end
        if ( vitesse < 10 )
            vitesse = 10;
        end
        if ( vitesse > 240 )
            vitesse = 240;
        end
        // orientation de la fleche
        angle = 225000 - ( vitesse * 1000 );
        // si le heros est tombe => retourne sur la route
        if ( heros_status > 0 )
            dvit = 0;
            if ( heros_status == 2 )
                calcul = level.circuit[ position ].dx[ 1 ] -190;
                if ( calcul < 190 and abs( heros_x ) < calcul )
                    if ( heros_x < 0 )
                        dx = -5;
                    end
                    if ( heros_x > 0 )
                        dx = + 5;
                    end
                else
                    if ( heros_x < 0 )
                        dx = + 5;
                    end
                    if ( heros_x > 0 )
                        dx = -5;
                    end
                end
                heros_x += dx;
                if ( id_col == 0 )
                    from i = 0 to 25;
                        heros_x += dx;
                        frame;
                    end
                    heros_status = 0;
                end
            end
        else
            // calcul de la vitesse
            dvit = ( vitesse / 48 ) + 1;
        end
        // les commandes de pilotage
        if ( heros_status == 0 )
            if ( key( _left ))
                heros_x += 15;
            end // tourner a gauche
            if ( key( _right ))
                heros_x -= 15;
            end // tourner a droite
        end
        // calcul du score et du temps restant
        score += dvit;
        if ( timer[ 0 ] >= 100 ) timer[ 0 ] = 0;
            temps--;
        end
        if ( temps < 0 )
            temps = 0;
        end
        // calcul de la distance parcourue
        position += dvit;
        // si on a terminer le parcour
        if ( position > max )
            position -= max;
            temps += 70;
            p_script++;
            if ( p_script < 5 )
                new_level = data_base.race[ n_script ].level[ p_script ];
                if ( new_level <> -1 )
                    load( nom[ new_level ], level );
                else
                    p_script = 5;
                end
            end
        end
        // derapage
        heros_x += ( vitesse * level.circuit[ position ].pos_x ) / 25;
        frame;
    end
end

/*
--------------------------------------
  la gestion de tous les autres motards
--------------------------------------
*/

process mad();
    private
        int dcol; // distance en Z du mad et du heros
        int ddx; // distance en X du heros et du mad
begin
    // au debut, positionement sur la piste
    from i = 6 to 10;
        moto[ i ].km = rand( 200, max );
        moto[ i ].image = rand( 0, 7 );
        moto[ i ].d_angle = rand( 0, 90000 );
    end
    loop
        frame;
        from i = 0 to 10;
            if ( i <= 5 ) // deplacement
                moto[ i ].km += ( i + 1 );
            else
                moto[ i ].km += 3;
            end
            if ( moto[ i ].km > max ) moto[ i ].km -= max;
            end // gestion du circuit
            if ( moto[ i ].km < 0 ) moto[ i ].km += max;
            end
            moto[ i ].pos_x = get_distx( moto[ i ].d_angle, 180 ); // zigzag
            if ( abs( level.circuit[ moto[ i ].km ].pos_x ) <= 1 ) moto[ i ].d_angle += 500;
            end
            dcol = abs( moto[ i ].km - position );
            if ( dcol < 5 ) // collision avec heros
                ddx = abs( moto[ i ].pos_x + heros_x );
                if ( ddx <= 20 )
                    vitesse -= 100;
                end
            end
        end
    end
end

/*
--------------------------------------
  la gestion graphique du heros
--------------------------------------
*/

process moto_heros();
    private
        int image;
        int anim[ 12 ] = -13, -12, -11, -10, -9, -8, 7, 8, 9, 10, 11, 12, 13;
        int flag;
        int dd;
        int dd0;
begin
    file = id_mbike;
    x = screen_x / 2;
    y = screen_y -2;;
    image = 6;
    loop
        // penche a gauche ou a droite
        if ( anim[ image ] < 0 ) flags = 1;
        else
            flags = 0;
        end
        graph = abs( anim[ image ] );
        frame;
        if ( key( _left ))
            image--;
        else
            if ( image < 7 )
                image++;
            end
        end
        if ( key( _right ))
            image++;
        else
            if ( image > 7 )
                image--;
            end
        end
        if ( image < 0 )
            image = 0;
        end
        if ( image > 12 )
            image = 12;
        end
        // chute du motard a gauche
        if ( level.circuit[ position ].graf[ 2 ] <> 0 and id_col == 1 and heros_x < 0 )

            sound_stop( channeld );
            sound_play( id_soundf );
            sound_set_volume( id_soundf, volume_effect );

            heros_status = 1;
            from i = 112 to 80 step -1;
                y = regen[ i ].pos_y;
                size -= 2;
                graph++;
                if ( graph > 20 )
                    graph = 20;
                    x += 2;
                end
                frame;
            end;
            heros_status = 2;
            x = screen_x / 2;
            y = screen_y -2;;
            image = 6;
            size = 100;
            graph = 7;

            channelD = sound_play( id_soundd, -1 );
            channel_set_volume( channelD, volume_effect );

            repeat
                frame;
            until ( heros_status == 0 );
        end
        // chute du motard a droite
        if ( level.circuit[ position ].graf[ 3 ] <> 0 and id_col == 1 and heros_x > 0 )
            sound_stop( channeld );
            sound_play( id_soundf );
            sound_set_volume( id_soundf, volume_effect );

            heros_status = 1;
            from i = 112 to 80 step -1;
                y = regen[ i ].pos_y;
                size -= 2;
                graph++;
                if ( graph > 20 )
                    graph = 20;
                    x += 2;
                end
                frame;
            end;
            heros_status = 2;
            x = screen_x / 2;
            y = screen_y -2;;
            image = 6;
            size = 100;
            graph = 7;

            channelD = sound_play( id_soundd, -1 );
            channel_set_volume( channelD, volume_effect );

            repeat
                frame;
            until ( heros_status == 0 );
        end
        // fin de la course
        if ( heros_status == 3 );
            set_fps( 20, 0 );
            from graph = 7 to 1 step -1;
                frame;
            end
            graph = 1;
            loop
                frame;
            end
        end
    end
end

/*
--------------------------------------
  le menu de gestion des circuits
--------------------------------------
*/

process operation_disk();
    private
        int dx;
        int dy;
        int N;
        int id_texte;
        string texte = "";
        int pp;
        int ppo;
        int info;
begin
    status = -3;
    frame;
    status = -2;
    frame;
    file = id_editeur;
    x = 144;
    y = 100;
    graph = 60;
    button( 1, 168, 8, 61 ); //1  move
    button( -3, 344, 8, 38 ); //2 close
    button( 3, 35, 24, 64 ); //3 load
    button( 4, 105, 24, 67 ); //4 save
    button( 5, 175, 24, 70 ); //5 edit
    button( 6, 245, 24, 73 ); //6 erase
    dx = 36;
    n = 0;
    from i = 0 to 4; // bouton tracks
        dy = 49;
        from j = 0 to 9;
            button_d( dx, dy, n );
            dy += 12;
            n++;
        end
        dx += 70;
    end
    dx = 36;
    n = 100;
    from i = 0 to 4; // bouton race
        dy = 184;
        from j = 0 to 4;
            button_d( dx, dy, n );
            dy += 12;
            n++;
        end
        dx += 70;
    end
    repeat
        status = 0;
        repeat
            frame;
        until ( status <> 0 );
        repeat;
            frame;
        until ( mouse.left == false );
        switch ( status );
            //---------------------
            case 1: // move
                repeat
                    x = mouse.x -176;
                    y = mouse.y -4;
                    if ( id_texte <> 0 )
                        write_delete( id_texte );
                    end
                    frame;
                until ( mouse.left == true );
            end
            //---------------------
            case 3: //load
                if ( id_texte <> 0 )
                    write_delete( id_texte );
                end
                id_texte = write( 0, x + 8, y + 247, 3, "Select a track to load" );
                select = -1;
                repeat;
                    frame;
                until ( select <> -1 );
                if ( data_base.track[ select ].nom[ 0 ] > 0 )
                    load( nom[ select ], level );
                    status = -3;
                    write_delete( id_texte );
                else
                    write_delete( id_texte );
                    id_texte = write( 0, x + 8, y + 247, 3, "This track don't exist" );
                end
            end
            //---------------------
            case 4: //save
                if ( id_texte <> 0 )
                    write_delete( id_texte );
                end
                id_texte = write( 0, x + 8, y + 247, 3, "Select a track to save" );
                select = -1;
                repeat;
                    frame;
                until ( select <> -1 );
                if ( data_base.track[ select ].nom[ 0 ] == 0 )
                    write_delete( id_texte );
                    write( 0, x + 8, y + 247, 3, "Enter a name:" );
                    texte = input_new( 8 );
                    id_texte = write_var( 0, x + 120, y + 247, 3, texte );
                    pp = 0;
                    LOOP
                        IF ( keyboard.scan_code <> _enter )
                            IF ( keyboard.scan_code == _backspace and pp > 0 )
                                input_delete( texte );
                                data_base.track[ select ].nom[ pp ] = 0;
                                pp--;
                                keyboard.ascii = 0;
                            ELSE
                                IF ( keyboard.ascii <> 0 )
                                    input_add( texte, keyboard.ascii );
                                    data_base.track[ select ].nom[ pp ] = keyboard.ascii;
                                    pp++;
                                    keyboard.ascii = 0;
                                END
                            END
                        ELSE
                            BREAK;
                        END
                        FRAME;
                    END
                    save( nom[ select ], level );
                    status = -3;
                    write_delete( all_text );
                else
                    write_delete( id_texte );
                    id_texte = write( 0, x + 8, y + 247, 3, "This track is not free" );
                end
            end
            //---------------------
            case 5: //edit
                if ( id_texte <> 0 )
                    write_delete( id_texte );
                end
                id_texte = write( 0, x + 8, y + 247, 3, "Select a track to copy" );
                select = -1;
                repeat;
                    frame;
                until ( select <> -1 );
                if ( data_base.track[ select ].nom[ 0 ] > 0 )
                    write_delete( id_texte );
                    id_texte = write( 0, x + 8, y + 247, 3, "Select a race" );
                    info = select;
                    loop
                        select = 0;
                        repeat;
                            frame;
                        until ( select >= 100 );
                        pp = select -100;
                        ppo = pp / 5;
                        pp = pp - ( ppo * 5 );
                        if ( data_base.race[ ppo ].level[ pp ] = -1 )
                            data_base.race[ ppo ].level[ pp ] = info;
                            status = -3;
                            write_delete( id_texte );
                            break;
                        else
                            write_delete( id_texte );
                            id_texte = write( 0, x + 8, y + 247, 3, "This position is not free" );
                        end
                    end
                else
                    write_delete( id_texte );
                    id_texte = write( 0, x + 8, y + 247, 3, "This track is empty" );
                end
            end
            //---------------------
            case 6: //erase
                if ( id_texte <> 0 )
                    write_delete( id_texte );
                end
                id_texte = write( 0, x + 8, y + 247, 3, "Select a track to erase" );
                select = -1;
                repeat;
                    frame;
                until ( select <> -1 );
                if ( select < 100 )
                    if ( data_base.track[ select ].nom[ 0 ] > 0 )
                        write_delete( id_texte );
                        from i = 0 to 8;
                            data_base.track[ select ]
                            .nom[ i ] = 0;
                        end
                        status = -3;
                    else
                        write_delete( id_texte );
                        id_texte = write( 0, x + 8, y + 247, 3, "This track is empty" );
                    end
                else
                    pp = select -100;
                    ppo = pp / 5;
                    pp = pp - ( ppo * 5 );
                    if ( data_base.race[ ppo ]
                        .level[ pp ] > 0 )
                        data_base.race[ ppo ].level[ pp ] = -1;
                        write_delete( id_texte );
                        status = -3;
                    else
                        write_delete( id_texte );
                        id_texte = write( 0, x + 8, y + 247, 3, "This race is empty" );
                    end
                end
            end
        end
        repeat;
            frame;
        until ( mouse.left == false );
    until ( status == -3 );
    save( "script.mbk", data_base );
    status = -1;
    id_menu = menu();
    from i = 1 to 34;
        button( i, icon[ i ]
        .gx, icon[ i ].gy, icon[ i ].graf );
    end
    button_b( -1, -11, 91 );
    button_b( 1, 39, 91 );
end

/*
--------------------------------------
  les boutons ou son afficher du textes
--------------------------------------
*/

process button_D( dx, dy, N );
    private
        int id_texte;
        int ox;
        int oy;
        int pos;
        string texte = "";
begin
    file = id_editeur;
    graph = 58;
    if ( n < 100 )
        if ( data_base.track[ n ].nom[ 0 ] > 0 )
            texte = input_new( 8 );
            from i = 0 to 7;
                if ( data_base.track[ n ].nom[ i ] > 0 )
                    input_add( texte, data_base.track[ n ].nom[ i ] );
                else
                    break;
                end
            end
            id_texte = write_var( 0, x, y, 4, texte );
        end
    else
        ox = n -100;
        oy = ox / 5;
        ox = ox - ( oy * 5 );
        if ( data_base.race[ oy ].level[ ox ] > 0 )
            texte = input_new( 8 );
            from i = 0 to 7;
                if ( data_base.track[ data_base.race[ oy ].level[ ox ]].nom[ i ] <> 0 )
                    input_add( texte, data_base.track[ data_base.race[ oy ].level[ ox ]].nom[ i ] );
                else
                    break;
                end
            end
            id_texte = write_var( 0, x, y, 4, texte );
        end
    end
    repeat
        x = id_menu.x + dx;
        y = id_menu.y + dy;
        if ( id_texte <> 0 )
            write_move( id_texte, x, y );
        end
        frame;
        if ( collision( type mouse ) and mouse.left and status <> 0 )
            graph = 59;
            repeat;
                frame;
            until ( mouse.left == false );
            graph = 58;
            select = n;
        end
    until ( status == -3 );
    if ( id_texte <> 0 )
        write_delete( id_texte );
    end
end

/*
  -----------------------------------------------
  defillement de la position dans le score
  -----------------------------------------------
*/

process scroll_position( n );
    private
        int id_texte;
begin
    y = 600;
    x = 10;
    if ( n == position )
        id_texte = write_var( id_fntd, x, y, 3, n );
    else
        id_texte = write_var( id_fntb, x, y, 3, n );
    end
    global_id++;
    repeat;
        write_move( id_texte, x, y );
        y -= 1;
        frame;
        if ( y == 480 )
            scroll_position( n -1 );
        end
    until ( y < -50 or n == 0 );
    write_delete( id_texte );
    global_id--;
end

/*
--------------------------------------
  defillement des initiales
--------------------------------------
*/

process scroll_txt( n );
    private
        string texte = "";
        int id_texte;
begin
    y = 600;
    x = 140;
    texte = input_new( 8 );
    from i = 0 to 7;
        if ( top_score[ n ].nom[ i ] <> 0 )
            input_add( texte, top_score[ n ].nom[ i ] );
        else
            break;
        end
    end
    if ( n == position )
        id_texte = write_var( id_fntd, x, y, 3, texte );
    else
        id_texte = write_var( id_fntb, x, y, 3, texte );
    end
    repeat;
        write_move( id_texte, x, y );
        y -= 1;
        frame;
        if ( y == 480 )
            scroll_txt( n -1 );
        end
    until ( y < -50 or n == 0 );
    write_delete( id_texte );
end

/*
--------------------------------------
  defillement des points
--------------------------------------
*/

process scroll_score( n );
    private
        int id_texte;
begin
    y = 600;
    x = 640;
    id_texte = write_var( id_fntb, x, y, 5, top_score[ n ].valeur );
    repeat;
        write_move( id_texte, x, y );
        y -= 1;
        frame;
        if ( y == 480 )
            scroll_score( n -1 );
        end
    until ( y < -50 or n == 0 );
    write_delete( id_texte );
end
