override KERNEL := os.elf
override ISO	:= image.iso
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
ifeq ($(origin CC), default)
CC := gcc
endif

# Likewise, "ld" here is just a placeholder and your mileage may vary if using the
# host's "ld".
ifeq ($(origin LD), default)
LD := ld
endif

# User controllable CFLAGS.
CFLAGS ?= -O2 -g -Wall -Wextra -pipe -nostdlib -Wno-unused-parameter -Wno-int-to-pointer-cast -gdwarf-2

# User controllable linker flags. We set none by default.
LDFLAGS ?=

QEMUFLAGS ?= 	-bios /usr/share/ovmf/OVMF.fd \
				--serial stdio -D ../log.txt -d int -M smm=off \
				-m 5G -cpu qemu64 -machine q35 \
				--no-reboot --no-shutdown \
				-rtc base=localtime -nic model=e1000 \
				-enable-kvm

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
all: $(ISO)
	@qemu-system-x86_64 $(ISO) $(QEMUFLAGS)
# @make clean -s

run: $(ISO)
	@qemu-system-x86_64 $(ISO) $(QEMUFLAGS)
	@make clean -s

gdb: $(ISO)
	@qemu-system-x86_64 $(ISO) $(QEMUFLAGS) -s -S
	@make clean -s

$(ISO): $(KERNEL)
	@cp ~/Documents/cs/jamesmvfs/bateman.tga .
	@cp ~/Documents/cs/jamesmvfs/test.txt .
	@cp ~/Documents/cs/jamesmvfs/test2.txt .
	@cp ~/Documents/cs/jamesmvfs/test3.txt .
# modify this as ../make_initrd <source1 filename> <dest1 filename> <source1 filename> <dest1 filename> etc..
	@../make_initrd test.txt test.txt test2.txt test2.txt test3.txt test3.txt bateman.tga bateman.tga 
	@mkdir -p ../iso_root
	@mv initrd.img ../iso_root
	@git submodule update --remote
	@make -C ../limine
	@cp -v os.elf \
		../limine.cfg ../limine/limine.sys ../limine/limine-cd.bin ../limine/limine-cd-efi.bin ../iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
                -no-emul-boot -boot-load-size 4 -boot-info-table \
                --efi-boot limine-cd-efi.bin \
                -efi-boot-part --efi-boot-image --protective-msdos-label \
                ../iso_root -o $(ISO)
	@../limine/limine-deploy $(ISO)

$(KERNEL): $(OBJ)
	@$(LD) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

%.o: %.asm 
	@nasm -felf64 $< -o $@

# -include $(HEADER_DEPS)
%.o: %.c stivale2.h
	@$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	@rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS) $(ISO)