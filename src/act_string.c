#include "act_string.h"

/// A default/NULL @em #act_string_t to be returned as a failure.
static const act_string_t ACT_STRING_NULLSTR = {0};

/// The default capacity to add when resizing a @em #act_string_t.
static const size_t STRING_RESIZE_CAP = 8;

act_string_t act_string_new(const act_allocator_t *allocator) {
  ACT_ASSERT(allocator != NULL, ACT_STRING_NULLSTR);

  return (act_string_t){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = 0,
      ._data = NULL,
  };
}

act_string_t act_string_with_capacity(const act_allocator_t *allocator,
                                      size_t capacity) {
  ACT_ASSERT(allocator != NULL, ACT_STRING_NULLSTR);

  return (act_string_t){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = capacity,
      ._data = NULL,
  };
}

act_string_t act_string_from_cstr(const act_allocator_t *allocator,
                                  const char *cstr) {
  ACT_ASSERT(allocator != NULL, ACT_STRING_NULLSTR);
  ACT_ASSERT(cstr != NULL, ACT_STRING_NULLSTR);

  size_t len = strlen(cstr);

  char *data = (*allocator->alloc)(len + 1, sizeof(char));
  memcpy(data, cstr, len);
  data[len] = '\0';

  return (act_string_t){
      ._allocator = allocator,
      ._len = len,
      ._capacity = len + 1,
      ._data = data,
  };
}

act_string_error_t act_string_free(act_string_t *string) {
  ACT_ASSERT(string != NULL, ACT_STRING_ERROR_NULL_STRING);

  (*string->_allocator->free)(string->_data);

  *string = ACT_STRING_NULLSTR;

  return ACT_STRING_ERROR_SUCCESS;
}

size_t act_string_len(act_string_t string) { return string._len; }

size_t act_string_capacity(act_string_t string) { return string._capacity; }

const char *act_string_as_cstr(act_string_t string) { return string._data; }

act_string_t act__string_resize(act_string_t string) {
  size_t new_cap = string._capacity + STRING_RESIZE_CAP;

  char *tmp = (*string._allocator->resize)(string._data, new_cap, sizeof(char));
  ACT_ASSERT(tmp != NULL, ACT_STRING_NULLSTR);
  string._data = tmp;
  string._capacity = new_cap;

  return string;
}

act_string_error_t act_string_push_char(act_string_t *string, char c) {
  ACT_ASSERT(string != NULL, ACT_STRING_ERROR_NULL_STRING);

  // Allocate on first push
  if (string->_len == 0) {
    if (string->_capacity == 0) {
      string->_capacity = 2;
    }
    char *data = (*string->_allocator->alloc)(string->_capacity, sizeof(char));

    string->_data = data;
  }

  size_t len = string->_len + 1;
  volatile size_t capacity = string->_capacity;

  // Resize if necessary
  while (len > capacity - 1) {
    act_string_t tmp = act__string_resize(*string);
    ACT_ASSERT(tmp._data != NULL, ACT_STRING_ERROR_RESIZE_FAILED);

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

  return ACT_STRING_ERROR_SUCCESS;
}

act_string_error_t act_string_push_cstr(act_string_t *string,
                                        const char *cstr) {
  ACT_ASSERT(string != NULL, ACT_STRING_ERROR_NULL_STRING);

  size_t cstr_len = strlen(cstr);

  // Allocate on first push
  if (string->_len == 0) {
    char *data = (*string->_allocator->alloc)(cstr_len + 1, sizeof(char));
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
    act_string_t tmp = act__string_resize(*string);
    ACT_ASSERT(tmp._data != NULL, ACT_STRING_ERROR_RESIZE_FAILED);

    string->_data = tmp._data;
    string->_capacity = tmp._capacity - 1;
    string->_len = len - 1;

    capacity = tmp._capacity;
  }

  // Push string to end
  string->_data = strcat(string->_data, cstr);
  string->_len += cstr_len;

  return ACT_STRING_ERROR_SUCCESS;
}

char act_string_pop_char(act_string_t *string) {
  const char null_term = '\0';
  ACT_ASSERT(string != NULL, null_term);
  ACT_ASSERT(string->_len != 0, null_term);
  ACT_ASSERT(string->_data != NULL, null_term);

  size_t len = string->_len;

  char retc = string->_data[len - 1];

  // Null terminate and decrement length
  string->_data[string->_len--] = null_term;

  return retc;
}

ssize_t act_string_find_first_idx_of_char(act_string_t string, char find_char) {
  ACT_ASSERT(string._len != 0, -1);
  ACT_ASSERT(string._data != NULL, -1);

  for (ssize_t i = 0; i < (ssize_t)act_string_len(string); i++) {
    if (string._data[i] == find_char) {
      return i;
    }
  }

  return -1;
}

act_string_t *act_string_split_at_idx(act_string_t string, size_t idx) {
  ACT_ASSERT(string._len != 0, NULL);
  ACT_ASSERT(string._data != 0, NULL);
  ACT_ASSERT(idx < string._len, NULL);

  act_string_t str1 = act_string_with_capacity(string._allocator, idx);
  act_string_t str2 =
      act_string_with_capacity(string._allocator, string._len - idx);

  // TODO: error check push!
  for (size_t i = 0; i < idx; i++) {
    act_string_push_char(&str1, string._data[i]);
  }
  act_string_push_char(&str1, '\0');
  for (size_t i = idx; i < string._len; i++) {
    act_string_push_char(&str2, string._data[i]);
  }
  act_string_push_char(&str2, '\0');

  act_string_t *splits = (*string._allocator->alloc)(2, sizeof(*splits));
  splits[0] = str1;
  splits[1] = str2;
  return splits;
}

act_string_comparison_t act_string_compare(act_string_t str1,
                                           act_string_t str2) {
  ACT_ASSERT(str1._data != NULL, ACT_STRING_COMPARISON_INVALID);
  ACT_ASSERT(str2._data != NULL, ACT_STRING_COMPARISON_INVALID);

  if (str1._len > str2._len) {
    return ACT_STRING_COMPARISON_GREATER_THAN;
  }
  if (str1._len < str2._len) {
    return ACT_STRING_COMPARISON_LESS_THAN;
  }

  for (size_t i = 0; i < str1._len; i++) {
    if (str1._data[i] != str2._data[i]) {
      return ACT_STRING_COMPARISON_UNEQUAL;
    }
  }

  return ACT_STRING_COMPARISON_EQUAL;
}

act_string_t act_string_copy(const act_string_t *string) {
  ACT_ASSERT(string != NULL, ACT_STRING_NULLSTR);
  ACT_ASSERT(string->_allocator != NULL, ACT_STRING_NULLSTR);

  act_string_t str_copy = {0};

  str_copy._allocator = string->_allocator;
  str_copy._len = string->_len;
  str_copy._capacity = string->_capacity;

  // Copy over data from original string
  const char *data_orig = act_string_as_cstr(*string);
  const char *data_copy =
      (*str_copy._allocator->alloc)(str_copy._capacity, sizeof(*data_copy));
  char *tmp =
      memmove((void *)data_copy, data_orig, str_copy._len + sizeof(*tmp));
  ACT_ASSERT(tmp != NULL, ACT_STRING_NULLSTR);
  str_copy._data = tmp;
  str_copy._data[str_copy._len] = '\0';

  return str_copy;
}

act_string_t act_string_concat(const act_string_t *str1,
                               const act_string_t *str2) {
  ACT_ASSERT(str1 != NULL, ACT_STRING_NULLSTR);
  ACT_ASSERT(str2 != NULL, ACT_STRING_NULLSTR);
  ACT_ASSERT(str1->_allocator != NULL, ACT_STRING_NULLSTR);

  act_string_t concat = {0};

  size_t combined_len = str1->_len + str2->_len;

  concat._allocator = str1->_allocator;
  concat._len = combined_len;
  concat._capacity = combined_len + 1;

  // Allocate enough space for both strings
  char *cat_str =
      (*concat._allocator->alloc)(combined_len + 1, sizeof(*cat_str));

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
