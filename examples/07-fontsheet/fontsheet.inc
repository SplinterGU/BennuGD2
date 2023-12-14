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

function int fontsheet_load(string filename)
begin
    int font;

    string folder;

    int path_sep_pos = rfind( filename, "/" );
    if ( path_sep_pos == -1 )
        path_sep_pos = rfind( filename, "\\" );
    end

    if ( path_sep_pos > -1 )
        path_sep_pos++;
        folder = substr( filename, 0, path_sep_pos );
        filename = substr( filename, path_sep_pos );
    end

    string name = filename;
    int sep_pos = rfind( filename, "." );
    if ( sep_pos > -1 )
        name = substr( filename, 0, sep_pos );
    end

    int u_pos = rfind(name, "_");
    if ( u_pos == -1 )
        return 0;
    end
    
    name = substr( name, 0, u_pos );

    // get dimensions
    string dimensions = substr( filename, u_pos + 1 );

    int w = atoi(dimensions);
    int h = atoi(substr(dimensions, find(dimensions, "x") + 1));

    string value;
    
    int charset = CHARSET_ISO8859;
    string charmap = "";
    int first = 32;
    int last = 128;
    int widthtype = NFB_VARIABLEWIDTH;

    string line;

    int fp = fopen( folder + name + ".font", O_READ );
    if ( fp )
        int spriteID = -1;
        while ( !feof( fp ) )
            line = fgets( fp );
            string keyword = lcase( strtok( line, "=" ) );
            if ( keyword != "" )
                string value = strtok( "", "\r\n" );
                switch ( keyword )
                    case "charmap":
                        charmap = UTF8ToISOLat1(value);
                    end
                    case "first":
                        first = atoi(value);
                    end
                    case "last":
                        last = atoi(value);
                    end
                    case "widthtype":
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
                    case "charset":
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
            end
        end
        fclose( fp );
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
