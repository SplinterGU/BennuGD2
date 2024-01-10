

Function Int SyncCamera()
begin
    camerax = tcamerax;
    cameray = tcameray;
end


function Int PlayerCamera()
private
    int i, xx, yy, nn;
begin
    incombat = 0;
    nn = 3;
    xx = mob[ 0 ][ 2 ] * nn;
    yy = mob[ 0 ][ 3 ] * nn;
    for ( i = 1; i <= mobs; i++ )
        if ( mob[ i ][ 0 ] )
            if ( abs( mob[ i ][ 2 ] - mob[ 0 ][ 2 ] ) < 3000 )
                if ( abs( mob[ i ][ 3 ] - mob[ 0 ][ 3 ] ) < 3000 )
                    incombat = 1;
                    xx += mob[ i ][ 2 ];
                    yy += mob[ i ][ 3 ];
                    nn++;
                end;
            end;
        end;
    end;
    tcamerax = xx / nn -1000;
    tcameray = yy / nn -1000;
end


Function Int ProcCamera()
begin
    camerax = camerax + ( tcamerax - camerax ) / 10;
    cameray = cameray + ( tcameray - cameray ) / 10;
    PlayerCamera();
end
