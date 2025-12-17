/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *  Copyright (C) 2002-2006 Fenix Team (Fenix)
 *  Copyright (C) 1999-2002 José Luis Cebrián Pagüe (Fenix)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

#ifndef __LIBMOD_ADS_EXPORTS
#define __LIBMOD_ADS_EXPORTS

/* ----------------------------------------------------------------- */

#include "bgddl.h"
#include "libmod_ads.h"

/* ----------------------------------------------------------------- */

#if defined(__BGDC__) || !defined(__STATIC__)

/* ----------------------------------------------------------------- */

DLCONSTANT  __bgdexport( libmod_ads, constants_def)[] =
{
    // Constantes para posiciones del banner
    { "ADS_BANNER_POS_TOP"          , TYPE_QWORD    , 0                     },
    { "ADS_BANNER_POS_BOTTOM"       , TYPE_QWORD    , 1                     },
    
    // Constantes para eventos
    { "ADS_EVENT_NONE"              , TYPE_QWORD    , 0                     },
    { "ADS_EVENT_INTERSTITIAL_CLOSED", TYPE_QWORD   , 1                     },
    { "ADS_EVENT_REWARDED_LOADED"   , TYPE_QWORD    , 2                     },
    { "ADS_EVENT_REWARDED_SHOW_FAILED", TYPE_QWORD  , 3                     },
    { "ADS_EVENT_REWARDED_CLOSED"   , TYPE_QWORD    , 4                     },
    { "ADS_EVENT_REWARDED_EARNED"   , TYPE_QWORD    , 5                     },
    { "ADS_EVENT_CONSENT_READY"     , TYPE_QWORD    , 6                     },
    { "ADS_EVENT_CONSENT_ERROR"     , TYPE_QWORD    , 7                     },
    { "ADS_EVENT_CONSENT_FORM_DISMISSED", TYPE_QWORD, 8                     },
    { "ADS_EVENT_CONSENT_FORM_ERROR", TYPE_QWORD    , 9                     },

    { NULL                          , 0             , 0                     }
} ;

#endif

/* ----------------------------------------------------------------- */
/* Functions declaration                                             */
// Tabla de funciones
DLSYSFUNCS __bgdexport(libmod_ads, functions_exports)[] = {
    FUNC( "ADS_INITIALIZE"              , ""    , TYPE_INT  , libmod_ads_initialize             ),
    // Interstitial
    FUNC( "ADS_LOAD_INTERSTITIAL"       , "S"   , TYPE_INT  , libmod_ads_load_interstitial      ),
    FUNC( "ADS_SHOW_INTERSTITIAL"       , ""    , TYPE_INT  , libmod_ads_show_interstitial      ),
    FUNC( "ADS_IS_INTERSTITIAL_LOADED"  , ""    , TYPE_INT  , libmod_ads_is_interstitial_loaded ),
    // Rewarded
    FUNC( "ADS_LOAD_REWARDED"           , "S"   , TYPE_INT  , libmod_ads_load_rewarded          ),
    FUNC( "ADS_SHOW_REWARDED"           , ""    , TYPE_INT  , libmod_ads_show_rewarded          ),
    FUNC( "ADS_IS_REWARDED_LOADED"      , ""    , TYPE_INT  , libmod_ads_is_rewarded_loaded     ),
    // Banner
    FUNC( "ADS_SHOW_BANNER"             , "SI"  , TYPE_INT  , libmod_ads_show_banner            ),
    FUNC( "ADS_HIDE_BANNER"             , ""    , TYPE_INT  , libmod_ads_hide_banner            ),
    // Events
    FUNC( "ADS_POLL_EVENT"              , ""    , TYPE_INT  , libmod_ads_poll_event             ),
    // Consent & Targeting
    FUNC( "ADS_CHECK_CONSENT"           , ""    , TYPE_INT  , libmod_ads_check_consent          ),
    FUNC( "ADS_SHOW_CONSENT_FORM"       , ""    , TYPE_INT  , libmod_ads_show_consent_form      ),
    FUNC( "ADS_SET_CHILD_DIRECTED"      , "I"   , TYPE_INT  , libmod_ads_set_child_directed     ),
    
    FUNC( 0                             , 0     , 0         , 0                                 )
};

/* ----------------------------------------------------------------- */

#endif
