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
ProjectCppFlag=$(IncludeDirs) $(ProjectCFlag)  -fno-exceptions -Wno-multichar -msoft-float -fpic -fPIE -ffunction-sections -fdata-sections -funwind-tables -fstack-protector  -fno-short-enums -march=armv7-a -mfloat-abi=softfp -mfpu=neon -Wno-unused-but-set-variable -fno-builtin-sin -fno-strict-volatile-bitfields -Wno-psabi -mthumb-interwork  -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith -g -Wstrict-aliasing=2 -fgcse-after-reload -frerun-cse-after-loop -frename-registers -fvisibility-inlines-hidden  -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith -Wsign-promo -Werror=return-type -Werror=non-virtual-dtor -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing  -fno-rtti 

ProjectLinkFlag= 
LinkLibs+=utils log ui binder gui dvm stdc++ stlport c m hardware EGL GLESv1_CM android_runtime cutils
####################################################################
cfiles_root := $(wildcard *.cpp)
cobjs_root := $(patsubst %.cpp,$(TEMP_DIR)%.cppo,$(notdir $(cfiles_root)))



cobjs := $(cobjs_root) 
cfiles := $(cfiles_root)
###################################################################
include ../../footer.mk
