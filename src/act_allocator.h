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
void *act__realloc_wrapper(void *ptr, size_t nelems, size_t elem_size);

/// @internal
void act__free_wrapper(const void *ptr);

// PUBLIC
// ===================================================================

/// @struct act_allocator_t
/// @brief An allocator used for allocating, resizing, and freeing memory.
typedef struct act_allocator_t {
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
} act_allocator_t;

/// A general purpose allocator for ease of use.
static const act_allocator_t GPA = {
    .alloc = calloc,
    .resize = act__realloc_wrapper,
    .free = act__free_wrapper,
};

#endif /* !ACT_ALLOCATOR_H */
