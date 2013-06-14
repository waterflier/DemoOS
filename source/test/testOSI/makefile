include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=testOSI
TargetType=bin
Target=test_osi

IncludeDirs = 
ProjectCFlag= -I ../../inc
ProjectCppFlag=

LibDirs=$(OUTPUT)
ProjectLinkFlag=
LinkLibs=osi_interface c m cutils

####################################################################
cfiles := testOSI.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))
###################################################################
include ../../footer.mk
