#ifndef ACT_SHOWABLE_H
#define ACT_SHOWABLE_H

#include "act_allocator.h"
#include "act_string.h"
#include "act_utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/// @file act_showable.h
///
/// This header defines an interface that allows priting structs as strings.

/// @brief A type defined for convinece.
typedef void act_Showable;

/// @brief The possible error values.
typedef enum act_ShowableError {
  /// Successful operation.
  ACT_SHOWABLE_ERROR_SUCCESS = 0x0,

  /// The #act_Showable pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_SHOWABLE,

  /// The header pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_HEADER,

  /// The #act_Allocator pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_ALLOCATOR,

  /// The pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_POINTER,

  /// The allocation failed.
  ACT_SHOWABLE_ERROR_ALLOCATION_FAILED,

  /// The provided logger (@em FILE*) pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_LOGGER,

  /// **@em fprintf** returned an error.
  ACT_SHOWABLE_ERROR_FPRINTF_ERROR,

  /// **@em sprintf** returned an error.
  ACT_SHOWABLE_ERROR_SPRINTF_ERROR,

  /// The string (#act_String) was empty.
  ACT_SHOWABLE_ERROR_EMPTY_STRING,

  /// #act_stringFree returned an error.
  ACT_SHOWABLE_ERROR_STRING_FREE_FAILED,

  /// A function allocating #act_String returned an error.
  ACT_SHOWABLE_ERROR_STRING_ALLOC_FAILED,
} act_ShowableError;

/// @brief A type alias to the function pointer for #act_showableNew.
typedef act_String (*AsStringFnPtr)(const act_Showable *showable);

/// @brief Create a new #act_Showable type for a struct.
///
/// @param[in]  allocator       The allocator used to make internal memory
///                             allocations.
/// @param[in]  struct_size     The size of the struct to create a showable for.
/// @param[in]  as_string_func  The function used to convert a struct of the
///                             given type to a string.
/// @param[out] error_code      The error code (#act_ShowableError) of the
///                             operation.
///
/// @return A pointer to an allocated struct of @em struct_size.
///
/// @note This function allocates ```16 + struct_size``` bytes of memory (The
/// 16 bytes is the size of the internal header used to store `showable`
/// information).
///
/// @sa #act_showableFree, #act_showableNew
act_Showable *
act_showableNew(const act_Allocator *allocator, size_t struct_size,
                act_String (*as_string_func)(const act_Showable *),
                int *error_code);

/// @brief Free the memory allocated by #act_Showable.
///
/// @param[in]  showable    The #act_Showable value to free resources for.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @sa #act_showableNew, #ACT_SHOWABLE_NEW
void act_showableFree(const act_Showable *showable, int *error_code);

/// @brief Returns the string representation of the given #act_Showable.
///
/// @param[in]  showable    The #act_Showable value to get as a string.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @return A heap allocated string (#act_String) representing the @em
/// showable.
///
/// @sa #act_stringFree
act_String act_showableStructAsString(const act_Showable *showable,
                                      int *error_code);

/// @brief Prints the given @em showable as a string to the specified @em
/// logger.
///
/// @param[in]  showable    The #act_Showable value to print as a string.
/// @param[in]  logger      The file stream to write the string to.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @sa #act_showableStructAsString
void act_showableDisplay(act_Showable *showable, FILE *logger, int *error_code);

/// @brief Convert a @em uint64_t value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em uint64_t value to turn into a string.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @return A heap allocated string (#act_String) representing the @em
/// uint64_t value.
///
/// @sa #act_stringFree
act_String act_showableUint64AsString(const act_Allocator *allocator,
                                      uint64_t val, int *error_code);

/// @brief Convert a @em int64_t value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em int64_t value to turn into a string.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @return A heap allocated string (#act_String) representing the @em
/// int64_t value.
///
/// @sa #act_stringFree
act_String act_showableInt64AsString(const act_Allocator *allocator,
                                     int64_t val, int *error_code);

/// @brief Convert a @em double value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em double value to turn into a string.
/// @param[in]  precision   The number of decimal points to display in the
///                         string.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @return A heap allocated string (#act_String) representing the @em
/// double value.
///
/// @sa #act_stringFree
act_String act_showableDoubleAsString(const act_Allocator *allocator,
                                      double val, size_t precision,
                                      int *error_code);

/// @brief Convert a @em C-string value to a heap allocated string.
///
/// This functions just adds quotes (```""```) around the supplied C-string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em C-string value.
/// @param[out] error_code  The error code (#act_ShowableError) of the
///                         operation.
///
/// @return A heap allocated string (#act_String) representing the quoted @em
/// C-string value.
///
/// @sa #act_stringFree
act_String act_showableCstrAsString(const act_Allocator *allocator,
                                    const char *val, int *error_code);

/// @brief A macro to mark a type as #act_Showable, so that it can be freed
/// with #act_showableFree.
///
/// @param T The type (of struct usually) to create a showable (#act_Showable)
/// for.
#define ACT_SHOWABLE(T) T *

/// @brief Create a new #act_Showable type for a struct of type T.
///
/// This is a convinience macro to avoid casts and sizeof when calling
/// #act_showableNew.
///
/// @param[in]  T               The type of struct to create the #act_Showable
///                             for.
/// @param[in]  allocator       The allocator used to make internal memory
///                             allocations.
/// @param[in]  show_func       The function used to convert a value of a given
///                             type to a string.
/// @param[out] error_code      The error code (#act_ShowableError) of the
///                             operation.
///
/// @return A pointer to an allocated struct of @em struct_size.
///
/// @note This function allocates ```16 + sizeof(T)``` bytes of memory (The
/// 16 bytes is the size of the internal header used to store `showable`
/// information).
///
/// @sa #act_showableFree
#define ACT_SHOWABLE_NEW(T, allocator, show_func, error_code)                  \
  act_showableNew(allocator, sizeof(T), (AsStringFnPtr)(show_func), error_code)

#endif /* !ACT_SHOWABLE_H */
