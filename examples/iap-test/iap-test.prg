import "libmod_gfx";
import "libmod_input";
import "libmod_iap"; // Importar el nuevo módulo
import "libmod_misc";

// Ejemplo de definiciones de constantes si no estás usando un include general
/*
CONST
    IAP_EVENT_NONE = 0;
    IAP_EVENT_SETUP_OK = 1;
    IAP_EVENT_SETUP_FAIL = 2;
    IAP_EVENT_PURCHASE_OK = 3;
    IAP_EVENT_PURCHASE_CANCEL = 4;
    IAP_EVENT_PURCHASE_FAIL = 5;
    IAP_EVENT_CONSUME_OK = 6;
    IAP_EVENT_CONSUME_FAIL = 7;
END
*/

PROCESS main();
PRIVATE
    int event_id;
    string txt_status = "Iniciando IAP...";
    string sku_monedas = "android.test.purchased"; // ID de prueba genérico de Google
    string sku_price = "...";
    int txt_id;
BEGIN
    set_mode(640, 480, 32);
    set_fps(60, 0);

    // IAP_INITIALIZE se llama automáticamente en SDLActivity con nuestra modificación,
    // pero podemos llamarlo para reinicializar si falla o para asegurar.
    IAP_INITIALIZE();

    txt_id = write_var(0, 320, 50, 4, txt_status);
    write_var(0, 320, 100, 4, sku_price);

    write(0, 320, 200, 4, "TOQUE ARRIBA para Comprar");
    draw_line(0, 240, 640, 240, 0xFFFFFF);
    write(0, 320, 300, 4, "TOQUE ABAJO para Consumir");

    LOOP
        event_id = IAP_POLL_EVENT();

        SWITCH (event_id)
            CASE IAP_EVENT_SETUP_OK:
                txt_status = "IAP Conectado. Obteniendo precios...";
                // Intentar obtener precio (esto dispara una consulta asíncrona interna)
                sku_price = IAP_GET_PRICE(sku_monedas);
            END

            CASE IAP_EVENT_SETUP_FAIL:
                txt_status = "Fallo de conexión IAP.";
            END

            CASE IAP_EVENT_PURCHASE_OK:
                txt_status = "¡Compra Exitosa!";
                string sku = IAP_GET_LAST_PURCHASE_SKU();
                string token = IAP_GET_LAST_PURCHASE_TOKEN();
                // Aquí deberías dar las monedas al usuario
                // Si es consumible, debes llamar a IAP_CONSUME(token) luego.
            END

            CASE IAP_EVENT_PURCHASE_CANCEL:
                txt_status = "Compra cancelada por usuario.";
            END

            CASE IAP_EVENT_PURCHASE_FAIL:
                txt_status = "Error en la compra.";
            END

            CASE IAP_EVENT_CONSUME_OK:
                txt_status = "Producto consumido. Listo para comprar de nuevo.";
            END
        END

        // Actualizar precio visualmente si cambió de "..."
        // (En un juego real, haz esto menos frecuente o tras un evento)
        if (sku_price == "...")
             sku_price = IAP_GET_PRICE(sku_monedas);
        end

        // Touch Handling
        IF (mouse.left)
            // Zona Superior: Comprar
            IF (mouse.y < 240)
                txt_status = "Iniciando compra...";
                IAP_PURCHASE(sku_monedas);
            
            // Zona Inferior: Consumir
            ELSE
                string token = IAP_GET_LAST_PURCHASE_TOKEN();
                if (token != "")
                    txt_status = "Consumiendo...";
                    IAP_CONSUME(token);
                else
                    txt_status = "No hay token para consumir.";
                end
            END
            
            // Esperar a que suelte el dedo para no repetir
            while(mouse.left) frame; end
        END

        IF (key(_esc)) exit("",0); END

        FRAME;
    END
END
