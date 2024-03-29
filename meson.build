project('act','c',version:'0.1',default_options: ['warning_level=3', 'buildtype=debug'])

# External Deps
cc = meson.get_compiler('c')
math_dep = cc.find_library('m', required : true)


# Includes
# ========================================
external_inc = include_directories('external')
public_inc = include_directories('include')
public_core_inc = include_directories('include/core')
public_interfaces_inc = include_directories('include/interfaces')
src_core_inc = include_directories('src/core')


# Library
# ========================================
subdir('src')
act_lib = static_library('act', 
  sources,
  include_directories: src_core_inc,
  dependencies: math_dep,
  install: true
)

# Copy headers to `include` directory
copy = find_program('cp')
run_command(
  copy,
  headers,
  'include',
  check: true
)


# Executable
# ========================================
exe = executable(
  'act',
  'act.c',
  include_directories: [public_inc, public_core_inc, public_interfaces_inc],
  link_with: act_lib,
  install: true
)


# Tests
# ========================================
subdir('tests')


# Docs
# ========================================
if get_option('docs')
  doxygen = find_program('doxygen', required : false)
  if not doxygen.found()
    error('MESON_SKIP_TEST doxygen not found.')
  endif

  config_data = configuration_data()
  config_data.set('VERSION', meson.project_version())
  config_data.set('INPUT', meson.source_root() + '/src')
  
  subdir('doc')
endif 

