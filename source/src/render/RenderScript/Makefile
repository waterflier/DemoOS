include ../../../mk.config
###################################################################
ProjectName=render
ModuleName=renderScript
TargetType=Lib
Target=libngre_script.a


IncludeDirs = 
ProjectCFlag= -I. -I../RenderEngine -I../../resource_manager -I../../../inc
ProjectCFlag+=-I../../lua
ProjectCppFlag=

ProjectLinkFlag:= -m32
LinkLibs:=
LibDirs:=

SubFileDirs:= lua 

####################################################################
cfiles_src:= $(wildcard *.c)
cfiles_lua:= $(wildcard lua/*.c)
cfiles:=$(cfiles_src) $(cfiles_lua)

cobjs := $(patsubst %.c,$(TEMP_DIR)%.co,$(cfiles))
#tests:
#	@echo $(cfiles)
#	@echo $(cobjs)

###################################################################
include ../../../footer.mk


