#include "act_vector.h"

typedef enum act_vector_error_t {
  ACT_VECTOR_ERROR_SUCCESS = 0x0,
  ACT_VECTOR_ERROR_NULL_POINTER,
} act_vector_error_t;

struct act_vector_header_t {
  const act_allocator_t *allocator;

  size_t len;

  size_t capacity;

  size_t data_size;
};

act_vector_t *act_vector_new(const act_allocator_t *allocator,
                             size_t data_size) {
  ACT_ASSERT(allocator != NULL, NULL);

  act_vector_header_t *header = (*allocator->alloc)(1, sizeof(*header) + 1);
  ACT_ASSERT(allocator, NULL);

  header->data_size = data_size;
  header->capacity = 0;
  header->len = 0;
  header->allocator = allocator;

  return (act_vector_t *)((uint8_t *)header + sizeof(*header));
}

act_vector_t *act_vector_with_capacity(const act_allocator_t *allocator,
                                       size_t data_size, size_t capacity) {
  ACT_ASSERT(allocator != NULL, NULL);

  act_vector_header_t *header =
      (*allocator->alloc)(1, sizeof(*header) + data_size * capacity);
  ACT_ASSERT(allocator, NULL);

  header->data_size = data_size;
  header->capacity = capacity;
  header->len = 0;
  header->allocator = allocator;

  return (act_vector_t *)((uint8_t *)header + sizeof(*header));
}

const act_vector_header_t *act__vector_get_header(const act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, NULL);

  const act_vector_header_t *header = vec;
  header = (act_vector_header_t *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT(header != NULL, NULL);

  return header;
}

act_vector_header_t *act__vector_get_mut_header(act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, NULL);

  act_vector_header_t *header = vec;
  header = (act_vector_header_t *)((uint8_t *)header - sizeof(*header));
  ACT_ASSERT(header != NULL, NULL);

  return header;
}

int act_vector_free(const act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  const act_vector_header_t *header = act__vector_get_header(vec);
  ACT_ASSERT(header != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  (*header->allocator->free)(header);

  return ACT_VECTOR_ERROR_SUCCESS;
}

act_vector_t *act__vector_resize(act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, NULL);

  act_vector_header_t *header = act__vector_get_mut_header(vec);
  ACT_ASSERT(vec != NULL, NULL);
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
      act_vector_with_capacity(allocator, data_size, new_cap);
  ACT_ASSERT(new_vec != NULL, NULL);
  memcpy(new_vec, vec, data_size * len);

  // Update the length
  act_vector_header_t *new_header = act__vector_get_mut_header(new_vec);
  new_header->len = len;

  act_vector_free(vec);

  return new_vec;
}

size_t act_vector_len(const act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  const act_vector_header_t *header = act__vector_get_header(vec);
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  return header->len;
}

size_t act_vector_capacity(const act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  const act_vector_header_t *header = act__vector_get_header(vec);
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  return header->capacity;
}

size_t act_vector_data_size(const act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  const act_vector_header_t *header = act__vector_get_header(vec);
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  return header->data_size;
}

int act__vector_incr_len(act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  act_vector_header_t *header = act__vector_get_mut_header(vec);
  ACT_ASSERT(header != NULL, ACT_VECTOR_ERROR_NULL_POINTER);
  header->len++;

  return ACT_VECTOR_ERROR_SUCCESS;
}

int act__vector_decr_len(act_vector_t *vec) {
  ACT_ASSERT(vec != NULL, ACT_VECTOR_ERROR_NULL_POINTER);

  act_vector_header_t *header = act__vector_get_mut_header(vec);
  ACT_ASSERT(header != NULL, ACT_VECTOR_ERROR_NULL_POINTER);
  header->len--;

  return ACT_VECTOR_ERROR_SUCCESS;
}
