import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

global
    int already_colliding=0;
    int data;
end

function control(myid, mouse_colliding, ifather, ismallbro, ibigbro, ison,
                 int pointer pgfather, int pointer pgsmallbro, int pointer pgbigbro, int pointer pgson,
                 int pointer follow_mouse, double pointer px, double pointer py);
begin
    if ( already_colliding!=myid && already_colliding!=0)
        return (0);
    end

    if ( mouse_colliding && already_colliding == 0 )
        if ( mouse.left )
            *follow_mouse = 1;
        end

        if ( keyboard.shift_status & STAT_SHIFT && mouse.left )
//            debug;  // Triggers the debug console in the next frame and dumps the instances
            return (1);
        end

        if ( keyboard.shift_status & STAT_SHIFT && mouse.right )
            signal(myid, s_kill_tree);
            return (0);
        end

        if ( mouse.right )
            if ( *pgfather == 0 && exists(ifather) )
                *pgfather = ifather.graph;
                ifather.size=300;
                ifather.graph=8;
            end
            if ( *pgsmallbro == 0 && exists(ismallbro) )
                *pgsmallbro = ismallbro.graph;
                ismallbro.size=200;
                ismallbro.graph=10;
            end
            if ( *pgbigbro == 0 && exists(ibigbro) )
                *pgbigbro = ibigbro.graph;
                ibigbro.size=250;
                ibigbro.graph=7;
            end
            if ( *pgson == 0 && exists(ison) )
                *pgson = ison.graph;
                ison.size=150;
                ison.graph=11;
            end
        end
        already_colliding=myid;
    end

    if ( !mouse.right )
        if ( *pgfather != 0 )
            if ( exists(ifather) )
                ifather.size=100;
                ifather.graph=*pgfather;
            end
            *pgfather=0;
        end
        if ( *pgsmallbro != 0)
            if ( exists(ismallbro) )
                ismallbro.size=100;
                ismallbro.graph=*pgsmallbro;
            end
            *pgsmallbro=0;
        end
        if ( *pgbigbro != 0)
            if ( exists(ibigbro) )
                ibigbro.size=100;
                ibigbro.graph=*pgbigbro;
            end
            *pgbigbro=0;
        end
        if ( *pgson != 0)
            if ( exists(ison) )
                ison.size=100;
                ison.graph=*pgson;
            end
            *pgson=0;
        end
    end

    if ( *follow_mouse AND mouse.left )
        *px = mouse.x;
        *py = mouse.y;
    else
        *follow_mouse = 0;
    end

    if ( !mouse.left && !mouse.middle && !mouse.right )
        already_colliding=0;
    end

    return (0);

end

process MyProcess4(x, y)
private
    int follow_mouse = 0;
    int gfather=0, gsmallbro=0, gbigbro=0, gson=0;
begin
    graph=5;

    loop
        if ( control (id, collision( type mouse ), father, smallbro, bigbro, son, &gfather, &gsmallbro, &gbigbro, &gson, &follow_mouse, &x, &y ) )
            return;
        end;

        frame;
    end;
end;

process MyProcess3(x, y)
private
    int follow_mouse = 0;
    int gfather=0, gsmallbro=0, gbigbro=0, gson=0;
begin
    graph=4;

    MyProcess4(x-30, y+40);
    MyProcess4(x   , y+40);
    MyProcess4(x+30, y+40);

    loop
        if ( control (id, collision( type mouse ), father, smallbro, bigbro, son, &gfather, &gsmallbro, &gbigbro, &gson, &follow_mouse, &x, &y ) )
            return;
        end;

        frame;
    end;
end;


process MyProcess2(x, y)
private
    int follow_mouse = 0;
    int gfather=0, gsmallbro=0, gbigbro=0, gson=0;
begin
    graph=3;

    MyProcess3(x-90, y+40);
    MyProcess3(x   , y+40);
    MyProcess3(x+90, y+40);

    loop
        if ( control (id, collision( type mouse ), father, smallbro, bigbro, son, &gfather, &gsmallbro, &gbigbro, &gson, &follow_mouse, &x, &y ) )
            return;
        end;

        frame;
    end;
end;



process MyProcess1(x,y);
private
    int follow_mouse = 0;
    int gfather=0, gsmallbro=0, gbigbro=0, gson=0;
begin
    graph=2;

    MyProcess2(160, y+40);
    MyProcess2(480, y+40);

    loop
        if ( control (id, collision( type mouse ), father, smallbro, bigbro, son, &gfather, &gsmallbro, &gbigbro, &gson, &follow_mouse, &x, &y ) )
            return;
        end;
        frame;
    end;
end;

/* Main */
private
    int follow_mouse = 0;
    int gfather=0, gsmallbro=0, gbigbro=0, gson=0;
begin
    y=20;
    x=640/2;
    SET_MODE(640,480);

    write(0,20,430,0,"Drag processes with left mouse button");
    write(0,20,440,0,"Right-click on a process to view its direct relatives");
    write(0,20,450,0,"Press SHIFT+<left mouse button> to kill processes");
    write(0,20,460,0,"Press SHIFT+<right mouse button> to kill processes and their children");

    data=fpg_load("res/data.fpg");
    file = data;
    graph=1;
    mouse.graph=9;

    myProcess1(320, y+40);

    loop
        if ( control (id, collision( type mouse ), father, smallbro, bigbro, son, &gfather, &gsmallbro, &gbigbro, &gson, &follow_mouse, &x, &y ) )
            return;
        end;
        if ( key(_ESC) ) break; end
        frame;
    end;

    let_me_alone();

end;
