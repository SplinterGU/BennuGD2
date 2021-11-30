import "libmod_gfx";
import "libmod_input";
import "libmod_misc";
import "libmod_debug";

#define RESX    256
#define RESY    256

global
    int * results = NULL, * resultsptr = NULL;
    int sx = -1, sy = -1;
    int tx = -1, ty = -1;
end

process main()
private
    int * path;
    int h, w;
    int color;
    byte r, g, b, a;
    string filename = "maze.png";
begin
    set_mode(RESX, RESY, WAITVSYNC);
    set_fps(0,0);

    background.file = 0;
    if ( argc > 1 )
        filename = argv[1];
    end

    background.graph = map_load( filename );

    if ( !background.graph )
        exit( filename + " can't be loaded" );
    end

    w = map_info(0,background.graph,g_width);
    h = map_info(0,background.graph,g_height);

    set_mode(w, h, WAITVSYNC);

    loop
        sx = rand(0,w-1);
        sy = rand(0,h-1);
        color = map_get_pixel( 0, background.graph, sx, sy );
        rgba_get( color, &r, &g, &b, &a );
        if ( !r && !g && !b ) break; end
    end

    while( !key(_ESC) && !wm_info.exit_status );

        sx = tx;
        sy = ty;

        loop
            tx = rand(0,w-1);
            ty = rand(0,h-1);
            color = map_get_pixel( 0, background.graph, tx, ty );
            rgba_get( color, &r, &g, &b, &a );
            if ( !r && !g && !b ) break; end
        end

        path = path_new( 0, background.graph );
        if ( path )
            results = path_find( path, sx, sy, tx, ty, 0 );
            if ( results )
                resultsptr = results;
                draw_delete(0);
                drawing_map(0,0);
                while( *resultsptr != -1 )
                    draw_circle(*resultsptr++, *resultsptr++, 1, rgba(0,0,255,255));
                end
                draw_circle_filled(sx, sy, 8, rgba(255,0,0,255));
                draw_circle_filled(tx, ty, 8, rgba(0,255,0,255));
                path_free_results( results );
            end
            path_destroy( path );
        end
        frame;
    end
    let_me_alone();
end
