#include "act_vector.h"

struct act_VectorHeader {
  const act_Allocator *allocator;

  size_t len;

  size_t capacity;

  size_t data_size;
};

act_Vector *act_vectorNew(const act_Allocator *allocator, ssize_t data_size,
                          int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_VECTOR_ERROR_NULL_ALLOCATOR);

  act_VectorHeader *header = (*allocator->alloc)(1, sizeof(*header) + 1);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  header->data_size = data_size;
  header->capacity = 0;
  header->len = 0;
  header->allocator = allocator;

  return (act_Vector *)((uint8_t *)header + sizeof(*header));
}

act_Vector *act_vectorWithCapacity(const act_Allocator *allocator,
                                   size_t data_size, size_t capacity,
                                   int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_VECTOR_ERROR_NULL_ALLOCATOR);

  act_VectorHeader *header =
      (*allocator->alloc)(1, sizeof(*header) + data_size * capacity);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  header->data_size = data_size;
  header->capacity = capacity;
  header->len = 0;
  header->allocator = allocator;

  return (act_Vector *)((uint8_t *)header + sizeof(*header));
}

const act_VectorHeader *act__vectorGetHeader(const act_Vector *vec,
                                             int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = vec;
  header = (act_VectorHeader *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header;
}

act_VectorHeader *act__vectorGetMutHeader(act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_VectorHeader *header = vec;
  header = (act_VectorHeader *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header;
}

void act_vectorFree(const act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = act__vectorGetHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  (*header->allocator->free)(header);
}

act_Vector *act__vectorResize(act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_VectorHeader *header = act__vectorGetMutHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  size_t len = header->len;
  size_t cap = header->capacity;
  size_t data_size = header->data_size;
  const act_Allocator *allocator = header->allocator;

  size_t new_cap = 0;
  if (cap == 0) {
    new_cap = 1;
  } else {
    new_cap = 2 * cap;
  }

  // Create new vec with double the capacity and copy over old data
  act_Vector *new_vec =
      act_vectorWithCapacity(allocator, data_size, new_cap, error_code);
  ACT_ASSERT_OR(new_vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);
  act_Vector *tmp = memcpy(new_vec, vec, data_size * len);
  ACT_ASSERT_OR(tmp != NULL, *error_code = ACT_VECTOR_ERROR_MEMCPY_FAILED);
  new_vec = tmp;

  // Update the length
  act_VectorHeader *new_header = act__vectorGetMutHeader(new_vec, error_code);
  ACT_ASSERT_OR(new_header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  new_header->len = len;

  act_vectorFree(vec, error_code);

  return new_vec;
}

size_t act_vectorLen(const act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = act__vectorGetHeader(vec, error_code);
  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->len;
}

size_t act_vectorCapacity(const act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = act__vectorGetHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->capacity;
}

size_t act_vectorDataSize(const act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = act__vectorGetHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->data_size;
}

const act_Allocator *act_vectorAllocator(const act_Vector *vec,
                                         int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_VectorHeader *header = act__vectorGetHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->allocator;
}

void act__vectorIncrLen(act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_VectorHeader *header = act__vectorGetMutHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  header->len++;
}

void act__vectorDecrLen(act_Vector *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_VectorHeader *header = act__vectorGetMutHeader(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  header->len--;
}
