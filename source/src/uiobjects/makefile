include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=uibojects
TargetType=Lib
Target=libuiobjects.a

IncludeDirs = 
ProjectCFlag= -I ../../inc -I ../Render/RenderScript -I ../Render/RenderEngine
ProjectCppFlag=

ProjectLinkFlag=
LinkLibss=
 
SubFileDirs := 

####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))

cobjs := $(cobjs_root)
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
