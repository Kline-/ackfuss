## Compiler to use. Either g++ or your own flavor of colorgcc, ccache, etc.
# Default: g++
CXX   = g++

### Name of binary file to generate.
# Default: ack
PROG = ack

## Compile mode. Either "RELEASE" or "DEBUG". Defaults to DEBUG otherwise.
# Default: DEBUG
MODE = DEBUG


### END CONFIGURATION OPTIONS ###


ifeq '$(MODE)' 'RELEASE'
	CXX_FLAGS = -w -O3 -std=c++0x
else
	CXX_FLAGS = -O0 -ggdb3 -pg -std=c++0x
endif

#-Wwrite-strings -Wconversion
MAKEFLAGS = -s
W_FLAGS = -Wall -Wformat-security -Wpointer-arith -Wredundant-decls -Wcast-align -Wshadow -Wno-write-strings -Werror
L_FLAGS = -lcrypt -lm -lpthread -ldl

C_FILES = $(wildcard *.c *.cpp)
O_FILES = $(patsubst %.cpp,o/%.o,$(patsubst %.c,o/%.o,$(C_FILES)))
DEPS = o/dependencies.d
VERS =  $(shell grep VERS_STRING h/config.h | cut -d\" -f2)

# Trickery to run a script on the -first ever- compile and re-create a directory
# structure that may be missing due to Git not tracking empty directories.
$(shell if [ -x ./.dirbuild ]; then ./.dirbuild; rm -f ./.dirbuild; fi )

.PHONY: help $(PROG) cbuild clean depend

help:
	echo "\n### $(VERS) Makefile Options ###"
	echo "    help     Displays this help menu."
	echo "    $(PROG)      Compiles the game into binary file $(PROG)."
	echo "    cbuild   Equivalent to: make clean && make depend && make $(PROG)"
	echo "    clean    Removes files: $(PROG) o/* d/* ../report/core"
	echo "    depend   Generate dependencies for all source code.\n"

$(PROG): $(O_FILES)
	$(MAKE) depend
	$(RM) $(PROG)
	$(CXX) -o $(PROG) $(O_FILES) $(L_FLAGS)
	echo "Finished building $(VERS) ($(MODE))."
	chmod +x $(PROG)

cbuild:
	$(MAKE) clean
	$(MAKE) $(PROG)

clean:
	$(RM) $(O_FILES) $(DEPS) $(PROG) ../report/core

depend:
	$(RM) $(DEPS)
	echo "Generating dependencies for $(VERS) ... "
	$(CXX) -MM $(CXX_FLAGS) $(C_FILES) > $(DEPS)
	perl -pi -e 's.^([a-z]).o/$$1.g' $(DEPS)
	echo "Finished writing dependencies to $(DEPS)"

# pull in dependency info for *existing* .o files
-include $(DEPS)

o/%.o: %.c
	echo "Compiling `echo $@ | cut -c 3-` ... ";
	$(CXX) -c $(CXX_FLAGS) $(W_FLAGS) $< -o $@

o/%.o: %.cpp
	echo "Compiling `echo $@ | cut -c 3-` ...";
	$(CXX) -c $(CXX_FLAGS) $(W_FLAGS) $< -o $@
