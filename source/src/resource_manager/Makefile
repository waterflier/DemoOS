include ../../mk.config
###################################################################
ProjectName=resource_manager
ModuleName=resource_manager
TargetType=Lib
Target=libngrm.a


IncludeDirs = 
ProjectCFlag= -I. 
ProjectCppFlag= -I.

ProjectLinkFlag:= 
LinkLibs:=
LibDirs:=

SubFileDirs:= stl

####################################################################
cfiles_src:= $(wildcard *.c)
cfiles:=$(cfiles_src) 


cppfiles_stl:= $(wildcard stl/*.cpp)
cppfiles:=$(cppfiles_stl)

cobjs:= $(patsubst %.c,$(TEMP_DIR)%.co,$(cfiles))
cppobjs:= $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(cppfiles))
#tests:
#	@echo $(cfiles)
#	@echo $(cobjs)

###################################################################
include ../../footer.mk


