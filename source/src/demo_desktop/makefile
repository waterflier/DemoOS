include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=demo_desktop
TargetType=Execute
Target=demo

IncludeDirs = 
ProjectCFlag= -I ../../inc -I ../../src/render/RenderScript -I ../../src/render/RenderEngine -I../../src/resource_manager 
ProjectCppFlag=

ProjectLinkFlag=
LibDirs=$(OUTPUT) $(SKIA_OUT) $(OUTPUT)../../resource_manager/$(CONFIG_NAME) $(OUTPUT)../../render/$(CONFIG_NAME)
LinkLibs=ngoskernel osi_interface uiobjects inputreader toolkit lua ngre_engine ngre_script ngrm

ifeq ($(ARCH), android)
	LinkLibs += skia stdc++ stlport c m EGL hardware GLESv1_CM ui
else
ifeq ($(ARCH), android41)
	LinkLibs += skia stdc++ stlport c m EGL hardware GLESv1_CM ui
else
	LinkLibs += rt pthread jpeg pthread freetype fontconfig dl GL GLU png skia_effects skia_images skia_core skia_opts skia_opts_ssse3 skia_ports skia_sfnt skia_utils skia_gr skia_skgr 
endif
endif

####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
