import "mod_misc";
import "mod_sound";


string  decoders[] =    "CMD",
                        "DRFLAC",
                        "DRMP3",
                        "FLAC",
                        "FLUIDSYNTH",
                        "MIDI",
                        "MOD",
                        "MODPLUG",
                        "MP3",
                        "MPG123",
                        "NATIVEMIDI",
                        "OGG",
                        "OPUS",
                        "TIMIDITY",
                        "WAVE",
                        "XMP";

begin
    int num_decoders = get_num_music_decoders();

    say( "Decodes (" + num_decoders + "): ");

    for ( int i = 0; i < num_decoders; i++ )
        say( get_music_decoder( i ) );
    end

    say( "" );

    for ( int i = 0; i < sizeof( decoders ) / sizeof( decoders[ 0 ] ); i++ )
        say( "Query has " + decoders[ i ] + ": " + ( has_music_decoder( decoders[ i ] ) ? "YES" : "NO") );
    end

end
