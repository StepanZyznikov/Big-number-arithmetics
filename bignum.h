//
// Created by Stepan Zyznikov on 14.02.2024.
//

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

#ifndef INC_1_BIGNUM_H
#define INC_1_BIGNUM_H

#endif //INC_1_BIGNUM_H

namespace bn {
    class bignum {
    public:
        explicit bignum(const std::string &s);

        explicit bignum(int a);

        [[nodiscard]] std::vector<char> get_exp() const;

        [[nodiscard]] int get_sign() const;

        [[nodiscard]] int get_size() const;

        [[nodiscard]] int get_mantissa_size() const;

        [[nodiscard]] int get_integer_size() const;

        [[nodiscard]] int get_digit_by_place(int place) const;

        [[nodiscard]] bool is_zero() const;

        bool operator==(bignum const &num) const;

        bool operator!=(bignum const &num) const;

        bool operator>(bignum const &num) const;

        bool operator>=(bignum const &num) const;

        bool operator<(bignum const &num) const;

        bool operator<=(bignum const &num) const;

        bignum operator-() const;

        [[nodiscard]] bignum abs() const;

        [[nodiscard]] bignum multiply_by(const bignum &num) const;

        [[nodiscard]] bignum divide_by(const bignum &num, int wanted_precision = 10) const;

        [[nodiscard]] bignum round(int wanted_mantissa_size = 0) const;

        bignum operator+(const bignum &num) const;

        void operator+=(const bignum &num);

        bignum operator-(const bignum &num) const;

        void operator-=(const bignum &num);

        bignum operator*(const bignum &num) const;

        void operator*=(const bignum &num);

        bignum operator/(const bignum &num) const;

        void operator/=(const bignum &num);

        void print(std::ostream &os) const;

        bignum(const std::vector<char> &d_exp, int d_mantissa_size, int d_sign);
        [[nodiscard]] bignum split(int len, int piece) const; // Игноpиpует значение длины мантиссы и знак!
        [[nodiscard]] bignum shift(int shift_size) const; // Смещение на shift_size nоpядков вnpаво, если shift_size > 0, иначе влево
        [[nodiscard]] int find_closest_lower_or_equal(const bignum &num) const; // Исnользуется в делении

        friend std::ostream &operator<<(std::ostream &os, const bignum &a);

    private:
        std::vector<char> exp;
        int mantissa_size;
        int sign;
    };
}

bn::bignum operator "" _bn(const char *s);