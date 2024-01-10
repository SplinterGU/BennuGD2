
global
end


function int ClearFields()
private
    int i;
begin
    for ( i = 0; i < 64; i++ )
        field_c[ i ] = 0;
    end;
end


Function int SimpleField( int index, xx, yy, zz )
private
    int i;
begin
    i = 0;
    while (( i < 64 ) and( field_c[ i ] > 0 ))
        i++;
    end;
    if ( i == 64 )
        i = rand( 0, 63 );
    end;
    field_c[ i ] = 1;
    field_g[ i ] = index;
    field_x[ i ] = xx;
    field_y[ i ] = yy;
    field_z[ i ] = zz;
    field_ix[ i ] = 0;
    field_iy[ i ] = 0;
    field_iz[ i ] = 0;
    field_iix[ i ] = 0;
    field_iiy[ i ] = 0;
    field_iiz[ i ] = 0;
end


Function int SmartField( int index, life, xx, yy, zz, ix, iy, iz, iix, iiy, iiz )
private
    int i;
begin
    i = 0;
    while (( i < 64 ) and( field_c[ i ] > 0 ))
        i++;
    end;
    if ( i == 64 )
        i = rand( 0, 63 );
    end;
    field_c[ i ] = life;
    field_g[ i ] = index;
    field_x[ i ] = xx;
    field_y[ i ] = yy;
    field_z[ i ] = zz;
    field_ix[ i ] = ix;
    field_iy[ i ] = iy;
    field_iz[ i ] = iz;
    field_iix[ i ] = iix;
    field_iiy[ i ] = iiy;
    field_iiz[ i ] = iiz;
end


function int RenderFields()
private
    int i, xx, yy, sx, sy;
begin
    for ( i = 0; i < 64; i++ )
        if ( field_c[ i ] > 0 )
            xx = field_x[ i ] - camerax;
            yy = field_y[ i ] - cameray;
            sx = 160 + ( xx - yy ) * 30 / cellsize;
            sy = 120 + ( xx + yy ) * 20 / cellsize;
            if (( sx > -30 ) & ( sx < 350 ) & ( sy > -50 ) & ( sy < 320 ))
                zlDrawz( sx, sy - field_z[ i ], 10200 - sy * 10, field_g[ i ] );
            end;
            field_x[ i ] += field_ix[ i ];
            field_y[ i ] += field_iy[ i ];
            field_z[ i ] += field_iz[ i ];
            field_ix[ i ] += field_iix[ i ];
            field_iy[ i ] += field_iiy[ i ];
            field_iz[ i ] += field_iiz[ i ];
            field_c[ i ]--;
        end;
    end;
end
