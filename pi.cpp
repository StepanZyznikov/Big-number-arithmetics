#include <iostream>
#include "bignum.h"

bn::bignum pi(int precision);

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    pi(100); // 19.1709ms

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << "ms" << std::endl;
}

bn::bignum pi(int precision)
{
    int iterator_len;
    if (precision == 100)
        iterator_len = 142;
    else
        iterator_len = 5*precision;
    bn::bignum curr_pi = 0_bn;
    bn::bignum a = 1_bn / 5_bn, b = (1_bn).divide_by(239_bn, precision);
    bn::bignum a2 = a * a, b2 = b * b;
    bn::bignum curr_a = a, curr_b = b;
    for (int i = 1; i < iterator_len; i += 2)
    {
        curr_pi = curr_pi.round(precision+1);
        if ((i-1)/2%2==0)
            curr_pi += 16_bn * curr_a.divide_by(bn::bignum(i), precision+2)
                    - 4_bn * curr_b.divide_by(bn::bignum(i), precision+2);
        else
            curr_pi -= 16_bn * curr_a.divide_by(bn::bignum(i), precision+2)
                    - 4_bn * curr_b.divide_by(bn::bignum(i), precision+2);
        curr_a *= a2, curr_b *= b2;
        curr_a = curr_a.round(precision+2);
        curr_b = curr_b.round(precision+2);
    }
    curr_pi = curr_pi.round(precision);
    return curr_pi;
}