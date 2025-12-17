# Mod Ads for BennuGD2 (Android)

This module allows you to monetize your BennuGD2 games on Android using Google AdMob (Banners and Interstitials).

## Integration Steps

To use this module in your Android project, you need to perform the following steps during your build process or project setup:

### 1. Java Source
Copy `AdsModule.java` to your Android project's source directory, in the `org.libsdl.app` package structure.
Example path: `app/src/main/java/org/libsdl/app/AdsModule.java`

### 2. Gradle Dependencies
Add the AdMob dependency to your `app/build.gradle` file (inside `dependencies` block):
```gradle
implementation 'com.google.android.gms:play-services-ads:21.5.0'
```

### 3. AndroidManifest.xml
Add the AdMob App ID meta-data to your `AndroidManifest.xml` inside the `<application>` tag:
```xml
<meta-data
    android:name="com.google.android.gms.ads.APPLICATION_ID"
    android:value="YOUR_ADMOB_APP_ID_HERE"/>
```
*Note: Replace `YOUR_ADMOB_APP_ID_HERE` with your actual AdMob App ID. For testing, you can use the sample ID: `ca-app-pub-3940256099942544~3347511713`*

Also ensure you have the required permissions:
```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
```

### 4. SDLActivity Integration
If you have a custom `SDLActivity.java`, ensure you handle the Banner Ad lifecycle events:
- In `onPause()`, call `AdsModule.hideBanner()` or access the ad view to pause it.
- In `onDestroy()`, ensure cleanup.
(The provided `AdsModule.java` handles `destroy` inside `hideBanner` and `showBanner` for safety).

## BennuGD Usage

Use the following functions in your BennuGD script (see `test_ads.prg` for example):

```bennugd
// Initialize
ADS_INITIALIZE();

// Load an Interstitial
// (Call this early, e.g., at start of level)
ADS_LOAD_INTERSTITIAL("YOUR_INTERSTITIAL_UNIT_ID");

// Check if loaded and show
IF (ADS_IS_INTERSTITIAL_LOADED())
    ADS_SHOW_INTERSTITIAL();
END

// Show Banner
// Position: ADS_BANNER_POS_TOP or ADS_BANNER_POS_BOTTOM
ADS_SHOW_BANNER("YOUR_BANNER_UNIT_ID", ADS_BANNER_POS_BOTTOM);

// Hide Banner
ADS_HIDE_BANNER();
```

## Cross-Platform Support

This module is designed to compile on all platforms (Windows, Linux, macOS) to support cross-compilation and bytecode generation.

- **On Android**: The functions are fully implemented and use JNI to interact with the Google Mobile Ads SDK.
- **On other platforms**: The functions exist but do nothing (stubs). They return 0 or 1 as appropriate but have no effect. This allows tools like `bgdc` to compile scripts that use this module without errors.
