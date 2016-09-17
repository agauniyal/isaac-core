#!/bin/sh

emulateGpio(){
	mkdir -p app/debug/raspi/path/gpio$1
	touch app/debug/raspi/path/export
	touch app/debug/raspi/path/unexport

	mkdir -p test/debug/raspi/path/gpio$1
	touch test/debug/raspi/path/export
	touch test/debug/raspi/path/unexport

	mkdir -p debug/raspi/path/gpio$1
	touch debug/raspi/path/export
	touch debug/raspi/path/unexport
}

cd "${MESON_BUILD_ROOT}"

emulateGpio "1"
emulateGpio "2"
emulateGpio "3"
emulateGpio "4"
emulateGpio "5"
emulateGpio "6"
emulateGpio "7"
emulateGpio "8"
emulateGpio "9"

echo -e "\033[32;1m  GPIO pins [1..9] have been emulated ✔ \033[0m"
