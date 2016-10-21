prog_sources = ['device.cpp', 'device_Led.cpp', 'config.cpp', 'deviceList.cpp',
                'device_Temperature.cpp', 'device_Switch.cpp', 'device_Buzzer.cpp',
				'device_TripWire.cpp', 'device_FlameSensor.cpp']

corelib = library('isaac-core', prog_sources, include_directories: inc,
		dependencies: [thread_dep, uWS_dep, json_dep, spdlog_dep])
