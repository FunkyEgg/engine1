#include <engine1/math/math.h>

uint64_t nearest_containing_power_of_2(uint64_t num) {
    if (num == 0) { return 1; }

    // Do some bitwie hackery to see if a power of 2, if so retrun num
    if (!(num & (num - 1))) { return num; }

    uint64_t pow = 1;
    while (pow < num) { pow = pow * 2; }
    return pow;
}
