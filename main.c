#include <stdio.h>
#include "stack.h"
#include "unity.h"


void test_ctor_dtor_stack_should_create_stack(void)
{
    stack_t stk = {};
    
    stack_ctor(&stk, 100);

    TEST_ASSERT_NOT_NULL_MESSAGE(stk.data, "expected not NULL pointer");

    stack_dtor(&stk);
}

void test_ctor_dtor_stack_100el_should_stack_size_100(void)
{
    const int size = 100;
    stack_t stk = {};
    
    stack_ctor(&stk, size);
    
    TEST_ASSERT_EQUAL_INT(size, stk.size);
    
    stack_dtor(&stk);
}

void test_ctor_dtor_count_stack_should_count_zero(void)
{
    stack_t stk = {};
    
    stack_ctor(&stk, 100);
    
    TEST_ASSERT_EQUAL_INT(0, stk.count);
    
    stack_dtor(&stk);
}

void test_push_one_should_return_one(void)
{
    stack_t stk = {};
    
    stack_ctor(&stk, 100);
    
    stack_push(&stk, 1);
    
    TEST_ASSERT_EQUAL_INT(1, stk.data[stk.count-1]);
    
    stack_dtor(&stk);
}

void test_push_one_pop_should_return_one(void)
{
    int x = 0;
    stack_t stk = {};
    
    stack_ctor(&stk, 100);
    
    stack_push(&stk, 1);
    stack_pop(&stk, &x);
    
    TEST_ASSERT_EQUAL_INT(1, x);
    
    stack_dtor(&stk);
}

void test_stack_ok_stack_count_change_sign_should_return_zero(void)
{
	int i = 0;
	stack_t stk = {};
	
	stack_ctor(&stk, 100);
	for (i = 0; i < 100; i++) {
		stack_push(&stk, 1);
	}
	
	stk.count = -1.0*stk.count;
	
	TEST_ASSERT_EQUAL(0, stack_ok(stk));
	
	stack_dtor(&stk);
}

void test_stack_ok_size_less_count_should_return_zero(void)
{
	int i = 0;
	stack_t stk = {};
	
	stack_ctor(&stk, 100);
	for (i = 0; i < 100; i++) {
		stack_push(&stk, 1);
	}
	
	stk.size = stk.count - 1;
	
	TEST_ASSERT_EQUAL(0, stack_ok(stk));
	
	stack_dtor(&stk);
}

void test_stack_ok_hash_change_one_value_return_zero(void)
{
	int i = 0;
	stack_t stk = {};
	
	stack_ctor(&stk, 100);
	for (i = 0; i < 100; i++) {
		stack_push(&stk, 1);
	}
	
	stk.data[0] = 3;
	
	TEST_ASSERT_EQUAL(0, stack_ok(stk));
	
	stack_dtor(&stk);
}
	

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_ctor_dtor_stack_should_create_stack);
    RUN_TEST(test_ctor_dtor_stack_100el_should_stack_size_100);
    RUN_TEST(test_ctor_dtor_count_stack_should_count_zero);
    RUN_TEST(test_push_one_should_return_one);
    RUN_TEST(test_push_one_pop_should_return_one);
    RUN_TEST(test_stack_ok_stack_count_change_sign_should_return_zero);
    RUN_TEST(test_stack_ok_size_less_count_should_return_zero);
    RUN_TEST(test_stack_ok_hash_change_one_value_return_zero);
    
    return UNITY_END();
}
