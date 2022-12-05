#include "act_showable.h"

typedef struct act_showable_header_t {
  const act_allocator_t *allocator;
  act_string_t (*as_string)(const act_showable_t *showable);
} act_showable_header_t;

static const act_string_t ACT_SHOWABLE_NULLSTR = {0};

act_showable_t *act_showable_new(
    const act_allocator_t *allocator, size_t struct_size,
    act_string_t (*as_string_func)(const act_showable_t *showable)) {
  ACT_ASSERT(allocator != NULL, NULL);

  // Allocate a act_showable_header_t
  act_showable_header_t *header =
      (*allocator->alloc)(1, sizeof(*header) + struct_size);
  ACT_ASSERT(header != NULL, NULL);

  header->allocator = allocator;
  header->as_string = as_string_func;

  // Return address right after header
  act_showable_t *retval = (uint8_t *)header + sizeof(*header);
  ACT_ASSERT(retval != NULL, NULL);

  return retval;
}

static const act_showable_header_t *
act__showable_get_header(const act_showable_t *showable) {
  ACT_ASSERT(showable != NULL, NULL);

  const act_showable_header_t *header = showable;
  header = (act_showable_header_t *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT(header != NULL, NULL);

  return header;
}

act_showable_error_t act_showable_free(const act_showable_t *showable) {
  ACT_ASSERT(showable != NULL, ACT_SHOWABLE_ERROR_NULL_SHOWABLE);

  const act_showable_header_t *header = act__showable_get_header(showable);
  ACT_ASSERT(header != NULL, ACT_SHOWABLE_ERROR_NULL_HEADER);
  ACT_ASSERT(header->allocator != NULL, ACT_SHOWABLE_ERROR_NULL_ALLOCATOR);

  (*header->allocator->free)(header);

  return ACT_SHOWABLE_ERROR_SUCCESS;
}

act_string_t act_showable_struct_as_string(const act_showable_t *showable) {
  ACT_ASSERT(showable != NULL, ACT_SHOWABLE_NULLSTR);

  const act_showable_header_t *header = act__showable_get_header(showable);
  ACT_ASSERT(header != NULL, ACT_SHOWABLE_NULLSTR);

  // Call `as_string` function of the `showable`
  act_string_t retstr = (*header->as_string)(showable);
  ACT_ASSERT(act_string_capacity(retstr) != 0, ACT_SHOWABLE_NULLSTR);
  ACT_ASSERT(act_string_len(retstr) != 0, ACT_SHOWABLE_NULLSTR);
  ACT_ASSERT(act_string_as_cstr(retstr) != NULL, ACT_SHOWABLE_NULLSTR);

  return retstr;
}

act_showable_error_t act_showable_display(act_showable_t *showable,
                                          FILE *logger) {
  ACT_ASSERT(showable != NULL, ACT_SHOWABLE_ERROR_NULL_SHOWABLE);
  ACT_ASSERT(logger != NULL, ACT_SHOWABLE_ERROR_NULL_LOGGER);

  // Call `as_string` of `showable`
  act_string_t display_str = act_showable_struct_as_string(showable);
  ACT_ASSERT(act_string_capacity(display_str) != 0, -1);
  ACT_ASSERT(act_string_len(display_str) != 0, -1);
  ACT_ASSERT(act_string_as_cstr(display_str) != NULL, -1);

  // Print the `display_str` to the `logger`
  int status = fprintf(logger, "%s", act_string_as_cstr(display_str));
  ACT_ASSERT(status > 0, ACT_SHOWABLE_ERROR_FPRINTF_ERROR);

  act_string_free(&display_str);

  return ACT_SHOWABLE_ERROR_SUCCESS;
}

act_string_t act_showable_uint64_as_string(const act_allocator_t *allocator,
                                           uint64_t val) {
  ACT_ASSERT(allocator != NULL, ACT_SHOWABLE_NULLSTR);

  // Max size required to hold `val` in string
  const size_t MAX_STR_SIZE =
      (size_t)((ceil(log10((double)val)) + 1) * sizeof(char));

  // Write val to string
  char cstr[MAX_STR_SIZE + 1];
  int status = sprintf(cstr, "%zu", val);
  ACT_ASSERT(status > 0, ACT_SHOWABLE_NULLSTR);
  cstr[MAX_STR_SIZE] = '\0';

  act_string_t str = act_string_from_cstr(allocator, cstr);
  return str;
}

act_string_t act_showable_int64_as_string(const act_allocator_t *allocator,
                                          int64_t val) {
  ACT_ASSERT(allocator != NULL, ACT_SHOWABLE_NULLSTR);

  // Max size required to hold `val` in string
  const size_t MAX_STR_SIZE =
      (size_t)((ceil(log10((double)val)) + 1) * sizeof(char));

  // Write val to string
  char cstr[MAX_STR_SIZE + 1];
  int status = sprintf(cstr, "%ld", val);
  ACT_ASSERT(status > 0, ACT_SHOWABLE_NULLSTR);
  cstr[MAX_STR_SIZE] = '\0';

  act_string_t str = act_string_from_cstr(allocator, cstr);
  return str;
}

/// Returns C-string with specified number of '0's.
static const char *act__printZeros(const act_allocator_t *allocator,
                                   size_t num_zeros) {
  char *ret = (*allocator->alloc)(num_zeros + 1, sizeof(char));
  for (size_t i = 0; i < num_zeros; i++) {
    ret[i] = '0';
  }
  ret[num_zeros] = '\0';

  return ret;
}

act_string_t act_showable_double_as_string(const act_allocator_t *allocator,
                                           double val, size_t precision) {
  ACT_ASSERT(allocator != NULL, ACT_SHOWABLE_NULLSTR);

  // Max size to store float as string
  const size_t MAX_STR_SIZE =
      (size_t)((ceil(log10((int)val)) + 1) * sizeof(char) + (double)precision +
               1);

  // Get number of digits in the fractional portion
  const double BASE10 = 10;
  double after_decimal = val - (double)(int)val;
  after_decimal *= pow(BASE10, (double)precision) + 1;
  const size_t after_decimal_size =
      (size_t)((ceil(log10(after_decimal))) * sizeof(char));

  // Get integral and fractional parts of the float
  double integral;
  double fractional = modf(val, &integral);
  fractional = round(fractional * pow(BASE10, (double)precision));

  char cstr[MAX_STR_SIZE + 1];
  // If fractional part has less precision than required, add 0s preceding it
  if (after_decimal_size < precision) {
    const char *zeros =
        act__printZeros(allocator, precision - after_decimal_size);
    int status =
        sprintf(cstr, "%d.%s%d", (int)integral, zeros, (int)fractional);
    (*allocator->free)(zeros);
    ACT_ASSERT(status > 0, ACT_SHOWABLE_NULLSTR);
  } else {
    int status = sprintf(cstr, "%d.%d", (int)integral, (int)fractional);
    ACT_ASSERT(status > 0, ACT_SHOWABLE_NULLSTR);
  }
  cstr[MAX_STR_SIZE] = '\0';

  act_string_t str = act_string_from_cstr(allocator, cstr);
  return str;
}

act_string_t act_showable_cstr_as_string(const act_allocator_t *allocator,
                                         const char *val) {
  ACT_ASSERT(allocator != NULL, ACT_SHOWABLE_NULLSTR);

  // Add quotes around string
  size_t len = strlen(val);
  char quoted[len + 3];
  int status = sprintf(quoted, "\"%s\"", val);
  ACT_ASSERT(status > 0, ACT_SHOWABLE_NULLSTR);

  act_string_t str = act_string_from_cstr(allocator, quoted);
  return str;
}