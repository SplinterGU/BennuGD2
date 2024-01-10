 /*
00 - init
01 - LOADING
02 - GPH logo
03 - BennuGD Logo
04 - ???
05 - MainMenu
06 - SPMenu
07 - Options
08 - Exit
09 - Thanks for playing
10 - Music Player
11 - Game System Configuration

12 - Monsters
13 - Cards
14 - NewGame menu

15 - InGame Menu
16 - Ingame Options
17 - Loading <3
18 - End Game
19 - Ingame
*/
/*gs_trig

-1 - no automated trigger
00 - click or tap
01-09 - 1-9 seconds

*/
global
    int gamestate = 0, nextgamestate = 1, stateswitch, gamestatecount;
//                       --  --  ++  ++  --  ??  --  --  --  --
//                       00  01  02  03  04  05  06  07  08  09  10  11  12   13   14  15  16  17   18  19  
    int gs_image[ 19 ] = 01, 01, 02, 03, 00, 00, 00, 00, 00, 10, 00, 13, 00, 175, 179, 23, 30, 00, 178, 00;
    int gs_trig[  19 ] = 00, 01, 03, 03, 00, -1, -1, -1, -1, -1, -1, -1, 00,  00,  00, -1, -1, -1,  00, -1;
    int gs_next[  19 ] = 02, 02, 03, 05, 05, 00, 00, 00, 00, 00, 00, 00, 15,  15,  17, 05, 15, 00,  09, 00;
    int gs_music[ 19 ] = -1, 01, 01, 01, 01, 01, 01, 01, 01, 01, -1, 01, 00,  00,  01, 00, -1, -1,  01, 00;
    int gs_fx0[   19 ] = 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,  01,  01, 01, 01, 01,  01, 00;
    int switchdelay = 20;
    int switchid = -12;
    int particle[ 16 ][ 8 ];
    int loadingcounts, loadingscreen;
end

include "mainmenu.inc"
include "ingame.inc"

function int ProcLoading()
private
    int k;
begin
    zldraw( 160, 120, 170 + loadingscreen );
    for ( k = 0; k <= loadingcounts * 320 / 500; k++ )
        zldraw( k * 5, 236, 189 );
    end;
    if (( loadingcounts == 0 ) | ( loadingcounts == 40 ))
        loadingscreen = rand( 0, 3 );
    elseif ( loadingcounts > 100 )
        nextgamestate = 19;
        loadingcounts = 0;
    end;
    if ( loadingcounts > 60 )
        loadingcounts += rand( 0, 3 );
    end;
    loadingcounts++;
end


Function Int GameProc()
Private
Begin
        if ( gamestate == 05 ) //main menu
        ProcMainMenu();
    elseif ( gamestate == 06 ) //sp menu
        ProcSPmenu();
    elseif ( gamestate == 07 ) //options menu
        ProcOptions();
    elseif ( gamestate == 08 ) //exit menu
        ProcExit();
    elseif ( gamestate == 09 ) //exit 
        if (( newbutton ) | ( tapped ))
            i_gameloopenabled = 0;
        end;
    elseif ( gamestate == 10 ) //music menu
        ProcMusicPlayer();
    elseif ( gamestate == 11 ) //sys menu
        ProcSystemMenu();
    elseif ( gamestate == 12 ) //mobs menu
        ProcMenuMobs();
    elseif ( gamestate == 15 ) //sys menu
        ProcInGameMenu();
    elseif ( gamestate == 16 ) //ingameoptions menu
        ProcMenuIngameOptions();
    elseif ( gamestate == 17 ) //sys menu
        ProcLoading();
    elseif ( gamestate == 19 ) //Game Play
        if ( nextgamestate != 17 )
            ProcInGame();
        end;
    end;
End


Function Int DrawParticle()
private
    int i;
begin
    if ( mousestate % 2 == 1 )
        i = 0;
        while (( i < 16 ) & ( particle[ i ][ 0 ] > 0 ))
            i++;
        end;
        if ( i == 16 )
            i = rand( 0, 15 );
        end;
        particle[ i ][ 0 ] = 20;
        particle[ i ][ 2 ] = mousex + rand( -8, 8 );
        particle[ i ][ 3 ] = mousey + rand( -8, 8 );
        particle[ i ][ 4 ] = rand( -3, 3 );
        particle[ i ][ 5 ] = rand( -8, 8 );
        particle[ i ][ 6 ] = rand( -3, 3 );
        particle[ i ][ 7 ] = rand( -1, -3 );
    end;
    for ( i = 0; i < 16; i++ )
        if ( particle[ i ][ 0 ] > 0 )
            zlDrawz( particle[ i ][ 2 ], particle[ i ][ 3 ], -1000, 165 );
            particle[ i ][ 2 ] += particle[ i ][ 4 ];
            particle[ i ][ 3 ] += particle[ i ][ 5 ];
            particle[ i ][ 4 ] += particle[ i ][ 6 ];
            particle[ i ][ 5 ] += particle[ i ][ 7 ];
            if ( particle[ i ][ 3 ] < 0 )
                particle[ i ][ 0 ] = 0;
            end;
        end;
    end;
end


Function Int GameStep()
Begin
    if ( gs_fx0[ gamestate ] )
        DrawParticle();
    end;
    if ( gs_image[ gamestate ] )
        zlDraw( 160, 120, gs_image[ gamestate ] );
    end;
    if ( stateswitch )
        newbutton = 0;
        tapped = 0;
        mousestate = 0;
    end;
    GameProc(); //!!!!!!!
    if ( stateswitch )
        zlDraw( -480 -960 * stateswitch / switchdelay, 120, 6 );
        if ( rand( 0, 10 ) == 0 )
            zlDraw( rand( 0, 320 ), rand( 0, 240 ), 28 );
        end;
        stateswitch++;
        if ( stateswitch == switchid )
            gamestate = nextgamestate;
            gamestatecount = 0;
            musictrack = gs_music[ gamestate ];
            timer[0] = 0;
        end;
    else
        gamestatecount++;
        
        switch ( gs_trig[ gamestate ] ) // Fixed time trigger SplinterGU
            case    0:
                if ( newbutton || tapped )
                    PSound( 3 );
                    nextgamestate = gs_next[ gamestate ];
                end;
            end
            case 1..9:
                if ( timer[ 0 ] > gs_trig[ gamestate ] * 100 )
                    PSound( 3 );
                    nextgamestate = gs_next[ gamestate ];
                end;
            end
        end;

        //
        if ( nextgamestate != gamestate )
            LoadingCount = rand( 5, 25 );
            stateswitch = -switchdelay;
        end;
    end;
    //if (i_button[8] & i_button[9])
    //i_gameloopenabled=0;
    //End
    //if (i_button[9])
    //cursoron=(cursoron+1)mod 2;
    //End
    DrawVolume();
    DrawLoadingCount();
End


Function Int ProcVolume()
Private
    int i;
Begin
    if (( i_button[ 6 ] ) | ( i_button[ 7 ] ))
        if ( volumechangestate < 10 )
            volumechangestate++;
            screenoffsety -= 2;
        end;
        if ( i_button[ 6 ] )
            if ( volume > 0 )
                volume--;
            end;
        elseif ( i_button[ 7 ] )
            if ( volume < 128 )
                volume++;
            end;
        end;
        for ( i = 0; i <= i_numsfx; i++ )
            sound_set_volume( i_sfx[ i ], volume );
        end;
        music_set_volume( volume );
    else
        if ( volumechangestate > 0 )
            volumechangestate--;
            screenoffsety += 2;
        end;
    end;
    if ( volumechangestate > 0 )
        tapped = 0;
        mousestate = 0;
    end;
End


Function Int DrawLoadingCount()
Private
Begin
    if ( LoadingCount )
        LoadingCount--;
        zlDraw( 260, 230, 24 + count / 2 % 2 );
    end;
End


Function Int DrawVolume()
Private
    int i, yy;
Begin
    if ( volumechangestate > 0 )
        zlDraw( 160, 245 - volumechangestate / 2, 21 );
        yy = 258 - volumechangestate / 2;
        for ( i = 0; i < volume * 250 / 1000; i++ )
            zlDraw( 12 + i * 10, yy, 22 );
        end;
    end;
End
