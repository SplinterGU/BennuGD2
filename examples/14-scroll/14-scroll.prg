// --------------------------------------------------------------------------
// Bennu Tests                                         2000 José Luis Cebrián
// --------------------------------------------------------------------------

import "mod_gfx";
import "mod_input";
import "mod_misc";
import "mod_sound";

#define SCR_DEPTH   8

GLOBAL
    int colmouse, outinner, outoutside;

PROCESS Main()
BEGIN
    set_fps(60,0);
    set_mode(640,480,SCR_DEPTH);

    fpg_load ("test.fpg") ;
    ScrollTest();
end

PROCESS Header(string title)
private
    int w, h;
BEGIN
    write_delete (0) ;

    w = graphic_info(0, 0, G_WIDTH) ;
    h = graphic_info(0, 0, G_HEIGHT) ;

    write(0, w/2,  4, 1, title) ;
    write(0, w/2-2, 15, 2, "Fps:") ; write_var(0, w/2+4, 15, 0, frame_info.fps) ;
    write(0, w/2-2, 25, 2, "Speed %:") ; write_var(0, w/2+4, 25, 0, frame_info.speed_gauge) ;
    write(0, w/2-2, 35, 2, "Mouse collision:") ; write_var(0, w/2+4, 35, 0, colmouse) ;
    write(0, w/2-2, 45, 2, "Out inner region:") ; write_var(0, w/2+4, 45, 0, outinner) ;
    write(0, w/2-2, 55, 2, "Out outside region:") ; write_var(0, w/2+4, 55, 0, outoutside) ;

    write(0, w/2, h-20, 1, "Press ESC to continue") ;
END

// ----------------------------------------------------------------------
// Scrolling
// ----------------------------------------------------------------------


PROCESS ScrollSphere (double x, y, int graph)
BEGIN
    priority = 101 ;
    ctype = c_scroll;
    LOOP
        IF (key(_right)) x += 4 ; END
        IF (key(_left))  x -= 4 ; END
        IF (key(_down))  y += 4 ; END
        IF (key(_up))    y -= 4 ; END
        IF (key(_space)) x = y = 0 ; END
        colmouse = collision (type mouse);
        FRAME;
    END
END

PROCESS ScrollTriangle (follow, angle)
BEGIN
    priority = 100 ;
    graph = 102 ;
    ctype = c_scroll ;
    LOOP
        x = follow.x + get_distx (angle, 40) ;
        y = follow.y + get_disty (angle, 40) ;
        angle += 185000 ;
        FRAME ;
        angle -= 180000 ;
    END
END


PROCESS ScrollTest()
BEGIN
    mouse.graph = 200;

    region_define (1,   0,   0, 320, 240) ;
    region_define (2,   0, 240, 320, 240) ;
    region_define (3, 320,   0, 320, 240) ;
    region_define (4, 320, 240, 320, 240) ;

    region_define (5,  80,  80, 160,  80) ;

    scroll_start(0, 0, 200, 1, 1, 15);
    scroll_start(1, 0, 200, 1, 2, 15);
    scroll_start(2, 0, 200, 1, 3, 15);
    scroll_start(3, 0, 200, 1, 4, 15);

    scroll[0].camera = ScrollSphere (160, 100, 100) ;
    scroll[1].camera = ScrollTriangle (scroll[0].camera, 0) ;
    scroll[2].camera = ScrollTriangle (scroll[0].camera, 120000) ;
    scroll[3].camera = ScrollTriangle (scroll[0].camera, 240000) ;

    scroll[0].region1 = 5 ;
    scroll[0].region2 = 1 ;

    scroll[0].ratio  = 100 ;
    scroll[1].ratio  = 100 ;
    scroll[2].ratio  = 100 ;
    scroll[3].ratio  = 100 ;

    scroll[0].speed  = 3 ;
    scroll[1].speed  = 3 ;
    scroll[2].speed  = 3 ;
    scroll[3].speed  = 3 ;

    scroll[0].flags1 = B_TRANSLUCENT ;
    scroll[1].flags1 = B_TRANSLUCENT ;
    scroll[2].flags1 = B_TRANSLUCENT ;
    scroll[3].flags1 = B_TRANSLUCENT ;

    scroll[0].flags2 = B_NOCOLORKEY | B_HMIRROR;
    scroll[1].flags2 = B_NOCOLORKEY | B_HMIRROR;
    scroll[2].flags2 = B_NOCOLORKEY | B_HMIRROR;
    scroll[3].flags2 = B_NOCOLORKEY | B_HMIRROR;

    Header ("Scroll test") ;

    REPEAT:
        outinner = region_out(scroll[0].camera, 5);
        outoutside = region_out(scroll[0].camera, 1);
        FRAME;
    UNTIL key(_esc);

    fade_off(500) ; while (fade_info.fading); frame; end;

    scroll_stop(0) ;
    scroll_stop(1) ;
    scroll_stop(2) ;
    scroll_stop(3) ;

    set_mode(320,200,SCR_DEPTH);
    signal(type ScrollSphere, s_kill);
    signal(type ScrollTriangle, s_kill);
END
