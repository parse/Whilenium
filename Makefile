# Path to cross tools

MIPS_PREFIX=/it/sw/cross/mips-idt/bin/mips-idt-elf

EXECUTABLES=$(addprefix bin/, _Boot)

# For testing
OBJS_TIMER=example_timer.o asm.o debug.o


# gcc flags for the MIPS architecture:
#  -EL     : Little endian
#  -G0     : Always use small data (using register gp)
#  -mips32 : Compile for a 32-bit MIPS architecture
#

ARCH=-EL -G0 -mips32

# Other gcc flags

CFLAGS	+= -ggdb -Wall -fno-builtin -I include

# Compiler and linker commands

CC=$(MIPS_PREFIX)-gcc

# Tell the linker where to start the text segment. 
LD=$(MIPS_PREFIX)-ld -Ttext 80020000


# Path to Simics installation

SIMICS=${HOME}/simics-workspace 




#### RULE USED TO START SIMICS 

doBoot: boot 
	./scripts/run.sh $(SIMICS) $<

#### RULES TO BUILD BINARIES FROM OBJECT FILES
boot: $(addprefix build/, _Boot.o Boot.o API.o Interrupt.o Memory.o Process.o Scheduler.o IOHandler.o stdlib.o UserPrograms.o _Interrupt.o) 
	$(LD) $(ARCH) -o $@ $^

bin/boot_tty1: build/boot_tty1.o 
	$(LD) $(ARCH) -o $@ $^

bin/boot_tty3: $(addprefix build/, boot_tty2.o tty3.o)
	$(LD) $(ARCH) -o $@ $^

bin/boot_tty%: $(addprefix build/, boot_tty%.o tty%.o)
	$(LD) $(ARCH) -o $@ $^

#### Add dependency on headerfile of various tty.o files

build/Boot.o: src/Boot.c include/Boot.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/IOHandler.o: src/IOHandler.c include/IOHandler.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@	
	
build/tty%.o: tty%.c include/tty%.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@	


###### GENERIC BUILD PATTERNS ########

build/%.o: src/%.c
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@	

build/%.o: src/%.S
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@


clean: 
	pwd
	rm -f build/*.o
	rm -f include/*~ include/#* include/*#
	rm -f src/*~ src/#* src/*#
	rm -f scripts/*~ scripts/#* scripts/*#
	rm -f ${EXECUTABLES}	
