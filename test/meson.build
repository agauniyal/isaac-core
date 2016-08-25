testexe = executable('testexe', 'test.cpp',
        include_directories : inc, link_with : [uWS_lib, corelib],
        dependencies : [gtest, thread_dep, uWS_dep])

test('app test', testexe)
