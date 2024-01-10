
/*
0- Enabled
1- SpriteID
2- X
3- Y
4-CX
5-CY
6-iX
7-iY
8-SX
9-SY
10-hp!!!
11-angle
12-skilldelay
13-hurtcount
14-attackdelay
15-index of type
16-passbyte
20-AIcode
21-AIDelay
22-Target
23-Armor
24-berzerk
*/
/*
AI
0- stand
1- walt to player
2-10 walk in direction
*/
///////////////
global //   00 01 02 03 04 05 06 07  08 09 10 11 12 13 14 15  16 17 18 19 20 21 22 23  24 25 26 27 28 29 30 31
    int mobspeed0[ 31 ] = 04, 04, 04, 04, 04, 02, 03, 01, 04, 04, 05, 03, 04, 04, 05, 05, 00, 02, 02, 02, 02, 02, 04, 04, 04, 03, 03, 03, 03, 03, 02, 04;
    int mobspeed1[ 31 ] = 08, 08, 06, 08, 08, 04, 09, 07, 14, 08, 09, 10, 12, 09, 08, 08, 00, 12, 10, 08, 08, 06, 11, 08, 08, 06, 06, 06, 06, 06, 12, 07;
    int mobpassbt[ 31 ] = 01, 01, 01, 01, 01, 01, 01, 01, 03, 01, 01, 02, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 15, 01;
    int mobheight[ 31 ] = 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 20, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40;
    int mobatspd[ 31 ] = 20, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30;
    int mobaiside[ 31 ] = 0, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 9, 2;
    int mobdrop0[ 31 ] = 0, 0, 7, 5, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 15, 07, 0, 0, 0, 11, 14, 09, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0;
    int mobdrop1[ 31 ] = 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 4, 4, 18, 0, 8, 12, 9, 0, 18, 0, 19, 14, 09, 07, 04, 0, 04, 04, 0, 0;
    int mobdmg0[ 31 ] = 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 5, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 2, 1, 1, 0, 0, 0, 0;
    int mobdmg1[ 31 ] = 12, 6, 1, 6, 10, 1, 6, 3, 3, 3, 3, 30, 6, 12, 4, 10, 0, 6, 8, 12, 4, 14, 6, 9, 5, 12, 8, 8, 6, 6, 50, 20;
    int mobhp[ 31 ] = 99, 36, 20, 36, 45, 15, 12, 8, 10, 12, 08, 99, 20, 25, 15, 99, 08, 26, 35, 50, 30, 80, 15, 30, 15, 50, 70, 70, 50, 40, 99, 500;
    int mob_await[ 31 ] = 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2;
    int mob_amove[ 31 ] = 2, 2, 2, 2, 2, 2, 3, 2, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2;
    int mob_arun[ 31 ] = 2, 2, 2, 2, 2, 2, 2, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2;
    int mob_awalk[ 31 ] = 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2;
    int mob_skill[ 31 ] = 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 11, 0, 0, 0, 12, 0, 0, 13, 0, 0, 0, 14;
    int mob_stime[ 31 ] = 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 0, 40, 0, 0, 0, 60, 0, 0, 90, 0, 0, 0, 50;
    int mob_Bleed[ 31 ] = 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 0, 3;
    int hurtsound;
end


function int healmob( int i, hp )
private
    int k;
begin
    for ( k = 0; k < 5; k++ )
        SmartField( 248, rand( 10, 20 ), mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, mobheight[ mob[ i ][ 15 ]], rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
    end;
    mob[ i ][ 10 ] += hp;
    if ( mob[ i ][ 10 ] > mobhp[ mob[ i ][ 15 ]] )
        mob[ i ][ 10 ] = mobhp[ mob[ i ][ 15 ]];
    end;
    if ( i == 0 )
        showhpcount = 40;
    end;
end


Function int SpawnMob( int xx, yy, ii )
private
    int i, k;
begin
    if ( gameovercount == 0 )
        i = 0;
        While (( i <= mobs ) & ( mob[ i ][ 0 ] > 0 ))
            i++;
        end;
        /// o_O
        if ( i <= mobs )
            for ( k = 0; k < 32; k++ )
                mob[ i ][ k ] = 0;
            end;
            mob[ i ][ 0 ] = 1;
            mob[ i ][ 1 ] = 77;
            if ( i > 0 )
                mob[ i ][ 1 ] = 102;
            end;
            mob[ i ][ 2 ] = xx * 1000 + 500;
            mob[ i ][ 3 ] = yy * 1000 + 500;
            mob[ i ][ 4 ] = xx;
            mob[ i ][ 5 ] = yy;
            mob[ i ][ 10 ] = mobhp[ ii ];
            mob[ i ][ 12 ] = mob_stime[ ii ];
            mob[ i ][ 15 ] = ii;
            mob[ i ][ 16 ] = mobpassbt[ ii ];
        end;
    end;
end


Function int ClearMobs()
private
    int i;
begin
    for ( i = 0; i < mobs; i++ )
        mob[ i ][ 0 ] = 0;
    end;
end


Function Int UpdateAngle( int i );
private
    int k, x0, y0, x1, y1, a;
begin
    k = mob[ i ][ 22 ];
    a = rand( 0, 7 );
    x0 = mob[ i ][ 2 ];
    y0 = mob[ i ][ 3 ];
    x1 = mob[ k ][ 2 ];
    y1 = mob[ k ][ 3 ];
    if ( abs( x1 - x0 ) < 1000 ) //vertical
        if ( y1 > y0 )
            a = 4;
        else
            a = 0;
        end;
    elseif ( abs( y1 - y0 ) < 1000 ) //horizontal
        if ( x1 > x0 )
            a = 2;
        else
            a = 6;
        end;
    elseif (( x1 - x0 > 0 ) & ( y1 - y0 < 0 ))
        a = 1;
    elseif (( x1 - x0 > 0 ) & ( y1 - y0 > 0 ))
        a = 3;
    elseif (( x1 - x0 < 0 ) & ( y1 - y0 > 0 ))
        a = 5;
    elseif (( x1 - x0 < 0 ) & ( y1 - y0 < 0 ))
        a = 7;
    end;
    mob[ i ][ 11 ] = a;
End


Function Int ProcMobAI( int i )
Private
    int tp, airoll, target, rng, rng0, k;
Begin
    tp = mob[ i ][ 15 ];
    if ( mob_skill[ mob[ i ][ 15 ]] > 0 )
        if ( mob[ i ][ 12 ] > 0 )
            mob[ i ][ 12 ]--;
        else
            UpdateAngle( i );
            UseSkill( i, mob_skill[ mob[ i ][ 15 ]] );
            mob[ i ][ 12 ] = mob_stime[ mob[ i ][ 15 ]];
        end;
    end;
    if ( mob[ i ][ 21 ] == 0 )
        //
        //target
        mob[ i ][ 22 ] = 0;
        rng = abs( mob[ i ][ 2 ] - mob[ 0 ][ 2 ] ) + abs( mob[ i ][ 3 ] - mob[ 0 ][ 3 ] );
        for ( k = 1; k < mobs; k++ )
            if ( mob[ k ][ 0 ] > 0 )
                if ( mobaiside[ mob[ i ][ 15 ]] != mobaiside[ mob[ k ][ 15 ]] )
                    rng0 = abs( mob[ i ][ 2 ] - mob[ k ][ 2 ] ) + abs( mob[ i ][ 3 ] - mob[ k ][ 3 ] );
                    if ( rng0 < rng )
                        rng = rng0;
                        mob[ i ][ 22 ] = k;
                    end;
                end;
            end;
        end;
        //target
        mob[ i ][ 21 ] = rand( 20, 80 );
        airoll = rand( 0, 7 );
        if ( airoll < mob_await[ tp ] )
            mob[ i ][ 20 ] = 0;
        else
            airoll -= mob_await[ tp ];
            if ( airoll < mob_awalk[ tp ] )
                mob[ i ][ 20 ] = 1;
            else
                airoll -= mob_awalk[ tp ];
                if ( airoll < mob_arun[ tp ] )
                    mob[ i ][ 20 ] = 2;
                else
                    mob[ i ][ 20 ] = 3;
                    mob[ i ][ 11 ] = rand( 0, 7 );
                end;
            end;
        end;
        //
    else
        mob[ i ][ 21 ]--;
    end;
    If ( musictrack == 0 )
        if (( mob[ i ][ 8 ] > 40 ) & ( mob[ i ][ 8 ] < 280 ))
            if (( mob[ i ][ 9 ] > 40 ) & ( mob[ i ][ 9 ] < 200 ))
                musictrack = 2 + level;
            end;
        end;
    End;
    target = mob[ i ][ 22 ];
    if ( mob[ i ][ 20 ] == 0 )
        mob[ i ][ 1 ] = 100 + mob[ i ][ 15 ] * 2 + count / 5 % 2;
    elseif ( mob[ i ][ 20 ] == 1 )
        //
        mob[ i ][ 1 ] = 100 + mob[ i ][ 15 ] * 2 + count / 4 % 2;
        if ( mob[ i ][ 2 ] > mob[ target ][ 2 ] + 200 )
            mob[ i ][ 6 ] -= mobspeed0[ tp ];
        elseif ( mob[ i ][ 2 ] < mob[ target ][ 2 ] -200 )
            mob[ i ][ 6 ] += mobspeed0[ tp ];
        end;
        if ( mob[ i ][ 3 ] > mob[ target ][ 3 ] + 200 )
            mob[ i ][ 7 ] -= mobspeed0[ tp ];
        elseif ( mob[ i ][ 3 ] < mob[ target ][ 3 ] -200 )
            mob[ i ][ 7 ] += mobspeed0[ tp ];
        end;
        //
    elseif ( mob[ i ][ 20 ] == 2 )
        mob[ i ][ 1 ] = 100 + mob[ i ][ 15 ] * 2 + count / 3 % 2;
        if ( mob[ i ][ 2 ] > mob[ target ][ 2 ] + 200 )
            mob[ i ][ 6 ] -= mobspeed1[ tp ];
        elseif ( mob[ i ][ 2 ] < mob[ target ][ 2 ] -200 )
            mob[ i ][ 6 ] += mobspeed1[ tp ];
        end;
        if ( mob[ i ][ 3 ] > mob[ target ][ 3 ] + 200 )
            mob[ i ][ 7 ] -= mobspeed1[ tp ];
        elseif ( mob[ i ][ 3 ] < mob[ target ][ 3 ] -200 )
            mob[ i ][ 7 ] += mobspeed1[ tp ];
        end;
        //
    elseif ( mob[ i ][ 20 ] == 3 )
        mob[ i ][ 1 ] = 100 + mob[ i ][ 15 ] * 2 + count / 4 % 2;
        mob[ i ][ 6 ] += dpadx[ mob[ i ][ 11 ]] * mobspeed1[ tp ];
        mob[ i ][ 7 ] += dpady[ mob[ i ][ 11 ]] * mobspeed1[ tp ];
    end;
    //begin try to vanish
    if ( abs( mob[ i ][ 8 ] -160 ) > 240 )
        if ( abs( mob[ i ][ 9 ] -120 ) > 180 )
            if ( map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] == 0 )
                mob[ i ][ 0 ] = 0;
                map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] = 23 + mob[ i ][ 15 ];
            end;
        end;
    end;
    //end   try to vanish
End


Function MobDripBlood( int i, cx, cy )
begin
    if (( cx >= 0 ) & ( cy >= 0 ) & ( cx <= 127 ) & ( cy <= 127 ))
        if ( mappass[ map[ cx ][ cy ]] & 1 > 0 )
            SmartField( 204 + rand( 0, 3 ), rand( 10, 120 ), cx * 1000 + 500 - rand( 300, 500 ), cy * 1000 + 500 - rand( 300, 500 ), -1, 0, 0, 0, 0, 0, 0 );
        end;
    end;
end


Function MobDie( int i )
private
    int tp, k;
begin
    if ( i == 0 )
        gameovercount = 1;
    end;
    if ( mob[ i ][ 15 ] == 31 )
        nextgamestate = 18;
    end;
    mob[ i ][ 0 ] = 0;
    tp = mob[ i ][ 15 ];
    if ( map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] == 0 )
        if ( mobdrop1[ tp ] > 0 )
            if ( rand( 0, 7 ) == 0 )
                if ( cardclass[ mobdrop1[ tp ] -3 ] != playerclass )
                    map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] = mobdrop1[ tp ];
                end;
            else
                if ( cardclass[ mobdrop0[ tp ] -3 ] != playerclass )
                    map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] = mobdrop0[ tp ];
                end;
            end;
        else
            if ( cardclass[ mobdrop0[ tp ] -3 ] != playerclass )
                map0[ mob[ i ][ 4 ]][ mob[ i ][ 5 ]] = mobdrop0[ tp ];
            end;
        end;
    end;
    if ( mob_bleed[ tp ] == 1 )
        MobDripBlood( i, mob[ i ][ 4 ], mob[ i ][ 5 ] );
    elseif ( mob_bleed[ tp ] == 2 )
        MobDripBlood( i, mob[ i ][ 4 ] -1, mob[ i ][ 5 ] -1 );
        MobDripBlood( i, mob[ i ][ 4 ], mob[ i ][ 5 ] -1 );
        MobDripBlood( i, mob[ i ][ 4 ] + 1, mob[ i ][ 5 ] -1 );
        MobDripBlood( i, mob[ i ][ 4 ] -1, mob[ i ][ 5 ] );
        MobDripBlood( i, mob[ i ][ 4 ], mob[ i ][ 5 ] );
        MobDripBlood( i, mob[ i ][ 4 ] + 1, mob[ i ][ 5 ] );
        MobDripBlood( i, mob[ i ][ 4 ] -1, mob[ i ][ 5 ] + 1 );
        MobDripBlood( i, mob[ i ][ 4 ], mob[ i ][ 5 ] + 1 );
        MobDripBlood( i, mob[ i ][ 4 ] + 1, mob[ i ][ 5 ] + 1 );
    elseif ( mob_bleed[ tp ] == 3 )
        for ( k = 1; k < 12; k++ )
            SmartField( 167, rand( 10, 20 ), mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, 40, rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
        end;
    end;
end


Function HurtMob( int i, damage )
private
    int n, k;
begin
    n = rand( 1, 3 );
    for ( k = 0; k < n; k++ )
        SmartField( 168, rand( 10, 20 ), mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, mobheight[ mob[ i ][ 15 ]], rand( -200, 200 ), rand( -100, 100 ), rand( 0, 2 ), rand( -10, 10 ), rand( -10, 10 ), 0 );
    end;
    hurtsound = 1;
    if ( mob[ i ][ 23 ] > 0 )
        damage = damage / 3;
        mob[ i ][ 23 ]--;
    end;
    if ( i == 0 )
        mob[ i ][ 13 ] = 4;
    else
        mob[ i ][ 13 ] = 10;
    end;
    mob[ i ][ 10 ] -= damage;
    if ( mob[ i ][ 10 ] <= 0 )
        MobDie( i );
    end;
    SimpleField( 200 + rand( 0, 3 ), mob[ i ][ 2 ], mob[ i ][ 3 ], mobheight[ mob[ i ][ 15 ]] );
    if ( damageindicator > 0 )
        if ( damage == 0 )
            SmartField( 209, 20, mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, mobheight[ mob[ i ][ 15 ]], 0, 0, 1, 0, 0, 0 );
        else
            if ( damage < 10 )
                SmartField( 210 + damage, 20, mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, mobheight[ mob[ i ][ 15 ]], 0, 0, 1, 0, 0, 0 );
            else
                SmartField( 210 + damage % 10, 20, mob[ i ][ 2 ] + 200, mob[ i ][ 3 ] + 000, mobheight[ mob[ i ][ 15 ]], 0, 0, 1, 0, 0, 0 );
                SmartField( 210 + damage / 10, 20, mob[ i ][ 2 ] + 100, mob[ i ][ 3 ] + 100, mobheight[ mob[ i ][ 15 ]], 0, 0, 1, 0, 0, 0 );
            end;
        end;
    end;
end


Function Int MobHit( int i, k )
private
    int damage;
begin
    mob[ i ][ 14 ] = mobatspd[ mob[ i ][ 15 ]];
    damage = rand( mobdmg0[ mob[ i ][ 15 ]], mobdmg1[ mob[ i ][ 15 ]] );
    if ( mob[ i ][ 24 ] > 0 )
        damage = damage * 2;
        mob[ i ][ 24 ]--;
    end;
    HurtMob( k, damage );
end


Function int ProcMobs()
private
    int i, k, ix, iy;
    int freex, freey;
    int mobsize;
begin
    hurtsound = 0;
    mobsize = 240;
    for ( i = 0; i < mobs; i++ )
        if ( mob[ i ][ 0 ] )
            if ( mob[ i ][ 14 ] > 0 )
                mob[ i ][ 14 ]--;
            end;
            if ( mob[ i ][ 13 ] > 0 )
                mob[ i ][ 13 ]--;
            else
                if ( i > 0 )
                    ProcMObAi( i );
                end;
            end;
            if ( i < mobs )
                for ( k = i + 1; k <= mobs; k++ )
                    if ( mob[ k ][ 0 ] )
                        if ( abs( mob[ k ][ 2 ] - mob[ i ][ 2 ] ) < 1000 )
                            if ( abs( mob[ k ][ 3 ] - mob[ i ][ 3 ] ) < 1000 )
                                ix = 10;
                                iy = 10;
                                if ( mobaiside[ mob[ i ][ 15 ]] != mobaiside[ mob[ k ][ 15 ]] )
                                    if ( mob[ i ][ 14 ] == 0 )
                                        MobHit( i, k );
                                    end;
                                    if ( mob[ k ][ 14 ] == 0 )
                                        MobHit( k, i );
                                    end;
                                end;
                                if ( mob[ i ][ 2 ] < mob[ k ][ 2 ] )
                                    ix = - ix;
                                end;
                                if ( mob[ i ][ 3 ] < mob[ k ][ 3 ] )
                                    iy = - iy;
                                end;
                                mob[ i ][ 6 ] += ix;
                                mob[ k ][ 6 ] -= ix;
                                mob[ i ][ 7 ] += iy;
                                mob[ k ][ 7 ] -= iy;
                            end;
                        end;
                    end;
                end;
            end;
            ix = mob[ i ][ 2 ] + mob[ i ][ 6 ];
            iy = mob[ i ][ 3 ] + mob[ i ][ 7 ];
            mob[ i ][ 4 ] = mob[ i ][ 2 ] / cellsize;
            mob[ i ][ 5 ] = mob[ i ][ 3 ] / cellsize;
            if ( abs( mob[ i ][ 6 ] ) > 30 )
                freex = 0;
                if ( mob[ i ][ 6 ] > 0 )
                    ix += mobsize;
                else
                    ix -= mobsize;
                end;
                if ( ix > 0 )
                    if ( ix < 127000 )
                        if (( mob[ i ][ 16 ] ) & ( mappass[ map[ ix / cellsize ][ ( mob[ i ][ 3 ] + mobsize ) / 1000 ]] ))
                            if (( mob[ i ][ 16 ] ) & ( mappass[ map[ ix / cellsize ][ ( mob[ i ][ 3 ] - mobsize ) / 1000 ]] ))
                                freex = 1;
                            end;
                        end;
                    end;
                end;
                if ( freex )
                    mob[ i ][ 2 ] += mob[ i ][ 6 ];
                else
                    // mob[i][2]=(mob[i][2]/1000)*1000+500;
                end;
                if ( mob[ i ][ 13 ] == 0 )
                    mob[ i ][ 6 ] = mob[ i ][ 6 ] * 7 / 8;
                end;
            end;
            if ( abs( mob[ i ][ 7 ] ) > 30 )
                freey = 0;
                if ( mob[ i ][ 7 ] > 0 )
                    iy += mobsize;
                else
                    iy -= mobsize;
                end;
                if ( iy > 0 )
                    if ( iy < 127000 )
                        if (( mob[ i ][ 16 ] ) & ( mappass[ map[ ( mob[ i ][ 2 ] + mobsize ) / cellsize ][ iy / cellsize ]] ))
                            if (( mob[ i ][ 16 ] ) & ( mappass[ map[ ( mob[ i ][ 2 ] - mobsize ) / cellsize ][ iy / cellsize ]] ))
                                freey = 1;
                            end;
                        end;
                    end;
                end;
                if ( freey )
                    mob[ i ][ 3 ] += mob[ i ][ 7 ];
                else
                    //mob[i][3]=(mob[i][3]/1000)*1000+500;
                end;
                if ( mob[ i ][ 13 ] == 0 )
                    mob[ i ][ 7 ] = mob[ i ][ 7 ] * 7 / 8;
                end;
            end;
        end;
    end;
end


Function int RenderMobs()
private
    int xx, yy, sx, sy;
    int i, c2, ii, kk, k;
begin
    c2 = count mod 2;
    for ( i = 0; i <= mobs; i++ )
        if ( mob[ i ][ 0 ] )
            ii = mob[ i ][ 15 ];
            xx = mob[ i ][ 2 ] - camerax;
            yy = mob[ i ][ 3 ] - cameray;
            sx = 160 + ( xx - yy ) * 30 / cellsize;
            sy = 120 + ( xx + yy ) * 20 / cellsize;
            mob[ i ][ 8 ] = sx;
            mob[ i ][ 9 ] = sy -20;
            if (( sx > -30 ) & ( sx < 350 ) & ( sy > -50 ) & ( sy < 320 ))
                zlDrawz( sx, sy - mobheight[ ii ], 10200 - sy * 10, mob[ i ][ 1 ] );
            end;
        end;
    end;
    //hp
    if (( incombat > 0 ) & ( showmobhp > 0 ))
        for ( i = 1; i <= mobs; i++ )
            if ( mob[ i ][ 0 ] )
                if ( mob[ i ][ 13 ] > 0 )
                    sx = mob[ i ][ 8 ];
                    sy = mob[ i ][ 9 ] + 20;
                    zlDraw( sx, sy, 188 );
                    k = ( 7 * mob[ i ][ 10 ] ) / mobhp[ mob[ i ][ 15 ]];
                    for ( kk = 0; kk < k; kk++ )
                        zlDraw( sx -20 + kk * 7, sy, 189 );
                    end;
                end;
            end;
        end;
    end;
    //hp
end
