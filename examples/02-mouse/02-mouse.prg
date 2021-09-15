import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

#include "../common/common.h"

process main()
begin
    set_mode(SCRW,SCRH);

    #include "../common/init_video.h"

//    mouse.graph = map_load("res/arrow.png");
//    center_set(0,mouse.graph,0,0);

    mouse.graph = map_load("../common/res/transparent-cursor.png");
    center_set(0,mouse.graph,160,60);
    mouse.size = 6;

    write(0,75,10,ALIGN_TOP_RIGHT, "x"     ); write_var(0,85,10,ALIGN_TOP_LEFT, mouse.x     );
    write(0,75,20,ALIGN_TOP_RIGHT, "y"     ); write_var(0,85,20,ALIGN_TOP_LEFT, mouse.y     );
    write(0,75,30,ALIGN_TOP_RIGHT, "left"  ); write_var(0,85,30,ALIGN_TOP_LEFT, mouse.left  );
    write(0,75,40,ALIGN_TOP_RIGHT, "right" ); write_var(0,85,40,ALIGN_TOP_LEFT, mouse.right );
    write(0,75,50,ALIGN_TOP_RIGHT, "middle"); write_var(0,85,50,ALIGN_TOP_LEFT, mouse.middle);

    write(0,235,10,ALIGN_TOP_RIGHT, "wheelup"    ); write_var(0,245,10,ALIGN_TOP_LEFT, mouse.wheelup    );
    write(0,235,20,ALIGN_TOP_RIGHT, "wheeldown"  ); write_var(0,245,20,ALIGN_TOP_LEFT, mouse.wheeldown  );
    write(0,235,30,ALIGN_TOP_RIGHT, "wheelleft"  ); write_var(0,245,30,ALIGN_TOP_LEFT, mouse.wheelleft  );
    write(0,235,40,ALIGN_TOP_RIGHT, "wheelright" ); write_var(0,245,40,ALIGN_TOP_LEFT, mouse.wheelright );

    while( !key(_ESC) ) 
        frame; 
    end

    let_me_alone();

end
