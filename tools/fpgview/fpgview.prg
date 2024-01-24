import "libmod_input";
import "libmod_gfx";
import "libmod_misc";
import "libmod_debug";

#include "mlwrite.inc"


local
    double w, h;
    double hw, hh;
    double real_size;
end


const
    usage = "help (keys)|"
            "-----------|"
            "|"
            "F1 - this screen|"
            "F2 - save to png|"
            "SHIFT+F2 - save to png with map name|"
            "F10/ESC - exit|"
            "i - show info|"
            "f - fit in screen|"
            "cursors/mouse+click - move image|"
            "pgdn - previous image|"
            "pgup - next image|"
            "a/q - zoom|"
            "0 - reset zoom|"
/*            "F11 - full screen|" */
            ;
end


global
    int scrw,  scrh;
    int deskx, desky;
    int mouse_hand, mouse_normal;
    double mouse_point_x, mouse_point_y;
end

//#define SCRW    800
//#define SCRH    600

#define BORDER  0

/* -------------------------------------------------------- */

function int calculate_size( fitonscreen, w, h )
begin
    if ( fitonscreen && ( w > SCRW || h > SCRH ) )
        if ( h > w )
            size = SCRH * 100 / h;
        else
            size = SCRW * 100 / w;
        end
    else
        size = father.real_size;
    end
    return size;
end


process draw_cursor()
begin
    graph = map_load("mouse-cross.png");
    size = 400;
    priority = father.priority - 1;
    z = father.z - 1;
    while(1)
        x = father.x;
        y = father.y;
        color_r += rand(0,16);
        color_g += rand(0,16);
        color_b += rand(0,16);
        frame;
    end
end

process zoom()
begin
    flags = b_nocolorkey;
    draw_cursor();
    size = 400;
    priority = father.priority - 1;
    z = father.z - 1;

    graph = map_new(50,50);

    center.x = 25;
    center.y = 25;

    x = scrw - 150 /* clip.h * 3 */;
    y = scrh - 150 /* clip.w * 3 */;

    while(1)
        map_clear(0,graph);
        map_block_copy( 0,
                        graph,
                        0,
                        0,
                        father.file,
                        father.graph,
                        mouse_point_x - center.x,
                        mouse_point_y - center.y,
                        50,
                        50,
                        0,
                        255,
                        255,
                        255,
                        255,
                        BLEND_NONE );

#if 0
        file = father.file;
        graph = father.graph;
        clip.h = 50;
        clip.w = 50;
        clip.x = mouse_point_x - center.x;
        clip.y = mouse_point_y - center.y;
#endif
        frame;
    end
end

process main()
private
    int g;
    string ginfo[6];
    int first = 0, last = 0;
    int total = 0;
    int current = 1;
    int showinfo = 1;
    int pulse = 0;

    double dx, dy;
    double ox, oy;

    int fitonscreen = 0;

    oltext_id * helpids = NULL;
    int helpcnt = 0;
    int showhelp = 0;

    oltext_id textinfo[6];
    int i;
begin
    desktop_get_size( &deskx, &desky );

    scrw=deskx-128;
    scrh=desky-128;

    set_mode(SCRW,SCRH,32);

    window_move((deskx-scrw)/2,(desky-scrh)/2);

    x = SCRW/2;
    y = SCRH/2;

    file = fpg_load( argv[ 1 ] );
    if ( file == -1 )
        file = fpg_new();
        graph = map_load( argv[ 1 ] );
        fpg_add( file, 1, 0, graph );
    end

    for(g=1;g<1000;g++)
        if ( map_exists(file, g) )
            if ( !first )
                first = g;
            end
            last = g;
            total++;
        end
    end

    graph = first;

    mouse_hand = map_load("mouse-hand.png");
    mouse_normal = map_load("mouse.png");
    center_set(0,mouse_normal,0,0);

    mouse.graph = mouse_normal;

    real_size = 100;

    int pid_zoom = zoom();

    while(!key(_ESC) && !wm_info.exit_status)
        if ( key_down(_I) )
            showinfo ^= 1;
            for ( i = 0; i < 7; i++ )
                oltext_delete( textinfo[i] );
            end
        end

        if ( showhelp )
            mlwrite_delete( &helpids, helpcnt );
            helpcnt = mlwrite(rgb(0,0,0), rgb(255,255,0), 0, SCRW / 2, 100, ALIGN_CENTER, usage, &helpids );
        else
            mlwrite_delete( &helpids, helpcnt );
        end

        if ( showinfo )
            for ( i = 0; i < 7; i++ )
                oltext_delete( textinfo[i] );
            end
            olwrite_var(textinfo[0],rgb(0,0,0),rgb(255,255,255),0,     10,10, ALIGN_CENTER_LEFT,ginfo[0])
            olwrite_var(textinfo[1],rgb(0,0,0),rgb(255,255,255),0,     10,20, ALIGN_CENTER_LEFT,ginfo[1])
            olwrite_var(textinfo[2],rgb(0,0,0),rgb(255,255,255),0,     10,30, ALIGN_CENTER_LEFT,ginfo[2])
            olwrite_var(textinfo[3],rgb(0,0,0),rgb(255,255,255),0,     10,40, ALIGN_CENTER_LEFT,ginfo[3])
            olwrite_var(textinfo[4],rgb(0,0,0),rgb(255,255,255),0,     10,50, ALIGN_CENTER_LEFT,ginfo[4])
            olwrite_var(textinfo[5],rgb(0,0,0),rgb(255,255,255),0,     10,60, ALIGN_CENTER_LEFT,ginfo[5])
            olwrite_var(textinfo[6],rgb(0,0,0),rgb(255,255,255),0,SCRW-10,10,ALIGN_CENTER_RIGHT,ginfo[6])
        end

        if ( key_down(_F) )
            fitonscreen ^= 1;
        end

        if ( key(_Q) )
            real_size += 10;
            while( ( key(_Q) ) && timer[0] < 20 ) frame; end
        end

        if ( key(_A) && size > 10 )
            real_size -= 10;
            while( ( key(_A) ) && timer[0] < 20 ) frame; end
        end

        if ( key_down(_0) )
            real_size = 100;
        end

        if ( key(_PGDN) || mouse.wheeldown )
            for(g=graph+1;g<=last;g++)
                if ( map_exists(file, g) )
                    break;
                end
            end

            if (g!=graph && g<=last) current++; x = SCRW/2; y = SCRH/2; end
            if (g<=last) graph = g; end

            while( ( key(_PGDN) || mouse.wheeldown ) && timer[0] < 20 ) frame; end
        end

        if ( key(_PGUP) || mouse.wheelup )
            for(g=graph-1;g>0;g--)
                if ( map_exists(file, g) )
                    break;
                end
            end

            if (g!=graph && g>0) current--; x = SCRW/2; y = SCRH/2; end
            if (g>0) graph = g; end

            while( ( key(_PGUP) || mouse.wheelup ) && timer[0] < 20 ) frame; end
        end

        w = map_info(file,graph,g_width);
        h = map_info(file,graph,g_height);

        hw = w / 2;
        hh = h / 2;

        point_set(file,graph,0,hw,hh);

        size = calculate_size( fitonscreen, w, h );

        int * hex, ihex;
        string hexs;

        mouse_point_x = ( mouse.x * 100 / size - ( x * 100 / size - hw ) );
        mouse_point_y = ( mouse.y * 100 / size - ( y * 100 / size - hh ) );

        hex = map_get_pixel(file,graph,mouse_point_x,mouse_point_y);

        ihex = hex;
        
        int r1, g1, b1, a1;

        rgba_get(hex, &r1, &g1, &b1, &a1);

        hexs = substr(""+hex, 8, 8 ) + " ( "+r1+", "+g1+", "+b1+", "+a1+" ) ( "+ ihex +" )";

        int cx, cy;
        point_get( file, graph, 0, &cx, &cy);

        ginfo[0] = "id: " + graph;
        ginfo[1] = "name: " + map_name(file,graph);
        ginfo[2] = "size (w,h): " + w + "," + h + " center: " + cx + "," + cy;
        ginfo[3] = "depth: " + map_info(file,graph,g_depth) + " bpp";
        ginfo[4] = "pixel color: " + hexs;
        ginfo[5] =  "x: " + mouse_point_x + " y: " + mouse_point_y;
        ginfo[6] =  "(" + current + "/" + total + ")";

        if ( !(
                key(_F1)    ||
                key(_F2)    ||
                key(_PGDN)  ||
                key(_PGUP)  ||
                key(_I)     ||
                key(_PLUS)  ||
                key(_MINUS)
              ) )
            timer[0] = 0;
        end

        if ( !fitonscreen && ( w > SCRW || h > SCRH ) )
            if ( w * size / 100 > SCRW )
                if ( mouse.left )
                    if ( !pulse )
                        dx = mouse.x - x;
                    end
                    x = mouse.x - dx;
                end

                if ( key( _RIGHT ) )
                    x-=10;
                end

                if ( key( _LEFT ) )
                    x+=10;
                end

                if ( ox > x && x + hw * size / 100 < SCRW - BORDER ) x = SCRW - (hw * size / 100 + BORDER); end
                if ( ox < x && x - hw * size / 100 > 0 - BORDER ) x = hw * size / 100 - BORDER; end
            end

            if ( h * size / 100 > SCRH )
                if ( mouse.left )
                    if ( !pulse )
                        dy = mouse.y - y;
                        pulse = 1;
                    end
                    y = mouse.y - dy;
                end

                if ( key( _DOWN ) )
                    y-=10;
                end

                if ( key( _UP ) )
                    y+=10;
                end

                if ( oy > y && y + hh * size / 100 < SCRH - BORDER ) y = SCRH - (hh * size / 100 + BORDER); end
                if ( oy < y && y - hh * size / 100 > 0 - BORDER ) y = hh * size / 100 - BORDER; end
            end

            ox = x;
            oy = y;

            if ( mouse.left )
                mouse.graph = mouse_hand;
                pulse = 1;
            else
                mouse.graph = mouse_normal;
                pulse = 0;
            end
        else
            x = SCRW / 2;
            y = SCRH / 2;
        end

        if ( key_down( _F1 ) ) showhelp ^= 1; end
        if ( key_down( _F2 ) )
            string name;
            name = "fpgview_" + graph + ".png";
            if ( keyboard.shift_status & STAT_SHIFT && trim(map_name(file,graph)) != "" )
                name = trim(map_name(file,graph)) + ".png";
            end
            map_save(file,graph,name);
        end

        frame;
    end

    fpg_unload(file);
    let_me_alone();

end
