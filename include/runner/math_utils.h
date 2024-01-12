#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <cstdint>

using namespace std;

class MathUtils {
    public:
        static int64_t modulo_inverse(int64_t value, int64_t modulus);
        static int64_t chinese_remainder_theorem(int num_equations, int64_t * values, int64_t * moduluses);
        static int64_t lcm(int num_factors, int64_t * values);
        static int64_t lcm(int64_t first, int64_t second);
        static int64_t gcd(int64_t first, int64_t second);
};

#endif
