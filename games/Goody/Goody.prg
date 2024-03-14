//------------------------------------------------//
//Program: ¡GOODY!  The Remake
//Author:  José Javier Cárceles Marmol (coptroner)
//Team: Coptron games (c) 2004-2006
//
//BennuGD2 Port by SplinterGU (2011/07/11)
//------------------------------------------------//

Program GOODY;

import "libmod_gfx";
import "libmod_input";
import "libmod_misc";
import "libmod_sound";
import "mod_debug";

#define FADE_TIME 1000

declare Process  Objeto_extra(double x,y, int file,graph,z,objeto,pantalla)
    Private
       int Prota;
       int eruct;
    end
end

#define put_screen(f,g) background.file = f; background.graph = g

declare process s_window(double x,y, int file,graph);
declare process op_win(double x,y, int file,graph,op5,z);
declare Process opciones(double x,y,int graph, double size, int z,opcion,_on);
declare Process Menu_opciones(double x,y,int graph,z,opcion);
declare Process menu_opc(double x,y,int graph,z);
declare process Manoselec(double x,y, int file,graph,z);
declare Process Configurar(double x,y,int graph,z);
declare Process Options2(double x,y, int graph,z,opt);
declare Process Options(double x,y, int z,opt);
declare Process goody_menu(double x,y, int file,graph,z,flags);
declare Process Enemi_menu(double x,y, int file,z);
declare Process Ayuda(double x,y, int file,graph,z,double size,int game);
declare process cortina(double x,y, int file,graph,z,double size, int vel,cor);
declare Process Game_over2(double x,y, double size);
declare process Banner(double x,y, int graph);
declare Process Continuar(double x,y, int z);
declare Process Abandona(double x,y, int z);
declare Process scanlines(double x,y, int file,graph,z);
declare Process Score(double x,y, int file,z,tipo);
declare process Lingo(lim,file,graph,z,double size);
declare Process score_herram(double x,y, int file,graph,z,pieza);
declare Process Herramientas (double x,y, int file,graph,z,herramienta);
declare Process Selector(double x,y, int file,graph,z);
declare Process selector2(double x,y, int file,graph,z);
declare Process Combos(double x,y, int file,graph,num);
declare Process sombraje(file,gfx,sombras);
declare Process GOODY(double x,y, int file,graph,flags,z,action) end
declare Process renace(double x,y, int re);
declare Process Llegada(x,y);
declare Process Ladrillo(double x,y, int file,graph,z,poder,flags);
declare Process rompeladrillo(file,graph,double x,y, int z, double size);
declare Process Escala(double x,y, int file,graph,z);
declare Process Goody_golpe(double x,y, int file,graph,z,porrazo);
declare Process Paredes_Proc(double x,y, int file,graph,z,muro);
declare Process cabinas(double x,y, int file,graph,z,abrir,herra);
declare Process Herracabina(double x,y, int file,graph,z);
declare process subepunt(double x,y, int file,graph,z);
declare Process Cubo_basura(double x,y, int file,graph,z,ratas);
declare Process Escenario(double x,y, int file,graph,z,alpha);
declare Process Ascensores(double x,y, int file,graph,z,tope_up,tope_down);
declare Process Barcas(double x,y, int file,graph,z);
declare Process Metro (double x,y, int file,graph,z,vagon);
declare Process Metro2 (double x,y, int file,graph,z,vagon);
declare Process Puerta_metro1(double x,y, int file,graph,z,activa,estac);
declare Process Puerta_metro2(double x,y, int file,graph,z,activa,estac);
declare Process Goody_tren(double x, int estacion);
declare Process caja_fuerte(double x,y, int file,graph,z);
declare Process Buitre(double x,y, int file,graph,z,salida,flags);
declare Process huevo(double x,y, int file,graph,z);
declare Process Gotas(double x,y, int file,graph,z,goteo);
declare Process Peces(double x,y, int file,graph,z,double size);
declare Process Chapuzon(double x,y, int file,graph,z);
declare Process rata(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Serpiente(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Ladron(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Karateka(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Gorila(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Policia(double x,y, int file,graph,z,flags,zona1,zona2);
declare Process Fantasmas(file,graph,z,alpha,xrelativa,yrelativa,incx,incy,distx,disty);
declare Process Limpiadora(double x,y, int file,graph,z);
declare Process Polvos(double x,y, int file,graph,z);
declare process Polvos2(double x,y) end
declare Process Banquero(double x,y, int file,graph,z);
declare Process Lingotes(double x,y, int file,graph,z);
declare Process Obrero_pala(double x,y, int file,graph,z);
declare Process Arena(double x,y, int file,graph,z);
declare Process Obrero_pico(double x,y, int file,graph,z);
declare Process Piedra(double x,y, int file,graph,z);
declare Process Secretario(double x,y, int file,graph,z);
declare Process Avion_papel(double x,y, int file,graph,z);
declare Process Luna(double x,y, int file,graph,z);
declare Process Agualuna(double x,y,int file,graph,z,angulo);
declare Process Agualuna2(double x,y, int file,graph,z);
declare Process Prostituta(double x,y, int file,graph,z,flags);
declare Process corazon(double x,y, int file,graph,z);
declare Process Panal(double x,y, int file,graph,z) end
declare Process Abejas(double x,y, int file,graph,z);
declare Process Helicoptero(double x,y, int file,graph,z,zona1,zona2);
declare Process Cohete(double x,y, int file,graph,z);
declare Process camara(double x,y, int file,graph,z,flags,lasers);
declare Process Laser(double x,y, int file,graph,z,flags);
declare Process opciones_menu1(double x,y,int file,graph,z,control);
declare Process OPERA(double x,y,int file,graph);
declare Process Redefine(double x,y, int file,graph,z);
declare process Tex_credit(double x,y, int file,graph,z);
declare process creditos2(double x,y, int file,graph,z);
declare process credits(double x,y, int file,graph,z);

Global
    //-- Goody --//
    int Cerveza;     int Vidas;       int Money;
    int Goodi;       double Goody_x;  double Goody_y;
    int Goody_flag;  int ASCOP=1;
    int Lanzar;      int Potencia2;   int Escalera;
    int Escala_x;    int Escala_y;    int Escala_stage;
    int Escalon;     int Metro_muerto;
    int Vida_extra;  int Mas_vida;
    int Sentado;     int Agachate;
    int Continues;

    //-- Inicio de Estructuras --//
    Int teclado[5];
    Int Redef[5];
    Int T_ascii[5];

    Int Cabin[7];
    Int Pared[7];
    Int Objeto[3];
    Int Objetos[3];
    Int Herramientas2[7];
    Int Bolsa_dinero[7];
    Int Jarra_cerveza[12];
    Int Combina[11];
    Int Combo[11];

    //-- Juego --//
    int Stage;        int pausado=false;
    int Opcioness;    int Opcion2;
    int Opcion3;      int Sonidos;
    int Idioma;       int Banda;
    int FIN;          String Combo_caja;
                      string combo_caja_digit[11];
    int Intentos;     String combina1;
    int Atraco;       int Policias;
    int Huevazo;      int Corazonazo;
    int Aviones;      int id_sel;
    int Sube;         int Cabina_in;
    int Cabina;       int Scanline;
    int G_fondo;      int Cortin=false;
    int Abejillas;    int Helices;
    int mapa_dureza;  

//    int Color_dureza1=90;
//    int Paredes=30;
//    int Techos=20;
//    int paredescala=110;
//    int color_lingote=87;

    int Color_dureza1=0xfce888ff;
    int Paredes=0x4040fcff;
    int Techos=0xe8e8fcff;
    int paredescala=0x006c00ff;
    int color_lingote=0xfcfc28ff;
    
    int p_completa;
    int sombras;
    int li;
    int Ferret;
    String Cheat="";
    int Joys_x=0;     int Joys_y=0;

    //-- Graficos --//
    int Ggoody;       int Genemigos;
    int Gnivel;       int Gscore;
    int Gscena;       int Gmenu;
    int Gwindow;      int Gscena2;
//    int Ggoodys;      int Genemigos2;
    //-- Sonidos --//
    int s_salto;      int s_ladrillo;
    int s_roto1;      int s_disparo;
    int s_grito1;     int s_golpe1;
    int s_golpe2;
    int s_eructo;     int s_grito2;
    int s_grito3;     int s_abejas;
    int s_atraco;     int s_atraco2;
    int s_gotas;      int s_chapuzon;
    int s_escala;     int s_helicoptero;
    int s_explosion1; int s_cohete1;
    int s_pico;       int s_metro;
    int s_puerta;     int s_gorila;
    int s_lata;       int s_rejas;
    int s_sirena;     int s_cheats;
    int s_queja;      int s_camara;
    int s_metro2;     int s_trago;
    int s_herram;     int s_beso;
    int S_huevo;      int s_buitre;
    int Musica1;      int Musica2;
    int Intro1;       int Volumen=60;
    //-- Texto --//
    int L_money;     int l_vidas;
    int L_mensajes;

    //-- VIDEO --//
    int Gvideo;
    int video;
    int tiempo_total;
    int tiempo_actual;

    int desktop_width, desktop_height;

    int logos = -1;

//Private
//    import "mpeg.dll";

Local
    int bug_084;
    int velocidad;
    int velocidad_gravedad;
    int no_sombra;
    int altura;
    int Accion;
    int Ascensor;
    int Barca;
    int Escaleras;
    int Rebote;
    int Herrami;
    int Vida;
    int Mueve;

Const
    int Parado=0;
    int Andar=1;
    int Saltar=2;
    int Agachar=3;
    int Muerto=4;
    int Disparar=5;
    int Subescalon=6;
    int Espera=7;
    int max_money=7;
    int max_vidas=15;

    int KUP=0;
    int KDOWN=1;
    int KRIGHT=2;
    int KLEFT=3;
    int KSPACE=4;
    int KENTER=5;



function int skip_action()
begin
  return key(_enter) OR key(_esc) OR key(_space) OR get_joy_button(JOY_BUTTON_BACK) OR get_joy_button(JOY_BUTTON_Y) OR get_joy_button(JOY_BUTTON_X) OR get_joy_button(JOY_BUTTON_A) OR get_joy_button(JOY_BUTTON_B);
end

function int confirm()
begin
  return key(_enter) OR key(_space) OR get_joy_button(JOY_BUTTON_Y) OR get_joy_button(JOY_BUTTON_X) OR get_joy_button(JOY_BUTTON_A) OR get_joy_button(JOY_BUTTON_B);
end


process main()
begin
    desktop_get_size(&desktop_width,&desktop_height);

    if ( os_id == os_switch )
      p_completa = 1;
    else
      screen.fullscreen=false;
      set_mode(320,240);
      window_move((desktop_width-320)/2,(desktop_height-240)/2);

      Gwindow=fpg_load("data1/gwindow.gdg");
      set_fps(40,0);
      Idioma=1;
      s_window(160,120,gwindow,1); while (exists(type s_window)) frame; end
      mouse.graph=0;
      fpg_unload(gwindow);
    end

    //--Inicializa el Modo de Pantalla--//
    If(p_completa)
        screen.fullscreen=false;
        set_mode(640,480);
        window_move((desktop_width-640)/2,(desktop_height-480)/2);
    Else
        screen.fullscreen=true;
        set_mode(640,480);
    End

    window_set_title("GOODY the remake 1.5");

    frame;

    select_joy(0);

    Logotipos(); While (exists(Type logotipos)) Frame; End

    set_fps(26,0);

    //--Inicializa las variables del Menu de configuracion--//
    Sonidos=true;
    Scanline=false;

    //-- Carga los graficos principales --//

    if (idioma&1)
        gmenu=fpg_load("data1/gmenu.gdg");
    else
        gmenu=fpg_load("data1/gmenu2.gdg");
    end

    gscore=fpg_load("data1/gscore.gdg");
    ggoody=fpg_load("data1/ggoody.gdg");
    genemigos=fpg_load("data1/genemigos.gdg");
    gscena=fpg_load("data1/gscena.gdg");
    gscena2=fpg_load("data1/gscena2.gdg");

    fade_on(FADE_TIME);
    inicio();

    Loop frame; end

End

process s_window(double x,y, int file,graph)
begin
    Mouse.file=file;
    Mouse.graph=12;
    Mouse.z=-500;
    Mouse.x=x+50; Mouse.y=y;
    op_win(229,103,file,2,1,-250);
    op_win(229,156,file,3,2,-250);
    op_win(229,209,file,4,3,-250);
    op_win(72,209,file,5,4,-250);
    loop
        if (idioma&1) graph=1; else graph=6; end
        frame;
    end
end

process op_win(double x,y, int file,graph,op5,z)
private
    int Wraton;
begin
    repeat
        Wraton=collision(type mouse);
        if (idioma&1) graph=op5+1; else graph=op5+6; end
        if(wraton)
            switch(op5) case 1..10: alpha=255;  end end
            if(mouse.left)
                switch(op5)
                    case 1: p_completa=0; op5=5; end
                    case 2: p_completa=1; op5=5; end
                    case 3:
                        while(mouse.left) frame; end
                        father.accion=1;
                        if (idioma&1) idioma=2; else  idioma=1; end
                    end
                    case 4: exit("",0); end
                end
            end
        else alpha=0;
        end
        frame;
    until(op5==5)
    if (not accion)
        signal(father,s_kill);
    end
    signal(type op_win,s_kill);
end

Process inicio()
begin
/*
   //---Tabla de transparencias---//
   sombras=blendop_new();
   blendop_tint(sombras,1,0,0,0);
   blendop_translucency(sombras,0.6);
*/
   //--Inicializa el teclado estandar--//
    teclado[KUP]=_UP;
    teclado[KDOWN]=_DOWN;
    teclado[KRIGHT]=_RIGHT;
    teclado[KLEFT]=_LEFT;
    teclado[KSPACE]=_SPACE;
    teclado[KENTER]=_ENTER;

    //--Inicializa la Musica--//
    music_unload(musica1);
    Musica1=music_load("data2/musica1.gdt");
    if ( exists( banda ) ) signal(banda,s_kill); frame; end
    banda=banda_sonora();
    menu_inicio();

End


function split_combo_caja()
begin
    int i;
    for ( i = 0; i < 12; i++ ) combo_caja_digit[i] = substr(combo_caja,i,1); end
end


//#Section: MENU
Process menu_inicio()
Private
   int seleccion;
   int ini;
   int cor;
begin
      let_me_alone();
      joypad();
      sound_stop(all_sound);
      write_delete(all_text);
      banda=banda_sonora();

      //-- Inicializa las variables de juego --//
       Potencia2=33;  Cerveza=186;   Money=0;
       Vidas=5;
       Sube=-4;       Escalera=0;
       Goody_x=370;   Goody_y=248;   Goody_flag=0;
       Combo_caja=""; split_combo_caja();
       Combina1="";   Fin=0;
       Intentos=2;    Vida_extra=0;  ASCOP=0;
       Pausado=false; Continues=2;
       region_define(2,225,366,23,potencia2);
       region_define(3,57,406,cerveza,7);

        From ini=0 To max_money;
           If (bolsa_dinero[ini]==1) Bolsa_dinero[ini]=0; End
        End
        From ini=0 To 12;
           If (jarra_cerveza[ini]==1) Jarra_cerveza[ini]=0; End
        End
        From ini=0 To 7;
           If (cabin[ini]<>0) cabin[ini]=0; End
        End
        From ini=0 To 7;
           If (Pared[ini]<>0) Pared[ini]=0; End
        End
        From ini=0 To 3;
           If (objeto[ini]<>0) objeto[ini]=0; End
        End
        From ini=0 To 3;
           If (objetos[ini]<>0) objetos[ini]=0; End
        End
        From ini=0 To 7;
           If (herramientas2[ini]<>0) herramientas2[ini]=0; End
        End
        From ini=0 To 11;
           If (combina[ini]<>0) combina[ini]=0; End
           Combo[ini]=0;
        End
      //-- Finaliza la Inicializacion las variables --//

      //--Luces--//
      Escenario(166,217,gscena2,1,-15,180);

      // Pone Scanlines en el Menu--//
      If (scanline) scanlines(320,240,gscore,100,-200); End

      opciones(260,216,24,100,-5,1,1);
      opciones(260,265,25,100,-5,1,2);
      opciones(260,310,27,100,-5,1,3);
      goody_menu(690,320,ggoody,1,-30,1);
      enemi_menu(770,319,genemigos,-30);

         if(cortin)
            from accion=22 to 480 step 44;
              from mueve=23 to 650 step 46;
                  cortina(mueve,accion,gscore,98,-300,100,-4,2);
              end
            end
         end
         cortin=false;

      put_screen(gmenu,1);
      fade_on(FADE_TIME);
      while(exists(type cortina)) frame; end

      If (!exists(Type cheatword)) Cheatword(); End

      Repeat
        file=gmenu;
          //--Cambia automaticamente de Ventana/completa--//
          If (key(_f3)); While(key(_f3)) Frame; End
             If(p_completa)
                  p_completa=0;
                  screen.fullscreen=true;
                  set_mode(640,480);
                  put_screen(gmenu,1);

               Else
                  p_completa=1;
                  screen.fullscreen=false;
                  set_mode(640,480);
                  window_move((desktop_width-640)/2,(desktop_height-480)/2);
                  put_screen(gmenu,1);

             End
          End
         //--Sale del juego echando leches!!!!--//
         if(key(_alt) and key(_s)) exit("",0); end

          Frame;
      Until(confirm())

       Switch(opcioness)
          Case 1:
              If(ASCOP==2) vidas+=5; End
              If(ASCOP==3) vidas+=5; End
              signal(Type cheatword,s_kill);
              sound_unload(s_cheats);
              cortin=true;
	          from accion=22 to 480 step 44;
		        from mueve=23 to 650 step 46;
		           cortina(mueve,accion,gscore,98,-300,1,+4,2);
     		    end
		        end
             write_delete(all_text);
             while(exists(type cortina))
              cor=get_id(type cortina);
                if(cor)
                   if(cor.size>94)
                     Score(320,240,gscore,-260,3);
                   end
                end
                frame;
             end
              fpg_unload(gmenu);
//              clear_screen();
              signal(Type opciones,s_kill);
              Stage=1; Nivel(stage);
          End
          Case 2:
             Menu_opciones(432,272,5,-7,1);
          End
          Case 3:
//             banner(320,240,90);
            music_fade_off(1200);
            fade_off(1250); While(fade_info.fading) Frame; End
            exit("",0);
          End
       End
End

Process opciones(double x,y,int graph, double size, int z,opcion,_on)
begin
    Loop
     file=gmenu;
      if(not exists(type menu_opciones))
         If (key(_up)or Joys_Y==1); While(key(_up)or Joys_Y==1) Frame; End
            opcion--; If (opcion<1) opcion=3; End
         End
         If (key(_down)or Joys_Y==2); While(key(_down)or Joys_Y==2) Frame; End
            opcion++; If (opcion>3) opcion=1; End
         End
      end
        switch(opcion)
             case 1:
                if (_on==1) graph=24; end
                if (_on==2) graph=25; end
                if (_on==3) graph=27; end
             end
             case 2:
                if (_on==1) graph=23; end
                if (_on==2) graph=26; end
                if (_on==3) graph=27; end
             end
             case 3:
                if (_on==1) graph=23; end
                if (_on==2) graph=25; end
                if (_on==3) graph=28; end
             end
        end

       If ((key_down(_esc) or get_joy_button(JOY_BUTTON_BACK)) and (not exists(type menu_opc) and not exists(type Menu_opciones)))
          while(get_joy_button(JOY_BUTTON_BACK)) frame; end
          opcion=3;
       End
       opcioness=opcion;
       Frame(200);
    End
End

Process cheatword()
Private
    int vale;
    string chea,str;
begin
    cheat="";
    S_cheats=sound_load("data2/cheats.gdt");

    Loop
        str=substr(cheat,-3);
        If(str=="cop");//trongamestudios")
            Ascop=1; vale=1; chea=str;
            sound_play(s_cheats,0);
        End
        str=substr(cheat,-9);
        If(str=="operasoft")
            Ascop=2; vale=1; chea=str;
            sound_play(s_cheats,0);
        End
        str=substr(cheat,-4);
        If(str=="redo")
            Ascop=3; vale=1; chea=str;
            sound_play(s_cheats,0);
        End
        cheat = substr(cheat,-9);
        if(vale)
            vale=0;
            x=320; y=120; z=-50; file=gmenu;
            graph=80; accion=0; mueve=0; text.z=-250;
            write (0,360,115,4,"Modo Especial");
            write_var(0,360,130,4,chea);
            repeat
                accion++;
                mueve++; if(mueve>2) mueve=0; graph++; end
                if(graph>81) graph=80; end
                frame;
            until(accion>80)
            graph=0; write_delete(all_text);
            cheat="";
        end
        If(keyboard.scan_code<>0)
            cheat+=chr(keyboard.ascii);
            Repeat Frame; Until(keyboard.scan_code==0);
        End
        Frame;
    End
End

//-- Menu OPCIONES --//
//#Section: MENU OPCIONES
Process Menu_opciones(double x,y,int graph,z,opcion)
begin
    file=gmenu;
    size=0;
    Repeat  size+=10; Frame; Until(size>90);
    size=100;
    Menu_opc(432,272,5,z);
    Repeat
       file=gmenu;
       Frame;
    Until((key_down(_esc) OR get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) and not exists(type menu_opc))
    while( get_joy_button(JOY_BUTTON_BACK) OR get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) frame; end
    signal(Type menu_opc,s_kill);
    signal(Type options,s_kill);
    Repeat  size-=10; Frame; Until(size<10);
    size=0;
    menu_inicio();
End


Process menu_opc(double x,y,int graph,z)
Private
   int manosel;
begin
    file=gmenu;
    Opcion2=1;

    Options(430,228,-9,1);
    Options(430,256,-9,2);
    Options(430,285,-9,3);
    Options(430,314,-9,4);
    manosel=Manoselec(515,241,file,4,z-5);
    Loop
      file=gmenu;
       If (key(_up)or joys_y==1); While(key(_up)or joys_y==1) Frame; End
          opcion2--; manosel.y-=29; If (opcion2<1) opcion2=4; manosel.y=328; End
       End
       If (key(_down)or Joys_y==2); While(key(_down)or joys_y==2) Frame; End
          opcion2++; manosel.y+=29; If (opcion2>4) opcion2=1; manosel.y=241; End
       End
       If ( confirm() )
          While( confirm() ) Frame; End

          Switch(opcion2)
             Case 1: Redefine(430,272,file,14,z-1); End
             Case 2: Configurar(515,238,4,z-5); End
             Case 3: Ayuda(320,240,file,29,-100,0,0); End
             Case 4: Creditos(); end
//             Case 4: Video_final(); End
          End

       End
       If (key_down(_esc) OR get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y))
          signal(id,s_kill);
          while( get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) frame; end
       End
       Frame;
    End
End

process Manoselec(double x,y, int file,graph,z)
begin
   repeat
     file=gmenu;
     frame;
   until(!exists(father))
end

//#Section: Configuracion
Process Configurar(double x,y,int graph,z)
begin
    file=gmenu;
    Opcion3=1;
    signal(father,s_sleep);
    signal(Type options,s_sleep);
    signal(Type manoselec,s_sleep);
    Options2(430,228,70,z+1,1);
    Options2(430,256,74,z+1,2);
    Options2(430,285,72,z+1,3);
    Options2(430,314,76,z+1,4);
    Loop
       file=gmenu;
       If (key(_up)or Joys_y==1); While(key(_up)or Joys_y==1) Frame; End
          opcion3--; y-=29; If (opcion3<1) opcion3=4; y=325; End
       End
       If (key(_down)or Joys_y==2); While(key(_down)or Joys_y==2) Frame; End
          opcion3++; y+=29; If (opcion3>4) opcion3=1; y=238; End
       End

       If (key_down(_esc) OR get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y))
          while( get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) frame; end
          signal(father,s_wakeup);
          signal(Type options,s_wakeup);
          signal(Type manoselec,s_wakeup);
          signal(Type options2,s_kill);
          signal(id,s_kill);
       End

       Frame;
    End
End


//--OPCIONES de configuracion--//
Process Options2(double x,y, int graph,z,opt)
begin
    file=gmenu;
    switch(opt)
       case 1:
          if(p_completa) graph=71; else graph=70; end
       end
       case 2:
          if(volumen==0) graph=75; else graph=74; end
       end
       case 3:
          if(scanline) graph=73; else graph=72; end
       end
    end
    Loop
     file=gmenu;
     if( confirm() )
        while ( confirm() ) frame; end
       Switch(opt)
          Case 1:
             If (opcion3==opt)
               If(p_completa)
                    p_completa=0; graph=70;
                    screen.fullscreen=true;
                    set_mode(640,480);
                    put_screen(gmenu,1);
                 Else
                    p_completa=1; graph=71;
                    screen.fullscreen=false;
                    set_mode(640,480);
                    window_move((desktop_width-640)/2,(desktop_height-480)/2);
                    put_screen(gmenu,1);
               End
             end
          End
          Case 2:
             If (opcion3==opt)
               If (volumen>0)
                   volumen=0; Graph=75;
                   channel_set_volume(-1,volumen);
									 music_set_volume(volumen);
                 Else
                   volumen=60; Graph=74;
                   channel_set_volume(-1,volumen+40);
                   music_set_volume(volumen);
               End

             end
          End
          Case 3:
             If (opcion3==opt)
                If (not scanline)
                    scanline=true; scanlines(320,240,gscore,100,-200);
                    graph=73;
                  else
                    scanline=false; graph=72; signal(type scanlines,s_kill);
                end
             end
          End

          Case 4:
             If (opcion3==opt)
                if (idioma&1)
                    idioma=2;
                    fpg_unload(gmenu);
                    gmenu=fpg_load("data1/gmenu2.gdg");
                    put_screen(gmenu,1);
                    graph=76;
                  Else
                    idioma=1;
                    fpg_unload(gmenu);
                    gmenu=fpg_load("data1/gmenu.gdg");
                    put_screen(gmenu,1);
                    graph=76;
                end
             End
          End
       End
     end
       Frame;
    End
End

//--OPCIONES principales--//
Process Options(double x,y, int z,opt)
begin
    Loop
     file=gmenu;
       Switch(opt)
          Case 1:
             If (opcion2==opt) graph=7; Else graph=6; End
          End
          Case 2:
             If (opcion2==opt) graph=9; Else graph=8; End
          End
          Case 3:
             If (opcion2==opt) graph=11; Else graph=10; End
          End
          Case 4:
             If (opcion2==opt) graph=13; Else graph=12; End
          End
       End
       Frame;
    End
End


Process goody_menu(double x,y, int file,graph,z,flags)
Private
  int veloc;
  int mueve1;
  int corre;
begin
   veloc=4; mueve1=3;

   Loop
      corre++; If (corre==75) veloc=8; mueve1=2; End
      mueve++; If(mueve=>mueve1) mueve=0; graph++; End
      If (graph>4) graph=1; End
      If (x<-85) x=690; veloc=4; Mueve1=3; corre=0; caras(1); End
      x-=veloc;
      Frame;
   End
End


Process Enemi_menu(double x,y, int file,z)
Private
  int veloc;
  int mueve1;
  int corre;
  int enemi;
begin
   veloc=4; mueve1=3;
   enemi=rand(1,5);
   Switch(enemi)
      Case 1: y=y; graph=3; End
      Case 2: y=y; graph=1; End
      Case 3: y=y+12; graph=5; End
      Case 4: y=y+2; graph=29; End
      Case 5: y=y; graph=26; End
   End
   Loop
      corre++; If (corre==75) veloc=8; mueve1=2; End
      mueve++; If(mueve=>mueve1) mueve=0; graph++; End
      Switch(enemi)
          Case 1:  If (graph>4) graph=3; End End
          Case 2:  If (graph>2) graph=1; End End
          Case 3:  If (graph>6) graph=5; End End
          Case 4:  If (graph>32) graph=29; End End
          Case 5:  If (graph>27) graph=26; End End
      End
      If (x<-10) enemi_menu(770,319,genemigos,-30); signal(id,s_kill); End
      x-=veloc;


      Frame;
   End
End


//-- Ayuda/Info --//
//#Section: AYUDA
Process Ayuda(double x,y, int file,graph,z,double size, int game)
begin
     signal(father,s_sleep);
     write_delete(all_text);
     x=320;
     set_fps(35,0);
     Repeat size+=10; Frame; Until(size>90); size=100;
     Repeat
        If ((key(teclado[KRIGHT])or Joys_x==2) AND graph<39)
           While(key(teclado[KRIGHT])or Joys_x==2) Frame; End
           graph++;
        End
        If ((key(teclado[KLEFT])or Joys_x==1) AND graph>29)
           While(key(teclado[KLEFT])or Joys_x==1) Frame; End
           graph--;
        End
        Frame;
     Until(key_down(_esc) OR get_joy_button(JOY_BUTTON_BACK) or key_down(_enter) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y));
     while(get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) frame; end

     Repeat size-=10; Frame; Until(size<0);
     pausado=false;
     signal(father,s_wakeup);
     set_fps(26,0);
     If (game) fpg_unload(gmenu);
        write_var(l_money,149,385,5,money);
        write_var(l_vidas,318,414,4,vidas);
        int i;
        for ( i = 0; i < 12; i++ )
            write_var(l_money,397+i*19,419,3,combo_caja_digit[i]);
        end
     End
End


function set_mapa_dureza(int n)
begin
  switch ( map_info(gnivel, n, g_depth ) )
    case 8:
      Color_dureza1=90;
      Paredes=30;
      Techos=20;
      paredescala=110;
      color_lingote=87;
    end

    case 16:
      Color_dureza1=0xfce888;
      Paredes=0x4040fc;
      Techos=0xe8e8fc;
      paredescala=0x006c00;
      color_lingote=0xfcfc28;
    end

    case 32:
      Color_dureza1=0xfce888ff;
      Paredes=0x4040fcff;
      Techos=0xe8e8fcff;
      paredescala=0x006c00ff;
      color_lingote=0xfcfc28ff;
    end
  end
  mapa_dureza = n;
end


//#Section: Niveles de juego
Process Nivel(stage)
Private
   int Aguas;
   int numer;
begin
   let_me_alone();
   Joypad();
   sound_stop(all_sound);
   write_delete(all_text);
   fade_on(FADE_TIME);
   banda=banda_sonora();

   put_screen(0,0);

frame;

   Lanzar=0;   Policias=0; Atraco=0;
   If(sube==0) sube=-4; else sube=4; End

   If (scanline) scanlines(320,240,gscore,100,-200); End

   ini_files();

   Switch(stage)
      Case 0://--CARCEL--//
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,20,0,1,0);
         set_mapa_dureza(21);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);
         goodi.accion=espera; graph=15;
//         opciones_menu1(320,240,genemigos,48,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //--Objetos escenario--//
         Cabinas(391,258,gscore,16,goodi.z-2,9,1);
         If (cabin[1]==1) Herracabina(391,256,gscore,9,-8); End
         If (pared[1]==0) Paredes_Proc(319,233,gscena,10,-5,1); End

         //--Enemigos--//
          Policia(200,278,Genemigos,1,-8,1,140,250);
          Policia(130,278,Genemigos,1,-8,1,145,250);

         //-- Fin si no tenemos LLAVE --//
         If (get_id(type Paredes_Proc)<>0)
             If (Objetos[0]<>9 AND Objetos[1]<>9 AND
                 Objetos[2]<>9 AND Objetos[3]<>9)
                 Game_over(1);
                 goodi.accion=espera; graph=15;
             End
         End
      End
      Case 1:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,1,g_fondo,1,4);
         scroll[0].ratio=350;
         set_mapa_dureza(2);
         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);
//         opciones_menu1(320,240,gscore,21,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(960,160,gnivel,90,-50,255);
         Cubo_basura(101,95,gscena,3,-8,1);
         Cubo_basura(135,95,gscena,3,-8,1);
         Cubo_basura(166,95,gscena,3,-8,1);

         //-- Enemigos --//
         Gotas(1506,203,genemigos,7,-8,30);
         Serpiente(900,95,genemigos,22,-8,0,830,1055);
         Serpiente(1400,95,genemigos,22,-8,0,1390,1575);

         //--Money--//
         If (bolsa_dinero[0]==0) Objeto_extra(274,103,gscore,5,0,1,0); End
         //--Cerveza--//
         If (Jarra_cerveza[0]==0) Objeto_extra(1370,273,gscore,40,0,2,0);End
         //--Lata combinacion--//
         If (Combina[0]==0) Objeto_extra(1400,280,gscore,17,0,3,0);End

         if(cortin)
            from numer=22 to 480 step 44;
              from aguas=23 to 650 step 46;
                  cortina(aguas,numer,gscore,98,-300,100,-4,2);
              end
            end
         end
         cortin=false;
         while(exists(type cortina)) pausado=1; frame; end

      End
      Case 2:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,3,g_fondo,1,4);
         set_mapa_dureza(4);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,67,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cubo_basura(935,281,gscena,3,-3,1);
         Cubo_basura(967,281,gscena,3,-3,1);
         Cubo_basura(1600,281,gscena,3,-3,1);

         //--Luces--//
         Escenario(1005,181,gscena2,1,-15,180);

         //--Enemigos--//
         ladron(rand(900,1400),278,Genemigos,3,-10,1,875,1900);
         Luna(1200,34,genemigos,20,-5);
         Prostituta(1030,280,genemigos,67,-5,0);

         //--Money--//
         If (bolsa_dinero[1]==0) Objeto_extra(1013,149,gscore,5,0,1,1); End

         //-- Herramientas --//
         If (Herramientas2[0]==0) Herramientas(120,127,gscore,7,-5,0); End
         If (Herramientas2[1]==0) Herramientas(202,127,gscore,8,-5,1); End
         If (Herramientas2[2]==0) Herramientas(424,127,gscore,9,-5,2); End
         If (Herramientas2[3]==0) Herramientas(504,127,gscore,10,-5,3); End
         If (Herramientas2[4]==0) Herramientas(121,175,gscore,11,-5,4); End
         If (Herramientas2[5]==0) Herramientas(199,177,gscore,12,-5,5); End
         If (Herramientas2[6]==0) Herramientas(425,174,gscore,13,-5,6); End
         If (Herramientas2[7]==0) Herramientas(506,176,gscore,14,-5,7); End
         pausado=false;
      End
      Case 3:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,5,0,1,0);
         set_mapa_dureza(6);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,15,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cabinas(218,193,gscore,16,goodi.z-2,14,0);
         If (cabin[0]==1) Herracabina(218,191,gscore,14,-8); End
         If (pared[0]==0) Paredes_Proc(-100,-100,gscena,10,-5,0); End
         Escenario(696,311,gscena,30,10,255);

         //--Enemigos--//
         rata(1100,290,Genemigos,5,-10,1,1050,1250);
         rata(650,200,Genemigos,5,-10,1,300,600);
         Peces(697,280,genemigos,11,-8,0);

         //--Cerveza--//
         If (Jarra_cerveza[1]==0) Objeto_extra(1044,289,gscore,40,0,2,1);End
         //--Lata combinacion--//
         If (Combina[1]==0) Objeto_extra(235,154,gscore,17,0,3,1);End
      End
      Case 4:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,7,0,1,0);
         set_mapa_dureza(8);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         From aguas=39 To 595 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //--Enemigos--//
         rata(100,130,Genemigos,5,-10,1,80,175);
         rata(820,290,Genemigos,5,-10,1,790,965);
         Gotas(235,87,genemigos,7,-8,30);
      End
      Case 5:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,9,g_fondo,1,4);
         set_mapa_dureza(10);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,52,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cubo_basura(235,282,gscena,3,-8,1);
         Cubo_basura(267,282,gscena,3,-8,1);
         Cubo_basura(300,282,gscena,3,-8,1);
         Cubo_basura(760,289,gscena,3,-8,1);

         //-- Enemigos --//
         Luna(290,34,genemigos,20,-5);
         Gorila(1100,180,genemigos,29,-10,0,990,1175);
         Panal(1230,107,genemigos,48,-9);

      End
      Case 6:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,11,0,1,0);
         set_mapa_dureza(12);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Escenario --//
         From aguas=39 To 1950 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //--Enemigos--//
         rata(300,265,Genemigos,5,-10,1,285,455);
         rata(1100,160,Genemigos,5,-10,1,1030,1300);
         rata(885,130,Genemigos,5,-10,1,750,900);
         rata(1400,120,Genemigos,5,-10,1,1340,1695);
         ladron(1200,130,Genemigos,3,-10,1,1080,1670);
         Gotas(216,90,genemigos,7,-8,20);
         Gotas(985,90,genemigos,7,-8,30);
         Gotas(1594,105,genemigos,7,-8,30);

         //--Cerveza--//
         If (Jarra_cerveza[2]==0) Objeto_extra(401,120,gscore,40,0,2,2);End
         //--Money--//
         If (bolsa_dinero[2]==0) Objeto_extra(719,274,gscore,5,0,1,2); End
      End
      Case 7:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,13,0,1,0);
         set_mapa_dureza(14);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(750,115,gscena,5,goodi.z-2,255);

         //--Luces--//
         Escenario(280,165,gscena2,6,-5,80);
         Escenario(488,164,gscena2,6,-5,80);
         Escenario(700,164,gscena2,6,-5,80);
         Escenario(891,164,gscena2,6,-5,80);
         Escenario(1083,164,gscena2,6,-5,80);
         Escenario(1305,67,gscena2,6,-5,80);
         Escenario(1637,67,gscena2,6,-5,80);

         //--Enemigos--//
         Fantasmas(genemigos,rand(18,19),-15,160,rand(300,500),150,90,40,60,100);
         Fantasmas(genemigos,rand(18,19),-15,160,rand(900,1200),150,100,50,60,120);
         Karateka(700,260,genemigos,26,-2,0,580,950);
         Karateka(1400,120,genemigos,26,-2,0,1310,1615);
         Karateka(500,100,genemigos,26,-2,0,330,700);
         Limpiadora(305,280,genemigos,40,-8);
         Limpiadora(805,103,genemigos,40,-8);

         //--Money--//
         If (bolsa_dinero[3]==0) Objeto_extra(840,113,gscore,5,0,1,3); End
      End
      Case 8:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,15,g_fondo,1,4);
         set_mapa_dureza(16);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

       //opciones_menu1(320,240,gscena,8,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cubo_basura(967,905,gscena,3,-8,1);
         Cubo_basura(1000,905,gscena,3,-8,1);

         //--Luces--//
         Escenario(280,502,gscena2,6,-5,80);
         Escenario(199,791,gscena2,6,-5,80);
         Escenario(519,791,gscena2,6,-5,80);
         Escenario(792,775,gscena2,6,-5,80);

         Escenario(1113,449,gscena2,1,-15,80);
         Escenario(967,595,gscena2,1,-15,80);
         Escenario(967,705,gscena2,1,-15,80);
         Escenario(1085,691,gscena2,1,-15,80);

         //--Enemigos--//
         Fantasmas(genemigos,rand(18,19),-12,160,rand(250,380),770,90,40,60,100);
         Fantasmas(genemigos,rand(18,19),-12,160,rand(500,700),490,100,50,60,120);
         Gotas(1125,540,genemigos,7,-8,30);
         Luna(1025,90,genemigos,20,-5);
         If (money<=0)
                Serpiente(700,220,genemigos,22,-8,0,630,910);
            Else
                Gorila(440,210,genemigos,29,-10,0,425,900);
         End
         Karateka(700,435,genemigos,26,-2,0,610,857);
         Karateka(450,904,genemigos,26,-2,0,225,795);
         Policia(980,904,Genemigos,1,-9,1,960,1175);
         Panal(97,200,genemigos,48,-12);
         Secretario(162,402,genemigos,45,-8);
         Limpiadora(417,730,genemigos,40,-8);


         //--Lata combinacion--//
         If (Combina[2]==0) Objeto_extra(1145,696,gscore,17,0,3,2);End
         //--Cerveza--//
         If (Jarra_cerveza[3]==0) Objeto_extra(56,593,gscore,40,0,2,3);End
         If (Jarra_cerveza[4]==0) Objeto_extra(881,225,gscore,40,0,2,4);End
         //--Money--//
         If (bolsa_dinero[4]==0) Objeto_extra(805,451,gscore,5,0,1,4); End
         If (bolsa_dinero[5]==0) Objeto_extra(263,227,gscore,5,0,1,5); End
      End
      Case 9:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,1,0,1,0);
         set_mapa_dureza(2);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         From aguas=679 To 1950 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //--Enemigos--//
         rata(135,290,Genemigos,5,-10,1,130,320);
         rata(1230,240,Genemigos,5,-10,1,1215,1440);
         Gotas(888,183,genemigos,7,-8,25);
         Gotas(1133,166,genemigos,7,-8,20);
         Gotas(1725,95,genemigos,7,-8,20);
         Peces(1525,280,genemigos,11,-8,0);
         Serpiente(930,112,genemigos,22,-8,0,900,985);

         //--Lata combinacion--//
         If (Combina[3]==0) Objeto_extra(1006,115,gscore,17,0,3,3);End
         //--Money--//
         If (bolsa_dinero[6]==0) Objeto_extra(953,111,gscore,5,0,1,6); End
      End
      Case 10:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,3,0,1,0);
         set_mapa_dureza(4);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //--Objetos escenario--//
         From aguas=39 To 600 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End
         From aguas=1319 To 1960 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //--Enemigos--//
         rata(170,130,Genemigos,5,-10,1,80,175);
         rata(400,130,Genemigos,5,-10,1,315,465);
         rata(850,290,Genemigos,5,-10,1,775,955);
         rata(1500,165,Genemigos,5,-10,1,1400,1545);
         Gotas(235,87,genemigos,7,-8,30);
         Gotas(1745,87,genemigos,7,-8,20);
         Peces(1620,280,genemigos,11,-8,0);

         //--Cerveza--//
         If (Jarra_cerveza[5]==0) Objeto_extra(1345,271,gscore,40,0,2,5);End
      End
      Case 11:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,5,0,1,0);
         set_mapa_dureza(6);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //--Objetos escenario--//
         Cubo_basura(1404,280,gscena,3,-6,0);
         Cubo_basura(1437,280,gscena,3,-6,0);

         From aguas=39 To 1350 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //--Enemigos--//
         rata(270,130,Genemigos,5,-10,1,135,425);
         rata(450,274,Genemigos,5,-10,1,405,515);
         rata(940,274,Genemigos,5,-10,1,920,1090);
         Gotas(330,105,genemigos,7,-8,19);
         Peces(845,280,genemigos,11,-8,0);
         Serpiente(1450,290,genemigos,22,-8,0,1420,1575);

         //--Lata combinacion--//
         If (Combina[4]==0) Objeto_extra(135,279,gscore,17,0,3,4);End
         //--Cerveza--//
         If (Jarra_cerveza[6]==0) Objeto_extra(1040,120,gscore,40,0,2,6);End
      End
      Case 12:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,7,0,1,4);
         set_mapa_dureza(8);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

         //-- Llegada del METRO --//
         If (goody_x>500 AND Metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(960,5);
             Metro2(1570,240,gscena,22,-9,2);
         End
         If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(951,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,410,720);
         Limpiadora(438,282,genemigos,40,-8);
      End
      Case 13:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,9,0,1,4);
         set_mapa_dureza(10);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,10,0);

         //-- Llegada del METRO --//
          If (metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(320,3);
             Metro2(-320,240,gscena,22,-9,1);
          End
          If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(311,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,570,880);
         Limpiadora(1055,186,genemigos,40,-8);
      End
      Case 14:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,11,g_fondo,1,4);
         set_mapa_dureza(12);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,8,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cubo_basura(740,601,gscena,3,-8,0);
         Cubo_basura(773,601,gscena,3,-8,1);

         //--Lata combinacion--//
         If (Combina[5]==0) Objeto_extra(1375,406,gscore,17,0,3,5);End
         If (Combina[6]==0) Objeto_extra(1835,166,gscore,17,0,3,6);End

         //--Jarra de cerveza--//
         If (Jarra_cerveza[7]==0) Objeto_extra(1570,382,gscore,2,0,2,7);End

         //--Luces--//
         Escenario(100,359,gscena2,1,-15,80);
         Escenario(710,501,gscena2,1,-15,80);

         //-- Enemigos --//
         Luna(340,331,genemigos,20,-5);
         Luna(880,110,genemigos,20,-5);

         Prostituta(733,600,genemigos,67,-9,0);

         Obrero_pala(1120,488,genemigos,60,-8);
         Obrero_pala(1705,376,genemigos,60,-8);
         Obrero_pala(1410,168,genemigos,60,-8);

         Obrero_pico(1433,489,genemigos,64,-8);
         Obrero_pico(1100,169,genemigos,64,-8);
         Obrero_pico(1690,265,genemigos,64,-8);
      End
      Case 15:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,13,g_fondo,1,4);
         set_mapa_dureza(14);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,13,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //--Lata combinacion--//
         If (Combina[7]==0) Objeto_extra(1011,150,gscore,17,0,3,7);End
         //--Jarra de cerveza--//
         If (Jarra_cerveza[8]==0) Objeto_extra(564,594,gscore,2,0,2,8);End

         //-- Enemigos --//
         Obrero_pala(133,136,genemigos,60,-8);

         Obrero_pico(360,264,genemigos,64,-8);
         Obrero_pico(475,441,genemigos,64,-8);
         Obrero_pico(125,489,genemigos,64,-8);
         Obrero_pico(806,137,genemigos,64,-8);
         Obrero_pico(1050,459,genemigos,64,-8);
      End
      Case 16:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,15,g_fondo,1,4);
         set_mapa_dureza(16);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,13,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         From aguas=37 To 1260 Step 79;
            Escenario(aguas,309,gscena,30,10,255);
         End

         //--Luces--//
         Escenario(1671,180,gscena2,1,-15,80);

         //--Jarra de cerveza--//
         If (Jarra_cerveza[9]==0) Objeto_extra(721,240,gscore,2,0,2,9);End
         //--Money--//
         If (bolsa_dinero[7]==0) Objeto_extra(1120,243,gscore,5,0,1,7); End

         //-- Enemigos --//
         Panal(121,153,genemigos,48,-12);
         Panal(1160,153,genemigos,48,-12);
         ladron(600,88,Genemigos,3,-10,1,180,850);
         ladron(1200,88,Genemigos,3,-10,1,1055,1290);
         Policia(1690,278,Genemigos,1,-8,1,1650,1890);
      End
      Case 17:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,1,0,1,4);
         set_mapa_dureza(2);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

         //-- Llegada del METRO --//
         If (goody_x>500 AND Metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(960,5);
             Metro2(1570,240,gscena,22,-9,2);
         End
         If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(951,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,410,720);
         Limpiadora(438,282,genemigos,40,-8);
      End
      Case 18:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,3,0,1,4);
         set_mapa_dureza(4);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,10,0);

         //-- Llegada del METRO --//
          If (metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(320,3);
             Metro2(-320,240,gscena,22,-9,1);
          End
          If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(311,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,570,880);
         Limpiadora(1055,186,genemigos,40,-8);
      End
      Case 19:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,5,g_fondo,1,4);
         set_mapa_dureza(6);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,35,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cubo_basura(203,281,gscena,3,-8,0);
         Cubo_basura(236,281,gscena,3,-8,1);
         Escenario(960,160,gnivel,90,-50,255);

         //--Luces--//
         Escenario(156,181,gscena2,1,-15,80);

         //-- Enemigos --//
         Luna(127,50,genemigos,20,-5);
         Panal(486,71,genemigos,48,-12);
         Panal(1427,36,genemigos,48,-12);
         ladron(500,216,Genemigos,3,-10,1,320,920);
         Gorila(1200,218,genemigos,29,-10,0,1130,1450);
         Serpiente(1600,294,genemigos,22,-8,0,1570,1665);
      End
      Case 20:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,7,g_fondo,1,4);
         set_mapa_dureza(8);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscore,17,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(640,160,gnivel,91,-50,255);

         //--Luces--//
         Escenario(197,112,gscena2,1,-15,90);
         Escenario(1028,161,gscena2,1,-15,90);

         //--Lata combinacion--//
         If (Combina[8]==0) Objeto_extra(201,87,gscore,17,0,3,8);End

         //-- Enemigos --//
         Luna(640,45,genemigos,20,-5);
         Panal(733,185,genemigos,48,-12);
         Gorila(rand(300,500),283,genemigos,29,-10,0,220,865);
         Serpiente(250,294,genemigos,22,-8,0,220,410);

      End
      Case 21://--Las barcas del Retiro--//
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,9,g_fondo,1,4);
         set_mapa_dureza(10);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(960,160,gnivel,92,-50,255);

         From aguas=148 To 1850 Step 79;
            Escenario(aguas,311,gscena,30,10,255);
         End

         barcas(140,301,gscena,35,0);
         barcas(220,301,gscena,35,0);

         barcas(380,301,gscena,35,0);
         barcas(460,301,gscena,35,0);

         barcas(620,301,gscena,35,0);

         barcas(780,301,gscena,35,0);
         barcas(860,301,gscena,35,0);
         barcas(940,301,gscena,35,0);

         barcas(1100,301,gscena,35,0);

         barcas(1260,301,gscena,35,0);
         barcas(1340,301,gscena,35,0);

         barcas(1500,301,gscena,35,0);

         barcas(1660,301,gscena,35,0);
         barcas(1740,301,gscena,35,0);

         //-- Enemigos --//
         Luna(428,35,genemigos,20,-5);
         Luna(1485,45,genemigos,20,-5);
         Panal(733,100,genemigos,48,-12);
      End
      Case 22:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,11,g_fondo,1,4);
         set_mapa_dureza(12);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,67,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(640,160,gnivel,93,-50,255);

         //--Luces--//
         Escenario(954,181,gscena2,1,-15,90);

         Cubo_basura(926,281,gscena,3,-8,0);
         Cubo_basura(893,281,gscena,3,-8,1);

         //--Jarra de cerveza--//
         If (Jarra_cerveza[10]==0) Objeto_extra(824,65,gscore,2,0,2,10);End

         //-- Enemigos --//
         Luna(160,45,genemigos,20,-5);
         Luna(900,40,genemigos,20,-5);
         Gorila(rand(400,600),220,genemigos,29,-10,0,270,750);
         Gorila(1000,280,genemigos,29,-10,0,950,1200);
         Serpiente(150,295,genemigos,22,-8,0,140,180);
         Prostituta(980,280,genemigos,67,-5,0);

      End

      Case 23:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,1,0,1,4);
         set_mapa_dureza(2);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

         //-- Llegada del METRO --//
         If (goody_x>500 AND Metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(960,5);
             Metro2(1570,240,gscena,22,-9,2);
         End
         If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,genemigos,40,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(951,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,410,720);
         Limpiadora(438,282,genemigos,40,-8);
      End
      Case 24:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,3,0,1,4);
         set_mapa_dureza(4);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,10,0);

         //-- Llegada del METRO --//
          If (metro_muerto==0)
             signal(Type goody,s_sleep);
             goody_tren(320,3);
             Metro2(-320,240,gscena,22,-9,1);
          End
          If(metro_muerto==1) metro_muerto=0; End
//         opciones_menu1(320,240,gscena,8,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(311,297,gscena,23,-12,255);

         //-- Enemigos --//
         Policia(600,275,Genemigos,1,-15,1,570,880);
         Limpiadora(1055,186,genemigos,40,-8);
      End
      Case 25:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,5,g_fondo,1,4);
         set_mapa_dureza(6);

         //--Luces--//
         Escenario(88,105,gscena2,1,-15,90);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,8,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cabinas(488,179,gscore,16,goodi.z-2,7,2);
         If (cabin[2]==1) Herracabina(487,177,gscore,7,-8); End
         If (pared[2]==0) Paredes_Proc(565,137,gscena,11,-5,2); End

         //-- Enemigos --//
         Policia(300,200,Genemigos,1,-15,1,115,415);
      End
      Case 26:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,7,g_fondo,1,4);
         set_mapa_dureza(8);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,70,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(640,160,gnivel,90,-8,255);

         //--Luces--//
         Escenario(252,118,gscena2,6,-5,80);
         Escenario(367,118,gscena2,6,-5,80);
         Escenario(1186,118,gscena2,6,-5,80);

         //--Lata combinacion--//
         If (Combina[9]==0) Objeto_extra(1007,184,gscore,17,0,3,9);End

         //--Jarra de cerveza--//
         If (Jarra_cerveza[11]==0) Objeto_extra(862,176,gscore,2,0,2,11);End

         //-- Enemigos --//
         camara (500,20,genemigos,70,-12,1,20);
         camara (55,20,genemigos,70,-12,0,15);
         Secretario(367,238,genemigos,45,-7);
         Secretario(692,240,genemigos,45,-7);
         Secretario(914,240,genemigos,45,-7);
         Policia(rand(600,1000),280,Genemigos,1,-15,1,445,1225);

      End
      Case 27:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,9,g_fondo,1,4);
         set_mapa_dureza(10);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,70,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Cabinas(1565,181,gscore,16,goodi.z-2,11,3);
         If (cabin[3]==1) Herracabina(1565,179,gscore,11,-8); End
         If (pared[3]==0) Paredes_Proc(1662,106,gscena,13,-5,3); End

         Cabinas(920,274,gscore,16,goodi.z-2,8,4);
         If (cabin[4]==1) Herracabina(920,272,gscore,8,-8); End
         If (pared[4]==0) Paredes_Proc(998,272,gscena,12,-5,4); End

         //-- Enemigos --//
         camara (20,20,genemigos,70,-12,0,20);
         camara (565,20,genemigos,70,-12,1,15);
         camara (630,20,genemigos,70,-12,0,15);
         camara (1275,20,genemigos,70,-12,0,20);

         Secretario(440,161,genemigos,45,-8);
         Secretario(830,161,genemigos,45,-8);
         Secretario(1080,161,genemigos,45,-8);
         Secretario(1380,161,genemigos,45,-8);
         Limpiadora(586,280,genemigos,40,-8);
         Limpiadora(1790,103,genemigos,40,-8);
         Policia(1200,278,Genemigos,1,-15,1,1050,1850);
      End
      Case 28:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,11,g_fondo,1,4);
         set_mapa_dureza(12);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,100,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//

         Cabinas(407,178,gscore,16,goodi.z-2,12,5);
         If (cabin[5]==1) Herracabina(407,176,gscore,12,-8); End
         If (pared[5]==0) Paredes_Proc(494,106,gscena,11,-5,5); End

         Cabinas(1735,178,gscore,16,goodi.z-2,10,6);
         If (cabin[6]==1) Herracabina(1735,175,gscore,10,-8); End
         If (pared[6]==0) Paredes_Proc(1832,121,gscena,11,-5,6); End

         //-- Enemigos --//
         camara (105,20,genemigos,70,-12,0,20);
         Policia(rand(100,300),278,Genemigos,1,-15,1,100,440);
         Policia(900,278,Genemigos,1,-15,1,830,960);
         Policia(1620,278,Genemigos,1,-15,1,1590,1685);

         Banquero(610,154,genemigos,100,-8);
         Banquero(798,199,genemigos,100,-8);
         Banquero(1030,72,genemigos,100,-8);
         Banquero(1520,200,genemigos,100,-8);
         Banquero(1495,72,genemigos,100,-8);


      End
      Case 29:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,13,g_fondo,1,4);
         set_mapa_dureza(14);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,30,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         From aguas=317 To 360 Step 29;
            Escenario(aguas,311,gscena,30,10,255);
         End
         From aguas=836 To 870 Step 26;
            Escenario(aguas,311,gscena,30,10,255);
         End

         Cabinas(468,258,gscore,16,goodi.z-2,13,7);
         If (cabin[7]==1) Herracabina(468,256,gscore,13,-8); End
         If (pared[7]==0) Paredes_Proc(543,247,gscena,10,-5,7); End

         //-- Enemigos --//
         Limpiadora(1010,190,genemigos,40,-8);
         Obrero_pico(245,41,genemigos,64,-8);
         Peces(330,280,genemigos,11,-8,0);
         Peces(845,280,genemigos,11,-8,0);
      End
      Case 30:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,15,g_fondo,1,4);
         set_mapa_dureza(16);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,genemigos,7,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Objetos escenario --//
         Escenario(197,310,gscena,30,10,255);
         From aguas=40 To 326 Step 26;
            Escenario(aguas,950,gscena,30,10,255);
         End

         Cabinas(586,834,gscore,16,goodi.z-2,14,0);
         If (cabin[0]==1) Herracabina(586,832,gscore,14,-8); End
         If (pared[0]==0) Paredes_Proc(30,800,gscena,12,-5,0); End

         //--Lata combinacion--//
         If (Combina[1]==0) Objeto_extra(597,796,gscore,17,0,3,1);End
         If (Combina[10]==0) Objeto_extra(77,135,gscore,17,0,3,10);End

         //--Jarra de cerveza--//
         If (Jarra_cerveza[12]==0) Objeto_extra(77,464,gscore,2,0,2,12);End

         //-- Enemigos --//
         Gotas(265,91,genemigos,7,-8,30);
         rata(rand(100,200),562,Genemigos,5,-10,1,87,225);
         rata(rand(80,120),723,Genemigos,5,-10,1,35,144);
         Peces(199,284,genemigos,11,-8,0);
         Peces(118,920,genemigos,11,-8,0);
         Peces(306,920,genemigos,11,-8,0);
      End
      Case 31:
         //-- Scroll de Nivel --//
         region_define(1,0,35,640,320);
         scroll_start (0,gnivel,17,0,1,4);
         set_mapa_dureza(18);

         //--Personaje--//
         Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,0);

//         opciones_menu1(320,240,gscena,50,-50,1); While(get_id(Type opciones_menu1)<>0) Frame; End

         //-- Numeros de CAJA FUERTE --//
         From aguas=91 To 486 Step 33;
            Combos(aguas,93,gscena,combo[numer]+50,numer);
            numer++;
         End

         //-- Objetos escenario --//
         From aguas=102 To 555 Step 36;
            Escenario(aguas,311,gscena,30,10,255);
         End
         From aguas=739 To 887 Step 37;
            Escenario(aguas,311,gscena,30,10,255);
         End
         From aguas=1028 To 1179 Step 30;
            Escenario(aguas,311,gscena,30,10,255);
         End

         //-- Caja FUERTE --//
         If (fin==2)
              caja_fuerte(212,167,gscena,45,-5);
           Else
              caja_fuerte(212,167,gscena,42,-5);
              caja_fuerte(212,167,gscena,40,-8);
              caja_fuerte(214,169,gscena,41,-6);
         End

         //--Lata combinacion--//
         If (Combina[11]==0) Objeto_extra(1144,88,gscore,17,0,3,11);End

         //-- Enemigos --//
         rata(950,133,Genemigos,5,-10,1,932,966);
      End

   End
   //--Ponemos los datos del marcador--//
    write_var(l_money,149,385,5,money);
    write_var(l_vidas,318,414,4,vidas);
//    write_var(l_money,397,419,3,combo_caja);
    int i;
    for ( i = 0; i < 12; i++ )
        write_var(l_money,397+i*19,419,3,combo_caja_digit[i]);
    end

   constructor(stage);
   principal();
   pausado=false;
End


//--Cortinillas--//
process cortina(double x,y, int file,graph,z,double size, int vel,cor)
begin
  graph=98;
  set_fps(35,0);

  switch(cor)
     case 1:
       repeat  size+=vel;  frame;
       until (size>1600 or size<100)
     end
     case 2:
       repeat size+=vel; frame;
       until (size>100 or size<=0)
     end
  end
  set_fps(25,0);
end


//-- Coloca los ascensores de toda la ciudad --//
//#Section: Constructor
Process Constructor(stage)
begin
    Switch(stage)
       Case 3,4,8,9,10,11,27,30: sube=4; End
       Case 1,5,14,19,29:       sube=-4; End
    End
    Switch(stage)
       Case 1:
         Ascensores(1160,100,Gscena,2,-8,87,262);
         Ascensores(1672,315,Gscena,2,-8,110,330);
       End
       Case 3:
         Ascensores(1306,45,Gscena,2,-8,0,280);
         Ascensores(105,45,Gscena,2,-8,0,245);
       End
       Case 4:
         Ascensores(1032,45,Gscena,2,-8,0,280);
       End
       Case 5:
         Ascensores(382,320,Gscena,2,-8,240,330);
       End
       Case 8:
         Ascensores(360,200,Gscena,2,-8,210,905);
       End
       Case 9:
         Ascensores(391,45,Gscena,2,-8,0,280);
       End
       Case 10:
         Ascensores(1032,45,Gscena,2,-8,0,280);
       End
       Case 11:
         Ascensores(1671,45,Gscena,2,-8,0,265);
       End
       Case 14:
         Ascensores(391,640,Gscena,2,-8,510,670);
         Ascensores(1495,300,Gscena,2,-8,183,600);
       End
       Case 19:
         Ascensores(1032,315,Gscena,2,-8,240,330);
       End
       Case 27:
         Ascensores(392,312,Gscena,2,-8,300,360);
       End
       Case 29:
         Ascensores(1110,312,Gscena,2,-8,200,360);
       End
       Case 30:
         Ascensores(470,45,Gscena,2,-8,0,877);
       End
       Case 31:
         Ascensores(rand(875,1000),200,Gscena,9,-8,845,1075);
       End
    End
End

//-- Carga Ficheros Graficos, sonidos y musica --//
//#Section: Profile
Process Ini_files()
begin
         //--Graficos--//
         If(gnivel<>0) fpg_unload(gnivel); End
         G_fondo=99;

         Switch(stage)
            Case 0..8:   gnivel=fpg_load("data1/gnivel1.gdg"); End
            Case 9..16:  gnivel=fpg_load("data1/gnivel2.gdg"); End
            Case 17..22: gnivel=fpg_load("data1/gnivel3.gdg"); End
            Case 23..31: gnivel=fpg_load("data1/gnivel4.gdg"); End
         End

         If (l_money==0) L_money=fnt_load("data1/money.glt"); End
         If (l_vidas==0) L_vidas=fnt_load("data1/vidas.glt"); End
         If (l_mensajes==0) L_mensajes=fnt_load("data1/mensajes.glt"); End

         If (ASCOP==1)
            //write(l_vidas,35,10,3,"Nivel:");
            //write_var(l_vidas,35,10,5,stage);
         End

         //--Sonidos--//
         If (s_salto==0)

             S_salto=sound_load("data2/salto1.gdt");
             S_disparo=sound_load("data2/disparo1.gdt");
             S_roto1=sound_load("data2/roto1.gdt");
             S_ladrillo=sound_load("data2/ladrillo.gdt");
             S_grito1=sound_load("data2/grito1.gdt");
             S_grito2=sound_load("data2/grito2.gdt");
             S_grito3=sound_load("data2/yeah1.gdt");
             S_golpe1=sound_load("data2/golpe1.gdt");
             S_golpe2=sound_load("data2/golpe2.gdt");
             S_eructo=sound_load("data2/eructo.gdt");
             S_atraco=sound_load("data2/atraco.gdt");
             S_atraco2=sound_load("data2/atraco2.gdt");
             S_gotas=sound_load("data2/goteo.gdt");
             S_abejas=sound_load("data2/abejas.gdt");
             S_chapuzon=sound_load("data2/chapoteo.gdt");
             S_escala=sound_load("data2/escala.gdt");
             S_helicoptero=sound_load("data2/helicoptero.gdt");
             S_explosion1=sound_load("data2/explos1.gdt");
             S_cohete1=sound_load("data2/cohete1.gdt");
             S_pico=sound_load("data2/pico.gdt");
             S_metro=sound_load("data2/metro1.gdt");
             S_puerta=sound_load("data2/metro2.gdt");
             S_gorila=sound_load("data2/gorila.gdt");
             S_lata=sound_load("data2/lata1.gdt");
             S_rejas=sound_load("data2/rejas1.gdt");
             S_sirena=sound_load("data2/sirena1.gdt");
             S_queja=sound_load("data2/queja.gdt");
             S_metro2=sound_load("data2/puertametro.gdt");
             S_camara=sound_load("data2/camara.gdt");
             S_herram=sound_load("data2/herram.gdt");
             S_beso=sound_load("data2/beso.gdt");
             S_huevo=sound_load("data2/huevo.gdt");
             S_buitre=sound_load("data2/buitre.gdt");
         End
         //-- Score --//
         Score(320,400,Gscore,-20,0);
         Score(232,368,Gscore,-21,1);
         Score(58,411,Gscore,-21,2);
         Score(27,418,Gscore,-21,4);

         score_herram(424,388,Gscore,objetos[0],-30,0);
         score_herram(480,388,Gscore,objetos[1],-30,1);
         score_herram(536,388,Gscore,objetos[2],-30,2);
         score_herram(592,388,Gscore,objetos[3],-30,3);

         //-- Variables --//
         Aviones=0;
         potencia2=33;
End


//#Section: Principal
Process PRINCIPAL()
Private
   int Ferreteria;
   int Chap;
   int fondo=1;
begin

   //-- Coloca escalera si la dejamos en una pantalla --//
     If (escalera AND escala_stage==stage)
         Escala(escala_x,escala_y,ggoody,22,-9);
     End
   Loop
        //--Estabilizador de FPS--//
//        if (frame_info.fps<25) frame_info.fps=25; set_fps(26,0); end

        //-- Comprueba si ganamos una vida extra--//
        If (mas_vida>=65)
            If (vidas<max_vidas)
              vidas++;
            End
           mas_vida=0;
        End
        //-- Controla el escenario --//
        Switch(fondo)
           Case 1: g_fondo=99; scroll_move(0); End
           Case 4: g_fondo=98; scroll_move(0); End
        End
        fondo++; If (fondo==8) fondo=1; End

        Switch(stage)
            Case 0:
                 If ((goodi.x<75 AND goodi.y>200) OR (key_down(_w) AND ASCOP==1)) stage=2;
                    goody_x=1770; goody_y=250; Goody_flag=goodi.flags;
                    escalera=0;
                    nivel(stage);
                 End
            End
            Case 1:
                 If ((key_down(_q) AND ASCOP==1)) stage=31;
//                    goody_x=33; goody_y=92; Goody_flag=goodi.flags;//28
                    goody_x=330; goody_y=170; Goody_flag=goodi.flags;//31
                    nivel(stage);
                 End
                 If ((key_down(_e) AND ASCOP==1)) stage=21;
                    goody_x=35; goody_y=230; Goody_flag=goodi.flags;
                    nivel(stage);
                 End
//                 scroll_start (0,gnivel,1,g_fondo,1,4);

                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=2;
                    goody_x=40; goody_y=56; Goody_flag=goodi.flags;
                    nivel(stage);
                 End
                 If (goodi.y>296 OR (key_down(_s) AND ASCOP==1)) stage=3;
                    goody_x=1310; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage);
                 End
                 If (key_down(_r) AND ASCOP==1)
                    Ferreteria=(objetos[3])-7;
                    Herramientas2[ferreteria]=0;
                    objetos[3]=0; Objeto[3]=0;
                    goody_x=600; goody_y=285; Goody_flag=1;
                    stage=0; escalera=0;
                    nivel(stage);

                 End
            End
            Case 2:
//                 scroll_start (0,gnivel,3,g_fondo,1,4);

                 If (get_id(Type policia)==0 AND get_id(Type Ladron)==0 AND (goodi.x>700 AND goodi.x<1400))
                    If (Money>0)
                         ladron(rand(1600,1800),278,Genemigos,3,-10,1,875,1900);
                       Else
                         Policia(rand(1600,1800),278,Genemigos,1,-8,1,875,1900);
                    End
                 End
                 If (get_id(Type helicoptero)==0 AND rand(1,35)==1
                     AND goodi.x>800 AND !exists(Type prostituta))
                       Helicoptero(rand(900,1400),rand (50,90),genemigos,51,-12,800,1550);
                       Helices=sound_play(s_helicoptero,-1);
                 End

                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=1;
                    goody_x=1885; goody_y=56; Goody_flag=goodi.flags;
                    nivel(stage);
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=5;
                    goody_x=30; goody_y=280; Goody_flag=goodi.flags;
                    nivel(stage); Break;

                 End
                 If (goodi.x<318 AND goodi.y>=180 AND get_id(Type selector)==0)
                   If (money>0 AND (objeto[0]==0 OR objeto[1]==0 OR
                       objeto[2]==0 OR objeto[3]==0))
                     goodi.velocidad=0; goodi.x=320; Ferret=1;
                     Selector(320,240,gscore,6,-10);
                    Else
                      If (objeto[0]<>0 AND objeto[1]<>0 AND
                          objeto[2]<>0 AND objeto[3]<>0 AND money>0)
                          If(get_id(Type mensaje)==0) Mensaje(9); End
                        Else
                          If(get_id(Type mensaje)==0) Mensaje(8); End
                      End
                  End
                 End
            End
            Case 3:
                 If (goodi.y<0 OR (key_down(_w) AND ASCOP==1)) stage=1;
                    goody_x=1670; goody_y=285; Goody_flag=goodi.flags;
                    ascensor=1; Sube=-4;
                    nivel(stage); Break;
                 End
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    signal(Type goody, s_sleep); goodi.no_sombra=1;
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen mas peces --//
                 If (goodi.x<610 AND get_id(Type peces)==0)
                     Peces(697,280,genemigos,11,-8,0);
                 End
                 If (goodi.x>1525 OR (key_down(_d) AND ASCOP==1)) stage=4;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage);  Break;
                 End
            End
            Case 4:
                 If (goodi.y<0 OR (key_down(_w) AND ASCOP==1)) stage=5;
                    goody_x=384; goody_y=285; Goody_flag=goodi.flags;
                    ascensor=1; Sube=-4;
                    nivel(stage); Break;
                 End
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=3;
                    goody_x=1515; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=6;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 5:
//                 scroll_start (0,gnivel,9,g_fondo,1,4);
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=2;
                    goody_x=1885; goody_y=280; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1245 OR (key_down(_d) AND ASCOP==1)) stage=7;
                    goody_x=30; goody_y=170; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (get_id(Type helicoptero)==0 AND rand(1,55)==1)
                       Helicoptero(rand(180,600),rand (50,90),genemigos,51,-12,150,700);
                       Helices=sound_play(s_helicoptero,-1);
                 End
                 If (goodi.y>296 OR (key_down(_s) AND ASCOP==1)) stage=4;
                    goody_x=1035; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End
            End
            Case 6:
                 If (key_down(_w) AND ASCOP==1) stage=7;
                    goody_x=30; goody_y=170; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen ladrones --//
                 If (goodi.x<900 AND get_id(Type ladron)==0 AND money>0)
                     ladron(1300,130,Genemigos,3,-10,1,1080,1670);
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=4;
                    goody_x=1245; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1870 OR (key_down(_d) AND ASCOP==1)) stage=9;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

            End
            Case 7:
                 If (key_down(_s) AND ASCOP==1) stage=6;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 //--Salen mas fantasmas--//
                 If (timer[1]>1000 AND get_id(Type fantasmas)==0)
                     Fantasmas(genemigos,rand(18,19),-12,160,rand(900,1200),150,100,50,60,120);
                     timer[1]=0;
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=5;
                    goody_x=1240; goody_y=170; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1745 OR (key_down(_d) AND ASCOP==1)) stage=8;
                    If (goodi.y<110) goody_y=730; Else goody_y=906; End
                    goody_x=30; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 8:
//                 scroll_start (0,gnivel,15,g_fondo,1,4);

                 //--Salen mas fantasmas--//
                 If (timer[1]>1000 AND get_id(Type fantasmas)==0)
                     Fantasmas(genemigos,rand(18,19),-12,160,rand(900,1200),650,100,50,60,120);
                     timer[1]=0;
                 End
                 //-- Salen serpientes --//
                 If (get_id(Type serpiente)==0 AND get_id(Type gorila)==0
                      AND goodi.y>400)
                      If (money<=0)
                           Serpiente(700,220,genemigos,22,-8,0,630,910);
                        Else
                           Gorila(440,210,genemigos,29,-10,0,425,900);
                      End
                 End
                 If ((goodi.x<25 AND goodi.y>600) OR (key_down(_a) AND ASCOP==1)) stage=7;
                    If (goodi.y<740) goody_y=104; Else goody_y=264; End
                    goody_x=1740; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1180 OR (key_down(_d) AND ASCOP==1)) stage=12;
                    goody_x=41; goody_y=106; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

            End
            Case 9:
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen mas peces --//
                 If (get_id(Type peces)==0)
                     Peces(1525,280,genemigos,11,-8,0);
                 End
                 If (goodi.y<0 OR (key_down(_w) AND ASCOP==1)) stage=14;
                    goody_x=390; goody_y=625; Goody_flag=goodi.flags;
                    ascensor=1; Sube=-4;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=10;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=6;
                    goody_x=1865; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 10:
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen mas peces --//
                 If (get_id(Type peces)==0)
                    Peces(1620,280,genemigos,11,-8,0);
                 End
                 If (goodi.y<0 OR (key_down(_w) AND ASCOP==1)) stage=19;
                    goody_x=1032; goody_y=285; Goody_flag=goodi.flags;
                    ascensor=1; Sube=-4;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=11;
                    goody_x=30; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=9;
                    goody_x=1875; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 11:
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen mas peces --//
                 If (get_id(Type peces)==0)
                     Peces(845,280,genemigos,11,-8,0);
                 End
                 If (goodi.x<25 OR (key_down(_a) AND ASCOP==1)) stage=10;
                    goody_x=1885; goody_y=120; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If ((goodi.y<0  AND goodi.x>1550) OR (key_down(_w) AND ASCOP==1)) stage=27;
                    goody_x=392; goody_y=282; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End
            End
            Case 12:
                 If (get_id(Type metro)==0)
                  if(exists(type goody))
                   If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=8;
                      goody_x=1175; goody_y=906; Goody_flag=goodi.flags;
                      nivel(stage); Break;
                   End
                   If (goodi.x>785 AND goodi.x<800 AND goodi.y>266
                       and goodi.flags==0 and goodi.velocidad_gravedad==0)
                      goodi.x=785; goodi.graph=1;
                      goody_tren(goodi.x,1); stage=13;
                      signal(type goody,s_freeze);
                      signal(Type policia,s_kill);
                      Escalera=1;
                   End
                  end
                 End
            End
            Case 13:
               If (get_id(Type metro)==0)
                if(exists(type goody))
                 If (goodi.x>450 AND goodi.x<480 AND goodi.y>266
                       and goodi.flags==1 and goodi.velocidad_gravedad==0)
                    goodi.x=480; goodi.graph=1;
                    signal(Type goody,s_freeze);
                    signal(Type policia,s_kill);
                    goody_tren(goodi.x,2); stage=12;
                    Escalera=1;
                 End
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=14;
                    goody_x=35; goody_y=590; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                end
               End
            End
            Case 14:
//                 scroll_start (0,gnivel,11,g_fondo,1,4);

                 If (get_id(Type helicoptero)==0 AND rand(1,35)==1
                     AND goodi.x<900)
                       Helicoptero(rand(180,500),rand (300,330),genemigos,51,-12,150,650);
                       Helices=sound_play(s_helicoptero,-1);
                 End

                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=13;
                    goody_x=1245; goody_y=90; Goody_flag=goodi.flags;
                    metro_muerto=1;
                    nivel(stage); Break;
                 End
                 If (goodi.y>630 OR (key_down(_s) AND ASCOP==1)) stage=9;
                    goody_x=390; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=15;
                    goody_x=35; goody_y=goodi.y; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1665 AND goodi.x<1708 AND goodi.y>132 AND goodi.y<150)
                     goodi.no_sombra=1;
                     Chapuzon(1686,119,genemigos,76,z); chap=1;
                   Else
                    If (goodi.x>935 AND goodi.x<965 AND goodi.y>560 AND goodi.y<600)
                        goodi.no_sombra=1;
                        Chapuzon(952,553,genemigos,76,z); chap=1;
                    End
                 End
                 If (chap)
                    chap=0; Caras(3);
                    sound_play(s_chapuzon,0);
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
            End
            Case 15:
//                 scroll_start (0,gnivel,13,g_fondo,1,4);

                 If (goodi.x>1230 OR (key_down(_d) AND ASCOP==1)) stage=16;
                    goody_x=35; goody_y=90; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=14;
                    goody_x=1885; goody_y=goodi.y; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>170 AND goodi.x<200 AND goodi.y>560 AND goodi.y<600)
                     Chapuzon(183,553,genemigos,76,z); chap=1;
                   Else
                     If (goodi.x>205 AND goodi.x<235 AND goodi.y>510 AND goodi.y<550)
                         Chapuzon(215,504,genemigos,76,z); chap=1;
                       Else
                         If (goodi.x>240 AND goodi.x<270 AND goodi.y>560 AND goodi.y<600)
                             Chapuzon(247,553,genemigos,76,z); chap=1;
                           Else
                             If (goodi.x>850 AND goodi.x<875 AND goodi.y>560 AND goodi.y<600)
                                 Chapuzon(861,553,genemigos,76,z); chap=1;
                               Else
                                 If (goodi.x>880 AND goodi.x<950 AND goodi.y>560 AND goodi.y<600)
                                     Chapuzon(909,553,genemigos,76,z); chap=1;
                                 End
                             End
                         End
                     End
                 End
                 If (chap)
                    goodi.no_sombra=1;
                    chap=0; Caras(3);
                    sound_play(s_chapuzon,0);
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage); Break;
                 End
            End
            Case 16:
//                 scroll_start (0,gnivel,15,g_fondo,1,4);
                 If (goodi.x>1885 OR (key_down(_d) AND ASCOP==1)) stage=17;
                    goody_x=41; goody_y=106; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

                 If (goodi.y>320 OR (key_down(_s) AND ASCOP==1)) stage=9;
                    goody_x=1395; goody_y=0; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=15;
                    goody_x=1225; goody_y=410; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (get_id(Type helicoptero)==0 AND rand(1,65)==1 AND goodi.x>1300)
                    Helicoptero(rand(1400,1750),rand (50,90),genemigos,51,-12,1400,1850);
                    Helices=sound_play(s_helicoptero,-1);
                 End
                 If (goodi.x>1550 AND goodi.x<1590 AND goodi.y>238 AND goodi.y<285)
                      Chapuzon(1574,233,genemigos,76,z); chap=1;
                   Else
                     If (goodi.y>scroll[0].y0+280 AND goodi.x<1220)
                         Chapuzon(goodi.x,285,genemigos,13,z); chap=1;
                     End
                 End
                 If (chap)
                    goodi.no_sombra=1;
                    chap=0; Caras(3);
                    sound_play(s_chapuzon,0);
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End

            End
            Case 17:
                 If (get_id(Type metro)==0)
                   If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=16;
                      goody_x=1885; goody_y=275; Goody_flag=goodi.flags;
                      nivel(stage); Break;
                   End
                   If (goodi.x>785 AND goodi.x<800 AND goodi.y>266
                       and goodi.flags==0 and goodi.velocidad_gravedad==0)
                      goodi.x=785; goodi.graph=1;
                      signal(Type goody,s_freeze);
                      signal(Type policia,s_kill);
                      goody_tren(goodi.x,1); stage=18;
                      Escalera=1;
                   End
                 End
            End
            Case 18:
               If (get_id(Type metro)==0)
                 If (goodi.x>450 AND goodi.x<480 AND goodi.y>266
                     and goodi.flags==1 and goodi.velocidad_gravedad==0)
                    goodi.x=480; goodi.graph=1;
                    signal(Type goody,s_freeze);
                    signal(Type policia,s_kill);
                    goody_tren(goodi.x,2); stage=17;
                    Escalera=1;
                 End
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=19;
                    goody_x=35; goody_y=275; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

               End
            End
            Case 19:
//                 scroll_start (0,gnivel,5,g_fondo,1,4);

                 //--Salen mas ladrones--//
                 If ((get_id(Type ladron)==0 AND get_id(Type policia)==0) AND goodi.x>1000)
                    If (rand(1,2)==1)
                        ladron(rand(500,650),216,Genemigos,3,-10,1,320,920);
                      Else
                        Policia(rand(500,650),216,Genemigos,1,-15,1,320,920);
                    End
                 End
                 If (goodi.y>296 OR (key_down(_s) AND ASCOP==1)) stage=10;
                    goody_x=1034; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End

                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=18;
                    goody_x=1245; goody_y=90; Goody_flag=goodi.flags;
                    metro_muerto=1;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=20;
                    goody_x=35; goody_y=217; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

            End
            Case 20:
//                 scroll_start (0,gnivel,7,g_fondo,1,4);

                 //-- Salida de helicoptero o buitre --//
                 If (NOT exists(Type helicoptero) AND rand(1,35)==1
                     AND goodi.x>600 AND get_id(Type gorila)==0)
                       Helicoptero(rand(380,900),rand (50,90),genemigos,51,-12,350,1100);
                       Helices=sound_play(s_helicoptero,-1);
                 End
                 mueve++;
                 If(NOT exists(Type buitre)AND mueve>30)
                    mueve=0;
                    buitre(-20,rand(50,90),genemigos,90,-14,1900,1);
                 End

                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=19;
                    goody_x=1885; goody_y=218; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1248 OR (key_down(_d) AND ASCOP==1)) stage=21;
                    goody_x=35; goody_y=232; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 21:
//                 scroll_start (0,gnivel,9,g_fondo,1,4);

                 If (goodi.y>scroll[0].y0+305 AND !exists(Type renace))
                    goodi.no_sombra=1;
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,285,genemigos,13,z);
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salida de helicoptero o Buitre--//
                 If (NOT exists(Type helicoptero) AND rand(1,60)==1
                     AND goodi.x>1000)
                       Helicoptero(rand(200,1600),rand (30,40),genemigos,51,-12,150,1800);
                       Helices=sound_play(s_helicoptero,-1);
                 End
                 mueve++;
                 If(NOT exists(Type buitre)AND mueve>40)
                    mueve=0;
                    buitre(-20,rand(50,90),genemigos,90,-14,1900,1);
                 End

                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=20;
                    goody_x=1245; goody_y=234; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1888 OR (key_down(_d) AND ASCOP==1)) stage=22;
                    goody_x=35; goody_y=232; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 22:
//                 scroll_start (0,gnivel,11,g_fondo,1,4);
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=23;
                    goody_x=41; goody_y=106; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=21;
                    goody_x=1885; goody_y=234; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 mueve++;
                 If(NOT exists(Type buitre)AND mueve>40)
                    mueve=0;
                    buitre(-20,rand(50,90),genemigos,90,-14,1900,1);
                 End

            End
            Case 23:
                 If (get_id(Type metro)==0)
                   If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=22;
                      goody_x=1250; goody_y=282; Goody_flag=goodi.flags;
                      nivel(stage); Break;
                   End
                   If (goodi.x>785 AND goodi.x<800 AND goodi.y>266
                       and goodi.flags==0 and goodi.velocidad_gravedad==0)
                      goodi.x=785; goodi.graph=1;
                      signal(Type goody,s_freeze);
                      signal(Type policia,s_kill);
                      goody_tren(goodi.x,1); stage=24;
                      Escalera=1;
                   End
                 End
            End
            Case 24:
               If (get_id(Type metro)==0)
                 If (goodi.x>450 AND goodi.x<480 AND goodi.y>266
                     and goodi.flags==1 and goodi.velocidad_gravedad==0)
                    goodi.x=480; goodi.graph=1;
                    signal(Type goody,s_freeze);
                    signal(Type policia,s_kill);
                    goody_tren(goodi.x,2); stage=23;
                    Escalera=1;
                 End
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=25;
                    goody_x=35; goody_y=275; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
               End
            End
            Case 25:
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=24;
                    goody_x=1245; goody_y=90; Goody_flag=goodi.flags;
                    metro_muerto=1;
                    nivel(stage); Break;
                 End
                 If (goodi.x>610 OR (key_down(_d) AND ASCOP==1)) stage=26;
                    goody_x=35; goody_y=186; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 26:
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=25;
                    goody_x=605; goody_y=188; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1250 OR (key_down(_d) AND ASCOP==1)) stage=27;
                    goody_x=33; goody_y=282; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

            End
            Case 27:
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=26;
                    goody_x=1245; goody_y=282; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (goodi.y>310 OR (key_down(_s) AND ASCOP==1)) stage=11;
                    goody_x=1671; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=28;
                    goody_x=33; Goody_flag=goodi.flags;
                    if (goodi.y<205) goody_y=132;
                    else goody_y=282;
                    end
                    nivel(stage); Break;
                 End

            End
            Case 28:
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=27;
                    goody_x=1885; Goody_flag=goodi.flags;
                    if(goodi.y<205) goody_y=106;
                    else goody_y=282;
                    end
                    nivel(stage); Break;
                 End
                 If (goodi.x>1890 OR (key_down(_d) AND ASCOP==1)) stage=29;
                    goody_x=30; goody_y=202; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End

                 //--Brillo lingotes--//
                 if(rand(1,20)==1)
                    Lingo(rand(1,5),genemigos,104,-9,0);
                    Lingo(rand(1,5),genemigos,104,-9,0);
                 end
            End
            Case 29:
//                 scroll_start (0,gnivel,13,g_fondo,1,4);

                 If (goodi.y>scroll[0].y0+295 AND goodi.x<950 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,goodi.y-15,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 //-- Salen mas peces --//
                 If (get_id(Type peces)==0)
                     Peces(330,280,genemigos,11,-8,0);
                     Peces(845,280,genemigos,11,-8,0);
                 End
                 If ((goodi.y>296 AND goodi.x>1050) OR (key_down(_s) AND ASCOP==1)) stage=30;
                    goody_x=472; goody_y=20; Goody_flag=goodi.flags;
                    ascensor=1; Sube=4;
                    nivel(stage); Break;
                 End
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=28;
                    goody_x=1885; goody_y=172; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
            End
            Case 30:
                 //-- Salen mas peces --//
                 If (get_id(Type peces)==0)
                     Peces(199,284,genemigos,11,-8,0);
                     Peces(118,920,genemigos,11,-8,0);
                     Peces(306,920,genemigos,11,-8,0);
                 End
                 If (goodi.y<0 OR (key_down(_w) AND ASCOP==1)) stage=29;
                    goody_x=1110; goody_y=285; Goody_flag=goodi.flags;
                    ascensor=1; Sube=-4;
                    nivel(stage); Break;
                 End
                 If (goodi.x<30 OR (key_down(_a) AND ASCOP==1)) stage=31;
                    If (goodi.y>650 AND goodi.y<718) goody_y=74;
                    Else goody_y=170;
                    End
                    goody_x=1255; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (((goodi.y>285 AND goodi.y<300 AND goodi.x<230 AND goodi.x>167) OR
                      (goodi.y>920 AND goodi.x<450)) AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,goodi.y-7,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
            End
            Case 31:
                 If (goodi.y>scroll[0].y0+295 AND !exists(Type renace))
                    sound_play(s_chapuzon,0); Chapuzon(goodi.x,goodi.y-15,genemigos,13,z);
                    goodi.no_sombra=1;
                    signal(Type goody, s_sleep);
                    While(get_id(Type chapuzon)<>0) Frame; End
                    cerveza=186;
                    vidas--;
                     If (vidas==-1)
                        If (ASCOP==1) vidas=5;
                          Else
                             Game_over2(320,180,0);
                             vidas=0;
                             signal(Type goody,s_sleep);
                             signal(id,s_kill);
                        End
                     End
                    Renace(goodi.x,goodi.y,1); signal(id,s_sleep);
                    //nivel(stage);
                 End
                 If (goodi.x>1259 OR (key_down(_d) AND ASCOP==1)) stage=30;
                    If (goodi.y<95) goody_y=714;
                    Else goody_y=810;
                    End
                    goody_x=35; Goody_flag=goodi.flags;
                    nivel(stage); Break;
                 End
                 If (Fin==0 AND goodi.x<310 AND goodi.y<200 AND get_id(Type selector2)==0)
                     goodi.graph=1; goodi.x=312; goodi.velocidad=0; goodi.y=172;
                     Selector2(106,116,gscore,6,-10);
                    Else
                      If (Fin==2)
                         If(goodi.x<230 AND goodi.x>180 AND goodi.y>150)
                            goodi.x=225; mueve=0;
                            signal(Type goody,s_freeze);
                            Repeat
                                goodi.alpha-=5;
                                Frame;
                            Until (goodi.alpha<10)
                            music_fade_off(1200);
                            fade_off(1250); While (fade_info.fading) Frame; End
                            scroll_stop(0);
                            //gmenu=0; menu_inicio();
                            video_final();
                         End
                      End
                 End
            End
            //--Fin control de NIVELES--//
        End
         //-- Sale del Juego --//
         If (( key_down(_esc) or get_joy_button(JOY_BUTTON_BACK)) AND NOT pausado AND not exists(Type game_over2)
               and not exists(type metro) and not exists (type metro2)
               and exists(type goody))
            if( goodi.accion<>muerto)
               while(get_joy_button(JOY_BUTTON_BACK)) frame; end
               Abandona(320,200,-100);
            End
         end

         //-- Pausa durante el Juego --//
         If ( ( key_down(_control) or get_joy_button(JOY_BUTTON_START) ) AND pausado==false and exists(type goody) and !exists(type pausar))
            if( goodi.accion<>muerto)
                pausado=true; pausar();
            end
         End
         //-- Ayuda durante el juego --//
        If (key_down(_F5))
            If(get_id(Type pausar)<>0) signal(Type pausar,s_kill); End
            pausado=true;
            if (idioma&1)
                gmenu=fpg_load("data1/gmenu.gdg");
            else
                gmenu=fpg_load("data1/gmenu2.gdg");
            end
            Ayuda(320,240,gmenu,29,-100,0,1);
        End
        
        //-- Activa/desactiva la PANTALLA COMPLETA--//
        If (key_down(_f3))
            If(p_completa)
                p_completa=0;
                screen.fullscreen=true;
                set_mode(640,480);
            Else
                p_completa=1;
                screen.fullscreen=false;
                set_mode(640,480);
                window_move((desktop_width-640)/2,(desktop_height-480)/2);
            End
        End

        //-- Activa Scanlines --//
        If (get_id(Type scanlines)==0 AND key(_f1))
            While (key(_f1)) Frame; End
            scanline=true; scanlines(320,240,gscore,100,-200);
        End
         //--Sale del juego echando leches!!!!--//
         if(key(_alt) and key(_s)) exit("",0); end

        //-- Activa el sonido y la musica --//
        If (key_down(_f2))
            If (volumen>0)
                volumen=0;
                channel_set_volume(-1,volumen);
                music_set_volume(volumen);// music_play(musica1,-1);
            Else
                volumen=60;
                channel_set_volume(-1,volumen+40);
                music_set_volume(volumen);// music_play(musica1,-1);
            End
        End
        Frame;
    End
End

//--- GAME OVER ---//
//#Section: GAME OVER
Process game_over(over)
begin
    ctype=c_screen;
//   opciones_menu1(320,240,gscore,50,-50,0); While(get_id(Type opciones_menu1)<>0) Frame; End
    Loop
        Switch(over)
            Case 1:
                file=gscore;
                if (idioma&1) graph=50; else graph=51; end
                x=320; y=170;
                Repeat
                    mueve++; Frame;
                Until(mueve>300 OR skip_action())
                Game_over2(320,180,0);
                signal(id,s_kill);
            End
        End
        Frame;
    End
End

Process Game_over2(double x,y, double size)
begin
   Continuar(320,200,-100);
   while(exists(type continuar)) frame; end
   if (vidas>0) return; else vidas=0; end
   Vidas=0;
      if (idioma&1)
           gmenu=fpg_load("data1/gmenu.gdg");
        else
           gmenu=fpg_load("data1/gmenu2.gdg");
      end
   File=gmenu; graph=40;
   Loop
      If (size<100) size+=3; Else size=100; End
      If (size==100) size=101;
           Repeat
              mueve++; Frame;
           Until(mueve>100 OR key_down(_esc) OR key_down(_space) OR key_down(_enter) OR get_joy_button(JOY_BUTTON_BACK) OR get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y))

           fade_off(1250); While(fade_info.fading) Frame; End
           scroll_stop(0);
           menu_inicio();
      End
      Frame;
   End
End


//--Banner Publicitario--//
process Banner(double x,y, int graph)
begin
    fade_off(1250); While(fade_info.fading) Frame; End
    file=gmenu; z=-500;
    let_me_alone();
    fade_on(1250); While(fade_info.fading) Frame; End
    While( skip_action() ) Frame; End
    loop
        if(altura==0)
            accion++;
            if(skip_action() or accion>500)
                altura=1;
            end
        else
            music_fade_off(1200);
            fade_off(1250); While(fade_info.fading) Frame; End
            exit("",0);
        end
        frame;
    end
end


//---CONTINUAR JUGANDO?--//
Process Continuar(double x,y, int z)
Private
   int Aban=1;
   int cor;
begin
    continues--; vidas=0;
    if (continues<0) return; end
    if (idioma&1)
        gmenu=fpg_load("data1/gmenu.gdg");
    else
        gmenu=fpg_load("data1/gmenu2.gdg");
    end

    file=gmenu; graph=41;
    size=0; pausado=true;
    While(size<100) size+=10; Frame; End size=100;
    Loop
        If(key(teclado[KLEFT]) or joys_x==1)
            Aban=1; graph=41;
        End
        If(key(teclado[KRIGHT]) or Joys_x==2)
            Aban=0; graph=42;
        End
        If (key_down(_enter) or key_down(_esc) or key_down(_space) or get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y))
            if(key(_esc) or get_joy_button(JOY_BUTTON_BACK)) Aban=0; end
            while(get_joy_button(JOY_BUTTON_BACK) or get_joy_button(JOY_BUTTON_B) or get_joy_button(JOY_BUTTON_A) or get_joy_button(JOY_BUTTON_X) or get_joy_button(JOY_BUTTON_Y)) frame; end

            If(Aban)
                vidas+=5;
                if(stage==0)
                    goody_x=1770; goody_y=250; Goody_flag=goodi.flags;
                    escalera=0; Stage=2;
                    nivel(stage);
                else
                    While(size>0) size-=10; Frame; End
				    Renace(goodi.x,goodi.y,0); if ( exists( goodi ) ) signal(goodi,s_sleep); end
				    while(exists(type renace)) frame; end
                    nivel(stage);
                end
                While(size>0) size-=10; Frame; End
                pausado=false;
                fpg_unload(gmenu);
                Break;
            Else
                While(size>0) size-=10; Frame; End
                pausado=false;
                fpg_unload(gmenu);
                Break;
            End
        End
        Frame;
    End
End

//--- Pausa durante el JUEGO ---//
//#Section: Game Paused
Process pausar()
Private
    int efecto;
    int direccion;
    int entry = 1;
begin
    frame;
    x=320; y=200;
    if (idioma&1)
        gmenu=fpg_load("data1/gmenu.gdg");
    else
        gmenu=fpg_load("data1/gmenu2.gdg");
    end
    file=gmenu; graph=43;
    z=-200;
    efecto=256;
    Direccion=1;
    Loop
        If (key_down(_control) OR key_down(_enter) OR key_down(_space) or ( !entry && get_joy_button( JOY_BUTTON_START ) ) )
            frame;
            pausado=false; Break;
        End
        if ( !get_joy_button(JOY_BUTTON_START)) entry = 0; end

        If (direccion==1)
            efecto-=10;
            If (efecto<=150) direccion=2; End
        Else
            efecto+=10;
            If (efecto>246) direccion=1; End
        End
        alpha=efecto;

        Frame;
    End
    while(get_joy_button(JOY_BUTTON_START)) frame; end
    fpg_unload(gmenu);
End


//#Section: Abandonar
Process Abandona(double x,y, int z)
Private
   int Aban=0;
   int cor;
begin

      if (idioma&1)
           gmenu=fpg_load("data1/gmenu.gdg");
        else
           gmenu=fpg_load("data1/gmenu2.gdg");
      end
//      pausado=false; scroll_stop(0); menu_inicio(); return;

      file=gmenu; graph=21;
      size=0; pausado=true;
      While(size<100) size+=10; Frame; End size=100;
    Loop
       If(key(teclado[KLEFT]) or joys_x==1)
          Aban=1; graph=22;
       End
       If(key(teclado[KRIGHT]) or Joys_x==2)
          Aban=0; graph=21;
       End
       If ( skip_action())
          if(key(_esc) or get_joy_button(JOY_BUTTON_BACK)) Aban=0; end
          While(confirm()) Frame; End
          If(Aban)
	        from accion=22 to 480 step 44;
	          from mueve=23 to 650 step 46;
	              cortina(mueve,accion,gscore,98,-300,1,+4,2);
	          end
	       end
             cor=get_id(type cortina);
             write_delete(all_text);
             while(exists(type cortina))
                if(cor)
                   if(cor.size>94)
                     Score(320,240,gscore,-260,3);
                   end
                end
                frame;
             end
             scroll_stop(0);
             fade_off(2500);
             cortin=true; menu_inicio();
             Else
               While(size>0) size-=10; Frame; End
               pausado=false;
               fpg_unload(gmenu);
               Break;
          End
       End
       Frame;
    End
End

//-- Scanlines --//
Process scanlines(double x,y, int file,graph,z)
begin
   alpha=60;
   Loop
       If (key(_f1)); While (key(_f1)) Frame; End
          scanline=false;
          signal(id,s_kill);
       End
       Frame;
   End
End


// -- Score en pantalla --//
//#Section: Score
Process Score(double x,y, int file,z,tipo)
begin
   Loop
       Switch(tipo)
          Case 0: graph=1; End
          Case 1:
              region_define(2,221,368,23,1000);
              region=2;
              graph=3;
              region_define(2,221,368,23,potencia2);
          End
          Case 2:
              region_define(3,58,408,258,7);
              region=3;
              graph=4;
              region_define(3,58,408,cerveza,7);
          End
          case 3:
             graph=99;
          end
          case 4:
              mueve++;
              switch (mueve)
                 case  0: graph=40; end
                 case  3: graph=41; end
                 case  7: graph=42; end
                 case 11: graph=40; mueve=0; end
              end
          end
       End
       Frame;
   End
End


//--Brillo de Lingotes--//
process Lingo(lim,file,graph,z,double size)
private
   int dir;
begin
   li++;
   ctype=c_scroll;
   alpha=255;
   accion=1;
   switch(lim)
      case 1:
         x=rand(545,800);
         y=rand(143,320);
      end
      case 2:
         x=rand(1045,1500);
         y=rand(15,95);
      end
      case 3:
         x=rand(1010,1515);
         y=rand(160,222);
      end
      case 4:
         x=rand(962,1550);
         y=rand(228,302);
      end
      case 5:
         x=rand(1675,1828);
         y=rand(255,302);
      end
   end
   if (map_get_pixel(gnivel,11,x,y)<>color_lingote) return; end
   loop
      dir++; if(dir>150 and size==0) li--; signal(id,s_kill); end
      switch(accion)
         case 1:
           if(size<100) size+=3; else size=100; accion=2; end
           angle+=pi/32;
           mueve++; if (mueve>1) mueve=0; graph++; end
           if (graph>106) graph=104; end
         end
         case 2:
           if(size>0) size-=3; else size=0; accion=1; end
           angle+=pi/32;
           mueve++; if (mueve>1) mueve=0; graph++; end
           if (graph>106) graph=104; end
         end
      end
      frame;
   end
end


// --Panel de Herramientas-- //
Process score_herram(double x,y, int file,graph,z,pieza)
Private
   int busca;
begin

   Loop
      graph=objetos[pieza];
      From busca=0 To 3;
          objeto[busca]=objetos[busca];
      End
      If (cabina_in)
         If((key(teclado[KLEFT])and key(_enter))or get_joy_button(JOY_BUTTON_A)>0)
            While((key(teclado[KLEFT])and key(_enter)) or get_joy_button(JOY_BUTTON_A)>0) Frame; End
            objetos[0]=objeto[1];
            objetos[1]=objeto[2];
            objetos[2]=objeto[3];
            objetos[3]=objeto[0];
         End
         If((key(teclado[KRIGHT])and key(_enter)) or get_joy_button(JOY_BUTTON_B)>0)
            While((key(teclado[KRIGHT])and key(_enter)) or get_joy_button(JOY_BUTTON_B)>0) Frame; End
            objetos[0]=objeto[3];
            objetos[1]=objeto[0];
            objetos[2]=objeto[1];
            objetos[3]=objeto[2];
         End
      End
      If ((key(teclado[KSPACE])or get_joy_button(JOY_BUTTON_X)>0 or get_joy_button(JOY_BUTTON_Y)>0) AND Cabina==true AND pieza==3)
          graph=0; cabina=false;
      End
      Frame;
   End
End


//-- HERRAMIENTAS --//
//#Section: Herramientas
Process Herramientas (double x,y, int file,graph,z,herramienta)
Private
  int Selec;
begin
   ctype=c_scroll;
   herrami=herramienta;
   Loop
      Frame;
   End
End

//-- Selector --//
Process Selector(double x,y, int file,graph,z)
Private
   int Herram;
   int Busca;
   int joyp;
begin
   ctype=c_scroll;
   //signal(Type goody,s_freeze);
   joyp=select_joy(0);
   Loop
       Herram=collision(Type herramientas);
       If ((key(teclado[KUP])or Joys_y==1) AND y>130) y-=5; End
       If ((key(teclado[KDOWN])or Joys_y==2)AND y<240) y+=5; End
       If ((key(teclado[KLEFT])or Joys_x==1) AND x>80) x-=5; End
       If ((key(teclado[KRIGHT])or Joys_x==2)AND x<550) x+=5; End
       If((key(_enter)) or (get_joy_button(JOY_BUTTON_A)>0) OR money<=0 OR (objeto[0]<>0 AND objeto[1]<>0
            AND objeto[2]<>0 AND objeto[3]<>0))
           While(key(_enter) or get_joy_button(JOY_BUTTON_X)>0) Frame; End
           From busca=0 To 3;
              objeto[busca]=objetos[busca];
           End
           signal(Type goody,s_wakeup);
           signal(id,s_kill);
       End
          If (herram AND (get_joy_button(JOY_BUTTON_X)>0 or get_joy_button(JOY_BUTTON_Y)>0 or key(teclado[KSPACE])) AND money>0)
            While(get_joy_button(JOY_BUTTON_X)>0 or get_joy_button(JOY_BUTTON_Y)>0 or key(teclado[KSPACE])) Frame; End
             herramientas2[herram.herrami]=1;
             From busca=0 To 3;
                If (objetos[busca]==0)
                   objetos[busca]=herram.graph; money-=100;
                   sound_play(s_atraco2,0); if ( exists( herram ) ) signal(herram,s_kill); end
                   Break;
                End
             End
          End
      Frame;
   End
End

//#Section: SELECTOR CAJA
//-- Selector CAJA FUERTE --//
Process selector2(double x,y, int file,graph,z)
Private
   int numero;
   int sirena;
begin
   ctype=c_scroll;
   signal(Type goody,s_freeze);
   id_sel=id;
   if(ASCOP==3)
        combo_caja="000000000000";
        split_combo_caja();
   end
   Loop
     If((key(teclado[KRIGHT])or Joys_X==2) AND numero<11)
          While(key(teclado[KRIGHT])or Joys_X==2) Frame; End numero++;
          x+=33;
       Else
         If((key(teclado[KLEFT]) or Joys_X==1) AND numero>0)
              While(key(teclado[KLEFT])or Joys_x==1) Frame; End numero--;
              x-=33;
         End
     End
       If((key(_enter)or get_joy_button(JOY_BUTTON_Y)>0)and not exists(type abandona))
         While (key(_enter)or get_joy_button(JOY_BUTTON_Y)>0) Frame; End
         From mueve=0 To 11;
             combina1+=itoa(combo[mueve]);
         End
         If(combina1<>combo_caja)
             sirena=sound_play(s_sirena,0); mueve=0;
             combina1="";
             Repeat
                mueve++;
                 fade(150,40,40,255,315); While(fade_info.fading) Frame; End
                 fade_on(315); While(fade_info.fading) Frame; End
                Frame;
             Until(mueve>3);
             sound_stop(sirena);
             intentos--;
             If (intentos<=0)
                 Intentos=2;
                 goody_x=600; goody_y=285; Goody_flag=1;
                 stage=0; escalera=0;
                 nivel(stage); Break;
             End
           Else
            combina1=""; Fin=1;
            sound_play(s_rejas,0);
         End
         signal(Type goody,s_wakeup);
         signal(id,s_kill);

       End
     Frame;
   End
End

//-- Numeros de CAJA FUERTE --//
Process Combos(double x,y, int file,graph,num)
Private
  int numero;
begin
   ctype=c_scroll;
   Loop
      If (get_id(Type selector2)>0)
       If(x<id_sel.x AND x>id_sel.x-30)
         If(key(teclado[KUP])or Joys_Y==1); While (key(teclado[KUP])or Joys_Y==1) Frame; End
              graph++; If(graph>59) graph=50; End
           Else
             If(key(teclado[KDOWN])or Joys_Y==2); While (key(teclado[KDOWN])or Joys_Y==2) Frame; End
                  graph--; If(graph<50) graph=59; End
             End
         End
         numero=graph-50;
         combo[num]=numero;
       End
      End
      Frame;
   End
End

//---Sombras en la pared---//
Process sombraje(file,gfx,sombras)
private
  int blen;
begin
   return
   ctype=c_scroll;
   z=father.z+1;
    from blen=1 to gfx;
// Splinter Revisar      blendop_assign (file,blen,sombras);
    end
   Loop
      if(father.no_sombra)
          signal(id,s_kill);
      end
      If(exists(father))
        x=father.x+7;
        y=father.y;
        graph=father.graph;
        flags=father.flags;
        angle=father.angle;
       Else
        signal(id,s_kill);
      End
      Frame;
   End
End


//--Control de JOYSTICK--//
process JOYPAD()
begin
   select_joy(0);
   loop
    // ejeX
     IF (get_joy_position(0)<7000 AND get_joy_position(0)>-7000) joys_X=0; END // neutral
     IF (get_joy_button(JOY_BUTTON_DPAD_LEFT) or get_joy_position(0)<-16384-8192) joys_X=1; END // izquierda
     IF (get_joy_button(JOY_BUTTON_DPAD_RIGHT) or get_joy_position(0)>16384+8192) joys_X=2; END // derecha

    // ejeY
     IF (get_joy_position(1)<7000 AND get_joy_position(1)>-7000 ) joys_Y=0; END
     IF (get_joy_button(JOY_BUTTON_DPAD_UP) or get_joy_position(1)<-16384-8192) joys_Y=1; END // arriba
     IF (get_joy_button(JOY_BUTTON_DPAD_DOWN) or get_joy_position(1)>16384+8192) joys_Y=2; END // abajo

      frame;
   end
end

//-- Prota GOODY --//
//#Section: GOODY
Process GOODY(double x,y, int file,graph,flags,z,action)
Private
    int incx=0;
    int ultima_veloc=0;
    int Muevete=0;
    int Max_veloc=6;
    int Disparo=false;
    int Cabins;
    int Potencia;
    int Ascens;
    int Barcas1;
    int Barcas2;
    int Ascemovil;
    int MontaEscala;
    int Muros;
    int Men;
begin
    priority=100;
    //sombraje(ggoodys,24,sombras);
    accion=action;
    altura=20;
    Goodi=id;
    ctype=c_scroll;
    scroll.camera=id;
    timer[0]=0;
    timer[1]=0;

    Loop
        // PROGRAMADOR //
        If (ASCOP==1)
            If (key(_1)) y=10;x-=15; velocidad=0; End
            If (key(_2)) y=10;x+=15; velocidad=0; End
            If (key(_3)) cerveza=186; End
            If (key(_6))
                combo_caja="000000000000";
                split_combo_caja();
            End
            if(key(_7)) money=100; end
        End
        
        If (ASCOP==1)
            If (key(_4))
                objetos[0]=11; objeto[0]=11;
                objetos[1]=12; objeto[1]=12;
                objetos[2]=7; objeto[2]=7;
                objetos[3]=10; objeto[3]=10;
            End
            If (key(_5))
                objetos[0]=13; objeto[0]=13;
                objetos[1]=8; objeto[1]=8;
                objetos[2]=9; objeto[2]=9;
                objetos[3]=14; objeto[3]=14;
            End
        End
        //-------------//

        //-- PAUSA de PROCESO --//
        While(pausado) Frame; End


        //--Controla el tiempo de espera--//
        /*
        if((scan_code==0 and Joys_x==0 and Joys_y==0 and not get_joy_button(JOY_BUTTON_Y)
        and not get_joy_button(JOY_BUTTON_B) and not get_joy_button(JOY_BUTTON_A)and not get_joy_button(JOY_BUTTON_X))
        and accion<>Espera and (en_pantalla() or ascensor))
        sentado++; if(sentado>150) accion=espera; muevete=0; graph=15; end
        else
        sentado=0;
        end
        */
        if((Joys_x==0 and Joys_y==0 and keyboard.scan_code==0 and accion<>Espera)
            and !get_joy_button(JOY_BUTTON_Y) and !get_joy_button(JOY_BUTTON_B)
            and !get_joy_button(JOY_BUTTON_A) and !get_joy_button(JOY_BUTTON_X)
            and (en_pantalla() or ascensor==0))
            sentado++; if(sentado>150) accion=espera; muevete=0; graph=15; end
        else
            sentado=0;
        end

        //--Nivel de cerveza--//
        If (timer[0]>2500 AND ASCOP<=0)
            timer[0]=0;
            if(pausado) Cerveza-=0; else Cerveza-=18; end
        End
        If (cerveza<=0) size=100; accion=muerto; graph=10; End
        If (cerveza<50 AND get_id(Type mensaje)==0 AND men<1) men++; mensaje(2); End
        If (cerveza>50) men=0; End
        If (cerveza>186) cerveza=186; End
        If (vida_extra==3) vida_extra=0; vidas++; End

        //  Deteccion de Paredes  //
        Muros=collision(type Paredes_Proc);
        ultima_veloc=velocidad;
        If (ultima_veloc<>0)
            incx=ultima_veloc/abs(ultima_veloc);
            While (ultima_veloc<>0)
                If (flags)
                    If (map_get_pixel(gnivel,mapa_dureza,((x-10)+incx)/2,(y-15)/2)<>Paredes)
                        If (map_get_pixel(gnivel,mapa_dureza,((x-10)+incx)/2,(y+15)/2)<>Paredes)
                            If (Muros==0)
                                x+=incx;
                            Else
                                If (muros)
                                    If (x<muros.x+25)
                                        x+=incx;
                                    Else velocidad=0;
                                    End
                                End
                            End
                        End
                    Else
                        velocidad=0;
                    End
                Else
                    If (map_get_pixel(gnivel,mapa_dureza,((x+10)+incx)/2,(y-15)/2)<>Paredes)
                        If (map_get_pixel(gnivel,mapa_dureza,((x+10)+incx)/2,(y+15)/2)<>Paredes)
                            If (Muros==0)
                                x+=incx;
                            Else
                                If (muros)
                                    If (x>muros.x-25)
                                        x+=incx;
                                    Else velocidad=0;
                                    End
                                End
                            End
                        End
                    Else
                        velocidad=0;
                    End
                End
                ultima_veloc-=incx;
            End
        End
        //-- Acciones de GOODY --//
        //--Deteccion de Cabinas de Herramientas-//
        Cabins=collision(Type cabinas);
        If (Cabins) cabina_in=1; Else cabina_in=0; End

        //--Deteccion de Ascensores--//
        If (ascensor==0) ascens=collision(Type ascensores); End
        If (ascens AND velocidad_gravedad>=0 AND ascensor==0)
            ascemovil=ascens;
            If(ascemovil.graph==2)
                If (y+22>ascemovil.y-20 AND y+22<ascemovil.y+22
                    AND x>ascemovil.x-44  AND x<ascemovil.x+44)
                    ascensor=1; velocidad_gravedad=0;
                    accion=parado; graph=1;
                End
            Else
                If (y+22>ascemovil.y-20 AND y+22<ascemovil.y+22
                    AND x>ascemovil.x-144 AND x<ascemovil.x+144)
                    ascensor=1; velocidad_gravedad=0;
                    accion=parado; graph=1;
                End
            End
        End
        If (ascemovil<>0 AND ascensor)
            If(ascemovil.graph==2)
                If (x<ascemovil.x-46 OR x>ascemovil.x+46)
                    ascensor=0;
                End
            Else
                If(x<ascemovil.x-145 OR x>ascemovil.x+145)
                    ascensor=0;
                End
            End
        End
        //--Deteccion de BARCAS--//
        If (barca==0) Barcas1=collision(Type Barcas); End
        If (Barcas1 AND velocidad_gravedad>-1 AND Barca==0)
            Barcas2=Barcas1;
            If (y+22>Barcas2.y-20 AND y+22<Barcas2.y+42
                AND x>Barcas2.x-58  AND x<Barcas2.x+58)
                y=Barcas2.y-23; Barca=1; velocidad_gravedad=0;
                accion=parado; graph=1;
            End
        End
        If (Barcas2<>0 AND Barca)
            If (x<Barcas2.x-50 OR x>Barcas2.x+50)
                Barca=0; velocidad_gravedad=-2;
            End
        End
        //--Deteccion de Escalera--//
        If (get_id(Type escala)>0)
            If (Escalera==1 AND escaleras==0 )
                If (x+15>escalon.x AND x-15<escalon.x
                    AND y+20>escalon.y-134 AND y<escalon.y+5)
                    escaleras=1; graph=1;
                End
            End
            If (escalera==1 AND escaleras==1 AND x+15<escalon.x OR x-15>escalon.x)
                escaleras=0; accion=parado; size=100;
            End
        End
        If (NOT ascensor AND NOT Escaleras AND NOT Barca) gravedad(); End
        // --Cogido por Ladron--//
        If (atraco)  While (atraco) gravedad(); graph=23; Frame; End End
        //--Cogido por policia--//
        If (Policias)  While (policias) gravedad(); graph=7; Frame; End End

        //--Golpeado por un huevo--//
        If (huevazo==1 or huevazo==3)
            While(huevazo==1 or huevazo==3)
                gravedad();
                If (NOT en_pantalla())
                    graph=18;
                Else
                    graph=19; huevazo=2;
                End
                Frame;
            End
            Huevazo=3; mueve=0;
            scroll.camera=0;
            Repeat
                mueve++;
                Switch(mueve)
                    Case 1,5,9,13,17,21,25: x+=2; End
                    Case 3,7,11,15,19,23,27: x-=2; End
                End
                Frame;
            Until(mueve>29 or huevazo==0);
            graph=1; Huevazo=0;
            scroll.camera=id;
        Else
            //--Entramos en la Ferreteria--//
            If(Ferret)
                While(Ferret)
                    velocidad=0;
                    gravedad();
                    If (NOT en_pantalla())
                        graph=7;
                    Else
                        graph=1; Ferret=0;
                        signal(id,s_freeze);
                    End
                    Frame;
                End
            end
            //--Golpeado por un corazon--//
            If (corazonazo)
                While(corazonazo)
                    gravedad();
                    If (NOT en_pantalla())
                        graph=7;
                    Else
                        graph=14; corazonazo=0;
                    End
                    Frame;
                End
                corazonazo=1; mueve=0;
                scroll.camera=0;
                Repeat
                    mueve++;
                    Switch(mueve)
                        Case 1,5,9,13,17,21,25: x+=2; End
                        Case 3,7,11,15,19,23,27: x-=2; End
                    End
                    Frame;
                Until(mueve>29 or corazonazo==0);
                graph=1; corazonazo=0;
                scroll.camera=id;
            End
        End
        //-- Estamos en el suelo, en el ascensor o en la escalera o en una Barca--//
        If (en_pantalla() OR ascensor OR Escaleras OR Barca)
            Agachate=0;
            If (ascensor)
                If(ascens.graph==2)
                    If (sube<0)
                        y=ascens.y-35;
                    Else
                        y=ascens.y-27;
                    End
                    If (sube==0)
                        y=ascens.y-31;
                    End
                Else
                    y=ascens.y-31;
                    If (map_get_pixel(gnivel,mapa_dureza,(x-15)/2,y/2)<>Paredes AND
                        map_get_pixel(gnivel,mapa_dureza,(x+15)/2,y/2)<>Paredes)
                        If (velocidad==0)
                            If(ascens.mueve) x+=5; Else x-=5; End
                        End
                    End
                End
            End
            If (Barca)
                If(x<barcas1.x-35 OR x>barcas1.x+35)
                    y=barcas1.y-30;
                Else
                    y=Barcas1.y-23;
                End
                angle=barcas1.angle;
                If (map_get_pixel(gnivel,mapa_dureza,(x-20)/2,y/2)<>Paredes)
                    If (velocidad==0) x-=3; End
                End
            Else
                angle=0;
            End

            Switch(accion)
                Case saltar:
                    accion=Parado; graph=1;
                    velocidad=0; max_veloc=6;
                    velocidad_gravedad=0;
                End
                Case Parado:
                    If ((key(teclado[KUP]) OR key(teclado[KDOWN]) or (Joys_Y==1 or Joys_Y==2)) AND Escaleras)
                        If (y<escalon.y+3)
                            accion=subescalon; graph=12;
                            x=escalon.x; velocidad=0;
                        End
                    End
                    If (velocidad<>0)
                        If (velocidad>0) velocidad--; End
                        If (velocidad<0) velocidad++; End
                        Muevete++; If (Muevete>=3) graph++; Muevete=0; End
                        If (graph>4) graph=1; End
                    Else
                        If (NOT escaleras) graph=1; End
                    End

                    //--Rutina de control de aceleracion y salto--//
                    If(NOT(key(teclado[KLEFT]) AND key(teclado[KRIGHT])))
                        If ((key(teclado[KUP]) OR
                            (key(teclado[KUP]) AND key(teclado[KLEFT]) AND NOT escaleras) OR
                            (key(teclado[KUP]) AND key(teclado[KRIGHT])) OR
                            (get_joy_button(JOY_BUTTON_Y)>0) or Joys_Y==1) AND
                            NOT escaleras)

                            accion=saltar; graph=6;
                            max_veloc=7;
                            If (velocidad<>0)
                                velocidad_gravedad=-18;
                                velocidad_gravedad-=(abs(velocidad)-4);
                            Else
                                velocidad_gravedad=-18;
                            End
                            ascensor=0; Barca=0;
                            sound_play(s_salto,0);
                        End

                        If ((key(teclado[KLEFT]) or Joys_x==1) AND velocidad>-max_veloc)
                            velocidad-=2; flags=1;
                        Else
                            If ((key(teclado[KRIGHT]) or Joys_x==2) AND velocidad<max_veloc)
                                velocidad+=2; flags=0;
                            Else
                                If ((key(teclado[KDOWN]) Xor Joys_y==2)AND NOT escaleras)
                                    graph=5; velocidad=0;
                                    While((key(teclado[KDOWN])Xor joys_y==2) and not agachate)
                                        If (map_get_pixel(gnivel,mapa_dureza,(x-20)/2,y/2)<>Paredes)
                                            If (velocidad==0 AND barca) x-=3; End
                                        End
                                        Frame;
                                    End
                                End
                            End
                        End
            
                        //--Rutina de potencia de disparo y lanzamiento--//
                        If(potencia>0) potencia--; End
                        If (lanzar==0 AND stage<>0)
                            If ((key(teclado[KSPACE]) or (get_joy_button(JOY_BUTTON_X)>0))AND potencia<=14)
                                disparo=true;
                                Potencia+=2;
                                Potencia2-=2;
                            Else
                                If(NOT (key(teclado[KSPACE]) or get_joy_button(JOY_BUTTON_X)>0) AND disparo)
                                    Muevete=0;
                                    accion=disparar; graph=8;
                                    velocidad=0;
                                    disparo=false;
                                End
                            End
                        End
                    End

                    //--Ponemos la escalera--//
                    If ((key(teclado[KENTER]) or get_joy_button(JOY_BUTTON_B)>0) AND ascensor==0 AND barca==0)// AND escalera==0)
                        graph=1;
                        While(key(teclado[KENTER])or get_joy_button(JOY_BUTTON_B)>0) Frame; End
                        If (stage==2 AND x<700)
                            stage=2;
                        Else
                            If (Escalera==0)
                                Escala(x,y,file,22,z+1);
                            Else
                                If (get_id(Type mensaje)==0 AND escaleras==0
                                    AND ((x<escala_x-150 OR x>escala_x+150)
                                    OR (y<escala_y-250 OR y>escala_y+250)))
                                    Mensaje(1);
                                End
                            End
                        End
                    End
                End// fin accion 0:parado - andando //
            
                Case disparar:
                    muevete++; Lanzar=1;
                    If (muevete>1) graph=9; End
                    if (muevete==3)
                        Ladrillo(x,y-15,file,20,z-1,potencia,flags);
                        potencia=0; potencia2=33;
                        sound_play(s_disparo,0);
                    end
                    If (muevete>3) graph=1;
                        accion=parado; muevete=0;
                    End
                End

                Case muerto:
                    Repeat
                        muevete++;
                        Switch(muevete)
                            Case 3: sound_play(s_atraco,0); End
                            Case 10,30: graph=11; End
                            Case 20,40: graph=10; End
                        End
                        Frame;
                    Until(muevete>50)
                    no_sombra=1;
                    cerveza=186;
                    vidas--;
                    If (vidas==-1)
                        If (ASCOP) vidas=5;
                        Else
                            Game_over2(320,180,0);
                            signal(id,s_freeze);
                        End
                    End
                    Renace(x,y,0); signal(id,s_sleep);
                End

                //--Esperamos sentados --//
                case Espera:
                    muevete++;
                    switch (muevete)
                        case 5: graph=16; end
                        case 10: graph=17; muevete=0; end
                    end
                    if ((key(teclado[KUP]) or key(teclado[KDOWN]) or key(teclado[KRIGHT])
                        or key(teclado[KLEFT]) or key(teclado[KSPACE])) and get_id(type game_over)==0)
                        accion=parado; sentado=0;
                    end
                    if ((Joys_x<>0 or Joys_y<>0 or get_joy_button(JOY_BUTTON_Y)>0 or get_joy_button(JOY_BUTTON_B)>0
                        or get_joy_button(JOY_BUTTON_A)>0 or get_joy_button(JOY_BUTTON_X)>0) and get_id(type game_over)==0)
                        accion=parado; sentado=0;
                    end
                end

                Case subescalon://--Subimos o bajamos por la escalera--//
                    velocidad=0;
                    If (y+22<=escalon.y-134 OR y>=escalon.y+4)
                        if(y>escalon.y+3) y=escalon.y+2; end
                        disparo=false;
                    Else
                        If (graph<12) graph=12; End
                    End
                    If ((key(teclado[KUP]) or Joys_Y==1)AND y+22>escalon.y-132 AND
                        map_get_pixel(gnivel,mapa_dureza,x/2,(y-33)/2)<>Techos)
                        If (NOT key(teclado[KLEFT]) OR key(teclado[KRIGHT]))
                            muevete++;
                            If (muevete>=3) muevete=0; graph++; y-=12;sound_play(s_escala,0); End
                            If (graph>13) graph=12; End
                        End
                    Else
                        If (NOT key(teclado[KLEFT]) OR key(teclado[KRIGHT]))
                            If ((key(teclado[KDOWN])or Joys_Y==2)AND y<escalon.y-3)
                                muevete++;
                                If (muevete>=3) muevete=0; graph++; y+=12;sound_play(s_escala,0); End
                                If (graph>13) graph=12; End
                            End
                        End
                    End
                    If ((key(teclado[KLEFT]) OR key(teclado[KRIGHT])) AND NOT (key(teclado[KUP]) OR key(teclado[KDOWN])))
                        accion=parado; graph=1; size=100;
                        disparo=false;
                    End
                    If ((Joys_x==1 OR Joys_x==2) AND NOT (Joys_y==1 OR Joys_y==2))
                        accion=parado; graph=1; size=100;
                        disparo=false;
                    End
                    If (y+22<=escalon.y-132 AND (key(teclado[KUP])or Joys_Y==1))
                        accion=saltar;
                        Escaleras=0;
                        size=100; graph=6;
                        velocidad_gravedad=-17;
                        sound_play(s_salto,0);
                    End
                End
            End

        Else // estamos en el aire //
            Switch(accion)
                Case Saltar:
                    If(potencia>0) potencia--; End
                    If ((key(teclado[KSPACE]) or get_joy_button(JOY_BUTTON_X)>0)AND potencia<=14 AND stage<>0)
                        disparo=true;
                        Potencia+=2;
                        Potencia2-=2;
                    Else
                        If(NOT (key(teclado[KSPACE]) or get_joy_button(JOY_BUTTON_X)>0) AND disparo)
                            Muevete=0;
                            accion=disparar; graph=8;
                            velocidad=0;disparo=false;
                            sound_play(s_disparo,0);
                        End
                    End
                    If (velocidad_gravedad>0)
                        graph=7;
                        If (map_get_pixel(gnivel,mapa_dureza,(x+12)/2,y/2)==Paredes) x-=2; velocidad=0; end
                        If (map_get_pixel(gnivel,mapa_dureza,(x-12)/2,y/2)==Paredes) x+=2; velocidad=0; end
                    End
                    If(NOT (key(teclado[KLEFT]) AND key(teclado[KRIGHT])))
                        If ((key(teclado[KLEFT]) or joys_x==1) AND velocidad>-max_veloc)
                            velocidad-=2; flags=1;
                        Else
                            If ((key(teclado[KRIGHT])or joys_x==2) AND velocidad<max_veloc)
                                velocidad+=2; flags=0;
                            End
                        End
                    End
                End
            
                Case disparar:
                    muevete++;
                    If (muevete>1) graph=9; End
                    If (muevete>3) graph=1;
                        Ladrillo(x,y-15,file,20,z-1,potencia,flags);
                        accion=parado; muevete=0;
                        potencia=0; potencia2=33;
                    End
                End

                Case Parado:
                    graph=7; max_veloc=6;
                    If (velocidad<0) velocidad=-1;
                    Else
                        If (velocidad>0) velocidad=1; End
                    End
                End
            End
        End// fin en_pantalla//

        Frame;
    End
End

//--Caras de Goody en el SCORE--//
//#Section: Goody CARA
Process Caras(cara)
begin
   x=322; y=385; z=-50;
   file=gscore; graph=21;
   Switch(cara)
      Case 1:
           Repeat
               mueve++;
               Switch(mueve)
                  Case 1,6,12,18,24,30: graph=21; End
                  Case 3,9,15,21,27,33: graph=22; End
               End
               Frame;
           Until(mueve>36)
      End
      Case 2:
		   if (goodi.accion<>muerto) sound_play(s_queja,0); sentado=0;end
           if(goodi.accion==Espera) goodi.accion=parado; end
           If (get_id(Type mensaje)==0) Mensaje(11); End
           Repeat
               mueve++;
               graph=23;
               Frame;
           Until(mueve>24)
      End
      Case 3:
           Repeat
               mueve++;
               graph=24;
               Frame;
           Until(mueve>35)
      End
      Case 4:
		   if (goodi.accion<>muerto) sound_play(s_queja,0); sentado=0;end
           If (get_id(Type mensaje)==0) Mensaje(11); End
           Repeat
               mueve++;
               graph=25;
               Frame;
           Until(mueve>35)
      End
      Case 5:
           Repeat
               mueve++;
               Switch(mueve)
                  Case 1,6,12,18,24,30: graph=26; End
                  Case 3,9,15,21,27,33: graph=27; End
               End
               Frame;
           Until(mueve>36)
      End
      Case 6:
           Repeat
               mueve++;
               graph=28;
               Frame;
           Until(mueve>35)
      End
   End
End


//--Proceso para devolver a GOODY al punto de inicio--//
Process renace(double x,y, int re)
Private
    int Llega;
    int Busca;
begin
    ctype=c_scroll;
    scroll.camera=id;
    escalera=0; goodi.escaleras=0;
    signal(type escala,s_kill);

    Busca=Llegada(goody_x,goody_y);
    Repeat
        angle=get_angle(Busca);
        advance(30);
        If (get_dist(Busca)<30)
            Llega=1;
        End
        Frame;
    Until(llega>0)
    if ( exists( Busca ) ) signal(Busca,s_kill); end
    If (stage==12 OR stage==13 OR
        stage==17 OR stage==18 OR
        stage==23 OR stage==24)
        metro_muerto=1;
    End
    pausado=false;
    sound_stop(all_sound);
    huevazo=0;  corazonazo=0;
    Lanzar=0;   Policias=0;
    signal(Type ascensores,s_kill);
    signal(Type goody,s_kill);
    Goody(Goody_x,Goody_y,ggoody,1,Goody_flag,-10,parado);
    If (re==1) signal(father,s_wakeup); End
    constructor(stage); Frame;

End

Process Llegada(double x,y)
begin
    ctype=c_scroll;
    Loop
        Frame;
    End
End

//#Section: Ladrillo
Process Ladrillo(double x,y, int file,graph,z,poder,flags)
Private
   int vel_x;
   int vel_y;
   int caida;
   int frena;
   int Rueda;
   int ladrilla;
   int Pareds;
   int Golpe;
   int Volver;
begin
  ctype=c_scroll;
    //sombraje(ggoodys,24,sombras);
  altura=6;
  rueda=40;
  vel_x=14+poder;
  If (vel_x>25) vel_x=25; End
  If (flags) vel_x=-vel_x; End
  velocidad_gravedad=-(poder+6);
  Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
    //--Controla la aceleracion del ladrillo--//
    frena++;
    If(frena==2 AND vel_x<>0)
       frena=0;
       If (flags) vel_x++; Else vel_x--; End
    End
    If(flags) angle+=pi/rueda; Else angle-=pi/rueda; End
    x+=vel_x; y+=vel_y;
    volver++; If(volver==7) lanzar=0; End
    //--Si choca con la pared--//
     If(golpe==0)
        From Pareds=-6 To 6 Step 2;
            If (map_get_pixel(gnivel,mapa_dureza,x/2+pareds,y/2)==paredes OR rebote);
                golpe++; vel_x=-vel_x; velocidad_gravedad-=3;
                If (NOT rebote) sound_play(s_ladrillo,0); End
                rueda=7; rebote=0;
                If(flags) flags=0; vel_x-=6; Else vel_x+=6; flags=1; End
                Break;
            End
        End
     End
    //--Si toca el suelo--//
    gravedad();
    If (en_pantalla() AND velocidad_gravedad>=0)
       golpe++;
          If(NOT ladrilla) ladrilla=true; sound_play(s_ladrillo,0); End
       //--Si cae al suelo y deja de rodar se rompe--//
       If (golpe>=1)
           Lanzar=0; golpe=-1; no_sombra=1;
           rompeladrillo(file,21,x,y+10,z,size); size=0;
           Repeat mueve++; Frame; Until (mueve>5)
           signal(id,s_kill);
       End
    End
    //--Si se sale del escenario se rompe--//
    If (x<scroll.x0-200 OR x>scroll.x0+840 OR y<scroll.y0-200 OR y>scroll.y0+400)
        Lanzar=0; golpe=-1;
        rompeladrillo(file,21,x,y+10,z,size); size=0;
        Repeat mueve++; Frame; Until (mueve>5)
        signal(id,s_kill);
    End
    Frame;
  End
End

//--------- Rompe ladrillo ---------------//
Process rompeladrillo(file,graph,double x,y, int z, double size)
Private
    int incr_x;
    int n_estrellas;
    int Trozos;
begin
    ctype=c_scroll;
    sound_play(s_roto1,0);
    From n_estrellas=0 To 6;
        CLONE
            flags=rand(0,1);
            Break;
        End
    End

    incr_x=rand(-3,3);
    velocidad_gravedad=rand(-10,-5);
    n_estrellas=18;

    While (n_estrellas-->0);
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      size-=2;
      angle+=pi/8;
        x+=incr_x;
        y+=velocidad_gravedad;
        velocidad_gravedad+=2;
        Frame;
    End
End


//#Section: Escalera
Process Escala(double x,y, int file,graph,z)
Private
  int Ascen2;
  int coger;
  int pon;
  int pone;
begin
   y=y-2; size=100;
   ctype=c_scroll;
   escalon=id;

  If(escalera==0)

   if(not pone)
    If (goodi.flags)
        pon=30; x=x+5;
        repeat
            pon--;
            x--;
            //frame;
            If (map_get_pixel(gnivel,mapa_dureza,(x-4)/2,y/2)==paredescala)
               pon=0; pone=1; end
        until (pon==0);
      else
          pon=30; x=x-6;
          repeat
              pon--;
              x++;
              //frame;
              If (map_get_pixel(gnivel,mapa_dureza,(x+4)/2,y/2)==paredescala)
                 pon=0; pone=1; end
          until (pon==0);
    end
   end

   From pon=0 To 30;
      If (map_get_pixel(gnivel,mapa_dureza,(x+1)/2,(y+pon)/2)==color_dureza1)
         break;
       else
         if (pon==30)
            signal(id,s_kill); Frame;
         end
      End
   End

   From pon=0 To 140;
      If (map_get_pixel(gnivel,mapa_dureza,(x+8)/2,(y-pon)/2)==Techos OR
          map_get_pixel(gnivel,mapa_dureza,(x-8)/2,(y-pon)/2)==Techos)
           signal(id,s_kill); Frame;
      End
   End

   Ascen2=collision(Type ascensores);
   If (ascen2) signal(id,s_kill); Frame; End
  End

   //sombraje(ggoodys,24,sombras);
   Escalera=1; size=100;
   Escala_x=x;  Escala_y=y+2;
   Escala_stage=stage;
   Goodi.graph=1;

Loop
      Ascen2=collision(Type ascensores);
      coger=collision(Type goody);
      If ((coger AND (key(teclado[KENTER]) or get_joy_button(JOY_BUTTON_B)>0)) OR Ascen2)
         If (get_joy_button(JOY_BUTTON_B)>0); While(get_joy_button(JOY_BUTTON_B)>0) Frame; End End
         If (key(teclado[KENTER])); While(key(teclado[KENTER])) Frame; End End
         escalera=0; goodi.escaleras=0;
         goodi.accion=parado; goodi.size=100;
         signal(id,s_kill);
      End
      Frame;
   End
End


//-- Atacado por un enemigo --//
Process Goody_golpe(double x,y, int file,graph,z,porrazo)
Private
   int vel_x;
   int frena;
begin
   Altura=22;
   ctype=c_scroll;
   scroll.camera=id;
   flags=father.flags;
   Lanzar=0;
   If (flags) vel_x=10; Else vel_x=-10; End
   If (porrazo==1)cerveza-=54; End
   If (porrazo==2)velocidad_gravedad=-20; End
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(porrazo)
         Case 1,2:
             frena++;
             If (porrazo==2)
                gravedad();
             End
             If (map_get_pixel(gnivel,mapa_dureza,(x-18)/2,(y-15)/2)==Paredes OR
                 map_get_pixel(gnivel,mapa_dureza,(x-18)/2,(y+15)/2)==Paredes OR
                 map_get_pixel(gnivel,mapa_dureza,(x+18)/2,(y-15)/2)==Paredes OR
                 map_get_pixel(gnivel,mapa_dureza,(x+18)/2,(y+15)/2)==Paredes)
                Vel_x=0;
             End
             x+=vel_x;
             If(frena==2) frena=0;
                If (flags) vel_x--; Else vel_x++; End
             End
             If (vel_x==0)
                goodi.x=x; goodi.y=y+5; goodi.flags=flags;
                goodi.velocidad=0; goodi.accion=parado;
                scroll.camera=goodi; Lanzar=0;
                goodi.velocidad_gravedad=0;
                signal(Type goody,s_wakeup);
                signal(id,s_kill);
             End
             if (x<scroll.x0-200 or x>scroll.x0+850)
                goodi.accion=muerto;
             end
         End
      End
      Frame;
   End
End

//#Section: Mensajes
Process Mensaje(mensa)
Private
   int titulo;
begin
 if (idioma&1)
   Switch(mensa)
      Case  1: Titulo=write(l_mensajes,320,452,4,"DONDE ESTARA MI ESCALERA!!"); End
      Case  2: Titulo=write(l_mensajes,320,452,4,"NECESITO TOMARME UNA CERVECITA..."); End
      Case  3: Titulo=write(l_mensajes,320,452,4,"AHHH!! ESTA NO FUNCIONA AQUI!!!"); End
      Case  4: Titulo=write(l_mensajes,320,452,4,"UMMM... QUE RICA ESTA!!"); End
      Case  5: Titulo=write(l_mensajes,320,452,4,"HOMBRE! OTRO NUMERITO PARA EL NENE!!"); End
      Case  6: Titulo=write(l_mensajes,320,452,4,"SERAS CHORIZOOO!!"); End
      Case  7: Titulo=write(l_mensajes,320,452,4,"GLGLGLGLG..."); End
      Case  8: Titulo=write(l_mensajes,320,452,4,"SI NO HAY MONEY NO HAY HERRAMIENTAS"); End
      Case  9: Titulo=write(l_mensajes,320,452,4,"UFF! NO PUEDO CON MAS HERRAMIENTAS"); End
      Case 10: Titulo=write(l_mensajes,320,452,4,"DINERITO FRESCO!!"); End
      Case 11: Titulo=write(l_mensajes,320,452,4,"¡OUH!"); End
   End
  else
   Switch(mensa)
      Case  1: Titulo=write(l_mensajes,320,452,4,"OH MY GOD! I FORGOT MY LADDER!"); End
      Case  2: Titulo=write(l_mensajes,320,452,4,"I NEED A BEER!"); End
      Case  3: Titulo=write(l_mensajes,320,452,4,"OOPS! DOESN´T WORK HERE!"); End
      Case  4: Titulo=write(l_mensajes,320,452,4,"UMM… FEELS DELICIOUS!!!"); End
      Case  5: Titulo=write(l_mensajes,320,452,4,"ANOTHER NUMBER TO THE GLORY!!"); End
      Case  6: Titulo=write(l_mensajes,320,452,4,"YOU SON OF THE *****!!!"); End
      Case  7: Titulo=write(l_mensajes,320,452,4,"GLGLGLGLG..."); End
      Case  8: Titulo=write(l_mensajes,320,452,4,"SHOW ME DA MONEY!"); End
      Case  9: Titulo=write(l_mensajes,320,452,4,"UFF! I CANT´T CARRY MORE TOOLS..."); End
      Case 10: Titulo=write(l_mensajes,320,452,4,"FRESH MONEY!!"); End
      Case 11: Titulo=write(l_mensajes,320,452,4,"¡OUCH!"); End
   End
  end
   Repeat mueve++; Frame; Until(mueve>70);
   write_delete(titulo);
End

//#Section: Paredes
//-- Muros para herramientas --//
Process Paredes_proc(double x,y, int file,graph,z,muro)
begin
   ctype=c_scroll;
   Loop
       If (Pared[muro]==1);
            signal(id,s_kill);
       End
      Frame;
   End
End


//#Section: Cabinas
//-- Cabina de herramientas--//
Process cabinas(double x,y, int file,graph,z,abrir,herra)
Private
   int Goody2;
   int Ferreteria;
   int sirena;
   int Pasta;
begin
   ctype=c_scroll;
   Loop
      goody2=collision(Type goody);
      If (goody2 AND (key(teclado[KSPACE]) or get_joy_button(JOY_BUTTON_Y)>0 or get_joy_button(JOY_BUTTON_X)>0) AND get_id(type Paredes_Proc)>0
           AND Objetos[3]<>0)
         While(key(teclado[KSPACE])or get_joy_button(JOY_BUTTON_Y)>0 or get_joy_button(JOY_BUTTON_X)>0) Frame; End
         If (objetos[3]==abrir)
            cabina=true; Herracabina(x,y-2,file,objetos[3],-8);
            cabin[herra]=1;
            objetos[3]=0; Objeto[3]=0;
            sound_play(s_herram,0);
            If (get_id(type Paredes_Proc)<>0)
               Pared[herra]=1;
            End
           Else // -- Vamos a la CARCEL!! -- //
             Ferreteria=(objetos[3])-7;
             Herramientas2[ferreteria]=0;
             objetos[3]=0; Objeto[3]=0;
             If (get_id(Type mensaje)==0) Mensaje(3); End
             sound_play(s_rejas,0);
             sirena=sound_play(s_sirena,0);
             signal(Type goody,s_freeze);
             graph=20;
               From pasta=0 To max_money;
                  If (bolsa_dinero[pasta]==1)
                        Bolsa_dinero[pasta]=0;
                        Break;
                  End
               End
             Repeat
                mueve++;
                 fade(150,40,40,255,315); While(fade_info.fading) Frame; End
                 fade_on(315); While(fade_info.fading) Frame; End
                Frame;
             Until(mueve>4);
             sound_stop(sirena);
             If(ASCOP==2 or ASCOP==3)
                 signal(Type goody,s_wakeup);
                 graph=16; mueve=0;
                 fade_on(FADE_TIME);
               Else
                 goody_x=600; goody_y=285; Goody_flag=1;
                 stage=0; escalera=0;
                 nivel(stage);
             End
         End
      End
      Frame;
   End
End

//--Herramienta en cabina--//
Process Herracabina(double x,y, int file,graph,z)
begin
   ctype=c_scroll;
   Loop
      Frame;
   End
End

//#Section: Objetos EXTRA
Process  Objeto_extra(double x,y, int file,graph,z,objeto,pantalla)
begin
   ctype=c_scroll;
   Loop
        prota=collision(Type goody);
        Switch(objeto)
           Case 1://-- Bolsa de MONEY --//
              If (prota AND size==100)
                 If (get_dist(prota)<35)
                     If (ASCOP==2 or ASCOP==3) money+=ASCOP*100; else money+=100; End
                     sound_play(s_atraco2,0);
                     Bolsa_dinero[pantalla]=1;
                     Caras(1);
                     subepunt(x,y-5,file,2,z);
                     If (get_id(Type mensaje)==0) Mensaje(10); End
                     Repeat  size-=10;  Frame; Until (size<=0)
                     Break;
                 End
              End
           End
           Case 2://-- Tanque de Cerveza --//
              mueve++;
              switch (mueve)
                 case  3: graph=41; end
                 case  7: graph=42; end
                 case 11: graph=40; mueve=0; end
              end
              If (prota AND size==100 AND cerveza<175)
                 If (get_dist(prota)<25)
                     Cerveza+=130;
                     Jarra_cerveza[pantalla]=1;
                     Repeat  size-=10;  Frame; Until (size<=0)
                     Repeat
                        eruct++;
                        If (eruct==10)
                             If (get_id(Type mensaje)==0) mensaje(4); End
                        End
                        Frame;
                     Until(eruct>14)
                     sound_play(s_eructo,0);
                     Break;
                 End
              End
           End
           Case 3://-- Lata Combinación --//
              mueve++;
              switch (mueve)
                 case 3,11: graph=18; end
                 case 7: graph=19; end
                 case 15: graph=17; mueve=0; end
              end
              if (graph>19) graph=17; end
              If (prota AND size==100)
                 If (get_dist(prota)<25)
                     sound_play(s_lata,0);
                     Combina[pantalla]=1;
                     Combo_caja+=(string)rand(0,9);
                     split_combo_caja();
                     Vida_extra++;
                     Caras(1);
                     If (get_id(Type mensaje)==0) mensaje(5); End
                     Repeat  size-=10;  Frame; Until (size<=0)
                     Break;
                 End
              End
           End
        End
      Frame;
   End
End


//-- Puntos money --//
process subepunt(double x,y, int file,graph,z)
begin
   ctype=c_scroll;
   repeat
       y-=2; mueve++;
       frame;
   until(mueve>35)
end



//--- Decorados ---//
//#Section: Decorados

Process Cubo_basura(double x,y, int file,graph,z,ratas)
Private
  int Sale_rata;
  int cierra;
begin
   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
       sale_rata=rand(1,120);
       If (ratas AND sale_rata==5 AND cierra==0 AND get_id(Type rata)==0
           AND get_dist(goodi)>150)
          graph=4;
            rata(x,y-20,Genemigos,5,-10,1,0,0);
       End
       If (graph==4) cierra++; If (cierra>=30) cierra=0; graph=3; End End
      Frame;
   End
End

//#Section: Escenario
//----Escalera de madera , luces de vela y farolas----//
Process Escenario(double x,y, int file,graph,z,alpha)
Private
    int luz;
begin
   if (x<>166) ctype=c_scroll; end
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(graph)
         case 1..4:
              alpha=90;
              //flags=16;
              graph++;
              if(graph>4) graph=1; end
         end
         Case 5:
             If (goodi.y>y) z=goodi.z+1; Else z=goodi.z-1; End
         End
         Case 6..8:
             alpha=120;
             mueve++; If (mueve>4) mueve=0; End
             Switch(mueve)
                Case 0: size=100;  End
                Case 2: size=96; End
             End
/*
             luz++;
             Switch(luz)
                Case 1..13:  alpha+=6;  End
                Case 14..27: alpha-=6;  End
                Case 28: luz=1; alpha=70; End
             End
*/
         End
         Case 30..32:
            mueve++; If (mueve==4) mueve=0; graph++; End
            If (graph>32) graph=30; End
         End
      End
      Frame;
   End
End

//---- Ascensores ----//
//#Section: Ascensores

Process Ascensores(double x,y, int file,graph,z,tope_up,tope_down)
Private
   int Tocado;
   int vel_y;
   int vel_x;
   int choque;
   int montado;
   int dir;
begin
   priority=90;
   vel_x=5;
   vel_y=sube;
   dir=sube;
   If (stage==27) vel_y=0; End
   If (graph==2) accion=1; Else accion=3; End
   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End

      Tocado=collision(Type goody);
      If(stage==27 AND key(teclado[KDOWN]) AND vel_y==0)
          While(key(teclado[KDOWN])) Frame; End
          vel_y=4;
      End
      //If(get_id(Type renace)<>0) sube=dir; End
      Switch(accion)
         Case 1:
            If (goodi.x>x-60 AND goodi.x<x+60) sube=vel_y; End
            y+=vel_y;
            If (y<tope_up) y=tope_up; vel_y=-vel_y;
              Else
                If (y>tope_down) y=tope_down; vel_y=-vel_y; End
            End
            if(stage<>14 and x<>391)
              If (y<goodi.y-250) y=goodi.y-250; vel_y=-vel_y;
                Else
                  If (y>goodi.y+250) y=goodi.y+250; vel_y=-vel_y; End
              End
            end
            If (key(teclado[KDOWN]) or Joys_Y==2)
                While(key(teclado[KDOWN])or Joys_Y==2) Frame; End
                choque=1; accion=2;
                Sube=0; dir=vel_y;
            End
            if(goodi.ascensor and vel_y<1)
               from montado=-15 to 15;
                 if (map_get_pixel(gnivel,mapa_dureza,(goodi.x+montado)/2,(goodi.y-20)/2)==Techos)
                     choque=1; accion=2;
                     Sube=0; dir=vel_y; y+=1;
                     Break;
                 end
               end
            end

            If (tocado AND y<goodi.y-15 AND vel_y>0) vel_y=-vel_y; End
         End
         Case 2:
           If (map_get_pixel(gnivel,mapa_dureza,(goodi.x)/2,(goodi.y-20)/2)<>Techos)
             If (key(teclado[KDOWN]) or Joys_Y==2)
                While(key(teclado[KDOWN])or Joys_Y==2) Frame; End
                accion=1; choque=0; vel_y=dir; sube=dir;
             End
           end
            If (NOT tocado AND choque==0)
                accion=1; sube=dir;
            End
         End
         Case 3:
            x+=vel_x;
            If (x<tope_up) x=tope_up; vel_x=-vel_x; mueve=1;
              Else
                If (x>tope_down) x=tope_down; vel_x=-vel_x; mueve=0; End
            End

         End
      End
      Frame;
   End
End


//--- Barcas del retiro ---//
//#Section: Barcas RETIRO
Process Barcas(double x,y, int file,graph,z)
Private
    int vel_x=3;
    int goodi2;
    int monta_y;
    int direc=1;
begin
    ctype=c_scroll;
    z=goodi.z-2;
    monta_y=y;
    Loop
        //-- PAUSA de PROCESO --//
        While(pausado) Frame; End

        goodi2=collision(Type goody);
        If (goodi2 AND (goodi.x>x-30 AND goodi.x<x+30))
            mueve++;
            Switch(mueve)
               Case 1: y=y+5; End
               Case 4: y=y-5; End
            End
          Else
            If (NOT goodi2) mueve=0; y=monta_y; End
        End
        If (goodi.accion==muerto) vel_x=0; Else vel_x=3; End
        x-=vel_x;
        If (x<=60) x=1890; End
      Switch(direc)
         Case 0:
            angle+=250;
            If(angle>3500) direc=1; End
         End
         Case 1:
            angle-=350;
            If(angle<-3500) direc=0; End
         End
      End
       Frame;
    End
End


//--- Vagón del METRO ---//
//#Section: METRO
Process Metro (double x,y, int file,graph,z,vagon)
Private
   int vel_x=35;
   int puertas;
begin
   signal(Type limpiadora,s_kill);
   signal(Type polvos,s_kill);
   signal(father,s_sleep);
   ctype=c_scroll;
   sound_play(s_metro,0);
   If (metro_muerto) metro_muerto=0; End
   Loop
    	if (z==goodi.z) z+=1; end
      //--Pausa PROCESO--//
      While(pausado) Frame; End

      Switch (vagon)
         Case 1:
            If (vel_x>0) x+=vel_x; vel_x-=1; End
            If (vel_x<=0) vel_x=0; End
            If(NOT puertas AND vel_x==0)
                Puerta_metro1(x-165,y+16,file,21,z+2,1,1);
                Puerta_metro1(x+167,y+16,file,21,z+2,0,1);
                puertas=1; graph=20;
            End
            If (mueve)
               Repeat  If (vel_x<20) vel_x++; End x+=vel_x; Frame;  Until(x>1600) Mueve=2;
            End
            If (mueve==2)
                fade_off(875); While(fade_info.fading) Frame; End
                goody_x=480; goody_y=268; Goody_flag=0;
                nivel(stage);
                signal(id,s_kill);
            End
         End
         Case 2:
            If (vel_x>0) x-=vel_x; vel_x-=1; End
            If (vel_x<=0) vel_x=0; End
            If(NOT puertas AND vel_x==0)
                Puerta_metro1(x-166,y+16,file,21,z+2,0,2);
                Puerta_metro1(x+167,y+16,file,21,z+2,1,2);
                puertas=1; graph=20;
            End
            If (mueve)
               Repeat  If (vel_x<20) vel_x++; End x-=vel_x; Frame;  Until(x<-320) Mueve=2;
            End
            If (mueve==2)
                fade_off(875); While(fade_info.fading) Frame; End
                goody_x=775; goody_y=268; Goody_flag=1;
                nivel(stage);
                signal(id,s_kill);
            End
         End
      End
      Frame;
   End
End


//#Section: METRO2
Process Metro2 (double x,y, int file,graph,z,vagon)
Private
   int vel_x=35;
   int puertas;
begin
   ctype=c_scroll;
   sound_play(s_metro,0);
   If (metro_muerto) metro_muerto=0; End

   Loop
   	  if (z==goodi.z) z+=1; end
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch (vagon)
         Case 1:
            If (vel_x>0) x+=vel_x; vel_x-=1; End
            If (vel_x<=0) vel_x=0; End
            If(NOT puertas AND vel_x==0)
                Puerta_metro2(x-166,y+16,file,21,z+2,0,1);
                Puerta_metro2(x+167,y+16,file,21,z+2,1,1);
                puertas=1; graph=20;
                signal(Type goody,s_freeze);
            End
            If (mueve)
               Repeat  If (vel_x<20) vel_x++; End x+=vel_x; Frame;  Until(x>980)
               goody_tren(320,4);
               Escalera=0;
               signal(id,s_kill);
            End
         End
         Case 2:
            If (vel_x>0) x-=vel_x; vel_x-=1; End
            If (vel_x<=0) vel_x=0; End
            If(NOT puertas AND vel_x==0)
                Puerta_metro2(x-165,y+16,file,21,z+2,1,2);
                Puerta_metro2(x+167,y+16,file,21,z+2,0,2);
                puertas=1; graph=20;
                signal(Type goody,s_freeze);
            End
            If (mueve)
               Repeat  If (vel_x<20) vel_x++; End x-=vel_x; Frame;  Until(x<320)
               goody_tren(960,6);
               Escalera=0;
               signal(id,s_kill);
            End
         End
      End
      Frame;
   End
End


Process Puerta_metro1(double x,y, int file,graph,z,activa,estac)
begin
   ctype=c_scroll;
   sound_play(s_metro2,0);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
     Switch(estac)
       Case 1:
         If (activa AND NOT father.mueve)
             Repeat x+=3; Frame; Until (x>860)
             goodi.z=z+2;
             Repeat x-=3; Frame; Until (x<787)
             if ( exists( goodi ) ) signal(goodi,s_kill); end
             father.mueve=1;  father.graph=22;
             sound_play(s_metro,0);
             signal(Type puerta_metro1,s_kill);
         End
       End
       Case 2:
         If (activa AND NOT father.mueve)
             Repeat x-=3; Frame; Until (x<405)
             goodi.z=z+2;
             Repeat x+=3; Frame; Until (x>475)
             if ( exists( goodi ) ) signal(goodi,s_kill); end
             father.mueve=1;  father.graph=22;
             sound_play(s_metro,0);
             signal(Type puerta_metro1,s_kill);
         End
       End
     End
     Frame;
   End
End


Process Puerta_metro2(double x,y, int file,graph,z,activa,estac)
begin
   ctype=c_scroll;
   sound_play(s_metro2,0);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
     Switch(estac)
       Case 1:
         If (activa AND NOT father.mueve)
             Repeat x-=3; Frame; Until (x<405)
             goodi.z=z-2;
             Repeat x+=3; Frame; Until (x>475)
             father.mueve=1;  father.graph=22;
             sound_play(s_metro,0);
             signal(Type puerta_metro2,s_kill);
         End
       End
       Case 2:
         If (activa AND NOT father.mueve)
             goodi.z=z+1;
             Repeat x+=3; Frame; Until (x>850)
             goodi.z=z-2;
             Repeat x-=3; Frame; Until (x<777)
             father.mueve=1;  father.graph=22;
             sound_play(s_metro,0);
             signal(Type puerta_metro2,s_kill);
         End
       End
     End
     Frame;
   End
End

Process Goody_tren(double x, int estacion)
Private
   int vel_x=10;
begin
   ctype=c_scroll;
   scroll.camera=id;
   Switch(estacion)
      Case 1:
         Repeat  x+=vel_x; Frame; Until(x>960)
         x=960; Frame;
         Metro(320,240,gscena,22,goodi.z+1,estacion);
      End
      Case 2:
         Repeat  x-=vel_x; Frame; Until(x<320)
         x=320; Frame;
         Metro(940,240,gscena,22,goodi.z+1,estacion);
      End
      Case 3:  x=320; Frame;  End

      Case 5:  x=960; Frame;  End

      Case 4:
         Repeat  x+=vel_x; Frame; Until(x>goody_x-10)
         signal(Type goody,s_wakeup);
         scroll.camera=goodi;
      End
      Case 6:
         Repeat  x-=vel_x; Frame; Until(x<goody_x+10)
         signal(Type goody,s_wakeup);
         scroll.camera=goodi;
      End
   End
End


//#Section: CAJA FUERTE
Process caja_fuerte(double x,y, int file,graph,z)
begin
    ctype=c_scroll;
    Loop
       Switch(graph)
          Case 40,41:
             If (fin>0) size=0; End
          /*
             If (key(teclado[KUP]) AND key(teclado[KSPACE]))
                While(key(teclado[KUP])) Frame; End
                angle+=pi/16;
             End
             If (key(teclado[KDOWN]) AND key(teclado[KSPACE]))
                While(key(teclado[KDOWN])) Frame; End
                angle-=pi/16;
             End
           */
          End
          Case 42:
            If(Fin)
               Repeat
                  mueve++;
                  Switch(mueve)
                     Case 1: graph=43; End
                     Case 5: graph=44; End
                     Case 9: graph=45; End
                  End
                  Frame;
               Until(mueve>=10);
               Fin=2;
            End
          End
       End
       Frame;
    End
End


//--- Enemigos ---//
//#Section: Enemigo: BUITRE

Process Buitre(double x,y, int file,graph,z,salida,flags)
Private
   int lanzars;
   int vel_y;
   int direc;
begin
   ctype=c_scroll;

   velocidad=rand(5,7);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
         Case 0:
            If (NOT exists(Type huevo)) lanzar=0; End
            mueve++; If(mueve>2) mueve=0; graph++; End
            If (graph>91) graph=90; End
            x+=velocidad;
            y+=vel_y;
            If (x>salida) signal(id, s_kill); End
              Switch(direc)
                 Case 0:
                    vel_y++;
                    If(vel_y>6) direc=1; End
                 End
                 Case 1:
                    vel_y--;
                    If(vel_y<-6) direc=0; End
                 End
              End
            if(not lanzars) rebote++; end
            If ((x>goodi.x-30 AND x<goodi.x+80) AND
                (NOT lanzars AND rebote>20))
               Lanzars=1; rebote=0; sound_play(s_buitre,0);
               huevo(x,y+25,file,94,z+1);
            End
         End
      End
      Frame;
   End
End


Process huevo(double x,y, int file,graph,z)
Private
   int goodi2;
begin
   ctype=c_scroll;
   velocidad_gravedad=-10;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End

      goodi2=collision(Type goody);
      gravedad();
      If (x>goodi.x+5) x-=2; End
      If (x<goodi.x-5) x+=2; End

      If(goodi2 AND huevazo<>2 AND huevazo<>3 AND
         goodi.accion<>muerto AND (goodi.graph<>18 OR goodi.graph<>19))
          huevazo=1; cerveza-=20;  sound_play(S_huevo,0);
          goodi.velocidad_gravedad=-15;
          goodi.graph=18;
          Agachate=1;
      End
      If(en_pantalla())
         mueve=0; graph=95; sound_play(S_huevo,0);
         Repeat
             mueve++;
             Frame;
         Until(mueve>10)
         signal(id,s_kill);
      End
      If (y>scroll.y0+300)
         sound_play(s_chapuzon,0);
         Chapuzon(x,285,genemigos,13,-10);
         signal(id,s_kill);
      End
      If (region_out(id,1) OR goodi2)
         signal(id,s_kill);
      End
      Frame;
   End
End

//#Section: Enemigo: GOTA AGUA

Process Gotas(double x,y, int file,graph,z,goteo)
Private
   int vel_y;
   int Goody2;
   int cae;
begin
    //sombraje(genemigos2,94,sombras);
   altura=10;
   vel_y=1;
   accion=1;
   ctype=c_scroll;
   cae=y;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Goody2=collision(Type goody);
      Switch (accion)
         Case 1:
            mueve++; If (mueve==2) mueve=0; vel_y++; End
            y+=vel_y;
            If (en_pantalla()OR goody2 OR y>scroll[0].y0+290)
               graph=8; accion=2; Mueve=0;
                If (x>scroll[0].x0 AND x<scroll[0].x0+620
                    AND y>scroll[0].y0 AND y<scroll[0].y0+340)
                    sound_play(s_gotas,0);
                End
                If (goody2 and goodi.accion<>muerto)
                    cerveza-=20; Lanzar=0;
                    Caras(2);
                    Agachate=1;
                End
            End
         End
         Case 2:
            mueve++;
            Switch(mueve)
               Case 3: graph=9; End
               Case 6: graph=10; End
               Case 9: size=0; End
               Case goteo:
                    y=cae; accion=1; graph=7;
                    size=100; Vel_y=2; mueve=0;
               End
            End
         End
      End
      Frame;
   End
End

//#Section: Enemigo: PIRAÑAS

Process Peces(double x,y, int file,graph,z, double size)
Private
   int vel_y;
   int Goody2;
   int golpea;
   int Salta;
   int golpe;
   int Ladrillos;
begin
   vel_y=-8;
   accion=0;
   salta=y;
   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      Goody2=collision(Type goody);
      Switch (accion)
         Case 0:
            mueve++;
             If(mueve>rand(35,60))
                mueve=0; accion=1; size=100; golpea=0; golpe=0;
                no_sombra=0; //sombraje(genemigos2,94,sombras);
                If (x>scroll[0].x0 AND x<scroll[0].x0+620
                    AND y>scroll[0].y0 AND y<scroll[0].y0+420)
                   sound_play(s_chapuzon,0); Chapuzon(x,y+5,file,13,z);
                End
             End
         End
         Case 1:
            mueve++; If (mueve==3) mueve=0; vel_y++; graph++; End
            y+=vel_y;
             If (map_get_pixel(gnivel,mapa_dureza,(x)/2,(y-10)/2)==Techos)
                vel_y=0;
             End
            If (graph>12) graph=11; End
            If (y>salta)
                Mueve=0;
                no_sombra=1;
                If (x>scroll[0].x0 AND x<scroll[0].x0+620
                    AND y>scroll[0].y0 AND y<scroll[0].y0+420)
                    sound_play(s_chapuzon,0); Chapuzon(x,y,file,13,z);
                End
                 accion=0; size=0; y=salta;
                 vel_y=rand(-9,-7);
            End
            If(goody2 AND golpea==0 and goodi.accion<>muerto)
                if (not goody2.escaleras)
                   goodi.velocidad_gravedad=-15;
                   goodi.accion=parado; goodi.graph=6;
                end
                golpea=1; cerveza-=20;
                goodi.velocidad=0;
                Lanzar=0; potencia2=33;
                Agachate=1;
                Caras(2);
            End
              If ((ladrillos) AND NOT golpe)
                  golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad=-5;
                  Mueve=0; sound_play(s_golpe1,0);

                  Repeat
                      mueve++; If (mueve==2) mueve=0; vel_y++; End
                      y+=vel_y+2;
                      angle+=pi/7;
                      Frame;
                  Until(y>salta);
                  sound_play(s_chapuzon,0); Chapuzon(x,y+5,file,13,z);
                  no_sombra=1;
                  signal(id,s_kill);

              End
         End
      End
      Frame;
   End
End

Process Chapuzon(double x,y, int file,graph,z)
begin
   ctype=c_scroll;
   Repeat
      mueve++;
        Switch(mueve)
           Case 3,6,9,12: graph++; End
        End
      Frame;
   Until(mueve>15)
End

//#Section: Enemigo: RATA
Process rata(double x,y, int file,graph,z,flags,zona1,zona2)
Private
    int vel_x;
    int Goody2;
    int Ladrillos;
    int ladri;
    int golpe;
    int inmune;
    int direc;
begin
     //sombraje(genemigos2,94,sombras);
    altura=10;
    vel_x=rand(1,3);

    ctype=c_scroll;
    Switch(stage)
        Case 1: zona1=100; zona2=900; End
        Case 2: zona1=875; zona2=1800; End
        Case 5:
            If (x>600) zona1=580; zona2=780; End
            If (x<400) zona1=50; zona2=300; End
        End
        Case 8: Zona1=940; Zona2=1180; End
        Case 14:zona1=580; zona2=890; End
        Case 19:zona1=80;  zona2=224; End
        Case 22:zona1=850;  zona2=1200; End
    End
    Loop
        //--Pausa PROCESO--//
        While(pausado) Frame; End
        Ladrillos=collision(Type Ladrillo);
        Ladri=collision(Type rompeladrillo);
        Goody2=collision(Type goody);
        gravedad();
        If (en_pantalla())
            x+=vel_x;
            mueve++; If (mueve>3) mueve=0; graph++; End
            If (graph>6) graph=5; End
            If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+5)/2)==paredes);
                vel_x=-vel_x; flags=1;
            Else
                If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+5)/2)==paredes);
                    vel_x=-vel_x; flags=0;
                End
            End
            direc++;
            if (direc==40 and rand(1,2)==1)
                direc=0;
                if(flags) vel_x=-vel_x; flags=0;
                else
                     vel_x=-vel_x; flags=1;
                end
            else
                if(direc>40) direc=0; end
            end
            If (x<=zona1 AND flags==0)
                vel_x=-vel_x; flags=1; direc=20;
            Else
                If (x>=zona2 AND flags)
                    vel_x=-vel_x; flags=0; direc=20;
                End
            End

            If (x<goodi.x-200 AND flags==0 AND x<zona2-200)
                vel_x=-vel_x; flags=1;
            Else
                If (x>goodi.x+200 AND flags  AND x>zona1+200)
                    vel_x=-vel_x; flags=0;
                End
            End

        End
        If (goody2 AND NOT atraco AND inmune==0 and goodi.accion<>muerto)
            if (not goody2.escaleras)
                goodi.velocidad_gravedad=-10;
                goodi.accion=parado; goodi.graph=6;
            end
            Agachate=1;
            cerveza-=25; inmune=25;
            Lanzar=0;
            Caras(2);
        End
        If(inmune>0) inmune--; End
        If (ladrillos AND NOT golpe)
            ladrillos.rebote=1; ladrillos.velocidad_gravedad=-5;
            velocidad_gravedad=-10;
            golpe=1;
            Mueve=0; sound_play(s_golpe1,0);
        end
        if(ladri and not golpe)
            if(ladri.velocidad_gravedad<-2)
                velocidad_gravedad=-10;
                golpe=1;
                Mueve=0; sound_play(s_golpe1,0);
            end
        end
        if(golpe)
            Repeat
                gravedad();
                angle+=pi/10;
                mueve++;
                Frame;
            Until(mueve>8);
            mas_vida++;
            signal(id,s_kill);
        end
        Frame;
    End
End

//#Section: Enemigo: SERPIENTE
Process Serpiente(double x,y, int file,graph,z,flags,zona1,zona2)
Private
  int vel_x;
  int Goody2;
  int Ladrillos;
  int ladri;
  int golpe;
  int Inmune=0;
  int direc;
begin
    //sombraje(genemigos2,94,sombras);
   altura=12;
   vel_x=rand(7,8);

   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      ladri=collision(type rompeladrillo);
      Goody2=collision(Type goody);
      gravedad();
      If (en_pantalla())
         mueve++; If (mueve>1) mueve=0; graph++; End
         If (graph>25) graph=22; x+=vel_x; End
        If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+5)/2)==paredes);
             vel_x=-vel_x; flags=1;
           Else
             If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+5)/2)==paredes);
                 vel_x=-vel_x; flags=0;
             End
        End
        direc++;
        if (direc==70 and rand(1,2)==1)
           direc=0;
           if(flags) vel_x=-vel_x; flags=0;
             else
                     vel_x=-vel_x; flags=1;
           end
          else
            if(direc>70) direc=0; end
        end

        If (x<=zona1 AND flags)
             vel_x=-vel_x; flags=0; direc=20;
           Else
              If (x>=zona2 AND flags==0)
                  vel_x=-vel_x; flags=1; direc=20;
              End
        End
        If (x<goodi.x-150 AND flags AND x<zona2-150)
              vel_x=-vel_x; flags=0;
          Else
             If (x>goodi.x+150 AND flags==0 AND x>zona1+150)
                   vel_x=-vel_x; flags=1;
             End
        End
      End
      If (goody2 AND NOT atraco AND inmune==0 and goodi.accion<>muerto)
          if (not goody2.escaleras)
               goodi.velocidad_gravedad=-10;
               goodi.accion=parado; goodi.graph=6;
          end
          cerveza-=25; inmune=20;
          Lanzar=0;
          Agachate=1;
          Caras(2);
     End
      If(inmune>0) inmune--; End
      If (ladrillos AND NOT golpe)
         ladrillos.rebote=1; ladrillos.velocidad_gravedad=-5;
         velocidad_gravedad=-10;
         golpe=1;
         Mueve=0; sound_play(s_golpe1,0);
      end
       if(ladri and not golpe)
          if(ladri.velocidad_gravedad<-2)
             velocidad_gravedad=-10;
             golpe=1;
             Mueve=0; sound_play(s_golpe1,0);
          end
       end
       if(golpe)
          Repeat
              gravedad();
              angle+=pi/10;
              mueve++;
              Frame;
          Until(mueve>8);
          mas_vida++;
          signal(id,s_kill);
       end
      Frame;
   End
End

//#Section: Enemigo: LADRON
Process Ladron(double x,y, int file,graph,z,flags,zona1,zona2)
Private
  int vel_x;
  int Goody2;
  int Ladrillos;
  int golpe;
  int Pasta;
  int Cuenta;
  int Money2;
  int direc;
  int Atracador;
begin
    //sombraje(genemigos2,94,sombras);
   altura=22;
   vel_x=rand(2,4);

   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      goody2=collision(Type goody);
      gravedad();
      If (en_pantalla() AND Atracador==0)
         x+=vel_x;
         mueve++; If (mueve>3) mueve=0; graph++; End
         If (graph>4) graph=3; End
        If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+10)/2)==paredes);
             vel_x=-vel_x; flags=1;
           Else
             If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+10)/2)==paredes);
                 vel_x=-vel_x; flags=0;
             End
        End
        direc++;
        if (direc==60 and rand(1,2)==1)
           direc=0;
           if(flags) vel_x=-vel_x; flags=0;
             else
                     vel_x=-vel_x; flags=1;
           end
          else
            if(direc>60) direc=0; end
        end
        If (x<=zona1 AND flags==0)
             vel_x=-vel_x; flags=1; direc=20;
           Else
              If (x>=zona2 AND flags)
                  vel_x=-vel_x; flags=0; direc=20;
              End
        End
        If (x<goodi.x-150 AND flags==0 AND x<zona2-200) vel_x=-vel_x; flags=1;
          Else
             If (x>goodi.x+150 AND flags AND x>zona1+200) vel_x=-vel_x; flags=0; End
        End

      End
      If (goody2 AND atraco==0 AND money>0
             AND get_dist(goodi)<30 AND cerveza>0
             AND goodi.accion<>muerto)
          goody2.size=100; Money2=money/100;
          Lanzar=0; If (get_id(Type mensaje)==0) mensaje(6); End
          Cuenta=sound_play(s_atraco2,-1);
          Atraco=1; Atracador=1;
          graph=37;
           If (goody2.x>x AND flags==0) flags=1; vel_x=-vel_x; End
            If (flags) x=goody2.x-47; goody2.flags=1;
               Else
                  x=goody2.x+47; goody2.flags=0;
            End
      End
      If (atraco and atracador)
         If (money>0) money-=5; Frame(200);
            Mueve++; If (mueve>1) mueve=0; graph++; End
            If (graph>38) graph=37; End
            Else
              If (money=<0)
                  atraco=0; money=0; Atracador=0;
                  sound_stop(cuenta);
                  Repeat
                     From pasta=0 To max_money;
                       If (bolsa_dinero[pasta]==1)
                           Bolsa_dinero[pasta]=0;
                           Money2-=Ascop+1;
                           If(money2<=0) Break; End
                       End
                     End
                     Frame;
                  Until (money2<=0)
              End
              graph=3; Mueve=0;
         End
      End

      If ((ladrillos) AND NOT golpe AND NOT atraco)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0;
//         If(atraco) atraco=0; graph=3; mueve=0; sound_stop(cuenta); End
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==5) sound_play(s_grito1,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         no_sombra=1;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


//#Section: Enemigo: KARATEKA
Process Karateka(double x,y, int file,graph,z,flags,zona1,zona2)
Private
  int vel_x;
  int Goody2;
  int Ladrillos;
  int golpe;
  int Patada;
  int direc;
begin
    //sombraje(genemigos2,94,sombras);
   accion=1;
   altura=23;
   vel_x=rand(-3,-2);
   vida=2;
   ctype=c_scroll;

   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      goody2=collision(Type goody);
      gravedad();
      Switch(accion)
         Case 1:
             x+=vel_x;
             mueve++; If (mueve>3) mueve=0; graph++; End
             If (graph>27) graph=26; End
             If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+10)/2)==paredes);
                 vel_x=-vel_x; flags=1;
               Else
                 If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+10)/2)==paredes);
                     vel_x=-vel_x; flags=0;
                 End
            End
            direc++;
            if (direc==60 and rand(1,2)==1)
               direc=0;
               if(flags) vel_x=-vel_x; flags=0;
                 else
                         vel_x=-vel_x; flags=1;
               end
              else
                if(direc>60) direc=0; end
            end
            If (x<=zona1 AND flags==0) vel_x=-vel_x; flags=1; direc=20;
               Else
                  If (x>=zona2 AND flags)  vel_x=-vel_x; flags=0; direc=20; End
            End
            If (x<goodi.x-200 AND flags==0 AND x<zona2-200)  vel_x=-vel_x; flags=1;
              Else
                 If (x>goodi.x+200 AND flags AND x>zona1+200) vel_x=-vel_x; flags=0; End
            End
            If (goody2 AND patada==0 AND cerveza>0 AND goodi.accion<>muerto)
                If (x<goodi.x AND flags==0) flags=1; vel_x=-vel_x; End
                If (x>=goodi.x AND flags) flags=0; vel_x=-vel_x; End
                accion=2; patada=1; sound_play(s_grito3,0);
                mueve=0;
                Agachate=1;
            End
         End
         Case 2:
            mueve++;
            Switch(mueve)
               Case 2:
                   graph=28;
                   If (goody2)
                      Lanzar=0;
                      if ( exists(goody2) ) signal(goody2,s_sleep); end
                      sound_play(s_golpe2,0);
                      Goody_golpe(goodi.x,goodi.y,goodi.file,7,goodi.z,1);
                      Caras(2);
                   End
               End
               Case 8: graph=26; accion=1; patada=0; golpe=0; End
            End
         End
      End
      If ((ladrillos) AND NOT golpe)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10; vida--;
         sound_play(s_golpe2,0); Mueve=0;
         If (vida==0)
             velocidad_gravedad=-13; Mueve=0;
             Repeat
                 gravedad();
                 Mueve++; y++;
                 If (flags) angle+=pi/20; Else angle-=pi/20; End
                 If (mueve==5) sound_play(s_grito1,0); End
                 Frame;
             Until(Mueve>10)
             size=0;
             no_sombra=1;
             Repeat
                 mueve=rand(1,80);
                 Frame;
             Until (mueve==5)
             mas_vida++;
             signal(id,s_kill);
           Else
              If (x<goodi.x AND flags==0) flags=1; vel_x=-vel_x; End
              If (x>=goodi.x AND flags) flags=0; vel_x=-vel_x; End
             accion=2; patada=1; sound_play(s_grito3,0);
             mueve=0; graph=28; velocidad_gravedad=-12;
         End
      End
      Frame;
   End
End

//#Section: Enemigo: GORILA
Process Gorila(double x,y, int file,graph,z,flags,zona1,zona2)
Private
  int vel_x;
  int Goody2;
  int Ladrillos;
  int golpe;
  int Agarra;
  int direc;
begin
    //sombraje(genemigos2,94,sombras);
   accion=1;
   altura=20;
   vel_x=rand(8,10);
   vida=2;

   ctype=c_scroll;

   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      goody2=collision(Type goody);
      gravedad();
      Switch(accion)
         Case 1:
             mueve++; If (mueve>2) mueve=0; graph++; x-=vel_x; End
             If (graph>32) graph=29; End
             If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+10)/2)==paredes);
                 vel_x=-vel_x; flags=1;
               Else
                 If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+10)/2)==paredes);
                     vel_x=-vel_x; flags=0;
                 End
            End
            direc++;
            if (direc==50 and rand(1,2)==1)
               direc=0;
               if(flags) vel_x=-vel_x; flags=0;
                 else
                         vel_x=-vel_x; flags=1;
               end
              else
                if(direc>50) direc=0; end
            end
            If (x<=zona1 AND flags==0) vel_x=-vel_x; flags=1; direc=20;
               Else
                  If (x>=zona2 AND flags)  vel_x=-vel_x; flags=0; direc=20 ;End
            End
            If (x<goodi.x-200 AND flags==0 AND x<zona2-200)  vel_x=-vel_x; flags=1;
              Else
                 If (x>goodi.x+200 AND flags AND x>zona1+200) vel_x=-vel_x; flags=0; End
            End
            If (goody2 AND agarra==0 AND cerveza>1
                 AND get_dist(goodi)<33)
                If (x<goodi.x AND flags==0) flags=1; vel_x=-vel_x; End
                If (x>=goodi.x AND flags) flags=0; vel_x=-vel_x; End
                accion=2; agarra=1;
                corazonazo=0; Huevazo=0; scroll.camera=goodi;
                Lanzar=0; If (get_id(Type mensaje)==0) Mensaje(7); End
                mueve=-1;
                Agachate=1;

            End
         End
         Case 2:
            mueve++;
            Switch(mueve)
               Case 0: if ( exists(goody2) ) signal(goody2,s_sleep); end; Caras(2); End
               Case 1,10,20,30: graph=33; cerveza-=15; End
               Case  5,15,25: graph=34; sound_play(s_golpe2,0); End
               Case 35: graph=29; accion=1; agarra=0;
                      Goody_golpe(goodi.x,goodi.y,goodi.file,10,goodi.z,2);
               End
            End
         End
      End
      If ((ladrillos) AND NOT golpe AND NOT agarra)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10; vida--;
         sound_play(s_golpe2,0); Mueve=0;
         If (vida==0)
             velocidad_gravedad=-13;
             Repeat
                 gravedad();
                 Mueve++; y++;
                 If (flags) angle+=pi/20; Else angle-=pi/20; End
                 If (mueve==3) sound_play(s_gorila,0); End
                 Frame;
             Until(Mueve>10)
             size=0;
             no_sombra=1;
             Repeat
                 mueve=rand(1,80);
                 Frame;
             Until (mueve==5)
             mas_vida++;
             signal(id,s_kill);
           Else
             velocidad_gravedad=-12;
             Repeat
                  mueve++;
                  gravedad();
                  If (mueve>3) mueve=0; graph++; End
                  If (graph>32) graph=29; End
                  Frame;
             Until (en_pantalla());
             mas_vida++;
             Golpe=0;
         End
      End
      Frame;
   End
End

//#Section: Enemigo: POLICIA

Process Policia(double x,y, int file,graph,z,flags,zona1,zona2)
Private
  int vel_x;
  int Goody2;
  int Ladrillos;
  int golpe;
  int direc;
begin
    //sombraje(genemigos2,94,sombras);
   altura=22;
   vel_x=rand(3,4);
   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      goody2=collision(Type goody);
      gravedad();
      If (en_pantalla() AND Atraco==0 AND policias==0)
         x+=vel_x;
         mueve++; If (mueve>3) mueve=0; graph++; End
         If (graph>2) graph=1; End
        If (flags==0 AND map_get_pixel(gnivel,mapa_dureza,(x/2)-10,(y+10)/2)==paredes);
             vel_x=-vel_x; flags=1;
           Else
             If (flags AND map_get_pixel(gnivel,mapa_dureza,(x/2)+10,(y+10)/2)==paredes);
                 vel_x=-vel_x; flags=0;
             End
        End
        direc++;
        if (direc==40 and rand(1,2)==1)
           direc=0;
           if(flags) vel_x=-vel_x; flags=0;
             else
                     vel_x=-vel_x; flags=1;
           end
          else
            if(direc>40) direc=0; end
        end
        If (x<=zona1 AND flags==0)
             vel_x=-vel_x; flags=1; direc=20;
           Else
              If (x>=zona2 AND flags)
                  vel_x=-vel_x; flags=0; direc=20;
              End
        End
        If (x<goodi.x-100 AND flags==0 AND x<zona2-200)
              vel_x=-vel_x; flags=1;
          Else
             If (x>goodi.x+100 AND flags AND x>zona1+200)
                   vel_x=-vel_x; flags=0;
             End
        End
          If (goody2 AND Policias==0 AND get_dist(goodi)<35 AND cerveza>0
               AND goodi.accion<>muerto)
              goody2.size=100; Policias=1;
              Lanzar=0; corazonazo=0; Huevazo=0; scroll.camera=goodi;
              Agachate=1;

              Caras(2);
               If (goody2.x>x AND flags==0) flags=1; vel_x=-vel_x; End
                If (flags) x=goody2.x-32; goody2.flags=1;
                   Else
                      x=goody2.x+32; goody2.flags=0;
                End
          End
      End
      If (policias)
          mueve++;
            Switch(mueve)
               Case 1,7,13,19: graph=35; End
               Case 3,10,16,22: graph=36;
                    If (goody2) cerveza-=14; sound_play(s_golpe2,0); End
               End
               Case 27: policias=0; graph=1;
                      corazonazo=0; Huevazo=0; scroll.camera=goodi;
                      signal(Type goody,s_sleep);
                      Goody_golpe(goodi.x,goodi.y,goodi.file,10,goodi.z,2);
               End
            End
      End
      If ((ladrillos) AND NOT golpe)
         mas_vida++;
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++; Policias=0;
             signal(Type goody,s_wakeup);
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==5) sound_play(s_grito1,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         no_sombra=1;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         signal(id,s_kill);
      End
      Frame;
   End
End

//#Section: Enemigo: FANTASMA
Process Fantasmas(file,graph,z,byte alpha, int xrelativa,yrelativa,incx,incy,distx,disty)
Private
    int angulo3;
    int angulo2;
    int Goody2;
    int Ladrillos;
    int Golpe;
    int inmune;
begin
    accion=1;
    ctype=c_scroll;
    Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
        Ladrillos=collision(Type Ladrillo);
        Goody2=collision(Type goody);
        Switch(accion)
           Case 1:
                angulo2+=pi/incx;                 // Halla el tama¤o de los incrementos
                angulo3+=pi/incy;                 // que se har n en las dos coordenadas
                x=xrelativa+get_distx(angulo2,distx);    // Incrementa la coordenada x
                y=yrelativa+get_disty(angulo3,disty);    // Incrementa la coordenada y

                If (x<goodi.x-20) flags=1; xrelativa+=2; End
                If (x>goodi.x+20) flags=0; xrelativa-=2; End
                mueve++;If (mueve>2) mueve=0; graph++;End
                If (graph>19) graph=18; End
           End
        End
      If (goody2 AND inmune==0 and goodi.accion<>muerto)
          cerveza-=30; inmune=15;
          Lanzar=0;
          Caras(2);
          Agachate=1;
          If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
      End
      If(inmune>0) inmune--; End
      If ((ladrillos) AND NOT golpe)
          golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad=-5;
          velocidad_gravedad=-10;
          Mueve=0; sound_play(s_golpe1,0);
          Repeat
              gravedad();
              angle+=pi/10;
              size-=2;
              Frame;
          Until(en_pantalla() AND velocidad_gravedad=>0);
          timer[1]=0;
          mas_vida++;
          signal(id,s_kill);
      End

       Frame;
    End
End

//#Section: Enemigo: LIMPIADORA
Process Limpiadora(double x,y, int file,graph,z)
Private
   int Polvo;
   int Barre;
   int Ladrillos;
   int ladri;
   int golpe;
begin
   ctype=c_scroll;
    //sombraje(genemigos2,94,sombras);

   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      Switch(accion)
         Case 1:
            Barre=rand(1,10);
            If (barre==1) accion=2; End
            If (x+10<goodi.x AND flags==0) flags=1; x+=15;
                Else
                If (x-10>goodi.x AND flags==1) flags=0; x-=15; End
            End
         End
         Case 2:
            mueve++;
            If (mueve==3)
                mueve=0; graph++;
                If (graph>42) graph=42;
                   If (rand(1,2)==1 AND get_id(Type polvos)==0)
                        polvos(x,y+20,file,rand(86,88),z-5);
                   End
                   //Frame(300);
                   accion=1; graph=40;
                End
            End
         End
      End
      If (ladrillos AND NOT golpe)
         golpe=1; accion=3;
         ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==4) sound_play(s_grito2,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         no_sombra=1;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


Process Polvos(double x,y, int file,graph,z)
Private
    int Goody2;
    int Cabina2;
    int inmune;
    int polv;
begin
   ctype=c_scroll;
   polvos2(x,y);
   polv=get_id(type polvos2);
   alpha=200;

   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End

      x=polv.x; y=polv.y;
      Goody2=collision(Type Goody);
      Cabina2=collision(Type Cabinas);
      mueve++; If(mueve==3) mueve=0; graph++; End
      If (graph>88) graph=86; End
      If (x<scroll.x0 OR x>scroll.x0+740
           OR y<scroll.y0 OR y>scroll.y0+360
           OR Cabina2)
         signal(id,s_kill_tree);
      End
      If (goody2 AND inmune==0 and goodi.accion<>muerto)
           cerveza-=20; inmune=20;
           Lanzar=0;
           Caras(2);
           Agachate=1;
           If (NOT goody2.ascensor or not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
      End
      If (inmune>0) inmune--; End
      Frame;
   End
End

process Polvos2(double x,y)
begin
   ctype=c_scroll;
   angle=get_angle(Goodi);
   loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      advance(6);
      frame;
   end
end


//#Section: Enemigo: BANQUERO
Process Banquero(double x,y, int file,graph,z)
Private
   int Polvo;
   int Barre;
   int Ladrillos;
   int ladri;
   int golpe;
begin
   ctype=c_scroll;

   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      Switch(accion)
         Case 1:
            Mueve++; if (mueve>5) mueve=0; graph++; end
            if(graph>101) graph=100; end
            Barre=rand(1,20);
            If (barre==1 and get_id(Type lingotes)==0) accion=2; mueve=0; End
            If (x+10<goodi.x AND flags==0) flags=1; x+=15;
                Else
                If (x-10>goodi.x AND flags==1) flags=0; x-=15; End
            End
         End
         Case 2:
            mueve++;
            switch(mueve)
               case 3: graph=100; end
               case 6: graph=101; end
               case 9: graph=102; Lingotes(x,y,file,85,z-5); end
               case 16: graph=103; end
               case 20: graph=100; accion=1; mueve=0; end

            end
         End
      End
      If (ladrillos AND NOT golpe)
         golpe=1; accion=3;
         ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==4) sound_play(s_grito1,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


Process Lingotes(double x,y, int file,graph,z)
Private
    int Goody2;
    int Cabina2;
    int golpe;
begin
   ctype=c_scroll;
   angle=get_angle(Goodi);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Goody2=collision(Type Goody);
      Cabina2=collision(Type Cabinas);

      advance(7);
      If (x<scroll.x0 OR x>scroll.x0+640
           OR y<scroll.y0 OR y>scroll.y0+320)
         signal(id,s_kill);
      End
      If ((goody2 AND NOT golpe and goodi.accion<>muerto) OR cabina2)
          If(NOT cabina2)
            cerveza-=25; golpe=1;
            Lanzar=0;
            Caras(2);
            Agachate=1;
            If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
          End
          velocidad_gravedad=-10;
          Repeat
              gravedad();
              angle+=pi/10;
              Frame;
          Until (en_pantalla())
          signal(id,s_kill);
      End

      Frame;
   End
End

//#Section: Enemigo: OBRERO PALA
Process Obrero_pala(double x,y, int file,graph,z)
Private
   int Barre;
   int Ladrillos;
   int golpe;
begin
   ctype=c_scroll;

    //sombraje(genemigos2,94,sombras);
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
         Case 1:
            Barre=rand(1,19);
            If (barre==1) accion=2; End
            If (x+5<goodi.x AND flags==0) flags=1; x+=5;
                Else
                If (x-5>goodi.x AND flags==1) flags=0; x-=5; End
            End
         End
         Case 2:
            mueve++;
            If (mueve==3)
                mueve=0; graph++;
                If (graph>62) graph=62;
                   If (rand(1,2)==1 AND get_id(Type arena)==0
                       AND get_id(Type piedra)==0 AND get_dist(goodi)>150)
                        arena(x,y+18,file,63,z-5);
                   End
                   //Frame(200);
                   accion=1; graph=60;
                End
            End
         End
      End
      Ladrillos=collision(Type Ladrillo);
      If (ladrillos AND NOT golpe)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0; accion=5;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==5) sound_play(s_grito1,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         no_sombra=1;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


Process Arena(double x,y, int file,graph,z)
Private
    int Goody2;
    int golpe;
begin
   ctype=c_scroll;
   angle=get_angle(Goodi);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      advance(8);
      Goody2=collision(Type Goody);
      If (x<scroll.x0 OR x>scroll.x0+640
           OR y<scroll.y0 OR y>scroll.y0+320)
         signal(id,s_kill);
      End
      If (goody2 AND NOT golpe and goodi.accion<>muerto)
          cerveza-=20; golpe=1; velocidad_gravedad=-10;
          Lanzar=0;
          Caras(2);
          Agachate=1;
          If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
          Repeat
              gravedad();
              angle+=pi/10;
              Frame;
          Until (en_pantalla())
          signal(id,s_kill);
      End

      Frame;
   End
End

//#Section: Enemigo: OBRERO PICO
Process Obrero_pico(double x,y, int file,graph,z)
Private
   int Barre;
   int Ladrillos;
   int golpe;
   int pico;
begin
   ctype=c_scroll;

    //sombraje(genemigos2,94,sombras);
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
         Case 1:
            Barre=rand(1,5);
            If (barre==1) accion=2;
               If ((stage==14 AND goodi.x>900)
                   OR (stage==15 AND goodi.x>20)
                     OR (stage==29 AND goodi.x<650))
                  sound_play(s_pico,0);
               End
            End
            If (x+5<goodi.x AND flags==0) flags=1; x+=5;
                Else
                If (x-5>goodi.x AND flags==1) flags=0; x-=5; End
            End
         End
         Case 2:
            mueve++;
            If (mueve==2)  mueve=0; graph++;
                If (graph>65) graph=64; End
                If (rand(1,8)==1)
                   accion=3; If (sound_is_playing(pico)==0) sound_stop(pico); End
                End
            End
         End
         Case 3:
            If (rand(1,2)==1 AND get_id(Type piedra)==0 AND get_dist(goodi)>150)
                piedra(x,y+5,file,66,z-5);
                //Frame(200);
            End
            accion=1; graph=64;
         End
      End
      Ladrillos=collision(Type Ladrillo);
      If (ladrillos AND NOT golpe)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0; accion=5;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==5) sound_play(s_grito1,0); End
             Frame;
         Until(Mueve>10)
         no_sombra=1;
         size=0;
         If (y==265 AND goodi.x<1680)
            Gorila(1880,265,genemigos,29,-10,0,1600,1890);
         End
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


Process Piedra(double x,y, int file,graph,z)
Private
    int Goody2;
    int golpe;
    int Cabina2;
begin
   ctype=c_scroll;
   angle=get_angle(Goodi);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      advance(8);
      Goody2=collision(Type Goody);
      Cabina2=collision(Type Cabinas);
      If (x<scroll.x0 OR x>scroll.x0+640
          OR y<scroll.y0 OR y>scroll.y0+320
           OR cabina2)
          Repeat
              mueve++;
              gravedad();
              angle+=pi/10;
              Frame;
          Until (en_pantalla() or mueve>40)
          signal(id,s_kill);
      End
      If (goody2 AND NOT golpe and goodi.accion<>muerto)
           if (not goody2.escaleras and not goody2.ascensor)
               goodi.velocidad_gravedad=-10;
               goodi.accion=parado; goodi.graph=6;
           end
          cerveza-=20; golpe=1;
          Lanzar=0;
          Caras(2);
          Agachate=1;
          Repeat
              gravedad();
              angle+=pi/10;
              Frame;
          Until (en_pantalla())
          signal(id,s_kill);
      End

      Frame;
   End
End

//#Section: Enemigo: SECRETARIO
Process Secretario(double x,y, int file,graph,z)
Private
   int Avioncito;
   int Ladrillos;
   int golpe;
begin
   ctype=c_scroll;
    //sombraje(genemigos2,94,sombras);
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type Ladrillo);
      Switch(accion)
         Case 1:
            avioncito=rand(1,32);
            If (avioncito==1 AND aviones<2
                AND x>scroll.x0 AND x<scroll.x0+640
                AND y>scroll.y0 AND y<scroll.y0+320)
               accion=2; Aviones++;
            End
            If (x<goodi.x) flags=0; Else flags=1; End
         End
         Case 2:
            mueve++;
            Switch(mueve)
               Case 3:
                   graph=46; sound_play(s_salto,0);
                   Avion_papel(x,y-10,file,47,z-3);
               End
               Case 15:
                   graph=45; accion=1; mueve=0;
               End
            End
         End
      End
      If ((ladrillos) AND NOT golpe)
         golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         Mueve=0; graph=45;
         sound_play(s_golpe2,0);
         Repeat
             Mueve++;
             Switch(mueve)
                Case 4,12,20,28: flags=0; End
                Case 5: sound_play(s_grito1,0); End
                Case 8,16,24,32: flags=1; End
             End
             Frame;
         Until(Mueve>35)
         size=0;
         no_sombra=1;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End


Process Avion_papel(double x,y, int file,graph,z)
Private
    int Goody2;
    int Cabina1;
    int busca;
begin
   ctype=c_scroll;
   angle=get_angle(Goodi);
   accion=1;
    //sombraje(genemigos2,94,sombras);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
        Case 1:
          advance(7);
          If (get_dist(goodi)>120 AND busca==0) angle=get_angle(Goodi); End
          If (get_dist(goodi)<120) busca=1; End
          Goody2=collision(Type Goody);
          Cabina1=collision(Type Cabinas);
          If (x<scroll.x0-100 OR x>scroll.x0+740
               OR y<scroll.y0-100 OR y>scroll.y0+420)
             aviones--;
             signal(id,s_kill);
          End
          If (goody2 and goodi.accion<>muerto) cerveza-=15; accion=2; mueve=0;
              velocidad_gravedad=-12;
              Lanzar=0;
              Caras(2);
              Agachate=1;
          If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
          End
          If (Cabina1) accion=2; mueve=0; velocidad_gravedad=-12; End
        End
        Case 2:
           aviones--;
           no_sombra=1;
           Repeat
               gravedad();
               mueve++;
               angle+=pi/6;
               Frame;
           Until(mueve>15)
           signal(id,s_kill);
        End
      End
      Frame;
   End
End


//#Section: Enemigo: LUNA
Process Luna(double x,y, int file,graph,z)
Private
   int Lluvia;
   int Ladrillos;
   int Angulo;
   int golpe;
   int anima;
begin
   ctype=c_scroll;
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type ladrillo);
      Switch (accion)
         Case 1:
            Lluvia=rand(1,70);
            If (lluvia==1 AND get_id(Type agualuna)==0 AND
                x>scroll[0].x0 AND x<scroll.x0+640 AND
                y>scroll[0].y0 AND y<scroll.y0+340 AND atraco==0)
               accion=2; Lluvia=0; graph=21;
            End
            If (x>goodi.x) flags=0; Else flags=1; End
         End
         Case 2:
            Lluvia++;
            Switch(lluvia)
               Case 4:
                  Angulo=get_angle(goodi);
                  Agualuna(x,y+5,file,8,z-5,angulo);
               End
               Case 12:
                  accion=1; graph=20;
               End
            End
         End
         Case 3:
            mueve++; If(mueve>2) mueve=0; graph++; End
            If(graph>83) graph=81; End
            Anima++;
            Switch(anima)
               Case 1..10,21..30,41..50,61..70,81..90,101..110:
                   //angle-=pi/100;
               End
               Case 11..20,31..40,51..60,71..80,91..100,111..120:
                   //angle+=pi/100;
               End
               Case 300:
                  angle=0; graph=20; accion=1; golpe=0;
                  mueve=0; anima=0;
               End
            End
         End
      End
      If ((ladrillos) AND NOT golpe)
          golpe=1; ladrillos.rebote=1; ladrillos.velocidad_gravedad=-5;
          Mueve=0; sound_play(s_golpe2,0); accion=3; graph=81;
          mas_vida++;
      End
      Frame;
   End
End

//#Section: Enemigo: AGUA LUNA

Process Agualuna(double x,y,int file,graph,z,angulo)
Private
   int vel_y;
   int cae;
   int busca;
begin
   altura=10;
   vel_y=1;
   accion=1;
   ctype=c_scroll;
   cae=y;
   angle=angulo;
   size=0;
   Agualuna2(x,y,file,graph,z);

   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch (accion)
         Case 1:
            mueve++;
            If (mueve==2) mueve=0; If (vel_y<15) vel_y++; End
               If (get_dist(goodi)>320 AND busca==0) angle=get_angle(goodi); End
               If (get_dist(goodi)<320) busca=1; End
            End
            advance(vel_y);
         End
      End
      Frame;
   End
End

Process Agualuna2(double x,y, int file,graph,z)
Private
   int Goody2;
   int golpe;
   int choca;
begin
   accion=1;
   ctype=c_scroll;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Goody2=collision(Type goody);
      Switch (accion)
         Case 1:
            x=father.x; y=father.y;
            From golpe=-6 To 6;
                If (map_get_pixel(gnivel,mapa_dureza,x/2+golpe,y/2+golpe)==paredes
                   OR map_get_pixel(gnivel,mapa_dureza,x/2+golpe,y/2+golpe)==color_dureza1);
                   choca=1;
                   Break;
                End
            End
            If (choca OR goody2 OR y>scroll[0].y0+285)
               graph=8; accion=2; Mueve=0;
               signal(father, s_kill);
                If (x>scroll[0].x0 AND x<scroll[0].x0+620)
                    sound_play(s_gotas,0);
                End
                If (goody2 and goodi.accion<>muerto) cerveza-=20; Lanzar=0; Caras(2); Agachate=1; end
            End
         End
         Case 2:
            mueve++;
            Switch(mueve)
               Case 3: graph=9; End
               Case 6: graph=10; End
               Case 9: size=0; End
               Case 12:
                  signal(id,s_kill);
               End
            End
         End
      End
      Frame;
   End
End

//#Section: Enemigo: PROSTITUTA
Process Prostituta(double x,y, int file,graph,z,flags)
Private
   int corazones;
   int Ladrillos;
   int golpe;
begin
   ctype=c_scroll;
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Ladrillos=collision(Type ladrillo);
      Switch (accion)
         Case 1:
            corazones=rand(1,30);
            If (corazones==1 AND !exists(Type Corazon) AND
                x>scroll[0].x0 AND x<scroll.x0+640 AND
                y>scroll[0].y0 AND y<scroll.y0+340 AND atraco==0)
               accion=2; corazones=0; graph=68;
            End
            If (x>goodi.x) flags=0; Else flags=1; End
         End
         Case 2:
            corazones++;
            Switch(corazones)
               Case 4: sound_play(s_beso,0); corazon(x,y,file,69,z-5);
                End
               Case 1,8,16,24:  graph=68; End
               Case 4,12,20,28: graph=67; End
               Case 32: accion=1; End
            End
         End
         Case 3:
            mueve++;
            Switch(mueve)
               Case 1..299:
                   If (flags)
                       angle+=pi/10;
                     Else
                       angle-=pi/10;
                   End
               End
               Case 300:
                  angle=0; graph=20; accion=1; golpe=0; mueve=0;
               End
            End
         End
      End
      If (ladrillos AND NOT golpe)
         golpe=1; accion=3;
         ladrillos.rebote=1; ladrillos.velocidad_gravedad-=10;
         velocidad_gravedad=-13;
         Mueve=0;
         sound_play(s_golpe2,0);
         Repeat
             gravedad();
             Mueve++; y++;
             If (flags) angle+=pi/20; Else angle-=pi/20; End
             If (mueve==4) sound_play(s_grito2,0); End
             Frame;
         Until(Mueve>10)
         size=0;
         Repeat
             mueve=rand(1,80);
             Frame;
         Until (mueve==5)
         mas_vida++;
         signal(id,s_kill);
      End
      Frame;
   End
End

Process corazon(double x,y, int file,graph,z)
Private
    int Goody2;
    int golpe;
    int vel_x;
    int vel_y;
    int direc;
    int gran;
begin
   ctype=c_scroll;
   Accion=1;
   //velocidad_gravedad=rand(-18,-14);
   y=y-rand(10,20);
   If (x>goodi.x) vel_x=rand(-7,-5);
      Else vel_x=rand(5,7);
   End

   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End

      Goody2=collision(Type Goody);

      x+=vel_x;
      If(gran)
         size+=2; If(size>=100) gran=0; End
       Else
         size-=2; If(size<=80) gran=1; End
      End

      If (x<scroll.x0 OR x>scroll.x0+640
          OR y<scroll.y0 OR y>scroll.y0+320)
          signal(id,s_kill);
      End
      If (goody2 AND NOT golpe AND NOT corazonazo and goodi.accion<>muerto)
          golpe=1; //velocidad_gravedad=-10;
          Lanzar=0; corazonazo=1;
          Caras(5);
          Agachate=1;
//          If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
          Repeat
              size-=5;
              angle+=pi/10;
              x=goody2.x; y=goody2.y;
              Frame;
          Until (size<=1)
          signal(id,s_kill);
      End
      Switch(accion)
         Case 0:
            gravedad();
            mueve++;
            If (mueve>20) accion=1; End
         End
         Case 1:
            y+=vel_y;
            Switch(direc)
               Case 0:
                  vel_y--;
                  If(vel_y<-4) direc=1; End
               End
               Case 1:
                  vel_y++;
                  If(vel_y>6) direc=0; End
               End
            End
         End
      End
      Frame;
   End
End

//#Section: ENEMIGO: ABEJAS
Process Panal(double x,y, int file,graph,z)
Private
   int salida;
begin
   ctype=c_scroll;
   accion=1;
   z=-9;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
         Case 1:
            salida=rand(1,20);
            If (salida==1 AND get_dist(goodi)<420 AND get_dist(goodi)>80
                AND x>scroll.x0 AND x<scroll.x0+600
                AND y>scroll.y0 AND y<scroll.y0+320)
                accion=2;
                Abejas(x,y,file,graph+1,z-3);
            End
         End
         Case 2:
            mueve++; If(mueve>160) accion=1; mueve=0; End
         End
      End
      Frame;
   End
End


Process Abejas(double x,y, int file,graph,z)
Private
   int Goody2;
   int ataque;
   int inmune;
begin
   ctype=c_scroll;
   if (sound_is_playing(abejillas)) sound_stop(abejillas); end
   Abejillas=sound_play(s_abejas,-1);
   size=140;
      Repeat
          ataque++;
          mueve++; If (mueve==2) mueve=0; graph++; End
          If (graph>50) graph=49; End
          Frame;
      Until(ataque>40)
   angle=get_angle(goodi);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      goody2=collision(Type goody);
      advance(8);
      mueve++; If (mueve==2) mueve=0; graph++; End
      If (graph>50) graph=49; End
      If (x<scroll.x0 OR x>scroll.x0+640
          OR y<scroll.y0 OR y>scroll.y0+320)
          sound_stop(abejillas);
         signal(id,s_kill);
      End
      If (goody2 AND inmune==0 and goodi.accion<>muerto and not corazonazo and not huevazo)
          cerveza-=25; inmune=15;
          Lanzar=0;
          Caras(4);
          Agachate=1;
          If (NOT goody2.ascensor and not goody2.escaleras) Goody2.velocidad_gravedad=-10;End
      End
      If (inmune>0) inmune--; End
      Frame;
   End
End


//#Section: Enemigo: HELICOPTERO
Process Helicoptero(double x,y, int file,graph,z,zona1,zona2)
Private
   int vel_x;
   int vel_y;
   int vuelo;
begin
   ctype=c_scroll;
   accion=1;
   size=0;
   If (x<goodi.x) flags=1; vel_x=8; Else flags=0; vel_x=-8; End
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch(accion)
         Case 1:
            mueve++; If (mueve==3) mueve=0; graph++; End
            If (graph>52) graph=51; End
            If (size<100) size+=2;
              Else
              size=100; mueve=0; graph=53; accion=2;
            End
         End
         Case 2:
            x+=vel_x; y+=vel_y;
            If (vuelo==0)
               If (vel_y<8) vel_y++; End
               If (vel_y>=8) vuelo=1; End
            End
            If (vuelo==1)
               If (vel_y>-8) vel_y--; End
               If (vel_y<=-8) vuelo=0; End
            End
            mueve++; If (mueve==2) mueve=0; graph++; End
            If (graph>54) graph=53; End

            If (x<=zona1 AND flags==0)
                 accion=3; Mueve=0; graph=51;
                 Repeat vel_x++; x+=vel_x; Frame; Until(vel_x==0);
               Else
                  If (x>=zona2 AND flags)
                       accion=4; Mueve=0; graph=51;
                       Repeat vel_x--; x+=vel_x; Frame; Until(vel_x==0);
                  Else
                    If (x<goodi.x-100 AND flags==0)// AND x<zona2-200)
                       accion=3; Mueve=0; graph=51;
                       Repeat vel_x++; x+=vel_x; Frame; Until(vel_x==0);
                     Else
                        If (x>goodi.x+100 AND flags==1)// AND x>zona1+200)
                           accion=4; Mueve=0; graph=51;
                           Repeat vel_x--; x+=vel_x; Frame; Until(vel_x==0);
                        End
                    End
                 End
            End
            If (rand(1,20)==1 AND get_id(Type cohete)==0)
               If ((stage==2 AND goodi.x<700) OR
                   (stage==5 AND goodi.x>500))
                  Else
                   Cohete(x,y,file,55,z);
                   sound_play(s_cohete1,0);
               End
            End
         End
         Case 3:
            mueve++;
            Switch(mueve);
               Case 3,9: graph=52; End
               Case 6,12: graph=51; End
               Case 15: accion=2; graph=53; mueve=0; flags=1; vel_x=8; End
            End
         End
         Case 4:
            mueve++;
            Switch(mueve);
               Case 3,9: graph=52; End
               Case 6,12: graph=51; End
               Case 15: accion=2; graph=53; mueve=0; flags=0; vel_x=-8; End
            End
         End
      End
      If (accion==3 OR accion==4)
         If(rand(1,50)==1 OR y<scroll.y0-20)
            graph=52;
            Repeat
               size-=2;
               Frame;
            Until(size<=4)
            sound_stop(helices);
            signal(id,s_kill);
         End
      End
      Frame;
   End
End

//#Section: Enemigo: COHETE
Process Cohete(double x,y, int file,graph,z)
Private
   int vel_y;
   int vel_x;
   int goody2;
   int golpe;
   int choca;
begin
   ctype=c_scroll;
   accion=1;
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      goody2=collision(Type goody);
      Switch(accion)
         Case 1:
            y+=vel_y;
            mueve++; If (mueve==2) mueve=0; vel_y++; End
            If (y>goodi.y-15)
               graph=56; accion=2;
               If (x<goodi.x) vel_x=12; flags=1;
                  Else
                     vel_x=-12; flags=0;
               End
            End
         End
         Case 2:
            x+=vel_x;
            From golpe=-6 To 6;
                If (map_get_pixel(gnivel,mapa_dureza,x/2+golpe,y/2+golpe)==paredes
                   OR map_get_pixel(gnivel,mapa_dureza,x/2+golpe,y/2+golpe)==color_dureza1);
                   choca=1;
                   Break;
                End
            End
            If (choca OR (goody2 AND NOT atraco and not policias)
                OR y>scroll[0].y0+285 OR x<-50 OR x>2000)
               corazonazo=0; Huevazo=0; scroll.camera=goodi;

               graph=57; accion=3; Mueve=0; sound_play(s_explosion1,0);
               If (goody2 AND NOT atraco and goodi.accion<>muerto)
                   if (not goody2.escaleras)
                      goodi.velocidad_gravedad=-15;
                      goodi.accion=parado; goodi.graph=6;
                   end
                   cerveza-=40;
                   goody2.velocidad=0;
                   Lanzar=0;
                   Caras(6);
                   Agachate=1;
               End
            End
         End
         Case 3:
            Repeat
               mueve++;
               Switch(mueve)
                  Case 0,12: graph=57; End
                  Case 3,9: graph=58; End
                  Case 6: graph=59; End
               End
               Frame;
            Until(mueve>15)
            signal(id,s_kill);
         End
      End
      Frame;
   End
End


//#Section: Enemigo: CAMARA
Process camara(double x,y, int file,graph,z,flags,lasers)
Private
   int Giro;
   int disparo;
   int direcc;
begin
   ctype=c_scroll;

   Accion=1;
   If(NOT flags) angle+=pi/8; Else angle-=pi/8; End
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      Switch (accion)
         Case 1:
         //-- Giro de Camara izquierda--//
         If (flags)
           mueve++;
           If (mueve>4) mueve=0; giro++;
             If (giro<5)
                If (direcc==0)
                    angle-=pi/14;
                  Else
                    angle+=pi/14;
                End
             End
             If (giro>=5) giro=0;
                If (direcc==1) direcc=0; Else direcc=1; End
             End
           End
         Else
         //-- Giro de Camara derecha --//
           mueve++;
           If (mueve>4) mueve=0; giro++;
             If (giro<5)
                If (direcc==0)
                    angle+=pi/14;
                  Else
                    angle-=pi/14;
                End
             End
             If (giro>=5) giro=0;
                If (direcc==1) direcc=0; Else direcc=1; End
             End
           End
         End
          //-- Disparo de Camara --//
          Disparo=rand(1,lasers);
          If (disparo==1 and x>scroll.x0 and x<scroll.x0+640
              and y>scroll.y0 and y<scroll.y0+230)
             accion=2; mueve=0;
          End
         End
         Case 2:
            Repeat
               mueve++; If (mueve==3) mueve=0; graph++; End
               If (graph==73 AND mueve==1)
               sound_play(s_camara,0);
                  laser(x,y,file,75,z+1,flags);
               End
               If (graph>74) mueve=99; graph=74; End
               Frame;
            Until(mueve==99)
            graph=70; disparo=0; accion=1;
         End
      End
      Frame;
   End
End

Process Laser(double x,y, int file,graph,z,flags)
Private
   int power;
   int cabina2;
   int goody2;
   int golpe;
begin
   ctype=c_scroll;
   angle=father.angle-90000;
   power=rand(8,12);
   Loop
      //--Pausa PROCESO--//
      While(pausado) Frame; End
      advance(power);
      Goody2=collision(Type Goody);
      Cabina2=collision(Type Cabinas);
      If (x<scroll.x0 OR x>scroll.x0+640
          OR y<scroll.y0 OR y>scroll.y0+320
           OR cabina2)
          signal(id,s_kill);
      End
      If (goody2 AND NOT golpe and goodi.accion<>muerto)
          if (not goody2.escaleras and not goody2.ascensor)
              goodi.velocidad_gravedad=-10;
              goodi.accion=parado; goodi.graph=6;
          end
          cerveza-=20; golpe=1;
          Lanzar=0;
          Caras(2);
          Agachate=1;
          signal(id,s_kill);
      End

     Frame;
   End
End

//-----------------------------------------------------------------------------
//#Section: Gravedad
Process gravedad()
Private
    int vgravedad;
    int Suelos;
begin
    vgravedad=(father.velocidad_gravedad+=2);
    If (vgravedad>16)
        father.velocidad_gravedad=16;
        vgravedad=16;
    End

    If (vgravedad<0)
        While (vgravedad++!=0)
            If (map_get_pixel(gnivel,mapa_dureza,father.x/2,(father.y-15)/2)<>Techos)
                father.y--;
            Else
                father.velocidad_gravedad=0;
                Break;
            End
        End
    Else
        father.y+=vgravedad;
        From vgravedad=-16 To 7;
            Suelos=map_get_pixel(gnivel,mapa_dureza,father.x/2,(father.y+father.altura+(vgravedad))/2);
            If (Suelos==color_dureza1)
                Break;
            End
        End
        If (vgravedad<8)
            father.y+=vgravedad;
            father.velocidad_gravedad=0;
        End
    End

End

//-----------------------------------------------------------------------------

Process en_pantalla()
Private
    int Suelos;
begin
    Suelos=map_get_pixel(gnivel,mapa_dureza,father.x/2,(father.y+father.altura)/2);
    If (Suelos==color_dureza1)
        Return(true);   // Si se toca suelo el proceso devuelve true
    Else
        Return(false);  // Si no, devuelve false
    End

End

//----- Posicionamiento de graficos con el mouse ---//
Process opciones_menu1(double x,y,int file,graph,z,control)
begin
    write_var(0,30,30,3,x);
    write_var(0,30,50,3,y);
    If (control==1)
      ctype=c_scroll;
      scroll.camera=id;
    End
    Repeat
       x=mouse.x+scroll[1].x0;
       y=mouse.y+scroll[1].y0;
       If (key(_right)) scroll[1].x0+=20; End
       If (key(_left)) scroll[1].x0-=20; End
       If (key(_down)) scroll[1].y0+=20; End
       If (key(_up)) scroll[1].y0-=20; End
       Frame;
    Until(mouse.left)
    scroll.camera=goodi;
End



//#Section: Logotipos
//------ LOGOTIPOS ----------------------//

function check_if_cancel_intro()
begin
    if ( skip_action() )
      fade_off(875);
      while(skip_action()) frame; end
      While(fade_info.fading) Frame; End
      fpg_unload(logos);
      signal(Type logotipos,s_kill_tree);
    end
end


Process logotipos()
begin
    set_fps(26,0);
    fade_on(FADE_TIME);
    logos=fpg_load("data1/logos.gdg");
    timer[0]=0; while(timer[0]<40) check_if_cancel_intro(); frame; end
//    Frame(1000);
    Musica1=music_load("data2/intro1.gdt");
    if ( exists( banda ) ) signal(banda,s_kill); frame; end
    banda=banda_sonora();
    timer[0]=0; while(timer[0]<60) check_if_cancel_intro(); frame; end
//    Frame(1600);
    OPERA(320,240,logos,1);
    set_fps(27,0);
    Loop
        If ( skip_action() )
            While ( skip_action() ) Frame; End
            fade_off(2500); While(fade_info.fading) Frame; End
            fpg_unload(logos);
            signal(Type logotipos,s_kill_tree);
            signal(id, s_kill);
        End

        If (get_id(Type OPERA)==0)
            fpg_unload(logos);
            signal(Type logotipos,s_kill_tree);
            signal(id, s_kill);
        End
        Frame;
    End
End

Process OPERA(double x,y,int file,graph)
begin
   Repeat
      check_if_cancel_intro();
      graph++;
      Frame(170);
   Until (graph>=20)
   graph=20;
   timer[0]=0; while(timer[0]<170) check_if_cancel_intro(); frame; end
   // Frame(4700);
   fade_off(250); While(fade_info.fading) Frame; End
   fade_on(FADE_TIME);
   graph=23; y-=50;
   Repeat
      check_if_cancel_intro();
      graph++;
      Frame(180);
   Until (graph>=42)
   graph=42;
   timer[0]=0; while(timer[0]<200) check_if_cancel_intro(); frame; end
   // Frame(4900);
   fade_off(250); While(fade_info.fading) Frame; End
   y+=50;
   graph=22; fade_on(500); While(fade_info.fading) Frame; End
   timer[0]=0; while(timer[0]<350) check_if_cancel_intro(); frame; end
   // Frame(8600);
   fade_off(875); While(fade_info.fading) Frame; End
End

// Redefine el Teclado de Juego//
//#Section: Redefinir

Process Redefine(double x,y, int file,graph,z)
Private
    String Push;
    String ver_tecla;
    int Tecla_x=192;
    int tecla_y=255;
    int Teclas;
    int asigna;
    int tec;
begin
    signal(father,s_sleep);
    signal(Type options,s_sleep);
    signal(Type manoselec,s_sleep);

    //write_var(0,50,50,4,redef[teclas]);

        Repeat
//           push=redefine[teclas].red;
//           write_var(letra1,320,290,4,push);
           Repeat
                Redef[teclas]=keyboard.scan_code;
                If (redef[teclas]==72) t_ascii[teclas]="arriba";
                  Else
                   If (redef[teclas]==80) t_ascii[teclas]="abajo";
                    Else
                      If (redef[teclas]==77) t_ascii[teclas]="derecha";
                       Else
                        If (redef[teclas]==75) t_ascii[teclas]="izquierda";
                         Else
                          If (redef[teclas]==57) t_ascii[teclas]="espacio";
                           Else
                             T_ascii[teclas]=chr(keyboard.ascii); End
                        End
                      End
                   End
                End
                Frame;
           Until (redef[teclas]<>0 OR key(_esc) OR get_joy_button(JOY_BUTTON_BACK));
           if ( key(_esc) OR get_joy_button(JOY_BUTTON_BACK) )
              While(key(_esc) OR get_joy_button(JOY_BUTTON_BACK)) Frame; End
            break;
           end
           //28=Enter;
           If (redef[teclas]==15 OR
               redef[teclas]==29 OR redef[teclas]==56 OR
               redef[teclas]==42 OR redef[teclas]==54 OR
               redef[teclas]==14)
               push="invalid key!";
             Else
               If (teclas>0)
                 From asigna=0 To teclas-1;
                   If (redef[teclas]==redef[asigna])
                       push="invalid key!"; Frame(2000);
                   End
                 End
               End
           End
           If (push=="invalid key!")
                keyboard.scan_code=0;
                push="";
             Else
                Teclas++;
                graph++;
           End
             keyboard.scan_code=0;
             Frame;
        Until (teclas>5 OR key(_esc) OR get_joy_button(JOY_BUTTON_BACK));
       While(key(_esc) OR get_joy_button(JOY_BUTTON_BACK)) Frame; End
    graph=20;
    Repeat Frame; Until(key(_s) or key(_y) OR key(_n) OR key(_esc) OR get_joy_button(JOY_BUTTON_BACK))
    If (key(_s) or key(_y))
       From asigna=0 To 5;
          teclado[asigna]=redef[asigna];
       End
    End
    While(key(_esc) OR get_joy_button(JOY_BUTTON_BACK)) Frame; End
    signal(father,s_wakeup);
    signal(Type options,s_wakeup);
    signal(Type manoselec,s_wakeup);
End

//#Section: Video FINAL
Process video_final()
private
   int pa;
   int mid;
begin
    let_me_alone();
//    clear_screen();
    write_delete(all_text);
    music_unload(musica1);

    if ( exists( banda ) ) signal(banda,s_kill); frame; end

    set_fps(40,0);

    fade_off(2000); While(fade_info.fading) Frame; End

    file=0;
    mid = media_load( "data2/final.gdm", &graph, 640, 360 );
    map_clear(0,graph,rgba(0,0,0,0));
    x=320; y=240;

    fade_on(2000); // While(fade_info.fading) Frame; End

    media_play(mid);

    while( media_get_status( mid ) != MEDIA_STATUS_ENDED ) frame; end

    fade_off(2000); While(fade_info.fading) Frame; End

    media_unload( mid );

    /* Parte 2 */
    Musica1=music_load("data2/musica1.gdt");
    banda=banda_sonora();

    fpg_unload(gmenu);
    if (idioma&1)
        gmenu=fpg_load("data1/gmenu.gdg");
    else
        gmenu=fpg_load("data1/gmenu2.gdg");
    end

    set_fps(26,0);
    creditos2(320,240,gmenu,60,-50);

    fade_on(2000); while(exists (type creditos2)) frame; end

    menu_inicio();

End

//--musica el juego--//
process banda_sonora()
   begin
   loop
      if (music_is_playing()<>1) 
          music_set_volume(volumen);
          music_play(musica1,0); 
          music_set_volume(volumen);
      end
   frame; end
end


//#Section: CREDITOS
Process creditos()
private
    int cor;
begin
    signal(father,s_freeze);

    //--Cortina de Bloques (cierra)--//
    from accion=22 to 480 step 44;
        from mueve=23 to 650 step 46;
            cortina(mueve,accion,gscore,98,-300,1,+4,2);
        end
    end
    cor=get_id(type cortina);
    write_delete(all_text);
    while(exists(type cortina))
        if(cor)
            if(cor.size>94)
                Score(320,240,gscore,-60,3);
            end
        end
        frame;
    end
    music_fade_off(1200);
    file=gmenu; graph=3; z=-70;
    x=320; y=240;
    from accion=22 to 480 step 44;
        from mueve=23 to 650 step 46;
            cortina(mueve,accion,gscore,98,-300,100,-4,2);
        end
    end
    while(exists(type cortina)) frame; end
    Tex_credit(320,750,file,2,z-1);

    music_unload(&musica1);
    Musica1=music_load("data2/final_2.gdt");
    music_set_volume(volumen);
    music_play(musica1,-1);

    loop
        If ( skip_action())
            While(skip_action()) Frame; End
            //--Cortina de Bloques (cierra)--//
            set_fps(26,0);
            from accion=22 to 480 step 44;
                from mueve=23 to 650 step 46;
                    cortina(mueve,accion,gscore,98,-300,1,+4,2);
                end
            end
            cor=get_id(type cortina);
            while(exists(type cortina))
                if(cor)
                    if(cor.size>94)
                        Score(320,240,gscore,-80,3);
                    end
                end
                frame;
            end
            size=0;
            music_fade_off(1200);
            signal(father,s_wakeup);
            put_screen(gmenu,1);
            signal(type tex_credit,s_kill);
            signal(type score,s_kill);
            from accion=22 to 480 step 44;
                from mueve=23 to 650 step 46;
                    cortina(mueve,accion,gscore,98,-300,100,-4,2);
                end
            end
            while(exists(type cortina)) frame; end
            music_unload(musica1);
            Musica1=music_load("data2/musica1.gdt");
            music_set_volume(volumen);
            music_play(musica1,-1);

            signal(id,s_kill);
        end
        frame;
    end
end

process Tex_credit(double x,y, int file,graph,z)
begin
   set_fps(40,0);
   loop
      if (y>-280) y--; else y=750; end

      frame;
   end
end

//--Tele CREDITOS--//
process creditos2(double x,y, int file,graph,z)
begin
   region_define(5,100,160,410,160);
   loop
      if (not exists (type credits))
         accion++;
         credits(305,360,file,49+accion,z-1);
      end
      mueve++;
      Switch(mueve)
         case 0:    graph=60; end
         case 2,14: graph=61; end
         case 4,12: graph=62; end
         case 6,10: graph=63; end
         case 8:    graph=64; end
         case 16:   graph=60; mueve=0; end
      end
      frame;
   end
end

process credits(double x,y, int file,graph,z)
private
   int sal;
begin
   region=5;
   switch(graph)
      case 50: sal=130; y=360; end
      case 51: sal=110; y=360; end
      case 52: sal=110; y=370; end
      case 53: sal=130; y=360; end
      case 54: sal=110; y=370; end
      case 55: sal=100; y=400; end
      case 56: sal=90; y=410; end
      case 57: sal=80; y=420; end
   end
   loop
      if(graph<>58)
          if (y>sal) y--; else signal(id,s_kill); end
        else
          if(y>270) y--; else y=270; mueve++; end
      end
      if(mueve>0 and accion==0)
         mueve++; if(mueve>200 or key(_esc) or get_joy_button(JOY_BUTTON_BACK)) accion=1; end
      end
      if (accion)
         music_fade_off(1200);
         fade_off(2500); While(fade_info.fading) Frame; End
         signal(type creditos2,s_kill);
         signal(id,s_kill);
      end
      frame;
   end
end
