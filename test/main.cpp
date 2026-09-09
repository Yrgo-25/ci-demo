/**
 * @file Unit test suite.
 */
#include "yrgo/test/test.h"

/**
 * @brief Run all test cases.
 *
 * @return 0 if every test case succeeded, -1 otherwise.
 */
int main() { return yrgo::test::runAllTests() ? 0 : -1; }
