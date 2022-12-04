#ifndef ACT_UTILS_H
#define ACT_UTILS_H

/// @file act_utils.h
///
/// This header defines common utilities used by various programs.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/// Checks if the given pointer is NULL.
///
/// @param ptr The pointer to check against NULL.
/// @param logger The stream to log the error to.
/// @param file The file this function is called in.
/// @param line The line number this function is called in.
static inline void act_null_check(const void *ptr, FILE *logger,
                                  const char *file, size_t line) {
  if (ptr == NULL) {
    int err = fprintf(
        logger,
        "[ERROR] NullCheckFailure: Pointer supplied at [%s:%zu] was NULL\n",
        file, line);
    if (!err) {
      exit(EXIT_FAILURE);
    }
  }
}

#ifndef ACT_ASSERT_NOOP

/// This macro calls @a act_null_check with `stderr` as the logger.
///
/// @param ptr The pointer to check against @p NULL.
#define ACT_NULLCHECK(ptr) act_null_check(ptr, stderr, __FILE__, __LINE__)

/// This macro asserts that the given expression is `true`.
///
/// @param expr The boolean expression that is checked against `true`
/// @param retval The value that that is return if @a expr is `false`.
#define ACT_ASSERT(expr, retval)                                               \
  do {                                                                         \
    if ((expr) == false) {                                                     \
      return retval;                                                           \
    }                                                                          \
  } while (0);

#else
#define ACT_NULLCHECK(_ptr)

#define ACT_ASSERT(_expr)
#endif

#endif /* !ACT_UTILS_H */
