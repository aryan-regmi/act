#include "act_vector.h"

struct act_vector_header_t {
  const act_allocator_t *allocator;

  size_t len;

  size_t capacity;

  size_t data_size;
};

act_vector_t *act_vector_new(const act_allocator_t *allocator,
                             ssize_t data_size, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_VECTOR_ERROR_NULL_ALLOCATOR);

  act_vector_header_t *header = (*allocator->alloc)(1, sizeof(*header) + 1);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  header->data_size = data_size;
  header->capacity = 0;
  header->len = 0;
  header->allocator = allocator;

  return (act_vector_t *)((uint8_t *)header + sizeof(*header));
}

act_vector_t *act_vector_with_capacity(const act_allocator_t *allocator,
                                       size_t data_size, size_t capacity,
                                       int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_VECTOR_ERROR_NULL_ALLOCATOR);

  act_vector_header_t *header =
      (*allocator->alloc)(1, sizeof(*header) + data_size * capacity);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  header->data_size = data_size;
  header->capacity = capacity;
  header->len = 0;
  header->allocator = allocator;

  return (act_vector_t *)((uint8_t *)header + sizeof(*header));
}

const act_vector_header_t *act__vector_get_header(const act_vector_t *vec,
                                                  int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_vector_header_t *header = vec;
  header = (act_vector_header_t *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header;
}

act_vector_header_t *act__vector_get_mut_header(act_vector_t *vec,
                                                int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_vector_header_t *header = vec;
  header = (act_vector_header_t *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header;
}

void act_vector_free(const act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_vector_header_t *header = act__vector_get_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  (*header->allocator->free)(header);
}

act_vector_t *act__vector_resize(act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_vector_header_t *header = act__vector_get_mut_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  size_t len = header->len;
  size_t cap = header->capacity;
  size_t data_size = header->data_size;
  const act_allocator_t *allocator = header->allocator;

  size_t new_cap = 0;
  if (cap == 0) {
    new_cap = 1;
  } else {
    new_cap = 2 * cap;
  }

  // Create new vec with double the capacity and copy over old data
  act_vector_t *new_vec =
      act_vector_with_capacity(allocator, data_size, new_cap, error_code);
  ACT_ASSERT_OR(new_vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);
  act_vector_t *tmp = memcpy(new_vec, vec, data_size * len);
  ACT_ASSERT_OR(tmp != NULL, *error_code = ACT_VECTOR_ERROR_MEMCPY_FAILED);
  new_vec = tmp;

  // Update the length
  act_vector_header_t *new_header =
      act__vector_get_mut_header(new_vec, error_code);
  ACT_ASSERT_OR(new_header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  new_header->len = len;

  act_vector_free(vec, error_code);

  return new_vec;
}

size_t act_vector_len(const act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_vector_header_t *header = act__vector_get_header(vec, error_code);
  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->len;
}

size_t act_vector_capacity(const act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_vector_header_t *header = act__vector_get_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->capacity;
}

size_t act_vector_data_size(const act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  const act_vector_header_t *header = act__vector_get_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);

  return header->data_size;
}

void act__vector_incr_len(act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_vector_header_t *header = act__vector_get_mut_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  header->len++;
}

void act__vector_decr_len(act_vector_t *vec, int *error_code) {
  *error_code = ACT_VECTOR_ERROR_SUCCESS;

  ACT_ASSERT_OR(vec != NULL, *error_code = ACT_VECTOR_ERROR_NULL_VECTOR);

  act_vector_header_t *header = act__vector_get_mut_header(vec, error_code);
  ACT_ASSERT_OR(header != NULL, *error_code = ACT_VECTOR_ERROR_NULL_HEADER);
  header->len--;
}
