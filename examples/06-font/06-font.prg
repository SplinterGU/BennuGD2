Import "libmod_gfx";
Import "libmod_input";
import "libmod_misc";

const
    scrw = 800;
    scrh = 480;
end

private
    int textid[32];
    int fnt[9];
    int timer_change = 0, sentido = 1, tw;
begin
    set_mode(800,480);
    set_fps(60,0);

    mouse.graph = map_load("res/mouse.png");
    center_set(0,mouse.graph,0,0);

    background.graph = map_load("res/back.png");
    background.size_x = 130;

    fnt[0]=FNT_LOAD("res/fnt0.fnt");
    fnt[1]=FNT_LOAD("res/fnt1.fnt");
    fnt[2]=FNT_LOAD("res/fnt2.fnt");
    fnt[3]=FNT_LOAD("res/fnt3.fnt");
    fnt[4]=FNT_LOAD("res/fnt4.fnt");
    fnt[5]=FNT_LOAD("res/fnt5.fnt");
    fnt[6]=FNT_LOAD("res/fnt6.fnt");
    fnt[7]=FNT_LOAD("res/fnt7.fnt");
    fnt[8]=FNT_LOAD("res/fnt8.fnt");
    fnt[9]=FNT_LOAD("res/fnt9.fnt");

    textid[0] = write(fnt[0],0,0,0,"Hello World!");
    textid[1] = write(fnt[1],0,15,0,"Hello World!");
    textid[2] = write(fnt[2],0,30,0,"Hello World!");
    textid[3] = write(fnt[3],0,45,0,"Hello World!");
    textid[4] = write(fnt[4],0,80,0,"Hello World!");
    textid[5] = write(fnt[5],0,120,0,"Hello World!");
    textid[6] = write(fnt[6],0,160,0,"Hello World!");
    textid[7] = write(fnt[7],0,180,0,"Hello World!");
    textid[8] = write(fnt[8],0,260,0,"Hello World!");
    textid[9] = write(fnt[9],0,350,0,"Hello World!");

    tw = text_width(textid[9], "Hello World!");

    while( !key(_ESC) && !wm_info.exit_status );
        x += 2.5 * sentido;
        if ( x > scrw - tw || x < 0 ) sentido = -sentido; end
        write_move(textid[9], x, 350 );
        if ( get_timer() > timer_change )
            write_set_rgba(textid[9],rand(0,255),rand(0,255),rand(0,255),255);
            timer_change = get_timer() + 200;
        end
        frame;
    end
end
