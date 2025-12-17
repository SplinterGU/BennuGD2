// AdsModule.java
package org.libsdl.app;

import android.app.Activity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import androidx.annotation.NonNull;
import com.google.android.gms.ads.*;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;
import com.google.android.ump.*;
import com.google.android.gms.ads.rewarded.RewardItem;

public class AdsModule {
    private static Activity activity;
    private static InterstitialAd mInterstitialAd;
    private static RewardedAd mRewardedAd;
    private static AdView bannerAd;
    private static ConsentInformation consentInformation;
    private static final String TAG = "AdsModule";

    // Event tracking
    private static int lastEvent = 0;
    // Event constants (must match C defines)
    public static final int EVENT_NONE = 0;
    public static final int EVENT_INTERSTITIAL_CLOSED = 1;
    public static final int EVENT_REWARDED_LOADED = 2;
    public static final int EVENT_REWARDED_SHOW_FAILED = 3;
    public static final int EVENT_REWARDED_CLOSED = 4;
    public static final int EVENT_REWARDED_EARNED = 5;
    public static final int EVENT_CONSENT_READY = 6;
    public static final int EVENT_CONSENT_ERROR = 7;
    public static final int EVENT_CONSENT_FORM_DISMISSED = 8;
    public static final int EVENT_CONSENT_FORM_ERROR = 9;

    // Método para inicializar con la actividad de SDL2
    public static void initialize(Activity act) {
        activity = act;
        MobileAds.initialize(activity, initializationStatus -> {
        });
        lastEvent = EVENT_NONE;
    }

    // Método para obtener el layout principal de SDL2
    private static ViewGroup getSDLViewGroup() {
        return (ViewGroup) activity.getWindow().getDecorView().findViewById(android.R.id.content);
    }

    // --- Events ---
    public static int pollEvent() {
        int e = lastEvent;
        lastEvent = EVENT_NONE;
        return e;
    }

    // --- Consent (UMP) ---
    public static void checkConsent() {
        activity.runOnUiThread(() -> {
            ConsentRequestParameters params = new ConsentRequestParameters.Builder()
                    .setTagForUnderAgeOfConsent(false)
                    .build();

            consentInformation = UserMessagingPlatform.getConsentInformation(activity);

            // Uncomment to force testing (use your device hash)
            // consentInformation.reset();

            consentInformation.requestConsentInfoUpdate(
                    activity,
                    params,
                    (ConsentInformation.OnConsentInfoUpdateSuccessListener) () -> {
                        // The consent information state was updated.
                        // Now we check if a form is available.
                        if (consentInformation.isConsentFormAvailable()) {
                            lastEvent = EVENT_CONSENT_READY;
                        } else {
                            // No form needed, effectively ready
                            lastEvent = EVENT_CONSENT_READY;
                        }
                    },
                    (ConsentInformation.OnConsentInfoUpdateFailureListener) formError -> {
                        // Handle the error.
                        lastEvent = EVENT_CONSENT_ERROR;
                    });
        });
    }

    public static void showConsentForm() {
        activity.runOnUiThread(() -> {
            UserMessagingPlatform.loadAndShowConsentFormIfRequired(
                    activity,
                    (ConsentForm.OnConsentFormDismissedListener) loadAndShowError -> {
                        if (loadAndShowError != null) {
                            // Consent gathering failed.
                            lastEvent = EVENT_CONSENT_FORM_ERROR;
                        } else {
                            // Consent has been gathered.
                            if (consentInformation.canRequestAds()) {
                                MobileAds.initialize(activity, initializationStatus -> {
                                });
                            }
                            lastEvent = EVENT_CONSENT_FORM_DISMISSED;
                        }
                    });
        });
    }

    public static void setChildDirected(int childDirected) {
        activity.runOnUiThread(() -> {
            RequestConfiguration.Builder requestConfigurationBuilder = MobileAds.getRequestConfiguration().toBuilder();

            if (childDirected == 1) {
                requestConfigurationBuilder
                        .setTagForChildDirectedTreatment(RequestConfiguration.TAG_FOR_CHILD_DIRECTED_TREATMENT_TRUE);
            } else if (childDirected == 0) {
                requestConfigurationBuilder
                        .setTagForChildDirectedTreatment(RequestConfiguration.TAG_FOR_CHILD_DIRECTED_TREATMENT_FALSE);
            } else {
                requestConfigurationBuilder.setTagForChildDirectedTreatment(
                        RequestConfiguration.TAG_FOR_CHILD_DIRECTED_TREATMENT_UNSPECIFIED);
            }

            MobileAds.setRequestConfiguration(requestConfigurationBuilder.build());
        });
    }

    // --- Interstitial ---

    public static void loadInterstitial(String adUnitId) {
        // ... (existing code)
        activity.runOnUiThread(() -> {
            // Check calling canRequestAds() is recommended before loading
            if (consentInformation != null && !consentInformation.canRequestAds()) {
                // But we let it fail naturally or handled by MobileAds init check if we want
                // strict enforcement
                // For now, assume initialized
            }

            AdRequest adRequest = new AdRequest.Builder().build();

            InterstitialAd.load(activity, adUnitId, adRequest,
                    new InterstitialAdLoadCallback() {
                        @Override
                        public void onAdLoaded(@NonNull InterstitialAd interstitialAd) {
                            mInterstitialAd = interstitialAd;

                            mInterstitialAd.setFullScreenContentCallback(new FullScreenContentCallback() {
                                @Override
                                public void onAdDismissedFullScreenContent() {
                                    if (mInterstitialAd == interstitialAd) {
                                        mInterstitialAd = null;
                                    }
                                    lastEvent = EVENT_INTERSTITIAL_CLOSED;
                                }

                                @Override
                                public void onAdFailedToShowFullScreenContent(AdError adError) {
                                    if (mInterstitialAd == interstitialAd) {
                                        mInterstitialAd = null;
                                    }
                                }

                                @Override
                                public void onAdShowedFullScreenContent() {
                                    // Do nothing
                                }
                            });
                        }

                        @Override
                        public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                            mInterstitialAd = null;
                        }
                    });
        });
    }

    public static boolean showInterstitial() {
        if (mInterstitialAd != null) {
            activity.runOnUiThread(() -> mInterstitialAd.show(activity));
            return true;
        }
        return false;
    }

    public static boolean isInterstitialLoaded() {
        return mInterstitialAd != null;
    }

    // --- Rewarded ---

    public static void loadRewarded(String adUnitId) {
        activity.runOnUiThread(() -> {
            AdRequest adRequest = new AdRequest.Builder().build();
            RewardedAd.load(activity, adUnitId, adRequest,
                    new RewardedAdLoadCallback() {
                        @Override
                        public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                            if (mRewardedAd == null) { // Only clear if we were expecting this one
                                // ...
                            }
                        }

                        @Override
                        public void onAdLoaded(@NonNull RewardedAd rewardedAd) {
                            mRewardedAd = rewardedAd;
                            lastEvent = EVENT_REWARDED_LOADED;

                            mRewardedAd.setFullScreenContentCallback(new FullScreenContentCallback() {
                                @Override
                                public void onAdShowedFullScreenContent() {
                                }

                                @Override
                                public void onAdFailedToShowFullScreenContent(AdError adError) {
                                    if (mRewardedAd == rewardedAd) {
                                        mRewardedAd = null;
                                    }
                                    lastEvent = EVENT_REWARDED_SHOW_FAILED;
                                }

                                @Override
                                public void onAdDismissedFullScreenContent() {
                                    if (mRewardedAd == rewardedAd) {
                                        mRewardedAd = null;
                                    }
                                    lastEvent = EVENT_REWARDED_CLOSED;
                                }
                            });
                        }
                    });
        });
    }

    public static void showRewarded() {
        if (mRewardedAd != null) {
            activity.runOnUiThread(() -> {
                RewardedAd adToShow = mRewardedAd;
                mRewardedAd = null; // Clear from "ready to show" immediately
                adToShow.show(activity, new OnUserEarnedRewardListener() {
                    @Override
                    public void onUserEarnedReward(@NonNull RewardItem rewardItem) {
                        lastEvent = EVENT_REWARDED_EARNED;
                    }
                });
            });
        }
    }

    public static boolean isRewardedLoaded() {
        return mRewardedAd != null;
    }

    // --- Banner ---

    public static void showBanner(String adUnitId, int position) {
        activity.runOnUiThread(() -> {
            // Eliminar banner existente
            if (bannerAd != null) {
                ViewGroup parent = (ViewGroup) bannerAd.getParent();
                if (parent != null)
                    parent.removeView(bannerAd);
                bannerAd.destroy();
                bannerAd = null;
            }

            // Crear nuevo banner
            bannerAd = new AdView(activity);
            bannerAd.setAdSize(AdSize.BANNER);
            bannerAd.setAdUnitId(adUnitId);

            // Configurar layout params para SDL2
            RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
                    RelativeLayout.LayoutParams.WRAP_CONTENT,
                    RelativeLayout.LayoutParams.WRAP_CONTENT);

            if (position == 0) { // Top
                params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
                params.addRule(RelativeLayout.CENTER_HORIZONTAL);
            } else { // Bottom
                params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
                params.addRule(RelativeLayout.CENTER_HORIZONTAL);
            }

            // Añadir a la vista principal de SDL2
            getSDLViewGroup().addView(bannerAd, params);

            // Cargar anuncio
            AdRequest adRequest = new AdRequest.Builder().build();
            bannerAd.loadAd(adRequest);
        });
    }

    public static void hideBanner() {
        activity.runOnUiThread(() -> {
            if (bannerAd != null) {
                // remove from parent entirely to be safe
                ViewGroup parent = (ViewGroup) bannerAd.getParent();
                if (parent != null)
                    parent.removeView(bannerAd);
                bannerAd.destroy();
                bannerAd = null;
            }
        });
    }
}