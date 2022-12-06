#ifndef ACT_STRING_H
#define ACT_STRING_H

/// @file act_string.h
///
/// This header defines a heap allocated string and various functions related to
/// it.
///
/// TODO: DOCUMENT ALL ALLOCATING/RESIZING FUNCTIONS!!
/// TODO: DOCUMENT ALL FUNCTIONS THAT RETURN NULLSTR!!
/// TODO: Add `shrink_to_fit` function
/// TODO: Add `insert_at_index` function
/// TODO: Add `delete_from_index` function

#include "act_allocator.h"
#include "act_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief **[PRIVATE]** Represents the a heap allocated string.
///
/// @param allocator  An allocator for making internal allocations
///                   #act_allocator_t.
/// @param len        The length of the allocated string.
/// @param capacity   The total number of bytes allocated for the string.
/// @param data       The actual C-string being stored.
///
/// @note All parameters of this struct are **private** and should not be
/// accessed directly; use the associated functions to access them instead.
///
/// @sa #act_string_len, #act_string_capacity, #act_string_as_cstr
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

  /// The given string was **NULL**.
  ACT_STRING_ERROR_NULL_STRING,

  /// A failure in resizing.
  ACT_STRING_ERROR_RESIZE_FAILED,

  /// The given allocator pointer was **NULL**.
  ACT_STRING_ERROR_NULL_ALLOCATOR,

  /// A failure during allocation.
  ACT_STRING_ERROR_ALLOCATION_FAILED,

  /// A failure during memcpy.
  ACT_STRING_ERROR_MEMCPY_FAILED,

  /// A failure during memmove.
  ACT_STRING_ERROR_MEMMOVE_FAILED,

  /// The given string was empty.
  ACT_STRING_ERROR_EMPTY_STRING,

  /// The character was not found in the string.
  ACT_STRING_ERROR_CHAR_NOT_IN_STRING,

  /// The index was out of bounds.
  ACT_STRING_ERROR_INDEX_OUT_OF_BOUNDS,
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
  ACT_STRING_COMPARISON_NOT_EQUAL,
} act_string_comparison_t;

/// @brief Creates a new #act_string_t.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return A new heap allocated string.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @sa #act_string_free
act_string_t act_string_new(const act_allocator_t *allocator,
                            act_string_error_t *error_code);

/// @brief Creates a new #act_string_t with a specified capacity.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  capacity    The number of bytes allocated.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return A new heap allocated string with the specified capacity.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @sa #act_string_free
act_string_t act_string_with_capacity(const act_allocator_t *allocator,
                                      size_t capacity,
                                      act_string_error_t *error_code);

/// @brief Creates a new #act_string_t from the given C-string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  cstr        The C-string to create the #act_string_t from.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return A heap allocated string.
///
/// @note This function allocates as much memory as the length of the given
/// C-string (plus one for the null terminator).
///
/// @sa #act_string_free
act_string_t act_string_from_cstr(const act_allocator_t *allocator,
                                  const char *cstr,
                                  act_string_error_t *error_code);

/// @brief Frees all memory allocated by the #act_string_t.
///
/// @param[in]  string      The string to free.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
void act_string_free(act_string_t *string, act_string_error_t *error_code);

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

/// @brief Push a character to the end of the #act_string_t.
///
/// @param[in]  string      The string to push the character to.
/// @param[in]  c           The character to push to the string.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @note This function allocates memory if the length of the string is zero.
/// @note This function @em possibly allocates memory if a resize is triggered.
///
/// @sa #act_string_pop_char
void act_string_push_char(act_string_t *string, char c,
                          act_string_error_t *error_code);

/// @brief Push a C-string (null-terminated) to the end of the #act_string_t.
///
/// @param[in]  string      The string to push the character to.
/// @param[in]  cstr        The C-string to push to the string.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return Error code.
///
/// @note This function allocates memory if the length of the string is zero.
/// @note This function @em possibly allocates memory if a resize is triggered.
void act_string_push_cstr(act_string_t *string, const char *cstr,
                          act_string_error_t *error_code);

/// @brief Pop a character from the end of the #act_string_t.
///
/// The last character in the given string is returned and the length of the
/// string is decremented.
///
/// @param[in]  string      The string to pop the character from.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return The last character in the string.
///
/// @sa #act_string_push_char
char act_string_pop_char(act_string_t *string, act_string_error_t *error_code);

/// @brief Finds the index where the given character first occurs in the
/// #act_string_t.
///
/// @param[in]  string      The string find the character in.
/// @param[in]  find_char   The character to find.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return The first index of the character in the string.
size_t act_string_find_first_idx_of_char(act_string_t string, char find_char,
                                         act_string_error_t *error_code);

/// @brief Splits the given #act_string_t into two at the given index.
///
/// @param[in]  string      The string to split.
/// @param[in]  idx         The character index to split the string at.
/// @param[out] splits      The array containing the two split strings.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return An array containing the two split strings (@em splits).
///
/// @note This function allocates 2 #act_string_t that need to be freed.
///
/// @sa #act_string_free, #act_string_find_first_idx_of_char
act_string_t *act_string_split_at_idx(act_string_t string, size_t idx,
                                      act_string_t splits[2],
                                      act_string_error_t *error_code);

/// @brief Compares two #act_string_t strings to each other.
///
/// @param[in]  str1        The first string to compare.
/// @param[in]  str2        The second string to compare.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return An enum describing the comparison result (#act_string_comparison_t).
///
/// @sa #act_string_comparison_t
act_string_comparison_t act_string_compare(act_string_t str1, act_string_t str2,
                                           act_string_error_t *error_code);

/// @brief Deep copy the given string.
///
/// @param[in]  string      The string to create a deep copy of.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return A deep-copied version of the input string.
///
/// @note This function allocates as much space as the length of the input
/// string.
///
/// @sa #act_string_free
act_string_t act_string_copy(const act_string_t *string,
                             act_string_error_t *error_code);

/// @brief Concatenate the two strings into a new one.
///
/// This function does not modify the input parameters.
///
/// @param[in]  str1        The string to concat to.
/// @param[in]  str2        The string to concat.
/// @param[out] error_code  The error code (#act_string_error_t) of the
///                         operation.
///
/// @return A string with the first string concatenated to the second.
///
/// @note This function allocates as much space as the length of the input
/// strings combined.
///
/// @sa #act_string_free
act_string_t act_string_concat(const act_string_t *str1,
                               const act_string_t *str2,
                               act_string_error_t *error_code);
#endif /* !ACT_STRING_H */
