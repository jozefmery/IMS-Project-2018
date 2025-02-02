#	File:    Makefile
#	Date:    20.11.2018
# 	Project: IMS - Corn Growth Simulation
#	Authors: 
#				Denis Dovičic	xdovic01@stud.fit.vutbr.cz
#			 	Jozef Méry 		xmeryj00@stud.fit.vutbr.cz

# $@ - target
# $< - first dep
# $^ - all deps

LOGIN1 		= xmeryj00
LOGIN2		= xdovic01

# use this for final archive name
ARCHIVE 	= 05_$(LOGIN1)_$(LOGIN2)
ARCHIVEEXT  = zip

# directory definitions
BINDIR 		= bin
SRCDIR		= src
OBJDIR 		= obj
INCLUDEDIR 	= include

# archive content definition
ARCHIVELIST = $(SRCDIR)/ $(INCLUDEDIR)/ Makefile README technicka_sprava.pdf

# helper programs
ARCHIVER 	= zip -r
DIRMAKER 	= @mkdir -p

# target name
TARGET      = cornsim

# file extensions
SRCEXT 		= cpp
OBJEXT      = o
HDREXT 		= hpp

# compiler options 
CC          = g++
PLATFORM	= -m64
CFLAGS      = -pedantic -Wextra -Wall $(PLATFORM)
RELCFLAGS	= -O2 -s -DNDEBUG -flto
DCFLAGS		= -g -O0
STD			= c++17
EXTRACFLAGS = -Werror

# additional includes
INCLUDES 	= $(addprefix -I,)

# linker options
LFLAGS		= $(PLATFORM)

# link libraries
LIBS		= $(addprefix -l, simlib)
LIBDIRS		= $(addprefix -L, )

default: release
all: default
.PHONY: default all clean run archive crun debug release

RELDIR	= Release
DDIR	= Debug

# fetch sources
SOURCES  = $(wildcard $(SRCDIR)/*.$(SRCEXT))
# convert to obj name
RELOBJECTS  = $(patsubst $(SRCDIR)/%.$(SRCEXT), $(OBJDIR)/$(RELDIR)/%.$(OBJEXT), $(SOURCES))
DOBJECTS  = $(patsubst $(SRCDIR)/%.$(SRCEXT), $(OBJDIR)/$(DDIR)/%.$(OBJEXT), $(SOURCES))
# fetch headers
HEADERS  = $(wildcard $(INCLUDEDIR)/**/*.$(HDREXT))

# object directory target
$(OBJDIR):
	 $(DIRMAKER) $(OBJDIR)

$(OBJDIR)/$(DDIR): $(OBJDIR)
	$(DIRMAKER) $(OBJDIR)/$(DDIR)

$(OBJDIR)/$(RELDIR): $(OBJDIR)
	$(DIRMAKER) $(OBJDIR)/$(RELDIR)

# binary directory target
$(BINDIR):
	$(DIRMAKER) $(BINDIR)

# compile in release mode
$(OBJDIR)/$(RELDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(HEADERS) | $(OBJDIR)/$(RELDIR)
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -I./$(INCLUDEDIR) $(INCLUDES) -std=$(STD) $(RELCFLAGS) -c $< -o $@

# link release objects
$(BINDIR)/$(TARGET): $(RELOBJECTS) | $(BINDIR)
	$(CC) $^ $(LIBS) $(LIBDIRS) $(LFLAGS) -o $@

# compile in debug mode
$(OBJDIR)/$(DDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(HEADERS) | $(OBJDIR)/$(DDIR)
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -I./$(INCLUDEDIR) $(INCLUDES) -std=$(STD) $(DCFLAGS) -c $< -o $@

# link debug objects
$(BINDIR)/$(TARGET)_d: $(DOBJECTS) | $(BINDIR)
	$(CC) $^ $(LIBS) $(LIBDIRS) $(LFLAGS) -o $@

release: $(BINDIR)/$(TARGET)
debug: $(BINDIR)/$(TARGET)_d

# run
run: release
	@./$(BINDIR)/$(TARGET) $(ARGS)

# run with clear
crun: release
	@clear
	@./$(BINDIR)/$(TARGET) $(ARGS)

# clean directory
clean:
	-rm -rf $(OBJDIR)/
	-rm -rf $(BINDIR)/
	-rm -f  $(ARCHIVE).$(ARCHIVEEXT)

# create final archive
archive:
	$(ARCHIVER) $(ARCHIVE).$(ARCHIVEEXT) $(ARCHIVELIST)