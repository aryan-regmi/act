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

/// A type defined for convinece.
typedef void act_Vector;

/// @brief The header for the "fat-pointer" that holds the vector info.
///
/// This header stores the allocator used for internal allocations, the number
/// of elements stored, the number of bytes allocated, and the size of an
/// element.
typedef struct act_VectorHeader act_VectorHeader;

/// @brief Defines various error codes returned by functions that operate on
/// #act_Vector.
typedef enum act_VectorError {
  /// A succesful operation.
  ACT_VECTOR_ERROR_SUCCESS = 0x0,

  /// The supplied #act_Vector pointer was NULL.
  ACT_VECTOR_ERROR_NULL_VECTOR,

  /// The #act_VectorHeader pointer was NULL.
  ACT_VECTOR_ERROR_NULL_HEADER,

  /// The #act_Allocator pointer was NULL.
  ACT_VECTOR_ERROR_NULL_ALLOCATOR,

  /// **@em memcpy** failed.
  ACT_VECTOR_ERROR_MEMCPY_FAILED,

  /// Resize #act_Vector failed.
  ACT_VECTOR_ERROR_RESIZE_FAILED,
} act_VectorError;

/// @brief Create a new #act_Vector that stores elements of @em data_size.
///
/// @param[in]  allocator   The #act_Allocator used for internal allocations.
/// @param[in]  data_size   The size of the elements stored in the vector.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return A heap allocated array.
///
/// @note This function allocates the ```sizeof(#act_VectorHeader) + 1```.
///
/// @sa #act_vector_free
act_Vector *act_vectorNew(const act_Allocator *allocator, size_t data_size,
                          int *error_code);

/// @brief Create a new #act_Vector with the specified capacity.
///
/// @param[in]  allocator   The #act_Allocator used for internal allocations.
/// @param[in]  data_size   The size of the elements stored in the vector.
/// @param[in]  capacity    The number of the elements (of @em data_size) to
///                         allocate space for.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return A heap allocated array.
///
/// @note This function allocates the ```sizeof(#act_VectorHeader) +
/// data_size*capacity```.
///
/// @sa #act_vector_free
act_Vector *act_vectorWithCapacity(const act_Allocator *allocator,
                                   size_t data_size, size_t capacity,
                                   int *error_code);

/// @brief Shrinks the capacity to fit the length of the #act_Vector.
///
/// The vector is only resized if the capacity is larger than the length.
///
/// @param[in]  vec         The vec to (@em possibly) shrink.
/// @param[out] error_code  The error code (#act_StringError) of the
///                         operation.
///
/// @return The shrinked vector.
///
/// @note This function @em possibly allocates memory if a resize is triggered.
void act_vectorShrinkToFit(act_Vector *vec, int *error_code);

/// @brief Free the memory allocated by the #act_Vector.
///
/// @param[in]  vec         The vector to free resources for.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @sa #act_vector_free
void act_vectorFree(const act_Vector *vec, int *error_code);

/// @brief Get the length of the #act_Vector.
///
/// @param[in]  vec         The vector to get the length of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return The length of @em vec.
size_t act_vectorLen(const act_Vector *vec, int *error_code);

/// @brief Get the capacity of the #act_Vector.
///
/// @param[in]  vec         The vector to get the capacity of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return The capacity of @em vec.
size_t act_vectorCapacity(const act_Vector *vec, int *error_code);

/// @brief Get the data size of the #act_Vector.
///
/// @param[in]  vec         The vector to get the data size of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return The data size of @em vec.
size_t act_vectorDataSize(const act_Vector *vec, int *error_code);

/// @brief Get the allocator of the #act_Vector.
///
/// @param[in]  vec         The vector to get the allocator of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return The allocator used for @em vec allocations.
const act_Allocator *act_vectorAllocator(const act_Vector *vec,
                                         int *error_code);

/// @brief Create a new #act_Vector that stores elements of type @em T.
///
/// This macro calls #act_vector_new, and passes the size of @em T as the @em
/// data_size parameter.
///
/// @param[in]  T           The type of the elements stored in the vector.
/// @param[in]  allocator   The #act_Allocator used for internal allocations.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return A heap allocated array.
///
/// @note This macro allocates the ```sizeof(#act_VectorHeader) + 1```.
///
/// @sa #act_vector_free, #act_vector_new
#define ACT_VEC_NEW(T, allocator, error_code)                                  \
  act_vectorNew(allocator, sizeof(T), error_code)

/// @brief Create a new #act_Vector with the specified capacity.
///
/// This macro calls #act_vector_with_capacity, and passes the size of @em T as
/// the @em data_size parameter.
///
/// @param[in]  T           The type of the elements stored in the vector.
/// @param[in]  allocator   The #act_Allocator used for internal allocations.
/// @param[in]  capacity    The number of the elements (of @em data_size) to
///                         allocate space for.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return A heap allocated array.
///
/// @note This macro allocates the ```sizeof(#act_VectorHeader) +
/// sizeof(T)*capacity```.
///
/// @sa #act_vector_free
#define ACT_VEC_WCAP(T, allocator, capacity, error_code)                       \
  act_vectorWithCapacity(allocator, sizeof(T), capacity, error_code)

/// @brief Push the value to the end of #act_Vector.
///
/// @param[in]  vec         The vector to push the value to.
/// @param[in]  value       The value to push.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note The @em value parameter must be of the same type/size as the
/// data size stored in the #act_Vector.
///
/// @note This macro @em possibly allocates if a resize if triggered (if the
/// capacity is less than the new length after the push).
#define ACT_VEC_PUSH(vec, value, error_code)                                   \
  do {                                                                         \
    *(error_code) = ACT_VECTOR_ERROR_SUCCESS;                                  \
                                                                               \
    ACT_ASSERT_OR((vec) != NULL,                                               \
                  *(error_code) = ACT_VECTOR_ERROR_NULL_VECTOR);               \
                                                                               \
    size_t len = act_vectorLen(vec, error_code);                               \
    size_t cap = act_vectorCapacity(vec, error_code);                          \
                                                                               \
    if (++len > cap) {                                                         \
      (vec) = act__vectorResize(vec, error_code);                              \
      ACT_ASSERT_OR((vec) != NULL,                                             \
                    *(error_code) = ACT_VECTOR_ERROR_NULL_VECTOR);             \
    }                                                                          \
                                                                               \
    (vec)[len - 1] = value;                                                    \
    act__vectorIncrLen(vec, error_code);                                       \
  } while (0);

/// @brief Pop the value at the end of #act_Vector.
///
/// @param[in]  vec         The vector to pop the value from.
/// @param[in]  retptr      The pointer to store the returned value.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note The type stored in the @em retptr parameter must be of the same
/// type/size as the data_size stored in the #act_Vector.
#define ACT_VEC_POP(vec, retptr, error_code)                                   \
  do {                                                                         \
    *(error_code) = ACT_VECTOR_ERROR_SUCCESS;                                  \
                                                                               \
    ACT_ASSERT_OR((vec) != NULL,                                               \
                  *(error_code) = ACT_VECTOR_ERROR_NULL_VECTOR);               \
                                                                               \
    size_t len = act_vectorLen(vec, error_code);                               \
                                                                               \
    act__vectorDecrLen(vec, error_code);                                       \
    *(retptr) = (vec)[len - 1];                                                \
  } while (0);

/// @brief Create a deep copy the #act_Vector.
///
/// @param[in]  T           The type of values stored in the vector.
/// @param[in]  original    The vector to copy (#act_Vector).
/// @param[out] copy        The copy (#act_Vector).
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @sa #ACT_VEC_COPY
#define ACT_VEC_COPY(T, original, copy, error_code)                            \
  do {                                                                         \
    if ((original) == NULL || (copy) == NULL) {                                \
      *(error_code) = ACT_VECTOR_ERROR_NULL_VECTOR;                            \
      return;                                                                  \
    }                                                                          \
                                                                               \
    T *original_vec = original;                                                \
    T *copy_vec = copy;                                                        \
    for (size_t i = 0; i < act_vectorLen(original, error_code); i++) {         \
      ACT_VEC_PUSH(copy_vec, original_vec[i], error_code);                     \
    }                                                                          \
  } while (0);

/// @brief Shrink the #act_Vector's capacity to its length.
///
/// @param[in]  T           The type of values stored in the vector.
/// @param[in]  vec         The vector to pop the value from.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @sa #ACT_VEC_COPY
#define ACT_VEC_SHRINK_TO_FIT(T, vec, error_code)                              \
  do {                                                                         \
    *(error_code) = ACT_VECTOR_ERROR_SUCCESS;                                  \
                                                                               \
    size_t len = act_vectorLen(vec, error_code);                               \
    size_t capacity = act_vectorCapacity(vec, error_code);                     \
    size_t data_size = act_vectorDataSize(vec, error_code);                    \
    const act_Allocator *allocator = act_vectorAllocator(vec, error_code);     \
                                                                               \
    if (capacity > len) {                                                      \
      act_Vector *shrunk =                                                     \
          act_vectorWithCapacity(allocator, data_size, len, error_code);       \
                                                                               \
      ACT_VEC_COPY(T, vec, shrunk, error_code);                                \
                                                                               \
      act_VectorHeader *header = act__vectorGetMutHeader(vec, error_code);     \
      (*allocator->free)(header);                                              \
                                                                               \
      (vec) = shrunk;                                                          \
    }                                                                          \
  } while (0)

/// @brief A convienience macro defined to remind the user to free the given
/// pointer.
///
/// @sa #act_vector_free
#define ACT_VEC(T) T *

// PRIVATE
// ============================================================================

/// @internal
/// @brief [PRIVATE] Gets a constant pointer to the #act_Vector's header.
///
/// This is for internal use only; the #act_VectorHeader should never be
/// accessed from public API.
///
/// @param[in]  vec         The vector to get the header of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note This function allocates ````vec->capacity * 2`` bytes.
///
/// @return The header of the vector.
///
/// @sa #act_VectorHeader
const act_VectorHeader *act__vectorGetHeader(const act_Vector *vec,
                                             int *error_code);

/// @internal
/// @brief [PRIVATE] Gets a mutable pointer to the #act_Vector's header.
///
/// This is for internal use only; the #act_VectorHeader should never be
/// accessed from public API.
///
/// @param[in]  vec         The vector to get the header of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note This function allocates ````vec->capacity * 2`` bytes.
///
/// @return The header of the vector.
///
/// @sa #act_VectorHeader
act_VectorHeader *act__vectorGetMutHeader(act_Vector *vec, int *error_code);

/// @internal
/// @brief [PRIVATE] Increments the length of the #act_Vector.
///
/// This is purely for macros that don't have access to the internals of the
/// #act_VectorHeader.
///
/// @param[in]  vec         The vector to increment the length of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note This function allocates ````vec->capacity * 2`` bytes.
///
/// @sa #ACT_VEC_PUSH, #ACT_VEC_POP
void act__vectorIncrLen(act_Vector *vec, int *error_code);

/// @internal
/// @brief [PRIVATE] Decrements the length of the #act_Vector.
///
/// This is purely for macros that don't have access to the internals of the
/// #act_VectorHeader.
///
/// @param[in]  vec         The vector to decrement the length of.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @note This function allocates ````vec->capacity * 2`` bytes.
///
/// @sa #ACT_VEC_PUSH, #ACT_VEC_POP
void act__vectorDecrLen(act_Vector *vec, int *error_code);

/// @internal
/// @brief [PRIVATE] Resizes the given #act_Vector by doubling its capacity.
///
/// @param[in]  vec         The vector to resize.
/// @param[out] error_code  The error code (#act_VectorError) of the
///                         operation.
///
/// @return The resized vector, @em vec.
///
/// @note This function allocates ````vec->capacity * 2`` bytes.
act_Vector *act__vectorResize(act_Vector *vec, int *error_code);

#endif /* !ACT_VECTOR_H */
