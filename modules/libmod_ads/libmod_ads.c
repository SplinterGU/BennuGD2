#include "bgddl.h"
#include "dlvaracc.h"
#include "xstrings.h"

#include <SDL.h>
#include <SDL_system.h>

#ifdef __ANDROID__

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "BennuGD_Ads"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* Cache de clases y métodos JNI */
static struct {
    jclass adsModuleClass;
    jclass sdlActivityClass;
    jmethodID initMethod;
    jmethodID loadInterstitialMethod;
    jmethodID showInterstitialMethod;
    jmethodID showBannerMethod;
    jmethodID hideBannerMethod;
    jmethodID isInterstitialLoadedMethod;
    // Rewarded
    jmethodID loadRewardedMethod;
    jmethodID showRewardedMethod;
    jmethodID isRewardedLoadedMethod;
    // Consent & Targeting
    jmethodID checkConsentMethod;
    jmethodID showConsentFormMethod;
    jmethodID setChildDirectedMethod;
    // Events
    jmethodID pollEventMethod;
    // SDL
    jmethodID getContextMethod;
} jniCache;

/* Estado de inicialización */
static int initialized = 0;

// Inicializar la cache JNI
int initialize_jni_cache(JNIEnv *env) {
    if (initialized) return 1;

    // Obtener clases
    jniCache.adsModuleClass = (*env)->FindClass(env, "org/libsdl/app/AdsModule");
    if (!jniCache.adsModuleClass) {
        LOGE("No se encontró la clase AdsModule");
        return 0;
    }
    jniCache.adsModuleClass = (*env)->NewGlobalRef(env, jniCache.adsModuleClass);

    jniCache.sdlActivityClass = (*env)->FindClass(env, "org/libsdl/app/SDLActivity");
    if (!jniCache.sdlActivityClass) {
        LOGE("No se encontró la clase SDLActivity");
        return 0;
    }
    jniCache.sdlActivityClass = (*env)->NewGlobalRef(env, jniCache.sdlActivityClass);

    // Obtener métodos de AdsModule
    jniCache.initMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass, "initialize", "(Landroid/app/Activity;)V");
    
    // Interstitial
    jniCache.loadInterstitialMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"loadInterstitial", "(Ljava/lang/String;)V");
    jniCache.showInterstitialMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"showInterstitial", "()Z");
    jniCache.isInterstitialLoadedMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"isInterstitialLoaded", "()Z");
    
    // Banner
    jniCache.showBannerMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"showBanner", "(Ljava/lang/String;I)V");
    jniCache.hideBannerMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"hideBanner", "()V");

    // Rewarded
    jniCache.loadRewardedMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"loadRewarded", "(Ljava/lang/String;)V");
    jniCache.showRewardedMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"showRewarded", "()V");
    jniCache.isRewardedLoadedMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"isRewardedLoaded", "()Z");

    // Consent & Targeting
    jniCache.checkConsentMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"checkConsent", "()V");
    jniCache.showConsentFormMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"showConsentForm", "()V");
    jniCache.setChildDirectedMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"setChildDirected", "(I)V");

    // Events
    jniCache.pollEventMethod = (*env)->GetStaticMethodID(env, jniCache.adsModuleClass,"pollEvent", "()I");

    // Obtener método de SDLActivity
    jniCache.getContextMethod = (*env)->GetStaticMethodID(env, jniCache.sdlActivityClass,
        "getContext", "()Landroid/content/Context;");

    initialized = 1;
    return 1;
}

// Función para obtener la actividad de SDL2
jobject get_sdl_activity(JNIEnv *env) {
    if (!initialized || !jniCache.sdlActivityClass || !jniCache.getContextMethod) {
        LOGE("JNI cache no inicializada");
        return NULL;
    }
    return (*env)->CallStaticObjectMethod(env, jniCache.sdlActivityClass, jniCache.getContextMethod);
}

// Función initialize modificada para SDL2
int64_t libmod_ads_initialize(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialize_jni_cache(env)) return 0;

    jobject activity = get_sdl_activity(env);
    if (!activity) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.initMethod, activity);
    (*env)->DeleteLocalRef(env, activity);

    LOGI("Módulo de anuncios inicializado correctamente con SDL2");
    return 1;
}

int64_t libmod_ads_load_interstitial(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    char *adUnitId = (char *)string_get(params[0]);
    if (!adUnitId) {
        string_discard(params[0]);
        LOGE("ID de unidad de anuncio no válido");
        return 0;
    }

    jstring jAdUnitId = (*env)->NewStringUTF(env, adUnitId);
    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.loadInterstitialMethod, jAdUnitId);
    (*env)->DeleteLocalRef(env, jAdUnitId);

    string_discard(params[0]);
    return 1;
}

int64_t libmod_ads_show_interstitial(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    jboolean result = (*env)->CallStaticBooleanMethod(env, jniCache.adsModuleClass, jniCache.showInterstitialMethod);
    return (int64_t)result;
}

int64_t libmod_ads_is_interstitial_loaded(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    jboolean result = (*env)->CallStaticBooleanMethod(env, jniCache.adsModuleClass, jniCache.isInterstitialLoadedMethod);
    return (int64_t)result;
}

// --- Rewarded ---

int64_t libmod_ads_load_rewarded(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    char *adUnitId = (char *)string_get(params[0]);
    if (!adUnitId) {
        string_discard(params[0]);
        return 0;
    }

    jstring jAdUnitId = (*env)->NewStringUTF(env, adUnitId);
    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.loadRewardedMethod, jAdUnitId);
    (*env)->DeleteLocalRef(env, jAdUnitId);

    string_discard(params[0]);
    return 1;
}

int64_t libmod_ads_show_rewarded(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.showRewardedMethod);
    return 1;
}

int64_t libmod_ads_is_rewarded_loaded(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    jboolean result = (*env)->CallStaticBooleanMethod(env, jniCache.adsModuleClass, jniCache.isRewardedLoadedMethod);
    return (int64_t)result;
}

// --- Events ---

int64_t libmod_ads_poll_event(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;
    
    jint event = (*env)->CallStaticIntMethod(env, jniCache.adsModuleClass, jniCache.pollEventMethod);
    return (int64_t)event;
}

// --- Banner ---

int64_t libmod_ads_show_banner(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    char *adUnitId = (char *)string_get(params[0]);
    int64_t position = params[1];

    if (!adUnitId) {
        string_discard(params[0]);
        return 0;
    }

    jstring jAdUnitId = (*env)->NewStringUTF(env, adUnitId);
    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.showBannerMethod, jAdUnitId, (jint)position);
    (*env)->DeleteLocalRef(env, jAdUnitId);

    string_discard(params[0]);
    return 1;
}

int64_t libmod_ads_hide_banner(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.hideBannerMethod);
    return 1;
}

// --- Consent & Targeting ---

int64_t libmod_ads_check_consent(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.checkConsentMethod);
    return 1;
}

int64_t libmod_ads_show_consent_form(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.showConsentFormMethod);
    return 1;
}

int64_t libmod_ads_set_child_directed(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    (*env)->CallStaticVoidMethod(env, jniCache.adsModuleClass, jniCache.setChildDirectedMethod, (jint)params[0]);
    return 1;
}


#else

// Stubs for non-Android platforms
int64_t libmod_ads_initialize(INSTANCE *my, int64_t *params) {
    return 1; 
}

int64_t libmod_ads_load_interstitial(INSTANCE *my, int64_t *params) {
    string_discard(params[0]);
    return 0; 
}

int64_t libmod_ads_show_interstitial(INSTANCE *my, int64_t *params) {
    return 0; 
}

int64_t libmod_ads_is_interstitial_loaded(INSTANCE *my, int64_t *params) {
    return 0; 
}

int64_t libmod_ads_load_rewarded(INSTANCE *my, int64_t *params) {
    string_discard(params[0]);
    return 0;
}

int64_t libmod_ads_show_rewarded(INSTANCE *my, int64_t *params) {
    return 0;
}

int64_t libmod_ads_is_rewarded_loaded(INSTANCE *my, int64_t *params) {
    return 0;
}

int64_t libmod_ads_poll_event(INSTANCE *my, int64_t *params) {
    return 0;
}

int64_t libmod_ads_show_banner(INSTANCE *my, int64_t *params) {
    string_discard(params[0]);
    return 0; 
}

int64_t libmod_ads_hide_banner(INSTANCE *my, int64_t *params) {
    return 0; 
}

int64_t libmod_ads_check_consent(INSTANCE *my, int64_t *params) { return 0; }
int64_t libmod_ads_show_consent_form(INSTANCE *my, int64_t *params) { return 0; }
int64_t libmod_ads_set_child_directed(INSTANCE *my, int64_t *params) { return 0; }

#endif

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_ads, module_initialize )() {
}

/* --------------------------------------------------------------------------- */

void __bgdexport( libmod_ads, module_finalize )() {
}

/* --------------------------------------------------------------------------- */

#include "libmod_ads_exports.h"

