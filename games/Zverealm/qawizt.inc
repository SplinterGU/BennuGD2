
Global
    int i_gameloopenabled;
    int cardclass[ 20 ] = 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    int i_procID[ 255 ];
    int i_gfx[ 999 ];
    int i_sfx[ 63 ];
    int i_music;
    int volume = 32, volumechangestate;
    int screenoffsetx, screenoffsety;
    int loadingcount;
    int musictrack, lastmusictrack = -1;
    int level;
    int showhpcount;
    int i_dpad;
    int i_button[ 9 ];
    int mousex, mousey, mousestate;
    int tapped, newbutton;
    int dpadmode = 0, damageindicator = 1, showmobhp = 1, cursoron = 1;
    int i_numgfx = 300;
    int i_numsfx = 11;
    int i_sp;
    int count;
    int incombat, playeridle;
    int GP2X_BUTTON_DIR[ 7 ] = 0, 7, 6, 5, 4, 3, 2, 1;
    int PC_KEY_DIR[ 7 ] = _up, _i, _right, _i, _down, _i, _left, _i;
    int PC_KEY_DIR2[ 7 ] = _i, _i, _right, _i, _down, _i, _left, _i;
    int GP2X_BUTTON_MAP[ 9 ] = 12, 13, 14, 15, 10, 11, 17, 16, 8, 9;
    int PC_KEY_MAP[ 9 ] = _a, _d, _x, _w, _z, _c, _q, _e, _esc, _tab;
    string i2s[ 9 ] = "0", "1", "2", "3", "4", "5", "6", "7", "8", "9";
    int cellsize = 1000;
    int camerax, cameray, tcamerax, tcameray, gameovercount;
    int mob[ 127 ][ 31 ];
    int mobs = 127;
    byte map[ 127 ][ 127 ];
    byte map0[ 127 ][ 127 ];
    int dpadx[ 7 ] = 0, 1, 1, 1, 0, -1, -1, -1;
    int dpady[ 7 ] = -1, -1, 0, 1, 1, 1, 0, -1;
    int mappass[ 31 ] =
    1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4;
    int pcards[ 64 ];
    int playerclass;
    int field_c[ 63 ];
    int field_g[ 63 ];
    int field_x[ 63 ];
    int field_y[ 63 ];
    int field_z[ 63 ];
    int field_ix[ 63 ];
    int field_iy[ 63 ];
    int field_iz[ 63 ];
    int field_iix[ 63 ];
    int field_iiy[ 63 ];
    int field_iiz[ 63 ];
End


Process pSprite( double x, double y, int graph )
begin
    z = 256 - i_sp;
    loop
        frame;
    end
end


function int PlayMusic( int track )
Private
    string songname[ 6 ] = "Theme", "Title", "Harbor", "Dungeon", "Caverns", "River", "Castle";
Begin
    if ( track >= 0 )
        i_music = music_load( "sfx/" + songname[ track ] + ".mp3" );
        music_play( i_music, -1 );
    else
        music_stop();
    end;
    lastmusictrack = musictrack;
End


Function int zlCls()
private
    int i;
begin
    for ( i = 0; i < i_sp; i++ )
        i_procID[ i ].graph = 0;
    end
    i_sp = 0;
end


Function int zlDraw( double x, double y, int i_sprite )
begin
    i_procId[ i_sp ].x = x + screenoffsetx;
    i_procId[ i_sp ].y = y + screenoffsety;
    i_procId[ i_sp ].z = -10 * i_sp;
    i_procId[ i_sp ].graph = i_gfx[ i_sprite ];
    i_sp++;
end


Function int zlDrawz( double x, double y, int z, int i_sprite )
begin
    i_procId[ i_sp ].x = x + screenoffsetx;
    i_procId[ i_sp ].y = y + screenoffsety;
    i_procId[ i_sp ].z = z;
    i_procId[ i_sp ].graph = i_gfx[ i_sprite ];
    i_sp++;
end


Function int zlInitGraph()
private
    int i;
    string path;
Begin
    for ( i = 0; i < 255; i++ )
        i_procId[ i ] = pSprite( x, y, i_gfx[ 0 ] );
    end
    path = "gfx/001.png";
    i_gfx[ 1 ] = map_load( path );
    zlDraw( 160, 120, 1 );
    // jjp    frame;
    for ( i = 2; i <= i_numgfx; i++ )
        path = "gfx/" + i2s[ ( i / 100 ) mod 10 ] + i2s[ ( i / 10 ) mod 10 ] + i2s[ ( i ) mod 10 ] + ".png";
        i_gfx[ i ] = map_load( path );
    end;
    i_sp = 0;
End


Function Int zlInitSound()
Private
    int i;
    string path;
Begin
    for ( i = 0; i <= i_numsfx; i++ )
        path = "sfx/" + i2s[ ( i / 10 ) mod 10 ] + i2s[ ( i ) mod 10 ] + ".wav";
        i_sfx[ i ] = sound_load( path );
    end;
End


Function int zlInit()
begin
    window_set_title( "Zverealm" );
    screen.scale_resolution = 06400480;
    set_mode( 320, 240, mode_modal );
    set_fps( 25, 0 );
    zlInitGraph();
    zlInitSound();
end


Function int zlUnloadGraph()
private
    int i;
Begin
    for ( i = 1; i <= i_numgfx; i++ )
        map_unload( i_gfx[ i ], 0 );
    end;
End


Function int zlUnloadSound()
private
    int i;
begin
    for ( i = 0; i <= i_numsfx; i++ )
        sound_unload( i_sfx[ i ] );
    end;
    music_unload( i_music );
End


Function int zlShutDown()
Begin
    zlUnloadSound();
    zlUnloadGraph();
End


Function int zlControlState()
Private
    Int bt[ 9 ];
    int dirbt[ 7 ];
    Int i;
Begin
    i_dpad = -1;
#if 0
    if (( Joy_number() > 0 ) & ( cursoron == 0 ))
        for ( i = 0; i < 10; i++ )
            bt[ i ] = 0;
            if ( joy_getbutton( 0, GP2X_BUTTON_MAP[ i ] ))
                bt[ i ] = 1;
            end;
        end;
        for ( i = 0; i < 8; i++ )
            dirbt[ i ] = 0;
            if ( joy_getbutton( 0, GP2X_BUTTON_DIR[ i ] ))
                dirbt[ i ] = 1;
                i_dpad = i;
            end;
        end;
    else //keyboard
#endif
        for ( i = 0; i < 10; i++ )
            bt[ i ] = 0;
            if ( key( PC_KEY_MAP[ i ] ))
                bt[ i ] = 1;
            end;
        end;
        for ( i = 0; i < 7; i++ )
            dirbt[ i ] = 0;
            if ( key( PC_KEY_DIR[ i ] ))
                dirbt[ i ] = 1;
                i_dpad = i;
            end;
        end;
        if ( dirbt[ 0 ] & dirbt[ 2 ] )
            i_dpad = 1;
        end;
        if ( dirbt[ 2 ] & dirbt[ 4 ] )
            i_dpad = 3;
        end;
        if ( dirbt[ 4 ] & dirbt[ 6 ] )
            i_dpad = 5;
        end;
        if ( dirbt[ 6 ] & dirbt[ 0 ] )
            i_dpad = 7;
        end;
#if 0
    end; //ifjoy
#endif
    for ( i = 0; i < 10; i++ )
        if ( bt[ i ] )
            i_button[ i ]++;
        else
            i_button[ i ] = 0;
        end;
    end;
    newbutton = 0;
    for ( i = 0; i < 10; i++ )
        if (( i != 7 ) & ( i != 6 ) & ( i_button[ i ] == 1 ))
            newbutton = 1;
        end;
    end;
    mousex = mouse.x;
    mousey = mouse.y;
    if ( mouse.left )
        mousestate++;
    else
        mousestate = 0;
    end;
    tapped = 0;
    if ( mousestate == 1 )
        tapped = 1;
    end;
End


Function Int ifclick( double x, y, xx, yy )
private
    int a;
Begin
    a = 0;
    if (( mousestate == 1 ) & ( mousex > x ) & ( mousey > y ) & ( mousex < xx ) & ( mousey < yy ))
        a = 1;
    end;
    return a;
End


Function int PSound( int i )
Begin
    sound_play( i_sfx[ i ], 0 );
End
