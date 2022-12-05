#ifndef ACT_VECTOR_H
#define ACT_VECTOR_H

#include "act_allocator.h"
#include "act_utils.h"
#include <stdint.h>
#include <string.h>

/// @file act_vector.h
///
/// This header defines a heap allocated array and various functions related to
/// it.
///
/// TODO: DOCUMENT ALL ALLOCATING/RESIZING FUNCTIONS!!
/// TODO: Add `shrink_to_fit` function
/// TODO: Only allocate on first push?

/// A type defined for convinece.
typedef void act_vector_t;

/// @brief The header for the "fat-pointer" that holds the vector info.
///
/// This header stores the allocator used for internal allocations, the number
/// of elements stored, the number of bytes allocated, and the size of an
/// element.
typedef struct act_vector_header_t act_vector_header_t;

/// @brief Defines various error codes returned by functions that operate on
/// #act_vector_t.
typedef enum act_vector_error_t {
  /// A succesful operation.
  ACT_VECTOR_ERROR_SUCCESS = 0x0,

  /// The supplied #act_vector_t pointer was NULL.
  ACT_VECTOR_ERROR_NULL_VECTOR,

  /// The #act_vector_header_t pointer was NULL.
  ACT_VECTOR_ERROR_NULL_HEADER,
} act_vector_error_t;

/// @brief Create a new #act_vector_t that stores elements of @em data_size.
///
/// @param allocator The #act_allocator_t used for internal allocations.
/// @param data_size The size of the elements stored in the vector.
///
/// @return A heap allocated array.
///
/// @note This function allocates the ```sizeof(#act_vector_header_t) + 1```.
///
/// @sa #act_vector_free
act_vector_t *act_vector_new(const act_allocator_t *allocator,
                             ssize_t data_size);

/// @brief Create a new #act_vector_t with the specified capacity.
///
/// @param allocator The #act_allocator_t used for internal allocations.
/// @param data_size The size of the elements stored in the vector.
/// @param capacity The number of the elements (of @em data_size) to allocate
/// space for.
///
/// @return A heap allocated array.
///
/// @note This function allocates the ```sizeof(#act_vector_header_t) +
/// data_size*capacity```.
///
/// @sa #act_vector_free
act_vector_t *act_vector_with_capacity(const act_allocator_t *allocator,
                                       ssize_t data_size, ssize_t capacity);

/// @brief Free the memory allocated by the #act_vector_t.
///
/// @param vec The vector to free resources for.
///
/// @return Error code.
///
/// @sa #act_vector_free
act_vector_error_t act_vector_free(const act_vector_t *vec);

/// @brief Get the length of the #act_vector_t.
///
/// @param vec The vector to get the length of.
///
/// @return The length or **-1** if error occured.
ssize_t act_vector_len(const act_vector_t *vec);

/// @brief Get the capacity of the #act_vector_t.
///
/// @param vec The vector to get the capacity of.
///
/// @return The capacity or **-1** if error occured.
ssize_t act_vector_capacity(const act_vector_t *vec);

/// @brief Get the data size of the #act_vector_t.
///
/// @param vec The vector to get the data size of.
///
/// @return The data size or **-1** if error occured.
ssize_t act_vector_data_size(const act_vector_t *vec);

/// @brief Push the value to the end of #act_vector_t.
///
/// @param vec The vector to push the value to.
/// @param value The value to push.
///
/// @note The @em value parameter must be of the same type/size as the
/// data size stored in the #act_vector_t.
///
/// @note This macro @em possibly allocates if a resize if triggered (if the
/// capacity is less than the new length after the push).
#define ACT_VEC_PUSH(vec, value)                                               \
  do {                                                                         \
    ACT_NULLCHECK(vec);                                                        \
                                                                               \
    const act_vector_header_t *header = act__vector_get_header(vec);           \
    ssize_t len = act_vector_len(vec);                                         \
    ssize_t cap = act_vector_capacity(vec);                                    \
    ACT_NULLCHECK(header);                                                     \
                                                                               \
    if (++len > cap) {                                                         \
      (vec) = act__vector_resize(vec);                                         \
      header = act__vector_get_mut_header(vec);                                \
    }                                                                          \
                                                                               \
    (vec)[len - 1] = value;                                                    \
    act__vector_incr_len(vec);                                                 \
  } while (0);

/// @brief Pop the value at the end of #act_vector_t.
///
/// @param vec The vector to pop the value from.
/// @param retptr The pointer to store the returned value.
///
/// @note The type stored in the @em retptr parameter must be of the same
/// type/size as the data_size stored in the #act_vector_t.
#define ACT_VEC_POP(vec, retptr)                                               \
  do {                                                                         \
    ACT_NULLCHECK(vec);                                                        \
                                                                               \
    const act_vector_header_t *header = act__vector_get_header(vec);           \
    ssize_t len = act_vector_len(vec);                                         \
    ACT_NULLCHECK(header);                                                     \
                                                                               \
    act__vector_decr_len(vec);                                                 \
    *(retptr) = (vec)[len - 1];                                                \
  } while (0);

/// @brief A convienience macro defined to remind the user to free the given
/// pointer.
///
/// @sa #act_vector_free
#define ACT_VEC(T) T *

// PRIVATE
// ============================================================================

/// @internal
const act_vector_header_t *act__vector_get_header(const act_vector_t *vec);

/// @internal
act_vector_header_t *act__vector_get_mut_header(act_vector_t *vec);

/// @internal
act_vector_error_t act__vector_incr_len(act_vector_t *vec);

/// @internal
act_vector_error_t act__vector_decr_len(act_vector_t *vec);

/// @internal
act_vector_t *act__vector_resize(act_vector_t *vec);

#endif /* !ACT_VECTOR_H */
