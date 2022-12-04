#ifndef ACT_VECTOR_H
#define ACT_VECTOR_H

#include "act_allocator.h"
#include "act_utils.h"
#include <stdint.h>
#include <string.h>

// TODO: Only allocate on first push?

typedef void act_vector_t;
typedef struct act_vector_header_t act_vector_header_t;

act_vector_t *act_vector_new(const act_allocator_t *allocator,
                             size_t data_size);

act_vector_t *act_vector_with_capacity(const act_allocator_t *allocator,
                                       size_t data_size, size_t capacity);

int act_vector_free(const act_vector_t *vec);

size_t act_vector_len(const act_vector_t *vec);

size_t act_vector_capacity(const act_vector_t *vec);

size_t act_vector_data_size(const act_vector_t *vec);

#define ACT_VEC_PUSH(vec, value)                                               \
  do {                                                                         \
    ACT_NULLCHECK(vec);                                                        \
                                                                               \
    const act_vector_header_t *header = act__vector_get_header(vec);           \
    size_t len = act_vector_len(vec);                                          \
    size_t cap = act_vector_capacity(vec);                                     \
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

//
#define ACT_VEC_POP(vec, retptr)                                               \
  do {                                                                         \
    ACT_NULLCHECK(vec);                                                        \
                                                                               \
    const act_vector_header_t *header = act__vector_get_header(vec);           \
    size_t len = act_vector_len(vec);                                          \
    ACT_NULLCHECK(header);                                                     \
                                                                               \
    act__vector_decr_len(vec);                                                 \
    *(retptr) = (vec)[len - 1];                                                \
  } while (0);

#define ACT_VEC(T) T *

// PRIVATE
// ============================================================================

const act_vector_header_t *act__vector_get_header(const act_vector_t *vec);

act_vector_header_t *act__vector_get_mut_header(act_vector_t *vec);

int act__vector_incr_len(act_vector_t *vec);

int act__vector_decr_len(act_vector_t *vec);

act_vector_t *act__vector_resize(act_vector_t *vec);

#endif /* !ACT_VECTOR_H */
