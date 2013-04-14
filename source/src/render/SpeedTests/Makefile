include ../../../mk.config

SKIA_PATH=/root/win_work/skia
SKIA_OUT:=$(SKIA_PATH)/linux_out/$(CONFIG_NAME)/obj.target/gyp

###################################################################
ProjectName=render
ModuleName=renderSpeedTests
TargetType=
Target=SpeedTests


IncludeDirs = 
ProjectCFlag=
ProjectCppFlag='-DSK_GAMMA_SRGB' '-DSK_GAMMA_APPLY_TO_A8' '-DSK_ALLOW_STATIC_GLOBAL_INITIALIZERS=1' \
		'-DSK_SCALAR_IS_FLOAT' '-DSK_CAN_USE_FLOAT' '-DSK_SUPPORT_GPU=1' '-DSK_SAMPLES_FOR_X' \
		'-DSK_BUILD_FOR_UNIX' '-DSK_USE_POSIX_THREADS' '-DGR_LINUX_BUILD=1' '-DSK_DEBUG' '-DGR_DEBUG=1' \
		'-DSK_DEVELOPER=1' -I$(SKIA_PATH)/include/core -I$(SKIA_PATH)/include/config -I$(SKIA_PATH)/include/gpu \
		-I$(SKIA_PATH)/include/utils -I$(SKIA_PATH)/include/pipe -I$(SKIA_PATH)/include/images \
		-I$(SKIA_PATH)/include/device -I$(SKIA_PATH)/include/effects -I$(SKIA_PATH)/include/text \
		-I$(SKIA_PATH)/include/lazy -Wall -Wextra -Wno-unused -Wno-unused-parameter -Wno-c++11-extensions -m32 -MMD

ProjectLinkFlag:= -m32
LinkLibs:= skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils cityhash skia_gr  \
		skia_skgr jpeg pthread freetype fontconfig dl GL GLU png
LibDirs:= $(SKIA_OUT)

SubFileDirs = 

####################################################################

cppfiles := main.cpp
cppobjs := $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(notdir $(cppfiles)))

#tests:
#	@echo $(cppobjs)

###################################################################
include ../../../footer.mk
