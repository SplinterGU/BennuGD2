import "mod_gfx"
import "mod_input"
import "mod_sound"
import "mod_misc";

private
    int font_id;
    int title_id;
begin
    set_mode( 960, 540 );

    font_id = ttf_load( "japanese.otf" );
	
    if ( font_id < 0 )
        say( "TTF_LOAD failed" );
        exit();
    end

    say( "Family: " + ttf_get_family( font_id ) );
    say( "Style: " + ttf_get_style( font_id ) );

    ttf_set_size( font_id, 42 );
	title_id = write( font_id, 480, 80, ALIGN_CENTER, "BennuGD2 + FreeType2" );
#pragma no_decode_utf8_strings
    title_id = write( font_id, 480, 120, ALIGN_CENTER, "ひらがな Hiragana, カタカナ Katakana, 漢字 Kanji, ローマ字 Rōmaji" );
	title_id = write( 0, 480, 140, ALIGN_CENTER, "ひらがな Hiragana, カタカナ Katakana, 漢字 Kanji, ローマ字 Rōmaji" );
#pragma decode_utf8_strings
	title_id = write( font_id, 480, 180, ALIGN_CENTER, "ひらがな Hiragana, カタカナ Katakana, 漢字 Kanji, ローマ字 Rōmaji" );
	title_id = write( 0, 480, 200, ALIGN_CENTER, "ひらがな Hiragana, カタカナ Katakana, 漢字 Kanji, ローマ字 Rōmaji" );

    ttf_set_size( font_id, 22 );
    write( font_id, 480, 300, ALIGN_CENTER, "Press ESC to exit" );

    while ( !key( _esc ) )
        frame;
    end

    write_delete( title_id );
    fnt_unload( font_id );
end
