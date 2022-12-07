#include "act_allocator.h"
#include "act_vector.h"
#include <stdio.h>

#define PROJECT_NAME "act"

int main(int argc, char **argv) {
  if (argc != 1) {
    printf("%s takes no arguments.\n", argv[0]);
    return 1;
  }
  printf("This is project %s.\n", PROJECT_NAME);

  int err = 0;
  ACT_VEC(int) vec = ACT_VEC_NEW(int, &GPA, &err);

  act_vectorFree(vec, &err);
  // act_string_free(&str, &err);

  return 0;
}
