#ifndef ACT_SHOWABLE_H
#define ACT_SHOWABLE_H

#include "act_allocator.h"
#include "act_string.h"
#include "act_utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef void act_showable_t;

typedef enum act_showable_error_t {
  ACT_SHOWABLE_ERROR_SUCCESS = 0x0,
  ACT_SHOWABLE_ERROR_NULL_SHOWABLE,
  ACT_SHOWABLE_ERROR_NULL_HEADER,
  ACT_SHOWABLE_ERROR_NULL_ALLOCATOR,
  ACT_SHOWABLE_ERROR_NULL_LOGGER,
  ACT_SHOWABLE_ERROR_FPRINTF_ERROR,
} act_showable_error_t;

#define ACT_SHOWABLE(T) T *

typedef act_string_t (*as_string_fnptr)(const act_showable_t *showable);

act_showable_t *
act_showable_new(const act_allocator_t *allocator, size_t struct_size,
                 act_string_t (*as_string_func)(const act_showable_t *));

#define ACT_SHOWABLE_NEW(T, allocator, show_func)                              \
  act_showable_new(allocator, sizeof(T), (as_string_fnptr)(show_func))

act_showable_error_t act_showable_free(const act_showable_t *showable);

act_string_t act_showable_struct_as_string(const act_showable_t *showable);

act_showable_error_t act_showable_display(act_showable_t *showable,
                                          FILE *logger);

act_string_t act_showable_uint64_as_string(const act_allocator_t *allocator,
                                           uint64_t val);

act_string_t act_showable_int64_as_string(const act_allocator_t *allocator,
                                          int64_t val);

act_string_t act_showable_double_as_string(const act_allocator_t *allocator,
                                           double val, size_t precision);

act_string_t act_showable_cstr_as_string(const act_allocator_t *allocator,
                                         const char *val);

#endif /* !ACT_SHOWABLE */
