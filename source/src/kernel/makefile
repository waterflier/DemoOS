include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=ngoskernel
TargetType=Lib
Target=libngoskernel.a

IncludeDirs = 
ProjectCFlag= -I ../../inc -I ../render/RenderScript -I ../render/RenderEngine
ProjectCppFlag=

ProjectLinkFlag=
LinkLibss=
 
SubFileDirs := objectIndex event input math animation

####################################################################
cfiles_root := $(wildcard *.c)
cobjs_root := $(patsubst %.c,$(TEMP_DIR)%.co,$(notdir $(cfiles_root)))

SubDir = objectIndex
cfiles1 := $(wildcard $(SubDir)/*.c)
cobjs1 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles1))

SubDir = event
cfiles2 := $(wildcard $(SubDir)/*.c)
cobjs2 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles2))

SubDir = input
cfiles3 := $(wildcard $(SubDir)/*.c)
cobjs3 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles3))

SubDir = math
cfiles4 := $(wildcard $(SubDir)/*.c)
cobjs4 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles4))

SubDir = animation
cfiles5 := $(wildcard $(SubDir)/*.c)
cobjs5 := $(patsubst %.c,$(TEMP_DIR)%.co, $(cfiles5))

cobjs := $(cobjs_root) $(cobjs1) $(cobjs2) $(cobjs3) $(cobjs4) $(cobjs5)
cfiles := $(cfiles_root) $(cfiles1) $(cfiles2) $(cfiles3) $(cfiles4) $(cfiles5) 
###################################################################
include ../../footer.mk
