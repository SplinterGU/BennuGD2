import "mod_video";
import "mod_map";
import "mod_text";
import "mod_key";
import "mod_say";

private
    int font_id;
    int title_id;
begin
    set_mode( 960, 540 );

    font_id = ttf_load( "DejaVuSans.ttf" );
    if ( font_id < 0 )
        say( "TTF_LOAD failed" );
        exit();
    end

    say( "Family: " + ttf_get_family( font_id ) );
    say( "Style: " + ttf_get_style( font_id ) );

    ttf_set_size( font_id, 42 );
    title_id = write( font_id, 480, 220, ALIGN_CENTER, "BennuGD2 + FreeType" );

    ttf_set_size( font_id, 22 );
    write( font_id, 480, 300, ALIGN_CENTER, "Press ESC to exit" );

    while ( !key( _esc ) )
        frame;
    end

    delete_text( title_id );
    fnt_unload( font_id );
end
