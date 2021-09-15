import "libmod_gfx";
import "libmod_input";

#include "../common/common.h"

process main()
begin
    #include "../common/init_video.h"

    write(0,160,10,ALIGN_CENTER,"Hello world!");
    write(0,160,190,ALIGN_CENTER,"(press F for switch fullscreen/window)");

    while( !key(_ESC) )
        if ( key(_f) )
            screen.fullscreen ^= 1;
            set_mode(SCRW,SCRH);
            while(key(_f)) frame; end
        end
        frame;
    end
end
