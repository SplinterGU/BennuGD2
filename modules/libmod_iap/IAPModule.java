package org.libsdl.app;

import android.app.Activity;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import com.android.billingclient.api.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class IAPModule {
    private static Activity activity;
    private static BillingClient billingClient;
    private static boolean isConnected = false;

    // Events
    private static int lastEvent = 0;
    public static final int EVENT_NONE = 0;
    public static final int EVENT_SETUP_OK = 1;
    public static final int EVENT_SETUP_FAIL = 2;
    public static final int EVENT_PURCHASE_OK = 3;
    public static final int EVENT_PURCHASE_CANCEL = 4;
    public static final int EVENT_PURCHASE_FAIL = 5;
    public static final int EVENT_CONSUME_OK = 6;
    public static final int EVENT_CONSUME_FAIL = 7;

    // Last purchase info
    private static String lastSku = "";
    private static String lastToken = "";

    // Cache for SKU details (to get prices synchronously-ish)
    private static Map<String, ProductDetails> productDetailsMap = new HashMap<>();

    private static final String TAG = "IAPModule";

    public static void initialize(Activity act) {
        activity = act;
        android.util.Log.d(TAG, "IAPModule.initialize() called");

        PurchasesUpdatedListener purchasesUpdatedListener = (billingResult, purchases) -> {
            if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK && purchases != null) {
                for (Purchase purchase : purchases) {
                    handlePurchase(purchase);
                }
            } else if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.USER_CANCELED) {
                lastEvent = EVENT_PURCHASE_CANCEL;
            } else {
                lastEvent = EVENT_PURCHASE_FAIL;
            }
        };

        billingClient = BillingClient.newBuilder(activity)
                .setListener(purchasesUpdatedListener)
                .enablePendingPurchases()
                .build();

        connectToGooglePlay();
    }

    private static void connectToGooglePlay() {
        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(BillingResult billingResult) {
                if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    isConnected = true;
                    lastEvent = EVENT_SETUP_OK;
                    android.util.Log.d(TAG, "Billing setup SUCCESS");
                } else {
                    lastEvent = EVENT_SETUP_FAIL;
                    android.util.Log.e(TAG, "Billing setup FAILED: " + billingResult.getResponseCode());
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
                isConnected = false;
                // Try to restart connection later if needed
            }
        });
    }

    public static int pollEvent() {
        int e = lastEvent;
        lastEvent = EVENT_NONE;
        return e;
    }

    public static String getLastPurchaseSku() {
        return lastSku;
    }

    public static String getLastPurchaseToken() {
        return lastToken;
    }

    public static String getPrice(String sku) {
        // Since getPrice is synchronous in our API but async in Google,
        // we check cache first. If not found, we trigger a query but return "..."
        // This is a simplification. Ideally, user should call IAP_QUERY_PRODUCTS first.
        // For now, we try to return cached price or "Scanning..."

        if (productDetailsMap.containsKey(sku)) {
            ProductDetails detail = productDetailsMap.get(sku);
            // Billing v5/v6 structure:
            if (detail.getProductType().equals(BillingClient.ProductType.INAPP)) {
                if (detail.getOneTimePurchaseOfferDetails() != null) {
                    return detail.getOneTimePurchaseOfferDetails().getFormattedPrice();
                }
            }
        } else {
            // Trigger query for this SKU
            querySkuDetails(sku);
        }
        return "...";
    }

    private static void querySkuDetails(String sku) {
        if (!isConnected)
            return;

        List<QueryProductDetailsParams.Product> productList = new ArrayList<>();
        productList.add(
                QueryProductDetailsParams.Product.newBuilder()
                        .setProductId(sku)
                        .setProductType(BillingClient.ProductType.INAPP)
                        .build());

        QueryProductDetailsParams params = QueryProductDetailsParams.newBuilder()
                .setProductList(productList)
                .build();

        billingClient.queryProductDetailsAsync(params, (billingResult, productDetailsList) -> {
            if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                for (ProductDetails pd : productDetailsList) {
                    productDetailsMap.put(pd.getProductId(), pd);
                }
            }
        });
    }

    public static void purchase(String sku) {
        android.util.Log.d(TAG, "purchase() called with SKU: " + sku);

        if (!isConnected) {
            android.util.Log.e(TAG, "purchase() FAILED: not connected");
            lastEvent = EVENT_SETUP_FAIL;
            return;
        }

        // Special handling for android.test.* SKUs (they don't have ProductDetails)
        if (sku.startsWith("android.test.")) {
            android.util.Log.d(TAG, "Using test SKU, creating mock ProductDetails");
            // For test SKUs, we can't query details, so we launch with minimal params
            // This is a workaround - test SKUs should work without ProductDetails in older
            // APIs
            // but Billing Library v5+ requires them. We'll try a different approach.
            lastEvent = EVENT_PURCHASE_FAIL;
            android.util.Log.e(TAG, "Test SKUs (android.test.*) are not fully supported with Billing Library 6.0+");
            android.util.Log.e(TAG, "Please use a real product ID configured in Google Play Console");
            return;
        }

        // Ensure we have details
        if (!productDetailsMap.containsKey(sku)) {
            android.util.Log.w(TAG, "Product details not cached for: " + sku);
            querySkuDetails(sku); // Try to fetch for next time
            lastEvent = EVENT_PURCHASE_FAIL;
            return;
        }

        ProductDetails productDetails = productDetailsMap.get(sku);
        List<BillingFlowParams.ProductDetailsParams> productDetailsParamsList = new ArrayList<>();
        productDetailsParamsList.add(
                BillingFlowParams.ProductDetailsParams.newBuilder()
                        .setProductDetails(productDetails)
                        .build());

        BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                .setProductDetailsParamsList(productDetailsParamsList)
                .build();

        billingClient.launchBillingFlow(activity, billingFlowParams);
    }

    private static void handlePurchase(Purchase purchase) {
        if (purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED) {
            lastSku = purchase.getProducts().get(0); // v6 returns list
            lastToken = purchase.getPurchaseToken();
            lastEvent = EVENT_PURCHASE_OK;

            // Acknowledge if not consumed (for non-consumables)
            if (!purchase.isAcknowledged()) {
                AcknowledgePurchaseParams acknowledgePurchaseParams = AcknowledgePurchaseParams.newBuilder()
                        .setPurchaseToken(purchase.getPurchaseToken())
                        .build();
                billingClient.acknowledgePurchase(acknowledgePurchaseParams, billingResult -> {
                });
            }
        }
    }

    public static void consume(String purchaseToken) {
        if (!isConnected)
            return;

        ConsumeParams consumeParams = ConsumeParams.newBuilder()
                .setPurchaseToken(purchaseToken)
                .build();

        billingClient.consumeAsync(consumeParams, (billingResult, purchaseTokenOut) -> {
            if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                lastEvent = EVENT_CONSUME_OK;
            } else {
                lastEvent = EVENT_CONSUME_FAIL;
            }
        });
    }
}
