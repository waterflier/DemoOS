include ../../../mk.config

###################################################################
ProjectName=render
ModuleName=renderSpeedTests
TargetType=
Target=SpeedTests


IncludeDirs= 
ProjectCFlag= -I. -I../RenderEngine -I../RenderScript -I../../resource_manager -I../../../inc
ProjectCppFlag=

ProjectLinkFlag:= 
LinkLibs:= lua ngre_engine ngre_script ngrm
ifeq ($(ARCH), android)
	LinkLibs += skia stdc++ stlport c m
else
	LinkLibs += cityhash jpeg pthread freetype fontconfig dl GL GLU png skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils skia_gr skia_skgr 
endif

LibDirs:= $(OUTPUT) $(OUTPUT)../../resource_manager/$(CONFIG_NAME)

SubFileDirs = 

####################################################################

cfiles := testScript.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))


###################################################################
include ../../../footer.mk
