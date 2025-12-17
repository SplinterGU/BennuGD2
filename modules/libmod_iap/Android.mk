LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libmod_iap
LOCAL_SRC_FILES := libmod_iap.c
LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := bgdrt
include $(BUILD_SHARED_LIBRARY)
