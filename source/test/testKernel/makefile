include ../../mk.config
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


LinkLibs:=osi_interface ngoskernel uiobjects lua ngre_engine ngre_script ngrm toolkit

ifeq ($(ARCH), android)
	LinkLibs += skia stdc++ stlport c m cutils 
else
	LinkLibs += rt pthread cityhash jpeg pthread freetype fontconfig dl GL GLU png skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils skia_gr skia_skgr 
endif

####################################################################
cfiles := main.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))
###################################################################
include ../../footer.mk
