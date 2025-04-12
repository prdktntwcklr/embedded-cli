#ifndef CHECK_ASSERT_H
#define CHECK_ASSERT_H

#include "CException.h"

#define SHOULD_FAIL_ASSERT(_code_under_test)                                   \
    {                                                                          \
        CEXCEPTION_T e;                                                        \
        Try                                                                    \
        {                                                                      \
            _code_under_test;                                                  \
            TEST_FAIL_MESSAGE("Code under test did not assert!");              \
        }                                                                      \
        Catch(e) {}                                                            \
    }

#endif // CHECK_ASSERT_H
