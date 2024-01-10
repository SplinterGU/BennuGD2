

Function Int UseSkill( int ii, skill )
Private
    int k;
Begin
    if ( skill > 0 )
        psound( rand( 8, 11 ));
    end;
    if ( skill == 1 ) //HP Potion
        HealMob( ii, 25 );
    elseif ( skill == 2 ) //Knife
        spawnbullet( 0, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 3 ) //Armor
        if ( mob[ ii ][ 23 ] < 1 )
            mob[ ii ][ 23 ] = 10;
        end;
    elseif ( skill == 4 ) //Arrow
        spawnbullet( 1, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 5 ) //Food
        HealMob( ii, 10 );
    elseif ( skill == 6 ) //Sword
        spawnbullet( 2, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 7 ) //Shield
        if ( mob[ ii ][ 23 ] < 1 )
            mob[ ii ][ 23 ] = 1;
        end;
    elseif ( skill == 8 ) //Bandage
        HealMob( ii, 5 );
    elseif ( skill == 9 ) //Berzerk
        mob[ ii ][ 24 ] = 1;
    elseif ( skill == 10 ) //Great Slash
        spawnbullet( 2, mob[ ii ][ 2 ], mob[ ii ][ 3 ], ( mob[ ii ][ 11 ] + 7 ) % 8, ii );
        spawnbullet( 2, mob[ ii ][ 2 ], mob[ ii ][ 3 ], ( mob[ ii ][ 11 ] + 0 ), ii );
        spawnbullet( 2, mob[ ii ][ 2 ], mob[ ii ][ 3 ], ( mob[ ii ][ 11 ] + 1 ) % 8, ii );
    elseif ( skill == 11 ) //Magic Missle
        spawnbullet( 3, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 12 ) //Fire Bolt
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 13 ) //Lighting
        spawnbullet( 6, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 14 ) //Fire Ball
        spawnbullet( 5, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 15 ) //Firebolt ring
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 0, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 1, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 2, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 3, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 4, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 5, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 6, ii );
        spawnbullet( 4, mob[ ii ][ 2 ], mob[ ii ][ 3 ], 7, ii );
    elseif ( skill == 16 ) //Healing
        HealMob( ii, 15 );
    elseif ( skill == 17 ) //Turn Undead
        for ( k = 1; k < mobs; k++ )
            if ( mob[ k ][ 0 ] > 0 )
                if ( mobaiside[ mob[ k ][ 15 ]] == 02 )
                    HurtMob( k, rand( 0, 5 ));
                end;
            end;
        end;
    elseif ( skill == 18 ) //Holy Bolt
        spawnbullet( 7, mob[ ii ][ 2 ], mob[ ii ][ 3 ], mob[ ii ][ 11 ], ii );
    elseif ( skill == 19 ) //Holy Armor
        if ( mob[ ii ][ 23 ] < 5 )
            mob[ ii ][ 23 ] = 5;
        end;
    elseif ( skill == 20 ) //Charm :)
        HealMob( ii, 50 );
    end;
    //spawnbullet(6,mob[ii][2],mob[ii][3],mob[ii][11],ii);
End
