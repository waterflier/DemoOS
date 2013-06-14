include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=inputreader
TargetType=Lib
Target=libinputreader.a

IncludeDirs =
ProjectCFlag= -I ../../inc
ProjectCppFlag= $(ProjectCFlag)

ProjectLinkFlag= 
LinkLibs=
####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
