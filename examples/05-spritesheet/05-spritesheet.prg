// Assets from https://kangkan.itch.io/helmet-girl

import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

#include "sprsheet.inc"

const
    scrw = 640;
    scrh = 200;
end

global
    __SPRSHEET_LIB * sprites = NULL;
end

process animate( int start_spr, int end_spr, double x, y )
private
    int i, ii;
    int * animation_offsetx;
    int sentido = 1;
begin
    i = start_spr;

    sprsheet_set_sprite( sprites, start_spr );

    while(1)
        ii = wrap( ii + 1, 0, 1 );
        if ( ii == 0 )
            i = wrap( i + 1, start_spr, end_spr );
            sprsheet_set_sprite( sprites, i );
        end
        x += 2.5 * sentido;
        if ( x > scrw - 32 || x < 32 ) sentido = -sentido; end
        if ( sentido < 0 ) flags = b_hmirror; else flags &= ~b_hmirror; end
        frame;
    end
end

process main()
private
    int sprite_sheet;
begin
    set_mode(scrw,scrh);
    set_fps(60,0);

    sprites = sprsheet_load("res/girlme.png");

    animate( 42, 61,  60, 100);
    animate( 42, 61, 200, 100);
    animate( 42, 61, 340, 100);

    while(!key(_ESC))
        frame;
    end

    sprsheet_unload( sprites );

    let_me_alone();

end
