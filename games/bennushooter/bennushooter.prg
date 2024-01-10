 // Simple shoot'em up
// John Buscher 1/23/2010-2/8/2010
// A side scrolling shooter with code borrowed from
// Squarepushes Shoot'em up tutorial(movement code and starting code)
import "mod_gfx";
import "mod_input";
import "mod_sound";
import "mod_misc";
import "mod_debug";

#define init_bgd1_background_emulation() background.file = 0; background.graph = map_new( 320, 240 )
#define put_screen( f, g ) map_clear( 0, background.graph ); map_put( 0, background.graph, f, g, 160, 120 )
#define clear_screen() map_clear( 0, background.graph )
#define put( f, g, x, y ) map_put( 0, background.graph, f, g, x, y )
#define map_xput( fdst, gdst, gsrc, x, y, angle, size, flags ) map_put( fdst, gdst, fdst, gsrc, x, y, angle, size, size, flags, 255, 255, 255, 255 )
#define xput(f,g,x,y,angle,size,flags,region) map_put(0, background.graph, f, g, x, y, angle, size, size, flags, 255, 255, 255, 255)
#define set_text_color(color) rgba_get(color, &text.color_r,  &text.color_g,  &text.color_b, &text.alpha);

#include "jkey.lib"

Global
    int shootemGfx;
    int ship_id;
    int lives = 3;
    int waves[ 5 ];
    int my_text[ 3 ]; //Misc display text
    String pause_text[ 2 ]; //text choices for the pause menu
    int _level;
    int lastShot = -300;
    int score = 0;
    int highScore;
    int act;
    int respawnTime;
    int EndGame = 0;
    int world;
    int bgd_logo;
End


Process Main()
Begin
    jkeys_set_default_keys();
    jkeys_controller();
    SetupGeneral();
    Loop
        world = 0;
        _level = 0;
        lives = 3;
        score = 0;
        EndGame = 0;
        act = 0;
        TitleScreen();
        If (( jkeys_state[ _JKEY_MENU ] ) || ( wm_info.exit_status ))
            exit();
        End
        Game();
        Repeat
            if ( jkeys_state[ _JKEY_SELECT ] )
                PauseGame();
                if ( exists( type PauseGame ))
                    repeat
                        frame;
                    until ( !exists( type PauseGame )) //continues until killed in itself
                end
            end
            if ( lives >= 0 )
                my_text[ 0 ] = write( 0, 140, 5, 4, "lives: " + lives );
                my_text[ 1 ] = write( 0, 5, 0, 0, "Score: " + score );
                frame( 600 );
            else
                my_text[ 0 ] = GameOver();
                let_me_alone();
                jkeys_controller();
                frame( 24000 );
                break;
            end
            write_delete( my_text[ 0 ] );
            write_delete( my_text[ 1 ] );
        Until (( jkeys_state[ _JKEY_MENU ] ) || EndGame == 1 )
        let_me_alone();
        jkeys_controller();
        write_delete( all_text );
    End
End


function SetupGeneral()
Begin
    set_mode( 320, 240, mode_waitvsync );

    set_fps( 60, 0 );

    init_bgd1_background_emulation();

    region_define( 1, 0, 0, 320, 240 );
    waves[ 0 ] = sound_load( "die.wav" );
    waves[ 1 ] = sound_load( "die2.wav" );
    waves[ 2 ] = sound_load( "hit.wav" );
    waves[ 3 ] = sound_load( "movesel.wav" );
    waves[ 4 ] = sound_load( "shoot.wav" );
    waves[ 5 ] = sound_load( "shootlazer.wav" );
    shootemGfx = fpg_load( "testing.fpg" );
    bgd_logo = map_load("bennugd.png");

    window_set_icon( shootemGfx, 1 );
    window_set_title( "Simple BennuGD Shooter" );
    /*scale_mode = scale_normal2x;*/

    xput( file, bgd_logo, 160, 120, 0, 100, 0, region );

    frame( 6000 );

    clear_screen();

end


Process SetupScreen( int levelNum )
Private
    int f;
Begin
    clear_screen();
    scroll_start( 1, shootemGfx, 50 + levelNum, 0, 1, 1 );
    scroll[ 1 ].speed = 1;
    Loop
        For ( f = 1; f < 5; f = f + 1 )
            frame;
        End
        scroll[ 1 ].x0 = scroll[ 1 ].x0 + 1;
    End
End


Process VolControl()
private
    int vol = 128;
    int c;
Begin
    loop
        c = 0;
        if ( jkeys_state[ _JKEY_VOLUP ] )
            if ( vol == 128 )
                vol = 128;
            else
                vol += 16;
            end
        elseif ( jkeys_state[ _JKEY_VOLDOWN ] )
            if ( vol == 0 )
                vol = 0;
            else
                vol -= 16;
            end
        end
        music_set_volume( vol );
        repeat
            sound_set_volume( waves[ x ], vol );
            c++;
        until ( x >= 5 )
        frame;
    end
end


function TitleScreen()
Begin
    region = 1;

    set_text_color( rgb( 255, 0, 0 ));
    my_text[ 0 ] = write( 0, 160, 100, 4, "Play" );
    set_text_color( rgb( 255, 255, 255 ));
    my_text[ 1 ] = write( 0, 160, 120, 4, "Exit" );
    repeat
        frame;
    until ( !jkeys_state[ _JKEY_SELECT ] && !jkeys_state[ _JKEY_A ] && !jkeys_state[ _JKEY_B ] )
    act = 1;
    repeat
        if ( jkeys_state[ _JKEY_UP ] )
            sound_play( waves[ 3 ], 0 );
            write_set_rgba( my_text[ 0 ], 255, 0, 0, 255 );
            write_set_rgba( my_text[ 1 ], 255, 255, 255, 255 );
            act = 1;
        elseif ( jkeys_state[ _JKEY_DOWN ] )
            sound_play( waves[ 3 ], 0 );
            write_set_rgba( my_text[ 0 ], 255, 255, 255, 255 );
            write_set_rgba( my_text[ 1 ], 255, 0, 0, 255 );
            act = 2;
        end
        frame;
    until ( jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_A ] || jkeys_state[ _JKEY_B ] );
    write_delete( my_text[ 0 ] );
    write_delete( my_text[ 1 ] );
    if ( act == 2 )
        exit();
    end
End


Process PauseGame()
private
    int choice = 1;
    int lastPress = 0;
Begin
    signal( type Game, s_freeze_tree ); //pauses game
    signal( type SetupScreen, s_sleep ); //stops the scroll created
    set_text_color( rgb( 255, 0, 0 )); //writes the simple pause menu, starting with Paused in red
    pause_text[ 0 ] = write( 0, 160, 50, 4, "Paused" );
    set_text_color( rgb( 0, 255, 0 )); //turns text green
    pause_text[ 1 ] = write( 0, 160, 80, 4, "Continue" );
    set_text_color( rgb( 255, 255, 255 )); //returns text to white
    pause_text[ 2 ] = write( 0, 160, 100, 4, "Exit" ); //end write
    while ( jkeys_state[ _JKEY_SELECT ] ) frame; end //repeats until you release the enter/select key
    repeat
        if ( jkeys_state[ _JKEY_UP ] )
            choice = 1;
            sound_play( waves[ 3 ], 0 );
            write_set_rgba( pause_text[ 1 ], 0, 255, 0, 255 );
            write_set_rgba( pause_text[ 2 ], 255, 255, 255, 255 );
        elseif ( jkeys_state[ _JKEY_DOWN ] )
            choice = 2;
            sound_play( waves[ 3 ], 0 );
            write_set_rgba( pause_text[ 1 ], 255, 255, 255, 255 );
            write_set_rgba( pause_text[ 2 ], 0, 255, 0, 255 );
        end
        frame;
    until ( jkeys_state[ _JKEY_SELECT ] || jkeys_state[ _JKEY_A ] || jkeys_state[ _JKEY_B ] ) //repeats until you push the enter/select key
    while ( jkeys_state[ _JKEY_SELECT ] ) frame; end //repeats until you release the enter/select key
    write_delete( pause_text[ 0 ] );
    write_delete( pause_text[ 1 ] );
    write_delete( pause_text[ 2 ] );
    signal( type Game, s_wakeup_tree ); //resumes every process
    signal( type SetupScreen, s_wakeup ); //that was stopped
    if ( choice == 2 ) //follows through which action you chose
        exit();
    end
end


Process Game()
private
    int count;
Begin
    ship_id = Ship();
    count = 1;
    while ( _level < 6 )
        SetupScreen( _level / 2 );
        Level( _level );
        repeat
            frame;
        until ( _level == count )
        signal( type SetupScreen, s_kill );
        count += 1;
    end
    GameEnd();
End


Process Ship();
Private
    int xm;
    int ym;
Begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 1;
    x = 200;
    y = 1060;
    frame;
    Loop
        If (( jkeys_state[ _JKEY_B ] ) || ( jkeys_state[ _JKEY_A ] ))
            if ( get_timer() - lastShot > 300 )
                Shoot();
                lastShot = get_timer();
            end
        elseif ( !( jkeys_state[ _JKEY_B ] ) || ( jkeys_state[ _JKEY_A ] ))
            lastShot = -300;
        else
        end
        If (( jkeys_state[ _JKEY_RIGHT ] ) || ( jkeys_state[ _JKEY_LEFT ] ))
            If ( jkeys_state[ _JKEY_RIGHT ] )
                If ( !jkeys_state[ _JKEY_LEFT ] )
                    xm = xm + 6;
                end
            end
            If ( jkeys_state[ _JKEY_LEFT ] )
                If ( !jkeys_state[ _JKEY_RIGHT ] )
                    xm = xm - 6;
                end
            end
            If ( xm > 18 )
                xm = 18;
            end
            If ( xm < -18 )
                xm = -18;
            end
        else
            If ( xm > 0 )
                xm = xm - 2;
            end
            If ( xm < 0 )
                xm = xm + 2;
            end
        end
        If (( jkeys_state[ _JKEY_UP ] ) || ( jkeys_state[ _JKEY_DOWN ] ))
            If ( jkeys_state[ _JKEY_UP ] )
                If ( !jkeys_state[ _JKEY_DOWN ] )
                    ym = ym - 6;
                End
            End
            If ( jkeys_state[ _JKEY_DOWN ] )
                If ( !jkeys_state[ _JKEY_UP ] )
                    ym = ym + 6;
                End
            End
            If ( ym > 18 )
                ym = 18;
            End
            If ( ym < -18 )
                ym = -18;
            End
        else
            If ( ym > 0 )
                ym = ym - 2;
            End
            If ( ym < 0 )
                ym = ym + 2;
            End
        End
        If ( y > 1900 )
            y = 1900;
        End
        If ( y < 140 )
            y = 140;
        End
        If ( x > 3000 )
            x = 3000;
        End
        If ( x < 200 )
            x = 200;
        End
        x = x + xm;
        y = y + ym;
        frame;
    End
End


process Explode();
Begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 3;
    x = ship_id.x;
    y = ship_id.y;
    signal( ship_id, s_sleep );
    sound_play( waves[ 1 ], 0 );
    repeat
        graph += 1;
        frame( 600 );
    until ( graph == 7 )
    signal( ship_id, s_wakeup );
    ship_id.x = 200;
    ship_id.y = 1060;
    respawnTime = get_timer();
    lives -= 1;
    frame;
end


process Shoot();
Begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 4;
    x = ship_id.x;
    y = ship_id.y;
    sound_play( waves[ 4 ], 0 );
    repeat
        x += 25;
        if ( collision( type Enemy ) || collision( type Boss ))
            frame;
            x = 10000;
        end
        frame;
    until ( region_out( id, region ));
    signal( id, s_kill );
end


Process enemyFire()
private
    float xc;
    float yc;
begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 4;
    x = father.x;
    y = father.y;
    yc = ( ship_id.y - y );
    xc = ( ship_id.x - x );
    sound_play( waves[ 4 ], 0 );
    if ( xc > 5 || yc > 5 )
        yc = ( yc / xc ) * -7;
        xc = 7;
    elseif ( yc > 20 && yc <= 50 )
        yc = ( yc / xc ) * -3;
        xc = 3;
    elseif ( yc > 50 )
        yc = yc / xc;
        xc = 1;
    elseif ( yc < -10 )
        yc = ( yc / xc ) * 7;
        xc = 7;
    elseif ( yc < -20 && yc >= -50 )
        yc = ( yc / xc ) * 3;
        xc = 3;
    elseif ( yc < -50 )
        yc = yc / xc;
        xc = 1;
    end
    if ( ship_id.y >= y )
        yc *= -1;
    end
    repeat
        x -= xc;
        y -= yc;
        if ( collision( ship_id ))
            if ( get_timer() - respawnTime > 2000 )
                Explode();
                frame;
                x = 10000;
            end
        end
        frame;
    until ( region_out( id, region ));
    signal( id, s_kill );
end


Process betterEnemyFire()
Begin
    resolution = 10;
    file = shootemGfx;
    graph = 4;
    region = 1;
    x = father.x;
    y = father.y;
    angle = fget_angle( ship_id.x, ship_id.y, father.x, father.y );
    sound_play( waves[ 4 ], 0 );
    repeat
        advance( -19 );
        if ( collision( ship_id ))
            Explode();
            signal( id, s_kill );
        end
        frame;
    until ( region_out( id, region ))
    signal( id, s_kill );
end


Process Enemy( int xpos, int ypos, int enemyNum );
private
    int timeShot = 0;
    int move = 7;
    int num = 0;
Begin
    resolution = 10;
    flags = B_HMIRROR;
    file = shootemGfx;
    region = 1;
    graph = 2;
    x = xpos;
    y = ypos;
    xput( file, graph, x, y, 0, 100, flags, region );
    loop
        num += 1;
        if ( collision( ship_id ))
            Explode();
            Death();
        end
        if ( collision( type Shoot ))
            score += enemyNum * 100;
            Death();
        end
        if ( enemyNum != 4 )
            x -= 25;
        end
        if ( enemyNum == 2 )
            y += move;
            if ( num <= 50 )
                move *= -1;
            elseif ( num > 50 )
                move *= -1;
            end
        end
        if ( enemyNum == 3 )
            if (( fget_dist( ship_id.x, ship_id.y, id.x, id.y ) < 1500 ) && ( get_timer() - timeShot > 2000 ))
                betterEnemyFire();
                timeShot = get_timer();
            end
        end
        if ( enemyNum == 4 )
            repeat
                x -= 30;
            until ( x <= 3000 )
            repeat
                if ( get_timer() - timeShot > 1500 )
                    betterEnemyFire();
                    timeShot = get_timer();
                end
            until ( collision( type Shoot ))
            Death();
        end
        if ( enemyNum == 5 )
            x += 20;
            if ( y > ship_id.y )
                y -= 7;
            elseif ( y < ship_id )
                y += 7;
            else
                x += 15;
            end
        end
        if ( enemyNum == 6 )
            if (( fget_dist( ship_id.x, ship_id.y, id.x, id.y ) < 1500 ) && ( get_timer() - timeShot > 2000 ))
                betterEnemyFire();
                timeShot = get_timer();
            end
        end
        if ( region_out( id, region ))
            signal( id, s_kill );
        end
        frame;
    end
end


process Death();
Begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 3;
    x = father.x;
    y = father.y;
    signal( father, s_sleep );
    sound_play( waves[ 0 ], 0 );
    repeat
        graph += 1;
        frame( 600 );
    until ( graph == 7 )
    father.x = -10000;
    signal( father, s_kill );
    frame;
end


Process GameOver()
private
    String thing = "Game Over"; //String to be written
Begin
    write_delete( all_text );
    write( 0, 160, 220, 4, thing );
    highScore = score;
end


Process Level( int levelNum )
private
    int i = 0;
    int j = 0;
    int k = 0;
Begin
    my_text[ 2 ] = write( 0, 160, 140, 4, "Level " + ( levelNum + 1 ));
    frame( 24000 );
    write_delete( my_text[ 2 ] );
    if ( levelNum == 0 )
        repeat
            j += 1;
            Enemy( 3300, 400, 1 );
            frame( 1000 );
            Enemy( 3300, 1000, 1 );
            frame( 1000 );
            Enemy( 3300, 1600, 1 );
            frame( 30000 );
        until ( j > 3 )
        repeat
            k += 1;
            Enemy( 3300, 400, 1 );
            frame( 1000 );
            Enemy( 3300, 1000, 1 );
            frame( 1000 );
            Enemy( 3300, 1600, 1 );
            frame( 30000 );
        until ( k > 3 )
        frame( 6000 );
    elseif ( levelNum == 1 )
        repeat
            j = 0;
            k = 0;
            while ( k < 3 )
                Enemy( 3300, 1000, 1 );
                frame( 1000 );
                Enemy( 3300, 400, 1 );
                frame( 1000 );
                Enemy( 3300, 1600, 1 );
                frame( 1000 );
                Enemy( 3300, 200, 2 );
                frame( 1000 );
                Enemy( 3300, 1800, 2 );
                frame( 24000 );
                k += 1;
            end
            while ( j < 3 )
                Enemy( 3300, 1000, 2 );
                frame( 1000 );
                Enemy( 3300, 600, 2 );
                frame( 1000 );
                Enemy( 3300, 1400, 2 );
                frame( 1000 );
                Enemy( 3300, 200, 1 );
                frame( 1000 );
                Enemy( 3300, 1800, 1 );
                frame( 24000 );
                j += 1;
            end
            i += 1;
        until ( i > 3 )
        frame( 6000 );
    elseif ( levelNum == 2 )
        while ( i < 5 )
            k = 0;
            j = 0;
            repeat
                Enemy( 3300, 1400, 3 );
                frame( 1200 );
                Enemy( 3300, 1000, 3 );
                Enemy( 3300, 600, 3 );
                frame( 1200 );
                Enemy( 3300, 1700, 2 );
                Enemy( 3300, 300, 2 );
                frame( 24000 );
                j += 1;
            until ( j > 1 )
            repeat
                Enemy( 3300, 1400, 2 );
                frame( 1200 );
                Enemy( 3300, 1000, 2 );
                Enemy( 3300, 600, 2 );
                frame( 1200 );
                Enemy( 3300, 300, 1 );
                Enemy( 3300, 1700, 1 );
                frame( 24000 );
                k += 1;
            until ( k > 2 )
            i += 1;
        end
    elseif ( levelNum == 3 )
        while ( i < 6 )
            k = 0;
            j = 0;
            repeat
                Enemy( 3300, 1000, 3 );
                frame( 1200 );
                Enemy( 3300, 1400, 3 );
                Enemy( 3300, 600, 3 );
                frame( 1200 );
                Enemy( 3300, 300, 3 );
                Enemy( 3300, 1700, 3 );
                frame( 24000 );
                k++;
            until ( k > 3 )
            Enemy( 3300, 1, 4 );
            Enemy( 3300, 2000, 4 );
            i++;
            repeat
                Enemy( 3300, 1000, 3 );
                frame( 1200 );
                Enemy( 3300, 600, 2 );
                Enemy( 3300, 1400, 2 );
                frame( 1200 );
                j++;
            until ( j > 3 )
        end
    elseif ( levelNum == 4 )
        while ( i < 7 )
            Enemy( 3300, 1060, 4 );
            Enemy( 3300, 1400, 4 );
            Enemy( 3300, 600, 4 );
            frame( 1200 );
            repeat
                Enemy( 3300, 800, 3 );
                Enemy( 3300, 1200, 3 );
                frame( 10000 );
            until ( !exists( type Enemy ))
            i++;
            Enemy( 3300, 1060, 4 );
            Enemy( 3300, 1400, 4 );
            Enemy( 3300, 600, 4 );
            frame( 1200 );
            repeat
                Enemy( 3300, 900, 3 );
                Enemy( 3300, 1100, 1 );
                Enemy( 3300, 800, 1 );
                Enemy( 3300, 700, 1 );
                Enemy( 3300, 1200, 1 );
                Enemy( 3300, 1300, 3 );
                frame( 10000 );
            until ( !exists( type Enemy ))
        end
    elseif ( levelNum == 5 )
        while ( i < 8 )
            Enemy( 3300, 1060, 4 );
            Enemy( 3300, 800, 5 );
            Enemy( 3300, 1200, 5 );
            frame;
            repeat
                Enemy( 3300, 1400, 3 );
                Enemy( 3300, 600, 3 );
                Enemy( 3300, 400, 3 );
                Enemy( 3300, 1600, 3 );
                frame( 10000 );
            until ( !exists( type Enemy ))
            i++;
            k = 0;
            repeat
                Enemy( 3300, rand( 140, 1700 ), 3 );
                Enemy( 3300, rand( 140, 1700 ), 4 );
                Enemy( 3300, rand( 140, 1700 ), 3 );
                frame( 10000 );
                Enemy( 3300, rand( 140, 1700 ), 4 );
                Enemy( 3300, rand( 140, 1700 ), 3 );
                Enemy( 3300, rand( 140, 1700 ), 4 );
                frame( 10000 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                Enemy( 3300, rand( 140, 1700 ), 4 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                k++;
            until ( k > 3 )
        end
    elseif ( levelNum == 6 )
        while ( i < 7 )
            Enemy( 3300, 1060, 4 );
            Enemy( 3300, 800, 4 );
            Enemy( 3300, 1200, 4 );
            frame;
            repeat
                Enemy( 3300, rand( 140, 1700 ), 5 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                frame( 10000 );
            until ( !exists( type Enemy ))
            Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
            Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
            Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
            frame;
            repeat
                Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
                Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
                Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
                Enemy( 3300, rand( 140, 1700 ), 6 );
                Enemy( 3300, rand( 140, 1700 ), 6 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                Enemy( 3300, rand( 140, 1700 ), 5 );
                frame( 10000 );
                k++;
            until ( k > 5 )
        end
    elseif ( levelNum == 7 )
        repeat
            k++;
            Enemy( 3300, rand( 140, 1700 ), rand( 1, 6 ));
        until ( k > 34 )
        k = 0;
        repeat
            k++;
            Enemy( 3300, rand( 140, 1700 ), rand( 4, 6 ));
        until ( k > 20 )
    end
    my_text[ 2 ] = write( 0, 200, 0, 0, "Boss time!" );
    Boss( levelNum );
    repeat
        frame;
    until ( !exists( type Boss ))
    levelEnd( levelNum );
    _level += 1;
end


Process Boss( int bossNum );
private
    int health;
    int move = 5;
    int timeShot = -1000;
    int counter = 0;
Begin
    resolution = 10;
    file = shootemGfx;
    region = 1;
    graph = 8;
    flags = B_HMIRROR;
    xput( file, graph, x, y, 0, 100, flags, region );
    x = 3300;
    y = 1000;
    health = ( bossNum + 1 ) * 15;
    repeat
        while ( x > 3000 )
            x -= move;
            frame;
        end
        if ( bossNum == 0 )
            y += move;
            if ( y <= 200 )
                move = move * -1;
            elseif ( y >= 1950 );
                move = move * -1;
            end
        elseif ( bossNum == 1 )
            y += move;
            if ( y <= 200 )
                move = move * -1;
            elseif ( y >= 2000 );
                move = move * -1;
            end
        elseif ( bossNum == 2 )
            move = rand( -6, 6 );
            y += move;
            if ( y >= 1900 )
                move *= -1;
            elseif ( y <= 200 )
                move *= -1;
            end
        elseif ( bossNum == 3 )
            if ( id.y > ship_id.x )
                y -= move;
            else
                y += move;
            end
        elseif ( bossNum == 4 )
            if ( y > ship_id.y )
                if ( y <= 60 )
                    y = 1999;
                else
                    y -= move;
                end
            else
                if ( y >= 2000 )
                    y = 70;
                else
                    y += move;
                end
            end
        elseif ( bossNum == 5 )
            y += move;
            if ( y >= 2000 )
                y = 80;
                move *= -1;
            elseif ( y <= 79 )
                y = 1999;
                move *= -1;
            end
        elseif ( bossNum == 6 )
            if ( collision( type Shoot ))
                if ( !region_out( id, region ))
                    repeat
                        x -= 20;
                        frame;
                    until ( region_out( id, region ))
                end
            end
            if ( region_out( id, region ))
                y = rand( 140, 1700 );
                repeat
                    x += 30;
                    frame;
                until ( x <= 3000 )
            end
        elseif ( bossNum == 7 )
            x += move;
            y += move;
            if ( y == 1890 )
                move *= -1;
            elseif ( y == 140 )
                move *= -1;
            end
            frame;
        end
        if (( get_timer() - timeShot >= 900 ) && ( bossNum < 4 ))
            enemyFire();
            timeShot = get_timer();
        elseif (( get_timer() - timeShot >= 1200 ) && ( bossNum >= 4 ) && ( bossNum < 6 ))
            betterEnemyFire();
            timeShot = get_timer();
        elseif (( get_timer() - timeShot >= 700 ) && ( bossNum == 6 ))
            bossLaser();
            timeShot = get_timer();
        elseif (( get_timer() - timeShot >= 500 ) && ( bossNum == 7 ))
            repeat
                bossLaser();
                counter++;
            until ( counter == 6 )
            timeShot = get_timer();
            counter = 0;
        end
        if ( collision( type Shoot ))
            health -= 1;
            sound_play( waves[ 2 ], 0 );
            frame;
        end
        if ( collision( ship_id ))
            Explode();
        end
        frame;
    until ( health <= 0 )
    score += bossNum * 1000;
    write_delete( my_text[ 2 ] );
    Death();
end


Process bossLaser()
Begin
    resolution = 10;
    file = shootemGfx;
    graph = 4;
    region = 1;
    x = father.x;
    y = father.y;
    angle = fget_angle( ship_id.x, ship_id.y, x, y );
    sound_play( waves[ 5 ], 0 );
    loop
        advance( -24 );
        if ( collision( ship_id ))
            if ( get_timer() - respawnTime > 2000 )
                Explode();
            end
        end
        if ( region_out( id, region ))
            signal( id, s_kill );
        end
        frame;
    end
end


Process levelEnd( int levelNum )
Begin
    my_text[ 2 ] = write( 0, 160, 80, 4, "Level " + ( levelNum + 1 ) + " Complete!" );
    signal( type Game, s_freeze );
    signal( type Level, s_freeze );
    frame( 12000 );
    signal( type Level, s_wakeup );
    signal( type Game, s_wakeup );
    write_delete( my_text[ 2 ] );
    signal( id, s_kill );
    frame;
end


Process GameEnd()
Begin
    my_text[ 2 ] = write( 0, 160, 80, 4, "CONGRATULATIONS!" );
    repeat
        my_text[ 1 ] = write( 0, 160, 100, 4, "Lives: " + lives );
        my_text[ 0 ] = write( 0, 160, 100, 4, "Score: " + score );
        frame( 10000 );
        lives--;
        score += 10000;
        write_delete( my_text[ 1 ] );
        write_delete( my_text[ 0 ] );
    until ( lives == 0 )
    highScore = score;
    EndGame = 1;
end
