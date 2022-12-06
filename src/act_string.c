#include "act_string.h"

/// The default capacity to add when resizing a @em #act_string_t.
static const size_t STRING_RESIZE_CAP = 8;

act_string_t act_string_new(const act_allocator_t *allocator,
                            act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  return (act_string_t){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = 0,
      ._data = NULL,
  };
}

act_string_t act_string_with_capacity(const act_allocator_t *allocator,
                                      size_t capacity,
                                      act_string_error_t *error_code) {

  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  return (act_string_t){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = capacity,
      ._data = NULL,
  };
}

act_string_t act_string_from_cstr(const act_allocator_t *allocator,
                                  const char *cstr,
                                  act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);
  ACT_ASSERT_OR(cstr != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);

  size_t len = strlen(cstr);

  char *data = (*allocator->alloc)(len + 1, sizeof(char));
  ACT_ASSERT_OR(data != NULL, *error_code = ACT_STRING_ERROR_ALLOCATION_FAILED);
  memcpy(data, cstr, len);
  ACT_ASSERT_OR(data != NULL, *error_code = ACT_STRING_ERROR_MEMCPY_FAILED);
  data[len] = '\0';

  return (act_string_t){
      ._allocator = allocator,
      ._len = len,
      ._capacity = len + 1,
      ._data = data,
  };
}

void act_string_free(act_string_t *string, act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);

  (*string->_allocator->free)(string->_data);

  string = NULL;
}

size_t act_string_len(act_string_t string) { return string._len; }

size_t act_string_capacity(act_string_t string) { return string._capacity; }

const char *act_string_as_cstr(act_string_t string) { return string._data; }

act_string_t act__string_resize(act_string_t string,
                                act_string_error_t *error_code) {
  size_t new_cap = string._capacity + STRING_RESIZE_CAP;

  char *tmp = (*string._allocator->resize)(string._data, new_cap, sizeof(char));
  ACT_ASSERT_OR(tmp != NULL, *error_code = ACT_STRING_ERROR_RESIZE_FAILED);
  string._data = tmp;
  string._capacity = new_cap;

  *error_code = ACT_STRING_ERROR_SUCCESS;

  return string;
}

void act_string_push_char(act_string_t *string, char c,
                          act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);

  // Allocate on first push
  if (string->_len == 0) {
    if (string->_capacity == 0) {
      string->_capacity = 2;
    }
    char *data = (*string->_allocator->alloc)(string->_capacity, sizeof(char));
    ACT_ASSERT_OR(data != NULL,
                  *error_code = ACT_STRING_ERROR_ALLOCATION_FAILED);

    string->_data = data;
  }

  size_t len = string->_len + 1;
  volatile size_t capacity = string->_capacity;

  // Resize if necessary
  while (len > capacity - 1) {
    act_string_t tmp = act__string_resize(*string, error_code);
    ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS,
                  *error_code = ACT_STRING_ERROR_RESIZE_FAILED);

    string->_data = tmp._data;
    string->_capacity = tmp._capacity;
    string->_len = len - 1;

    capacity = tmp._capacity;
  }

  // Push char to end
  if (c == '\0') {
    string->_data[string->_len] = c;
  } else {
    string->_data[string->_len] = c;
    string->_data[++string->_len] = '\0'; // Null-terminate string manually
  }
}

void act_string_push_cstr(act_string_t *string, const char *cstr,
                          act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);

  size_t cstr_len = strlen(cstr);

  // Allocate on first push
  if (string->_len == 0) {
    char *data = (*string->_allocator->alloc)(cstr_len + 1, sizeof(char));
    ACT_ASSERT_OR(data != NULL,
                  *error_code = ACT_STRING_ERROR_ALLOCATION_FAILED);
    data[cstr_len] = '\0';

    if (string->_capacity == 0) {
      string->_capacity = cstr_len + 1;
    }
    string->_data = data;
  }

  size_t len = string->_len + 1;
  volatile size_t capacity = string->_capacity;

  // Resize if necessary
  while (len + cstr_len > capacity - 1) {
    act_string_t tmp = act__string_resize(*string, error_code);
    ACT_ASSERT_OR(error_code == ACT_STRING_ERROR_SUCCESS,
                  *error_code = ACT_STRING_ERROR_RESIZE_FAILED);

    string->_data = tmp._data;
    string->_capacity = tmp._capacity - 1;
    string->_len = len - 1;

    capacity = tmp._capacity;
  }

  // Push string to end
  string->_data = strcat(string->_data, cstr);
  string->_len += cstr_len;
}

char act_string_pop_char(act_string_t *string, act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  const char null_term = '\0';
  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(string->_len != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(string->_data != NULL,
                *error_code = ACT_STRING_ERROR_EMPTY_STRING);

  size_t len = string->_len;

  char retc = string->_data[len - 1];

  // Null terminate and decrement length
  string->_data[string->_len--] = null_term;

  return retc;
}

size_t act_string_find_first_idx_of_char(act_string_t string, char find_char,
                                         act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string._len != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(string._data != NULL,
                *error_code = ACT_STRING_ERROR_EMPTY_STRING);

  for (size_t i = 0; i < act_string_len(string); i++) {
    if (string._data[i] == find_char) {
      return i;
    }
  }

  *error_code = ACT_STRING_ERROR_CHAR_NOT_IN_STRING;
  return 0;
}

act_string_t *act_string_split_at_idx(act_string_t string, size_t idx,
                                      act_string_t splits[2],
                                      act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string._len != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(string._data != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(idx < string._len,
                *error_code = ACT_STRING_ERROR_INDEX_OUT_OF_BOUNDS);

  act_string_t str1 =
      act_string_with_capacity(string._allocator, idx, error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS, NULL);

  act_string_t str2 = act_string_with_capacity(string._allocator,
                                               string._len - idx, error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS, NULL);

  for (size_t i = 0; i < idx; i++) {
    act_string_push_char(&str1, string._data[i], error_code);
  }
  act_string_push_char(&str1, '\0', error_code);
  for (size_t i = idx; i < string._len; i++) {
    act_string_push_char(&str2, string._data[i], error_code);
  }
  act_string_push_char(&str2, '\0', error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS,
                NULL); // Checks if all pushes were successful

  // act_string_t *splits = (*string._allocator->alloc)(2, sizeof(*splits));
  splits[0] = str1;
  splits[1] = str2;

  return splits;
}

act_string_comparison_t act_string_compare(act_string_t str1, act_string_t str2,
                                           act_string_error_t *error_code) {
  ACT_ASSERT_OR(str1._data != NULL,
                *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(str2._data != NULL,
                *error_code = ACT_STRING_ERROR_EMPTY_STRING);

  if (str1._len > str2._len) {
    return ACT_STRING_COMPARISON_GREATER_THAN;
  }
  if (str1._len < str2._len) {
    return ACT_STRING_COMPARISON_LESS_THAN;
  }

  for (size_t i = 0; i < str1._len; i++) {
    if (str1._data[i] != str2._data[i]) {
      return ACT_STRING_COMPARISON_NOT_EQUAL;
    }
  }

  return ACT_STRING_COMPARISON_EQUAL;
}

act_string_t act_string_copy(const act_string_t *string,
                             act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(string->_allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  act_string_t str_copy = {0};

  str_copy._allocator = string->_allocator;
  str_copy._len = string->_len;
  str_copy._capacity = string->_capacity;

  // Copy over data from original string
  const char *data_orig = act_string_as_cstr(*string);
  const char *data_copy =
      (*str_copy._allocator->alloc)(str_copy._capacity, sizeof(*data_copy));
  ACT_ASSERT_OR(data_copy != NULL,
                *error_code = ACT_STRING_ERROR_ALLOCATION_FAILED);

  char *tmp =
      memmove((void *)data_copy, data_orig, str_copy._len + sizeof(*tmp));
  ACT_ASSERT_OR(tmp != NULL, *error_code = ACT_STRING_ERROR_MEMMOVE_FAILED);

  str_copy._data = tmp;
  str_copy._data[str_copy._len] = '\0';

  return str_copy;
}

act_string_t act_string_concat(const act_string_t *str1,
                               const act_string_t *str2,
                               act_string_error_t *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(str1 != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(str2 != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(str1->_allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  act_string_t concat = {0};

  size_t combined_len = str1->_len + str2->_len;

  concat._allocator = str1->_allocator;
  concat._len = combined_len;
  concat._capacity = combined_len + 1;

  // Allocate enough space for both strings
  char *cat_str =
      (*concat._allocator->alloc)(combined_len + 1, sizeof(*cat_str));
  ACT_ASSERT_OR(cat_str != NULL,
                *error_code = ACT_STRING_ERROR_ALLOCATION_FAILED);

  // Loop over both strings and combine them into `cat_str`
  for (size_t i = 0; i < str1->_len; i++) {
    cat_str[i] = str1->_data[i];
  }
  for (size_t i = 0; i < str2->_len; i++) {
    cat_str[i + str1->_len] = str2->_data[i];
  }
  cat_str[combined_len] = '\0'; // Null-terminate concatenation

  concat._data = cat_str;

  return concat;
}
