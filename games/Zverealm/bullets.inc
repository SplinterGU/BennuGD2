
global
    int bulletc[ 63 ], bulletx[ 63 ], bullety[ 63 ], bulletz[ 63 ], bulletix[ 63 ], bulletiy[ 63 ], bulletg[ 63 ], bullett[ 63 ], bulleto[ 63 ];
    int bulletdata[ 7 ][ 6 ] =
    // CNT AT0 AT1  SPD  GFX  ANI
    30, 1, 6, 100, 220, 3, 3, // 0 dagger
    30, 0, 8, 170, 230, 0, 3, // 1 arrow
    13, 4, 12, 250, 240, 0, 3, // 2 slash
    30, 1, 8, 200, 250, 0, 3, // 3 magic missle
    20, 1, 9, 150, 260, 3, 3, // 4 firebolt 
    30, 4, 24, 180, 270, 3, 3, // 5 fireball
    30, 0, 14, 220, 280, 2, 1, // 6 lighting
    25, 1, 10, 160, 290, 3, 15; // 7 holybolt
end


function int SpawnBullet( int tp, xx, yy, angle, owner )
private
    int i, a;
begin
    i = 0;
    while (( bulletc[ i ] > 0 ) & ( i < 64 ))
        i++;
    end;
    if ( i == 64 )
        i = rand( 0, 63 );
    end;
    bulletc[ i ] = bulletdata[ tp ][ 0 ];
    bulletx[ i ] = xx;
    bullety[ i ] = yy;
    bulletz[ i ] = 40;
    bullett[ i ] = tp;
    bulleto[ i ] = owner;
    a = angle;
    bulletix[ i ] = bulletdata[ tp ][ 3 ] * dpadx[ a ];
    bulletiy[ i ] = bulletdata[ tp ][ 3 ] * dpady[ a ];
    bulletg[ i ] = bulletdata[ tp ][ 4 ] + ( angle + 1 ) % 8;
end


Function Int FadeBullet( int i, forced )
Private
    int t, k, n;
Begin
    bulletc[ i ] = 0;
    t = bullett[ i ];
    if ( t == 0 )
        if ( rand( 0, 1 ) == 0 )
            SmartField( 228, 8, bulletx[ i ], bullety[ i ] + 500, 40, 80, -80, 4, 0, 0, 0 );
        else
            SmartField( 229, 8, bulletx[ i ] + 500, bullety[ i ], 40, -80, + 80, 4, 0, 0, 0 );
        end;
    elseif ( t == 1 )
        SmartField( bulletg[ i ], 20, bulletx[ i ], bullety[ i ], 50, 0, 0, 0, 0, 0, 0 );
    elseif ( t == 2 )
        if ( rand( 0, 1 ) == 0 )
            SmartField( 228, 8, bulletx[ i ], bullety[ i ] + 500, 40, 80, -80, 4, 0, 0, 0 );
        else
            SmartField( 229, 8, bulletx[ i ] + 500, bullety[ i ], 40, -80, + 80, 4, 0, 0, 0 );
        end;
    elseif ( t == 3 )
        n = rand( 1, 3 );
        for ( k = 0; k < n; k++ )
            SmartField( 166, rand( 10, 20 ), bulletx[ i ] + 100, bullety[ i ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    elseif ( t == 4 )
        n = rand( 2, 4 );
        for ( k = 0; k < n; k++ )
            SmartField( 164, rand( 10, 20 ), bulletx[ i ] + 100, bullety[ i ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    elseif ( t == 5 )
        n = rand( 4, 6 );
        for ( k = 0; k < n; k++ )
            SmartField( 164, rand( 10, 20 ), bulletx[ i ] + 100, bullety[ i ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    elseif ( t == 6 )
        SmartField( 288, 2, bulletx[ i ] + 100, bullety[ i ] + 100, 50, 0, 0, 0, 0, 0, 0 );
        n = rand( 3, 5 );
        for ( k = 0; k < n; k++ )
            SmartField( 165, rand( 10, 20 ), bulletx[ i ] + 100, bullety[ i ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    end;
End


function BulletEffect( int i )
begin
    if ( bullett[ i ] == 5 )
        if ( count / 2 % 6 == 0 )
            SmartField( 167, rand( 10, 20 ), bulletx[ i ] + 100, bullety[ i ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    end;
end


function int RenderBullets()
private
    int i, xx, yy, sx, sy, ii;
    int blocked, blockedbymob;
begin
    for ( i = 0; i < 64; i++ )
        if ( bulletc[ i ] > 0 )
            ///
            xx = bulletx[ i ] - camerax;
            yy = bullety[ i ] - cameray;
            if ( bulletdata[ bullett[ i ]][ 5 ] > 1 )
                bulletg[ i ] = bulletdata[ bullett[ i ]][ 4 ] + ( count / bulletdata[ bullett[ i ]][ 5 ] ) % 8;
            end;
            sx = 160 + ( xx - yy ) * 30 / cellsize;
            sy = 120 + ( xx + yy ) * 20 / cellsize;
            BulletEffect( i );
            if (( sx > -30 ) & ( sx < 350 ) & ( sy > -50 ) & ( sy < 320 ))
                zlDrawz( sx, sy - bulletz[ i ], 10200 - sy * 10, bulletg[ i ] );
            end;
            blocked = 0;
            if ( bulletx[ i ] < 1000 )
                blocked = 1;
            elseif ( bullety[ i ] < 1000 )
                blocked = 1;
            elseif ( bulletx[ i ] > 127000 )
                blocked = 1;
            elseif ( bullety[ i ] > 127000 )
                blocked = 1;
            end;
            if ( blocked == 0 )
                if (( mappass[ ( map[ bulletx[ i ] / 1000 ][ bullety[ i ] / 1000 ] ) ] & bulletdata[ bullett[ i ]][ 6 ] ) == 0 )
                    blocked = 1;
                end;
            end;
            if ( blocked == 0 )
                for ( ii = 0; ii < mobs; ii++ )
                    if ( mob[ ii ][ 0 ] > 0 )
                        if ( ii != bulleto[ i ] )
                            if ( abs( mob[ ii ][ 2 ] - bulletx[ i ] ) < 600 )
                                if ( abs( mob[ ii ][ 3 ] - bullety[ i ] ) < 600 )
                                    blocked = 1;
                                    HurtMob( ii, rand( bulletdata[ bullett[ i ]][ 1 ], bulletdata[ bullett[ i ]][ 2 ] ));
                                end;
                            end;
                        end;
                    end;
                end;
            end;
            bulletx[ i ] += bulletix[ i ];
            bullety[ i ] += bulletiy[ i ];
            bulletc[ i ]--;
            if ( blocked == 1 )
                FadeBullet( i, 1 );
            else
                if ( bulletc[ i ] == 0 )
                    FadeBullet( i, 0 );
                end;
            end;
            ///
        end;
    end;
end
