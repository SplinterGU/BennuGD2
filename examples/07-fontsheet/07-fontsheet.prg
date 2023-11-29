import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

#include "fontsheet.inc"

global
    int err_msg;
    int font;
    int list;
    string fonts_names[256];
    int fonts_total = 0;
    int selected;
    string sample_text;
end

function int input_number(double x, double y)
private
    string number = "";
    int cursor = 0;
    int flash = 0;
    int k;
begin

    timer[0] = 0;

    while( !key(_ENTER) && !key(_ESC) )
        if ( k = keyboard.ascii )
            if ( err_msg )
                write_delete( err_msg );
                err_msg = 0;
            end
            if ( k >= asc('0') && k <= asc('9') )
                number += chr( k );
            end
            if ( k == 8 && len( number ) )
                number = substr( number, 0, len(number) - 1 );
            end
            while ( keyboard.ascii == k )
                frame;
            end
        end
        if ( cursor ) write_delete( cursor ); end
        cursor = write( 0, x, y, 0, number + ( flash ? "_" : "" ) );
        if ( timer[0] > 20 )
            flash ^= 1;
            timer[0] = 0;
        end
        frame;
    end

    if ( err_msg )
        write_delete( err_msg );
        err_msg = 0;
    end

    if ( cursor ) write_delete( cursor ); end

    if ( keyboard.ascii == 27 ) return -1; end

    return atoi( number );
end

process int main()
begin
    set_mode(1024,768);

    list = diropen( "res/font-*.png" );

    if ( !list ) say( "no fonts in current directory" ); return; end

    while( fonts_total < 256 && ( fonts_names[ fonts_total ] = dirread(list) ) != "" )
        fonts_total++;
    end
    dirclose(list);

    sort(fonts_names,fonts_total);

    while( 1 )
        write_delete(ALL_TEXT);

        write( 0, 0, 0, 0, "Available font bitmaps:" );

        int i;

        for ( i = 0; i < fonts_total; i++ )
            write( 0, 0, 20 + i * 10, 0, ( i + 1 ) + " - " + fonts_names[ i ] );
        end

        write( 0, 0, 30 + i * 10, 0, "Select a bitmap: " );

        int number;

        while ( 1 )
            number = input_number( 105, 30 + i * 10 );

            if ( key(_ESC ) ) return; end
            if ( number < 1 || number > fonts_total )
                err_msg = write( 0, 0, 50 + i * 10, 0, "ERROR: invalid input, try again!" );
            else
                break;
            end
            frame;
        end

        while(key(_ENTER))
            frame;
        end        

        write_delete(ALL_TEXT);

        font = fontsheet_load("res/" + fonts_names[ number - 1 ]);

        int ii;

        string text;
        int h = text_height(font, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

        write( font, 0, 0, 0, sample_text != "" ? sample_text : "Hello World!" );
        write( font, 0, h, 0, UTF8Toisolat1("áéíóúñÁÉÍÓÚÑ") );
        write( 0, 800, 0, 0, sample_text != "" ? sample_text : "Hello World!" );
        write( 0, 800, h, 0, UTF8Toisolat1("áéíóúñÁÉÍÓÚÑ") );

        for ( ii = 0; ii < 16; ii++ )
            text = "";
            for ( i = 0; i < 16; i++ )
                text += chr(i+16*ii);
            end
            write( font, 0, ( 5 + h ) * ( ii + 2 ), 0, text );
        end

        while(!key(_ENTER) && !key(_ESC))
            frame;
        end
        while(key(_ENTER) || key(_ESC))
            frame;
        end        
    end

end
