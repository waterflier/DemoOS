include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=input_test
TargetType=Execute
Target=inputtest_exe

IncludeDirs =
ProjectCFlag= -I ../../inc
ProjectCppFlag= $(ProjectCFlag)

ProjectLinkFlag= 
LibDirs=$(OUTPUT)
LinkLibs= ngoskernel osi_interface toolkit
####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
