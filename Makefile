# Path to cross tools

MIPS_PREFIX=/it/sw/cross/mips-idt/bin/mips-idt-elf

EXECUTABLES=Boot

# gcc flags for the MIPS architecture:
#  -EL     : Little endian
#  -G0     : Always use small data (using register gp)
#  -mips32 : Compile for a 32-bit MIPS architecture
#

ARCH=-EL -G0 -mips32

# Other gcc flags

CFLAGS	+= -ggdb -Wall -fno-builtin -I/include

# Compiler and linker commands

CC=$(MIPS_PREFIX)-gcc

# Tell the linker where to start the text segment. 
LD=$(MIPS_PREFIX)-ld -Ttext 80020000


# Path to Simics installation

SIMICS=/home/dale2453/simics-workspace 


###### MAKE EXECUTABLES AND EXECUTE ######

_Boot: Boot.o _Boot.o
	$(LD) $(ARCH) -o $@ $^

doBoot: _Boot
	./scripts/run.sh $(SIMICS) $<

###### GENERIC BUILD PATTERNS ########

%.o: %.c
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@	

%.o: %.S
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@


clean: 
	pwd
	rm -f *.o
	rm -f *~
	rm -f \#* *\#
	rm -f ${EXECUTABLES}	
	rm -f scripts/#* scripts/*~
