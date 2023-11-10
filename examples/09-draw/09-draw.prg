import "libmod_input";
import "libmod_gfx";
import "libmod_misc";
import "libmod_debug";

const
   cResX = 800;
   cResY = 600;
end;

#define gNumActors 1000

global
    int gActorMap;
    int idActors[gNumActors];
    byte color[gNumActors][3];
    G_POINT actorsInc[gNumActors];
    G_RECT actorsCoords[gNumActors];
    G_POINT actorsCoords2[gNumActors];
    G_POINT actorsCoords3[gNumActors];
    G_POINT actorsCoords4[gNumActors];
    int actors_radius[gNumActors];
    int actors_radius2[gNumActors];
    int actors_angle[gNumActors];
    int actors_angle2[gNumActors];
    int animate = 1;
    int pn[gNumActors];
    float* pv[gNumActors];
    float thickness = 1.0;
end;

/* ----------------------------- */

process capture()
private
    int g;
begin
    while( !key(_ESC) )
        if ( key_down( _F12 ) ) g = screen_get(); map_save( 0, g, "screenshot_" + time() + ".png"); map_del( 0, g ); end
        if ( key_down( _SPACE ) ) animate ^= 1; end
        frame;
    end
end

/* ----------------------------- */

process animate()
private
    int i;
begin
    while( !key(_ESC) )
        if ( animate )
            from i=0 to gNumActors;
                actorsCoords[ i ].x = clamp( actorsCoords[ i ].x + ( 1 - rand( 0, 1 ) * 2 ), 0, cResX );
                actorsCoords[ i ].y = clamp( actorsCoords[ i ].y + ( 1 - rand( 0, 1 ) * 2 ), 0, cResY );
                draw_move( idActors[ i ], actorsCoords[ i ].x, actorsCoords[ i ].y );
            end;
        end
        frame;
    end
end


/* ----------------------------- */

function TestPoints()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_point( actorsCoords[ i ].x, actorsCoords[ i ].y );
    end
end

/* ----------------------------- */

function TestLines()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_line( actorsCoords[ i ].x, actorsCoords[ i ].y, actorsCoords[ i ].x + actorsCoords[ i ].w, actorsCoords[ i ].y + actorsCoords[ i ].h );
    end
end

/* ----------------------------- */

function TestRectangles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_rectangle( actorsCoords[ i ].x, actorsCoords[ i ].y, 16, 16 );
    end
end

/* ----------------------------- */

function TestFRectangles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_rectangle_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, 16, 16 );
    end
end

/* ----------------------------- */

function TestCircles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_circle( actorsCoords[ i ].x, actorsCoords[ i ].y, 16 );
    end
end

/* ----------------------------- */

function TestFCircles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_circle_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, 16 );
    end
end

/* ----------------------------- */

function TestCurves()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_curve( actorsCoords[ i ].x, actorsCoords[ i ].y,
                                    actorsCoords[ i ].x + actorsCoords2[ i ].x, actorsCoords[ i ].y + actorsCoords2[ i ].y,
                                    actorsCoords[ i ].x + actorsCoords3[ i ].x, actorsCoords[ i ].y + actorsCoords3[ i ].y,
                                    actorsCoords[ i ].x + actorsCoords4[ i ].x, actorsCoords[ i ].y + actorsCoords4[ i ].y,
                                    15 );
    end
end

/* ----------------------------- */

function TestRectanglesRound()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_rectangle_round( actorsCoords[ i ].x, actorsCoords[ i ].y, 32, 32, 6 );
    end
end

/* ----------------------------- */

function TestFRectanglesRound()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_rectangle_round_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, 32, 32, 6 );
    end
end

/* ----------------------------- */

function TestArcs()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_arc( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_angle[ i ], actors_angle2[ i ] );
    end
end

/* ----------------------------- */

function TestFArcs()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_arc_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_angle[ i ], actors_angle2[ i ] );
    end
end

/* ----------------------------- */

function TestEllipses()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_ellipse( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_radius2[ i ], actors_angle[ i ] );
    end
end

/* ----------------------------- */

function TestFEllipses()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_ellipse_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_radius2[ i ], actors_angle[ i ] );
    end
end

/* ----------------------------- */

function TestSectors()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_sector( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_radius2[ i ], actors_angle[ i ], actors_angle2[ i ] );
    end
end

/* ----------------------------- */

function TestFSectors()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_sector_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, actors_radius[ i ], actors_radius2[ i ], actors_angle[ i ], actors_angle2[ i ] );
    end
end

/* ----------------------------- */

function TestTriangles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_triangle( actorsCoords[ i ].x, actorsCoords[ i ].y, actorsCoords[ i ].x + actorsCoords2[ i ].x, actorsCoords[ i ].y + actorsCoords2[ i ].y, actorsCoords[ i ].x + actorsCoords3[ i ].x, actorsCoords[ i ].y + actorsCoords3[ i ].y );
    end
end

/* ----------------------------- */

function TestFTriangles()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_triangle_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, actorsCoords[ i ].x + actorsCoords2[ i ].x, actorsCoords[ i ].y + actorsCoords2[ i ].y, actorsCoords[ i ].x + actorsCoords3[ i ].x, actorsCoords[ i ].y + actorsCoords3[ i ].y  );
    end
end

/* ----------------------------- */

function TestPolygons()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_polygon( pn[ i ], pv[ i ] );
    end
end

/* ----------------------------- */

function TestFPolygons()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_polygon_filled( pn[ i ], pv[ i ] );
    end
end

/* ----------------------------- */

function TestPolylines()
private
    int i;
begin
    for ( i = 0; i < gNumActors; i++ )
        drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
        idActors[ i ] = draw_polyline( pn[ i ], pv[ i ], 1 );
    end
end

/* ----------------------------- */

process main()
private
    int i, itemid = -1;
begin
    set_mode(cResX,cResY);
    set_fps(0,0);

    for ( i = 0; i < gNumActors; i++ )
        actorsCoords[ i ].x = rand(0,cResX);
        actorsCoords[ i ].y = rand(0,cResY);
        actorsCoords[ i ].w = 16 - rand(0,32);
        actorsCoords[ i ].h = 16 - rand(0,32);

        actorsCoords2[ i ].x = 16 - rand(0,32);
        actorsCoords2[ i ].y = 16 - rand(0,32);
        actorsCoords3[ i ].x = 16 - rand(0,32) * 2;
        actorsCoords3[ i ].y = 16 - rand(0,32) * 2;
        actorsCoords4[ i ].x = 16 - rand(0,32) * 3;
        actorsCoords4[ i ].y = 16 - rand(0,32) * 3;

        actors_radius[ i ] = rand( 1, 16 );
        actors_radius2[ i ] = rand( 1, 16 );
        actors_angle[ i ] = rand( 0, 359000 );
        actors_angle2[ i ] = rand( 0, 359000 );


            float cx;
            float cy;
            float radius;

            cx = rand(0, cResX);
            cy = rand(0, cResY);
            radius = 20 + rand(0,cResX/8);

            int j;

            pn[i] = rand(3,11);
            pv[i] = (float*)alloc(2*pn[i]*sizeof(float));

            for(j = 0; j < pn[i]*2; j+=2)
                pv[i][j] = cx + radius*cos(2*PI*(((float)j)/(pn[i]*2))) + rand(0,(int)radius/2);
                pv[i][j+1] = cy + radius*sin(2*PI*(((float)j)/(pn[i]*2))) + rand(0,(int)radius/2);
            end

        color[ i ][ 0 ] = rand( 0, 255 );
        color[ i ][ 1 ] = rand( 0, 255 );
        color[ i ][ 2 ] = rand( 0, 255 );
        color[ i ][ 3 ] = rand( 128, 255 );
    end



    write_var( 0, cResX - 40, 10, 0, frame_info.fps );
    if ( os_id != OS_SWITCH )
        write( 0, 10, cResY - 40, 0,
                "1:Points "
                "2:Lines "
                "3:Rectangles "
                "4:F.Rectangles "
                "5:Circles "
                "6:F.Circles "
                "7:Rectangles Round "
                "8:F.Rectangles Round "
             );
        write( 0, 10, cResY - 30, 0,
                "q:Arc "
                "w:F.Arc "
                "e:Ellipse "
                "r:F.Ellipse "
                "t:Sector "
                "y:F.Sector "
                "u:Triangles "
                "i:F.Triangles "
                "o:Polygons "
                "p:F.Polygons "
            );
        write( 0, 10, cResY - 20, 0,
                "a:Polylines "
                "z:thickness(-) "
                "x:thickness(+) "
            );
    else
        write( 0, 10, cResY - 40, 0,
                "1:Points "
                "2:Lines "
                "3:Rectangles "
                "4:F.Rectangles "
                "5:Circles "
                "6:F.Circles "
             );
    end

    write( 0, 10, cResY - 10, 0,
            "v:Enable vsync "
            "shift+v:Disable vsync "
            "F12: screenshot "
            "SPACE - Stop/Start Movement "
            "ESC - to Exit "
        );

    capture();
    animate();

    TestPoints();

    while( !key( _ESC ) )
        if ( key_down( _1 ) ) draw_delete(0); TestPoints(); end
        if ( key_down( _2 ) ) draw_delete(0); TestLines(); end
        if ( key_down( _3 ) ) draw_delete(0); TestRectangles(); end
        if ( key_down( _4 ) ) draw_delete(0); TestFRectangles(); end
        if ( key_down( _5 ) ) draw_delete(0); TestCircles(); end
        if ( key_down( _6 ) ) draw_delete(0); TestFCircles(); end

        if ( key_down( _v ) ) if ( keyboard.shift_status & STAT_SHIFT) set_mode(cResX,cResY); else set_mode(cResX,cResY,waitvsync); end end

        if ( os_id != OS_SWITCH )
            if ( key_down( _7 ) ) draw_delete(0); TestRectanglesRound(); end
            if ( key_down( _8 ) ) draw_delete(0); TestFRectanglesRound(); end
            if ( key_down( _9 ) ) draw_delete(0); TestCurves(); end

            if ( key_down( _q ) ) draw_delete(0); TestArcs(); end
            if ( key_down( _w ) ) draw_delete(0); TestFArcs(); end
            if ( key_down( _e ) ) draw_delete(0); TestEllipses(); end
            if ( key_down( _r ) ) draw_delete(0); TestFEllipses(); end
            if ( key_down( _t ) ) draw_delete(0); TestSectors(); end
            if ( key_down( _y ) ) draw_delete(0); TestFSectors(); end
            if ( key_down( _u ) ) draw_delete(0); TestTriangles(); end
            if ( key_down( _i ) ) draw_delete(0); TestFTriangles(); end
            if ( key_down( _o ) ) draw_delete(0); TestPolygons(); end
            if ( key_down( _p ) ) draw_delete(0); TestFPolygons(); end

            if ( key_down( _a ) ) draw_delete(0); TestPolylines(); end

            if ( key_down( _z ) )
                thickness = clamp(thickness-0.5,1,10);
                draw_set_thickness( thickness );
                for ( i = 0; i < gNumActors; i++ )
                    draw_set_thickness( idActors[ i ], thickness );
                end
                timer[0] = 0; while( key( _z ) && timer[0] < 20 ) frame; end;
            end
            if ( key_down( _x ) )
                thickness = clamp(thickness+0.5,1,10);
                draw_set_thickness( thickness );
                for ( i = 0; i < gNumActors; i++ )
                    draw_set_thickness( idActors[ i ], thickness );
                end
                timer[0] = 0; while( key( _x ) && timer[0] < 20 ) frame; end;
            end
        end
        frame;
    end

    let_me_alone();

end

/* ----------------------------- */
