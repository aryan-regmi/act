#include "act_allocator.h"
#include "act_vector.h"
#include "acutest.h"
#include <stdlib.h>

void test_can_create_new_vector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  ACT_VEC(double) vec = act_vector_new(&GPA, sizeof(double), &err);

  TEST_CHECK(vec != NULL);
  TEST_CHECK(act_vector_len(vec, &err) == 0);
  TEST_CHECK(act_vector_capacity(vec, &err) == 0);
  TEST_CHECK(act_vector_data_size(vec, &err) == sizeof(double));

  act_vector_free(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_create_new_vector_with_capacity(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 10;
  ACT_VEC(double)
  vec = act_vector_with_capacity(&GPA, sizeof(double), CAP, &err);

  TEST_CHECK(vec != NULL);
  TEST_CHECK(act_vector_len(vec, &err) == 0);
  TEST_CHECK(act_vector_capacity(vec, &err) == CAP);
  TEST_CHECK(act_vector_data_size(vec, &err) == sizeof(double));

  act_vector_free(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_push_to_vector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 5;
  ACT_VEC(size_t)
  vec = act_vector_with_capacity(&GPA, sizeof(size_t), CAP, &err);

  for (size_t i = 0; i < (size_t)CAP; i++) {
    ACT_VEC_PUSH(vec, i, &err);
    TEST_CHECK(vec[i] == i);
  }

  act_vector_free(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_pop_from_vector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 5;
  ACT_VEC(size_t)
  vec = act_vector_with_capacity(&GPA, sizeof(size_t), CAP, &err);

  for (size_t i = 0; i < (size_t)CAP; i++) {
    ACT_VEC_PUSH(vec, i, &err);
  }

  for (size_t i = 0; i < (size_t)CAP; i++) {
    size_t cval = 0;
    ACT_VEC_POP(vec, &cval, &err);
    TEST_CHECK(cval == CAP - 1 - i);
  }

  act_vector_free(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_can_resize_vector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  ACT_VEC(double) vec = act_vector_new(&GPA, sizeof(double), &err);

  const size_t LEN = 11;
  const double SCALE = 0.5;
  for (size_t i = 0; i < LEN; i++) {
    ACT_VEC_PUSH(vec, i * SCALE, &err);
  }

  const size_t EXPECTED_CAP = 16;
  TEST_CHECK(act_vector_capacity(vec, &err) == EXPECTED_CAP);

  act_vector_free(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

TEST_LIST = {
    {"[VECTOR] Can create new act_vector_t", test_can_create_new_vector},
    {"[VECTOR] Can create new act_vector_t with capacity",
     test_can_create_new_vector_with_capacity},
    {"[VECTOR] Can push values to act_vector_t", test_can_push_to_vector},
    {"[VECTOR] Can pop values from act_vector_t", test_can_pop_from_vector},
    {"[VECTOR] Can resize act_vector_t", test_can_resize_vector},
    {NULL, NULL}};
