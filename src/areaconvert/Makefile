CC   = g++
VERS = AreaConvert 1.0

#-Werror -Wwrite-strings -Wconversion
W_FLAGS = -Wall -Wformat-security -Wpointer-arith -Wredundant-decls -Wcast-align -Wshadow -Wno-write-strings
C_FLAGS = -O2 -g $(W_FLAGS)
L_FLAGS =

C_FILES = $(wildcard *.c)

O_FILES = $(patsubst %.c,o/%.o,$(C_FILES))

all:
	@$(MAKE) areaconvert

# pull in dependency info for *existing* .o files
-include dependencies.d

areaconvert: $(O_FILES)
	@rm -f areaconvert
	@$(CC) -o areaconvert $(O_FILES) $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done compiling $(VERS).";
	@chmod g+w areaconvert
	@chmod a+x areaconvert
	@chmod g+w $(O_FILES)

clean:
	@rm -f o/*.o areaconvert

o/%.o: %.c
	@echo "Compiling `echo $@ | cut -c 1-` ...";
	@$(CC) -c $(C_FLAGS) $< -o $@
