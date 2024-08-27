BUILD:=./build
SRC:=./src
INCLUDE:=./include
ENV:=./env
CFLAGS:= -m64 # 64 位的程序
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
# CFLAGS+= -nostdinc		# 不需要标准头文件
# CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
# CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
# CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
CFLAGS+= -mcmodel=large
CFLAGS+= -o0
CFLAGS:=$(strip ${CFLAGS})
DEBUG:=-g

CINCLUDE:= -I$(INCLUDE)


$(BUILD)/%.o: $(SRC)/%.c
	$(shell mkdir -p $(dir $@))
	gcc $(CFLAGS) $(CINCLUDE) $(DEBUG) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.s
	$(shell mkdir -p $(dir $@))
	as --64 $(DEBUG) -o  $@ $<


$(BUILD)/kernel/system.o: $(BUILD)/kernel/kernel.o \
						  $(BUILD)/kernel/bootInfo.o \
						  $(BUILD)/kernel/draw.o \
						  $(BUILD)/kernel/head.o \
						  $(BUILD)/kernel/interrupt.o \
						  $(BUILD)/kernel/interrupt_entry.o \
						  $(BUILD)/kernel/tss.o \
						  $(BUILD)/kernel/init.o \
						  $(BUILD)/kernel/exception.o \
						  $(BUILD)/kernel/memory.o \
						  $(BUILD)/kernel/debug.o \
						  $(BUILD)/device/mouse.o \
						  $(BUILD)/device/keyboard.o \
						  $(BUILD)/device/8259A.o \
						  $(BUILD)/lib/ioqueue.o \
						  $(BUILD)/lib/string.o \
						  $(BUILD)/lib/bitmap.o \
						  $(BUILD)/lib/list.o  \
						  $(BUILD)/lib/stdio.o \

						  
	$(shell mkdir -p $(dir $@))
	ld $(DEBUG) -b elf64-x86-64 -z muldefs $^ -o $@ -T linker.ld
	objdump -D $@ > system_o_dump
	readelf -all $@ > system_o_readelf

$(BUILD)/kernel/system.bin: $(BUILD)/kernel/system.o
	$(shell mkdir -p $(dir $@))
	objcopy -I elf64-x86-64 -S  -R ".comment" -R ".eh_frame" -O binary $< $@
	objdump -b binary -m i386:x86-64 -D $@ > system_bin_dump

.PHONY:clean
clean:
	rm -rf $(BUILD)/kernel

.PHONY:qemu
qemu:clean $(BUILD)/kernel/system.bin
	@objcopy --only-keep-debug $(BUILD)/kernel/system.o $(BUILD)/kernel/system.dbg
	@cp $(BUILD)/bootloader/DEBUG_GCC5/X64/BootLoader.efi $(ENV)/run-ovmf/esp/EFI/Boot/BootX64.efi
	@cp $(BUILD)/kernel/system.bin $(ENV)/run-ovmf/esp/
	@qemu-system-x86_64 -m 4096 \
	-s -S \
	-boot c \
	-drive if=pflash,format=raw,file=$(ENV)/run-ovmf/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,file=$(ENV)/run-ovmf/OVMF_VARS.fd,readonly=on \
	-drive format=raw,file=fat:rw:$(ENV)/run-ovmf/esp \
	-net none \
	-vnc 172.28.47.230:0,to=99,id=default  \


	
# @gdb -s build/kernel/system.dbg -ex "target remote localhost:1234"
# #  -S -s \
