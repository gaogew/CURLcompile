LOCAL_PATH := $(call my-dir)

#crypto
include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := ./openssl/libcrypto.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openssl/
include $(PREBUILT_SHARED_LIBRARY)


#openssl
include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := ./openssl/libssl.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openssl/
include $(PREBUILT_SHARED_LIBRARY)

#libz
include $(CLEAR_VARS)
LOCAL_MODULE := libz
LOCAL_SRC_FILES := ./openssl/libz.so
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openssl/
include $(PREBUILT_SHARED_LIBRARY)


#libcurl
include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := ./curllib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

#ourLib
include $(CLEAR_VARS)
LOCAL_CFLAGS := -D_GNU_SOURCE
LOCAL_CPPFLAGS := -frtti
LOCAL_MODULE:= hello-jni
LOCAL_STATIC_LIBRARIES := libcurl
LOCAL_SHARED_LIBRARIES := crypto ssl libz
LOCAL_SRC_FILES := hello-jni.c
LOCAL_LDLIBS := -llog -lz
include $(BUILD_SHARED_LIBRARY)

