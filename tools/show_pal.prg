/* CABECERAS DE FICHEROS */

import "mod_misc";

#define MAP_MAGIC "map\x1A\x0D\x0A\x00"
#define M32_MAGIC "m32\x1A\x0D\x0A\x00"
#define M16_MAGIC "m16\x1A\x0D\x0A\x00"
#define M01_MAGIC "m01\x1A\x0D\x0A\x00"

#define PAL_MAGIC "pal\x1A\x0D\x0A\x00"

#define FNT_MAGIC "fnt\x1A\x0D\x0A\x00"
#define FNX_MAGIC "fnx\x1A\x0D\x0A\x00"

#define FPG_MAGIC "fpg\x1A\x0D\x0A\x00"
#define F32_MAGIC "f32\x1A\x0D\x0A\x00"
#define F16_MAGIC "f16\x1A\x0D\x0A\x00"
#define F01_MAGIC "f01\x1A\x0D\x0A\x00"

struct MAP_HEADER
     uint8 magic[6] ;
     uint8 version ;
     uint16 width ;
     uint16 height ;
     uint32 code ;
     int8 name[31] ;
end;

/* --------------------------------------------------------------------------- */
#if 0
struct {
    int     code;
    int     regsize;
    char    name[32];
    char    fpname[12];
    int     width;
    int     height;
    int     flags;
} chunk;
#endif
/* --------------------------------------------------------------------------- */
#if 0
typedef struct _chardata {
    int width;
    int height;
    int xadvance;
    int yadvance;
    int xoffset;
    int yoffset;
    int fileoffset;
} _chardata;
#endif
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------- */

function char * gr_read_pal( int * fp )
begin
    uint8 * colors = mem_alloc( 768 );
    int i;

    if ( !fread( colors, 768, fp ) ) return NULL; end

    for ( i = 0; i < 768; i++ ) colors[i] <<= 2; end

    return ( char * ) colors;
end

/* --------------------------------------------------------------------------- */

function char * gr_read_pal_with_gamma( int * fp )
begin
    char * colors = gr_read_pal( fp );

    /* Skip gamma definition */
    if ( colors ) fseek( fp, 576, SEEK_CUR ); end

    return colors;
end

/* --------------------------------------------------------------------------- */

/* Static convenience function */
function gr_read_lib( int fp )
begin
    char header[7];
    short px, py;
    int bpp, ii;
    uint32 y;
    char * colors = NULL;
    int st = 0;

    if ( fread( &header[0], sizeof( header ), fp ) != sizeof( header ) )
        say( "error reading" );
        return -1;
    end

      if ( header == FPG_MAGIC || header == PAL_MAGIC || header == FNT_MAGIC ) 
    elif ( header == MAP_MAGIC )
        fseek( fp, 48, SEEK_SET );
    else
        say( "input file not supported");
        return -1;
    end

    if ( !( colors = gr_read_pal_with_gamma( fp ) ) )
        return -1;
    end

    if ( colors )
        for ( int i = 0; i < 256; i++ )
            say ( i + " =  0x" + substr(
                                        ((int *)
                                            ( ( (int)asc(colors[i*3]) << 24 ) +
                                              ( (int)asc(colors[i*3+1]) << 16 ) +
                                                ( (int)asc(colors[i*3+2]) << 8 ) +
                                                255 ) ), -8 ) + " ( " + asc(colors[i*3]) + "," + asc(colors[i*3+1]) + "," + asc(colors[i*3+2]) + "," + 255 + " )" );
        end
        mem_free( colors );
    end


#if 0
    uint32_t rmask, gmask, bmask, amask;
    getRGBA_mask( bpp, &rmask, &gmask, &bmask, &amask );

    while ( !file_eof( fp ) ) {
        if ( file_read( fp, &chunk, sizeof( chunk ) ) != sizeof( chunk ) ) break; end

        ARRANGE_DWORD( &chunk.code );
        if ( chunk.code < 0 || chunk.code > 999 ) break; end
        ARRANGE_DWORD( &chunk.regsize );
        ARRANGE_DWORD( &chunk.width );
        ARRANGE_DWORD( &chunk.height );
        ARRANGE_DWORD( &chunk.flags );

        /* Graph header */

        SDL_Surface* surface = SDL_CreateRGBSurface(0, chunk.width, chunk.height, bpp, rmask, gmask, bmask, amask );
        if ( !surface ) {
            if ( pal ) SDL_FreePalette( pal );
            grlib_destroy( libid );
            return -1;
        }

        if ( pal ) SDL_SetSurfacePalette( surface, pal );

        // Set transparent color
        if ( bpp != 32 ) {
            if ( bpp == 1 ) SDL_SetColorKey( surface, SDL_TRUE, 1 );
            else            SDL_SetColorKey( surface, SDL_TRUE, 0 );
        }

        int ncpoints = chunk.flags;
        CPOINT * cpoints = NULL;

        if ( ncpoints )
            cpoints = ( CPOINT * ) malloc( ncpoints * sizeof( CPOINT ) );
            if ( !cpoints )
                return -1;
            end

            for ( int c = 0; c < ncpoints; c++ )
                fread( fp, px );
                fread( fp, py );
                if ( px == -1 && py == -1 )
                    cpoints[c].x = CPOINT_UNDEFINED;
                    cpoints[c].y = CPOINT_UNDEFINED;
                else
                    cpoints[c].x = px;
                    cpoints[c].y = py;
                end
            end
        end

        /* Graphic data */

        int widthb = chunk.width * bpp / 8;
        if (( widthb * 8 / bpp ) < chunk.width ) widthb++;

        for ( y = 0; y < chunk.height; y++ ) {
            uint8_t * line = ( uint8_t * ) surface->pixels + surface->pitch * y;

            switch ( bpp ) {
                case    32:
                    st = file_readUint32A( fp, ( uint32_t * ) line, chunk.width );
                    break;

                case    16:
                    st = file_readUint16A( fp, ( uint16_t * ) line, chunk.width );
                    break;

                case    8:
                    st = file_read( fp, line, widthb );
                    break;

                case    1:
                    st = file_read( fp, line, widthb );
                    for ( ii = 0; ii < widthb; ii++ ) line[ii] = ~line[ii];
                    break;

            }

            if ( !st ) {
                free( cpoints );
                SDL_FreeSurface( surface );
                if ( pal ) SDL_FreePalette( pal );
                grlib_destroy( libid );
                return -1;
            }
        }

        GRAPH *gr = bitmap_new( chunk.code, 0, 0, surface );
        if ( !gr ) {
            free( cpoints );
            SDL_FreeSurface( surface );
            if ( pal ) SDL_FreePalette( pal );
            grlib_destroy( libid );
            return -1;
        }
        SDL_FreeSurface( surface );

        memcpy( gr->name, chunk.name, sizeof( chunk.name ) );
        gr->name[31] = 0;
        gr->ncpoints = ncpoints;
        gr->cpoints = cpoints;

        grlib_add_map( libid, gr );
    }
#endif
    return 0;
end



process int main()
begin
say(argv[1]);
    int f = fopen(argv[1], o_read);
    gr_read_lib( f );
    fclose( f );

end
