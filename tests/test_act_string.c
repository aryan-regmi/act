#include "act_allocator.h"
#include "act_string.h"
#include "act_utils.h"
#include "acutest.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_canCreateNewString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;
  act_String str = act_stringNew(&GPA, &err_code);

  TEST_CHECK(act_stringLen(str) == 0);
  TEST_CHECK(act_stringCapacity(str) == 0);
  TEST_CHECK(act_stringAsCstr(str) == NULL);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canCreateNewStringWithCapacity(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;
  const size_t CAP = 10;
  act_String str = act_stringWithCapacity(&GPA, CAP, &err_code);

  TEST_CHECK(act_stringLen(str) == 0);
  TEST_CHECK(act_stringCapacity(str) == 10);
  TEST_CHECK(act_stringAsCstr(str) == NULL);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canCreateStringFromCstr(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;
  act_String str = act_stringFromCstr(&GPA, "Hello World!", &err_code);

  TEST_CHECK(act_stringLen(str) == 12);
  TEST_CHECK(act_stringCapacity(str) == 13);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "Hello World!") == 0);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canShrinkStringToFit(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringWithCapacity(&GPA, 20, &err_code);

  TEST_CHECK(act_stringCapacity(str) == 20);

  act_stringPushChar(&str, 'H', &err_code);
  act_stringPushChar(&str, 'e', &err_code);
  act_stringPushChar(&str, 'l', &err_code);
  act_stringPushChar(&str, 'l', &err_code);
  act_stringPushChar(&str, 'o', &err_code);

  str = act_stringShrinkToFit(str, &err_code);

  TEST_CHECK(act_stringCapacity(str) == 6);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canPushCharToString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;
  act_String str = act_stringNew(&GPA, &err_code);

  act_stringPushChar(&str, 'H', &err_code);
  act_stringPushChar(&str, 'e', &err_code);
  act_stringPushChar(&str, 'l', &err_code);
  act_stringPushChar(&str, 'l', &err_code);
  act_stringPushChar(&str, 'o', &err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "\n[ERROR] Push failed: %u\n", err_code);
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_stringLen(str) == 5);
  TEST_CHECK(act_stringCapacity(str) == 10);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "Hello") == 0);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canPushCstrToString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringFromCstr(&GPA, "Hello", &err_code);

  act_stringPushCstr(&str, " World!", &err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    fprintf(stderr, "[ERROR] Pushing C-string failed: %u", err_code);
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(act_stringLen(str) == 12);
  TEST_CHECK(act_stringCapacity(str) == 13);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "Hello World!") == 0);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canPopCharFromString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringFromCstr(&GPA, "Hello", &err_code);

  size_t len = act_stringLen(str);

  char *popped_str = (*GPA.alloc)(len + 1, sizeof(*popped_str));

  for (size_t i = 0; i < len; i++) {
    popped_str[i] = act_stringPopChar(&str, &err_code);
    if (err_code != ACT_STRING_ERROR_SUCCESS) {
      printf("\nRETUNRED NULL\n");
    }
    TEST_CHECK(act_stringLen(str) == len - (i + 1));
  }

  TEST_CHECK(act_stringLen(str) == 0);
  TEST_CHECK(strcmp(popped_str, "olleH") == 0);

  (*GPA.free)(popped_str);
  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canFindCharIndexInString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringFromCstr(&GPA, "Hello", &err_code);

  TEST_CHECK(act_stringFindFirstIdxOfChar(str, 'H', &err_code) == 0);
  TEST_CHECK(act_stringFindFirstIdxOfChar(str, 'e', &err_code) == 1);
  TEST_CHECK(act_stringFindFirstIdxOfChar(str, 'l', &err_code) == 2);
  TEST_CHECK(act_stringFindFirstIdxOfChar(str, 'l', &err_code) == 2);
  TEST_CHECK(act_stringFindFirstIdxOfChar(str, 'o', &err_code) == 4);
  TEST_CHECK(act_stringFindFirstIdxOfChar(str, '!', &err_code) == 0);
  TEST_CHECK(err_code == ACT_STRING_ERROR_CHAR_NOT_IN_STRING);

  act_stringFree(&str, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canSplitStringAtIdx(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringFromCstr(&GPA, "Hello World!", &err_code);

  size_t idx = act_stringFindFirstIdxOfChar(str, ' ', &err_code);

  act_String splits[2];
  act_stringSplitAtIdx(str, idx, splits, &err_code);
  act_String split1 = splits[0];
  act_String split2 = splits[1];
  act_stringFree(&str, &err_code);

  TEST_CHECK(strcmp(act_stringAsCstr(split1), "Hello") == 0);
  TEST_CHECK(strcmp(act_stringAsCstr(split2), " World!") == 0);

  act_stringFree(&split1, &err_code);
  act_stringFree(&split2, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canCompareStrings(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str1 = act_stringFromCstr(&GPA, "Hello", &err_code);
  act_String str2 = act_stringFromCstr(&GPA, "Hello", &err_code);
  act_String str3 = act_stringFromCstr(&GPA, "World!", &err_code);
  act_String str4 = act_stringFromCstr(&GPA, "World", &err_code);

  TEST_CHECK(act_stringCompare(str1, str2, &err_code) ==
             ACT_STRING_COMPARISON_EQUAL);
  TEST_CHECK(act_stringCompare(str1, str3, &err_code) ==
             ACT_STRING_COMPARISON_LESS_THAN);
  TEST_CHECK(act_stringCompare(str3, str1, &err_code) ==
             ACT_STRING_COMPARISON_GREATER_THAN);
  TEST_CHECK(act_stringCompare(str1, str4, &err_code) ==
             ACT_STRING_COMPARISON_NOT_EQUAL);

  act_stringFree(&str1, &err_code);
  act_stringFree(&str2, &err_code);
  act_stringFree(&str3, &err_code);
  act_stringFree(&str4, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canCopyString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str = act_stringFromCstr(&GPA, "Hello World!", &err_code);
  act_String copy = act_stringCopy(&str, &err_code);

  act_stringPushChar(&str, '!', &err_code);
  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  TEST_CHECK(strcmp(act_stringAsCstr(str), "Hello World!!") == 0);
  TEST_CHECK(strcmp(act_stringAsCstr(copy), "Hello World!") == 0);
  TEST_CHECK(act_stringLen(copy) == act_stringLen(str) - 1);
  TEST_CHECK(act_stringCapacity(copy) == act_stringLen(copy) + 1);

  act_stringFree(&str, &err_code);
  act_stringFree(&copy, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canConcatString(void) {
  int err_code = ACT_STRING_ERROR_SUCCESS;

  act_String str1 = act_stringFromCstr(&GPA, "Hello", &err_code);
  act_String str2 = act_stringFromCstr(&GPA, " World!", &err_code);

  act_String catstr = act_stringConcat(&str1, &str2, &err_code);

  TEST_ASSERT(strcmp(act_stringAsCstr(catstr), "Hello World!") == 0);
  TEST_ASSERT(act_stringLen(catstr) == 12);
  TEST_ASSERT(act_stringCapacity(catstr) == 13);

  act_stringFree(&str1, &err_code);
  act_stringFree(&str2, &err_code);
  act_stringFree(&catstr, &err_code);

  if (err_code != ACT_STRING_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

TEST_LIST = {
    {"[STRING] Can create new act_String", test_canCreateNewString},
    {"[STRING] Can create act_String with capacity",
     test_canCreateNewStringWithCapacity},
    {"[STRING] Can create act_String from C-string",
     test_canCreateStringFromCstr},
    {"[STRING] Can push char to act_String", test_canPushCharToString},
    {"[STRING] Can push C-string to act_String", test_canPushCstrToString},
    {"[STRING] Can pop char from act_String", test_canPopCharFromString},
    {"[STRING] Can find index of char in act_String",
     test_canFindCharIndexInString},
    {"[STRING] Can split act_String at index", test_canSplitStringAtIdx},
    {"[STRING] Can compare two act_String", test_canCompareStrings},
    {"[STRING] Can copy act_String", test_canCopyString},
    {"[STRING] Can concat act_String", test_canConcatString},
    {"[STRING] Can shrink act_String to fit length", test_canShrinkStringToFit},
    {NULL, NULL}};
