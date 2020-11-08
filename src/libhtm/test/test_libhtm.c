
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

#include "test_libhtm.h"

entity_t *test_entity;

extern void TEST_celerity(CuTest*);
extern void TEST_position(CuTest*);
extern void TEST_quat(CuTest*);
extern void TEST_zpu(CuTest*);
extern void TEST_mem(CuTest*);

void test_libhtm_init(void);

int main(int argc, char *argv[])
{
  CuString *output = CuStringNew();
  CuSuite* suite = CuSuiteNew();
  int fails;

  test_libhtm_init();

  /* core tests */
  SUITE_ADD_TEST(suite, TEST_position);
  SUITE_ADD_TEST(suite, TEST_quat);

	/* module tests */
  SUITE_ADD_TEST(suite, TEST_celerity);
  SUITE_ADD_TEST(suite, TEST_zpu);
  SUITE_ADD_TEST(suite, TEST_mem);

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);
  CuStringDelete(output);
  fails = suite->failCount;
  CuSuiteDelete(suite);

  return (fails);
}

void test_libhtm_init(void)
{
	(void)htm_entity_init(&test_entity, "test");
}


