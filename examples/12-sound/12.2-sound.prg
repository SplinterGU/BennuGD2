import "mod_gfx";
import "mod_sound";
import "mod_input";
import "mod_misc";

#include "fontsheet.inc"
#include "sprsheet.inc"

#define SCR_W   1024
#define SCR_H   600

// --- Globals ---
int font;
int font_big;

__SPRSHEET_LIB * buttons_lib = NULL;

int master_vol = 128;
int music_vol = 128;
int sfx_vol = 128;

int current_music = 0;
string * music_files;
int total_tracks = 0;
int current_track = 0;
int music_handle = 0;

int is_muted = 0;

int sfx_handle[3]; // [0..2]
string sfx_file_0, sfx_file_1, sfx_file_2;

string playing_msg = "Select a track";
string track_title = "";
string track_artist = "";
string track_album = "";
string track_copyright = "";
string progress_str = "--:-- / --:--";

int repeat_mode = 0; // 0 = no repeat, 1 = repeat all

string tooltip_msg = ""; // Tooltip to show on mouse hover

int active_sfx_channels[256]; // Active SFX channels
int active_sfx_count = 0;

// --- UI Auto-hide ---
int inactive_time = 0;
int ui_hide_timeout = 3000; // Milliseconds of inactivity before hiding UI (3 seconds)
int last_mouse_activity = 0; // Timestamp of last mouse activity
int last_mouse_x = -1;
int last_mouse_y = -1;
int ui_alpha = 255; // Current UI alpha (0-255)
int ui_visible = 1; // 1 = visible, 0 = hidden
int tooltip_timeout = 100;

// --- Scrolling text when UI hidden ---
string playing = "NO MUSIC LOADED";
int playing_text_x = SCR_W / 2;
int playing_text_width = 0;

// --- Helpers ---

function text_set_alpha(int tid, int alpha)
begin
    byte r, g, b;
    write_get_rgba(tid, &r, &g, &b, NULL);
    write_set_rgba(tid, r, g, b, alpha);
end

function string time_to_hms(int time_seconds)
begin
    if (time_seconds < 0) time_seconds = 0; end
    int hours = time_seconds / 3600;
    int remaining_seconds = time_seconds % 3600;
    int minutes = remaining_seconds / 60;
    int seconds = remaining_seconds % 60;

    if (hours > 0)
        return hours + ":" + substr("00" + minutes, -2) + ":" + substr("00" + seconds, -2);
    else
        return minutes + ":" + substr("00" + seconds, -2);
    end
end

function string * get_music_list( string pattern )
begin
    string * files = string_new_array();
    int count = 0;
    glob("");
    string filename = glob(pattern);
    while( filename != "" )
        if ( !fileinfo.directory )
            int dot_pos = rfind( filename, "." );
            if ( dot_pos > -1 )
                switch ( lcase( substr( filename, dot_pos + 1 ) ) )
                    case "opus", "mod", "s3m", "wav", "mp3", "xm", "flac", "ogg", "mid":
                        string_resize_array( &files, count + 1 );
                        files[ count ] = filename;
                        count++;
                    end
                end
            end
        end
        filename = glob(pattern);
    end
    return files;
end

// --- UI Processes ---

// Values for button actions
#define ACT_PREV        1
#define ACT_REWIND      2
#define ACT_PLAY        3
#define ACT_FORWARD     4
#define ACT_NEXT        5
#define ACT_STOP        6
#define ACT_PAUSE       7
#define ACT_POWER       8
#define ACT_FADEIN      11
#define ACT_FADEOUT     12
#define ACT_MUTE        13

process button(int gid, double posX, double posY, int action, string tooltip, int gid2 = 0, int action2 = 0, string tooltip2 = "", int curr_gid = 0)
private
    int hovered = 0;
    int pressed = 0;

public
    int _curr_gid = 0;
    int _gid = 0;
    int _gid2 = 0;

begin

    _curr_gid = clamp(curr_gid, 0, 1);

    if (!gid2) _curr_gid = 0; end

    _gid = gid;
    _gid2 = gid2;

    sprsheet_set_sprite( buttons_lib, _curr_gid ? gid2 : gid );

    z = -10;
    x = posX;
    y = posY;

    while(1)
        hovered = 0;
        if ( collision( type mouse ) )
            hovered = 1;
            tooltip_msg = _curr_gid ? tooltip2 : tooltip;
            if ( mouse.left )
                if ( !pressed )
                    pressed = 1;
                    trigger_action(_curr_gid ? action2 : action);
                    if ( gid2 )
                        _curr_gid ^= 1;
                        sprsheet_set_sprite( buttons_lib, _curr_gid ? gid2 : gid );
                    end
                end
                size = 90;
            else
                pressed = 0;
                size = 110;
            end
        else
            size = 100;
        end

        // Mute button color
        if ( action == ACT_MUTE && is_muted )
            color_r = 255; color_g = 50; color_b = 50;
        else
            if ( hovered )
                color_r = 255; color_g = 255; color_b = 200;
            else
                color_r = 255; color_g = 255; color_b = 255;
            end
        end

        // Apply UI alpha
        alpha = ui_alpha;

        frame;
    end
end

button button_play_id = 0;

function trigger_action(int action)
begin
    switch(action)
        case ACT_PREV:
            current_track = wrap(current_track - 1, 0, total_tracks - 1);
        end
        case ACT_REWIND:
            if ( music_handle )
                double pos = music_get_playback_position(music_handle);
                double dur = music_get_duration(music_handle);
                pos = pos - 10.0;
                if ( pos < 0.0 ) pos = 0.0; end
                music_set_playback_position(pos);
            end
        end
        case ACT_PLAY:
            if ( music_handle )
                music_pause();
                music_resume();
                if (!music_is_playing())
                    music_play(music_handle, 0);
                end
            end
        end
        case ACT_FORWARD:
            if ( music_handle )
                double pos = music_get_playback_position(music_handle);
                double dur = music_get_duration(music_handle);
                pos = pos + 10.0;
                if ( pos > dur ) pos = dur; end
                music_set_playback_position(pos);
            end
        end
        case ACT_NEXT:
            current_track = wrap(current_track + 1, 0, total_tracks - 1);
        end
        case ACT_STOP:
            music_stop();
        end
        case ACT_PAUSE:
            if ( music_handle )
                music_pause();
            end
        end
        case ACT_POWER:
            let_me_alone();
        end
        case ACT_FADEIN:
            if ( music_handle ) music_fade_in(music_handle, 0, 3000); end
        end
        case ACT_FADEOUT:
            music_fade_off(3000);
        end
        case ACT_MUTE:
            is_muted ^= 1;
            if ( is_muted )
                music_set_volume(0);
            else
                music_set_volume(music_vol);
            end
        end
    end
end

// Process to handle autoplay with sleep/wakeup
process play_controller()
private
    int last_track = -1;
begin
    while(1)
        // Handle Track change
        if ( current_track != last_track )
            if ( music_handle ) music_unload(music_handle); end
            music_handle = music_load("res/songs/" + music_files[current_track]);
            music_play(music_handle, 0);
            music_set_volume(music_vol);
            last_track = current_track;

            playing_msg = "Track " + (current_track + 1) + " / " + total_tracks;
            track_title = music_get_title_tag(); track_title = "Title: " + ( track_title == "" ? music_files[current_track] : music_get_title_tag());
            track_artist = music_get_artist_tag(); track_artist = "Artist: " + ( track_artist == "" ? "N/A" : music_get_artist_tag());
            track_album = music_get_album_tag(); track_album = "Album: " + ( track_album == "" ? "N/A" : track_album );
            track_copyright = music_get_copyright_tag(); track_copyright = "Copyright: " + ( track_copyright == "" ? "N/A" : track_copyright );

            // Build scrolling text
            playing = "Playing: [" + music_files[current_track] + "]";
            string str = music_get_title_tag(); if ( str != "" ) playing += " - Title: " + str ; end
            str = music_get_artist_tag(); if ( str != "" ) playing += " - Artist: " + str; end
            str = music_get_album_tag(); if ( str != "" ) playing += " - Album: " + str ; end
            str = music_get_copyright_tag(); if ( str != "" ) playing += " - Copyright: " + str; end
            playing_text_width = text_width(font, playing) / 2;
            playing_text_x = SCR_W + playing_text_width;

        end

        // Autoplay: advance to next track when finished
        if ( music_handle && !music_is_playing() )
            double dur = music_get_duration(music_handle);
            double pos = music_get_playback_position(music_handle);
            if ( dur > 0 && pos >= dur - 0.5 )
                current_track = wrap(current_track + 1, 0, total_tracks - 1);
            end
        end

        if (music_is_playing())
            if (music_is_paused())
                button_play_id._curr_gid = 0;
            else
                button_play_id._curr_gid = 1;
            end
        else
            button_play_id._curr_gid = 0;
        end
        sprsheet_set_sprite( buttons_lib, button_play_id._curr_gid ? button_play_id._gid2 : button_play_id._gid, button_play_id );

        frame;
    end
end

process volume_slider(double posX, double posY, int * val, string label)
private
    int bar_w = 200;
    int text_id = 0;
begin
    text_id = write(font, (int)posX, (int)(posY - 15), ALIGN_CENTER_LEFT, label);
    x = posX;
    y = posY;

    while(1)
        if ( mouse.left && mouse.x >= x && mouse.x <= x + bar_w && mouse.y >= y - 5 && mouse.y <= y + 15 )
            *val = remap(mouse.x - x, 0, bar_w, 0, 128);
            if ( *val < 0 ) *val = 0; end
            if ( *val > 128 ) *val = 128; end

            // Apply volume immediately
            if ( label == "MASTER" ) set_master_volume(*val); end
            if ( label == "MUSIC" ) music_set_volume(*val); is_muted = 0; end
        end

        // Apply UI alpha to text
        text_set_alpha(text_id, ui_alpha);

        frame;
    end
end

process sfx_tester(double posX, double posY)
private
    int area_w = 300;
    int area_h = 200;
    int mouse_was_pressed = 0;
    int text_id_1 = 0;
    int text_id_2 = 0;
begin
    text_id_1 = write(font, (int)(posX + area_w / 2), (int)(posY - 20), ALIGN_CENTER, "SFX TEST ZONE (Click to Play)");
    text_id_2 = write(font, (int)(posX + 20), (int)(posY + area_h + 10), 0, "X: Panning | Y: Distance/Vol");
    x = posX;
    y = posY;

    while(1)
        // Apply UI alpha to texts
        text_set_alpha(text_id_1, ui_alpha);
        text_set_alpha(text_id_2, ui_alpha);
        if ( mouse.x >= x && mouse.x <= x + area_w && mouse.y >= y && mouse.y <= y + area_h )
            tooltip_msg = "Click to test SFX with panning and distance";

            if ( mouse.left && !mouse_was_pressed )
                mouse_was_pressed = 1;

                // Relative position
                int rel_x = mouse.x - x;
                int rel_y = mouse.y - y;

                // Panning: 0 to 255 (left to right)
                int pan = remap(rel_x, 0, area_w, 0, 255);
                // Distance: 0 to 255
                int dist = remap(rel_y, 0, area_h, 0, 255);

                int ch = sound_play(sfx_handle[rand(0, 2)], 0);
                if ( ch != -1 )
                    set_panning(ch, 255 - pan, pan);
                    set_distance(ch, dist);
                    channel_set_volume(ch, sfx_vol);

                    // Save active channel
                    if ( active_sfx_count < 256 )
                        active_sfx_channels[active_sfx_count] = ch;
                        active_sfx_count++;
                    end
                end
            end

            if ( !mouse.left )
                mouse_was_pressed = 0;
            end
        end
        frame;
    end
end


process tooltip()
begin
    priority = father.priority - 1;

    while(1)
        // Visual tooltip with box
        if ( tooltip_msg != "" && ui_alpha > 0 && inactive_time > tooltip_timeout)
            if (graph) map_unload(0, graph); end

            int tooltip_w = text_width(font, tooltip_msg) + 10;
            int tooltip_h = text_height(font, tooltip_msg) + 10;
            x = mouse.x;
            y = mouse.y - tooltip_h;

            graph = map_new(tooltip_w, tooltip_h);

            center_set(0, graph, 0, 0);

            // Adjust if it goes off screen
            if ( x + tooltip_w > SCR_W ) x = mouse.x - tooltip_w - 5; end
            if ( y + tooltip_h > SCR_H ) y = mouse.y - tooltip_h - 5; end

            // Tooltip background
            drawing_map(0, graph);
            drawing_rgba(0, 0, 128, clamp((200 * ui_alpha) / 255, 0, 200));
            draw_rectangle_round_filled(0, 0, tooltip_w, tooltip_h, 5);
            drawing_map(0, 0);

            // Tooltip text
            write_in_map(0, graph, font, tooltip_w / 2, tooltip_h / 2, tooltip_msg, ALIGN_CENTER);

            z = mouse.z - 1;
            alpha = ui_alpha;

        else
            if (graph) map_unload(0, graph); end
            graph = 0;
        end

        frame;
    end
end


// Dedicated UI drawer to avoid artifacts
process UI_Drawer()
private
    int draw_sfx_zone = 0;
    int draw_slider_bg[3];
    int draw_slider_fill[3];
    int draw_progress_bg = 0;
    int draw_progress_fill = 0;
begin
    priority = -100; // Draw last

    tooltip();

    while(1)
        // SFX Zone
        if ( draw_sfx_zone ) draw_delete(draw_sfx_zone); end
        drawing_rgba(50, 50, 80, ui_alpha);
        drawing_z(100);
        draw_sfx_zone = draw_rectangle_round_filled(50, 50, 300, 200, 10);

        // Sliders background and fill
        int sx = SCR_W - 250;
        int sy = 50;
        for ( int s = 0; s < 3; s++ )
            int cur_y = sy + s * 50;

            // Delete old draws
            if ( draw_slider_bg[s] ) draw_delete(draw_slider_bg[s]); end
            if ( draw_slider_fill[s] ) draw_delete(draw_slider_fill[s]); end

            // Background
            drawing_rgba(100, 100, 100, ui_alpha);
            drawing_z(100);
            draw_slider_bg[s] = draw_rectangle_filled(sx, cur_y, 200, 10);

            // Fill
            int val = 0;
            if ( s == 0 ) val = master_vol; end
            if ( s == 1 ) val = music_vol; end
            if ( s == 2 ) val = sfx_vol; end

            int cur_x = remap(val, 0, 128, 0, 200);
            if (s == 1 && is_muted)
                drawing_rgba(200, 50, 50, ui_alpha);
            else
                drawing_rgba(200, 200, 50, ui_alpha);
            end
            drawing_z(99);
            draw_slider_fill[s] = draw_rectangle_filled(sx, cur_y, cur_x, 10);
        end

        // Update SFX volume at runtime for all active channels
        for ( int i = 0; i < active_sfx_count; i++ )
            if ( sound_is_playing(active_sfx_channels[i]) )
                channel_set_volume(active_sfx_channels[i], sfx_vol);
            end
        end

        // Progress bar
        double dur = music_get_duration(music_handle);
        double pos = music_get_playback_position(music_handle);
        if ( dur > 0 )
            // Delete old draws
            if ( draw_progress_bg ) draw_delete(draw_progress_bg); end
            if ( draw_progress_fill ) draw_delete(draw_progress_fill); end

            // Background
            drawing_rgba(40, 40, 40, ui_alpha);
            drawing_z(100);
            draw_progress_bg = draw_rectangle_filled(100, 480, SCR_W - 200, 10);

            // Fill
            int bar_fill = (int)remap(pos, 0, dur, 0, SCR_W - 200);
            drawing_rgba(0, 200, 255, ui_alpha);
            drawing_z(99);
            draw_progress_fill = draw_rectangle_filled(100, 480, bar_fill, 10);
        end

        frame;
    end
end

#define gNumActors 30

int idActors[gNumActors];
byte color[gNumActors][3];
G_POINTF actorsCoords[gNumActors];
double actorsRadiusFrom[gNumActors];
double actorsRadiusTo[gNumActors];

function effect()
begin
    for ( int i = 0; i < gNumActors; i++ )
        if ( music_is_playing() && !music_is_paused() && music_vol > 0 )
            if ( actorsRadiusFrom[ i ] == actorsRadiusTo[ i ] )
                actorsCoords[ i ].x = rand(0,SCR_W);
                actorsCoords[ i ].y = rand(0,SCR_H);

                actorsRadiusFrom[ i ] = rand( 1, 96 );
                actorsRadiusTo[ i ] = rand( 1, 96 );

                color[ i ][ 0 ] = rand( 0, 255 );
                color[ i ][ 1 ] = rand( 0, 255 );
                color[ i ][ 2 ] = rand( 0, 255 );
                color[ i ][ 3 ] = 0;
            end
        end

        if ( idActors[ i ] ) draw_delete( idActors[ i ] ); idActors[ i ] = 0; end

        if ( actorsRadiusFrom[ i ] != actorsRadiusTo[ i ] )
            color[ i ][ 3 ] = clamp(color[ i ][ 3 ] + 1, 0, 128);

            actorsRadiusFrom[ i ] = towards( actorsRadiusFrom[ i ], actorsRadiusTo[ i ], 0.5 );

            drawing_z(300);
            drawing_rgba( color[ i ][ 0 ], color[ i ][ 1 ], color[ i ][ 2 ], color[ i ][ 3 ] );
            idActors[ i ] = draw_circle_filled( actorsCoords[ i ].x, actorsCoords[ i ].y, actorsRadiusFrom[ i ] );
        end
    end
end

// --- Main ---

begin
    set_fps(60, 0);
    set_mode(SCR_W, SCR_H);

    priority = 100;

    font = fontsheet_load("res/font/font-22_12x16.png");
    font_big = fontsheet_load("res/font/font-07_31x34.png");
    buttons_lib = sprsheet_load("res/gfx/buttons.png");

    // Create mouse cursor map
    mouse.graph = map_new(16, 16, 32);
    drawing_map(0, mouse.graph);
    drawing_rgba(255, 255, 255, 255);
    draw_line(0, 0, 15, 8);
    draw_line(0, 0, 8, 15);
    draw_line(8, 15, 15, 8);
    // Fill it a bit
    for ( int i = 1; i < 8; i++ )
        draw_line(0, 0, 15-i, 8+i);
    end
    drawing_map(0, 0);
    center_set(0, mouse.graph, 0, 0);

    sfx_file_0 = "res/fx/footstep-sfx-6.mp3";
    sfx_file_1 = "res/fx/human-sfx-1.mp3";
    sfx_file_2 = "res/fx/doors-sfx-2.mp3";

    // Load SFX
    sfx_handle[0] = sound_load(sfx_file_0);
    sfx_handle[1] = sound_load(sfx_file_1);
    sfx_handle[2] = sound_load(sfx_file_2);

    music_files = get_music_list("res/songs/*");
    total_tracks = string_size_array(music_files);

    // UI Elements - Main buttons centered
    double bx = (double)(SCR_W / 2 - (3.5 * 60)); // Center 8 buttons
    double by = (double)(SCR_H - 60);

    button(1, bx + 0*60, by, ACT_PREV, "Previous Track");
    button(2, bx + 1*60, by, ACT_REWIND, "Rewind 10s");
    button_play_id = button(3, bx + 2*60, by, ACT_PLAY, "Play", 7, ACT_PAUSE, "Pause", 1);
    button(4, bx + 3*60, by, ACT_FORWARD, "Forward 10s");
    button(5, bx + 4*60, by, ACT_NEXT, "Next Track");
    button(6, bx + 5*60, by, ACT_STOP, "Stop");
    button(9, bx + 6*60, by, ACT_MUTE, "Mute");
    button(10, bx + 7*60, by, ACT_POWER, "Exit");

    // Fade buttons (same Y as main buttons)
    button(11, bx - 3*60, by, ACT_FADEIN, "Fade In 3s");
    button(12, bx - 2*60, by, ACT_FADEOUT, "Fade Out 3s");

    volume_slider((double)(SCR_W - 250), 50.0, &master_vol, "MASTER");
    volume_slider((double)(SCR_W - 250), 100.0, &music_vol, "MUSIC");
    volume_slider((double)(SCR_W - 250), 150.0, &sfx_vol, "SFX");
    sfx_tester(50.0, 50.0);
    UI_Drawer();

    int text_id_playing = write_var(font, SCR_W / 2, 300, ALIGN_CENTER, playing_msg);
    int text_id_title = write_var(font, SCR_W / 2, 340, ALIGN_CENTER, track_title);
    int text_id_artist = write_var(font, SCR_W / 2, 360, ALIGN_CENTER, track_artist);
    int text_id_album = write_var(font, SCR_W / 2, 380, ALIGN_CENTER, track_album);
    int text_id_copyright = write_var(font, SCR_W / 2, 400, ALIGN_CENTER, track_copyright);
    int text_id_progress = write_var(font, SCR_W / 2, 450, ALIGN_CENTER, progress_str);

    // Scrolling text (shown when UI is hidden)
    int text_id_scrolling = write_var(font_big, SCR_W / 2, SCR_H / 2, ALIGN_CENTER, playing);

    // Start play controller with autoplay
    play_controller();

    // Initialize activity timestamp
    last_mouse_activity = get_timer();
    last_mouse_x = mouse.x;
    last_mouse_y = mouse.y;

    while(!key(_ESC))
        tooltip_msg = ""; // Reset tooltip each frame

        // Detect mouse activity (movement or clicks)
        if ( mouse.x != last_mouse_x || mouse.y != last_mouse_y || mouse.left || mouse.right )
            last_mouse_activity = get_timer();
            last_mouse_x = mouse.x;
            last_mouse_y = mouse.y;
        end

        // Calculate inactivity time
        inactive_time = get_timer() - last_mouse_activity;

        // Fade UI in/out based on inactivity
        if ( inactive_time > ui_hide_timeout )
            // Fade out
            if ( ui_alpha > 0 )
                ui_alpha = clamp(ui_alpha - 2, 0, 255);
                if ( ui_alpha == 0 ) ui_visible = 0; end
            end
        else
            // Fade in
            if ( ui_alpha < 255 )
                ui_alpha = clamp(ui_alpha + 3, 0, 255);
                ui_visible = 1;
            end
        end

        // Apply alpha to all texts
        text_set_alpha(text_id_playing, ui_alpha);
        text_set_alpha(text_id_title, ui_alpha);
        text_set_alpha(text_id_artist, ui_alpha);
        text_set_alpha(text_id_album, ui_alpha);
        text_set_alpha(text_id_copyright, ui_alpha);
        text_set_alpha(text_id_progress, ui_alpha);

        // Apply alpha to mouse cursor and hide it if outside the screen
        if ( !wm_info.mouse_status )
            mouse.alpha = 0; // Hide cursor outside screen
        else
            mouse.alpha = ui_alpha; // Apply UI fade
        end

        // Scrolling text animation (inverse alpha to UI)
        if ( music_is_playing() && !music_is_paused() )
            playing_text_x = wrap( playing_text_x - 1, -playing_text_width, SCR_W + playing_text_width );
        else
            if ( !music_handle )
                playing_text_x = SCR_W / 2;
                playing = "NO MUSIC LOADED";
                playing_text_width = text_width(font, playing) / 2;
            else
                // Music is loaded but stopped/paused, keep text centered
                playing_text_x = SCR_W / 2;
            end
        end

        write_move( text_id_scrolling, playing_text_x, SCR_H / 2 );
        // Inverse alpha: visible when UI is hidden
        text_set_alpha(text_id_scrolling, 255 - ui_alpha);

        // Update progress string
        double dur = music_get_duration(music_handle);
        double pos = music_get_playback_position(music_handle);
        if ( dur > 0 )
            progress_str = time_to_hms((int)pos) + " / " + time_to_hms((int)dur);

            // Seek on click
            if ( mouse.left && mouse.y >= 470 && mouse.y <= 500 && mouse.x >= 100 && mouse.x <= SCR_W - 100 )
                double seek_target = remap(mouse.x - 100, 0, SCR_W - 200, 0, dur);
                music_set_playback_position(seek_target);
            end
        end

        effect();

        frame;
    end

    let_me_alone();
end
