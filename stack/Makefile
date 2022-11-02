.PHONY: all clean debug release

include CMakefile

#Compiler flags
CFLAGS=$(CFLAGS_WARNINGS) $(CFLAGS_SYNTAX)
LD=ld
LDFLAGS=-relocatable
CCLDFLAGS=$(CFLAGS_LINK)


SOURCEDIR=src/code
HEADERDIR=src/include
BUILDDIR=build
SOURCES=$(wildcard $(SOURCEDIR)/*.c)
HEADFILES=$(wildcard $(HEADERDIR)/*.h)
OBJFILES=$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE=$(BUILDDIR)/libstack.o


all: dir $(EXECUTABLE)

debug: CFLAGS+= $(CFLAGS_FLAGS) $(CFLAGS_DEBUG) 
debug: all

release: CFLAGS+= -D NDEBUG -D NDEBUG_LOG -D NDEBUG_HASH -D NDEBUG_CANARY
release: all

dir:
	mkdir $(BUILDDIR)

$(EXECUTABLE): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $?

$(OBJFILES): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADFILES)
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) -o $@ $<


clean:
	rm -rf $(BUILDDIR)

