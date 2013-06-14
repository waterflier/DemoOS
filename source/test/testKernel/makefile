include ../../mk.config

SKIA_PATH=/root/skia/trunk
SKIA_OUT:=$(SKIA_PATH)/linux_out/$(CONFIG_NAME)
###################################################################
ProjectName=DemoOS
ModuleName=testKernel
TargetType=bin
Target=test_kernel

IncludeDirs = 
ProjectCFlag= -I ../../inc -I ../../src/Render/RenderScript -I ../../src/Render/RenderEngine -I../../src/resource_manager 
ProjectCppFlag=

LibDirs=$(SKIA_OUT) $(OUTPUT) $(OUTPUT)../../resource_manager/$(CONFIG_NAME) $(OUTPUT)../../render/$(CONFIG_NAME)
ProjectLinkFlag=
LinkLibs=osi_interface pthread ngoskernel uiobjects rt skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils cityhash skia_gr  \
		skia_skgr pthread freetype fontconfig dl GL GLU png lua ngre_engine ngre_script ngrm

####################################################################
cfiles := main.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))
###################################################################
include ../../footer.mk
