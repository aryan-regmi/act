project('act','c',version:'0.1',default_options: ['warning_level=3', 'buildtype=debug'])


# Includes
# ========================================
external_inc = include_directories('external')
public_inc = include_directories('include')


# Library
# ========================================
subdir('src')
act_lib = static_library('act', 
  sources,
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
  include_directories: public_inc,
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

