override KERNEL := os.elf
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
ifeq ($(origin CC), default)
CC := x86_64-elf-gcc
endif

# Likewise, "ld" here is just a placeholder and your mileage may vary if using the
# host's "ld".
ifeq ($(origin LD), default)
LD := x86_64-elf-ld
endif

AS := x86_64-elf-as

# User controllable CFLAGS.
CFLAGS ?= -O2 -g -Wall -Wextra -pipe -nostdlib -Wno-implicit

# User controllable linker flags. We set none by default.
LDFLAGS ?=

# Internal C flags that should not be changed by the user.
override INTERNALCFLAGS :=   \
	-I.                  \
	-std=gnu11           \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic             \
	-mabi=sysv           \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone        \
	-mcmodel=kernel      \
	-MMD

# Internal linker flags that should not be changed by the user.
override INTERNALLDFLAGS :=    \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static

# Use find to glob all *.c files in the directory and extract the object names.
override CFILES := $(shell find ./ -type f -name '*.c')
override ASMFILES := $(shell find ./ -type f -name '*.asm')
override OBJ := $(CFILES:.c=.o) $(ASMFILES:.asm=.o)
override HEADER_DEPS := $(CFILES:.c=.d)

# Default target.
.PHONY: all
all: $(KERNEL)
	../iso.sh
	qemu-system-x86_64 image.iso -serial stdio -D ../log.txt -d cpu_reset -m 512M
	make clean -s

stivale2.h:
	curl https://raw.githubusercontent.com/stivale/stivale/master/stivale2.h -o $@

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	$(LD) -r -b binary -o sfn_fonts/font_unifont.o sfn_fonts/unifont.sfn
	$(LD) sfn_fonts/font_unifont.o $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

# Compilation rules for *.c files.

%.o: %.asm 
	nasm -felf64 $< -o $@

# -include $(HEADER_DEPS)
%.o: %.c stivale2.h
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@



# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS) # image.iso

.PHONY: distclean
distclean: clean
	rm -f stivale2.h