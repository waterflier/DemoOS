include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=demo_desktop
TargetType=Execute
Target=demodesktop

IncludeDirs = 
ProjectCFlag= -I ../../inc -I ../Render/RenderScript -I ../Render/RenderEngine
ProjectCppFlag=

ProjectLinkFlag=
LibDirs=$(OUTPUT)
LinkLibs=ngoskernel osi_interface uiobjects
####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
