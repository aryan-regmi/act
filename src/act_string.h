#ifndef ACT_STRING_H
#define ACT_STRING_H

/// @file act_string.h
///
/// This header defines a heap allocated string and various functions related to
/// it.

#include "act_allocator.h"
#include "act_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief **[PRIVATE]** Represents the a heap allocated string.
///
/// @param allocator An allocator for making internal allocations
/// #act_allocator_t.
/// @param len The length of the allocated string.
/// @param capacity The total number of bytes allocated for the string.
/// @param data The actual C-string being stored.
typedef struct act_string_t {
  /// @cond
  /// @internal The allocator used to make necessary allocations.
  const act_allocator_t *_allocator;

  /// @internal The length of the string.
  size_t _len;

  /// @internal The number of bytes allocated.
  size_t _capacity;

  /// @internal The actual string.
  char *_data;
  /// @endcond
} act_string_t;

/// @brief The possible error values.
typedef enum act_string_error_t {
  /// Successful operation.
  ACT_STRING_ERROR_SUCCESS = 0x0,

  /// A null string error.
  ACT_STRING_ERROR_NULL_STRING,

  /// A failure in resizing.
  ACT_STRING_ERROR_RESIZE_FAILED,
} act_string_error_t;

/// @brief Possible return values from #act_string_compare.
typedef enum act_string_comparison_t {
  /// The strings are equal.
  ACT_STRING_COMPARISON_EQUAL = 0x0,

  /// The first string is longer than the second.
  ACT_STRING_COMPARISON_GREATER_THAN,

  /// The first string is shorter than the second.
  ACT_STRING_COMPARISON_LESS_THAN,

  /// The strings are not equal.
  ACT_STRING_COMPARISON_UNEQUAL,

  /// The function returned an error.
  ACT_STRING_COMPARISON_INVALID,
} act_string_comparison_t;

/// @brief Creates a new #act_string_t.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @param allocator The allocator used to make internal memory allocations.
///
/// @return A new heap allocated string.
act_string_t act_string_new(const act_allocator_t *allocator);

/// @brief Creates a new #act_string_t with a specified capacity.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @param allocator The allocator used to make internal memory allocations.
/// @param capacity The number of bytes allocated.
///
/// @return A new heap allocated string with the specified capacity.
act_string_t act_string_with_capacity(const act_allocator_t *allocator,
                                      size_t capacity);

/// @brief Creates a new #act_string_t from the given C-string.
///
/// @note This function allocates as much memory as the length of the given
/// C-string (plus one for the null terminator).
///
/// @param allocator The allocator used to make internal memory allocations.
/// @param cstr The C-string to create the #act_string_t from.
///
/// @return A heap allocated string.
act_string_t act_string_from_cstr(const act_allocator_t *allocator,
                                  const char *cstr);

/// @brief Frees all memory allocated by the #act_string_t.
///
/// @param string The string to free.
///
/// @return Error code.
act_string_error_t act_string_free(act_string_t *string);

/// @brief Returns the length of the #act_string_t.
///
/// @param string The string to get the length of.
///
/// @return Length of @a string.
size_t act_string_len(act_string_t string);

/// @brief Returns the capacity of the #act_string_t.
///
/// @param string The string to get the capacity of.
///
/// @return Capacity of @a string.
size_t act_string_capacity(act_string_t string);

/// @brief Returns the underlying string stored in the #act_string_t.
///
/// @param string The string to get the C-string from.
///
/// @return A C-string (null-terminated @em{const char}).
const char *act_string_as_cstr(act_string_t string);

act_string_error_t act_string_push_char(act_string_t *string, char c);

act_string_error_t act_string_push_cstr(act_string_t *string, const char *cstr);

char act_string_pop_char(act_string_t *string);

ssize_t act_string_find_first_idx_of_char(act_string_t string, char find_char);

act_string_t *act_string_split_at_idx(act_string_t string, size_t idx);

/// @brief Compares two #act_string_t strings to each other.
act_string_comparison_t act_string_compare(act_string_t str1,
                                           act_string_t str2);

act_string_t act_string_copy(const act_string_t *string);

act_string_t act_string_concat(const act_string_t *str1,
                               const act_string_t *str2);
#endif /* !ACT_STRING_H */
