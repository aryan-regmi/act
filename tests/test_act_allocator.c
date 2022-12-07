#include "act_allocator.h"
#include "acutest.h"

void test_canCreateAllocator(void) {
  act_Allocator gpa = GPA;

  TEST_CHECK(gpa.alloc != NULL);
  TEST_CHECK(gpa.resize != NULL);
  TEST_CHECK(gpa.free != NULL);
}

void test_canUseAllocator(void) {
  act_Allocator gpa = GPA;

  void *ptr = NULL;
  ptr = (*gpa.alloc)(1, sizeof(int));

  TEST_CHECK(ptr != NULL);

  (*gpa.free)(ptr);
}

TEST_LIST = {
    {"[ALLOCATOR] Can create act_allocator", test_canCreateAllocator},
    {"[ALLOCATOR] Can allocate using act_allocator", test_canUseAllocator},
    {NULL, NULL}};
