include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=os_interface
TargetType=Lib
Target=libosi_interface.a

IncludeDirs = 
ProjectCFlag= -I ../../inc
ProjectCppFlag=

ProjectLinkFlag=
LinkLibss=
SubDir = linux
SubFileDirs = linux
ifeq ($(ARCH),android)
SubDir = android
SubFileDirs = android
endif


####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))
 
cfiles1 := $(wildcard $(SubDir)/*.c)
cobjs1 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles1))

cobjs := $(cobjs_root) $(cobjs1)
cfiles := $(cfiles_root) $(cfiles1)
###################################################################
include ../../footer.mk
