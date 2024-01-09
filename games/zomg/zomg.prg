program Wiz_Project;

import "mod_gfx";
import "mod_input";
import "mod_sound";
import "mod_misc";

#define init_bgd1_background_emulation() background.file = 0; background.graph = map_new(320,240)
#define put_screen(f,g) map_clear(0, background.graph ); map_put(0, background.graph, f, g, 160, 120)
#define clear_screen() map_clear(0, background.graph )
#define put( f, g, x, y ) map_put(0, background.graph, f, g, x, y)
#define map_xput(fdst,gdst,gsrc,x,y,angle,size,flags) map_put(fdst, gdst, fdst, gsrc, x, y, angle, size, size, flags, 255, 255, 255, 255)

#include "jkeys.lib"

declare process Final_boss( double x, y );
declare process zombie( double x, y, int zombie_type, AI, speed, starting_way );
declare process player( double x, y );
declare process shot( double x, y, int z, shot_type, tangle, dmg );
declare process zombie_shot( double x, y, int z, shot_type, tangle, dmg );
declare process morter_shot( double x, y, int z, shot_type, tangle, dmg );
declare process gilza( double x, y, int tangle, speed, gilza_type );
declare process blood( double x, y, size );
declare process after_flame( double x, y, size );
declare process muzzle( double x, y, size, int z, angle, dir );
declare process death_menu_gfx( double x, y, int z, double size, int alpha, graph );
declare process fake_flame_scroll( double x, y, int tangle, speed );
declare process letters( double x, y, int graph, double size );

const
    maximum_number_of_zombies_on_screen = 100;

Global
    int player_file;
    int items_drop_sequal;
    int items_droped;
    int item_min_type;
    int item_max_type;
    int first_sound;
    int credits;
    int refresh_volume;
    int weapon1_sound;
    int weapon2_sound;
    int weapon3_sound;
    int weapon4_sound;
    int weapon5_sound;
    int weapon6_sound;
    int weapon7_sound;
    int weapon8_sound;
    int weapon9_sound;
    int weapon10_sound;
    int weapon11_sound;
    int zombie1_sound;
    int zombie2_sound;
    int zombie3_sound;
    int zombie4_sound;
    int zombie5_sound;
    int zombie6_sound;
    int zombie7_sound;
    int zombie8_sound;
    int zombie9_sound;
    int zombie10_sound;
    int zombie11_sound;
    int zombie12_sound;
    int zombie13_sound;
    int zombie14_sound;
    int empty_clip_sound;
    int reload_sound;
    int explode1_sound;
    int explode2_sound;
    int explode3_sound;
    //hit1_sound;hit2_sound;
    int scream1_sound;
    int scream2_sound;
    int gothit1_sound;
    int gothit2_sound;
    int gothit3_sound;
    int line1_sound;
    int line2_sound;
    int line3_sound;
    int line4_sound;
    int line5_sound;
    int line6_sound;
    int line7_sound;
    int line8_sound;
    int line9_sound;
    int fire_sound;
    int grass1_sound;
    int grass2_sound;
    int grass3_sound;
    int grass4_sound;
    int finalboss_sound;
    int music1;
    int intro_sound;
    int menu_sound;
    int hit_sound; //not a sound
    int zombie_die;
    int punch_line_sound;
    int got_hit_sound;
    int grass_walking_sound;
    int grass_walking_time;
    int player_speed = 4;
    int player_hp = 100;
    int attack_player;
    int player_aim;
    int weapon_aim_raise;
    int weapon_aim_decrease;
    int weapon_aim_max;
    int max_items_on_screen;
    int items_on_screen;
    int background_map;
    int background_map_c;
    int background_map_c2;
    int mouse_x;
    int mouse_y;
    int blend_shadow;
    int flame_fired;
    int flame_time;
    int fire_sound_wav;
    int pause_screen;
    int player_id;
    int god_mode;
    int ammo;
    int morter_x;
    int morter_y;
    int morter_dmg;
    int morter_tim;
    int boom_x;
    int boom_y;
    int boom_dmg;
    int boom_tim;
    int level = 1;
    int level_start = 0;
    int level_time;
    int level_end_time;
    int number_of_zombies;
    int max_number_of_zombies;
    int max_number_of_zombies_screen;
    int zombies_killed;
    int zombies_created;
    int blooddrain_time;
    int death_delay;
    //play_sound_weapon;
    int weapons_volume = 128 / 4;
    int zombie_sound;
    int zombie_sound_time;
    int collision_matrix[ 60 ][ 45 ][ 2 ]; //0=amount,1=id1,2=id2;
    int health_bar;
    int level_complete;
    int boss_arrive;
    int final_boss_hp = 1986;
    int gas_attack;
    int intro_game;
    int intro_screen;
    int intro_screen_time;
    int end_of_game_time;

    int disabled_mouse = 0;

local
    int move;
    int anim;
    int i;
    int i2;
    int i3;
    int i4;
    int move_x;
    int move_y;
    int ix;
    int iy;
    int dir;
    int anim_dir;
    int I_got_hit;
    int last_x;
    int last_y;
    int id1;
    int id2;
    int id3;
    int id4;
    int refresh_time;
    int m_last_x;
    int m_last_y;
    int zombie_number;
    int weapon;
    int weapon_mode;
    int attack;
    int mouse_click;
    int weapon_reload;
    int weapon_dmg;
    int weapon_ammo;
    int reload_time;
    int weapon_reload_time;
    int per_shot_time;
    int shot_time;
    int graph1;
    int graph2;
    double graph1_x;
    double graph1_y;
    double graph2_x;
    double graph2_y;
    int angle1;
    int angle2;
    int tangle;
    int hp;
    int dmg;
    int on_fire;
    int on_fire_time;
    int insert_dir;
    int weapon_refresh;
    int new_weapon;

private
    double axis2_x, axis2_y;
    g_pointf oldmouse;

begin
    ctype = c_scroll;
    set_fps( 30, 0 );
    rand_seed( time());

    screen.scale_resolution = 06400480;

    set_mode( 320, 240, mode_modal );

    init_bgd1_background_emulation();

    player_file = fpg_load( 'fpg/player.fpg' );

    weapon1_sound = sound_load( 'sounds/weapon1.wav' );
    weapon2_sound = sound_load( 'sounds/weapon2.wav' );
    weapon3_sound = sound_load( 'sounds/weapon3.wav' );
    weapon4_sound = sound_load( 'sounds/weapon4.wav' );
    weapon5_sound = sound_load( 'sounds/weapon5.wav' );
    weapon6_sound = sound_load( 'sounds/weapon6.wav' );
    weapon7_sound = sound_load( 'sounds/weapon7.wav' );
    weapon8_sound = sound_load( 'sounds/weapon8.wav' );
    weapon11_sound = sound_load( 'sounds/weapon11.wav' );
    fire_sound = sound_load( 'sounds/fire.wav' );
    finalboss_sound = sound_load( 'sounds/finalboss.wav' );
    music1 = music_load( 'sounds/alien_opera.mp3' );
    intro_sound = sound_load( 'sounds/intro.wav' );
    menu_sound = sound_load( 'sounds/menu.wav' );
    scream1_sound = sound_load( 'sounds/scream1.wav' );
    scream2_sound = sound_load( 'sounds/scream2.wav' );
    explode1_sound = sound_load( 'sounds/explode1.wav' );
    explode2_sound = sound_load( 'sounds/explode2.wav' );
    explode3_sound = sound_load( 'sounds/explode3.wav' );
    empty_clip_sound = sound_load( 'sounds/empty.wav' );
    reload_sound = sound_load( 'sounds/reload.wav' );
    //hit1_sound=sound_load('sounds/hit1.wav');
    //hit2_sound=sound_load('sounds/hit2.wav');
    zombie1_sound = sound_load( 'sounds/1.wav' );
    zombie2_sound = sound_load( 'sounds/2.wav' );
    zombie3_sound = sound_load( 'sounds/3.wav' );
    zombie4_sound = sound_load( 'sounds/4.wav' );
    zombie5_sound = sound_load( 'sounds/5.wav' );
    zombie6_sound = sound_load( 'sounds/6.wav' );
    zombie7_sound = sound_load( 'sounds/7.wav' );
    zombie8_sound = sound_load( 'sounds/8.wav' );
    zombie9_sound = sound_load( 'sounds/9.wav' );
    zombie10_sound = sound_load( 'sounds/10.wav' );
    zombie11_sound = sound_load( 'sounds/11.wav' );
    zombie12_sound = sound_load( 'sounds/12.wav' );
    zombie13_sound = sound_load( 'sounds/13.wav' );
    zombie14_sound = sound_load( 'sounds/14.wav' );
    gothit1_sound = sound_load( 'sounds/gothit1.wav' );
    gothit2_sound = sound_load( 'sounds/gothit2.wav' );
    gothit3_sound = sound_load( 'sounds/gothit3.wav' );
    line1_sound = sound_load( 'sounds/l1.wav' );
    line2_sound = sound_load( 'sounds/l2.wav' );
    line3_sound = sound_load( 'sounds/l3.wav' );
    line4_sound = sound_load( 'sounds/l4.wav' );
    line5_sound = sound_load( 'sounds/l5.wav' );
    line6_sound = sound_load( 'sounds/l6.wav' );
    line7_sound = sound_load( 'sounds/l7.wav' );
    line8_sound = sound_load( 'sounds/l8.wav' );
    line9_sound = sound_load( 'sounds/l9.wav' );
    grass1_sound = sound_load( 'sounds/grass1.wav' );
    grass2_sound = sound_load( 'sounds/grass2.wav' );
    grass3_sound = sound_load( 'sounds/grass3.wav' );
    grass4_sound = sound_load( 'sounds/grass4.wav' );
    jkeys_set_default_keys();
    jkeys_controller();
    background_map = map_new( 400, 300 );
    background_map_c = map_new( 400, 300 );
    background_map_c2 = map_new( 400, 300 );
    graph = background_map_c;

    x = 200;
    y = 150;
    from ix = 0 to 30;
        from iy = 0 to 22;
            map_put( 0, background_map, 0, rand( 805, 808 ), 8 + ix * 16, 8 + iy * 16 );
        end
    end

    temp_gfx( x, y, z + 1, flags, size, 255, 0, background_map_c2, angle ); //background blood
    id1 = temp_gfx( mouse_x -4 - player_aim, mouse_y -4 - player_aim, -500, 0, 100, 255, player_file, 20, 0 ); //mouse pointer
    id2 = temp_gfx( mouse_x + 4 + player_aim, mouse_y -4 - player_aim, -500, 1, 100, 255, player_file, 20, 0 );
    id3 = temp_gfx( mouse_x -4 - player_aim, mouse_y + 4 + player_aim, -500, 2, 100, 255, player_file, 20, 0 );
    id4 = temp_gfx( mouse_x + 4 + player_aim, mouse_y + 4 + player_aim, -500, 3, 100, 255, player_file, 20, 0 );
    //level=12;
    volume_refresher();
    first_sound = sound_play( menu_sound, -1, 2 );
    loop
        if ( refresh_volume == 0 and jkeys_state[ _JKEY_VOLUP ] and weapons_volume < 128 ) weapons_volume += 16;
            refresh_volume = 1;
        end
        if ( refresh_volume == 0 and jkeys_state[ _JKEY_VOLDOWN ] and weapons_volume > 0 ) weapons_volume -= 16;
            refresh_volume = 1;
        end
        if ( refresh_volume == 1 )
            volume_refresher();
            refresh_volume = 2;
        end
        if ( refresh_volume == 2 and !jkeys_state[ _JKEY_VOLUP ] and !jkeys_state[ _JKEY_VOLDOWN ] )
            refresh_volume = 0;
        end
        IF ( level_start == 0 )
            if ( intro_screen == 0 )
                if ( intro_screen_time == 0 )
                    put_screen( 0, 997 );
                end
                if ( intro_screen_time == 75 )
                    put_screen( 0, 998 );
                end
                if ( intro_screen_time == 150 )
                    put_screen( 0, 999 );
                end
                intro_screen_time++;
                if ( intro_screen_time >= 225 )
                    put_screen( 0, 924 );
                    intro_screen = 1;
                    intro_screen_time = 0;
                end
            end
            if ( intro_screen == 1 )
                if ( intro_screen_time == 0 )
                    letters( 40, 52, 950, 200 );
                end
                if ( intro_screen_time == 10 )
                    letters( 85, 52, 951, 200 );
                end
                if ( intro_screen_time == 20 )
                    letters( 134, 52, 952, 200 );
                end
                if ( intro_screen_time == 30 )
                    letters( 182, 52, 953, 200 );
                end
                if ( intro_screen_time == 40 )
                    letters( 217, 52, 954, 200 );
                end
                if ( intro_screen_time == 50 )
                    letters( 247, 52, 955, 200 );
                end
                if ( intro_screen_time == 60 )
                    letters( 289, 52, 956, 200 );
                end
                if ( intro_screen_time == 70 )
                    letters( 60, 117, 957, 200 );
                end
                if ( intro_screen_time == 80 )
                    letters( 105, 117, 958, 200 );
                end
                if ( intro_screen_time == 90 )
                    letters( 220, 117, 959, 200 );
                end
                if ( intro_screen_time == 100 )
                    letters( 270, 117, 960, 200 );
                end
                if ( intro_screen_time == 110 )
                    letters( 75, 182, 961, 200 );
                end
                if ( intro_screen_time == 120 )
                    letters( 118, 182, 962, 200 );
                end
                if ( intro_screen_time == 130 )
                    letters( 172, 182, 963, 200 );
                end
                if ( intro_screen_time == 140 )
                    letters( 220, 182, 964, 200 );
                end
                if ( intro_screen_time == 150 )
                    letters( 260, 182, 964, 200 );
                end
                intro_screen_time += 5;
                if ( intro_screen_time == 280 )
                    sound_play( intro_sound, 0, 1 );
                end
                if ( intro_screen_time >= 300 )
                    signal( type letters, s_kill );
                    intro_screen = 2;
                    intro_screen_time = 0;
                end
            end
            if ( intro_screen == 2 )
                if ( intro_screen_time == 0 )
                    signal( type zombie, s_kill_tree );
                    signal( type player, s_kill_tree );
                    signal( type pick_up_item, s_kill_tree );
                    signal( type Final_boss, s_kill_tree );
                end
                if ( intro_screen_time == 10 )
                    letters( 61, 32, 950, 200 );
                end
                if ( intro_screen_time == 15 )
                    letters( 97, 48, 965, 200 );
                end
                if ( intro_screen_time == 20 )
                    letters( 125, 32, 957, 200 );
                end
                if ( intro_screen_time == 15 )
                    letters( 156, 48, 965, 200 );
                end
                if ( intro_screen_time == 30 )
                    letters( 193, 32, 959, 200 );
                end
                if ( intro_screen_time == 15 )
                    letters( 232, 48, 965, 200 );
                end
                if ( intro_screen_time == 40 )
                    letters( 264, 32, 961, 200 );
                end
                if ( intro_screen_time == 50 )
                    from i = 0 to 15;
                        fake_flame_scroll( 45 + rand( -10, 10 ) + 16 * i, 62 + rand( -3, 3 ), 90000, 2 );
                    end
                end
                intro_screen_time += 5;
                if ( intro_screen_time >= 55 )
                    main_menu();
                    intro_screen_time = 0;
                    intro_screen = 3;
                end
            end
        END
        IF ( intro_screen > 3 ) //....................................................................................<-start
            health_bar.graph = 770 + ( player_hp / 4 );
            if ( morter_tim > 0 )
                morter_tim--;
            end
            if ( boom_tim > 0 )
                boom_tim--;
            end
            if ( flame_fired == 1 )
                if ( flame_time == 0 )
                    fire_sound_wav = sound_play( fire_sound, 0, 1 );
                end
                flame_time++;
                if ( flame_time == 28 )
                    flame_fired = 0;
                    flame_time = 0;
                end
            end
            if ( punch_line_sound == 1 )
                sound_play( line1_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 2 )
                sound_play( line2_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 3 )
                sound_play( line3_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 4 )
                sound_play( line4_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 5 )
                sound_play( line5_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 6 )
                sound_play( line6_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 7 )
                sound_play( line7_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 8 )
                sound_play( line8_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( punch_line_sound == 9 )
                sound_play( line9_sound, 0, 6 );
                punch_line_sound = 0;
            end
            if ( got_hit_sound == 1 )
                sound_play( gothit1_sound, 0, 6 );
                got_hit_sound = 0;
            end
            if ( got_hit_sound == 2 )
                sound_play( gothit2_sound, 0, 6 );
                got_hit_sound = 0;
            end
            if ( got_hit_sound == 3 )
                sound_play( gothit3_sound, 0, 6 );
                got_hit_sound = 0;
            end
            if ( pause_screen == 0 and number_of_zombies > 0 )
                if ( zombie_die == 1 )
                    sound_play( zombie6_sound, 0, 4 );
                    zombie_die = 0;
                end
                if ( zombie_die == 2 )
                    sound_play( zombie10_sound, 0, 4 );
                    zombie_die = 0;
                end
                if ( zombie_die == 3 )
                    sound_play( zombie13_sound, 0, 4 );
                    zombie_die = 0;
                end
                if ( zombie_sound_time < 1 )
                    zombie_sound = rand( 1, 12 );
                    if ( zombie_sound == 1 )
                        sound_play( zombie1_sound, 0, 3 );
                    end
                    if ( zombie_sound == 2 )
                        sound_play( zombie2_sound, 0, 3 );
                    end
                    if ( zombie_sound == 3 )
                        sound_play( zombie3_sound, 0, 3 );
                    end
                    if ( zombie_sound == 4 )
                        sound_play( zombie4_sound, 0, 3 );
                    end
                    if ( zombie_sound == 5 )
                        sound_play( zombie5_sound, 0, 3 );
                    end
                    if ( zombie_sound == 6 )
                        sound_play( zombie7_sound, 0, 3 );
                    end
                    if ( zombie_sound == 7 )
                        sound_play( zombie8_sound, 0, 3 );
                    end
                    if ( zombie_sound == 8 )
                        sound_play( zombie9_sound, 0, 3 );
                    end
                    if ( zombie_sound == 9 )
                        sound_play( zombie11_sound, 0, 3 );
                    end
                    if ( zombie_sound == 11 )
                        sound_play( zombie12_sound, 0, 3 );
                    end
                    if ( zombie_sound == 12 )
                        sound_play( zombie14_sound, 0, 3 );
                    end
                    zombie_sound_time = rand( 220 - zombie_sound_time * 2, zombie_sound_time * 2 + 220 );
                end
                zombie_sound_time--;
            end
            //if(god_mode==1)player_hp=100;end
            //if(jkeys_state[_JKEY_L])set_fps(0,0);god_mode=1;end
            //if(jkeys_state[_JKEY_R])set_fps(30,4);god_mode=0;end
            if ( level_start == 1 and pause_screen == 0 and jkeys_state[ _JKEY_MENU ] )
                pause_hand();
                pause_screen = 1;
            end
        END //.........................................................................................................................<-end

        if ( ( mouse.x != oldmouse.x || mouse.y != oldmouse.y ) )
            axis2_x = mouse.x;
            axis2_y = mouse.y;
            oldmouse.x = mouse.x;
            oldmouse.y = mouse.y;
        else
            axis2_x = clamp( axis2_x + jkeys_state[ _JKEY_AXIS_RIGHTX ] / 2500, 0, 320);
            axis2_y = clamp( axis2_y + jkeys_state[ _JKEY_AXIS_RIGHTY ] / 2500, 0, 240);
        end

        mouse_x = axis2_x + scroll.x0;
        mouse_y = axis2_y + scroll.y0;

        if ( player_aim > 0 )
            player_aim -= weapon_aim_decrease;
        end
        if ( player_aim > weapon_aim_max )
            player_aim = weapon_aim_max;
        end
        id1.x = mouse_x -6 - player_aim;
        id1.y = mouse_y -6 - player_aim;
        id2.x = mouse_x + 6 + player_aim;
        id2.y = mouse_y -6 - player_aim;
        id3.x = mouse_x -6 - player_aim;
        id3.y = mouse_y + 6 + player_aim;
        id4.x = mouse_x + 6 + player_aim;
        id4.y = mouse_y + 6 + player_aim;
        //...................... levels with zombie regeneration...........................//
        IF ( level_start == 1 and pause_screen == 0 ) //start game
            //
            if ( level != 12 )
                if ( level_time > 0 and number_of_zombies < 1 and zombies_created >= max_number_of_zombies )
                    level_complete++;
                    if ( level_complete > 20 )
                        game_over();
                        level++;
                        level_start = 0;
                        level_complete = 0;
                    end
                end
            else
                if ( final_boss_hp < 1 )
                    end_of_game_time++;
                    if ( end_of_game_time > 150 )
                        final_boss_hp = 1986;
                        end_of_game();
                        end_of_game_time = 0;
                    end
                end
            end
            IF ( level == 1 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 6;
                        level_end_time = 300;
                        max_number_of_zombies = 25;
                        max_number_of_zombies_screen = 6;
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 0 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time == 250 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 160, 320 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 2 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 6;
                        level_end_time = 300;
                        max_number_of_zombies = 50;
                        max_number_of_zombies_screen = 8;
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 160 + i * rand( 10, 30 ), 0, rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time == 250 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 160, 320 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 3 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 6;
                        level_end_time = 300;
                        max_number_of_zombies = 50;
                        max_number_of_zombies_screen = 10;
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 4;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + rand( -30, 30 ), rand( 0, 400 ), rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time == 250 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 300 + rand( -30, 30 ), rand( 0, 400 ), rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 4 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 4;
                        max_items_on_screen = 6;
                        level_end_time = 320;
                        max_number_of_zombies = 75;
                        max_number_of_zombies_screen = 18;
                    end
                end
                if ( level_time == 90 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 0, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time = 200 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 100 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 3, 0 );
                        end
                    end
                end
                if ( level_time == 260 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 300, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 5 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 4;
                        max_items_on_screen = 6;
                        level_end_time = 300;
                        max_number_of_zombies = 100;
                        max_number_of_zombies_screen = 18;
                    end
                end
                if ( level_time == 80 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 0, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time == 150 )
                    if ( boss_arrive == 0 )
                        zombie( 400, 120, 5, 0, 2, 0 );
                        boss_arrive = 1;
                    end
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 100 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 2, 0 );
                        end
                    end
                end
                if ( level_time == 200 )
                    from i = 0 to 2;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 100 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 2, 0 );
                        end
                    end
                end
                if ( level_time == 230 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 300, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 6 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 5;
                        level_end_time = 300;
                        max_number_of_zombies = 100;
                        max_number_of_zombies_screen = 16;
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 16;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 200 + get_distx( 22500 * i, 210 ), 150 + get_disty( 22500 * i, 160 ), rand( 1, 4 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 7 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 5;
                        level_end_time = 300;
                        max_number_of_zombies = 125;
                        max_number_of_zombies_screen = 18;
                    end
                end
                if ( level_time == 70 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 0, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100 + i * rand( 10, 30 ), 400, rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time == 250 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 100 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 100 + i * rand( 10, 30 ), rand( 1, 4 ), 0, 3, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 8 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        items_drop_sequal = 3;
                        max_items_on_screen = 4;
                        level_end_time = 200;
                        max_number_of_zombies = 150;
                        max_number_of_zombies_screen = 16;
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 16;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 200 + get_distx( 22500 * i, 150 ), 150 + get_disty( 22500 * i, 100 ), rand( 1, 4 ), 0, 3, 1 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 0, 6, 0, 2, 0 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 0, 6, 0, 2, 0 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 300, 6, 0, 2, 0 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 300, 6, 0, 2, 0 );
                    end
                    level_time = 0;
                end
            END
            //
            IF ( level == 9 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_drop_sequal = 3;
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        max_items_on_screen = 4;
                        level_end_time = 150;
                        max_number_of_zombies = 100;
                        max_number_of_zombies_screen = 15;
                    end
                end
                if ( level_time == 50 )
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 120 + i * rand( 10, 30 ), rand( 1, 3 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time == 100 )
                    if ( boss_arrive == 0 )
                        zombie( 0, 120, 5, 0, 2, 0 );
                        boss_arrive = 1;
                    end
                    from i = 0 to 5;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 120 + i * rand( 10, 30 ), rand( 1, 3 ), 0, 2, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 10 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_drop_sequal = 3;
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        max_items_on_screen = 4;
                        level_end_time = 210;
                        max_number_of_zombies = 200;
                        max_number_of_zombies_screen = 18;
                    end
                end
                if ( level_time == 50 )
                    from i = 0 to 4;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 400, 120 + i * rand( 10, 30 ), rand( 1, 3 ), 0, 2, 0 );
                        end
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 120, 6, 0, 3, 0 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 140, 6, 0, 3, 0 );
                    end
                    from i = 0 to 4;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 160 + i * rand( 10, 30 ), 0, rand( 1, 3 ), 1, 3, 0 );
                        end
                    end
                end
                if ( level_time == 130 )
                    from i = 0 to 4;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 0, 120 + i * rand( 10, 30 ), rand( 1, 3 ), 1, 3, 0 );
                        end
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 120, rand( 1, 3 ), 0, 3, 0 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 140, rand( 1, 3 ), 0, 3, 0 );
                    end
                    from i = 0 to 4;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 160, 320 + i * rand( 10, 30 ), rand( 1, 3 ), 1, 3, 0 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 11 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 200, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_drop_sequal = 3;
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        max_items_on_screen = 4;
                        level_end_time = 300;
                        max_number_of_zombies = 150;
                        max_number_of_zombies_screen = 18;
                    end
                end
                if ( level_time == 50 )
                    from i = 0 to 1;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 300, 120 + i * rand( 10, 30 ), 6, 0, 3, 1 );
                        end
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time == 100 )
                    from i = 0 to 1;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100, 120 + i * rand( 10, 30 ), 6, 0, 3, 1 );
                        end
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 200 + i * rand( 10, 30 ), 100, rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time == 150 )
                    from i = 0 to 1;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 200 + i * rand( 10, 30 ), 100, 6, 0, 3, 1 );
                        end
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time == 200 )
                    from i = 0 to 1;
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 300, 120 + i * rand( 10, 30 ), 6, 0, 3, 1 );
                        end
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( 100, 120 + i * rand( 10, 30 ), rand( 1, 4 ), 1, 2, 1 );
                        end
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            IF ( level == 12 )
                if ( level_time == 0 )
                    if ( !exists( player_id ))
                        player( 100, 150 );
                        punch_line_sound = rand( 1, 9 );
                        items_drop_sequal = 3;
                        items_droped = items_drop_sequal;
                        item_min_type = 1;
                        item_max_type = 13;
                        max_items_on_screen = 4;
                        final_boss_hp = 1986;
                        level_end_time = 300;
                        max_number_of_zombies = 999;
                        max_number_of_zombies_screen = 10;
                    end
                end
                if ( level_time == 50 )
                    if ( boss_arrive == 0 )
                        Final_boss( 390, 150 );
                        boss_arrive = 1;
                    end
                end
                if ( level_time == 299 )
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 0, 6, 3, 3, 1 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 0, 300, 6, 3, 3, 1 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 0, 6, 3, 3, 1 );
                    end
                    if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                        zombie( 400, 300, 6, 3, 3, 1 );
                    end
                end
                if ( level_time < level_end_time )
                    if ( player_hp > 0 )
                        level_time++;
                    end
                else
                    level_time = 0;
                end
            END
            //
            if ( player_hp < 1 )
                death_delay++;
                if ( death_delay > 20 )
                    level_start = 2;
                    death_delay = 0;
                    level_time = 0;
                    game_over();
                end
            else
                if ( attack_player == 1 )
                    if ( blooddrain_time < 1 and gas_attack == 0 )
                        blood( player_id.x, player_id.y, 15 );
                        blooddrain_time = 10;
                        attack_player = 0;
                        got_hit_sound = rand( 1, 3 );
                        if ( got_hit_sound == 1 )
                            sound_play( gothit1_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                        if ( got_hit_sound == 2 )
                            sound_play( gothit2_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                        if ( got_hit_sound == 3 )
                            sound_play( gothit3_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                    end
                    if ( gas_attack == 1 and blooddrain_time < 1 )
                        gas( player_id.x, player_id.y, 0, 0 );
                        blooddrain_time = 10;
                        attack_player = 0;
                        gas_attack = 0;
                        got_hit_sound = rand( 1, 3 );
                        if ( got_hit_sound == 1 )
                            sound_play( gothit1_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                        if ( got_hit_sound == 2 )
                            sound_play( gothit2_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                        if ( got_hit_sound == 3 )
                            sound_play( gothit3_sound, 0, 6 );
                            got_hit_sound = 0;
                        end
                    end
                    blooddrain_time--;
                end
            end
        END
       IF ( level_start == 2 ) //player died
       end
       frame;
   end
end


process main_menu()
    private;
        int credit_id;
        g_pointf oldmouse;
        int timer_key;
        double y2;
begin
    mouse.graph = 910;
    graph = 897;
    x = 160;
    y = 140; y2 = y;
    z = 100;
    id1 = death_menu_gfx( 160, 90, z - 1, 100, 255, 920 ); // New Game
    id2 = death_menu_gfx( 160, 140, z - 1, 100, 255, 922 ); // Credits
    id3 = death_menu_gfx( 160, 190, z - 1, 100, 255, 923 ); // Exit Game
    write_delete( all_text );
    text.color_r=text.color_g=text.color_b=255;
    write( 0, 160, 234, 4, "made by: Ariel Yust 2010" );
    sound_stop( first_sound );
    //write_var(0,15,35,4,frame_info.fps);
    //write_var(0,70,15,4,zombies_killed);
    music_stop();
    sound_play( menu_sound, -1, 2 );
    oldmouse.x = mouse.x;
    oldmouse.y = mouse.y;
    loop
        if ( jkeys_state[ _JKEY_UP ] || jkeys_state[ _JKEY_DOWN ] || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] )
            disabled_mouse = 1;
        else
            timer_key = 0;
        end
        if ( ( mouse.x != oldmouse.x || mouse.y != oldmouse.y ) )
            disabled_mouse = 0;
            oldmouse.x = mouse.x;
            oldmouse.y = mouse.y;
        end

        if ( !mouse.left && !jkeys_state[ _JKEY_SELECT ] && !jkeys_state[ _JKEY_R ] )
            i = 1;
        end

        if ( !timer_key )
            if ( jkeys_state[ _JKEY_UP ] )
                y2 = clamp( y2 - 50, 90, 190 );
                timer_key = 6;
            end

            if ( jkeys_state[ _JKEY_DOWN ] )
                y2 = clamp( y2 + 50, 90, 190 );
                timer_key = 6;
            end
        end

        if ( credits == 1 ) // show credits
            if ( i2 == 0 )
                id1.alpha = 0;
                id2.alpha = 0;
                id3.alpha = 0;
                alpha = 0;
                credit_id = death_menu_gfx( 160, 510, z, size, 255, 996 );
                i2 = 1;
            end
            credit_id.y--;
            if ( i2 == 1 and credit_id.y < -350 or ( i == 1 and ( mouse.left || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) ) )
                signal( credit_id, s_kill );
                id1.alpha = 255;
                id2.alpha = 255;
                id3.alpha = 255;
                alpha = 255;
                credits = 0;
                i = 0;
                i2 = 0;
            end
        end

        if ( credits == 0 and ( ( !disabled_mouse and mouse.y <= 110 ) || ( disabled_mouse && y2 <= 110 ) ) ) // New Game
            if ( i == 1 and ( mouse.left || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) )
                sound_stop( all_sound );
                write_delete( all_text );
                signal( type death_menu_gfx, s_kill );
                signal( type letters, s_kill );
//                restore_type = NO_RESTORE;
                health_bar = death_menu_gfx( 26, 10, -499, 100, 255, 770 );
                text.color_r=text.color_g=text.color_b=255;
                write( 0, 292, 10, 4, "level: " );
                write_var( 0, 312, 10, 4, level );
                text.color_r=text.color_g=text.color_b=5;
                write( 0, 293, 10, 4, "level: " );
                write_var( 0, 313, 10, 4, level );
                write( 0, 291, 10, 4, "level: " );
                write_var( 0, 311, 10, 4, level );
                write( 0, 292, 9, 4, "level: " );
                write_var( 0, 312, 9, 4, level );
                write( 0, 292, 11, 4, "level: " );
                write_var( 0, 312, 11, 4, level );
                clear_screen();
                scroll_start( 0, 0, background_map, 0, 0, 0 );
                game_over();
                music_play( music1, -1 );
                intro_screen = 4;
                mouse.graph = 0;
                break;
            end
            if ( y > 90 )
                y -= 10;
            end
        end
        if ( credits == 0 and ( ( !disabled_mouse && mouse.y > 110 and mouse.y <= 170 ) ||  ( disabled_mouse && y2 > 110 and y2 <= 170 ) ) )
            if ( i == 1 and ( mouse.left || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) and credits == 0 )
                credits = 1;
                i = 0;
            end
            if ( y > 140 )
                y -= 10;
            end
            if ( y < 140 )
                y += 10;
            end
        end
        if ( credits == 0 and ( ( !disabled_mouse and mouse.y > 170 ) ||  ( disabled_mouse && y2 > 170 ) ) )
            if ( i == 1 and ( mouse.left || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) )
                exit( "Thanks for playing~! =] (c)Ariel Yust 2010", 0 );
            end
            if ( y < 190 )
                y += 10;
            end
        end
        timer_key = clamp( timer_key - 1, 0, 255 );
        frame;
    end
end


process end_of_game()
begin
    signal( type zombie, s_freeze_tree );
    signal( type player, s_freeze_tree );
    signal( type pick_up_item, s_freeze_tree );
    signal( type Final_boss, s_freeze_tree );
    sound_stop( all_sound );
    graph = 0;
    first_sound = sound_play( menu_sound, -1, 2 );
    music_stop();
    loop
        if ( move == 0 )
            level_start = 0;
            graph = 995;
            x = 160;
            y = 120;
            i++;
            if ( i > 200 )
                move = 1;
            end
        end
        if ( move == 1 )
            signal( type zombie, s_kill_tree );
            signal( type player, s_kill_tree );
            signal( type pick_up_item, s_kill_tree );
            signal( type Final_boss, s_kill_tree );
            signal( health_bar, s_kill );
            sound_stop( all_sound );
            map_clear( 0, background_map_c, 0 );
            map_clear( 0, background_map_c2, 0 );
            level_time = 0;
            boss_arrive = 0;
            level_end_time = 0;
            number_of_zombies = 0;
            max_number_of_zombies = 0;
            max_number_of_zombies_screen = 0;
            zombies_killed = 0;
            zombies_created = 0;
            blooddrain_time = 0;
            death_delay = 0;
            attack_player = 0;
            player_hp = 100;
            level_start = 0;
            hit_sound = 0;
            credits = 1;
            items_on_screen = 0;
            intro_screen = 2;
            intro_screen_time = 0;
            music_stop();
            scroll_stop( 0 );
            put_screen( 0, 924 );
            write_delete( all_text );
//            restore_type = PARTIAL_RESTORE;
            pause_screen = 0;
            break;
        end
        frame;
    end
end


process Final_boss( double x, y )
    private
        int attack_time;
        int speed;
        int shots;
        int distance;
        int max_hp;
        int spawn_zombies_time;
begin
    ctype = c_scroll;
    file = player_file;
    graph = 600;
    angle = 180000;
    speed = 3;
    max_hp = final_boss_hp;
    from i = 0 to 10;
        dirt( x - rand( 0, 20 ), y + rand( -20, 20 ), 0, 0 );
    end
    id1 = temp_gfx( x, y + 20, z -1, 0, 100, 255, file, 720, 0 );
    loop
        IF ( player_hp > 0 and final_boss_hp > 0 )
            distance = get_dist( player_id );
            if ( distance < 20 )
                player_hp -= 2;
                attack_player = 1;
                gas_attack = 0;
            end
            if ( move == 0 )
                attack_time++;
                if ( attack_time > 30 )
                    move_x = 200;
                    move_y = 150;
                    angle = fget_angle( x, y, move_x, move_y );
                    move = 1;
                    attack_time = 0;
                end
            end
            if ( move == 1 )
                angle = fget_angle( x, y, move_x, move_y );
                attack_time++;
                if ( anim > 2 )
                    graph++;
                    anim = 0;
                end
                if ( graph > 608 )
                    graph = 600;
                end
                if ( fget_dist( move_x, move_y, player_id.x, player_id.y ) < 50 )
                    if ( shots == 0 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                    end
                    if ( shots == 10 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                    end
                    if ( shots == 20 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        shots = -10;
                    end
                    shots++;
                end
                spawn_zombies_time++;
                if ( spawn_zombies_time > 50 )
                    tangle = fget_angle( x, y, player_id.x, player_id.y );
                    from i = -2 to 2;
                        ix = get_distx( tangle + i * 25000, 60 );
                        iy = get_disty( tangle + i * 25000, 60 );
                        if ( zombies_created < max_number_of_zombies and number_of_zombies < max_number_of_zombies_screen )
                            zombie( x + ix, y + iy, rand( 1, 4 ), 0, 3, 1 );
                        end
                    end
                    spawn_zombies_time = 0;
                end
                if ( fget_dist( x, y, move_x, move_y ) < 10 )
                    if ( attack_time > 30 )
                        angle = fget_angle( x, y, player_id.x, player_id.y );
                        move = 2;
                        attack_time = 0;
                    end
                else
                    x += get_distx( angle, speed );
                    y += get_disty( angle, speed );
                    anim++;
                end
            end
            if ( move == 2 )
                anim++;
                if ( anim > 2 )
                    graph++;
                    anim = 0;
                end
                if ( graph > 608 )
                    graph = 600;
                end
                if ( distance > 20 )
                    x += get_distx( angle, speed );
                    y += get_disty( angle, speed );
                    if ( shots == 0 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        attack_time += rand( 1, 10 );
                    end
                    if ( shots == 10 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        attack_time += rand( 1, 10 );
                    end
                    if ( shots == 20 )
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        gas_shot( x, y, angle + rand( -10000, 10000 ), 20 );
                        attack_time += rand( 1, 10 );
                        shots = -10;
                    end
                    shots++;
                end
                if ( attack_time > 30 )
                    move_x = 200;
                    move_y = 150;
                    angle = fget_angle( x, y, move_x, move_y );
                    move = 1;
                    attack_time = 0;
                end
            end
        END
        if ( final_boss_hp > 0 )
            id1.x = x;
            id1.y = y + 20;
            id1.z = z -20;
            id1.graph = 721 + ( final_boss_hp * 100 / max_hp ) / 10;
            if ( morter_tim > 0 and fget_dist( x, y, morter_x, morter_y ) < 55 )
                hp -= morter_dmg;
            end
            if ( boom_tim > 0 and fget_dist( x, y, boom_x, boom_y ) < 55 )
                hp -= boom_dmg;
            end
            z = - y;
        else
            if ( graph != 609 )
                sound_stop( all_sound );
                signal( type pick_up_item, s_kill_tree );
                sound_play( finalboss_sound, 0, 1 );
                from i = 0 to 16;
                    gas( x, y, i * 25000, 6 );
                    gas( x, y, i * 25000, 10 );
                end
                signal( id1, s_kill );
                graph = 609;
                z = 0;
            end
        end
        //if(key(_1))final_boss_hp=0;end
        frame;
    end
end


process game_over()
begin
    signal( type zombie, s_freeze_tree );
    signal( type player, s_freeze_tree );
    signal( type pick_up_item, s_freeze_tree );
    signal( type Final_boss, s_freeze_tree );
    graph = 905;
    z = -501;
    x = 160;
    y = 120;
    size = 0;
    if ( player_hp > 0 )
        x = 160;
        y = 120;
        move = 1;
        alpha = 255;
        size = 0;
        move = 1;
    end
    id1 = death_menu_gfx( 160, 120, z + 1, 100, 0, 896 );
    loop
        if ( move != 4 and id1.alpha < 255 )
            id1.alpha += 10;
        else
            if ( id1.alpha > 0 )
                id1.alpha -= 10;
            end
        end
        if ( move == 0 ) //apear
            if ( i3 == 0 )
                i3 = rand( 1, 2 );
            end
            if ( i3 == 1 )
                sound_play( scream1_sound, 0, 1 );
                i3 = 3;
            end
            if ( i3 == 2 )
                sound_play( scream2_sound, 0, 1 );
                i3 = 3;
            end
            if ( size < 100 )
                size += 5;
            end
            if ( size => 100 )
                size = 100;
                y--;
                if ( y < 120 )
                    alpha -= 10;
                end
            end
            if ( alpha < 80 )
                x = 160;
                y = 120;
                move = 1;
                alpha = 255;
                size = 0;
            end
        end
        if ( move == 1 ) //3
            graph = 906;
            if ( size < 100 )
                size += 5;
            end
            if ( size => 100 )
                size = 100;
                move++;
                size = 0;
            end
        end
        if ( move == 2 ) //2
            graph = 907;
            if ( size < 100 )
                size += 5;
            end
            if ( size => 100 )
                size = 100;
                move++;
                size = 0;
            end
        end
        if ( move == 3 ) //1
            graph = 908;
            if ( size < 100 )
                size += 5;
            end
            if ( size => 100 )
                size = 100;
                move++;
                size = 0;
            end
        end
        if ( move == 4 ) //attack!
            graph = 909;
            if ( size < 100 )
                size += 5;
            end
            if ( size => 100 )
                if ( i2 == 0 )
                    //   sound_play(fire_sound,0,1);
                    from i = 0 to 10;
                        fake_flame_scroll( 80 + rand( -10, 10 ) + 16 * i, 120 + rand( -3, 3 ), 90000, 2 );
                    end;
                    i2 = 1;
                    i = 0;
                end
                size = 100;
                i++;
            end
            if ( i > 10 )
                move = 5;
            end
        end
        if ( move == 5 )
            mouse.graph = 0;
            map_clear( 0, background_map_c, 0 );
            map_clear( 0, background_map_c2, 0 );
            signal( type zombie, s_kill_tree );
            signal( type player, s_kill_tree );
            signal( type pick_up_item, s_kill_tree );
            signal( type Final_boss, s_kill_tree );
            level_time = 0;
            boss_arrive = 0;
            level_end_time = 0;
            number_of_zombies = 0;
            max_number_of_zombies = 0;
            max_number_of_zombies_screen = 0;
            zombies_killed = 0;
            zombies_created = 0;
            blooddrain_time = 0;
            death_delay = 0;
            attack_player = 0;
            player_hp = 100;
            level_start = 1;
            hit_sound = 0;
            items_on_screen = 0;
            signal( id1, s_kill );
            break;
        end
        frame;
    end
end


process pause_hand()
    private
        int id5;
        int id6;
        g_pointf oldmouse;
        int timer_key;
        double y2;        
begin
    music_pause();
    first_sound = sound_play( menu_sound, -1, 2 );
    signal( type zombie, s_freeze_tree );
    signal( type player, s_freeze_tree );
    signal( type pick_up_item, s_freeze_tree );
    signal( type Final_boss, s_freeze_tree );
    graph = 900;
    z = -500;
    x = 150;
    y = 360;
    from i = 0 to 60;
        from i2 = 0 to 45;
            from i3 = 0 to 2;
                collision_matrix[ i ][ i2 ][ i3 ] = 0;
            end
        end
    end
    id2 = death_menu_gfx( 160, 120, z + 1, 100, 0, 903 ); // background
    id3 = death_menu_gfx( 160, 50, z -2, 100, 0, 902 ); // text pause
    id4 = death_menu_gfx( 160, 110, z -2, 100, 0, 898 ); //continue
    id5 = death_menu_gfx( 160, 170, z -2, 100, 0, 899 ); //quit game
    id6 = death_menu_gfx( 160, 110, z -1, 100, 0, 897 ); //cursor
    oldmouse.x = mouse.x;
    oldmouse.y = mouse.y;
    loop
        if ( id2.alpha < 255 and move < 3 )
            id2.alpha = clamp( id2.alpha + 10, 0, 255 );
            id3.alpha = clamp( id3.alpha + 10, 0, 255 );
            id4.alpha = clamp( id4.alpha + 10, 0, 255 );
            id5.alpha = clamp( id5.alpha + 10, 0, 255 );
            id6.alpha = clamp( id6.alpha + 10, 0, 255 );
        end
        if ( move == 0 )
            if ( x < 160 )
                x++;
            end
            if ( y > 120 )
                y -= 10;
            end
            if ( x == 160 and y == 120 )
                move = 1;
                graph = 901;
                y2 = y;
            end
        end
        if ( move == 1 )
            id1 = death_menu_gfx( 160, 140, z -1, 100, 0, 902 );
            move = 2;
            mouse.graph = 910;
            i = 0;
        end
        if ( move == 2 )
            if ( jkeys_state[ _JKEY_UP ] || jkeys_state[ _JKEY_DOWN ] || jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] )
                disabled_mouse = 1;
            else
                timer_key = 0;
            end
            if ( ( mouse.x != oldmouse.x || mouse.y != oldmouse.y ) )
                disabled_mouse = 0;
                oldmouse.x = mouse.x;
                oldmouse.y = mouse.y;
            end

            if ( !mouse.left && !jkeys_state[ _JKEY_SELECT ] && !jkeys_state[ _JKEY_R ] )
                i = 1;
            end

            if ( !timer_key )
                if ( jkeys_state[ _JKEY_UP ] )
                    y2 = clamp( y2 - 60, 110, 170 );
                    timer_key = 6;
                end

                if ( jkeys_state[ _JKEY_DOWN ] )
                    y2 = clamp( y2 + 60, 110, 170 );
                    timer_key = 6;
                end
            end

            if ( i == 1 )
                if ( ( !disabled_mouse and mouse.y < 140 ) || ( disabled_mouse && y2 < 140 ) )
                    if ( id6.y < 110 )
                        id6.y += 10;
                    end
                    if ( id6.y > 110 )
                        id6.y -= 10;
                    end
                    if ( ( !disabled_mouse and mouse.x > 50 and mouse.x < 270 and mouse.left ) || ( disabled_mouse && ( jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) ) )
                        signal( id3, s_kill );
                        signal( id4, s_kill );
                        signal( id5, s_kill );
                        signal( id6, s_kill );
                        move = 3;
                        mouse.graph = 0;
                    end
                end
                if ( ( !disabled_mouse and mouse.y >= 140 ) || ( disabled_mouse && y2 >= 140 ) )
                    if ( id6.y < 170 )
                        id6.y += 10;
                    end
                    if ( id6.y > 170 )
                        id6.y -= 10;
                    end
                    if ( ( !disabled_mouse and mouse.x > 50 and mouse.x < 270 and mouse.left ) || ( disabled_mouse && ( jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_R ] ) ) )
                        move = 4;
                        mouse.graph = 0;
                    end
                end
            end

            timer_key = clamp( timer_key - 1, 0, 255 );
        end
        if ( move == 3 ) //continue
            y += 10;
            id1.size -= 10;
            id2.alpha -= 10;
            if ( y > 360 )
                signal( type zombie, s_wakeup_tree );
                signal( type player, s_wakeup_tree );
                signal( type pick_up_item, s_wakeup_tree );
                signal( type Final_boss, s_wakeup_tree );
                signal( id1, s_kill );
                signal( id2, s_kill );
                signal( id3, s_kill );
                signal( id4, s_kill );
                signal( id5, s_kill );
                signal( id6, s_kill );
                music_resume();
                sound_stop( first_sound );
                pause_screen = 0;
                break;
            end
        end
        if ( move == 4 ) //exit
            signal( type zombie, s_kill_tree );
            signal( type player, s_kill_tree );
            signal( type pick_up_item, s_kill_tree );
            signal( type Final_boss, s_kill_tree );
            signal( id1, s_kill );
            signal( id2, s_kill );
            signal( id3, s_kill );
            signal( id4, s_kill );
            signal( id5, s_kill );
            signal( id6, s_kill );
            signal( health_bar, s_kill );
            sound_stop( all_sound );
            map_clear( 0, background_map_c, 0 );
            map_clear( 0, background_map_c2, 0 );
            level_time = 0;
            boss_arrive = 0;
            level_end_time = 0;
            number_of_zombies = 0;
            max_number_of_zombies = 0;
            max_number_of_zombies_screen = 0;
            zombies_killed = 0;
            zombies_created = 0;
            blooddrain_time = 0;
            death_delay = 0;
            attack_player = 0;
            player_hp = 100;
            level_start = 0;
            hit_sound = 0;
            items_on_screen = 0;
            intro_screen = 2;
            intro_screen_time = 0;
            music_stop();
            scroll_stop( 0 );
            put_screen( 0, 924 );
            write_delete( all_text );
//            restore_type = PARTIAL_RESTORE;
            pause_screen = 0;
            break;
        end
        frame;
    end
end


process dead_zombie( x, y, graph, angle, speed, i2 )
begin
    ctype = c_scroll;
    file = player_file;
    if ( speed > 6 )
        speed = 6;
    end
    loop
        if ( speed > 0 )
            i++;
            if ( i > i2 )
                speed--;
                i = 0;
                if ( graph < 370 )
                    map_xput( 0, background_map_c2, rand( 360, 361 ), x, y, angle + rand( -90000, 90000 ), size, 0 );
                end
            end
            x -= get_distx( angle, speed );
            y -= get_disty( angle, speed );
        end
        if ( speed < 2 )
            if ( graph => 370 )
                map_xput( 0, background_map_c, graph, x, y, angle, size, 0 );
                break;
            end
            if ( graph < 370 )
                insert_dir = rand( 0, 1 );
                if ( insert_dir == 0 )
                    map_xput( 0, background_map_c, rand( 362, 364 ), x, y, angle, size, 0 );
                else
                    map_xput( 0, background_map_c2, rand( 362, 364 ), x, y, angle, size, 0 );
                end
                if ( graph > 300 and graph < 370 )
                    map_xput( 0, background_map_c, graph, x, y, angle, size, 0 );
                    break;
                end
                alpha -= 5;
                if ( alpha < 100 )
                    break;
                end
            end
        end
        if ( region_out( id, 0 ))
            break;
        end
        frame;
    end
end


process pick_up_item( x, y, item_type )
begin
    ctype = c_scroll;
    file = player_file;
    graph = item_type + 699;
    z = - y -10;
    items_on_screen++;
    loop
        if ( move == 0 )
            i++;
            if ( i > 300 )
                items_on_screen--;
                break;
            end
            if ( i2 = get_id( type player ))
                if ( get_dist( i2 ) < 25 )
                    if ( item_type < 12 )
                        i2.new_weapon = item_type;
                        i2.weapon_refresh = 1;
                    end
                    items_on_screen--;
                    move = 1;
                    i = 0;
                end
            end
        end
        if ( move == 1 )
            if ( item_type == 13 )
                player_hp += 20;
                if ( player_hp > 100 )
                    player_hp = 100;
                end
                break;
            end
            if ( item_type < 12 )
                break;
            end
            if ( item_type == 12 )
                if ( graph < 740 )
                    graph = 740;
                end
                anim++;
                if ( anim > 10 )
                    graph++;
                    i++;
                    anim = 0;
                end
                if ( i > 2 )
                    boom_tim = 1;
                    boom_x = x;
                    boom_y = y;
                    boom_dmg = 20;
                    from i = 0 to 16;
                        fake_flame( x, y, i * 25000, 6 );
                    end
                    i2 = rand( 1, 3 );
                    if ( i2 == 1 )
                        sound_play( explode1_sound, 0, 1 );
                    end
                    if ( i2 == 2 )
                        sound_play( explode2_sound, 0, 1 );
                    end
                    if ( i2 == 3 )
                        sound_play( explode3_sound, 0, 1 );
                    end
                    break;
                end
            end
        end
        frame;
    end
end


process zombie( double x, y, int zombie_type, AI, speed, starting_way )
    private
        int snork_action;
        int snork_time;
        int snork_dir;
        int snork_time2;
        int enter_map;
        int turn_speed;
        int a;
        int closest_zombie;
        int refresh_time2;
        int ai_move;
        int ai_time;
        int distance;
        int _shot;
        int dig_out_time;
        int id6;
        int call_for_backup;
        int pid;
        int last_cellx = -1;
        int last_celly = -1;
        int last_id = 1;
        int cellx;
        int celly;
        int shoot_time;
        int zombie_shoot_time;
begin
    ctype = c_scroll;
    file = player_file;
    graph = 200;
    last_x = x;
    last_y = y;
    move_x = x;
    move_y = y;
    move_x = 240;
    move_y = 160;
    switch ( zombie_type )
        case 1:
            graph = 200;
            hp = 15 + rand( -5, 5 );
        end
        case 2:
            graph = 201;
            hp = 15 + rand( -5, 5 );
        end
        case 3:
            graph = 202;
            hp = 15 + rand( -5, 5 );
        end
        case 4:
            graph = 203;
            hp = 15 + rand( -5, 5 );
        end
        case 5:
            graph = 204;
            hp = 400;
            zombie_shoot_time = 50;
            ai = 0;
            id6 = temp_gfx( x, y + 20, z -1, 0, 100, 255, file, 720, angle );
        end //boss1
        case 6:
            graph = 205;
            speed = 3;
            hp = 25 + rand( -5, 5 );
            ai = 3;
        end //snork
    end
    if ( speed < 2 )
        speed = 2;
    end
    number_of_zombies++;
    zombies_created++;
    if ( zombie_type != 6 )
        id1 = zombie_body();
    end
    if ( starting_way == 1 )
        dirt( x, y, 0, 0 );
        dig_out_time = 20;
    end
    loop
        if ( final_boss_hp < 1 )
            hp = 0;
            on_fire = 0;
        end
        if ( dig_out_time > 0 )
            dig_out_time--;
        end
        if ( morter_tim > 0 and fget_dist( x, y, morter_x, morter_y ) < 55 )
            hp -= morter_dmg;
        end
        if ( boom_tim > 0 and fget_dist( x, y, boom_x, boom_y ) < 55 )
            hp -= boom_dmg;
        end
        if ( on_fire == 1 )
            I_got_hit = 1;
            on_fire_time++;
            if ( on_fire_time > 12 )
                if ( !region_out( id, 0 ))
                    fake_flame( x, y, 90000, 2 );
                end
                hp -= 1;
                on_fire_time = 0;
            end
        end
        if ( hp < 1 )
            if ( zombie_type == 5 )
                signal( id6, s_kill );
            end
            if ( items_droped <= 0 )
                pick_up_item( x, y, rand( item_min_type, item_max_type ));
                items_droped = items_drop_sequal;
            end
            if ( zombie_die == 0 )
                zombie_die = rand( 1, 3 );
            end
            zombies_killed++;
            number_of_zombies--;
            collision_matrix[ last_cellx ][ last_celly ][ last_id ] = 0;
            collision_matrix[ last_cellx ][ last_celly ][ 0 ]--;
            if ( on_fire == 0 )
                from i = 0 to - hp / 3;
                    dead_zombie( x, y, rand( 350, 354 ), rand( 0, 360000 ), 4 - ( hp ), rand( 1, 2 ));
                end
                if ( zombie_type != 6 )
                    dead_zombie( x, y, graph + 50, angle, 6 - ( hp ), 1 );
                    dead_zombie( x, y, graph + 100, angle + rand( -90000, 90000 ), 4 - ( hp ), 2 );
                else
                    dead_zombie( x, y, 255, angle, 6 - ( hp ), 1 );
                    dead_zombie( x, y, 305, angle + rand( -90000, 90000 ), 4 - ( hp ), 2 );
                end
            end
            if ( on_fire == 1 )
                dead_zombie( x, y, rand( 370, 371 ), angle, 0, 0 );
            end
            if ( items_on_screen < max_items_on_screen )
                items_droped--;
            end
            if ( zombie_type != 6 )
                signal( id1, s_kill );
            end
            break;
        end
        //.........................last movement.............................//
        if ( enter_map == 0 and dig_out_time < 1 )
            x += get_distx( angle, speed );
            y += get_disty( angle, speed );
            if ( x < 10 or x > 470 or y < 10 or y > 350 )
                angle = fget_angle( x, y, move_x, move_y );
            else
                enter_map = 1;
            end
        end
        if ( enter_map == 1 and dig_out_time < 1 )
            if ( player_hp > 0 )
                refresh_time++;
                If ( refresh_time > 4 )
                    cellx = x / 8;
                    celly = y / 8;
                    if ( collision_matrix[ cellx ][ celly ][ 2 ] == 0 )
                        collision_matrix[ cellx ][ celly ][ 0 ]++;
                        if ( last_cellx != -1 and last_celly != -1 )
                            collision_matrix[ last_cellx ][ last_celly ][ last_id ] = 0;
                            collision_matrix[ last_cellx ][ last_celly ][ 0 ]--;
                        end
                        last_cellx = cellx;
                        last_celly = celly;
                        collision_matrix[ cellx ][ celly ][ 2 ] = id;
                        last_id = 2;
                    end
                    if ( collision_matrix[ cellx ][ celly ][ 0 ] == 1 and last_id == 2 )
                        if ( last_cellx != -1 and last_celly != -1 )
                            collision_matrix[ last_cellx ][ last_celly ][ last_id ] = 0;
                        end
                        last_cellx = cellx;
                        last_celly = celly;
                        collision_matrix[ cellx ][ celly ][ 1 ] = id;
                        last_id = 1;
                    end
                    if ( collision_matrix[ cellx ][ celly ][ 0 ] > 1 and last_id == 2 )
                        pid = collision_matrix[ cellx ][ celly ][ 1 ];
                        if ( exists( pid ))
                            tangle = fget_angle( x, y, pid.x, pid.y ) / 5000 * 5000;
                            x -= get_distx( tangle, 8 );
                            y -= get_disty( tangle, 8 );
                            pid.x += get_distx( tangle, 8 );
                            pid.y += get_disty( tangle, 8 );
                        end
                    end
                    refresh_time = 0;
                End
                refresh_time2++;
                if ( refresh_time2 > 3 )
                    angle = fget_angle( x, y, move_x, move_y ) / 5000 * 5000;
                end
                distance = get_dist( player_id );
                if ( distance > 16 )
                    if ( zombie_type == 5 ) //boss 1 shooting
                        shoot_time--;
                        if ( shoot_time < 1 )
                            if ( _shot == 0 )
                                get_real_point( 1, &graph1_x, &graph1_y );
                                zombie_shot( graph1_x, graph1_y, z, 1, angle, 10 );
                            end
                            if ( _shot == 10 )
                                get_real_point( 1, &graph1_x, &graph1_y );
                                zombie_shot( graph1_x, graph1_y, z, 1, angle, 10 );
                            end
                            _shot++;
                            if ( _shot == 20 )
                                get_real_point( 1, &graph1_x, &graph1_y );
                                zombie_shot( graph1_x, graph1_y, z, 1, angle, 10 );
                                shoot_time = zombie_shoot_time;
                                _shot = 0;
                            end
                        end
                    end
                    if ( AI == 0 ) //simple zombie
                        move_x = player_id.x;
                        move_y = player_id.y;
                        x += get_distx( angle, speed );
                        y += get_disty( angle, speed );
                    end
                    if ( AI == 1 ) //smarter zombie
                        If ( I_got_hit == 0 or distance >= 100 )
                            if ( fget_dist( x, y, move_x, move_y ) < 10 )
                                move_x += rand( -5, 5 ) * 10;
                                move_y += rand( -5, 5 ) * 10;
                                if ( move_x < 15 )
                                    move_x = 15;
                                end
                                if ( move_x > 385 )
                                    move_x = 385;
                                end
                                if ( move_y < 15 )
                                    move_y = 15;
                                end
                                if ( move_y > 285 )
                                    move_y = 285;
                                end
                                ai_move = 1;
                            end
                        End
                        If ( I_got_hit == 1 and distance > 300 or ai_time > 100 )
                            I_got_hit = 0;
                            ai_time = 0;
                        end
                        if ( I_got_hit == 1 or distance < 100 )
                            ai_time++;
                            if ( call_for_backup == 0 )
                                while ( pid = get_id( type zombie ))
                                    if ( fget_dist( x, y, pid.x, pid.y ) < 25 )
                                        pid.I_got_hit = 1;
                                    end
                                end
                                call_for_backup = 1;
                            end
                            move_x = player_id.x;
                            move_y = player_id.y;
                            ai_move = 1;
                        end
                        x += get_distx( angle, speed );
                        y += get_disty( angle, speed );
                    End
                    if ( AI == 3 ) //snork zombie
                        snork_time++;
                        if ( I_got_hit != 0 and snork_action != 3 )
                            snork_action = 3;
                        end
                        if ( snork_time > 50 )
                            snork_action++;
                            if ( snork_action > 3 )
                                snork_action = 0;
                            end
                            I_got_hit = 0;
                            snork_time = 0;
                        end
                        if ( snork_action == 0 )
                            move_x = player_id.x;
                            move_y = player_id.y;
                            x += get_distx( angle + 45000, speed );
                            y += get_disty( angle + 45000, speed );
                        end
                        if ( snork_action == 1 )
                            move_x = player_id.x;
                            move_y = player_id.y;
                            x += get_distx( angle, speed );
                            y += get_disty( angle, speed );
                        end
                        if ( snork_action == 2 )
                            move_x = player_id.x;
                            move_y = player_id.y;
                            x += get_distx( angle -45000, speed );
                            y += get_disty( angle -45000, speed );
                        end
                        if ( snork_action == 3 ) //jump
                            snork_time2++;
                            if ( snork_time2 == 10 )
                                move_x = player_id.x;
                                move_y = player_id.y;
                            end
                            if ( snork_time2 > 10 )
                                graph = 208;
                                x += get_distx( angle, speed * 4 );
                                y += get_disty( angle, speed * 4 );
                                if ( fget_dist( x, y, move_x, move_y ) < 10 )
                                    snork_time = 0;
                                    snork_action = 0;
                                    snork_time2 = 0;
                                    I_got_hit = 0;
                                end
                            end
                        end
                    end
                    if ( zombie_type == 6 )
                        anim++;
                        if ( snork_dir == 0 and anim > 2 )
                            graph++;
                            anim = 0;
                        end
                        if ( snork_dir == 1 and anim > 2 )
                            graph--;
                            anim = 0;
                        end
                        if ( graph > 207 )
                            graph = 207;
                            snork_dir = 1;
                        end
                        if ( graph < 205 )
                            graph = 205;
                            snork_dir = 0;
                        end
                    end
                else
                    if ( zombie_type == 6 )
                        graph = 208;
                    end
                    player_hp -= 2;
                    attack_player = 1;
                end
            end
            if ( x < 10 )
                x = 10;
            end
            if ( x > 390 )
                x = 390;
            end
            if ( y < 10 )
                y = 10;
            end
            if ( y > 290 )
                y = 290;
            end
        end
        z = - y;
        if ( zombie_type != 6 )
            id1.x = x;
            id1.y = y + 4;
            id1.z = z + 1;
            id1.angle = angle;
        end
        last_x = x;
        last_y = y;
        if ( zombie_type == 5 )
            id6.x = x;
            id6.y = y + 20;
            id6.z = z -20;
            id6.graph = 721 + ( hp * 100 / 400 ) / 10;
        end
        frame;
    end
end


process zombie_body()
begin
    ctype = c_scroll;
    file = player_file;
    graph = 150;
    loop
        if ( last_x == x and last_y == y )
            move = 0;
        else;
            move = 1;
        end
        if ( move > 0 )
            anim++;
            if ( anim > 2 )
                graph++;
                anim = 0;
            end
            if ( graph > 157 )
                graph = 150;
            end
        else
            graph = 150;
        end
        last_x = x;
        last_y = y;
        m_last_x = mouse_x;
        m_last_y = mouse_y;
        frame;
    end
end


process player( double x, y )
    private
        int last_weapon_mode;
        int b;
        int temp_key;
begin
    ctype = c_scroll;
    player_id = id;
    file = player_file;
    weapon = 1;
    last_weapon_mode = 1;
    weapon = 1;
    graph1 = 50;
    ammo = 255;
    last_x = x;
    last_y = y;
    m_last_x = mouse_x;
    m_last_y = mouse_y;
    scroll.camera = id;
    graph = 1;
    id1 = player_body();
    id2 = temp_gfx( graph1_x, graph1_y, z -1, flags, size, alpha, file, graph1, angle1 );
    id3 = temp_gfx( graph2_x, graph2_y, z -1, flags, size, alpha, file, graph2, angle2 );
    //new_weapon=9;weapon_refresh=1;
    loop
        if ( player_hp < 1 )
            from i = 0 to 20;
                dead_zombie( x, y, rand( 350, 354 ), rand( 0, 360000 ), rand( 5, 7 ), 1 );
            end
            signal( id1, s_kill );
            signal( id2, s_kill );
            signal( id3, s_kill ); //signal(id4,s_kill);
            break;
        end
        //.........................weapons.............................//
        if ( weapon_refresh == 1 )
            mouse.graph = 0;
            if (( last_weapon_mode == 1 or last_weapon_mode == 2 ) and weapon == new_weapon )
                last_weapon_mode = 2;
                graph2 = graph1;
                graph = 2;
            else
                weapon = new_weapon;
                if ( new_weapon == 1 )
                    last_weapon_mode = 1;
                    graph = 1;
                    graph1 = 50;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 2 )
                    last_weapon_mode = 1;
                    graph = 1;
                    graph1 = 51;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 3 )
                    last_weapon_mode = 1;
                    graph = 1;
                    graph1 = 52;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 4 )
                    last_weapon_mode = 1;
                    graph = 1;
                    graph1 = 53;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 5 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 54;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 6 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 55;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 7 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 56;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 8 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 57;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 9 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 58;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 10 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 59;
                    graph2 = 0;
                    ammo = 255;
                end
                if ( new_weapon == 11 )
                    last_weapon_mode = 4;
                    graph = 4;
                    graph1 = 60;
                    graph2 = 0;
                    ammo = 255;
                end
            end
            ammo = 255;
            weapon_refresh = 0;
        end
        if ( jkeys_state[ _JKEY_L ] )
            if ( last_weapon_mode == 2 )
                weapon_mode = 3;
                graph = 3;
                angle2 = angle + 90000;
                angle1 = angle + 270000;
            end
        end
        if ( !jkeys_state[ _JKEY_L ] or( jkeys_state[ _JKEY_L ] and last_weapon_mode != 2 ))
            weapon_mode = last_weapon_mode;
            angle1 = angle;
            angle2 = angle;
        end
        if ( weapon_mode == 1 )
            graph = 1;
        end
        if ( weapon_mode == 2 )
            graph = 2;
        end
        if ( weapon_mode == 4 )
            graph = 4;
        end
        if ( weapon_mode == 5 )
            graph = 5;
        end
        //.........................shadow.............................//
        refresh_time++;
        if ( refresh_time > 0 )
            if ( m_last_x != mouse_x or m_last_y != mouse_y or last_x != x or last_y != y )
                get_real_point( 1, &graph1_x, &graph1_y );
                get_real_point( 2, &graph2_x, &graph2_y );
                id2.x = graph1_x;
                id2.y = graph1_y;
                id2.z = z -1;
                id2.angle = angle1;
                id2.graph = graph1;
                id3.x = graph2_x;
                id3.y = graph2_y;
                id3.z = z -1;
                id3.angle = angle2;
                id3.graph = graph2;
            end
            refresh_time = 0;
        end
        //.........................using weapons.............................//
        if ( final_boss_hp > 0 and ( mouse.left || jkeys_state[ _JKEY_R ] ) and shot_time <= 0 )
            attack = 1;
        end
        if ( weapon == 10 and !mouse.left and !jkeys_state[ _JKEY_R ] and flame_fired == 1 )
            flame_fired = 0;
            flame_time = 0;
            sound_stop( fire_sound_wav );
        end
        switch ( weapon )
            case 1: //pistol
                weapon_dmg = 5;
                weapon_ammo = 12;
                weapon_reload_time = 30;
                weapon_aim_raise = 2;
                weapon_aim_decrease = 2;
                weapon_aim_max = 20;
                per_shot_time = 5;
            end
            case 2: //revolver
                weapon_dmg = 10;
                weapon_ammo = 6;
                weapon_reload_time = 45;
                weapon_aim_raise = 10;
                weapon_aim_decrease = 2;
                weapon_aim_max = 20;
                per_shot_time = 6;
            end
            case 3: //mpk
                weapon_dmg = 5;
                weapon_ammo = 30;
                weapon_reload_time = 40;
                weapon_aim_raise = 4;
                weapon_aim_decrease = 1;
                weapon_aim_max = 30;
                per_shot_time = 2;
            end
            case 4: //mac
                weapon_dmg = 5;
                weapon_ammo = 30;
                weapon_reload_time = 40;
                weapon_aim_raise = 4;
                weapon_aim_decrease = 1;
                weapon_aim_max = 30;
                per_shot_time = 1;
            end
            case 5: //double barrle
                weapon_dmg = 5;
                weapon_ammo = 2;
                weapon_reload_time = 40;
                weapon_aim_raise = 10;
                weapon_aim_decrease = 1;
                weapon_aim_max = 40;
                per_shot_time = 10;
            end
            case 6: //pump shotgun
                weapon_dmg = 5;
                weapon_ammo = 8;
                weapon_reload_time = 80;
                weapon_aim_raise = 10;
                weapon_aim_decrease = 1;
                weapon_aim_max = 40;
                per_shot_time = 8;
            end
            case 7: //mechingun
                weapon_dmg = 15;
                weapon_ammo = 100;
                weapon_reload_time = 90;
                weapon_aim_raise = 4;
                weapon_aim_decrease = 1;
                weapon_aim_max = 50;
                per_shot_time = 4;
            end
            case 8: //ak47
                weapon_dmg = 10;
                weapon_ammo = 30;
                weapon_reload_time = 45;
                weapon_aim_raise = 5;
                weapon_aim_decrease = 1;
                weapon_aim_max = 30;
                per_shot_time = 2;
            end
            case 9: //grenade launcher
                weapon_dmg = 15;
                weapon_ammo = 1;
                weapon_reload_time = 50;
                weapon_aim_raise = 5;
                weapon_aim_decrease = 1;
                weapon_aim_max = 30;
                per_shot_time = 20;
            end
            case 10: //flamethrower
                weapon_dmg = 1;
                weapon_ammo = 200;
                weapon_reload_time = 40;
                weapon_aim_raise = 0;
                weapon_aim_decrease = 1;
                weapon_aim_max = 30;
                per_shot_time = 1;
            end
            case 11: //minigun
                weapon_dmg = 6;
                weapon_ammo = 200;
                weapon_reload_time = 140;
                weapon_aim_raise = 2;
                weapon_aim_decrease = 1;
                weapon_aim_max = 50;
                per_shot_time = 0;
            end
        end
        if ( ammo > weapon_ammo )
            attack = 0;
            reload_time = 0;
            weapon_reload = 0;
            ammo = weapon_ammo;
        end
        if ( shot_time > 0 )
            shot_time--;
        end
        IF ( attack == 1 )
            IF ( ammo > 0 )
                if ( weapon > 0 and weapon < 5 ) //pistols+subs
                    if ( shot_time <= 0 )
                        if ( weapon_mode == 1 )
                            player_aim += weapon_aim_raise;
                            attack = 0;
                            shot_time = per_shot_time;
                            shot( graph1_x, graph1_y, z + 1, 0, angle1 + rand( - player_aim, player_aim ) * 500, weapon_dmg );
                            gilza( graph1_x, graph1_y, angle1 + 90000, 5, 1 );
                            muzzle( graph1_x, graph1_y, size, z -1, angle1, 0 );
                            if ( weapon == 1 )
                                sound_play( weapon1_sound, 0, 1 );
                            end
                            if ( weapon == 2 )
                                sound_play( weapon2_sound, 0, 1 );
                            end
                            if ( weapon == 3 )
                                sound_play( weapon3_sound, 0, 1 );
                            end
                            if ( weapon == 4 )
                                sound_play( weapon4_sound, 0, 1 );
                            end
                        else
                            if ( weapon == 1 )
                                sound_play( weapon1_sound, 0, 1 );
                            end
                            if ( weapon == 2 )
                                sound_play( weapon2_sound, 0, 1 );
                            end
                            if ( weapon == 3 )
                                sound_play( weapon3_sound, 0, 1 );
                            end
                            if ( weapon == 4 )
                                sound_play( weapon4_sound, 0, 1 );
                            end
                            player_aim += weapon_aim_raise * 2;
                            attack = 0;
                            shot_time = per_shot_time;
                            shot( graph1_x, graph1_y, z + 1, 0, angle1 + rand( - player_aim, player_aim ) * 500, weapon_dmg );
                            shot( graph2_x, graph2_y, z + 1, 0, angle2 + rand( - player_aim, player_aim ) * 500, weapon_dmg );
                            gilza( graph1_x, graph1_y, angle1 + 90000, 5, 1 );
                            gilza( graph2_x, graph2_y, angle2 -90000, 5, 1 );
                            muzzle( graph1_x, graph1_y, size, z -1, angle1, 0 );
                            muzzle( graph2_x, graph2_y, size, z -1, angle2, 0 );
                        end
                    end
                end
                if ( weapon == 5 or weapon == 6 ) //shotguns
                    if ( shot_time <= 0 )
                        if ( weapon == 5 )
                            sound_play( weapon5_sound, 0, 1 );
                        end
                        if ( weapon == 6 )
                            sound_play( weapon6_sound, 0, 1 );
                        end
                        player_aim += weapon_aim_raise;
                        attack = 0;
                        shot_time = per_shot_time;
                        gilza( graph1_x, graph1_y, angle1 + 90000, 5, 2 );
                        muzzle( graph1_x, graph1_y, size, z -1, angle1, 1 );
                        from b = 0 to 8;
                            shot( graph1_x, graph1_y, z + 1, 0, angle1 + rand( -10, 10 ) * 2000, weapon_dmg );
                        end
                    end
                end
                if ( weapon == 7 or weapon == 8 or weapon == 11 ) //machinguns
                    if ( shot_time <= 0 )
                        if ( weapon == 7 )
                            sound_play( weapon7_sound, 0, 1 );
                        end
                        if ( weapon == 8 )
                            sound_play( weapon8_sound, 0, 1 );
                        end
                        if ( weapon == 11 )
                            sound_play( weapon11_sound, 0, 1 );
                        end
                        player_aim += weapon_aim_raise;
                        attack = 0;
                        shot_time = per_shot_time;
                        shot( graph1_x, graph1_y, z + 1, 0, angle1 + rand( - player_aim, player_aim ) * 500, weapon_dmg );
                        gilza( graph1_x, graph1_y, angle1 + 90000, 5, 0 );
                        if ( weapon == 11 )
                            i4++;
                            if ( i4 > 1 )
                                muzzle( graph1_x, graph1_y, size, z -1, angle1, 1 );
                                i4 = 0;
                            end;
                        else
                            muzzle( graph1_x, graph1_y, size, z -1, angle1, 1 );
                        end
                    end
                end
                if ( weapon == 9 ) //grenade launcher
                    if ( shot_time <= 0 )
                        player_aim += weapon_aim_raise;
                        attack = 0;
                        shot_time = per_shot_time;
                        morter_shot( graph1_x, graph1_y, z + 1, 0, angle1, weapon_dmg );
                    end
                end
                if ( weapon == 10 ) //flamethrower
                    if ( shot_time <= 0 )
                        player_aim += weapon_aim_raise;
                        attack = 0;
                        shot_time = per_shot_time;
                        flame( x, y, angle1 );
                        flame_fired = 1;
                    end
                end
                ammo--;
            END
        END
        if ( ammo < 1 )
            ammo = 0;
            if ( reload_time == 0 )
                sound_play( empty_clip_sound, 0, 2 );
            end
            mouse.graph = 750 + ( reload_time * 100 / weapon_reload_time ) / 10;
            reload_time++;
            if ( reload_time => weapon_reload_time -1 )
                sound_play( reload_sound, 0, 2 );
            end
            if ( reload_time => weapon_reload_time )
                attack = 0;
                ammo = 255;
                mouse.graph = 0;
                reload_time = 0;
            end
        end
        //.........................last movement.............................//
        if ( x < 10 )
            x = 10;
        end
        if ( x > 390 )
            x = 390;
        end
        if ( y < 10 )
            y = 10;
        end
        if ( y > 290 )
            y = 290;
        end
        z = - y;
        id1.x = x;
        id1.y = y + 4;
        id1.z = z + 1;
        angle = fget_angle( x, y, mouse_x, mouse_y ) / 5000 * 5000;
        frame;
    end
end


process player_body()
begin
    ctype = c_scroll;
    file = player_file;
    graph = 10;
    loop
        if ( grass_walking_time < 1 )
            if ( grass_walking_sound == 1 )
                sound_play( grass1_sound, 0, 7 );
                grass_walking_sound = 0;
                grass_walking_time = 15;
            end
            if ( grass_walking_sound == 2 )
                sound_play( grass2_sound, 0, 7 );
                grass_walking_sound = 0;
                grass_walking_time = 15;
            end
            if ( grass_walking_sound == 3 )
                sound_play( grass3_sound, 0, 7 );
                grass_walking_sound = 0;
                grass_walking_time = 15;
            end
            if ( grass_walking_sound == 4 )
                sound_play( grass4_sound, 0, 7 );
                grass_walking_sound = 0;
                grass_walking_time = 15;
            end
        else
            grass_walking_time--;
        end
        //PC version
        if ( jkeys_state[ _JKEY_LEFT ] ) father.x -= player_speed;
            move = 1;
        end
        if ( jkeys_state[ _JKEY_RIGHT ] ) father.x += player_speed;
            move = 2;
        end
        if ( jkeys_state[ _JKEY_UP ] ) father.y -= player_speed;
            move = 3;
        end
        if ( jkeys_state[ _JKEY_DOWN ] ) father.y += player_speed;
            move = 4;
        end
        if ( !jkeys_state[ _JKEY_LEFT ] and !jkeys_state[ _JKEY_RIGHT ] and !jkeys_state[ _JKEY_UP ] and !jkeys_state[ _JKEY_DOWN ] ) move = 0;
        end
        if ( move > 0 )
            if ( grass_walking_sound == 0 )
                grass_walking_sound = rand( 1, 4 );
            end
            if ( last_x > x and last_y > y )
                angle = 315000;
            end
            if ( last_x > x and last_y == y )
                angle = 0;
            end
            if ( last_x > x and last_y < y )
                angle = 45000;
            end
            if ( last_x == x and last_y < y )
                angle = 90000;
            end
            if ( last_x < x and last_y < y )
                angle = 135000;
            end
            if ( last_x < x and last_y == y )
                angle = 180000;
            end
            if ( last_x < x and last_y > y )
                angle = 225000;
            end
            if ( last_x == x and last_y > y )
                angle = 270000;
            end
            anim++;
            if ( anim > 2 )
                graph++;
                anim = 0;
            end
            if ( graph > 17 )
                graph = 10;
            end
        else
            graph = 10;
        end
        last_x = x;
        last_y = y;
        m_last_x = mouse_x;
        m_last_y = mouse_y;
        frame;
    end
end


process temp_gfx( x, y, z, flags, size, alpha, file, graph, angle )
begin
    ctype = c_scroll;
    loop
        frame;
    end
end


process shot( double x, y, int z, shot_type, tangle, dmg )
    private
        int pid;
begin
    ctype = c_scroll;
    angle = tangle;
    flags = 16;
    hp = 1;
    graph = 22 + shot_type;
    loop
        z = - y;
        x += get_distx( tangle, 8 );
        y += get_disty( tangle, 8 );
        while ( pid = get_id( type zombie ))
            if ( fget_dist( x, y, pid.x, pid.y ) < 15 )
                map_xput( 0, background_map_c2, rand( 360, 361 ), x, y, angle + rand( -90000, 90000 ), size, 0 );
                blood( x, y, rand( 40, 60 ));
                pid.I_got_hit = 1;
                pid.hp -= dmg;
                hp = 0;
                zombie_sound_time -= dmg;
                break;
            end
        end
        if ( level == 12 )
            while ( pid = get_id( type final_boss ))
                if ( fget_dist( x, y, pid.x, pid.y ) < 25 )
                    map_xput( 0, background_map_c2, rand( 360, 361 ), x, y, angle + rand( -90000, 90000 ), size, 0 );
                    blood( x, y, rand( 40, 60 ));
                    pid.I_got_hit = 1;
                    final_boss_hp -= dmg;
                    hp = 0;
                    zombie_sound_time -= dmg;
                    break;
                end
            end
        end
        if ( hp == 0 or region_out( id, 0 ))
            break;
        end
        frame( 20 );
    end
end


process zombie_shot( double x, y, int z, shot_type, tangle, dmg )
    private
        int pid;
begin
    ctype = c_scroll;
    angle = tangle;
    flags = 16;
    hp = 1;
    graph = 22 + shot_type;
    loop
        z = - y;
        x += get_distx( tangle, 8 );
        y += get_disty( tangle, 8 );
        if ( player_hp > 0 )
            if ( get_dist( player_id ) < 15 )
                map_xput( 0, background_map_c2, rand( 360, 361 ), x, y, angle + rand( -90000, 90000 ), size, 0 );
                blood( x, y, rand( 40, 60 ));
                player_hp -= dmg;
                hp = 0;
                blooddrain_time = 0;
                break;
            end
        end
        if ( hp == 0 or region_out( id, 0 ))
            break;
        end
        frame( 40 );
    end
end


process morter_shot( double x, y, int z, shot_type, tangle, dmg )
    private
        int pid;
begin
    ctype = c_scroll;
    angle = tangle;
    flags = 16;
    hp = 1;
    loop
        z = - y;
        if ( shot_type == 0 )
            graph = 22;
        end
        x += get_distx( tangle, 8 );
        y += get_disty( tangle, 8 );
        while ( pid = get_id( type zombie ))
            if ( fget_dist( x, y, pid.x, pid.y ) < 15 )
                from i = 0 to 16;
                    fake_flame( x, y, i * 25000, 6 );
                end
                morter_tim = 1;
                morter_x = x;
                morter_y = y;
                morter_dmg = dmg;
                hp = 0;
            end
        end
        if ( level == 12 )
            while ( pid = get_id( type final_boss ))
                if ( fget_dist( x, y, pid.x, pid.y ) < 15 )
                    from i = 0 to 16;
                        fake_flame( x, y, i * 25000, 6 );
                    end
                    morter_tim = 1;
                    morter_x = x;
                    morter_y = y;
                    morter_dmg = dmg;
                    hp = 0;
                end
            end
        end
        if ( hp == 0 )
            i = rand( 1, 3 );
            if ( i == 1 )
                sound_play( explode1_sound, 0, 1 );
            end
            if ( i == 2 )
                sound_play( explode2_sound, 0, 1 );
            end
            if ( i == 3 )
                sound_play( explode3_sound, 0, 1 );
            end
            break;
        end
        if ( region_out( id, 0 ))
            break;
        end
        frame( 40 );
    end
end


process gilza( double x, y, int tangle, speed, gilza_type );
    private
        int mid_speed;
begin
    ctype = c_scroll;
    graph = rand( 25, 32 ) + ( gilza_type * 8 );
    x += rand( -1, 1 );
    y += rand( -1, 1 );
    loop
        z = - y;
        i++;
        if ( i > 2 )
            i2++;
            speed--;
            i = 0;
        end
        anim++;
        if ( anim > i2 )
            graph++;
            anim = 0;
            i2--;
        end
        if ( graph > 32 + ( gilza_type * 8 ))
            graph = 25 + ( gilza_type * 8 );
        end
        if ( speed > 2 )
            x += get_distx( tangle, speed );
            y += get_disty( tangle, speed );
        end
        if ( region_out( id, 0 ))
            break;
        end
        if ( speed < 2 )
            map_put( 0, background_map_c, 0, graph, x + rand( -1, 1 ), y + rand( -1, 1 ));
            break;
        end
        frame;
    end
end


process flame( x, y, tangle )
    private
        int pid;
begin
    ctype = c_scroll;
    graph = 108;
    flags = 16;
    angle = rand( 0, 360000 );
    x += get_distx( tangle, 20 );
    y += get_disty( tangle, 20 );
    ix = get_distx( tangle, 8 );
    iy = get_disty( tangle, 8 );
    size = 25;
    red_flame( x, y, tangle, 8 );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            if ( graph < 111 )
                while ( pid = get_id( type zombie ))
                    if ( fget_dist( x, y, pid.x, pid.y ) < 10 )
                        pid.on_fire = 1;
                        pid.hp -= 1;
                        break;
                    end
                end
            end
            if ( level == 12 )
                while ( pid = get_id( type final_boss ))
                    if ( fget_dist( x, y, pid.x, pid.y ) < 25 )
                        final_boss_hp -= dmg;
                        break;
                    end
                end
            end
            if ( i == 0 )
                i = rand( 0, 1 );
            end
            if ( i == 1 )
                after_flame( x + rand( -15, 15 ), y + rand( -5, 5 ), 50 );
                i = -1;
            end
            graph++;
            anim = 0;
        end
        if ( graph > 115 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process fake_flame( x, y, tangle, speed )
begin
    ctype = c_scroll;
    graph = 108;
    flags = 16;
    angle = rand( 0, 360000 );
    size = 25;
    fake_red_flame( x, y, tangle, speed );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            if ( i == 0 )
                i = rand( 0, 1 );
            end
            if ( i == 1 )
                after_flame( x + rand( -15, 15 ), y + rand( -5, 5 ), 50 );
                i = -1;
            end
            graph++;
            anim = 0;
        end
        if ( graph > 115 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process blood( double x, y, size )
begin
    ctype = c_scroll;
    graph = 100;
    angle = rand( 0, 360000 );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 1 )
            graph++;
            anim = 0;
        end
        if ( graph > 107 )
            break;
        end
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -2;
        frame;
    end
end


process red_flame( x, y, tangle, speed )
begin
    ctype = c_scroll;
    graph = 100;
    flags = 16;
    angle = rand( 0, 360000 );
    size = rand( 10, 25 );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 107 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process fake_red_flame( x, y, tangle, speed )
begin
    ctype = c_scroll;
    graph = 100;
    flags = 16;
    angle = rand( 0, 360000 );
    size = rand( 10, 25 );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 107 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process after_flame( double x, y, size );
begin
    ctype = c_scroll;
    graph = 124;
    flags = 16;
    loop
        angle += 5000;
        y -= 4;
        alpha -= 10;
        anim++;
        z = - y;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 131 or alpha < 100 )
            break;
        end
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -2;
        frame;
    end
end


process dirt( x, y, tangle, speed )
begin
    ctype = c_scroll;
    graph = 132;
    angle = rand( 0, 360000 );
    size = rand( 10, 25 );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 139 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process gas_shot( x, y, tangle, dmg )
    private
        int pid;
begin
    ctype = c_scroll;
    angle = tangle;
    flags = 16;
    hp = 1;
    graph = 116;
    size = 50;
    loop
        z = - y;
        angle += 5000;
        x += get_distx( tangle, 8 );
        y += get_disty( tangle, 8 );
        i++;
        if ( i > 2 )
            gas( x, y, angle, 0 );
            i = 0;
        end
        if ( player_hp > 0 )
            if ( get_dist( player_id ) < 25 )
                player_hp -= dmg;
                hp = 0;
                blooddrain_time = 0;
                gas_attack = 1;
                break;
            end
        end
        if ( hp == 0 or x < 0 or x > 400 or y < 0 or y > 300 )
            break;
        end
        frame( 50 );
    end
end


process gas( x, y, tangle, speed )
begin
    ctype = c_scroll;
    graph = 116;
    angle = rand( 0, 360000 );
    size = rand( 10, 25 );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 123 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process muzzle( double x, y, size, int z, angle, dir );
begin
    ctype = c_scroll;
    graph = 365 + dir;
    flags = 16;
    frame;
end


process death_menu_gfx( double x, y, int z, double size, int alpha, graph );
begin
    loop
        frame;
    end
end


process fake_flame_scroll( double x, y, int tangle, speed )
begin
    graph = 108;
    flags = 16;
    angle = rand( 0, 360000 );
    size = 25;
    fake_red_flame_scroll( x, y, tangle, speed );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 115 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process fake_red_flame_scroll( x, y, tangle, speed )
begin
    graph = 100;
    flags = 16;
    angle = rand( 0, 360000 );
    size = rand( 10, 25 );
    ix = get_distx( tangle, speed );
    iy = get_disty( tangle, speed );
    loop
        anim++;
        if ( size < 100 )
            size += 10;
        end
        angle += 5000;
        if ( anim > 2 )
            graph++;
            anim = 0;
        end
        if ( graph > 107 )
            break;
        end
        x += ix;
        y += iy;
        if ( region_out( id, 0 ))
            break;
        end
        z = - y -3;
        frame;
    end
end


process letters( double x, y, int graph, double size )
begin
    loop
        if ( size > 100 )
            size -= 10;
        end
        frame;
    end
end


process volume_refresher()
begin
    if ( weapons_volume < 0 )
        weapons_volume = 0;
    end
    if ( weapons_volume > 128 )
        weapons_volume = 128;
    end
    sound_set_volume( explode1_sound, weapons_volume );
    sound_set_volume( explode2_sound, weapons_volume );
    sound_set_volume( explode3_sound, weapons_volume );
    sound_set_volume( weapon1_sound, weapons_volume );
    sound_set_volume( weapon2_sound, weapons_volume );
    sound_set_volume( weapon3_sound, weapons_volume );
    sound_set_volume( weapon4_sound, weapons_volume );
    sound_set_volume( weapon5_sound, weapons_volume );
    sound_set_volume( weapon6_sound, weapons_volume );
    sound_set_volume( weapon7_sound, weapons_volume );
    sound_set_volume( weapon8_sound, weapons_volume );
    sound_set_volume( weapon11_sound, weapons_volume );
    sound_set_volume( empty_clip_sound, weapons_volume );
    sound_set_volume( reload_sound, weapons_volume );
    sound_set_volume( zombie1_sound, weapons_volume );
    sound_set_volume( zombie2_sound, weapons_volume );
    sound_set_volume( zombie3_sound, weapons_volume );
    sound_set_volume( zombie4_sound, weapons_volume );
    sound_set_volume( zombie5_sound, weapons_volume );
    sound_set_volume( zombie6_sound, weapons_volume );
    sound_set_volume( zombie7_sound, weapons_volume );
    sound_set_volume( zombie8_sound, weapons_volume );
    sound_set_volume( zombie9_sound, weapons_volume );
    sound_set_volume( zombie10_sound, weapons_volume );
    sound_set_volume( zombie11_sound, weapons_volume );
    sound_set_volume( zombie12_sound, weapons_volume );
    sound_set_volume( zombie13_sound, weapons_volume );
    sound_set_volume( zombie14_sound, weapons_volume );
    sound_set_volume( fire_sound, weapons_volume );
    sound_set_volume( scream1_sound, weapons_volume );
    sound_set_volume( scream2_sound, weapons_volume );
    sound_set_volume( line1_sound, weapons_volume );
    sound_set_volume( line2_sound, weapons_volume );
    sound_set_volume( line3_sound, weapons_volume );
    sound_set_volume( line4_sound, weapons_volume );
    sound_set_volume( line5_sound, weapons_volume );
    sound_set_volume( line6_sound, weapons_volume );
    sound_set_volume( line7_sound, weapons_volume );
    sound_set_volume( line8_sound, weapons_volume );
    sound_set_volume( line9_sound, weapons_volume );
    sound_set_volume( intro_sound, weapons_volume );
    sound_set_volume( menu_sound, weapons_volume );
    sound_set_volume( gothit1_sound, weapons_volume );
    sound_set_volume( gothit2_sound, weapons_volume );
    sound_set_volume( gothit3_sound, weapons_volume );
    sound_set_volume( finalboss_sound, weapons_volume );
    music_set_volume( weapons_volume );
    sound_set_volume( grass1_sound, weapons_volume );
    sound_set_volume( grass2_sound, weapons_volume );
    sound_set_volume( grass3_sound, weapons_volume );
    sound_set_volume( grass4_sound, weapons_volume );
    frame;
end
