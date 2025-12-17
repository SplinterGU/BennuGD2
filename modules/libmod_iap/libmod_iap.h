#ifndef MOD_IAP_H
#define MOD_IAP_H

#include "bgddl.h"

int64_t libmod_iap_initialize(INSTANCE *my, int64_t *params);
int64_t libmod_iap_purchase(INSTANCE *my, int64_t *params);
int64_t libmod_iap_consume(INSTANCE *my, int64_t *params);
int64_t libmod_iap_get_price(INSTANCE *my, int64_t *params);
int64_t libmod_iap_poll_event(INSTANCE *my, int64_t *params);
// Helper to get last purchase data (sku, token, etc)
int64_t libmod_iap_get_last_purchase_sku(INSTANCE *my, int64_t *params);
int64_t libmod_iap_get_last_purchase_token(INSTANCE *my, int64_t *params);

#endif
