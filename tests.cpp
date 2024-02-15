//#include <gtest/gtest.h>
#include "bignum.h"
#include "cmake-build-debug/_deps/googletest-src/googletest/include/gtest/gtest.h"

TEST(Bignum_initialization, str_and_int_positive) {
bn::bignum a("12346780");
bn::bignum b(12346780);
EXPECT_EQ(a, b);
}

TEST(Bignum_initialization, str_and_int_negative) {
bn::bignum a("-1234678");
bn::bignum b(-1234678);
EXPECT_EQ(a, b);
}

TEST(Bignum_initialization, str_and_user_literal_integer) {
bn::bignum a("123467800");
bn::bignum b = 123467800_bn;
EXPECT_EQ(a, b);
}

TEST(Bignum_initialization, str_and_user_literal_float) {
bn::bignum a("-123.4678");
bn::bignum b = -123.4678_bn;
EXPECT_EQ(a, b);
}

TEST(Bignum_initialization, bad_str_and_user_literal_float) {
bn::bignum a("00123.40067800");
bn::bignum b = 00123.40067800_bn;
EXPECT_EQ(a, b);
}

TEST(Getting_info, get_exp) {
std::vector<char> exp{'3', '2', '3', '4', '8', '1'};
bn::bignum a = 18432300_bn;
EXPECT_EQ(a.get_exp(), exp);
}

TEST(Getting_info, get_sign) {
bn::bignum a = -0.052_bn;
EXPECT_EQ(a.get_sign(), -1);
}

TEST(Getting_info, get_size) {
bn::bignum a = -0.052_bn;
EXPECT_EQ(a.get_size(), 2);
}

TEST(Getting_info, get_mantissa_size) {
bn::bignum a = 0.0528965_bn;
EXPECT_EQ(a.get_mantissa_size(), 7);
}

TEST(Getting_info, get_integer_size) {
bn::bignum a = 14589600.1_bn;
EXPECT_EQ(a.get_integer_size(), 8);
}

TEST(Getting_info, get_digit_by_place_from_out_of_number) {
bn::bignum a = 14589600.1_bn;
EXPECT_EQ(a.get_digit_by_place(15), 0);
}

TEST(Getting_info, get_digit_by_place_from_number) {
bn::bignum a = 14589600.1_bn;
EXPECT_EQ(a.get_digit_by_place(4), 8);
}

TEST(Rounding, round_1) {
bn::bignum a = 123456.987654_bn;
EXPECT_EQ(a.round(3), 123456.987_bn);
}

TEST(Rounding, round_2) {
bn::bignum a = 123456.987654_bn;
EXPECT_EQ(a.round(), 123456_bn);
}

TEST(Rounding, round_3) {
bn::bignum a = 123456.987654_bn;
EXPECT_EQ(a.round(-2), 123400_bn);
}

TEST(Rounding, round_4) {
bn::bignum a = 123456.987654_bn;
EXPECT_EQ(a.round(20), 123456.987654_bn);
}

TEST(Zero_compare, is_zero_true) {
bn::bignum a = 0_bn;
EXPECT_EQ(a.is_zero(), true);
}

TEST(Zero_compare, is_zero_false) {
bn::bignum a = 1_bn;
EXPECT_EQ(a.is_zero(), false);
}

TEST(Zero_compare, zero_eq_zero) {
    bn::bignum a = 0_bn;
    bn::bignum b = 0_bn;
    EXPECT_EQ(a == b, true);
}

TEST(Zero_compare, minus_zero_eq_zero) {
    bn::bignum a = -0_bn;
    bn::bignum b = 0_bn;
    EXPECT_EQ(a == b, true);
}

TEST(Zero_compare, minus_zero_eq_minus_zero) {
    bn::bignum a = -0_bn;
    bn::bignum b = -0_bn;
    EXPECT_EQ(a == b, true);
}

TEST(Zero_compare, zero_eq_minus_zero) {
    bn::bignum a = 0_bn;
    bn::bignum b = -0_bn;
    EXPECT_EQ(a == b, true);
}

TEST(Compare, compare_diff_sign) {
bn::bignum a = 123_bn;
bn::bignum b = -123_bn;
EXPECT_EQ(a == b, false);
}

TEST(Compare, compare_same_sign) {
bn::bignum a = 123.145_bn;
bn::bignum b = 123.145_bn;
EXPECT_EQ(a == b, true);
}

TEST(Compare, compare_diff_sizes) {
bn::bignum a = 123.145_bn;
bn::bignum b = 0.123145_bn;
EXPECT_EQ(a == b, false);
}

TEST(Compare, unequal) {
bn::bignum a = 123.145_bn;
bn::bignum b = 0.123145_bn;
EXPECT_EQ(a != b, true);
}

TEST(Compare, bigger1) {
bn::bignum a = 123.145_bn;
bn::bignum b = 123.145_bn;
EXPECT_EQ(a > b, false);
}

TEST(Compare, bigger2) {
bn::bignum a = 123.145_bn;
bn::bignum b = -123.145_bn;
EXPECT_EQ(a > b, true);
}

TEST(Compare, bigger3) {
bn::bignum a = 124.145_bn;
bn::bignum b = 123.145_bn;
EXPECT_EQ(a > b, true);
}

TEST(Compare, bigger4) {
bn::bignum a = 123.145_bn;
bn::bignum b = 128.145_bn;
EXPECT_EQ(a > b, false);
}

TEST(Compare, bigger_or_eq1) {
bn::bignum a = 123.145_bn;
bn::bignum b = 123.145_bn;
EXPECT_EQ(a >= b, true);
}

TEST(Compare, bigger_or_eq2) {
bn::bignum a = 124.145_bn;
bn::bignum b = 123.145_bn;
EXPECT_EQ(a >= b, true);
}

TEST(Compare, smaller) {
bn::bignum a = 123_bn;
bn::bignum b = 123_bn;
EXPECT_EQ(a < b, false);
}

TEST(Compare, smaller_or_eq) {
bn::bignum a = 123_bn;
bn::bignum b = 123_bn;
EXPECT_EQ(a <= b, true);
}

TEST(Minus, minus_zero) {
EXPECT_EQ(-0_bn, 0_bn);
}

TEST(Minus, minus_non_zero) {
bn::bignum a("-123");
bn::bignum b = 123_bn;
EXPECT_EQ(a, -b);
}

TEST(Abs, abs) {
bn::bignum a("-123");
EXPECT_EQ(a.abs(), -a);
}

TEST(Addition, pos_plus_pos1) {
EXPECT_EQ(123_bn+456_bn, 579_bn);
}

TEST(Addition, pos_plus_pos2) {
EXPECT_EQ(500_bn+500_bn, 1000_bn);
}

TEST(Addition, neg_plus_neg) {
EXPECT_EQ((-500_bn)+(-250_bn), -750_bn);
}

TEST(Addition, pos_plus_eq_neg) {
EXPECT_EQ(12345_bn+(-12345_bn), 0_bn);
}

TEST(Addition, pos_plus_smaller_neg) {
EXPECT_EQ(500_bn+(-490_bn), 10_bn);
}

TEST(Addition, pos_plus_bigger_neg) {
EXPECT_EQ(500_bn+(-510_bn), -10_bn);
}

TEST(Addition, neg_plus_smaller_pos) {
EXPECT_EQ((-510_bn)+500_bn, -10_bn);
}

TEST(Addition, neg_plus_bigger_pos) {
EXPECT_EQ((-490_bn)+500_bn, 10_bn);
}

TEST(Subtraction, subtraction) {
EXPECT_EQ(123456_bn-12345_bn, 111111_bn);
}

TEST(Multiplication, regular_multiplication_1) {
bn::bignum a("12300");
EXPECT_EQ(a.multiply_by(a), 151290000_bn);
}

TEST(Multiplication, regular_multiplication_2) {
bn::bignum a("-12.3");
EXPECT_EQ(a.multiply_by(a), 151.29_bn);
}

TEST(Multiplication, regular_multiplication_by_operator) {
bn::bignum a("12300");
EXPECT_EQ(a*a, 151290000_bn);
}

TEST(Multiplication, Karatsuba_by_operator) {
bn::bignum a("12345678901234567890123456789012345678901234567890");
EXPECT_EQ(a*a, 152415787532388367504953515625666819450083828733757049236500533455762536198787501905199875019052100_bn);
}

TEST(Division, division_with_precision1) {
EXPECT_EQ((1_bn).divide_by(3_bn, 5), 0.33333_bn);
}

TEST(Division, division_with_precision2) {
EXPECT_EQ((1_bn).divide_by(4_bn, 2), 0.25_bn);
}

TEST(Division, division_by_zero) {
EXPECT_ANY_THROW(1_bn/0_bn);
}

TEST(Something_and_eq, plus_eq) {
    bn::bignum a = 1234.567_bn;
    a += 1010_bn;
EXPECT_EQ(a, 2244.567_bn);
}

TEST(Something_and_eq, minus_eq) {
bn::bignum a = 1234.567_bn;
a -= 1010_bn;
EXPECT_EQ(a, 224.567_bn);
}

TEST(Something_and_eq, mult_eq) {
bn::bignum a = 11_bn;
a *= 110_bn;
EXPECT_EQ(a, 1210_bn);
}

TEST(Something_and_eq, div_eq) {
bn::bignum a = 144_bn;
a /= 12_bn;
EXPECT_EQ(a, 12_bn);
}