LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -DANDROID_NDK \
				-O3 

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../freetype/include/ \
		$(LOCAL_PATH)/../include/ \
		$(LOCAL_PATH)/../MyGUIEngine/include/

myguiadapter_SOURCES :=\
		MyGUIVertexBuffer.cpp \
		MyGUITexture.cpp \
		MyGUIDataManager.cpp \
		MyGUIRenderManager.cpp \
		MyGUIAdapter.cpp

LOCAL_STATIC_LIBRARIES :=freetype mygui core
		
LOCAL_MODULE    := myguiadapter
		
LOCAL_SRC_FILES := \
		$(myguiadapter_SOURCES)
		
		
LOCAL_ARM_MODE   := arm 
LOCAL_LDLIBS := -ldl -llog -lstdc++ -Wl
include $(BUILD_STATIC_LIBRARY)
