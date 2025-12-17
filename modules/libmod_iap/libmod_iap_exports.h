#ifndef __LIBMOD_IAP_EXPORTS
#define __LIBMOD_IAP_EXPORTS

#include "bgddl.h"
#include "libmod_iap.h"

#if defined(__BGDC__) || !defined(__STATIC__)

DLCONSTANT  __bgdexport( libmod_iap, constants_def)[] =
{
    { "IAP_EVENT_NONE"              , TYPE_QWORD    , 0 },
    { "IAP_EVENT_SETUP_OK"          , TYPE_QWORD    , 1 },
    { "IAP_EVENT_SETUP_FAIL"        , TYPE_QWORD    , 2 },
    { "IAP_EVENT_PURCHASE_OK"       , TYPE_QWORD    , 3 },
    { "IAP_EVENT_PURCHASE_CANCEL"   , TYPE_QWORD    , 4 },
    { "IAP_EVENT_PURCHASE_FAIL"     , TYPE_QWORD    , 5 },
    { "IAP_EVENT_CONSUME_OK"        , TYPE_QWORD    , 6 },
    { "IAP_EVENT_CONSUME_FAIL"      , TYPE_QWORD    , 7 },

    { NULL                          , 0             , 0 }
} ;

#endif

DLSYSFUNCS __bgdexport(libmod_iap, functions_exports)[] = {
    FUNC( "IAP_INITIALIZE"              , ""    , TYPE_INT  , libmod_iap_initialize             ),
    FUNC( "IAP_PURCHASE"                , "S"   , TYPE_INT  , libmod_iap_purchase               ),
    FUNC( "IAP_CONSUME"                 , "S"   , TYPE_INT  , libmod_iap_consume                ),
    FUNC( "IAP_GET_PRICE"               , "S"   , TYPE_STRING, libmod_iap_get_price             ),
    FUNC( "IAP_POLL_EVENT"              , ""    , TYPE_INT  , libmod_iap_poll_event             ),
    FUNC( "IAP_GET_LAST_PURCHASE_SKU"   , ""    , TYPE_STRING, libmod_iap_get_last_purchase_sku  ),
    FUNC( "IAP_GET_LAST_PURCHASE_TOKEN" , ""    , TYPE_STRING, libmod_iap_get_last_purchase_token),
    
    FUNC( 0                             , 0     , 0         , 0                                 )
};

#endif
