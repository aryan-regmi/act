#include "act_allocator.h"
#include "act_string.h"
#include "act_utils.h"
#include "acutest.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_can_create_new_string(void) {
  act_string_t str = act_string_new(&GPA);

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(act_string_capacity(str) == 0);
  TEST_CHECK(act_string_as_cstr(str) == NULL);

  act_string_free(&str);
}

void test_can_create_new_string_with_capacity(void) {
  const size_t CAP = 10;
  act_string_t str = act_string_with_capacity(&GPA, CAP);

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(act_string_capacity(str) == 10);
  TEST_CHECK(act_string_as_cstr(str) == NULL);

  act_string_free(&str);
}

void test_can_create_string_from_cstr(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello World!");

  TEST_CHECK(act_string_len(str) == 12);
  TEST_CHECK(act_string_capacity(str) == 13);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!") == 0);

  act_string_free(&str);
}

void test_can_push_char_to_string(void) {
  act_string_t str = act_string_new(&GPA);

  act_string_error_t err_code = -1;
  err_code = act_string_push_char(&str, 'H');
  err_code = act_string_push_char(&str, 'e');
  err_code = act_string_push_char(&str, 'l');
  err_code = act_string_push_char(&str, 'l');
  err_code = act_string_push_char(&str, 'o');
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "\n[ERROR] Push failed\n");
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_string_len(str) == 5);
  TEST_CHECK(act_string_capacity(str) == 10);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello") == 0);

  act_string_free(&str);
}

void test_can_push_cstr_to_string(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello");

  act_string_error_t err_code = act_string_push_cstr(&str, " World!");
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "[ERROR] Pushing C-string failed");
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_string_len(str) == 12);
  TEST_CHECK(act_string_capacity(str) == 13);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!") == 0);

  act_string_free(&str);
}

void test_can_pop_char_from_string(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello");

  size_t len = act_string_len(str);

  char *popped_str = (*GPA.alloc)(len + 1, sizeof(*popped_str));

  for (size_t i = 0; i < len; i++) {
    popped_str[i] = act_string_pop_char(&str);
    if (popped_str[i] == '\0') {
      printf("\nRETUNRED NULL\n");
    }
    TEST_CHECK(act_string_len(str) == len - (i + 1));
  }

  TEST_CHECK(act_string_len(str) == 0);
  TEST_CHECK(strcmp(popped_str, "olleH") == 0);

  (*GPA.free)(popped_str);
  act_string_free(&str);
}

void test_can_find_char_index_in_string(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello");

  TEST_CHECK(act_string_find_first_idx_of_char(str, 'H') == 0);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'e') == 1);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'l') == 2);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'l') == 2);
  TEST_CHECK(act_string_find_first_idx_of_char(str, 'o') == 4);
  TEST_CHECK(act_string_find_first_idx_of_char(str, '!') == -1);

  act_string_free(&str);
}

void test_can_split_string_at_idx(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello World!");

  size_t idx = act_string_find_first_idx_of_char(str, ' ');

  act_string_t *splits = act_string_split_at_idx(str, idx);
  act_string_t split1 = splits[0];
  act_string_t split2 = splits[1];
  (*str._allocator->free)(splits);
  act_string_free(&str);

  TEST_CHECK(strcmp(act_string_as_cstr(split1), "Hello") == 0);
  TEST_CHECK(strcmp(act_string_as_cstr(split2), " World!") == 0);

  act_string_free(&split1);
  act_string_free(&split2);
}

void test_can_compare_strings(void) {
  act_string_t str1 = act_string_from_cstr(&GPA, "Hello");
  act_string_t str2 = act_string_from_cstr(&GPA, "Hello");
  act_string_t str3 = act_string_from_cstr(&GPA, "World!");
  act_string_t str4 = act_string_from_cstr(&GPA, "World");

  TEST_CHECK(act_string_compare(str1, str2) == ACT_STRING_COMPARISON_EQUAL);
  TEST_CHECK(act_string_compare(str1, str3) == ACT_STRING_COMPARISON_LESS_THAN);
  TEST_CHECK(act_string_compare(str3, str1) ==
             ACT_STRING_COMPARISON_GREATER_THAN);
  TEST_CHECK(act_string_compare(str1, str4) == ACT_STRING_COMPARISON_UNEQUAL);

  act_string_free(&str1);
  act_string_free(&str2);
  act_string_free(&str3);
  act_string_free(&str4);
}

void test_can_copy_string(void) {
  act_string_t str = act_string_from_cstr(&GPA, "Hello World!");
  act_string_t copy = act_string_copy(&str);

  act_string_error_t err = act_string_push_char(&str, '!');
  if (err != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(strcmp(act_string_as_cstr(str), "Hello World!!") == 0);
  TEST_CHECK(strcmp(act_string_as_cstr(copy), "Hello World!") == 0);
  TEST_CHECK(act_string_len(copy) == act_string_len(str) - 1);
  TEST_CHECK(act_string_capacity(copy) == act_string_len(copy) + 1);

  act_string_free(&str);
  act_string_free(&copy);
}

void test_can_concat_string(void) {
  act_string_t str1 = act_string_from_cstr(&GPA, "Hello");
  act_string_t str2 = act_string_from_cstr(&GPA, " World!");

  act_string_t catstr = act_string_concat(&str1, &str2);

  TEST_ASSERT(strcmp(act_string_as_cstr(catstr), "Hello World!") == 0);
  TEST_ASSERT(act_string_len(catstr) == 12);
  TEST_ASSERT(act_string_capacity(catstr) == 13);

  act_string_free(&str1);
  act_string_free(&str2);
  act_string_free(&catstr);
}

TEST_LIST = {
    {"[STRING] Can create new act_string_t", test_can_create_new_string},
    {"[STRING] Can create act_string_t with capacity",
     test_can_create_new_string_with_capacity},
    {"[STRING] Can create act_string_t from C-string",
     test_can_create_string_from_cstr},
    {"[STRING] Can push char to act_string_t", test_can_push_char_to_string},
    {"[STRING] Can push C-string to act_string_t",
     test_can_push_cstr_to_string},
    {"[STRING] Can pop char from act_string_t", test_can_pop_char_from_string},
    {"[STRING] Can find index of char in act_string_t",
     test_can_find_char_index_in_string},
    {"[STRING] Can split act_string_t at index", test_can_split_string_at_idx},
    {"[STRING] Can compare two act_string_t", test_can_compare_strings},
    {"[STRING] Can copy act_string_t", test_can_copy_string},
    {"[STRING] Can concat act_string_t", test_can_concat_string},
    {NULL, NULL}};
