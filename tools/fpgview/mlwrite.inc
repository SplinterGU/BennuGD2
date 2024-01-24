#ifndef __MLWRITE
#define __MLWRITE

/* -------------------------------------------------------- */

type oltext_id
    int ids[8];
end

/* -------------------------------------------------------- */

#define olwrite(tid,bcolor,fcolor,font,x,y,align,str)  \
                                            rgba_get(bcolor,&text.color_r,&text.color_g,&text.color_b,&text.alpha); \
                                            (tid).ids[0] = write( font, x - 1, y    , align, str ); \
                                            (tid).ids[1] = write( font, x + 1, y    , align, str ); \
                                            (tid).ids[2] = write( font, x    , y + 1, align, str ); \
                                            (tid).ids[3] = write( font, x    , y - 1, align, str ); \
                                            (tid).ids[4] = write( font, x - 1, y + 1, align, str ); \
                                            (tid).ids[5] = write( font, x - 1, y - 1, align, str ); \
                                            (tid).ids[6] = write( font, x + 1, y + 1, align, str ); \
                                            (tid).ids[7] = write( font, x + 1, y - 1, align, str ); \
                                            text.z--; \
                                            rgba_get(fcolor,&text.color_r,&text.color_g,&text.color_b,&text.alpha); \
                                            (tid).ids[8] = write( font, x    , y    , align, str ); \
                                            text.z++;

#define olwrite_var(tid,bcolor,fcolor,font,x,y,align,var)  \
                                            rgba_get(bcolor,&text.color_r,&text.color_g,&text.color_b,&text.alpha); \
                                            (tid).ids[0] = write_var( font, x - 1, y    , align, var ); \
                                            (tid).ids[1] = write_var( font, x + 1, y    , align, var ); \
                                            (tid).ids[2] = write_var( font, x    , y + 1, align, var ); \
                                            (tid).ids[3] = write_var( font, x    , y - 1, align, var ); \
                                            (tid).ids[4] = write_var( font, x - 1, y + 1, align, var ); \
                                            (tid).ids[5] = write_var( font, x - 1, y - 1, align, var ); \
                                            (tid).ids[6] = write_var( font, x + 1, y + 1, align, var ); \
                                            (tid).ids[7] = write_var( font, x + 1, y - 1, align, var ); \
                                            text.z--; \
                                            rgba_get(fcolor,&text.color_r,&text.color_g,&text.color_b,&text.alpha); \
                                            (tid).ids[8] = write_var( font, x    , y    , align, var ); \
                                            text.z++;

/* -------------------------------------------------------- */

function oltext_delete( oltext_id * tid )
private
    int i;
begin
    for ( i = 0; i < 9; i++ )
        if ( tid.ids[ i ] )
            write_delete( tid.ids[ i ] );
        end
    end
end

/* -------------------------------------------------------- */

function mlwrite_delete( oltext_id ** array, int n )
private
    int i;
begin
    if ( array && *array )
        for( i = 0; i < n; i++ )
            oltext_delete( (*array)[ i ] );
        end
        if ( *array ) free( *array ); *array = NULL; end
    end
end

/* -------------------------------------------------------- */

function int mlwrite( bcolor, fcolor, font, double x, double y, int align, string txt, oltext_id ** ids )
private
    string str = "";
    int spos = 0, epos = 0;
    int count = 0;
    int i;
    int lasth = 0;
begin
    *ids = NULL;
    while( ( epos = find( txt, "|", spos ) ) > 0 )
        str = substr( txt, spos, epos - spos );
        if ( str != "" )
            *ids = realloc( *ids, sizeof( oltext_id ) * ( count + 1 ) );
             olwrite( (*ids)[count], bcolor, fcolor, font, x, y, align, str );
             count++;
             lasth = text_height( font, str );
        end
        y += lasth + 2;
        spos = epos + 1;
    end
    return count;
end

/* -------------------------------------------------------- */

#endif
