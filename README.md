# [redacted]os
_NOTE: This kernel is still in early stages of development - check back after a few months to see a potentially usable OS._
_It lacks proper documentation and a proper license, but for now, just don't copy anything from here without asking me._

[redacted]os is a hobbyist OS currently only on x86_64. feel free to contribute if you want.

What I'm working on right now:
* Filesystem

What I'm going to work on soon:
* Processes / multithreaded context
 
Long term goal: run doom! (https://github.com/ozkl/doomgeneric)

# why 
i like making stuff and this is a lot of stuff to make.

# how to run ?!!1!!
to build the image yourself, you'll need the following dependencies:
* make (apt or equivalent)
* xorriso (apt or equivalent)
* x86-elf-gcc and binutils (https://wiki.osdev.org/GCC_Cross-Compiler), modified to allow -mno-red-zone
* nasm (apt or equivalent)
* limine (will be automatically installed through the Makefile)

to run the built image (.elf),
* ovmf (apt or equivalent)
* qemu (qemu-system-x86_64, apt or equivalent)
* gdb (optional, for debugging only)

# who 
for now, it's just me.
to know what [redacted] is or for other queries, add me on discord: xxyl#3636.


