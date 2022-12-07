#include "act_allocator.h"
#include "act_showable.h"
#include "acutest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_canShowUint(void) {
  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  uint8_t v1 = 10;
  act_String str = act_showableUint64AsString(&GPA, v1, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "10") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint16_t v2 = 20;
  str = act_showableUint64AsString(&GPA, v2, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "20") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint32_t v3 = 30;
  str = act_showableUint64AsString(&GPA, v3, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "30") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint64_t v4 = 40;
  str = act_showableUint64AsString(&GPA, v4, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "40") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  uint v5 = 50;
  str = act_showableUint64AsString(&GPA, v5, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "50") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canShowInt(void) {
  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  int8_t v1 = 10;
  act_String str = act_showableInt64AsString(&GPA, v1, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "10") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int16_t v2 = 20;
  str = act_showableInt64AsString(&GPA, v2, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "20") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int32_t v3 = 30;
  str = act_showableInt64AsString(&GPA, v3, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "30") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int64_t v4 = 40;
  str = act_showableInt64AsString(&GPA, v4, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "40") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  int v5 = 50;
  str = act_showableInt64AsString(&GPA, v5, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "50") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canShowFloat(void) {
  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  float v1 = 10.11F;
  act_String str = act_showableDoubleAsString(&GPA, v1, 2, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "10.11") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  double v2 = 20.22;
  str = act_showableDoubleAsString(&GPA, v2, 2, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str), "20.22") == 0);
  act_stringFree(&str, &str_err);
  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canShowCstring(void) {
  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  const char *v1 = "Hello World!";

  act_String str = act_showableCstrAsString(&GPA, v1, &err);

  TEST_CHECK(strcmp(act_stringAsCstr(str), "\"Hello World!\"") == 0);

  act_stringFree(&str, &str_err);

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

act_String Tst_show(act_Showable *tst) {
  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  Tst *tst_struct = tst;

  act_String val1 = act_showableInt64AsString(&GPA, tst_struct->val1, &err);
  act_String val2 = act_showableDoubleAsString(&GPA, tst_struct->val2, 3, &err);
  act_String val3 = act_showableCstrAsString(&GPA, tst_struct->val3, &err);

  char *fmtstr = "Tst {\n  val1 = %s,\n  val2 = %s,\n  val3 = %s\n}";
  size_t fmtlen = strlen(fmtstr);

  char ret[act_stringLen(val1) + act_stringLen(val2) + act_stringLen(val3) +
           fmtlen];
  int status = sprintf(ret, fmtstr, act_stringAsCstr(val1),
                       act_stringAsCstr(val2), act_stringAsCstr(val3));
  if (status < 0 || err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  act_stringFree(&val1, &str_err);
  act_stringFree(&val2, &str_err);
  act_stringFree(&val3, &str_err);

  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }

  return act_stringFromCstr(&GPA, ret, &str_err);
}

void test_canCreateShowableStruct(void) {
  Tst tst = {
      .val1 = 22,
      .val2 = 42.99,
      .val3 = "Hello World!",
  };

  int err = ACT_SHOWABLE_ERROR_SUCCESS;
  int str_err = ACT_STRING_ERROR_SUCCESS;

  ACT_SHOWABLE(Tst) tst_showable = ACT_SHOWABLE_NEW(Tst, &GPA, Tst_show, &err);
  *tst_showable = tst;

  act_String str = act_showableStructAsString(tst_showable, &err);
  TEST_CHECK(strcmp(act_stringAsCstr(str),
                    "Tst {\n  val1 = 22,\n  val2 = 42.990,\n "
                    " val3 = \"Hello World!\"\n}") == 0);

  act_showableDisplay(tst_showable, stderr, &err);

  act_stringFree(&str, &str_err);
  act_showableFree(tst_showable, &err);

  if (str_err != ACT_STRING_ERROR_SUCCESS ||
      err != ACT_SHOWABLE_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

TEST_LIST = {{"[SHOWABLE] Can show struct", test_canCreateShowableStruct},
             {"[SHOWABLE] Can show unsigned integers", test_canShowUint},
             {"[SHOWABLE] Can show signed integers", test_canShowUint},
             {"[SHOWABLE] Can show floats", test_canShowFloat},
             {"[SHOWABLE] Can show C-string", test_canShowCstring},
             {NULL, NULL}};
