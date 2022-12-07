include_directories('../types')

interfaces_headers = files([
  'act_showable.h'
])

sources += files([
  'act_showable.c'
])

# Copy headers to include/interfaces
interfaces_include_dir = meson.source_root() + '/include/interfaces'
mkdir = find_program('mkdir')
run_command(
  mkdir,
  '-p',
  interfaces_include_dir,
  check: true
)
copy = find_program('cp')
run_command(
  copy,
  interfaces_headers,
  interfaces_include_dir,
  check: true
)

