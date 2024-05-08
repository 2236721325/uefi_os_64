
qemu:
	qemu-system-x86_64 -m 4096 \
	-drive if=pflash,format=raw,file=OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,file=OVMF_VARS.fd,readonly=on \
	-drive format=raw,file=fat:rw:esp \
	-net none