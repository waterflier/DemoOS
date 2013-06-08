include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=toolkit
TargetType=Lib
Target=libtoolkit.a

IncludeDirs = 
ProjectCFlag= -I ../../inc
ProjectCppFlag=

ProjectLinkFlag=
LinkLibss=

####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))

cobjs := $(cobjs_root) $(cobjs1)
cfiles := $(cfiles_root) $(cfiles1)
###################################################################
include ../../footer.mk
