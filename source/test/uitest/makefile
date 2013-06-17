include ../../mk.config
###################################################################
ProjectName=DemoOS
ModuleName=uitest
TargetType=Execute
Target=uitest_exe
ANDROID_SOURCE=/media/android

IncludeDirs = -I$(ANDROID_PATH)/frameworks/native/include \
			 -I$(ANDROID_PATH)/hardware/libhardware/include \
			-I$(ANDROID_PATH)/hardware/ril/include \
			-I$(ANDROID_PATH)/frameworks/base/include \
			-I$(ANDROID_PATH)/frameworks/native/opengl/include \
			-I$(ANDROID_PATH)/frameworks/native/include \
			-I$(ANDROID_PATH)/out/target/product/grouper/obj/include \

ProjectCFlag= 
ProjectCppFlag=$(IncludeDirs) $(ProjectCFlag)

ProjectLinkFlag= 
LinkLibs+=utils log ui binder gui dvm stdc++ stlport c m hardware EGL GLESv1_CM android_runtime cutils
####################################################################
cfiles_root := $(wildcard *.cpp)
cobjs_root := $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
