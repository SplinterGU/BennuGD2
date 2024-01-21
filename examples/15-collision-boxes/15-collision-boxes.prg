import "libmod_gfx";
import "libmod_input";
import "libmod_misc";
import "libmod_debug";

#define scrw 640
#define scrh 480

#define GRID_SIZE 10


/*
functions:
    box_set( file, graph, box_id, shape_type, cx, cy, width/radius, height );
    box_set( box_id, shape_type, cx, cy, width/radius, height ); // use my instance graph

    box_get( file, graph, box_id, &shape_type, &cx, &cy, &width/radius, &height );
    box_get( box_id, &shape_type, &cx, &cy, &width/radius, &height ); // use my instance graph

    box_get( file, graph, pos, &box_id, &shape_type, &cx, &cy, &width/radius, &height );
    box_get( pos, &box_id, &shape_type, &cx, &cy, &width/radius, &height ); // use my instance graph

    box_remove( file, graph, box_id );
    box_remove( box_id ); // use my instance graph

    box_get_real_vertex( process_id, file, graph, box_id, double *vertices[8], int * radius );
    box_get_real_vertex( file, graph, box_id, double *vertices[8], int * radius ); // use my instance, but need graph
    box_get_real_vertex( box_id, double *vertices[8], int * radius ); // use my instance_graph    

shape_type:
    SHAPE_BOX
    SHAPE_CIRCLE

width/heigh:
    GRAPH_SIZE
    GRAPH_WIDTH
    GRAPH_HEIGHT
    GRAPH_MIN_SIZE
    GRAPH_MAX_SIZE
    GRAPH_AVERAGE_SIZE

locals:
    INT cshape = SHAPE_BOX;
    G_RECT cbox = POINT_UNDEFINED, POINT_UNDEFINED, GRAPH_SIZE, GRAPH_SIZE;

globals:
    STRUCT collision_info
       INT collider_cbox;
       INT collided_id;
       INT collided_cbox;
       INT penetration_x; // only box vs box
       INT penetration_y; // only box vs box
    END

*/


process actor( graph, x, y, z, size, ang_inc, inc_size, alpha )
private
    int blink;
    int _collision = 0;

begin
    region = 1;
    priority = -1;

    while ( 1 )
        x = clamp( x + ( -key( _LEFT ) + key( _RIGHT ) ) * 5, 0, scrw );
        y = clamp( y + ( -key( _UP ) + key( _DOWN ) ) * 5, 0, scrh );

        angle += ang_inc * ( -key( _1 ) + key( _2 ) );
        size = clamp( size + ( -key( _3 ) + key( _4 ) ) * inc_size, 50, 1000 );

        flags ^= ( key_down( _H ) * B_HMIRROR + key_down( _V ) * B_VMIRROR );

        if ( !key( _SPACE ))
            while (( _collision = collision( type enemy )))
                if ( timer[ 0 ] & 0xe )
                    switch ( collision_info.collider_cbox )
                        case 100:
                            _collision.color_b = 0;
                            _collision.color_g = 0;
                        end
                        case 101:
                            _collision.color_g = 0;
                            _collision.color_r = 0;
                        end
                        case 102:
                            _collision.color_b = 0;
                            _collision.color_r = 0;
                        end
                    end
                end
            end
        end
        frame;
    end
end

process enemy( graph, x, y, z, size, ang_inc )
begin
    region = 1;
    while ( 1 )
        color_r = 255;
        color_g = 255;
        color_b = 255;
        angle += ang_inc;
        frame;
    end
end

process main( x )
private
    int g;
    int i, ii;
    int n;
    int box;
    int g8;
    int enemies_type = 0;
begin
    rand_seed( time());

    set_mode( scrw, scrh );
    set_fps( 60, 0 );

    box = map_new( 64, 64 );
    map_clear( 0, box, rgb( 0, 0, 255 ));
    map_clear( 0, box, 1, 1, 62, 62, rgba( 255, 255, 0, 96 ));

    drawing_map( 0, box );
    draw_circle_filled( 0, 0, 30, rgba( 255, 0, 0, 255 ));
    draw_rectangle_filled( 54, 54, 10, 10, rgba( 0, 0, 255, 255 ));
    draw_circle_filled( 44, 44, 10, rgba( 0, 255, 0, 255 ));
    drawing_map( 0, 0 );

    g8 = map_new( 5, 5 );
    map_clear( 0, g8, rgba( 255, 255, 255, 128 ));

    box_set( 0, box, 100, SHAPE_CIRCLE, 0, 0, 30, 0 );
    box_set( 0, box, 101, SHAPE_BOX, 54, 54, 10, 10 );
    box_set( 0, box, 102, SHAPE_CIRCLE, 44, 44, 10, 0 );

//    box_set( 0, g8, 100, SHAPE_BOX, 2, 2, GRAPH_WIDTH, GRAPH_HEIGHT );

    actor( box, scrw / 2, scrh / 2, 0, 100, 5000, 10, 128 );
    
    write( 0, scrw / 2, scrh - 20, 1, "Use cursor key for move the actor - 1/2 rotate actor - 3/4 change size");
    write( 0, scrw / 2, scrh - 10, 1, "V/H mirror - SPACE hide collision - Enter random enemies/grid");

    region_define( 1, 0, 0, scrw, scrh - 20 );

    for ( i = 0; i < 500; i++ )
        enemy( g8, rand( 0, scrw ), rand( 0, scrh - 20 ), 10, rand( 100, 600 ), rand( -5, 5 ) * 1000 );
    end

    while ( !key( _ESC ) && !wm_info.exit_status );
        if ( key_down( _ENTER) )
            enemies_type ^= 1;
            signal( type enemy, s_kill );
            switch( enemies_type )
                case 0:
                    for ( i = 0; i < 500; i++ )
                        enemy( g8, rand( 0, scrw ), rand( 0, scrh - 20 ), 10, rand( 100, 600 ), rand( -5, 5 ) * 1000 );
                    end
                end

                case 1:
                    for ( i = 0; i < scrh; i += GRID_SIZE )
                        for ( ii = 0; ii < scrw - 20; ii += GRID_SIZE )
                            enemy( g8, GRID_SIZE / 2 + ii, GRID_SIZE / 2 + i, 10, 100, 0 );
                        end
                    end
                end
            end
        end
        frame;
    end
    let_me_alone();
end
