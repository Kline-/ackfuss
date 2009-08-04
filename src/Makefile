CC   = g++
VERS = AckFUSS 4.4.1
#PROF    = -pg

# Debugging flags possible:  DEBUG_MEM DEBUG_MEM_CORRUPT DEBUG_MEM_DUP_FREE
# DEBUG_MEM is most basic, just checks magic numbers when freeing
# DEBUG_MEM_CORRUPT checks every free block to see if magic numbers ok, every
#                   call to alloc_mem
#		    Also makes calls to void check_free_mem(void) check for
#		    corrupted free memory blocks.
# DEBUG_MEM_DUP_FREE checks to see if freed blocks are overlapping every call
# 		     to mem_alloc.
#

#-DDEBUG_MEM -DDEBUG_MEM_CORRUPT -DDEBUG_MEM_DUP_FREE
#-DDEBUG_MEM -DDEBUG_MEM_CORRUPT

#Uncomment to compile in Cygwin
#CYGWIN = -DCYGWIN -DWIN32

#-Werror -Wwrite-strings -Wconversion
W_FLAGS = -Wall -Wformat-security -Wpointer-arith -Wredundant-decls -Wcast-align -Wshadow -Wno-write-strings
C_FLAGS = -O2 -g $(W_FLAGS) $(PROF) $(CYGWIN)
L_FLAGS = -lcrypt -lm -lpthread -ldl -L`pwd` -llua

#IMC2 - Comment out to disable IMC2 support
IMC = 1
IMC_FILES = imc.c sha256.c

C_FILES = $(filter-out $(IMC_FILES),$(wildcard *.c))

ifdef IMC
   C_FILES = $(wildcard *.c)
   C_FLAGS := $(C_FLAGS) -DIMC
endif

O_FILES = $(patsubst %.c,o/%.o,$(C_FILES))

H_FILES = $(wildcard h/*.h)

all:
	@$(MAKE) -j4 ack

# pull in dependency info for *existing* .o files
-include dependencies.d

ifdef CYGWIN
ack: $(O_FILES)
	@rm -f ack.exe
	@$(CC) -o ack.exe $(O_FILES) $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done compiling $(VERS).";
	@chmod g+w ack.exe
	@chmod a+x ack.exe
	@chmod g+w $(O_FILES)

clean:
	@rm -f o/*.o ack.exe dependencies.d ../area/core
else
ack: $(O_FILES)
	@rm -f ack
	@$(CC) -o ack $(O_FILES) $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done compiling $(VERS).";
	@chmod g+w ack
	@chmod a+x ack
	@chmod g+w $(O_FILES)

clean:
	@rm -f o/*.o ack dependencies.d ../area/core
endif

o/%.o: %.c
	@echo "Compiling `echo $@ | cut -c 3-` ...";
	@$(CC) -c $(C_FLAGS) $< -o $@
