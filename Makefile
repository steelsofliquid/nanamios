GCCPARAMS = -std=gnu99 --ffreestanding - O2 -Wall -Wextra

TIMESTAMP = $(shell date +"%Y-%m-%d %H:%M:%S")

objects = boot.o \
 kernel.o

%.o: %.c
	$HOME/opt/cross/bin/i686-elf-gcc $(GCCPARAMS) -o $@ -c $<
	echo 'compile attempt at $(TIMESTAMP)... $< to $@' >> buildlog.txt

%.o: $.s
	$HOME/opt/cross/bin/i686-elf-as -o $@ $<
	echo 'assemble attempt at $(TIMESTAMP)... $< to $@' >> buildlog.txt

nanamios:

