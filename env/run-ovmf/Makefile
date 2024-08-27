
qemu:
	qemu-system-x86_64 -m 512 \
	-drive if=pflash,format=raw,file=OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,file=OVMF_VARS.fd,readonly=on \
	-drive format=raw,file=fat:rw:esp \
	-net none \
	-vnc 172.28.37.163:0,to=99,id=default \
