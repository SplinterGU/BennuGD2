#include "bgddl.h"
#include "dlvaracc.h"
#include "xstrings.h"
#include <SDL.h>

#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "BennuGD_IAP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static struct {
    jclass classRef;
    jmethodID initMethod;
    jmethodID purchaseMethod;
    jmethodID consumeMethod;
    jmethodID getPriceMethod;
    jmethodID pollEventMethod;
    jmethodID getLastSkuMethod;
    jmethodID getLastTokenMethod;
} jniCache;

static int initialized = 0;

static int init_jni(JNIEnv *env) {
    if (initialized) return 1;

    jclass cls = (*env)->FindClass(env, "org/libsdl/app/IAPModule");
    if (!cls) { LOGE("Class org/libsdl/app/IAPModule not found"); return 0; }
    jniCache.classRef = (*env)->NewGlobalRef(env, cls);

    jniCache.initMethod = (*env)->GetStaticMethodID(env, cls, "initialize", "(Landroid/app/Activity;)V");
    jniCache.purchaseMethod = (*env)->GetStaticMethodID(env, cls, "purchase", "(Ljava/lang/String;)V");
    jniCache.consumeMethod = (*env)->GetStaticMethodID(env, cls, "consume", "(Ljava/lang/String;)V");
    jniCache.getPriceMethod = (*env)->GetStaticMethodID(env, cls, "getPrice", "(Ljava/lang/String;)Ljava/lang/String;");
    jniCache.pollEventMethod = (*env)->GetStaticMethodID(env, cls, "pollEvent", "()I");
    jniCache.getLastSkuMethod = (*env)->GetStaticMethodID(env, cls, "getLastPurchaseSku", "()Ljava/lang/String;");
    jniCache.getLastTokenMethod = (*env)->GetStaticMethodID(env, cls, "getLastPurchaseToken", "()Ljava/lang/String;");

    initialized = 1;
    return 1;
}

int64_t libmod_iap_initialize(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !init_jni(env)) return 0;

    // Get Activity
    jclass activityClass = (*env)->FindClass(env, "org/libsdl/app/SDLActivity");
    jmethodID getContext = (*env)->GetStaticMethodID(env, activityClass, "getContext", "()Landroid/content/Context;");
    jobject activity = (*env)->CallStaticObjectMethod(env, activityClass, getContext);

    (*env)->CallStaticVoidMethod(env, jniCache.classRef, jniCache.initMethod, activity);
    (*env)->DeleteLocalRef(env, activity);
    (*env)->DeleteLocalRef(env, activityClass);
    return 1;
}

int64_t libmod_iap_purchase(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    char *str = (char *)string_get(params[0]);
    jstring jstr = (*env)->NewStringUTF(env, str);
    (*env)->CallStaticVoidMethod(env, jniCache.classRef, jniCache.purchaseMethod, jstr);
    (*env)->DeleteLocalRef(env, jstr);
    string_discard(params[0]);
    return 1;
}

int64_t libmod_iap_consume(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;

    char *str = (char *)string_get(params[0]);
    jstring jstr = (*env)->NewStringUTF(env, str);
    (*env)->CallStaticVoidMethod(env, jniCache.classRef, jniCache.consumeMethod, jstr);
    (*env)->DeleteLocalRef(env, jstr);
    string_discard(params[0]);
    return 1;
}

int64_t libmod_iap_get_price(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) { string_discard(params[0]); return string_new((unsigned char *)"0.00"); }

    char *sku = (char *)string_get(params[0]);
    jstring jSku = (*env)->NewStringUTF(env, sku);
    string_discard(params[0]);

    jstring jPrice = (jstring)(*env)->CallStaticObjectMethod(env, jniCache.classRef, jniCache.getPriceMethod, jSku);
    (*env)->DeleteLocalRef(env, jSku);

    if (jPrice) {
        const char *cPrice = (*env)->GetStringUTFChars(env, jPrice, 0);
        int64_t ret = string_new((unsigned char *)cPrice);
        (*env)->ReleaseStringUTFChars(env, jPrice, cPrice);
        (*env)->DeleteLocalRef(env, jPrice);
        return ret;
    }
    return string_new((unsigned char *)"N/A");
}

int64_t libmod_iap_poll_event(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return 0;
    return (*env)->CallStaticIntMethod(env, jniCache.classRef, jniCache.pollEventMethod);
}

int64_t libmod_iap_get_last_purchase_sku(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return string_new((unsigned char *)"");

    jstring jStr = (jstring)(*env)->CallStaticObjectMethod(env, jniCache.classRef, jniCache.getLastSkuMethod);
    if (!jStr) return string_new((unsigned char *)"");

    const char *cStr = (*env)->GetStringUTFChars(env, jStr, 0);
    int64_t ret = string_new((unsigned char *)cStr);
    (*env)->ReleaseStringUTFChars(env, jStr, cStr);
    (*env)->DeleteLocalRef(env, jStr);
    return ret;
}

int64_t libmod_iap_get_last_purchase_token(INSTANCE *my, int64_t *params) {
    JNIEnv *env = SDL_AndroidGetJNIEnv();
    if (!env || !initialized) return string_new((unsigned char *)"");

    jstring jStr = (jstring)(*env)->CallStaticObjectMethod(env, jniCache.classRef, jniCache.getLastTokenMethod);
    if (!jStr) return string_new((unsigned char *)"");

    const char *cStr = (*env)->GetStringUTFChars(env, jStr, 0);
    int64_t ret = string_new((unsigned char *)cStr);
    (*env)->ReleaseStringUTFChars(env, jStr, cStr);
    (*env)->DeleteLocalRef(env, jStr);
    return ret;
}

#else

// Stubs
int64_t libmod_iap_initialize(INSTANCE *my, int64_t *params) { return 0; }
int64_t libmod_iap_purchase(INSTANCE *my, int64_t *params) { string_discard(params[0]); return 0; }
int64_t libmod_iap_consume(INSTANCE *my, int64_t *params) { string_discard(params[0]); return 0; }
int64_t libmod_iap_get_price(INSTANCE *my, int64_t *params) { string_discard(params[0]); return string_new((unsigned char *)"N/A"); }
int64_t libmod_iap_poll_event(INSTANCE *my, int64_t *params) { return 0; }
int64_t libmod_iap_get_last_purchase_sku(INSTANCE *my, int64_t *params) { return string_new((unsigned char *)""); }
int64_t libmod_iap_get_last_purchase_token(INSTANCE *my, int64_t *params) { return string_new((unsigned char *)""); }

#endif

void __bgdexport( libmod_iap, module_initialize )() {}
void __bgdexport( libmod_iap, module_finalize )() {}

#include "libmod_iap_exports.h"
