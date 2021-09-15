// Sprite sheet credits http://www.court-records.net/

import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

const
    scrw = 640;
    scrh = 200;
end

type _animation_clip
    int x, y, w, h;
end


global
    _animation_clip animation_clip[]    =   10,0,100,168,
                                            10,168,114,168,
                                            10,336,100,168;

    int animation_frames[]              =   6,
                                            5,
                                            9;

    int animation_offsetx1[]            =   0,109,107,103,106,101;
    int animation_offsetx2[]            =   0,119,119,120,117;
    int animation_offsetx3[]            =   0,113,110,110,112,114,109,113,111;
                        
    int *animation_offsetx_ptr_array[16];

end

process animate( int idx, double x, y, int graph )
private
    int i, ii;
    int * animation_offsetx;
    int sentido = 1;
begin

    clip.x = animation_clip[idx].x;
    clip.y = animation_clip[idx].y;
    clip.w = animation_clip[idx].w;
    clip.h = animation_clip[idx].h;

    animation_offsetx = animation_offsetx_ptr_array[idx];

    while(1)
        if ( ii++ > 10 )
            ii = 0;
            i++;
            if ( i >= animation_frames[idx] )
                i = 0;          
                clip.x = animation_clip[idx].x;
            end

            clip.x += animation_offsetx[i];
        end
        x += 2.5 * sentido;
        if ( x > scrw - 32 || x < 32 ) sentido = -sentido; end
        frame;
    end
end

process main()
private
    int sprite_sheet;
begin
    set_mode(scrw,scrh);
    set_fps(60,0);

    sprite_sheet = map_load("res/DSsheet-maya.png");

    animation_offsetx_ptr_array[0] = &animation_offsetx1;
    animation_offsetx_ptr_array[1] = &animation_offsetx2;
    animation_offsetx_ptr_array[2] = &animation_offsetx3;
    animation_offsetx_ptr_array[3] = &animation_offsetx3;

    animate(0,60,100,sprite_sheet);
    animate(1,200,100,sprite_sheet);
    animate(2,340,100,sprite_sheet);

    while(!key(_ESC))
        frame;
    end

    let_me_alone();

end
