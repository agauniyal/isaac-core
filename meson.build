project('isaac-core', 'cpp', version : '0.1.0',
		default_options : ['cpp_std=c++14'])

inc = include_directories('include')

thread_dep = dependency('threads')

uWS_sp = subproject('uWS')
uWS_dep = uWS_sp.get_variable('uWS_dep')

json_sp = subproject('json')
json_dep = json_sp.get_variable('json_dep')

spdlog_sp = subproject('spdlog')
spdlog_dep = spdlog_sp.get_variable('spdlog_dep')

uv_dep = dependency('libuv', version : '>=1.0')

gtest = dependency('gtest', main : true, required : false)

if not gtest.found()
    gtest_proj = subproject('gtest')
    gtest_inc = gtest_proj.get_variable('incdir')
    gtest_proj.get_variable('incdir')
    gtest_lib = static_library('gtest', gtest_proj.get_variable('libsources'),
                                gtest_proj.get_variable('mainsources'),
                                include_directories : gtest_inc)

    gtest = declare_dependency(include_directories : gtest_inc,
                               link_with : gtest_lib)
endif


subdir('src')
subdir('app')
subdir('test')
subdir('benchmark')

emulate_rasp = find_program('scripts/emulate_rasp.sh')
run_target('emulate', command : emulate_rasp.path())

fix_gpio_permissions = find_program('scripts/fix_gpio_permissions.sh')
run_target('fixGpioPermissions', command : fix_gpio_permissions.path())
