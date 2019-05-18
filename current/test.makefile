
# define a root build directory base on the platform
# if without a SRC_BASE defined, just use local src directory

ifeq ($(DEBUG_SYMBOLS), TRUE)
BUILDDIR = ./build/$(PLATFORM)_dbg
else
BUILDDIR = ./build/$(PLATFORM)
endif

OBJDIR = $(BUILDDIR)/obj/$(MODULE)
LIBDIR = $(BUILDDIR)/lib
BINDIR = $(BUILDDIR)/bin


ifeq ($(MAKELEVEL), 1)
	all : msg
else
all:lib bin
endif

lib : $(OBJDIR) $(SRC_LIB)

bin : $(OBJDIR) $(SRC_BIN)

$(OBJDIR) :
	@echo "   MKDIR $(notdir $@)..."
	@mkdir -p $@

