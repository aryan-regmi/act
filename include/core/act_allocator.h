#ifndef ACT_ALLOCATOR_H
#define ACT_ALLOCATOR_H

#include <stdlib.h>

/// @file act_allocator.h
///
/// This header defines an allocator type used for supplying custom allocators
/// to functions and structs.

// PRIVATE
// ===================================================================

/// @internal
/// @brief [PRIVATE] A wrapper for the standard library's **@em realloc**
/// function.
///
/// This is for internal use by #GPA.
void *act__reallocWrapper(void *ptr, size_t nelems, size_t elem_size);

/// @internal
/// @brief [PRIVATE] A wrapper for the standard library's **@em free** function.
///
/// This is for internal use by #GPA.
void act__freeWrapper(const void *ptr);

// PUBLIC
// ===================================================================

/// @brief An allocator used for allocating, resizing, and freeing memory.
typedef struct act_Allocator {
  /// @brief The function used to allocate memory.
  ///
  /// @param nelems The number of elements to allocate.
  /// @param elem_size The size of a single element.
  void *(*alloc)(size_t nelems, size_t elem_size);

  /// @brief The function used to resize a pointer to memory.
  ///
  /// @param ptr The pointer to reallocate.
  /// @param nelems The number of elements to allocate.
  /// @param elem_size The size of a single element.
  void *(*resize)(void *ptr, size_t nelems, size_t elem_size);

  /// @brief The function used to free a pointer to memory.
  ///
  /// @param ptr The pointer to free.
  void (*free)(const void *ptr);
} act_Allocator;

/// @brief A general purpose allocator for ease of use.
static const act_Allocator GPA = {
    .alloc = calloc,
    .resize = act__reallocWrapper,
    .free = act__freeWrapper,
};

#endif /* !ACT_ALLOCATOR_H */
