#include "act_allocator.h"
#include "acutest.h"

void test_can_create_allocator(void) {
  act_allocator_t gpa = GPA;

  TEST_CHECK(gpa.alloc != NULL);
  TEST_CHECK(gpa.resize != NULL);
  TEST_CHECK(gpa.free != NULL);
}

void test_can_use_allocator(void) {
  act_allocator_t gpa = GPA;

  void *ptr = NULL;
  ptr = (*gpa.alloc)(1, sizeof(int));

  TEST_CHECK(ptr != NULL);

  (*gpa.free)(ptr);
}

TEST_LIST = {
    {"[ALLOCATOR] Can create act_allocator", test_can_create_allocator},
    {"[ALLOCATOR] Can allocate using act_allocator", test_can_use_allocator},
    {NULL, NULL}};
