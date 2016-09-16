#!/bin/sh

if [ $EUID != 0 ]; then
    sudo "$0" "$@"
    exit $?
fi

destdir=/etc/udev/rules.d/99-com.rules

read -r -d '' CONTENTS <<- EORULE
	SUBSYSTEM=="input", GROUP="input", MODE="0660"
	SUBSYSTEM=="i2c-dev", GROUP="i2c", MODE="0660"
	SUBSYSTEM=="spidev", GROUP="spi", MODE="0660"
	SUBSYSTEM=="bcm2835-gpiomem", GROUP="gpio", MODE="0660"

	SUBSYSTEM=="gpio*", PROGRAM="/bin/sh -c 'chown -R root:gpio /sys/class/gpio && chmod -R 770 /sys/class/gpio;chown -R root:gpio /sys/devices/virtual/gpio && chmod -R 770 /sys/devices/virtual/gpio;chown -R root:gpio /sys$devpath && chmod -R 770 /sys$devpath'"

	KERNEL=="ttyAMA[01]", PROGRAM="/bin/sh -c 'ALIASES=/proc/device-tree/aliases; if cmp -s $ALIASES/uart0 $ALIASES/serial0; then echo 0;elif cmp -s $ALIASES/uart0 $ALIASES/serial1; then echo 1;else exit 1; fi'", SYMLINK+="serial%c"

	KERNEL=="ttyS0", PROGRAM="/bin/sh -c 'ALIASES=/proc/device-tree/aliases; if cmp -s $ALIASES/uart1 $ALIASES/serial0; then echo 0; elif cmp -s $ALIASES/uart1 $ALIASES/serial1; then echo 1; else exit 1; fi '", SYMLINK+="serial%c"
EORULE

Green='\033[0;32m'
Red='\033[0;31m'
Yellow='\033[0;33m'
NC='\033[0m'
echo -e "${Yellow}WARNING: You should do this only on a development board.${NC}";

read -p "Are you sure? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "$CONTENTS" > "$destdir"
	udevadm trigger
	echo -e "${Green}Rules exported and udev reloaded!${NC}"
else
	echo -e "${Red}Aborted.${NC}"
fi

