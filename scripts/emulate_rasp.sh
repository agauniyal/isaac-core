#!/bin/sh

CONFIG=$(cat <<- EORULE
{
    "db_path": "data/",
    "log_path": "logs/",
    "gpio_path": "debug/raspi/path/",
    "tempSensor_path": "debug/raspi/path/"
}
EORULE
)

TEMPERATURE=$(cat <<- EORULE
ab cd ef gh ab cd ef gh 12 : crc=12 YES
ab cd ef gh ab cd ef gh 12 t=12345
EORULE
)

emulateGpio(){
	mkdir -p app/debug/raspi/path/gpio$1
	echo "out" > app/debug/raspi/path/gpio$1/direction
	echo 1 > app/debug/raspi/path/gpio$1/value
	echo 1 > app/debug/raspi/path/export
	echo 0 > app/debug/raspi/path/unexport

	mkdir -p app/debug/raspi/path/tempSensor
	touch app/debug/raspi/path/tempSensor/w1_slave
	echo "$TEMPERATURE" > app/debug/raspi/path/tempSensor/w1_slave

	touch app/config.json
	echo "$CONFIG" > app/config.json

	mkdir -p app/logs
	mkdir -p app/data


	mkdir -p test/debug/raspi/path/gpio$1
	echo "out" > test/debug/raspi/path/gpio$1/direction
	echo 1 > test/debug/raspi/path/gpio$1/value
	echo 1 > test/debug/raspi/path/export
	echo 0 > test/debug/raspi/path/unexport

	mkdir -p test/debug/raspi/path/tempSensor
	touch test/debug/raspi/path/tempSensor/w1_slave
	echo "$TEMPERATURE" > test/debug/raspi/path/tempSensor/w1_slave

	touch test/config.json
	echo "$CONFIG" > test/config.json

	mkdir -p test/logs
	mkdir -p test/data


	mkdir -p debug/raspi/path/gpio$1
	echo "out" > debug/raspi/path/gpio$1/direction
	echo 1 > debug/raspi/path/gpio$1/value
	echo 1 > debug/raspi/path/export
	echo 0 > debug/raspi/path/unexport

	mkdir -p debug/raspi/path/tempSensor
	touch debug/raspi/path/tempSensor/w1_slave
	echo "$TEMPERATURE" > debug/raspi/path/tempSensor/w1_slave

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
