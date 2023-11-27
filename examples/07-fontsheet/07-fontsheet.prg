import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

global
    int err_msg;
    int font;
    int list;
    string fonts_names[256];
    int fonts_total = 0;
    int selected;
    string sample_text;
end

function string UTF8Toisolat1( string in )
begin
    byte c, d;
    int trailing, ix, l;
    string out;

    l = len(in);

    out = "";
    ix = 0;

    while ( ix < l )
        d = in[ix++];
        
        if   ( d < 0x80 ) c = d; trailing = 0;
        elif ( d < 0xC0 ) c = '?'; trailing = 0; /* trailing byte in leading position */
        elif ( d < 0xE0 ) c = d & 0x1F; trailing = 1;
        elif ( d < 0xF0 ) c = d & 0x0F; trailing = 2;
        elif ( d < 0xF8 ) c = d & 0x07; trailing = 3;
        else              c = d; trailing = 0; end
        
        for ( ; trailing; trailing-- )
            if ( ( ( d = in[ix++] ) & 0xC0 ) != 0x80 ) break; end
            c <<= 6;
            c |= d & 0x3F;
        end
        out += chr(c);
    end
    return out;
end


function string get_value(string data, string key )
begin
    int lenkey, vp, endv;
    key += "=";
    lenkey = len(key);
    vp = find(data,key);
    if ( vp != -1 )
        endv = find( data, 13, vp + lenkey ); // Search ENTER
        if ( endv != -1 )
            return substr( data, vp + lenkey , endv - ( vp + lenkey ) );
        else
            return substr( data, vp + lenkey );
        end
    end
    return "";
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

function int fontsheet_load(string folder, string filename)
begin
    int font;
    int w, h;
    int pw, ph, px, pe;

    string name;

    name = substr( filename, 0, find(filename, "_") );

    // get dimensions

    pw = find(filename, "_", 5 ) + 1;
    px = find(filename, "x", pw );
    pe = find(filename, ".", px + 1 );

    w = atoi(substr(filename,pw,px-pw));
    h = atoi(substr(filename,px+1, pe-(px+1)));

    int first, last;
    string info;
    string charmap, value;
    int charset;

    charset = CHARSET_ISO8859;
    charmap = "";
    first = 32;
    last = 128;
    sample_text = "";

    if ( fexists( folder + name + ".dat" ) )
        info = file( folder + name + ".dat" );

        // charmap
        if ( ( value = get_value( info, "charmap" ) ) != "" ) charmap = UTF8Toisolat1(value); end

        // first
        if ( ( value = get_value( info, "first" ) ) != "" ) first = atoi(value); end

        // last
        if ( ( value = get_value( info, "last" ) ) != "" ) last = atoi(value); end

        // sample_text
        if ( ( value = get_value( info, "sample_text" ) ) != "" ) sample_text = value; end

        // charset
        if ( ( value = get_value( info, "charset" ) ) != "" )
            switch( value )
                case "CHARSET_ISO8859", "ISO8859", "ISO8859-1", "ISOLATIN-1", "ISOLATIN", "LATIN-1", "WIN":
                    charset = CHARSET_ISO8859; 
                end
                case "CHARSET_CP850", "CP850", "ISO8859-1", "ISOLATIN-1", "ISOLATIN", "LATIN-1", "WIN":
                    charset = CHARSET_CP850; 
                end
            end
        end

    end

    int map = map_load( folder + filename );
    center_set(0,map,0,0);

    if ( charmap != "" )
        font = fnt_new(0,map,CHARSET_ISO8859,w,h,first,last,NFB_VARIABLEWIDTH,charmap);
    else
        font = fnt_new(0,map,CHARSET_ISO8859,w,h,first,last,NFB_VARIABLEWIDTH);//,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:?!-_~#\"'&()[]|`\\/@º+=*$ª€<>%");  // NFB_FIXEDWIDTH
    end

    return font;
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

        font = fontsheet_load("res/", fonts_names[ number - 1 ]);

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
