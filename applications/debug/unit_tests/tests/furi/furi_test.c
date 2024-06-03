#include <stdio.h>
#include <furi.h>
#include <furi_hal.h>
#include "../test.h"

// v2 tests
void test_furi_create_open(void);
void test_furi_concurrent_access(void);
void test_furi_pubsub(void);

void test_furi_memmgr(void);
void test_furi_memmgr_advanced(void);
void test_furi_memmgr_aligned8(void);

static int foo = 0;

void test_setup(void) {
    foo = 7;
}

void test_teardown(void) {
    /* Nothing */
}

MU_TEST(test_check) {
    mu_check(foo != 6);
}

// v2 tests
MU_TEST(mu_test_furi_create_open) {
    test_furi_create_open();
}

MU_TEST(mu_test_furi_pubsub) {
    test_furi_pubsub();
}

MU_TEST(mu_test_furi_memmgr) {
    // this test is not accurate, but gives a basic understanding
    // that memory management is working fine
    test_furi_memmgr();
    test_furi_memmgr_advanced();
    test_furi_memmgr_aligned8();
}

MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_check);

    // v2 tests
    MU_RUN_TEST(mu_test_furi_create_open);
    MU_RUN_TEST(mu_test_furi_pubsub);
    MU_RUN_TEST(mu_test_furi_memmgr);
}

int run_minunit_test_furi(void) {
    MU_RUN_SUITE(test_suite);

    return MU_EXIT_CODE;
}

TEST_API_DEFINE(run_minunit_test_furi)
