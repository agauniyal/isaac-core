prog_sources = ['device.cpp', 'device_Led.cpp', 'config.cpp', 'deviceList.cpp','device_Temperature.cpp']

corelib = library('isaac-core', prog_sources, include_directories: inc,
		dependencies: [uWS_dep, json_dep, spdlog_dep])
