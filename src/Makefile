CC   = gcc
VERS = AckFUSS 4.3.8
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
#CYGWIN = -DCYGWIN

# Uncomment the two lines below if compiling on a Solaris box
#SOLARIS_FLAG = -Dsun -DSYSV -Wno-char-subscripts
#SOLARIS_LINK = -lnsl -lsocket -lresolv

#-Wshadow -Wformat-security -Wcast-align -Wredundant-decls -Wconversion -Wwrite-strings
W_FLAGS = -Wall -Werror -Wpointer-arith
C_FLAGS = -O2 -g $(W_FLAGS) -DACK_43 $(PROF) $(SOLARIS_FLAG)
L_FLAGS = -O2 -g -lcrypt -lm $(PROF) $(SOLARIS_LINK)

#IMC2 - Comment out to disable IMC2 support
IMC = 1

C_FILES = act_clan.c act_comm.c act_info.c act_mob.c act_move.c act_obj.c act_wiz.c \
	  areachk.c areasave.c bitmask.c board.c build.c buildare.c buildtab.c clutch.c comm.c \
	  const.c db.c email.c enchant.c fight.c handler.c hash.c hunt.c interp.c lists.c \
	  macros.c magic.c magic2.c magic3.c magic4.c mapper.c mob_commands.c mob_prog.c \
	  money.c mount.c mquest.c obj_fun.c pdelete.c quest.c rulers.c save.c scheck.c social-edit.c \
	  special.c spell_dam.c spendqp.c ssm.c strfuns.c sysdata.c trigger.c update.c \
	  vampire.c werewolf.c wizutil.c write.c

ifdef IMC
   C_FILES := imc.c sha256.c $(C_FILES)
   C_FLAGS := $(C_FLAGS) -DIMC -DIMCACK
endif

O_FILES = $(patsubst %.c,o/%.o,$(C_FILES))

H_FILES = $(wildcard *.h)

all:
	@$(MAKE) ack

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
	@echo "Compiling $@ ...";
	@$(CC) -c $(C_FLAGS) $< -o $@

.c.o: ack.h
	@$(CC) -c $(C_FLAGS) $<

scan: scan.o
	rm -f scan
	$(CC) $(L_FLAGS) -o scan scan.o
