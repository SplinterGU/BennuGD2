IMPORT "libmod_gfx";
IMPORT "libmod_input";
IMPORT "libmod_ads";

PROCESS bouncing_ball(double x, double y)
PRIVATE
    double dx = 4.0;
    double dy = 4.0;
BEGIN
    // Crear grafico simple (circulo rojo)
    graph = map_new(64, 64, 32);
    drawing_map(0, graph);
    draw_circle_filled(32, 32, 30, rgba(255, 0, 0, 255));
    drawing_map(0, 0); // Restore

    LOOP
        x += dx;
        y += dy;

        IF (x < 32 OR x > 608) dx = -dx; END
        IF (y < 32 OR y > 448) dy = -dy; END

        FRAME;
    END
END

PROCESS main()
PRIVATE
    int last_touch_state = 0;
BEGIN
    set_mode(640, 480);
    set_fps(60, 0);

    // Inicializar módulo
    ADS_INITIALIZE();

    // Cargar anuncio intersticial (ID de prueba)
    ADS_LOAD_INTERSTITIAL("ca-app-pub-3940256099942544/1033173712");

    // Mostrar banner en la parte inferior (ID de prueba)
    ADS_SHOW_BANNER("ca-app-pub-3940256099942544/6300978111", ADS_BANNER_POS_BOTTOM);

    // Crear un objeto visual para ver si el juego se detiene
    bouncing_ball(320, 240);

    LOOP
        // Limpiar textos anteriores
        write_delete(0);

        write(0, 320, 20, ALIGN_CENTER, "BennuGD2 Ads Demo");
        write(0, 320, 40, ALIGN_CENTER, "Touch screen to show Interstitial");

        IF (ADS_IS_INTERSTITIAL_LOADED())
            write(0, 320, 460, ALIGN_CENTER, "Status: Interstitial READY");
        ELSE
            write(0, 320, 460, ALIGN_CENTER, "Status: Loading...");
        END

        // Detectar toque (simple, solo primer dedo)
        IF (touches[0].active)
            IF (last_touch_state == 0) // On pres
                IF (ADS_IS_INTERSTITIAL_LOADED())
                    ADS_SHOW_INTERSTITIAL();
                    // Recargar inmediato
                    ADS_LOAD_INTERSTITIAL("ca-app-pub-3940256099942544/1033173712");
                END
            END
            last_touch_state = 1;
        ELSE
            last_touch_state = 0;
        END

        FRAME;
    END
END

