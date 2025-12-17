import "libmod_gfx";
import "libmod_input";
import "libmod_ads";
import "libmod_misc"; // For say/exit

// Constantes de Eventos (Deben coincidir con el módulo si no están definidas)
/*
CONST
    ADS_EVENT_CONSENT_READY = 6;
    ADS_EVENT_CONSENT_ERROR = 7;
    ADS_EVENT_CONSENT_FORM_DISMISSED = 8;
    ADS_EVENT_CONSENT_FORM_ERROR = 9;
END
*/

PROCESS main();
PRIVATE
    int event_id;
    int txt_status;
    string status_str = "Checking Consent...";
BEGIN
    set_mode(640, 480, 32);
    set_fps(60, 0);

    // Initialize Module (JNI + Activity)
    ADS_INITIALIZE();

    // Initial message
    txt_status = write_var(0, 320, 240, 4, status_str);

    // Step 1: Check Consent Status
    // This connects to Google servers to see if a form is required (EU users)
    ADS_CHECK_CONSENT();

    LOOP
        event_id = ADS_POLL_EVENT();

        SWITCH (event_id)
            CASE ADS_EVENT_CONSENT_READY:
                status_str = "Consent Info Ready. Showing Form if needed...";
                // Step 2: Show form (only shows if required by UMP)
                // If not required, it returns immediately (or quickly) via callback
                ADS_SHOW_CONSENT_FORM();
            END

            CASE ADS_EVENT_CONSENT_ERROR:
                status_str = "Error checking consent. Proceeding anyway...";
                // Fallback: Try to initialize ads normally?
                ADS_INITIALIZE();
            END

            CASE ADS_EVENT_CONSENT_FORM_DISMISSED:
                status_str = "Form Dismissed/Finished. Ads Initialized!";
                // Now we are ready to load ads
                // Load an interstitial for testing
                ADS_LOAD_INTERSTITIAL("ca-app-pub-3940256099942544/1033173712");
            END

            CASE ADS_EVENT_CONSENT_FORM_ERROR:
                status_str = "Form Error. Proceeding...";
                ADS_INITIALIZE(); // Try manual init
            END

            CASE 5: // ADS_EVENT_REWARDED_EARNED match
            END
        END

        IF (key(_esc)) exit("",0); END

        FRAME;
    END
END
