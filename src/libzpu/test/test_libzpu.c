
/*
 * @copyright
 *
 *  Copyright 2015 Brian Burrell
 *
 *  This file is part of Shioncoin.
 *  (https://github.com/neonatura/shioncoin)
 *        
 *  ShionCoin is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version. 
 *
 *  ShionCoin is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ShionCoin.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @endcopyright
 */  

#include "test_libzpu.h"

extern void TEST_zpu_bios(CuTest*);
extern void TEST_zpu_math(CuTest*);
extern void TEST_zpu_vaddr(CuTest*);

void test_libzpu_init(void);

zprocessor_t *test_zproc;
zpu_t *test_zpu;


int main(int argc, char *argv[])
{
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  int fails;

  test_libzpu_init();

  SUITE_ADD_TEST(suite, TEST_zpu_bios);
  SUITE_ADD_TEST(suite, TEST_zpu_math);
  SUITE_ADD_TEST(suite, TEST_zpu_vaddr);

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
  CuStringDelete(output);
  fails = suite->failCount;
  CuSuiteDelete(suite);

  return (fails);
}

void test_libzpu_init(void)
{
	test_zproc = zpu_init(1, 0xFFFFFFFF);
	test_zpu = zpu_core(test_zproc, 0);
}


