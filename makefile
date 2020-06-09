###############################################################################################
#------------------------------------------makefile-------------------------------------------#
###############################################################################################
#                                                                                             #
# Specifies the compilation of the OS. To compile fully run the command:                      #
#   make all                                                                                  #
# There is also the capacity to start emumation of the OS through QEMU using the command:     #
#   make qemu                                                                                 #
# To clean all compilation files (not including the final image) use:                         #
#   make clean                                                                                #
#                                                                                             #
###############################################################################################
#-----------------------------<Iain McWhinnie>----<13/05/2020>--------------------------------#
###############################################################################################



C_SOURCES = $(wildcard kernel/*.c drivers/*.c) # define C source files located in /kernel/ and /drivers/
HEADERS = $(wildcard kernel/*.h drivers/*.h) # define C header files located in /kernel/ and /drivers/
OBJ = ${C_SOURCES:.c=.o} # every C source is associated with an object file during compilation
ASM_KERNEL_SOURCES = $(filter-out kernel/kernel_entry.asm, $(wildcard kernel/*.asm)) # define assembly sources used by the kernel (excludes kernel_entry since it needs to be specified as first)
OBJ_ASM = ${ASM_KERNEL_SOURCES:.asm=.o} # the related object files for the asm sources

ASM_SOURCES = $(wildcard boot/*.asm ) # define all assembly sources for the bootloader



# fully compile the OS into a disk image
all: image.img


# compile and run emulator
qemu: all
	qemu-system-i386 image.img


# combine compiled binary files into image
# bootloader.bin is a bootloader in the first sector that is 512 bytes long
# kernel.bin is all the kernel code
# space.bin is simply padding of zero initialised bytes to lengthen the file since the bootloader loads more sectors than necessary
image.img : boot/bootloader.bin kernel/kernel.bin boot/space.bin
	powershell -Command "gc boot/bootloader.bin,kernel/kernel.bin,boot/space.bin -Enc Byte -Read 512 |sc image.img -Enc Byte"


# build the kernel binary by linking all object files
# creates a tmp file kernel.tmp that is in the PE executable format
# objcopy converts the PE executable to binary and then the temporary file is deleted

#https://html.developreference.com/article/18381777/MinGW's+ld+cannot+perform+PE+operations+on+non+PE+output+file
kernel/kernel.bin : kernel/kernel_entry.o ${OBJ_ASM} ${OBJ}
	ld -Ttext 0x1000 -o kernel/kernel.tmp $^
	objcopy -O binary kernel/kernel.tmp $@



# rule for building object files from C source
# as a general rule object files depend on headers files so will recompile if a header file is updated
%.o : %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

# rule for building object files with assembly source
%.o : %.asm
	nasm $< -f elf -o $@

# rule for compiling to binary files from assembly source
# as a general rule one assembly file depends on all other assembly files to make sure updates are found
%.bin : %.asm ${ASM_SOURCES}
	nasm $< -f bin -o $@

# clean all compilation files
clean:
	powershell -Command "rm kernel\*.bin, kernel\*.o, drivers\*.o, boot\*.bin, kernel\*.tmp"