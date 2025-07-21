#include <scn/scan.h>
#include <cstdio>

#include "monolithic_examples.h"

#if defined(BUILD_MONOLITHIC)
#define main scanf_consumer_test_main
#endif

int main(void)
{
    auto result = scn::input<int>("{}");
    if (!result) {
        return 1;
    }
    std::printf("%d", result->value());
    return 0;
}
