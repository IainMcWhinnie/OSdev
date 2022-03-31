
TARGET_BUILD := ./build/image.img
TARGET_BOOT := ./build/bootloader.bin
TARGET_SPACE := ./build/space.bin
TARGET_KERNEL := ./build/kernel.bin

ROOT_SRC_DIR := ./src
ROOT_BUILD_DIR := ./build
BOOT_SRC_DIR := $(ROOT_SRC_DIR)/boot
DRIVER_SRC_DIR := $(ROOT_SRC_DIR)/drivers
KERNEL_SRC_DIR := $(ROOT_SRC_DIR)/kernel

KERNEL_ENTRY_SRC := ./src/kernel/kernel_entry.asm
SPACE_SRC := ./src/boot/space.asm

TEMP_KERNEL_FILE = ./build/kernel.tmp


DRIVER_SRCS := $(notdir $(wildcard $(DRIVER_SRC_DIR)/*.c))
DRIVER_HEADERS := $(wildcard $(DRIVER_SRC_DIR)/*.h)
KERNEL_SRCS := $(wildcard $(KERNEL_SRC_DIR)/*.c)
KERNEL_HEADERS := $(wildcard $(KERNEL_SRC_DIR)/*.h)

C_SRCS := $(notdir $(DRIVER_SRCS)) $(notdir $(KERNEL_SRCS))
ASM_SRCS := $(notdir $(wildcard $(KERNEL_SRC_DIR)/*.asm $(DRIVER_SRC_DIR)/*.asm))

KERNEL_ENTRY_OBJ = $(notdir $(KERNEL_ENTRY_SRC:.asm=.o))

OBJS := $(C_SRCS:.c=.o) $(ASM_SRCS:.asm=.o)
OBJS_WITHOUT_KERNEL_ENTRY := $(addprefix $(ROOT_BUILD_DIR)/,$(filter-out $(KERNEL_ENTRY_OBJ),$(OBJS) ))

BOOT_SRCS := $(wildcard $(BOOT_SRC_DIR)/*.asm)

comma := ,
empty := 
space := $(empty) $(empty)



all: $(TARGET_BUILD)

qemu: all
	qemu-system-i386 build/image.img

$(TARGET_BUILD) : $(TARGET_BOOT) $(TARGET_SPACE) $(TARGET_KERNEL)
	powershell -Command "gc $(subst $(space),$(comma),$^) -Enc Byte -Read 512 |sc $@ -Enc Byte"


$(TARGET_BOOT) : $(BOOT_SRCS)
	nasm $(BOOT_SRC_DIR)/$(notdir $(TARGET_BOOT:.bin=.asm)) -f bin -o $@

$(TARGET_SPACE) : $(SPACE_SRC) ## Will become unnecessary when OS is big enough
	nasm $< -f bin -o $@



## We need to specify the kernel_entry.o file specifically
$(TARGET_KERNEL) : $(ROOT_BUILD_DIR)/$(KERNEL_ENTRY_OBJ) $(OBJS_WITHOUT_KERNEL_ENTRY)
	ld -Ttext 0x1000 -o $(TEMP_KERNEL_FILE) $^
	objcopy -O binary $(TEMP_KERNEL_FILE) $@

## OBJECTS FROM C SRC

$(ROOT_BUILD_DIR)/%.o : $(DRIVER_SRC_DIR)/%.c $(DRIVER_HEADERS)
	gcc -ffreestanding -c $< -o $@

$(ROOT_BUILD_DIR)/%.o : $(KERNEL_SRC_DIR)/%.c $(KERNEL_HEADERS)
	gcc -ffreestanding -c $< -o $@

## OBJECTS FROM ASM SRC

$(ROOT_BUILD_DIR)/%.o : $(DRIVER_SRC_DIR)/%.asm
	nasm $< -f elf -o $@

$(ROOT_BUILD_DIR)/%.o : $(KERNEL_SRC_DIR)/%.asm
	nasm $< -f elf -o $@


clean:
	powershell -Command "rm build\*"