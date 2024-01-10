
global
    int menuscroll = 0;
    int screenkeydelay;
end


Function Int ProcMenuIngameOptions();
begin
    zldraw( 270 + rand( -2, 2 ), 80 + rand( -2, 2 ), 181 - damageindicator );
    zldraw( 270 + rand( -2, 2 ), 110 + rand( -2, 2 ), 181 - showmobhp );
    if ( ifclick( 19, 71, 253, 93 ))
        PSound( 2 );
        damageindicator = ( damageindicator + 1 ) % 2;
    elseif ( ifclick( 20, 101, 238, 122 ))
        PSound( 2 );
        showmobhp = ( showmobhp + 1 ) % 2;
    elseif ( ifclick( 19, 128, 116, 149 ))
        PSound( 2 );
        nextgamestate = 15;
    end;
end


function int ProcMenuMobs();
begin
    zlDraw( 160, 120, 176 + count / 3 % 2 );
end


Function Int ProcMainMenu()
Begin
    zlDraw( 1000 - menuscroll mod 1800, 120, 4 );
    zlDraw( 160, 120, 5 );
    menuscroll++;
    if ( ifclick( 70, 94, 264, 117 ))
        PSound( 0 );
        nextgamestate = 06;
    elseif ( ifclick( 72, 125, 186, 147 ))
        PSound( 0 );
        nextgamestate = 07;
    elseif ( ifclick( 72, 154, 209, 175 ))
        PSound( 0 );
        nextgamestate = 08;
    elseif ( ifclick( 6, 202, 43, 237 ))
        nextgamestate = 02;
    elseif ( ifclick( 50, 202, 86, 237 ))
        nextgamestate = 03;
    end;
    if (( newbutton == 1 ) | ( i_dpad >= 0 ))
        if ( screenkeydelay == 0 )
            screenkeydelay = 40;
        end;
    end;
    if ( screenkeydelay > 0 )
        if ( screenkeydelay / 7 % 2 == 0 )
            zlDraw( 160, 190, 29 );
        end;
        screenkeydelay--;
    end;
End


Function Int ProcSPMenu()
Begin
    zlDraw( 1000 - menuscroll mod 1800, 100, 4 );
    zlDraw( 160, 120, 11 );
    menuscroll += 2;
    if ( ifclick( 15, 98, 108, 127 ))
        PlayerClass = 0;
        StartNewGame();
    elseif ( ifclick( 133, 98, 196, 127 ))
        PlayerClass = 1;
        StartNewGame();
    elseif ( ifclick( 225, 98, 288, 127 ))
        PlayerClass = 2;
        StartNewGame();
    elseif ( ifclick( 245, 176, 310, 210 ))
        PSound( 1 );
        nextgamestate = 05;
    end;
end


Function Int ProcInGameMenu()
Begin
    if ( ifclick( 75, 54, 249, 74 ))
        PSound( 2 );
        gs_music[ 19 ] = 0;
        nextgamestate = 19;
    elseif ( ifclick( 47, 78, 278, 98 ))
        PSound( 2 );
        nextgamestate = 12;
    elseif ( ifclick( 74, 103, 250, 123 ))
        PSound( 2 );
        nextgamestate = 13;
    elseif ( ifclick( 103, 128, 220, 146 ))
        PSound( 2 );
        nextgamestate = 16;
    elseif ( ifclick( 95, 151, 227, 170 ))
        PSound( 1 );
        nextgamestate = 05;
    end;
end


Function Int ProcMusicPlayer()
Begin
    zlDraw( 1000 - menuscroll mod 1800, 120, 4 );
    zlDraw( 160, 120, 12 );
    menuscroll += 1;
    if ( ifclick( 6, 44, 94, 63 ))
        musictrack = 0;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 65, 129, 85 ))
        musictrack = 1;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 90, 136, 106 ))
        musictrack = 2;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 110, 149, 130 ))
        musictrack = 3;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 133, 103, 150 ))
        musictrack = 4;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 155, 143, 173 ))
        musictrack = 5;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 6, 178, 164, 195 ))
        musictrack = 6;
        LoadingCount = rand( 5, 25 );
    elseif ( ifclick( 100, 207, 164, 233 ))
        nextgamestate = 07;
        PSound( 1 );
    end;
End


Function Int ProcSystemMenu()
Begin
    zlDraw( 80, 130, 14 + dpadmode );
    zlDraw( 215, 100, 17 + damageindicator );
    zlDraw( 215, 150, 19 + showmobhp );
    if ( ifclick( 30, 80, 130, 180 ))
        dpadmode++;
        PSound( 0 );
        if ( dpadmode == 3 )
            dpadmode = 0;
        end;
    elseif ( ifclick( 135, 80, 295, 120 ))
        PSound( damageindicator );
        damageindicator = ( damageindicator + 1 ) mod 2;
    elseif ( ifclick( 135, 130, 295, 170 ))
        PSound( showmobhp );
        showmobhp = ( showmobhp + 1 ) mod 2;
    elseif ( ifclick( 14, 200, 77, 221 ))
        nextgamestate = 07;
        PSound( 1 );
    end;
End


Function Int ProcOptions()
Begin
    zlDraw( 1000 - menuscroll mod 1800, 80, 4 );
    zlDraw( 160, 120, 8 );
    menuscroll += 2;
    if ( ifclick( 31, 98, 176, 130 ))
        nextgamestate = 10;
        PSound( 0 );
    elseif ( ifclick( 31, 142, 189, 178 ))
        nextgamestate = 11;
        PSound( 0 );
    elseif ( ifclick( 31, 187, 245, 215 ))
        nextgamestate = 05;
        PSound( 1 );
    end;
end


Function Int ProcExit()
Begin
    zlDraw( 1000 - menuscroll mod 1800, 160, 4 );
    zlDraw( 160, 120, 7 );
    menuscroll += 2;
    if ( ifclick( 61, 92, 147, 144 ))
        nextgamestate = 09;
        PSound( 0 );
    elseif ( ifclick( 180, 92, 242, 144 ))
        nextgamestate = 05;
        PSound( 1 );
    end;
end
