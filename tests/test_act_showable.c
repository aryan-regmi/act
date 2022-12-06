#include "act_allocator.h"
#include "act_showable.h"
#include "acutest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_can_show_uint(void) {
  act_showable_error_t err = ACT_SHOWABLE_ERROR_SUCCESS;
  act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;

  uint8_t v1 = 10;
  act_string_t str = act_showable_uint64_as_string(&GPA, v1, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "10") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint16_t v2 = 20;
  str = act_showable_uint64_as_string(&GPA, v2, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "20") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint32_t v3 = 30;
  str = act_showable_uint64_as_string(&GPA, v3, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "30") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint64_t v4 = 40;
  str = act_showable_uint64_as_string(&GPA, v4, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "40") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint v5 = 50;
  str = act_showable_uint64_as_string(&GPA, v5, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "50") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_show_int(void) {
  act_showable_error_t err = ACT_SHOWABLE_ERROR_SUCCESS;
  act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;

  int8_t v1 = 10;
  act_string_t str = act_showable_int64_as_string(&GPA, v1, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "10") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int16_t v2 = 20;
  str = act_showable_int64_as_string(&GPA, v2, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "20") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int32_t v3 = 30;
  str = act_showable_int64_as_string(&GPA, v3, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "30") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int64_t v4 = 40;
  str = act_showable_int64_as_string(&GPA, v4, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "40") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int v5 = 50;
  str = act_showable_int64_as_string(&GPA, v5, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "50") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_show_float(void) {
  act_showable_error_t err = ACT_SHOWABLE_ERROR_SUCCESS;
  act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;

  float v1 = 10.11F;
  act_string_t str = act_showable_double_as_string(&GPA, v1, 2, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "10.11") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  double v2 = 20.22;
  str = act_showable_double_as_string(&GPA, v2, 2, &err);
  TEST_CHECK(strcmp(act_string_as_cstr(str), "20.22") == 0);
  act_string_free(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_show_cstring(void) {
  act_showable_error_t err = ACT_SHOWABLE_ERROR_SUCCESS;
  act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;

  const char *v1 = "Hello World!";

  act_string_t str = act_showable_cstr_as_string(&GPA, v1, &err);

  TEST_CHECK(strcmp(act_string_as_cstr(str), "\"Hello World!\"") == 0);

  act_string_free(&str, &str_err);

  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

typedef struct {
  int val1;
  double val2;
  const char *val3;
} Tst;

act_string_t Tst_show(act_showable_t *tst) {
  act_showable_error_t err = ACT_SHOWABLE_ERROR_SUCCESS;

  Tst *tst_struct = tst;

  act_string_t val1 =
      act_showable_int64_as_string(&GPA, tst_struct->val1, &err);
  act_string_t val2 =
      act_showable_double_as_string(&GPA, tst_struct->val2, 3, &err);
  act_string_t val3 = act_showable_cstr_as_string(&GPA, tst_struct->val3, &err);

  char *fmtstr = "Tst {\n  val1 = %s,\n  val2 = %s,\n  val3 = %s\n}";
  size_t fmtlen = strlen(fmtstr);

  char ret[act_string_len(val1) + act_string_len(val2) + act_string_len(val3) +
           fmtlen];
  int status = sprintf(ret, fmtstr, act_string_as_cstr(val1),
                       act_string_as_cstr(val2), act_string_as_cstr(val3));
  if (status < 0 || err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;
  act_string_free(&val1, &str_err);
  act_string_free(&val2, &str_err);
  act_string_free(&val3, &str_err);

  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  return act_string_from_cstr(&GPA, ret, &str_err);
}

void test_can_create_showable_struct(void) {
  Tst tst = {
      .val1 = 22,
      .val2 = 42.99,
      .val3 = "Hello World!",
  };

  act_showable_error_t *err = (*GPA.alloc)(1, sizeof(*err));
  *err = 0;

  ACT_SHOWABLE(Tst) tst_showable = ACT_SHOWABLE_NEW(Tst, &GPA, Tst_show, err);
  *tst_showable = tst;

  act_string_t str = act_showable_struct_as_string(tst_showable, err);
  // TEST_CHECK(strcmp(act_string_as_cstr(str),
  //                   "Tst {\n  val1 = 22,\n  val2 = 42.990,\n "
  //                   " val3 = \"Hello World!\"\n}") == 0);
  //
  // // act_showable_display(tst_showable, stderr);
  //
  // act_string_error_t str_err = ACT_STRING_ERROR_SUCCESS;
  // act_string_free(&str, &str_err);
  // act_showable_free(tst_showable, &err);
  //
  // if (str_err != ACT_STRING_ERROR_SUCCESS ||
  //     err != ACT_SHOWABLE_ERROR_SUCCESS) {
  //   exit(EXIT_FAILURE);
  // }
}

TEST_LIST = {{"[SHOWABLE] Can show struct", test_can_create_showable_struct},
             // {"[SHOWABLE] Can show unsigned integers", test_can_show_uint},
             // {"[SHOWABLE] Can show signed integers", test_can_show_uint},
             // {"[SHOWABLE] Can show floats", test_can_show_float},
             // {"[SHOWABLE] Can show C-string", test_can_show_cstring},
             {NULL, NULL}};
