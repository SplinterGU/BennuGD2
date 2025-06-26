import "mod_input";
import "mod_gfx";
import "mod_misc";

#include "fontsheet.inc"

FUNCTION STRING sensor_type_name(INT t)
BEGIN
    IF (t == SENSOR_ACCEL)     RETURN "ACCEL"; END
    IF (t == SENSOR_GYRO)      RETURN "GYRO"; END
    IF (t == SENSOR_ACCEL_L)   RETURN "ACCEL_L"; END
    IF (t == SENSOR_GYRO_L)    RETURN "GYRO_L"; END
    IF (t == SENSOR_ACCEL_R)   RETURN "ACCEL_R"; END
    IF (t == SENSOR_GYRO_R)    RETURN "GYRO_R"; END
    IF (t == SENSOR_UNKNOWN)   RETURN "UNKNOWN"; END
    RETURN "INVALID";
END

PROCESS main()
PRIVATE
    int i, j, font;
    float ax, ay;
    int screen_width, screen_height;
    int center_x, center_y;
    float px, py;
BEGIN
    set_fps(60, 0);
    screen.fullscreen = 1;

    set_mode(0, 0);

    desktop_get_size(&screen_width, &screen_height);

    center_x = screen_width / 2;
    center_y = screen_height / 2;

    font = fontsheet_load("font/font-07_31x34.png");

    WHILE (!key(_ESC))
        draw_delete(0);
        write_delete(0);

        y = 20;
        write(font, 20, y, 0, "sensor test");
        y += 32;

        // Dibujar círculo de referencia en el centro
        draw_circle(center_x, center_y, 60, rgba(255, 255, 255, 128));

        FOR (i = 0; i < MAX_SENSORS; i++)
            IF (sensors[i].active)
                write(font, 20, y, 0, "Sensor #" + i + " type: " + sensor_type_name(sensors[i].type) + " name: " + sensors[i].name);
                y += 32;

                STRING val = "";
                FOR (j = 0; j < MAX_SENSORS_VALUES; j++)
                    val += " ["+ sensors[i].values[j] +"] ";
                END
                write(font, 40, y, 0, val );
                y += 32;

                // Si es un acelerómetro, usarlo como nivel
                IF (sensors[i].type == SENSOR_ACCEL)
                    ax = sensors[i].values[0]; // eje X
                    ay = sensors[i].values[1]; // eje Y

                    // Obtener orientación
                    int orientation = display_get_orientation();

                    // Transformar según orientación
                    SWITCH (orientation)
                        CASE ORIENTATION_PORTRAIT:
                            px = center_x - ax * (screen_width / 25);
                            py = center_y + ay * (screen_height / 25);
                            END

                        CASE ORIENTATION_LANDSCAPE:
                            px = center_x + ay * (screen_width / 25);
                            py = center_y + ax * (screen_height / 25);
                            END

                        CASE ORIENTATION_PORTRAIT_FLIPPED:
                            px = center_x + ax * (screen_width / 25);
                            py = center_y - ay * (screen_height / 25);
                            END

                        CASE ORIENTATION_LANDSCAPE_FLIPPED:
                            px = center_x - ay * (screen_width / 25);
                            py = center_y - ax * (screen_height / 25);
                            END

                        DEFAULT:
                            px = center_x;
                            py = center_y;
                            END
                    END

                    // Dibujar burbuja (círculo pequeño), desplazado por gravedad
                    draw_circle_filled(
                        px,
                        py,
                        30,
                        rgba(0, 255, 128, 255)
                    );
                END
                y += 32;
            END
        END

        FRAME;
    END

    let_me_alone();
END
