#include "act.h"
#include "src/core/act_string.h"
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
  act_String str = act_stringNew(&GPA, &err);

  act_vectorFree(vec, &err);
  act_stringFree(&str, &err);

  return 0;
}
