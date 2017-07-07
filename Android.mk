LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := gdxpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/core/src

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/core/src/*.cpp) \
	$(wildcard $(LOCAL_PATH)/core/src/math/*.cpp) \
	$(wildcard $(LOCAL_PATH)/core/src/math/collision/*.cpp) \
	$(wildcard $(LOCAL_PATH)/core/src/graphics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/core/src/graphics/glutils/*.cpp))

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -lGLESv3 -llog -landroid

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image

include $(BUILD_SHARED_LIBRARY)
