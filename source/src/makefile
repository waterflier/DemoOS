include ../mk.config
###################################################################
ProjectName=test
ModuleName=testApp
TargetType=Lib
Target=libosi_interface.a

IncludeDirs = 
ProjectCFlag=
ProjectCppFlag=

ProjectLinkFlag=
LinkLibss=

SubFileDirs = sub_test sub_test/sub sub_test3

####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))


cfiles2 := $(wildcard $(SubDir)/*.c)
cobjs2 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles2))

cfiles3 := $(wildcard $(SubDir2)/*.c)
cobjs3 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles3))

cobjs := $(cobjs1) $(cobjs2) $(cobjs3)
cfiles := $(cfiles_root) $(cfiles2) $(cfiles3)
###################################################################
include ../footer.mk
