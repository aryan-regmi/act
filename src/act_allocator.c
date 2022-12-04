#include "act_allocator.h"
#include "act_utils.h"
#include <stdio.h>
#include <stdlib.h>

void *act__realloc_wrapper(void *ptr, size_t nelems, size_t elem_size) {
  ACT_NULLCHECK(ptr);

  void *tmp = realloc(ptr, nelems * elem_size);
  ACT_NULLCHECK(tmp);

  ptr = tmp;
  return ptr;
}

void act__free_wrapper(const void *ptr) { free((void *)ptr); }
