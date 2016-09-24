prog_sources = ['device.cpp', 'device_LED.cpp', 'config.cpp', 'deviceList.cpp']

corelib = library('isaac-core', prog_sources, include_directories: inc,
		dependencies: [uWS_dep, json_dep, spdlog_dep])
