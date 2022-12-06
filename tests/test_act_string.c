#include "act_allocator.h"
#include "act_string.h"
#include "act_utils.h"
#include "acutest.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_can_create_new_string(void) {
  act_string_error_t *err_code = NULL;
  act_string_t str = act_string_new(&GPA, err_code);

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(act_string_capacity(str) == 0);
  TEST_CHECK(act_string_as_cstr(str) == NULL);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_create_new_string_with_capacity(void) {
  act_string_error_t *err_code = NULL;
  const size_t CAP = 10;
  act_string_t str = act_string_with_capacity(&GPA, CAP, err_code);

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(act_string_capacity(str) == 10);
  TEST_CHECK(act_string_as_cstr(str) == NULL);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_create_string_from_cstr(void) {
  act_string_error_t *err_code = NULL;
  act_string_t str = act_string_from_cstr(&GPA, "Hello World!", err_code);

  TEST_CHECK(act_string_len(str) == 12);
  TEST_CHECK(act_string_capacity(str) == 13);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!") == 0);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_push_char_to_string(void) {
  act_string_error_t *err_code = NULL;
  act_string_t str = act_string_new(&GPA, err_code);

  act_string_push_char(&str, 'H', err_code);
  act_string_push_char(&str, 'e', err_code);
  act_string_push_char(&str, 'l', err_code);
  act_string_push_char(&str, 'l', err_code);
  act_string_push_char(&str, 'o', err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "\n[ERROR] Push failed: %u\n", *err_code);
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_string_len(str) == 5);
  TEST_CHECK(act_string_capacity(str) == 10);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello") == 0);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_push_cstr_to_string(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str = act_string_from_cstr(&GPA, "Hello", err_code);

  act_string_push_cstr(&str, " World!", err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "[ERROR] Pushing C-string failed: %u", *err_code);
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_string_len(str) == 12);
  TEST_CHECK(act_string_capacity(str) == 13);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!") == 0);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_pop_char_from_string(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str = act_string_from_cstr(&GPA, "Hello", err_code);

  size_t len = act_string_len(str);

  char *popped_str = (*GPA.alloc)(len + 1, sizeof(*popped_str));

  for (size_t i = 0; i < len; i++) {
    popped_str[i] = act_string_pop_char(&str, err_code);
    if (*err_code != ACT_STRING_ERROR_SUCCESS) {
      printf("\nRETUNRED NULL\n");
    }
    TEST_CHECK(act_string_len(str) == len - (i + 1));
  }

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(strcmp(popped_str, "olleH") == 0);

  (*GPA.free)(popped_str);
  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_find_char_index_in_string(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str = act_string_from_cstr(&GPA, "Hello", err_code);

  TEST_CHECK(act_string_find_first_idx_of_char(str, 'H', err_code) == 0);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'e', err_code) == 1);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'l', err_code) == 2);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'l', err_code) == 2);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'o', err_code) == 4);
  TEST_CHECK(act_string_find_first_idx_of_char(str, '!', err_code) == 0);
  TEST_CHECK(*err_code == ACT_STRING_ERROR_CHAR_NOT_IN_STRING);

  act_string_free(&str, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_split_string_at_idx(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str = act_string_from_cstr(&GPA, "Hello World!", err_code);

  size_t idx = act_string_find_first_idx_of_char(str, ' ', err_code);

  act_string_t *splits = NULL;
  splits = act_string_split_at_idx(str, idx, splits, err_code);
  act_string_t split1 = splits[0];
  act_string_t split2 = splits[1];
  act_string_free(&str, err_code);

  TEST_CHECK(strcmp(act_string_as_cstr(split1), "Hello") == 0);
  TEST_CHECK(strcmp(act_string_as_cstr(split2), " World!") == 0);

  act_string_free(&split1, err_code);
  act_string_free(&split2, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_compare_strings(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str1 = act_string_from_cstr(&GPA, "Hello", err_code);
  act_string_t str2 = act_string_from_cstr(&GPA, "Hello", err_code);
  act_string_t str3 = act_string_from_cstr(&GPA, "World!", err_code);
  act_string_t str4 = act_string_from_cstr(&GPA, "World", err_code);

  TEST_CHECK(act_string_compare(str1, str2, err_code) ==
             ACT_STRING_COMPARISON_EQUAL);
  TEST_CHECK(act_string_compare(str1, str3, err_code) ==
             ACT_STRING_COMPARISON_LESS_THAN);
  TEST_CHECK(act_string_compare(str3, str1, err_code) ==
             ACT_STRING_COMPARISON_GREATER_THAN);
  TEST_CHECK(act_string_compare(str1, str4, err_code) ==
             ACT_STRING_COMPARISON_NOT_EQUAL);

  act_string_free(&str1, err_code);
  act_string_free(&str2, err_code);
  act_string_free(&str3, err_code);
  act_string_free(&str4, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_copy_string(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str = act_string_from_cstr(&GPA, "Hello World!", err_code);
  act_string_t copy = act_string_copy(&str, err_code);

  act_string_push_char(&str, '!', err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!!") == 0);
  TEST_CHECK(strcmp(act_string_as_cstr(copy), "Hello World!") == 0);
  TEST_CHECK(act_string_len(copy) == act_string_len(str) - 1);
  TEST_CHECK(act_string_capacity(copy) == act_string_len(copy) + 1);

  act_string_free(&str, err_code);
  act_string_free(&copy, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_concat_string(void) {
  act_string_error_t *err_code = NULL;

  act_string_t str1 = act_string_from_cstr(&GPA, "Hello", err_code);
  act_string_t str2 = act_string_from_cstr(&GPA, " World!", err_code);

  act_string_t catstr = act_string_concat(&str1, &str2, err_code);

  TEST_ASSERT(strcmp(act_string_as_cstr(catstr), "Hello World!") == 0);
  TEST_ASSERT(act_string_len(catstr) == 12);
  TEST_ASSERT(act_string_capacity(catstr) == 13);

  act_string_free(&str1, err_code);
  act_string_free(&str2, err_code);
  act_string_free(&catstr, err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

TEST_LIST = {
    // {"[STRING] Can create new act_string_t", test_can_create_new_string},
    // {"[STRING] Can create act_string_t with capacity",
    //  test_can_create_new_string_with_capacity},
    // {"[STRING] Can create act_string_t from C-string",
    //  test_can_create_string_from_cstr},
    // {"[STRING] Can push char to act_string_t", test_can_push_char_to_string},
    // {"[STRING] Can push C-string to act_string_t",
    //  test_can_push_cstr_to_string},
    // {"[STRING] Can pop char from act_string_t",
    // test_can_pop_char_from_string},
    // {"[STRING] Can find index of char in act_string_t",
    //  test_can_find_char_index_in_string},
    // {"[STRING] Can split act_string_t at index",
    // test_can_split_string_at_idx},
    // {"[STRING] Can compare two act_string_t", test_can_compare_strings},
    // {"[STRING] Can copy act_string_t", test_can_copy_string},
    // {"[STRING] Can concat act_string_t", test_can_concat_string},
    {NULL, NULL}};
