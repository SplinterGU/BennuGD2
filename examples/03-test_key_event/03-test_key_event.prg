import "mod_gfx";
import "mod_input";

begin
    set_mode(320,200);

    int counter1, counter2, counter3;

    write( 0, 160, 10, ALIGN_CENTER, "Press SPACE" );

    write( 0, 150, 40, ALIGN_CENTER_RIGHT, "key( _SPACE )"      );  write_var( 0, 160, 40, ALIGN_CENTER_LEFT, counter1 );
    write( 0, 150, 50, ALIGN_CENTER_RIGHT, "key_down( _SPACE )" );  write_var( 0, 160, 50, ALIGN_CENTER_LEFT, counter2 );
    write( 0, 150, 60, ALIGN_CENTER_RIGHT, "key_up( _SPACE )"   );  write_var( 0, 160, 60, ALIGN_CENTER_LEFT, counter3 );

    while ( !key(_ESC) )
        if ( key     ( _SPACE ) ) counter1++; end
        if ( key_down( _SPACE ) ) counter2++; end
        if ( key_up  ( _SPACE ) ) counter3++; end
        frame;
    end

end
