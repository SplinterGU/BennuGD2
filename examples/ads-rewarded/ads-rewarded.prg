IMPORT "libmod_gfx";
IMPORT "libmod_input";
IMPORT "libmod_ads";

// Include fontsheet library if available, otherwise standard write is fine
// Using standard write for simplicity in this example

PROCESS main()
PRIVATE
    int event_id;
    int coins = 0;
    int last_touch_state = 0;
BEGIN
    set_mode(640, 480);
    set_fps(60, 0);

    // Inicializar módulo
    ADS_INITIALIZE();

    // Cargar anuncio bonificado (ID de prueba)
    ADS_LOAD_REWARDED("ca-app-pub-3940256099942544/5224354917");

    LOOP
        // Limpiar pantalla de texto
        write_delete(0);

        write(0, 320, 20, ALIGN_CENTER, "BennuGD2 Rewarded Ads Demo");
        write(0, 320, 60, ALIGN_CENTER, "Coins: " + coins);

        IF (ADS_IS_REWARDED_LOADED())
             write(0, 320, 100, ALIGN_CENTER, "Tap to Watch Ad & Earn 100 Coins!");
        ELSE
             write(0, 320, 100, ALIGN_CENTER, "Loading Ad...");
        END
        
        // Polling de eventos
        event_id = ADS_POLL_EVENT();
        IF (event_id != ADS_EVENT_NONE)
            SWITCH (event_id)
                CASE ADS_EVENT_REWARDED_EARNED:
                    coins += 100;
                    // Aquí podrías guardar partida o reproducir sonido
                END
                CASE ADS_EVENT_REWARDED_CLOSED:
                    // Cargar el siguiente inmediatamente
                    ADS_LOAD_REWARDED("ca-app-pub-3940256099942544/5224354917");
                END
                CASE ADS_EVENT_REWARDED_SHOW_FAILED:
                    // Reintentar carga
                    ADS_LOAD_REWARDED("ca-app-pub-3940256099942544/5224354917");
                END
            END
        END

        // Detectar toque
        IF (touches[0].active)
            IF (last_touch_state == 0) // On press
                IF (ADS_IS_REWARDED_LOADED())
                    ADS_SHOW_REWARDED();
                END
            END
            last_touch_state = 1;
        ELSE
            last_touch_state = 0;
        END

        FRAME;
    END
END
