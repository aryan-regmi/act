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
typedef void act_showable_t;

/// @brief The possible error values.
typedef enum act_showable_error_t {
  /// Successful operation.
  ACT_SHOWABLE_ERROR_SUCCESS = 0x0,

  /// The #act_showable_t pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_SHOWABLE,

  /// The header pointer was NULL.
  ACT_SHOWABLE_ERROR_NULL_HEADER,

  /// The #act_allocator_t pointer was NULL.
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

  /// The string (#act_string_t) was empty.
  ACT_SHOWABLE_ERROR_EMPTY_STRING,

  /// #act_string_free returned an error.
  ACT_SHOWABLE_ERROR_STRING_FREE_FAILED,

  /// A function allocating #act_string_t returned an error.
  ACT_SHOWABLE_ERROR_STRING_ALLOC_FAILED,
} act_showable_error_t;

/// @brief A type alias to the function pointer for #act_showable_new.
typedef act_string_t (*as_string_fnptr)(const act_showable_t *showable);

/// @brief Create a new #act_showable_t type for a struct.
///
/// @param[in]  allocator       The allocator used to make internal memory
///                             allocations.
/// @param[in]  struct_size     The size of the struct to create a showable for.
/// @param[in]  as_string_func  The function used to convert a struct of the
///                             given type to a string.
/// @param[out] error_code      The error code (#act_showable_error_t) of the
///                             operation.
///
/// @return A pointer to an allocated struct of @em struct_size.
///
/// @note This function allocates ```16 + struct_size``` bytes of memory (The
/// 16 bytes is the size of the internal header used to store `showable`
/// information).
///
/// @sa #act_showable_free, #ACT_SHOWABLE_NEW
act_showable_t *
act_showable_new(const act_allocator_t *allocator, size_t struct_size,
                 act_string_t (*as_string_func)(const act_showable_t *),
                 int *error_code);

/// @brief Free the memory allocated by #act_showable_t.
///
/// @param[in]  showable    The #act_showable_t value to free resources for.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @sa #act_showable_new, #ACT_SHOWABLE_NEW
void act_showable_free(const act_showable_t *showable, int *error_code);

/// @brief Returns the string representation of the given #act_showable_t.
///
/// @param[in]  showable    The #act_showable_t value to get as a string.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @return A heap allocated string (#act_string_t) representing the @em
/// showable.
///
/// @sa #act_string_free
act_string_t act_showable_struct_as_string(const act_showable_t *showable,
                                           int *error_code);

/// @brief Prints the given @em showable as a string to the specified @em
/// logger.
///
/// @param[in]  showable    The #act_showable_t value to print as a string.
/// @param[in]  logger      The file stream to write the string to.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @sa #act_showable_struct_as_string
void act_showable_display(act_showable_t *showable, FILE *logger,
                          int *error_code);

/// @brief Convert a @em uint64_t value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em uint64_t value to turn into a string.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @return A heap allocated string (#act_string_t) representing the @em
/// uint64_t value.
///
/// @sa #act_string_free
act_string_t act_showable_uint64_as_string(const act_allocator_t *allocator,
                                           uint64_t val, int *error_code);

/// @brief Convert a @em int64_t value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em int64_t value to turn into a string.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @return A heap allocated string (#act_string_t) representing the @em
/// int64_t value.
///
/// @sa #act_string_free
act_string_t act_showable_int64_as_string(const act_allocator_t *allocator,
                                          int64_t val, int *error_code);

/// @brief Convert a @em double value to a heap allocated string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em double value to turn into a string.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @return A heap allocated string (#act_string_t) representing the @em
/// double value.
///
/// @sa #act_string_free
act_string_t act_showable_double_as_string(const act_allocator_t *allocator,
                                           double val, size_t precision,
                                           int *error_code);

/// @brief Convert a @em C-string value to a heap allocated string.
///
/// This functions just adds quotes (```""```) around the supplied C-string.
///
/// @param[in]  allocator   The allocator used to make internal memory
///                         allocations.
/// @param[in]  val         The @em C-string value.
/// @param[out] error_code  The error code (#act_showable_error_t) of the
///                         operation.
///
/// @return A heap allocated string (#act_string_t) representing the quoted @em
/// C-string value.
///
/// @sa #act_string_free
act_string_t act_showable_cstr_as_string(const act_allocator_t *allocator,
                                         const char *val, int *error_code);

/// @brief A macro to mark a type as #act_showable_t, so that it can be freed
/// with #act_showable_free.
///
/// @param T The type (of struct usually) to create a showable (#act_showable_t)
/// for.
#define ACT_SHOWABLE(T) T *

/// @brief Create a new #act_showable_t type for a struct of type T.
///
/// This is a convinience macro to avoid casts and sizeof when calling
/// #act_showable_new.
///
/// @param T The type of struct to create the #act_showable_t for.
/// @param allocator The allocator used to make internal memory allocations.
/// @param show_func The function used to convert a value of a given type to a
/// string.
///
/// @return A pointer to an allocated struct of @em struct_size.
///
/// @note This function allocates ```16 + sizeof(T)``` bytes of memory (The
/// 16 bytes is the size of the internal header used to store `showable`
/// information).
///
/// @sa #act_showable_free
#define ACT_SHOWABLE_NEW(T, allocator, show_func, error_code)                  \
  act_showable_new(allocator, sizeof(T), (as_string_fnptr)(show_func),         \
                   error_code)

#endif /* !ACT_SHOWABLE_H */
