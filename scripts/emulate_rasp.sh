#!/bin/sh

read -r -d '' CONFIG <<- EORULE
{
    "db_path": "data/",
    "log_path": "logs/",
    "gpio_path": "debug/raspi/path/"
}
EORULE

emulateGpio(){
	mkdir -p app/debug/raspi/path/gpio$1
	touch app/debug/raspi/path/gpio$1/direction
	touch app/debug/raspi/path/gpio$1/value
	touch app/debug/raspi/path/export
	touch app/debug/raspi/path/unexport

	touch app/config.json
	echo "$CONFIG" > app/config.json

	mkdir -p app/logs
	mkdir -p app/data


	mkdir -p test/debug/raspi/path/gpio$1
	touch test/debug/raspi/path/gpio$1/direction
	touch test/debug/raspi/path/gpio$1/value
	touch test/debug/raspi/path/export
	touch test/debug/raspi/path/unexport
	touch test/config.json
	echo "$CONFIG" > test/config.json

	mkdir -p test/logs
	mkdir -p test/data


	mkdir -p debug/raspi/path/gpio$1
	touch debug/raspi/path/gpio$1/direction
	touch debug/raspi/path/gpio$1/value
	touch debug/raspi/path/export
	touch debug/raspi/path/unexport

	touch config.json
	echo "$CONFIG" > config.json

	mkdir -p logs
	mkdir -p data
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
emulateGpio "10"
emulateGpio "11"
emulateGpio "12"

echo -e "\033[32;1m  GPIO pin-nums [1..12] have been emulated ✔ \033[0m"
echo -e "\033[32;1m  Correct config files have been generated ✔ \033[0m"
echo -e "\033[32;1m  Required directories have been generated ✔ \033[0m"
