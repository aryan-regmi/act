# Allocator tests
allocator_test = executable(
  'act_unit_tests_allocator',
  'test_act_allocator.c',
  include_directories: [public_types_inc, public_interfaces_inc,external_inc],
  link_with: act_lib,
)
test('Unit Tests Allocator', allocator_test)

# Vector tests
vector_test = executable(
  'act_unit_tests_vector',
  'test_act_vector.c',
  include_directories: [public_types_inc, public_interfaces_inc,external_inc],
  link_with: act_lib,
)
test('Unit Tests Vector', vector_test)

# String tests
string_test = executable(
  'act_unit_tests_string',
  'test_act_string.c',
  include_directories: [public_types_inc, public_interfaces_inc,external_inc],
  link_with: act_lib,
)
test('Unit Tests String', string_test)

# Showable tests
showable_test = executable(
  'act_unit_tests_showable',
  'test_act_showable.c',
  include_directories: [public_types_inc, public_interfaces_inc,external_inc],
  link_with: act_lib,
)
test('Unit Tests Showable', showable_test)
