#include <iostream>
#include <assert.h>

// In C/C++, the modulo operator "%"" is actually computing the remainder - contrarily to
// the mathematical euclidean remainder (also known as euclidean division, see https://en.wikipedia.org/wiki/Euclidean_division).
// Indeed, there are three main ways to compute the remainder: see https://en.wikipedia.org/wiki/Modulo_operation#Variants_of_the_definition.
//
// Therefore, this can lead to confusing situations. For example:
// For positive numbers, the "C++ remainder" and euclidean remainder are equal, but for negative numbers, it is slightly different.
// The euclidean modulo operator always produces a positive result: mod(-21, 4) == -21 mod 4 is 3 because -21 + 4 x 6 is 3
// The "C++ modulo operator" however can be negative: mod(-21, 4) == -21 divided by 4 gives -5 with a remainder of -1
//
// Assume that the quotient q and the remainder r of a divided by b satisfy the following conditions:
// - q is an element of Z (the set of all integer numbers)
// - a = b*q + r
// - |r| < |b|

// This function compues the remainder, the typical C/C++ modulo.
// It uses use truncated division, where the quotient is defined by truncation q = trunc(a/b) and the remainder is thus given by:
// r = a − b * trunc(a/b)
// Therefore, the remainder has the same sign as the dividend. The quotient is rounded towards zero:
// equal to the first integer in the direction of zero from the exact rational quotient.
//
// Examples:
// 21 % 4 is 1
// -21 % 4 is -1
// Undefined behaviour if b is 0.
// From: https://codereview.stackexchange.com/q/253799
int64_t modulo_truncated(int64_t a, int64_t b)
{
    assert(b != 0);
    // INT64_MIN % -1 is undefined behavior, so the result for y == -1 is hard-coded.
    if (b == -1)
    {
        return 0;
    }
    return a % b;
}

// This function computes the remainder with floored division (the result has the same sign as the divisor).
// The quotient is defined by the floor function q = ⌊a/n⌋ and the remainder has the same sign as the divisor.
// Due to the floor function, the quotient is always rounded downwards, even if it is already negative.
// The remainder is thus given by:
// r = a − b*⌊a/b⌋
// Undefined behaviour if b is 0.
// From: https://codereview.stackexchange.com/q/253799
int64_t modulo_floored(int64_t a, int64_t b)
{
    assert(b != 0);
    if (b == -1)
    {
        return 0;
    }
    return a - b * (a / b - (a % b && (a ^ b) < 0));
}

// This function computes the euclidean remainder.
// 21 euclidean modulo 4 is 1
// -21 euclidean modulo 4 is 3
// Undefined behaviour if b is 0.
// From: https://stackoverflow.com/a/20638659.
int64_t euclidean_modulo(int64_t a, int64_t b)
{
    assert(b != 0);
    if (b == -1)
    {
        return 0; // This test needed to prevent UB of `INT_MIN % -1
    }
    int64_t m = a % b;
    if (m < 0)
    {
        m = (b < 0) ? m - b : m + b;
    }
    return m;
}

// This function computes the euclidean reminder for values where b > 0.
int64_t euclidean_modulo_natural(int64_t a, int64_t b)
{
    assert(b > 0);
    return ((a % b) + b) % b;
}

// This function computes the euclidean reminder for values where a >= 0 and b > 0.
// Due to the precondition, the euclidean reminder is the same as just using modulo.
inline uint64_t euclidean_modulo_all_natural(uint64_t a, uint64_t b)
{
    assert(a >= 0);
    assert(b > 0);
    return a % b;
}

int main(int argc, char **argv)
{
    std::cout << "C/C++ modulo (aka. remainder): " << std::endl;
    std::cout << "modulo_truncated(21, 4) = " << modulo_truncated(21, 4) << std::endl;
    std::cout << "modulo_truncated(-21, 4) = " << modulo_truncated(-21, 4) << std::endl;
    std::cout << "modulo_truncated(21, -4) = " << modulo_truncated(21, -4) << std::endl;
    std::cout << "modulo_truncated(100, -1) = " << modulo_truncated(100, -1) << std::endl;
    std::cout << "modulo_truncated(INT64_MIN, -1) = " << modulo_truncated(INT64_MIN, -1) << std::endl;
    std::cout << "modulo_truncated(INT64_MIN, 68488) = " << modulo_truncated(INT64_MIN, 68488) << std::endl;
    std::cout << "modulo_truncated(INT64_MAX, 76953) = " << modulo_truncated(INT64_MAX, 76953) << std::endl;

    std::cout << "Modulo floored: " << std::endl;
    std::cout << "modulo_floored(21, 4) = " << modulo_floored(21, 4) << std::endl;
    std::cout << "modulo_floored(-21, 4) = " << modulo_floored(-21, 4) << std::endl;
    std::cout << "modulo_floored(21, -4) = " << modulo_floored(21, -4) << std::endl;
    std::cout << "modulo_floored(100, -1) = " << modulo_floored(100, -1) << std::endl;
    std::cout << "modulo_floored(INT64_MIN, -1) = " << modulo_floored(INT64_MIN, -1) << std::endl;
    std::cout << "modulo_floored(INT64_MIN, 68488) = " << modulo_floored(INT64_MIN, 68488) << std::endl;
    std::cout << "modulo_floored(INT64_MAX, 76953) = " << modulo_floored(INT64_MAX, 76953) << std::endl;

    std::cout << "Mathematical euclidean modulo: " << std::endl;
    std::cout << "euclidean_modulo(21, 4) = " << euclidean_modulo(21, 4) << std::endl;
    std::cout << "euclidean_modulo(-21, 4) = " << euclidean_modulo(-21, 4) << std::endl;
    std::cout << "euclidean_modulo(21, -4) = " << euclidean_modulo(21, -4) << std::endl;
    std::cout << "euclidean_modulo(100, -1) = " << euclidean_modulo(100, -1) << std::endl;
    std::cout << "euclidean_modulo(INT64_MIN, -1) = " << euclidean_modulo(INT64_MIN, -1) << std::endl;
    std::cout << "euclidean_modulo(INT64_MIN, 68488) = " << euclidean_modulo(INT64_MIN, 68488) << std::endl;
    std::cout << "euclidean_modulo(INT64_MAX, 76953) = " << euclidean_modulo(INT64_MAX, 76953) << std::endl;

    std::cout << "Mathematical euclidean modulo (only positive b): " << std::endl;
    std::cout << "euclidean_modulo_natural(21, 4) = " << euclidean_modulo_natural(21, 4) << std::endl;
    std::cout << "euclidean_modulo_natural(-21, 4) = " << euclidean_modulo_natural(-21, 4) << std::endl;
    std::cout << "euclidean_modulo_natural(INT64_MIN, 68488) = " << euclidean_modulo_natural(INT64_MIN, 68488) << std::endl;
    std::cout << "euclidean_modulo_natural(INT64_MAX, 76953) = " << euclidean_modulo_natural(INT64_MAX, 76953) << std::endl;

    std::cout << "Mathematical euclidean modulo (only positive values): " << std::endl;
    std::cout << "euclidean_modulo_all_natural(21, 4) = " << euclidean_modulo_natural(21, 4) << std::endl;
    std::cout << "euclidean_modulo_all_natural(INT64_MAX, 76953) = " << euclidean_modulo_natural(INT64_MAX, 76953) << std::endl;
    return 0;
}