import "libmod_gfx";
import "libmod_input";
import "libmod_misc";

const
    scrw = 320;
    scrh = 240;
end

process sprite( double x, y, int graph )
private
    int sentido = 1;
begin

    while(!key(_ESC))
        x += 2.5 * sentido;
        if ( x > scrw - 32 || x < 32 ) sentido = -sentido; end

        frame;
    end

end

process main()
begin
    set_mode(scrw,scrh);
    set_fps(60,0);

    sprite(scrw / 2.0, scrh / 2.0, map_load("res/smiley.png") );

end
