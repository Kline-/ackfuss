## Compiler to use. Either g++ or your own flavor of colorgcc, ccache, etc.
# Default: g++
CC   = g++

### Name of binary file to generate.
# Default: ack
PROG = ack

## Compile mode. Either "RELEASE" or "DEBUG". Defaults to DEBUG otherwise.
# Default: DEBUG
MODE = DEBUG


### END CONFIGURATION OPTIONS ###


ifeq '$(MODE)' 'RELEASE'
	C_FLAGS = -w -O3
else
	C_FLAGS = -O0 -ggdb3 -pg $(W_FLAGS)
endif

#-Werror -Wwrite-strings -Wconversion
MAKEFLAGS = -j2
W_FLAGS = -Wall -Wformat-security -Wpointer-arith -Wredundant-decls -Wcast-align -Wshadow -Wno-write-strings
L_FLAGS = -lcrypt -lm -lpthread -ldl -L`pwd`

C_FILES = $(wildcard *.c *.cpp)
O_FILES = $(patsubst %.cpp,o/%.o,$(patsubst %.c,o/%.o,$(C_FILES)))
DEPS = o/dependencies.d
VERS =  $(shell grep VERS_STRING h/config.h | cut -d\" -f2)

# Trickery to run a script on the -first ever- compile and re-create a directory
# structure that may be missing due to Git not tracking empty directories.
$(shell if [ -x ./.dirbuild ]; then ./.dirbuild; rm -f ./.dirbuild; fi )

.PHONY: help $(PROG) clean depend cbuild
.SILENT: help $(PROG) clean depend cbuild $(O_FILES)

help:
	echo -e "\n### $(VERS) Makefile Options ###"
	echo -e "    help     Displays this help menu."
	echo -e "    $(PROG)      Compiles the game into binary file $(PROG)."
	echo -e "    cbuild   Equivalent to: make clean && make depend && make $(PROG)"
	echo -e "    clean    Removes files: $(PROG) o/* d/* ../report/core"
	echo -e "    depend   Generate dependencies for all source code.\n"

$(PROG): $(O_FILES)
	$(RM) $(PROG)
	$(CC) -o $(PROG) $(O_FILES) $(L_FLAGS)
	echo "Finished building $(VERS) ($(MODE))."
	chmod +x $(PROG)

clean:
	$(RM) $(O_FILES) $(DEPS) $(PROG) ../report/core

depend:
	$(RM) $(DEPS)
	echo "Generating dependencies for $(VERS) ... "
	$(CC) -MM $(C_FLAGS) $(C_FILES) > $(DEPS)
	perl -pi -e 's.^([a-z]).o/$$1.g' $(DEPS)
	echo "Finished writing dependencies to $(DEPS)"

cbuild:
	$(MAKE) clean
	$(MAKE) depend
	$(MAKE) $(PROG)

# pull in dependency info for *existing* .o files
-include $(DEPS)

o/%.o: %.c
	echo "Compiling `echo $@ | cut -c 3-` ... ";
	$(CC) -c $(C_FLAGS) $(W_FLAGS) $< -o $@

o/%.o: %.cpp
	echo "Compiling `echo $@ | cut -c 3-` ...";
	$(CC) -c $(C_FLAGS) $(W_FLAGS) $< -o $@
