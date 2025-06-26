import "mod_input";
import "mod_gfx";
import "mod_misc";

#include "fontsheet.inc"

PROCESS main()
PRIVATE
    int i;
    float px, py, pr, radius;
    int font;
BEGIN
    screen.fullscreen = 1;

    set_mode(0, 0); // Ajustá la resolución según tu salida

    font = fontsheet_load("font/font-07_31x34.png");

    WHILE ( !key(_ESC) )
        // Limpia pantalla cada frame
        draw_delete(0);
        write_delete(0);

        FOR (i = 0; i < 10; i++)
            IF (touches[i].active)
                px = touches[i].x;
                py = touches[i].y;
                pr = touches[i].pressure;

                radius = 100 + (pr * 125);

                draw_circle_filled(px, py, radius, rgba(255, 255 * (1.0 - pr), 255 * pr, 255));

                // Mostrar el fingerId como texto
                write(font, px - 8, py - radius - 14, ALIGN_CENTER, i + "+" + touches[i].fingerId);
            END
        END

        FRAME;
    END

    let_me_alone();

END
