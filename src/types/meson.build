base_headers = files([
  'act_allocator.h',
  'act_string.h',
  'act_string.h',
  'act_utils.h',
  'act_vector.h',
])

sources += files([
  'act_allocator.c',
  'act_string.c',
  'act_vector.c',
])

# Copy headers to include/base
base_include_dir = meson.source_root() + '/include/types'
mkdir = find_program('mkdir')
run_command(
  mkdir,
  '-p',
  base_include_dir,
  check: true
)
copy = find_program('cp')
run_command(
  copy,
  base_headers,
  base_include_dir,
  check: true
)
