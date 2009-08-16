CC   = g++
VERS = AreaConvert 1.0

#-Werror -Wwrite-strings -Wconversion
W_FLAGS = -Wall -Wformat-security -Wpointer-arith -Wredundant-decls -Wcast-align -Wshadow -Wno-write-strings
C_FLAGS = -O2 -g $(W_FLAGS)
L_FLAGS =

C_FILES = $(wildcard *.c)

O_FILES = $(patsubst %.c,%.o,$(C_FILES))

all:
	@$(MAKE) areaconvert

areaconvert: $(O_FILES)
	@rm -f areaconvert
	@$(CC) -o areaconvert $(O_FILES) $(L_FLAGS)
	@echo "Done compiling $(VERS).";
	@chmod g+w areaconvert
	@chmod a+x areaconvert
	@chmod g+w $(O_FILES)

clean:
	@rm -f *.o areaconvert

%.o: %.c
	@echo "Compiling `echo $@ | cut -c 1-` ...";
	@$(CC) -c $(C_FLAGS) $< -o $@
