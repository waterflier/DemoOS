include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=testKernel
TargetType=bin
Target=test_kernel

IncludeDirs = 
ProjectCFlag= -I ../../inc
ProjectCppFlag=

LibDirs=$(OUTPUT)
ProjectLinkFlag=
LinkLibs=osi_interface pthread ngoskernel uiobjects

####################################################################
cfiles := main.c
cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles)))
###################################################################
include ../../footer.mk
