
import "mod_gfx";
import "mod_input";
import "mod_sound";
import "mod_misc";
import "mod_debug";
include "qawizt.inc"
include "game.inc"

Process Main()
private
    int i;
Begin
    i_gameloopenabled = 1;
    zlInit();
    cursoron = 1;
    //StartNewGame();
    timer[0] = 0;
    repeat
        zlCls();
        zlControlState();
        ProcVolume();
        GameStep();
        if ( cursoron )
            zlDraw( mousex + 8, mousey + 8, 9 );
        end;
        if ( musictrack != lastmusictrack )
            PlayMusic( musictrack );
        end;
        count++;
        frame;
    until ( i_gameloopenabled == 0 );
    let_me_alone();
OnExit
    zlShutDown();
End
