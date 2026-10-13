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

function int gr_read_lib( int fp, int gen_aseprite )
begin
    char header[7];
    char * colors = NULL;
    int out_fp;
    int r, g, b;

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
        if ( gen_aseprite )
            out_fp = fopen( "output.pal", o_write );
            if ( out_fp )
                fputs( out_fp, "JASC-PAL\r\n0100\r\n256\r\n" );
                for ( int i = 0; i < 256; i++ )
                    r = asc( colors[i * 3] );
                    g = asc( colors[i * 3 + 1] );
                    b = asc( colors[i * 3 + 2] );
                    fputs( out_fp, r + " " + g + " " + b + "\r\n" );
                end
                fclose( out_fp );
                say( "Aseprite palette generated: output.pal" );
            end
        else
            for ( int i = 0; i < 256; i++ )
                say ( i + " =  0x" + substr(
                                            ((int *)
                                                ( ( (int)asc(colors[i*3]) << 24 ) +
                                                  ( (int)asc(colors[i*3+1]) << 16 ) +
                                                    ( (int)asc(colors[i*3+2]) << 8 ) +
                                                    255 ) ), -8 ) + " ( " + asc(colors[i*3]) + "," + asc(colors[i*3+1]) + "," + asc(colors[i*3+2]) + "," + 255 + " )" );
            end
        end
        mem_free( colors );
    end

    return 0;
end

/* --------------------------------------------------------------------------- */

process int main()
begin
    int gen_aseprite = 0;
    int arg_index = 1;

    if ( argc > 2 && argv[1] == "--aseprite" )
        gen_aseprite = 1;
        arg_index = 2;
    elif ( argc > 2 && argv[2] == "--aseprite" )
        gen_aseprite = 1;
    end

    say( argv[arg_index] );
    int f = fopen( argv[arg_index], o_zread );
    if ( f )
        gr_read_lib( f, gen_aseprite );
        fclose( f );
    else
        say( "Error opening input file" );
    end
end