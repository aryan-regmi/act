#include "act_string.h"
#include <stdio.h>

/// The default capacity to add when resizing a @em #act_String.
static const size_t STRING_RESIZE_CAP = 8;

act_String act_stringNew(const act_Allocator *allocator, int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  return (act_String){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = 0,
      ._data = NULL,
  };
}

act_String act_stringWithCapacity(const act_Allocator *allocator,
                                  size_t capacity, int *error_code) {

  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  return (act_String){
      ._allocator = allocator,
      ._len = 0,
      ._capacity = capacity,
      ._data = NULL,
  };
}

act_String act_stringFromCstr(const act_Allocator *allocator, const char *cstr,
                              int *error_code) {
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

  return (act_String){
      ._allocator = allocator,
      ._len = len,
      ._capacity = len + 1,
      ._data = data,
  };
}

void act_stringFree(act_String *string, int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);

  (*string->_allocator->free)(string->_data);

  string = NULL;
}

size_t act_stringLen(act_String string) { return string._len; }

size_t act_stringCapacity(act_String string) { return string._capacity; }

const char *act_stringAsCstr(act_String string) { return string._data; }

act_String *act__stringResize(act_String *string, int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  size_t new_cap = string->_capacity + STRING_RESIZE_CAP;

  char *tmp =
      (*string->_allocator->resize)(string->_data, new_cap, sizeof(char));
  if (*error_code != ACT_STRING_ERROR_SUCCESS) {
    return string;
  }

  string->_data = tmp;
  string->_capacity = new_cap;

  return string;
}

act_String act_stringShrinkToFit(act_String string, int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  // Only need to shrink if capacity is larger than length (plus null
  // terminator)
  if (string._capacity > string._len + 1) {
    string._capacity = string._len + 1;

    act_String *tmp = act__stringResize(&string, error_code);
    // Dont shrink if resize fails
    if (*error_code != ACT_STRING_ERROR_SUCCESS) {
      return string;
    }

    act_stringFree(&string, error_code);

    string = *tmp;
    string._capacity = string._len + 1;
  }

  return string;
}

void act_stringPushChar(act_String *string, char c, int *error_code) {
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
  while (len + 1 > capacity) {
    act_String *tmp = act__stringResize(string, error_code);
    ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS,
                  *error_code = ACT_STRING_ERROR_RESIZE_FAILED);

    string->_data = tmp->_data;
    string->_capacity = tmp->_capacity;
    string->_len = len - 1;

    capacity = tmp->_capacity;
  }

  // Push char to end
  if (c == '\0') {
    string->_data[string->_len] = c;
  } else {
    string->_data[string->_len] = c;
    string->_data[++string->_len] = '\0'; // Null-terminate string manually
  }
}

void act_stringPushCstr(act_String *string, const char *cstr, int *error_code) {
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
  while (len + cstr_len + 1 > capacity) {
    act_String *tmp = act__stringResize(string, error_code);
    ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS,
                  *error_code = ACT_STRING_ERROR_RESIZE_FAILED);

    string->_data = tmp->_data;
    string->_capacity = tmp->_capacity;
    string->_len = len - 1;

    capacity = tmp->_capacity;
  }

  // Push string to end
  string->_data = strcat(string->_data, cstr);
  string->_len += cstr_len;
  string->_capacity--;
}

char act_stringPopChar(act_String *string, int *error_code) {
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

size_t act_stringFindFirstIdxOfChar(act_String string, char find_char,
                                    int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string._len != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(string._data != NULL,
                *error_code = ACT_STRING_ERROR_EMPTY_STRING);

  for (size_t i = 0; i < act_stringLen(string); i++) {
    if (string._data[i] == find_char) {
      return i;
    }
  }

  *error_code = ACT_STRING_ERROR_CHAR_NOT_IN_STRING;
  return 0;
}

act_String *act_stringSplitAtIdx(act_String string, size_t idx,
                                 act_String splits[2], int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string._len != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(string._data != 0, *error_code = ACT_STRING_ERROR_EMPTY_STRING);
  ACT_ASSERT_OR(idx < string._len,
                *error_code = ACT_STRING_ERROR_INDEX_OUT_OF_BOUNDS);

  act_String str1 = act_stringWithCapacity(string._allocator, idx, error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS, NULL);

  act_String str2 =
      act_stringWithCapacity(string._allocator, string._len - idx, error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS, NULL);

  for (size_t i = 0; i < idx; i++) {
    act_stringPushChar(&str1, string._data[i], error_code);
  }
  act_stringPushChar(&str1, '\0', error_code);
  for (size_t i = idx; i < string._len; i++) {
    act_stringPushChar(&str2, string._data[i], error_code);
  }
  act_stringPushChar(&str2, '\0', error_code);
  ACT_ASSERT_OR(*error_code == ACT_STRING_ERROR_SUCCESS,
                NULL); // Checks if all pushes were successful

  // act_String *splits = (*string._allocator->alloc)(2, sizeof(*splits));
  splits[0] = str1;
  splits[1] = str2;

  return splits;
}

act_StringComparison act_stringCompare(act_String str1, act_String str2,
                                       int *error_code) {
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

act_String act_stringCopy(const act_String *string, int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(string != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(string->_allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  act_String str_copy = {0};

  str_copy._allocator = string->_allocator;
  str_copy._len = string->_len;
  str_copy._capacity = string->_capacity;

  // Copy over data from original string
  const char *data_orig = act_stringAsCstr(*string);
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

act_String act_stringConcat(const act_String *str1, const act_String *str2,
                            int *error_code) {
  *error_code = ACT_STRING_ERROR_SUCCESS;

  ACT_ASSERT_OR(str1 != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(str2 != NULL, *error_code = ACT_STRING_ERROR_NULL_STRING);
  ACT_ASSERT_OR(str1->_allocator != NULL,
                *error_code = ACT_STRING_ERROR_NULL_ALLOCATOR);

  act_String concat = {0};

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
