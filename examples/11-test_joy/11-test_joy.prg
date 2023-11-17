import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

#define ANALOG_MIN  -32768
#define ANALOG_MAX  32767

#define SCRW    1000
#define SCRH    700

local
    int graph2;
end

int num_joys;

int buttons_proc[15][JOY_BUTTON_DPAD_RIGHT - JOY_BUTTON_A];
int trigger_proc[15][1];
int axis_proc[15][1];

byte rgb_table[15][2] = 
    255, 255, 255,
    255, 255,   0,
      0, 255,   0,
      0, 255, 255,
      0,   0, 255,
    255,   0, 255,
    255,   0,   0,
    128, 128, 128,
    192, 192, 192,
    255, 255, 128,
    128, 255, 128,
    128, 255, 255,
    128, 128, 255,
    255, 128, 255,
    255, 128, 128,
     64,  64,  64,
;

int coords[JOY_BUTTON_TOUCHPAD - JOY_BUTTON_A][1] =
    697,325,
    750,272,
    644,272,
    697,219,
    403,272,
    483,273,
    563,272,
    266,264,
    593,380,
    288,162,
    679,162,
    368,358,
    368,409,
    342,384,
    393,384,
;

string graphs[JOY_BUTTON_DPAD_RIGHT - JOY_BUTTON_A] =
    "res/circle-button.png",         // JOY_BUTTON_A
    "res/circle-button.png",         // JOY_BUTTON_B
    "res/circle-button.png",         // JOY_BUTTON_X
    "res/circle-button.png",         // JOY_BUTTON_Y
    "res/back-button.png",           // JOY_BUTTON_BACK
    "res/guide-button.png",          // JOY_BUTTON_GUIDE
    "res/start-button.png",          // JOY_BUTTON_START
    "res/circle-button-medium.png",  // JOY_BUTTON_LEFTSTICK
    "res/circle-button-medium.png",  // JOY_BUTTON_RIGHTSTICK
    "res/left_shoulder.png",         // JOY_BUTTON_LEFTSHOULDER
    "res/right_shoulder.png",        // JOY_BUTTON_RIGHTSHOULDER
    "res/dpad-up-button.png",        // JOY_BUTTON_DPAD_UP
    "res/dpad-down-button.png",      // JOY_BUTTON_DPAD_DOWN
    "res/dpad-left-button.png",      // JOY_BUTTON_DPAD_LEFT
    "res/dpad-right-button.png",     // JOY_BUTTON_DPAD_RIGHT
    ;

process joy_connected(int joy, double x, y )
begin
    graph = map_load( "res/led-connected.png" );
    color_r = rgb_table[joy][0];
    color_g = rgb_table[joy][1];
    color_b = rgb_table[joy][2];
    while (1)
        if ( joy_query( joy, JOY_QUERY_ATTACHED ) )
            alpha = 255;
        else
            alpha = 48;
        end
        frame;
    end
end

process actor(int joy, double x, y, int graph2)
begin
    color_r = rgb_table[joy][0];
    color_g = rgb_table[joy][1];
    color_b = rgb_table[joy][2];
    while (1)
        alpha = 128 + ( num_joys ? 127 / num_joys : 0 );
        frame;
    end
end

function double mapCoordinate(double ax, value, sz)
begin
    return ax + (value * sz) / (ANALOG_MAX - ANALOG_MIN);
end

process main()
begin
    set_mode( SCRW, SCRH );

    int i, ii;

    z = 1;

    graph = map_load("res/xbox-joy.png");

    x = SCRW/2;
    y = SCRH/2;

    for ( i = 0; i < 16; i++ )
        for ( ii = 0; ii <= JOY_BUTTON_DPAD_RIGHT - JOY_BUTTON_A; ii++ )
            buttons_proc[i][ii] = actor(i, coords[ii][0],coords[ii][1], map_load(graphs[ii]));
        end
        trigger_proc[i][0] = actor(i, 262, 147, map_load("res/triggerleft-axis.png"));
        trigger_proc[i][1] = actor(i, 703, 147, map_load("res/triggerright-axis.png"));

        axis_proc[i][0] = actor(i, 266, 264, map_load("res/circle-button-small.png")); axis_proc[i][0].graph = axis_proc[i][0].graph2;
        axis_proc[i][1] = actor(i, 593, 377, map_load("res/circle-button-small.png")); axis_proc[i][1].graph = axis_proc[i][1].graph2;
    end

    for ( i = 0; i < 16; i++ )
        joy_connected(i, 110 + i * 15, 15 );
    end


    while( !key(_ESC) ) 
        num_joys = joy_numjoysticks();

        write( 0, 10, 15, ALIGN_CENTER_LEFT, "CONNECTED " + num_joys);
        write( 0, 10, 35, ALIGN_CENTER_LEFT, "FIRST CONNECTED " + joy_query( JOY_QUERY_FIRST_ATTACHED ) );

        for ( i = 0; i < 16; i++ )
            if ( joy_is_attached(i) )

                if ( joy_query(i, JOY_AXIS_TRIGGERRIGHT ) || joy_query(i, JOY_AXIS_TRIGGERLEFT) )
                    joy_set(i, joy_set_rumble, 1000, 10000, 100 );
                end

                for( ii = JOY_BUTTON_A; ii <= JOY_BUTTON_TOUCHPAD; ii++ )
                    int cxx = ii - JOY_BUTTON_A;
                    if ( joy_query( i, ii ) )
                        buttons_proc[i][cxx].graph = buttons_proc[i][cxx].graph2;
                    else
                        buttons_proc[i][cxx].graph = 0;
                    end
                 end

                for( ii = JOY_AXIS_LEFTX; ii <= JOY_AXIS_TRIGGERRIGHT; ii++ )
                    int value = joy_getaxis(i, ii);
                    switch ( ii )
                        case JOY_AXIS_TRIGGERLEFT:
                            if ( value > 16384 )
                                trigger_proc[i][0].graph = trigger_proc[i][0].graph2;
                            else
                                trigger_proc[i][0].graph = 0;
                            end
                        end
                    
                        case JOY_AXIS_TRIGGERRIGHT:
                            if ( value > 16384 )
                                trigger_proc[i][1].graph = trigger_proc[i][1].graph2;
                            else
                                trigger_proc[i][1].graph = 0;
                            end
                        end

                        case JOY_AXIS_LEFTX:
                            axis_proc[i][0].x = mapCoordinate(266, value, 35);
                        end

                        case JOY_AXIS_LEFTY:
                            axis_proc[i][0].y = mapCoordinate(264, value, 35);
                        end

                        case JOY_AXIS_RIGHTX:
                            axis_proc[i][1].x = mapCoordinate(593, value, 35);
                        end

                        case JOY_AXIS_RIGHTY:
                            axis_proc[i][1].y = mapCoordinate(380, value, 35);
                        end
                    end
                end
                axis_proc[i][0].graph = axis_proc[i][0].graph2;
                axis_proc[i][1].graph = axis_proc[i][1].graph2;
            else
                axis_proc[i][0].graph = 0;
                axis_proc[i][1].graph = 0;
            end
        end
        frame;
        write_delete(all_text);
    end

    let_me_alone();

end
