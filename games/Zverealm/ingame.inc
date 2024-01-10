include "camera.inc"
include "mobs.inc"
include "player.inc"
include "fields.inc"
include "bullets.inc"
include "skills.inc"
global
    int drawrange = 7;
    int animcell[ 31 ] = 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 28, 28, 28;
    int newanim;
end


Function Int StartNewGame()
Private
    int i;
Begin
    if ( playerclass == 0 )
        mobhp[ 0 ] = 99;
        mobdmg0[ 0 ] = 2;
        mobdmg1[ 0 ] = 12;
    elseif ( playerclass == 1 )
        mobhp[ 0 ] = 49;
        mobdmg0[ 0 ] = 0;
        mobdmg1[ 0 ] = 6;
    else
        mobhp[ 0 ] = 59;
        mobdmg0[ 0 ] = 1;
        mobdmg1[ 0 ] = 9;
    end;
    gameovercount = 0;
    LoadLevel( 0 );
    NextGameState = 14;
    gs_music[ 17 ] = 2;
    for ( i = 0; i < 32; i++ )
        pcards[ i ] = 0;
    end;
End


Function Int ProcLoadedLevel()
Private
    int xx, yy;
begin
    for ( yy = 0; yy < 128; yy++ )
        for ( xx = 0; xx < 128; xx++ )
            if ( map0[ xx ][ yy ] == 1 )
                tcamerax = xx * 1000 + 500;
                tcameray = yy * 1000 + 500;
                SyncCamera();
                SpawnMob( xx, yy, 0 );
            end;
        end;
    end;
    for ( yy = 0; yy < 128; yy++ )
        for ( xx = 0; xx < 128; xx++ )
            if ( map0[ xx ][ yy ] == 10 )
            end;
        end;
    end;
end


Function Int LoadLevel( int levelid )
Private
    int xx, yy;
    int handle;
Begin
    ClearMobs();
    level = levelid;
    handle = fopen( "map/map" + i2s[ levelid ] + ".zvr", O_READ );
    for ( yy = 0; yy < 128; yy++ )
        for ( xx = 0; xx < 128; xx++ )
            fread( handle, map[ xx ][ yy ] );
            fread( handle, map0[ xx ][ yy ] );
        end;
    end;
    fclose( handle );
    //musictrack=levelid+2;
    gs_music[ 19 ] = levelid + 2;
    ProcLoadedLevel();
End


Function Int ProcCell( int xx, yy )
private
    int i;
begin
    i = map[ xx ][ yy ];
    if ( animcell[ i ] )
        if ( newanim )
            i = animcell[ i ] + ( i + rand( 1, 3 )) mod 4;
        end;
    end;
    map[ xx ][ yy ] = i;
    if ( mob[ 0 ][ 0 ] > 0 )
        if ( map0[ xx ][ yy ] >= 24 )
            SpawnMob( xx, yy, map0[ xx ][ yy ] -23 );
            map0[ xx ][ yy ] = 0;
        end;
    end;
end


Function Int RenderMap()
Private
    int xx, yy, sx, sy, ccx, ccy, cx, cy, ix, iy;
    int x0, x1, y0, y1;
    int mapid, mapid1;
    int iix, iiy;
    int proxymap[ 14 ][ 14 ] =
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0;
Begin
    newanim = 0;
    if ( count mod 5 == 0 )
        newanim = 1;
    end;
    //ix=160;
    //iy=120;
    ccx = ( camerax ) / cellsize;
    ccy = ( cameray ) / cellsize;
    iix = ( camerax mod cellsize );
    iiy = ( cameray mod cellsize );
    x0 = ccx - drawrange;
    x1 = ccx + drawrange;
    y0 = ccy - drawrange;
    y1 = ccy + drawrange;
    ix = 160 + (( - ccx ) + ( ccy )) * 30;
    iy = 120 + (( - ccy ) + ( - ccx )) * 20;
    ix += ( 30 * ( - iix + iiy )) / cellsize;
    iy += ( 20 * ( - iiy - iix )) / cellsize;
    for ( yy = y0; yy <= y1; yy++ )
        for ( xx = x0; xx <= x1; xx++ )
            if ( proxymap[ 7 - ccy + yy ][ 7 - ccx + xx ] > 0 )
                //+++++++++++++//
                sx = ix + xx * 30 - yy * 30;
                sy = iy + yy * 20 + xx * 20;
                cx = xx;
                cy = yy;
                mapid = -1;
                if (( cx >= 0 ) & ( cy >= 0 ) & ( cx < 128 ) & ( cy < 128 ))
                    //if ((sx>=-30)&(sy>=-40)&(sx<350)&(sy<300))
                    ProcCell( cx, cy );
                    mapid = map[ cx ][ cy ];
                    mapid1 = map0[ cx ][ cy ];
                    //end;
                end;
                if ( mapid < 0 )
                elseif ( mapid < 16 )
                    zlDrawz( sx, sy, 10500 - sy * 10, 33 + mapid );
                    if ( mapid1 > 3 )
                        if ( mapid1 < 24 )
                            zlDrawz( sx, sy, 10400 - sy * 10, 78 + mapid1 );
                        end;
                    end;
                elseif ( mapid < 32 )
                    zlDrawz( sx, sy -20, 10000 - sy * 10, 33 + mapid );
                end;
            end;
            //+++++++++++++//
        end;
    end;
End


Function Int ProcInGame()
Begin
    ProcCamera();
    ProcMobs();
    if ( gameovercount == 0 )
        ProcPlayer();
    end;
    RenderMap();
    RenderMobs();
    RenderFields();
    RenderBullets();
    if ( gameovercount == 0 )
        RenderGui();
    elseif ( gameovercount < 80 )
        zlDraw( 160, 280 -2 * gameovercount, 174 );
        gameovercount++;
    elseif ( gameovercount < 250 )
        zlDraw( 160, 120, 174 );
        gameovercount++;
    else
        nextgamestate = 02;
    end;
    if ( hurtsound > 0 )
        PSound( rand( 4, 7 ));
    end;
End
