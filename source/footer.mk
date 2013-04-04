#
# footer.mk
# 
##########################################################################

ALL_OBJS:=$(cobjs) $(cppobjs) $(asmobjs)
ALL_DEPENDS:= $(cobjs:.co=.cd)

LIB_FLAGS := $(foreach name,$(LinkLibs),-l$(name)) 
LIB_FLAGS += $(foreach name,$(LibDirs),-L$(name)) 

CFLAGS := $(GLOBAL_CFLAGS) $(ProjectCFlag)
LFLAGS := $(LIB_FLAGS) $(GLOBAL_LFLAGS) $(ProjectLFlag) 
###########################################################################	
$(TEMP_DIR)%.cd : %.c
	@echo Dpends $<
	@set -e; rm -f $@; \
        $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.co $@ : ,g' < $@.$$$$ > $@; \
	
$(TEMP_DIR)%.co : %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -o $@ -c $<
	
$(TEMP_DIR)%.cppo : %.cpp
	@echo CPPC $<
	@$(CPPC) $(CPPFLAGS) -o $@ -c $<
	
complie:builddir $(ALL_OBJS)

link:builddir $(OUTPUT)$(Target)

$(OUTPUT)$(Target):$(ALL_OBJS)

ifeq ($(TargetType),Lib)
	@echo "LINK lib $(Target) ..."
	$(ARLIB) rc $(OUTPUT)$(Target) $(ALL_OBJS)
else
ifeq ($(TargetType),ShareLib)
	@echo "LINK sharelib $(Target) ..."
else
	@echo "LINK execute $(Target) ..."
	$(LINKER) -o $(OUTPUT)$(Target) $(ALL_OBJS) $(LFLAGS) 
endif
endif
	@echo Done.

builddir:
	@mkdir -p $(OUTPUT)
	@mkdir -p $(TEMP_DIR)
	@for sdir in $(SubFileDirs); do mkdir -p $(TEMP_DIR)$$sdir;done
	
	
all:builddir complie link

.PHONY:clean

clean:
	rm -rf $(OUTPUT)$(Target)
	find $(TEMP_DIR) -name "*.co" -exec rm -f {} \;
	find $(TEMP_DIR) -name "*.cd" -exec rm -f {} \;
	find $(TEMP_DIR) -name "*.cppo" -exec rm -f {} \;
	find $(TEMP_DIR) -name "*.asmo" -exec rm -f {} \;
	
info:
	@echo $(ALL_OBJS)
	@echo $(cfiles)
