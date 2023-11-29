#ifndef __FONTSHEET_H
#define __FONTSHEET_H

function string UTF8ToISOLat1( string in )
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
        int endv = find( data, chr(13), vp + lenkey ); // Search ENTER
        int endv1 = find( data, chr(10), vp + lenkey ); // Search LF
        if ( endv == -1 )
            endv = endv1;
        else
            if ( endv1 < endv && endv1 != -1 ) endv = endv1; end
        end
        if ( endv != -1 )
            return substr( data, vp + lenkey , endv - ( vp + lenkey ) );
        else
            return substr( data, vp + lenkey );
        end
    end
    return "";
end

function int fontsheet_load(string filename)
begin
    int font;

    string folder;
    string filename_reverse = strrev( filename );

    int dot_pos = find( filename_reverse, "/" );
    if ( dot_pos == -1 )
        dot_pos = find( filename_reverse, "\\" );
    end

    if ( dot_pos > -1 )
        folder = strrev( substr( filename_reverse, dot_pos ) );
        filename = strrev( substr( filename_reverse, 0, dot_pos ) );
    end

    string name = substr( filename, 0, find(filename, "_") );

    // get dimensions

    int pw = find(filename, "_", 5 ) + 1;
    int px = find(filename, "x", pw );
    int pe = find(filename, ".", px + 1 );

    int w = atoi(substr(filename,pw,px-pw));
    int h = atoi(substr(filename,px+1, pe-(px+1)));

    string value;
    
    int charset = CHARSET_ISO8859;
    string charmap = "";
    int first = 32;
    int last = 128;
    int widthtype = NFB_VARIABLEWIDTH;

    if ( fexists( folder + name + ".dat" ) )
        string info = file( folder + name + ".dat" );

        // charmap
        if ( ( value = get_value( info, "charmap" ) ) != "" ) charmap = UTF8ToISOLat1(value); end

        // first
        if ( ( value = get_value( info, "first" ) ) != "" ) first = atoi(value); end

        // last
        if ( ( value = get_value( info, "last" ) ) != "" ) last = atoi(value); end

        // width type
        if ( ( value = get_value( info, "widthtype" ) ) != "" ) 
            switch( ucase( value ) )
                case "VARIABLE":
                    widthtype = NFB_VARIABLEWIDTH; 
                end
                case "FIXED":
                    widthtype = NFB_FIXEDWIDTH; 
                end
                case "FIXED_CENTER":
                    widthtype = NFB_FIXEDWIDTHCENTER; 
                end
            end
        end

        // charset
        if ( ( value = get_value( info, "charset" ) ) != "" )
            switch( ucase( value ) )
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
        font = fnt_new(0,map,CHARSET_ISO8859,w,h,first,last,widthtype,charmap);
    else
        font = fnt_new(0,map,CHARSET_ISO8859,w,h,first,last,widthtype);//,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:?!-_~#\"'&()[]|`\\/@º+=*$ª€<>%");  // NFB_FIXEDWIDTH
    end

    return font;
end

#endif
