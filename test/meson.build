testexe = executable('testexe', 'test.cpp',
        include_directories : inc, link_with : corelib,
        dependencies : [gtest, thread_dep, json_dep, uWS_dep])

test('app test', testexe)
