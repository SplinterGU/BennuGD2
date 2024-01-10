
global
    int guicount, carddelay;
    int pcardx[ 64 ], pcardy[ 64 ], pcardtx[ 64 ], pcardty[ 64 ];
    int handx[ 8 ] = 283, 303, 283, 263, 255, 300, 285, 270;
    int handy[ 8 ] = 22, 32, 42, 32, 34, 34, 34, 34;
    int ssi[ 8 ] = 0, -1, -2, -3, -2, -1, 0, -1;
    int cardrestore[ 21 ] =
    4,
    10, 10, 10, 10, 10,
    100, 200, 50, 150, 500,
    25, 40, 80, 100, 140,
    60, 50, 500, 200, 500;
    int cardwait[ 20 ];
    int dpadmode_data[ 2 ] = 0, 1, 7;
end


Function Int UseCard( int ii )
Private
    int i, card;
Begin
    card = pcards[ ii ];
    if ( cardwait[ card ] > 0 )
        // :(
    else
        cardwait[ card ] = cardrestore[ card + 1 ];
        UseSkill( 0, Card );
        pcards[ ii ] = pcards[ 4 ];
        pcardx[ ii ] = pcardx[ 4 ];
        pcardy[ ii ] = pcardy[ 4 ];
        i = 4;
        while (( pcards[ i ] ) and( i < 31 ))
            pcards[ i ] = pcards[ i + 1 ];
            pcardx[ i ] = pcardx[ i + 1 ];
            pcardy[ i ] = pcardy[ i + 1 ];
            i++;
        end;
        pcards[ i ] = 0;
        if ( cardclass[ card ] == playerclass )
            i = 0;
            while (( pcards[ i ] ) and( i < 31 ))
                i++;
            end;
            pcards[ i ] = card;
        end;
    end;
End


Function Int TryLoot()
Private
    int c, i, k;
Begin
    c = map0[ mob[ 0 ][ 4 ]][ mob[ 0 ][ 5 ]];
    if (( c >= 4 ) and( c <= 23 ))
        i = 0;
        while (( pcards[ i ] > 0 ) and( i < 32 ))
            i++;
        end;
        if ( i < 32 )
            pcards[ i ] = c -3;
            pcardx[ i ] = mob[ 0 ][ 8 ] * 128;
            pcardy[ i ] = mob[ 0 ][ 9 ] * 128;
            map0[ mob[ 0 ][ 4 ]][ mob[ 0 ][ 5 ]] = 0;
            for ( k = 1; k < 5; k++ )
                SmartField( 169, rand( 10, 20 ), mob[ 0 ][ 2 ] + 100, mob[ 0 ][ 3 ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
            end;
            psound( rand( 8, 11 ));
        end; //i<32
    end;
    if ( map[ mob[ 0 ][ 4 ]][ mob[ 0 ][ 5 ]] >= 14 )
        nextgamestate = 17;
        gs_music[ 17 ] = 3 + level;
        LoadLevel( level + 1 );
    end;
End


Function Int RotateHandLeft()
Private
    int xx, yy, ii, i;
Begin
    ii = pcards[ 0 ];
    xx = pcardx[ 0 ];
    yy = pcardy[ 0 ];
    for ( i = 1; i <= 3; i++ )
        pcards[ i -1 ] = pcards[ i ];
        pcardx[ i -1 ] = pcardx[ i ];
        pcardy[ i -1 ] = pcardy[ i ];
    end;
    pcardx[ 3 ] = xx;
    pcardy[ 3 ] = yy;
    pcards[ 3 ] = ii;
    psound( 3 );
End


Function Int RotateHandRight()
Private
    int xx, yy, ii, i;
Begin
    ii = pcards[ 3 ];
    xx = pcardx[ 3 ];
    yy = pcardy[ 3 ];
    for ( i = 2; i >= 0; i-- )
        pcards[ i + 1 ] = pcards[ i ];
        pcardx[ i + 1 ] = pcardx[ i ];
        pcardy[ i + 1 ] = pcardy[ i ];
    end;
    pcardx[ 0 ] = xx;
    pcardy[ 0 ] = yy;
    pcards[ 0 ] = ii;
    psound( 3 );
End


Function Int RenderPlayerSpecial()
private
    int sx, sy;
begin
    sx = mob[ 0 ][ 8 ];
    sy = mob[ 0 ][ 9 ] + 20;
    //    if ( count % 2 == 0 )
    zlDrawz( sx, sy -15, 10230 - sy * 10, 81 );
    //    end;
    if (( mob[ 0 ][ 23 ] > 0 ) & ( count % 3 == 0 ))
        zlDrawz( sx, sy -15, 10230 - sy * 10, 249 );
    end;
    if (( mob[ 0 ][ 24 ] > 0 ))
        zlDrawz( sx - rand( -15, 15 ), sy -15 - rand( -5, 5 ), 10230 - sy * 10, 258 );
    end;
end


Function Int ProcPlayer()
Private
    int pdir, bx, by;
Begin
    RenderPlayerSpecial();
    if ( mob[ 0 ][ 10 ] < mobhp[ 0 ] )
        if ( count % 100 == 0 )
            mob[ 0 ][ 10 ]++;
        end;
    end;
    bx = 15;
    by = 20;
    pdir = -1;
    if ( i_dpad > -1 )
        pdir = ( i_dpad + dpadmode_data[ dpadmode ] ) % 8;
    end;
    if ( mousestate > 0 )
        if (( mousex > mob[ 0 ][ 8 ] + bx ) and( mousey < mob[ 0 ][ 9 ] - by ))
            pdir = 1;
        elseif (( mousex < mob[ 0 ][ 8 ] - bx ) & ( mousey < mob[ 0 ][ 9 ] - by ))
            pdir = 7;
        elseif (( mousex > mob[ 0 ][ 8 ] + bx ) & ( mousey > mob[ 0 ][ 9 ] + by ))
            pdir = 3;
        elseif (( mousex < mob[ 0 ][ 8 ] - bx ) & ( mousey > mob[ 0 ][ 9 ] + by ))
            pdir = 5;
        elseif (( mousex < mob[ 0 ][ 8 ] - bx ))
            pdir = 6;
        elseif (( mousex > mob[ 0 ][ 8 ] + bx ))
            pdir = 2;
        elseif (( mousey > mob[ 0 ][ 9 ] + by ))
            pdir = 4;
        elseif (( mousey < mob[ 0 ][ 9 ] - by ))
            pdir = 0;
        end;
    end;
    if ( pdir > -1 )
        mob[ 0 ][ 11 ] = ( pdir + 7 ) % 8;
        mob[ 0 ][ 6 ] += dpadx[ ( 7 + pdir ) mod 8 ] * 11;
        mob[ 0 ][ 7 ] += dpady[ ( 7 + pdir ) mod 8 ] * 11;
        mob[ 0 ][ 1 ] = 65 + pdir + ( count / 5 % 2 ) * 8;
    end;
    if ( i_button[ 8 ] )
        NextGameState = 15;
    End
    if ( carddelay )
        carddelay--;
    else
        if ( i_button[ 0 ] == 1 )
            carddelay = 5;
            UseCard( 3 );
        elseif ( i_button[ 1 ] == 1 )
            carddelay = 5;
            UseCard( 1 );
        elseif ( i_button[ 2 ] == 1 )
            carddelay = 5;
            UseCard( 2 );
        elseif ( i_button[ 3 ] == 1 )
            carddelay = 5;
            UseCard( 0 );
        elseif ( i_button[ 4 ] )
            carddelay = 5;
            RotateHandLeft();
        elseif ( i_button[ 5 ] )
            carddelay = 5;
            RotateHandRight();
        end;
    end;
    if (( pdir == -1 ) & ( carddelay == 0 ) & ( incombat == 0 ))
        playeridle++;
    else
        playeridle = 0;
    end;
    TryLoot();
End


Function Int RenderCards()
Private
    int i, cardnum, cx;
Begin
    for ( i = 0; i < 20; i++ )
        if ( cardwait[ i ] > 0 )
            cardwait[ i ]--;
        end;
    end;
    cardnum = 0;
    for ( i = 0; i < 32; i++ )
        if ( pcards[ i ] > 0 )
            cardnum++;
        end;
    end;
    cx = 18;
    if ( cardnum > 5 )
        cx = 180 / ( cardnum -4 );
        if ( cx > 20 )
            cx = 20;
        end;
    end;
    for ( i = 0; i < 32; i++ )
        if ( pcards[ i ] > 0 )
            zldraw( pcardx[ i ] / 128, pcardy[ i ] / 128, 81 + pcards[ i ] );
            if ( cardwait[ pcards[ i ]] > 0 )
                zldraw( pcardx[ i ] / 128, pcardy[ i ] / 128, 259 );
            end;
            if ( i < 4 )
                if ( guicount > 0 )
                    pcardtx[ i ] = handx[ i + 4 ] * 128;
                    pcardty[ i ] = handy[ i + 4 ] * 128;
                else
                    pcardtx[ i ] = handx[ i ] * 128;
                    pcardty[ i ] = handy[ i ] * 128;
                end;
            else
                pcardtx[ i ] = ( 190 - ( i -4 ) * cx ) * 128;
                pcardty[ i ] = ( 35 + ssi[ ( i + count / 8 ) % 8 ] ) * 128;
            end;
            pcardx[ i ] = pcardx[ i ] + ( pcardtx[ i ] - pcardx[ i ] ) / 8;
            pcardy[ i ] = pcardy[ i ] + ( pcardty[ i ] - pcardy[ i ] ) / 8;
        end;
    end;
End


Function Int RenderGUI()
Begin
    if ( playeridle > 100 )
        if ( guicount < 10 )
            guicount++;
        end;
    else
        if ( guicount > 0 )
            guicount--;
        end;
    end;
    if ( guicount > 0 )
        zldraw( 160, -22 + guicount * 5, 26 );
        zldraw( 40, 262 - guicount * 5, 27 );
    end;
    if (( incombat ) | ( guicount > 0 ) | ( showhpcount > 0 ))
        if ( showhpcount > 0 )
            showhpcount--;
        end;
        if ( incombat )
            showhpcount = 40;
        end;
        zldraw( 46, 216, 190 + ( 9 - mob[ 0 ][ 10 ] * 9 / mobhp[ 0 ] ));
        if ( mob[ 0 ][ 10 ] < 10 )
            zldraw( 50, 216, 210 + mob[ 0 ][ 10 ] );
        else
            zldraw( 50, 216, 210 + ( mob[ 0 ][ 10 ] % 10 ));
            zldraw( 43, 216, 210 + ( mob[ 0 ][ 10 ] / 10 ));
        end;
    end;
    RenderCards();
End
