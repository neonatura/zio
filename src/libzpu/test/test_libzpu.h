
#ifndef __TEST_ZPU_H__
#define __TEST_ZPU_H__


#include "zpu.h"
#include "CuTest.h"

#define _TEST(_name) void TEST_##_name(CuTest *ct) 
#define _END_TEST

#define _TRUE(testexpr) \
  CuAssertTrue(ct, (testexpr))
#define _FALSE(testexpr) \
  CuAssertTrue(ct, !(testexpr))

#define _TRUE_EQUAL(_a,_b) \
  _TRUE((_a) == (_b))
#define _TRUE_NE(_a,_b) \
  _TRUE((_a) != (_b))
#define _TRUE_GT(_a,_b) \
  _TRUE((_a) > (_b))
  

static void *_cutest_ptr;
#define _TRUEPTR(testptr) \
  _cutest_ptr = (testptr); CuAssertPtrNotNull(ct, _cutest_ptr)

int test_libzpu_main(void);

extern zprocessor_t *test_zproc;
extern zpu_t *test_zpu;

#endif /* ndef __TEST_ZPU_H__ */


