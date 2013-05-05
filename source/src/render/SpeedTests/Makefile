include ../../../mk.config

SKIA_PATH=/root/win_work/skia
SKIA_OUT:=$(SKIA_PATH)/linux_out/$(CONFIG_NAME)

###################################################################
ProjectName=render
ModuleName=renderSpeedTests
TargetType=
Target=SpeedTests


IncludeDirs= 
ProjectCFlag= -I. -I../RenderEngine -I../RenderScript -I../../resource_manager 
ProjectCppFlag=

ProjectLinkFlag:= 
LinkLibs:= skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils cityhash skia_gr  \
		skia_skgr jpeg pthread freetype fontconfig dl GL GLU png lua ngre_engine ngre_script ngrm
LibDirs:= $(SKIA_OUT) $(OUTPUT) $(OUTPUT)../../resource_manager/$(CONFIG_NAME)

SubFileDirs = 

####################################################################

cfiles := testScript.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))


###################################################################
include ../../../footer.mk
