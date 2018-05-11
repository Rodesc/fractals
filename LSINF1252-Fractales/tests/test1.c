#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../libfractal/fractal.h"

void test_getters(void) {
	const char* name = "myfractal";
	int width = 30;
	int height = 20;
	double a = 0.2;
	double b = 0.5;

	struct fractal *fract = fractal_new(name, width, height, a, b);

	CU_ASSERT_EQUAL(fractal_get_name(fract), name);
	CU_ASSERT_EQUAL(fractal_get_width(fract), width);
	CU_ASSERT_EQUAL(fractal_get_height(fract), height);
	CU_ASSERT_EQUAL(fractal_get_a(fract), a);
	CU_ASSERT_EQUAL(fractal_get_b(fract), b);

}

void test_set_and_get_value(void) {
	struct fractal *fract = fractal_new("myfractal", 20, 20, 0.2, 0.5);

	for(int x = 0; x < 20; x++){
		for(int y = 0; y< 20; y++){
			fractal_set_value(fract, x, y, x+y);
		}
	}

	for(int x = 0; x < 20; x++){
		for(int y = 0; y< 20; y++){
			CU_ASSERT_EQUAL(fractal_get_value(fract, x, y), x+y);
		}
	}

}
		


int main()
{
	CU_pSuite pSuite = NULL;


	
	if(CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();



	pSuite = CU_add_suite("Suite_1", NULL, NULL);

   	if (NULL == pSuite) {
      		CU_cleanup_registry();
      		return CU_get_error();
   	}


	if(NULL == CU_add_test(pSuite, "Test_Getters", test_getters) ||
	   NULL == CU_add_test(pSuite, "Test_GetSetValue", test_set_and_get_value))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}



	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();


	return CU_get_error();

}
