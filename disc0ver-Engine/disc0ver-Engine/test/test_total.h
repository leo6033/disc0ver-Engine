#pragma once

#define TEST_MASTER
#define TEST_RAY_TRACING

#ifdef TEST_MASTER
int test_master_main();
#endif // TEST_MASTER

#ifdef TEST_RAY_TRACING
int test_ray_tracing_main();
#endif // TEST_RAY_TRACING
