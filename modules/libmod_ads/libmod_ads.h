// mod_ads.h
#ifndef MOD_ADS_H
#define MOD_ADS_H

#include "bgddl.h"

// Prototipos de funciones públicas
int64_t libmod_ads_initialize(INSTANCE *my, int64_t *params);
int64_t libmod_ads_load_interstitial(INSTANCE *my, int64_t *params);
int64_t libmod_ads_show_interstitial(INSTANCE *my, int64_t *params);
int64_t libmod_ads_show_banner(INSTANCE *my, int64_t *params);
int64_t libmod_ads_hide_banner(INSTANCE *my, int64_t *params);
int64_t libmod_ads_is_interstitial_loaded(INSTANCE *my, int64_t *params);

// Rewarded
int64_t libmod_ads_load_rewarded(INSTANCE *my, int64_t *params);
int64_t libmod_ads_show_rewarded(INSTANCE *my, int64_t *params);
int64_t libmod_ads_is_rewarded_loaded(INSTANCE *my, int64_t *params);

// Events
int64_t libmod_ads_poll_event(INSTANCE *my, int64_t *params);

// Consent & Targeting
int64_t libmod_ads_check_consent(INSTANCE *my, int64_t *params);
int64_t libmod_ads_show_consent_form(INSTANCE *my, int64_t *params);
int64_t libmod_ads_set_child_directed(INSTANCE *my, int64_t *params);

#endif
