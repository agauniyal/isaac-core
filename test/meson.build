test_sources = ['device_test.cpp', 'device_Led_test.cpp', 'config_test.cpp', 'deviceList_test.cpp']

testexe = executable('testexe', test_sources,
        include_directories : inc, link_with : corelib,
        dependencies : [gtest, thread_dep, json_dep, uWS_dep, spdlog_dep])

test('app test', testexe, args: '--gtest_color=yes')
