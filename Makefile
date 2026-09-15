GCCPARAMS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LINKPARAMS = -ffreestanding -O2 -nostdlib

TIMESTAMP = $(shell date +"%Y-%m-%d %H:%M:%S")
COMPILERPATH = $(wildcard $(HOME)/opt/cross/bin/i686-elf-gcc)
ASSEMBLERPATH = $(wildcard $(HOME)/opt/cross/bin/i686-elf-as)

objects = boot.o \
 kernel.o

%.o: %.c
	$(COMPILERPATH) $(GCCPARAMS) -o $@ -c $<
	echo 'compile attempt at $(TIMESTAMP)... $< to $@' >> buildlog.txt

%.o: $.s
	$(ASSEMBLERPATH) -o $@ $<
	echo 'assemble attempt at $(TIMESTAMP)... $< to $@' >> buildlog.txt

nanamios: linker.ld $(objects)
	$(COMPILERPATH) -T $< -o $@  $(LINKPARAMS) $(objects) -lgcc
	echo 'link attempt at $(TIMESTAMP)...' >> buildlog.txt

NanamiOS.iso: nanamios
	mkdir isodir
	mkdir isodir/boot/
	mkdir isodir/boot/grub
	cp $< isodir/boot/
	echo 'menuentry "Nanami/OS 0.01 Alpha" {' >> isodir/boot/grub/grub.cfg
	echo '	multiboot /boot/nanamios' >> isodir/boot/grub/grub.cfg
	echo '	boot' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue --output=$@ isodir
	echo 'image build attempt at $(TIMESTAMP)... NanamiOS.iso' >> buildlog.txt

verifymultiboot: verifyMultiboot
verifyMultiboot:
	if grub-file --is-x86-multiboot nanamios; then
		echo The kernel has working multiboot.
	else
		echo The kernel\'s multiboot is broken.
	fi

testDisc: qemudiscimage
qemudiscimage:
	qemu-system-i386 -cdrom NanamiOS.iso

testKernel: qemukernel
qemukernel:
	qemu-system-i386 -kernel nanamios

