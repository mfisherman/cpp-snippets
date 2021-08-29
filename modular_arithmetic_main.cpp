#include <iostream>
#include <assert.h>

// This code shows how to do modular arithmetic in C++ (https://en.wikipedia.org/wiki/Modular_arithmetic).

// In the following, we assume that the input to the functions is valid, meaning is an element of Z_n.
// Therefore, the modulo operator is the special case of the euclidean reminder for values where a >= 0 and b > 0.
// This allows us to use just the modulo operator "%" to perform the modulo operation.

// This function computes the euclidean reminder for values where n > 0.
// It allows to convert any value (even negative) into the space Z_n.
inline uint64_t mod(int64_t a, int64_t n)
{
    assert(n > 0);
    return ((a % n) + n) % n;
}

// This function computes the euclidean reminder for values where n > 0 and a > 0.
// It allows to convert positive values into the space Z_n.
inline uint64_t mod_pos(uint64_t a, uint64_t n)
{
    assert(n > 0);
    return a % n;
}

// This function computes (a + b) % n.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_add(uint64_t a, uint64_t b, uint64_t n)
{
    assert(a < n);
    assert(b < n);
    assert(n > 0);

    if (b == 0)
        return a;

    // Returns mod_minus(a, m-b, m);
    b = n - b;
    if (a >= b)
        return a - b;
    else
        return n - b + a;
}

// This function computes (a - b) % n.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_subtract(uint64_t a, uint64_t b, uint64_t n)
{
    assert(a < n);
    assert(b < n);
    assert(n > 0);

    if (a >= b)
    {
        return a - b;
    }
    else
    {
        return n - b + a;
    }
}

// This function computes (a + 1) % n.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_increment(uint64_t a, uint64_t n)
{
    assert(a < n);
    assert(n > 0);

    a++;
    if (a == n)
    {
        a = 0;
    }
    return a;
}

// This function computes (a - 1) % n.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_decrement(uint64_t a, uint64_t n)
{
    assert(a < n);
    assert(n > 0);

    if (a == 0)
    {
        a = n - 1;
    }
    else
    {
        a--;
    }
    return a;
}

// This function computes the additive inverse of a, such that:
// mod_add(a, mod_additive_inverse(a, n), n) == 0.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_additive_inverse(uint64_t a, uint64_t n)
{
    assert(a < n);
    assert(n > 0);

    if (a == 0)
    {
        return 0;
    }
    else
    {
        return n - a;
    }
}

// This function computes (a * b) % n.
// It uses the double and add algorithm to achieve this, requires O(log(a) + log(b)) time.
inline uint64_t mod_multiply(uint64_t a, uint64_t b, uint64_t n)
{
    assert(a < n);
    assert(b < n);
    assert(n > 0);

    uint64_t product = 0;
    if (b > a)
    {
        std::swap(a, b);
    }

    while (b)
    {
        if (b & 0x1)
        {
            product = mod_add(product, a, n);
        }
        a = mod_add(a, a, n);
        b >>= 1;
    }
    return product;
}

// This function computes (a * a) % n.
inline uint64_t mod_sqr(uint64_t a, uint64_t n)
{
    assert(a < n);
    assert(n > 0);
    return mod_multiply(a, a, n);
}

// This function computes (a^e) % n.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
inline uint64_t mod_power(uint64_t a, uint64_t e, uint64_t n)
{
    assert(a < n);
    assert(n > 0);

    if (e == 0)
    {
        return 1;
    }
    else
    {
        uint64_t z = a;
        uint64_t y = 1;
        while (1)
        {
            if (e & 0x1)
            {
                y = mod_multiply(y, z, n); // y *= z;
            }
            e >>= 1;
            if (0 == e)
            {
                break;
            }
            z = mod_sqr(z, n); // z *= z;
        }
        return y;
    }
}

// Returns the multiplicative inverse of a, such that:
// mod_multiply(a, mod_multiplicative_inverse(a, n)) == 1.
// Note that the multiplicative inverse only exists when n is a prime.
inline uint64_t mod_multiplicative_inverse(uint64_t a, uint64_t n)
{
    assert(a < n);
    assert(n > 0);
    return mod_power(a, n - 2, n);
}

// This function returns u3 and sets tu1, tu2 such that that gcd(a,n) == u3 == a*tu1 + n*tu2.
// This can be used to determine the multiplicative inverse:
// To invert a % n, we need gcd(a, n) = 1.
// We can call the extended GCD algorithm with a and n as input and check if the GCD is 1.
// If so, we also get tu1, tu2 such that a*tu1 + n*tu2 = u3 = 1. We then see that:
// (a*tu1 + n*tu2) % n = a*tu1 % n = 1. Therefore, tu1 is the inverse of a.
// From: Matters Computational (https://www.jjj.de/fxt/fxtbook.pdf), chapter 39.1.
uint64_t extended_greatest_common_divisor(int64_t a, int64_t n, int64_t &tu1, int64_t &tu2)
{
    int64_t u1 = 1, u2 = 0;
    int64_t v1 = 0, v3 = n;
    int64_t u3 = a, v2 = 1;
    while (v3 != 0)
    {
        int64_t q = u3 / v3;
        int64_t t1 = u1 - v1 * q;
        u1 = v1;
        v1 = t1;
        int64_t t3 = u3 - v3 * q;
        u3 = v3;
        v3 = t3;
        int64_t t2 = u2 - v2 * q;
        u2 = v2;
        v2 = t2;
    }
    tu1 = u1;
    tu2 = u2;
    return u3;
}

int main(int argc, char **argv)
{
    std::cout << "-9978483 % 6742 = " << mod(-9978483, 6742) << std::endl;
    std::cout << "368554407370949273 % 698223547 = " << mod_pos(368554407370949273UL, 698223547) << std::endl;
    std::cout << "(3577888489959895 + 1944674407370949273) % 13686744073709492732 = " << mod_add(3577888489959895UL, 1944674407370949273UL, 13686744073709492732UL) << std::endl;
    std::cout << "(18226785267862220 - 6985665525488000877) % 7985665525488000877 = " << mod_subtract(18226785267862220UL, 6985665525488000877UL, 7985665525488000877UL) << std::endl;
    std::cout << "(68529989 + 1) % 68529990 = " << mod_increment(68529989UL, 68529990UL) << std::endl;
    std::cout << "(0 - 1) % 68529990 = " << mod_decrement(0, 68529990UL) << std::endl;
    std::cout << "(5478239525828 + x) % 678874930481234881 = 0 -> x = " << mod_additive_inverse(5478239525828UL, 678874930481234881UL) << std::endl;
    std::cout << "(5478239525828 + 678869452241709053) % 678874930481234881 = " << mod_add(5478239525828UL, mod_additive_inverse(5478239525828UL, 678874930481234881UL), 678874930481234881UL) << std::endl;
    std::cout << "(18446743983658366132 * 17446663900858366132) % 18446743988858366132 = " << mod_multiply(18446743983658366132UL, 17446663900858366132UL, 18446743988858366132UL) << std::endl;
    std::cout << "(9876743983658366132 * 9876743983658366132) % 18446743988858366132 = " << mod_sqr(9876743983658366132UL, 18446743988858366132UL) << std::endl;
    std::cout << "(7829454892340959985^437827489237484) % 12985254587577588852 = " << mod_power(7829454892340959985UL, 437827489237484UL, 12985254587577588852UL) << std::endl;
    std::cout << "(97845874148483 * x) % 9223372036854775337 = 1 -> x = " << mod_multiplicative_inverse(97845874148483UL, 9223372036854775337UL) << std::endl;
    std::cout << "(97845874148483 * 7706179975126099074) % 9223372036854775337 = " << mod_multiply(97845874148483, mod_multiplicative_inverse(97845874148483UL, 9223372036854775337UL), 9223372036854775337UL) << std::endl;
    std::cout << "(978458741484 * 18798863501111358) % 92233720368547753 = " << mod_multiply(978458741484, mod_multiplicative_inverse(978458741484, 92233720368547753UL), 92233720368547753UL) << std::endl;

    int64_t tu1 = 0;
    int64_t tu2 = 0;
    uint64_t u3 = extended_greatest_common_divisor(978458741484, 92233720368547753, tu1, tu2);
    assert(u3 == 1);
    std::cout << "(978458741484 * " << tu1 << " + 92233720368547753 * " << tu2 << ") % 92233720368547753 = " << mod_add(mod_multiply(978458741484, tu1, 92233720368547753), 0, 92233720368547753) << std::endl;
}