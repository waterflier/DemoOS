include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=input
TargetType=Execute
Target=inputtest

IncludeDirs = 
ProjectCFlag= -I ../../inc
ProjectCppFlag=

ProjectLinkFlag=
LibDirs=$(OUTPUT)
LinkLibs=
####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
