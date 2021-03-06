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

doBoot: Whilenium-1.0.simics
	./scripts/run.sh $(SIMICS) $<

Whilenium: bin/Whilenium-1.0.simics
	./scripts/run.sh $(SIMICS) $<

#### RULES TO BUILD BINARIES FROM OBJECT FILES
bin/Whilenium-1.0.simics: $(addprefix build/, _Boot.o Boot.o API.o Process.o Scheduler.o Interrupt.o IOHandler.o stdlib.o UserPrograms.o debug.o Shell.o Syscalls.o) 
	$(LD) $(ARCH) -o $@ $^

#### Add dependency on headerfile of various tty.o files

build/Boot.o: src/Boot.c include/Boot.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/IOHandler.o: src/IOHandler.c include/IOHandler.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/API.o: src/API.c include/API.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/Interrupt.o: src/Interrupt.c include/Interrupt.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/Process.o: src/Process.c include/Process.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/Scheduler.o: src/Scheduler.c include/Scheduler.h include/Process.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/stdlib.o: src/stdlib.c include/stdlib.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/UserPrograms.o: src/UserPrograms.c include/UserPrograms.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/Shell.o: src/Shell.c include/Shell.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@
	
build/_Boot.o: src/_Boot.S include/_Boot.h include/Settings.h
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@

build/Syscalls.o: src/Syscalls.c include/Syscalls.h include/Settings.h
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
