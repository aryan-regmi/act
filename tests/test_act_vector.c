#include "act_allocator.h"
#include "act_vector.h"
#include "acutest.h"
#include <stdlib.h>

void test_canCreateNewVector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  ACT_VEC(double) vec = act_vectorNew(&GPA, sizeof(double), &err);

  TEST_CHECK(vec != NULL);
  TEST_CHECK(act_vectorLen(vec, &err) == 0);
  TEST_CHECK(act_vectorCapacity(vec, &err) == 0);
  TEST_CHECK(act_vectorDataSize(vec, &err) == sizeof(double));

  act_vectorFree(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canCreateNewVectorWithCapacity(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 10;
  ACT_VEC(double)
  vec = act_vectorWithCapacity(&GPA, sizeof(double), CAP, &err);

  TEST_CHECK(vec != NULL);
  TEST_CHECK(act_vectorLen(vec, &err) == 0);
  TEST_CHECK(act_vectorCapacity(vec, &err) == CAP);
  TEST_CHECK(act_vectorDataSize(vec, &err) == sizeof(double));

  act_vectorFree(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canPushToVector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 5;
  ACT_VEC(size_t)
  vec = act_vectorWithCapacity(&GPA, sizeof(size_t), CAP, &err);

  for (size_t i = 0; i < (size_t)CAP; i++) {
    ACT_VEC_PUSH(vec, i, &err);
    TEST_CHECK(vec[i] == i);
  }

  act_vectorFree(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canPopFromVector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  const size_t CAP = 5;
  ACT_VEC(size_t)
  vec = act_vectorWithCapacity(&GPA, sizeof(size_t), CAP, &err);

  for (size_t i = 0; i < (size_t)CAP; i++) {
    ACT_VEC_PUSH(vec, i, &err);
  }

  for (size_t i = 0; i < (size_t)CAP; i++) {
    size_t cval = 0;
    ACT_VEC_POP(vec, &cval, &err);
    TEST_CHECK(cval == CAP - 1 - i);
  }

  act_vectorFree(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canResizeVector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  ACT_VEC(double) vec = act_vectorNew(&GPA, sizeof(double), &err);

  const size_t LEN = 11;
  const double SCALE = 0.5;
  for (size_t i = 0; i < LEN; i++) {
    ACT_VEC_PUSH(vec, i * SCALE, &err);
  }

  const size_t EXPECTED_CAP = 16;
  TEST_CHECK(act_vectorCapacity(vec, &err) == EXPECTED_CAP);

  act_vectorFree(vec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

void test_canShrinkToFitVector(void) {
  int err = ACT_VECTOR_ERROR_SUCCESS;

  ACT_VEC(int) mvec = ACT_VEC_WCAP(int, &GPA, 20, &err);
  for (int i = 0; i < 10; i++) {
    ACT_VEC_PUSH(mvec, i, &err);
  }

  TEST_ASSERT(act_vectorCapacity(mvec, &err) == 20);

  // act_vectorShrinkToFit(mvec, &err);
  ACT_VEC_SHRINK_TO_FIT(int, mvec, &err);

  TEST_ASSERT(act_vectorCapacity(mvec, &err) == 10);

  act_vectorFree(mvec, &err);

  if (err != ACT_VECTOR_ERROR_SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

TEST_LIST = {
    {"[VECTOR] Can create new act_vector_t", test_canCreateNewVector},
    {"[VECTOR] Can create new act_vector_t with capacity",
     test_canCreateNewVectorWithCapacity},
    {"[VECTOR] Can push values to act_vector_t", test_canPushToVector},
    {"[VECTOR] Can pop values from act_vector_t", test_canPopFromVector},
    {"[VECTOR] Can resize act_vector_t", test_canResizeVector},
    {"[VECTOR] Can shrink act_vector_t to fit length",
     test_canShrinkToFitVector},
    {NULL, NULL}};
