include ../../../mk.config
###################################################################

SkiaFlag:= -I$(SKIA_PATH)/include/core -I$(SKIA_PATH)/include/config -I$(SKIA_PATH)/include/gpu \
		-I$(SKIA_PATH)/include/utils -I$(SKIA_PATH)/include/pipe -I$(SKIA_PATH)/include/images \
		-I$(SKIA_PATH)/include/device -I$(SKIA_PATH)/include/effects -I$(SKIA_PATH)/include/text \
		-I$(SKIA_PATH)/include/lazy -I$(SKIA_PATH)/include -I$(SKIA_PATH)/gpu/include -I$(SKIA_PATH)/
		
ifeq ($(ARCH), android)
	SkiaFlag+=-I$(ANDROID_PATH)/hardware/libhardware_legacy/include \
			-I$(ANDROID_PATH)/hardware/ril/include \
			-I$(ANDROID_PATH)/dalvik/libnativehelper/include \
			-I$(ANDROID_PATH)/frameworks/base/include \
			-I$(ANDROID_PATH)/frameworks/base/opengl/include \
			-I$(ANDROID_PATH)/frameworks/base/native/include \
			-I$(ANDROID_PATH)/out/target/product/generic/obj/include \
			-I$(ANDROID_PATH)/bionic/libthread_db/include \
			-I$(ANDROID_PATH)/bionic/libstdc++/include 
endif

ifeq ($(ARCH), android41)
	SkiaFlag+=-DSK_BUILD_FOR_ANDROID -DANDROID -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES -I$(ANDROID_PATH)/hardware/libhardware_legacy/include \
			-I$(ANDROID_PATH)/hardware/ril/include \
			-I$(ANDROID_PATH)/dalvik/libnativehelper/include \
			-I$(ANDROID_PATH)/frameworks/base/include \
			-I$(ANDROID_PATH)/frameworks/native/opengl/include \
			-I$(ANDROID_PATH)/frameworks/native/include \
			-I$(ANDROID_PATH)/out/target/product/grouper/obj/include \
			-I$(ANDROID_PATH)/bionic/libthread_db/include \
			-I$(ANDROID_PATH)/bionic/libstdc++/include 
endif

ifeq ($(ARCH), linux)
	SkiaFlag+='-DSK_GAMMA_SRGB' '-DSK_GAMMA_APPLY_TO_A8' '-DSK_ALLOW_STATIC_GLOBAL_INITIALIZERS=1' \
		'-DSK_SCALAR_IS_FLOAT' '-DSK_CAN_USE_FLOAT' '-DSK_SUPPORT_GPU=1' '-DSK_SAMPLES_FOR_X' \
		'-DSK_BUILD_FOR_UNIX' '-DSK_USE_POSIX_THREADS' '-DGR_LINUX_BUILD=1' '-DSK_DEBUG' '-DGR_DEBUG=1' \
		'-DSK_DEVELOPER=1' -Wall -Wextra -Wno-unused -Wno-unused-parameter -Wno-c++11-extensions -MMD
endif

ProjectName=render
ModuleName=renderEngine
TargetType=Lib
Target=libngre_engine.a


IncludeDirs = 
ProjectCFlag= -I. -I../RenderScript -I ../../../inc
ProjectCppFlag= -I. -I../RenderScript -I ../../../inc $(SkiaFlag) -I$(ANDROID_PATH)/hardware/libhardware/include

ProjectLinkFlag:= -m32
LinkLibs:=
LibDirs:=

SubFileDirs:= skia 
SubFileDirs+= egl
#SubFileDirs+= fb

####################################################################
cfiles_src:= $(wildcard *.c)
cfiles_skia:= $(wildcard skia/*.c)
#cfiles_fb:= $(wildcard fb/*.c)
cfiles:=$(cfiles_src) $(cfiles_skia) $(cfiles_fb)

cppfiles_skia:= $(wildcard skia/*.cpp)
cppfiles_egl:= $(wildcard egl/*.cpp)
cppfiles:=$(cppfiles_skia) $(cppfiles_egl)

cobjs:= $(patsubst %.c,$(TEMP_DIR)%.co,$(cfiles))
cppobjs:= $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(cppfiles))
#tests:
#	@echo $(cfiles)
#	@echo $(cobjs)

###################################################################
include ../../../footer.mk


