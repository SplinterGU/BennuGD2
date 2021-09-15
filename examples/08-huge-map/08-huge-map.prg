/* --------------------------------------------------------------------------- */
/* By Splinter                                                                 */
/* --------------------------------------------------------------------------- */

import "libmod_gfx"
import "libmod_input"
import "libmod_misc"

#define SCR_DEPTH   32
#define SCR_WIDTH   1024
#define SCR_HEIGHT  768

GLOBAL
    int game_map;

    string images[] = "bellas-artes-desde-sears-3.jpg",
                      "man.jpg",
                      "pub00905.jpg";

    int image_actual = 0;

begin
    set_fps(0,0);

    set_mode(SCR_WIDTH,SCR_HEIGHT);

    graph = map_load("res/"+images[image_actual++]);
    flags = b_nocolorkey;
    x = SCR_WIDTH/2;
    y = SCR_HEIGHT/2;

    int font, font_map;

    font_map = map_load("res/font-06_8x8.png");
    font = fnt_new(0,font_map,CHARSET_ISO8859,8,8,32,126,NFB_FIXEDWIDTHCENTER);

    write( font, 10,   0, -1, 0, "keys:" );
    write( font, 10,  10, -1, 0, "arrow   - move image" );
    write( font, 10,  20, -1, 0, "1/2     - rotate image" );
    write( font, 10,  30, -1, 0, "3       - center to 0,0" );
    write( font, 10,  40, -1, 0, "4       - reset center" );
    write( font, 10,  50, -1, 0, "q/w     - resize image" );
    write( font, 10,  60, -1, 0, "v       - vertical mirror" );
    write( font, 10,  70, -1, 0, "h       - horizontal mirror" );
    write( font, 10,  80, -1, 0, "0       - reset mirror" );
    write( font, 10,  90, -1, 0, "<enter> - change image" );

    while(!key(_ESC))
        if ( timer[0] > 5 )
            if ( key(_UP)                   ) timer[0] = 0; y-=100; end
            if ( key(_DOWN)                 ) timer[0] = 0; y+=100; end
            if ( key(_LEFT)                 ) timer[0] = 0; x-=100; end
            if ( key(_RIGHT)                ) timer[0] = 0; x+=100; end
            if ( key( _1 )                  ) timer[0] = 0; angle-=1000; end
            if ( key( _2 )                  ) timer[0] = 0; angle+=1000; end
            if ( key(_q)      && size > 0   ) timer[0] = 0; size--; end
            if ( key(_w)      && size < 800 ) timer[0] = 0; size++; end
            if ( key(_3)                    ) timer[0] = 0; center_set( 0, graph, 0, 0 ); end
            if ( key(_4)                    ) timer[0] = 0; center_set( 0, graph, CPOINT_UNDEFINED, CPOINT_UNDEFINED ); end
        end

        if ( keydown(_enter)) map_unload( 0, graph ); graph = map_load("res/"+images[image_actual++]); if ( image_actual >= sizeof( images ) / sizeof( images[ 0 ] ) ) image_actual = 0; end; end
        if ( keydown(_h)) flags^=B_HMIRROR; end
        if ( keydown(_v)) flags^=B_VMIRROR; end

        if ( keydown(_0)) flags=b_nocolorkey; end

        frame;
    end
end
