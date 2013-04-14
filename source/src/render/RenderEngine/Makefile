include ../../../mk.config
###################################################################
SKIA_PATH=/root/win_work/skia
SKIA_OUT:=$(SKIA_PATH)/linux_out/$(CONFIG_NAME)/obj.target/gyp

SkiaFlag:='-DSK_GAMMA_SRGB' '-DSK_GAMMA_APPLY_TO_A8' '-DSK_ALLOW_STATIC_GLOBAL_INITIALIZERS=1' \
		'-DSK_SCALAR_IS_FLOAT' '-DSK_CAN_USE_FLOAT' '-DSK_SUPPORT_GPU=1' '-DSK_SAMPLES_FOR_X' \
		'-DSK_BUILD_FOR_UNIX' '-DSK_USE_POSIX_THREADS' '-DGR_LINUX_BUILD=1' '-DSK_DEBUG' '-DGR_DEBUG=1' \
		'-DSK_DEVELOPER=1' -I$(SKIA_PATH)/include/core -I$(SKIA_PATH)/include/config -I$(SKIA_PATH)/include/gpu \
		-I$(SKIA_PATH)/include/utils -I$(SKIA_PATH)/include/pipe -I$(SKIA_PATH)/include/images \
		-I$(SKIA_PATH)/include/device -I$(SKIA_PATH)/include/effects -I$(SKIA_PATH)/include/text \
		-I$(SKIA_PATH)/include/lazy -Wall -Wextra -Wno-unused -Wno-unused-parameter -Wno-c++11-extensions -m32 -MMD
		
ProjectName=render
ModuleName=renderEngine
TargetType=Lib
Target=libngre_engine.a


IncludeDirs = 
ProjectCFlag= -I. -I../RenderScript 
ProjectCppFlag= -I. -I../RenderScript $(SkiaFlag)

ProjectLinkFlag:= -m32
LinkLibs:=
LibDirs:=

SubFileDirs:= skia

####################################################################
cfiles_src:= $(wildcard *.c)
cfiles_skia:= $(wildcard skia/*.c)
cfiles:=$(cfiles_src) $(cfiles_skia)

cppfiles_skia:= $(wildcard skia/*.cpp)
cppfiles:=$(cppfiles_skia)

cobjs:= $(patsubst %.c,$(TEMP_DIR)%.co,$(cfiles))
cppobjs:= $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(cppfiles))
#tests:
#	@echo $(cfiles)
#	@echo $(cobjs)

###################################################################
include ../../../footer.mk


