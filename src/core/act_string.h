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
///                   #act_Allocator.
/// @param len        The length of the allocated string.
/// @param capacity   The total number of bytes allocated for the string.
/// @param data       The actual C-string being stored.
///
/// @note All parameters of this struct are **private** and should not be
/// accessed directly; use the associated functions to access them instead.
///
/// @sa #act_stringLen, #act_stringCapacity, #act_stringAsCstr
typedef struct act_String {
  /// @cond
  /// @internal The allocator used to make necessary allocations.
  const act_Allocator *_allocator;

  /// @internal The length of the string.
  size_t _len;

  /// @internal The number of bytes allocated.
  size_t _capacity;

  /// @internal The actual string.
  char *_data;
  /// @endcond
} act_String;

/// @brief The possible error values.
typedef enum act_StringError {
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
} act_StringError;

/// @brief Possible return values from #act_stringCompare.
typedef enum act_StringComparison {
  /// The strings are equal.
  ACT_STRING_COMPARISON_EQUAL = 0x0,

  /// The first string is longer than the second.
  ACT_STRING_COMPARISON_GREATER_THAN,

  /// The first string is shorter than the second.
  ACT_STRING_COMPARISON_LESS_THAN,

  /// The strings are not equal.
  ACT_STRING_COMPARISON_NOT_EQUAL,
} act_StringComparison;

/// @brief Creates a new #act_String.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return A new heap allocated string.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @sa #act_stringFree
act_String act_stringNew(const act_Allocator *allocator, int *error_code);

/// @brief Creates a new #act_String with a specified capacity.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  capacity    The number of bytes allocated.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return A new heap allocated string with the specified capacity.
///
/// @note This function does not allocate any memory until the first @em push
/// operation.
///
/// @sa #act_stringFree
act_String act_stringWithCapacity(const act_Allocator *allocator,
                                  size_t capacity, int *error_code);

/// @brief Creates a new #act_String from the given C-string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  cstr        The C-string to create the #act_String from.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return A heap allocated string.
///
/// @note This function allocates as much memory as the length of the given
/// C-string (plus one for the null terminator).
///
/// @sa #act_stringFree
act_String act_stringFromCstr(const act_Allocator *allocator, const char *cstr,
                              int *error_code);

/// @brief Frees all memory allocated by the #act_String.
///
/// @param[in]  string      The string to free.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
void act_stringFree(act_String *string, int *error_code);

/// @brief Returns the length of the #act_String.
///
/// @param string The string to get the length of.
///
/// @return Length of @a string.
size_t act_stringLen(act_String string);

/// @brief Returns the capacity of the #act_String.
///
/// @param string The string to get the capacity of.
///
/// @return Capacity of @a string.
size_t act_stringCapacity(act_String string);

/// @brief Returns the underlying string stored in the #act_String.
///
/// @param string The string to get the C-string from.
///
/// @return A C-string (null-terminated @em const @em char).
const char *act_stringAsCstr(act_String string);

/// @brief Shrinks the capacity to fit the length of the #act_String (plus one
/// for null terminator).
///
/// The string is only resized if the capacity is larger than the length.
///
/// @param[in]  string      The string to (@em possibly) shrink.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return The shrinked string.
///
/// @note This function @em possibly allocates memory if a resize is triggered.
act_String act_stringShrinkToFit(act_String string, int *error_code);

/// @brief Push a character to the end of the #act_String.
///
/// @param[in]  string      The string to push the character to.
/// @param[in]  c           The character to push to the string.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @note This function allocates memory if the length of the string is zero.
/// @note This function @em possibly allocates memory if a resize is triggered.
///
/// @sa #act_stringPopChar
void act_stringPushChar(act_String *string, char c, int *error_code);

/// @brief Push a C-string (null-terminated) to the end of the #act_String.
///
/// @param[in]  string      The string to push the character to.
/// @param[in]  cstr        The C-string to push to the string.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return Error code.
///
/// @note This function allocates memory if the length of the string is zero.
/// @note This function @em possibly allocates memory if a resize is triggered.
void act_stringPushCstr(act_String *string, const char *cstr, int *error_code);

/// @brief Pop a character from the end of the #act_String.
///
/// The last character in the given string is returned and the length of the
/// string is decremented.
///
/// @param[in]  string      The string to pop the character from.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return The last character in the string.
///
/// @sa #act_stringPushChar
char act_stringPopChar(act_String *string, int *error_code);

/// @brief Finds the index where the given character first occurs in the
/// #act_String.
///
/// @param[in]  string      The string find the character in.
/// @param[in]  find_char   The character to find.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return The first index of the character in the string.
size_t act_stringFindFirstIdxOfChar(act_String string, char find_char,
                                    int *error_code);

/// @brief Splits the given #act_String into two at the given index.
///
/// @param[in]  string      The string to split.
/// @param[in]  idx         The character index to split the string at.
/// @param[out] splits      The array containing the two split strings.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return An array containing the two split strings (@em splits).
///
/// @note This function allocates 2 #act_String that need to be freed.
///
/// @sa #act_stringFree, #act_stringFindFirstIdxOfChar
act_String *act_stringSplitAtIdx(act_String string, size_t idx,
                                 act_String splits[2], int *error_code);

/// @brief Compares two #act_String strings to each other.
///
/// @param[in]  str1        The first string to compare.
/// @param[in]  str2        The second string to compare.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return An enum describing the comparison result (#act_StringComparison).
///
/// @sa #act_StringComparison
act_StringComparison act_stringCompare(act_String str1, act_String str2,
                                       int *error_code);

/// @brief Deep copy the given string.
///
/// @param[in]  string      The string to create a deep copy of.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return A deep-copied version of the input string.
///
/// @note This function allocates as much space as the length of the input
/// string.
///
/// @sa #act_stringFree
act_String act_stringCopy(const act_String *string, int *error_code);

/// @brief Concatenate the two strings into a new one.
///
/// This function does not modify the input parameters.
///
/// @param[in]  str1        The string to concat to.
/// @param[in]  str2        The string to concat.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return A string with the first string concatenated to the second.
///
/// @note This function allocates as much space as the length of the input
/// strings combined.
///
/// @sa #act_stringFree
act_String act_stringConcat(const act_String *str1, const act_String *str2,
                            int *error_code);
#endif /* !ACT_STRING_H */
