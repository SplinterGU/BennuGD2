import "libmod_gfx";
import "libmod_input";
import "libmod_misc";
import "libmod_debug";

#define SRC_WIDTH   1280
#define SRC_HEIGHT  720

/*
video credits
Nature Stock Videos by Vecteezy (https://www.vecteezy.com/free-videos/nature Nature Stock Videos by Vecteezy)
https://www.vecteezy.com/video/10791112-beautiful-erawan-waterfall-in-the-tropical-rain-forest-erawan-national-park-kanchanaburi-thailand
Sound Effect from <a href="https://pixabay.com/?utm_source=link-attribution&utm_medium=referral&utm_campaign=music&utm_content=24060">Pixabay</a>
*/

global
    int font;
    int mh;
    int video_graph;
end

function string gettimestr( int t )
begin
    string ret;

    int hour, mins, secs;

    hour = t / 3600000;
    mins = ( t - hour * 3600000 ) / 60000;
    secs = ( t - hour * 3600000 - mins * 60000 ) / 1000;

    return hour + ":" + substr( "0" + mins, -2, 2 ) + ":" + substr( "0" + secs, -2, 2 );

end

process display_volume()
public
    int run = 1;
begin
    display_volume pid;

    if ( ( pid = get_id( type display_volume ) ) )
        pid.run = 0;
    end

    z = -1;

    x = 10;
    y = 10;

    string str;

    int t;
    t = get_timer() + 2000;

    while( run && t > get_timer())
        if ( media_get_mute( mh ) )
            str = "MUTE";
        else
            str = "VOL " + media_get_volume( mh ) ;
        end

        text.color_r = 0;
        text.color_b = 0;

        int w = text_width( font, str );
        int h = text_height( font, str );

        graph = map_new( w + 20, h + 20 );
        map_clear( 0, graph, rgba( 0, 0, 0, 128 ));
        center_set( 0, graph, 0, 0 );

        write_in_map( 0, graph, font, 10, 10, str, ALIGN_TOP_LEFT );

        frame;
        map_del( 0, graph );
    end
end

process display_time( )
public
    int run = 1;
begin
    display_time pid;

    if ( ( pid = get_id( type display_time ) ) )
        pid.run = 0;
        return;
    end

    z = -1;
    y = 10;

    string time_str;

    while( run )
        time_str = gettimestr( media_get_time( mh ) );

        text.color_r = 0;
        text.color_b = 0;

        int w = text_width( font, time_str );
        int h = text_height( font, time_str );

        graph = map_new( w + 20, h + 20 );
        map_clear( 0, graph, rgba( 0, 0, 0, 128 ));
        center_set( 0, graph, 0, 0 );

        write_in_map( 0, graph, font, 10, 10, time_str, ALIGN_TOP_LEFT );

        x = SRC_WIDTH - w - 20 - 10;
        frame;
        map_del( 0, graph );
    end
end

process video( double x, y )
begin
    z = 0;
    int s = 0;
//    alpha = 0;
    while(1)
        graph = video_graph;
        s = clamp( s + 10, 0, SRC_WIDTH );
        alpha = clamp( alpha + 5, 0, 255 );
        size = remap( 100.0, 0, map_info(0, graph, g_width), 0, s );
        frame;
    end
end

process main()
begin
    set_mode(SRC_WIDTH,SRC_HEIGHT,mode_frameless);

    int fontmap;

    fontmap = map_load("osd1.png");
    font = fnt_new(0,fontmap,CHARSET_ISO8859,24,28,0,0,NFB_FIXEDWIDTH,"./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^-" );

    set_fps(60,0);

    mh = media_load( "sample.ogg", &video_graph, 0 );

    map_clear(0,video_graph, rgba(255,255,255,0));

    video( SRC_WIDTH / 2, SRC_HEIGHT / 2 );

    media_play( mh );

    int t, v;

    while(!key(_ESC))
        // Pause/Resume
        if ( keydown( _SPACE ) )
            media_pause( mh, media_get_status( mh ) == MEDIA_STATUS_PAUSED ? 0 : 1 );
        end

        // Stop video
        if ( keydown( _S ) )
            media_stop( mh );
        end

        // Play video
        if ( keydown( _P ) )
            media_play( mh );
        end

        // Unload video
        if ( keydown( _U ) )
            media_unload( mh );
            mh = 0;
        end

        // Reload video
        if ( keydown( _ENTER ) )
            if ( media_get_status( mh ) != MEDIA_STATUS_ERROR )
                media_unload( mh );
            end
            mh = media_load( "sample.ogg", &video_graph, 0 );
            media_play( mh );
        end

        // Mute
        if ( keydown( _M ) )
            media_set_mute( mh, media_get_mute( mh ) ^ 1 );
            display_volume( );
        end

        // Vol-
        if ( key( _DOWN ) )
            v = media_get_volume( mh ) - 1;
            if ( v < 1 ) v = 0; end
            media_set_volume( mh, v );
            media_set_mute( mh, 0 );
            display_volume();
            timer[0] = 0;
            while( key( _DOWN ) and timer[0] < 10 ) frame; end
        end

        // Vol+
        if ( key( _UP ) )
            v = media_get_volume( mh ) + 1;
            if ( v > 128 ) v = 128; end
            media_set_volume( mh, v );
            media_set_mute( mh, 0 );
            display_volume();
            timer[0] = 0;
            while( key( _UP ) and timer[0] < 10 ) frame; end
        end

        // Show Time
        if ( keydown( _T ) )
            display_time();
        end

        // Auto-restart video
        if ( media_get_status( mh ) == MEDIA_STATUS_ENDED )
            media_play( mh );
        end

        frame;
    end

    media_stop( mh );

    let_me_alone();

end
