#include <iostream>
#include <vector>
#include <string>
#include "bignum.h"
#define NUM_SYS 10

int c2i(const char c) {
    return (int) (c - '0');
}

char i2c(const int i) {
    return (char) (i + '0');
}

bn::bignum operator "" _bn(const char *s) {
    return bn::bignum(s);
}

std::ostream &bn::operator<<(std::ostream &os, const bn::bignum &a) {
    a.print(os);
    return os;
}

namespace bn {
    bignum::bignum(const std::string &s) {
        this->sign = ((int) s.find('-') == -1) ? 1 : -1;

        int s_len = (int) s.length();
        int dot_pos = (int) s.find('.');

        if (dot_pos != -1)
            this->mantissa_size = s_len - dot_pos - 1;
        else
            this->mantissa_size = 0;

        bool begin_flag = true;
        int zeros_counter = 0;
        for (int i = s_len - 1; i >= 0; i -= 1)
        {
            if (begin_flag && (s[i] == '0' || s[i] == '.'))
                this->mantissa_size -= (s[i] == '0');
            else
            {
                begin_flag = false;
                if (s[i] == '0')
                    zeros_counter += 1;
                if (s[i] != '.' && s[i] != '-' && s[i] != '0')
                {
                    for (int j = 0; j < zeros_counter; j += 1)
                        this->exp.push_back('0');
                    zeros_counter = 0;
                    this->exp.push_back(s[i]);
                }
            }
        }
    }

    bignum::bignum(const int a) {
        int temp_a = a;
        if (a < 0) {
            this->sign = -1;
            temp_a *= -1;
        }
        else
            this->sign = 1;
        this->mantissa_size = 0;
        int begin_flag = 1;
        while (temp_a > 0) {
            if (temp_a % NUM_SYS == 0 && begin_flag == 1)
                this->mantissa_size -= 1;
            else {
                this->exp.push_back(i2c(temp_a % NUM_SYS));
                begin_flag = 0;
            }
            temp_a /= NUM_SYS;
        }
    }

    bignum::bignum(const std::vector<char>& d_exp, const int d_mantissa_size, const int d_sign) {
        this->mantissa_size = d_mantissa_size;
        this->sign = d_sign;
        this->exp = d_exp;
    }

    [[nodiscard]]
    std::vector<char> bignum::get_exp() const {
        return this->exp;
    }

    [[nodiscard]]
    int bignum::get_sign() const {
        return this->sign;
    }

    [[nodiscard]]
    int bignum::get_size() const {
        return (int) this->exp.size();
    }

    [[nodiscard]]
    int bignum::get_mantissa_size() const {
        return this->mantissa_size;
    }

    [[nodiscard]]
    int bignum::get_integer_size() const {
        return this->get_size() - this->get_mantissa_size();
    }

    [[nodiscard]]
    int bignum::get_digit_by_place(const int place) const
    {
        if (place + this->get_mantissa_size() < 0 || place > this->get_size() - this->get_mantissa_size() - 1)
            return 0;
        else
            return c2i(this->exp[place + this->get_mantissa_size()]);
    }

    [[nodiscard]]
    bignum bignum::round (int wanted_mantissa_size) const
    {
        if (wanted_mantissa_size >= this->get_mantissa_size())
            return *this;
        else {
            std::vector<char> new_exp;
            for (int i = -wanted_mantissa_size; i < this->get_integer_size(); i += 1)
                new_exp.push_back(i2c(this->get_digit_by_place(i)));
            return {new_exp, wanted_mantissa_size, this->get_sign()};
        }
    }

    void bignum::print(std::ostream &os) const {
        if (this->is_zero())
            os << 0;
        else
        {
            if (this->get_sign() == -1)
                os << "-";
            for (int i = std::max(0, this->get_integer_size() - 1); i >= std::min(-this->get_mantissa_size(), 0); i -= 1)
            {
                os << this->get_digit_by_place(i);
                if (i == 0 && this->get_mantissa_size() > 0)
                    os << ".";
            }
        }
    }

    [[nodiscard]]
    bool bignum::is_zero() const
    {
        bool flag = true;
        for (int i = -this->get_mantissa_size(); i < this->get_integer_size(); i += 1)
            if (this->get_digit_by_place(i) != 0)
                flag = false;
        return flag;
    }

    bool bignum::operator==(bignum const& num) const
    {
        if (this->is_zero() && num.is_zero())
            return true;
        if (this->get_sign() != num.get_sign())
            return false;
        int max_integer_size = std::max(this->get_integer_size(), num.get_integer_size());
        int max_mantissa_size = std::max(this->get_mantissa_size(), num.get_mantissa_size());
        for (int i = -max_mantissa_size; i < max_integer_size; i += 1)
            if (this->get_digit_by_place(i) != num.get_digit_by_place(i))
                return false;
        return true;
    }

    bool bignum::operator!=(bignum const& num) const {
        return !(*this == num);
    }

    bool bignum::operator>(bignum const& num) const
    {
        if (*this == num)
            return false;

        if (this->get_sign() != num.get_sign())
            return (this->get_sign() > num.get_sign());

        int max_integer_size = std::max(this->get_integer_size(), num.get_integer_size());
        int max_mantissa_size = std::max(this->get_mantissa_size(), num.get_mantissa_size());
        for (int i = max_integer_size - 1; i >= -max_mantissa_size; i -= 1)
        {
            if (this->get_digit_by_place(i) > num.get_digit_by_place(i))
                return true;
            if (this->get_digit_by_place(i) < num.get_digit_by_place(i))
                return false;
        }
        return false;
    }

    bool bignum::operator>=(bignum const& num) const {
        return ((*this == num) || (*this > num));
    }

    bool bignum::operator<(bignum const& num) const {
        return !(*this >= num);
    }

    bool bignum::operator<=(bignum const& num) const {
        return !(*this > num);
    }

    bignum bignum::operator-() const {
        if (this->is_zero())
            return {this->get_exp(), this->get_mantissa_size(), 1};
        else
            return {this->get_exp(), this->get_mantissa_size(), this->get_sign()*-1};
    }

    [[nodiscard]]
    bignum bignum::abs() const {
        return {this->get_exp(), this->get_mantissa_size(), 1};
    }

    [[nodiscard]]
    bignum bignum::multiply_by(const bignum& num) const
    {
        if (this->is_zero() || num.is_zero())
            return bignum(0);

        std::vector<char> new_exp;
        int a, b, i, j;
        int curr_digit, next_digit = 0;
        bool begin_flag = true;
        int new_mantissa_size = this->get_mantissa_size() + num.get_mantissa_size();

        for (int ij = 0; ij < this->get_size() + num.get_size() - 1; ij += 1)
        {
            curr_digit = next_digit;

            j = std::min(ij, num.get_size() - 1);
            i = ij - j;

            while (j >= 0 && i < this->get_size()) {
                a = c2i(this->exp[i]);
                b = c2i(num.exp[j]);
                curr_digit += a * b;
                i += 1, j -= 1;
            }

            next_digit = curr_digit / NUM_SYS;
            curr_digit = curr_digit % NUM_SYS;

            if (begin_flag && curr_digit == 0)
                new_mantissa_size -= 1;
            else {
                begin_flag = false;
                new_exp.push_back(i2c(curr_digit));
            }
        }
        if (next_digit != 0)
            new_exp.push_back(i2c(next_digit));
        return {new_exp, new_mantissa_size, this->get_sign() * num.get_sign()};
    }

    bignum bignum::operator+(const bignum& num) const {
        if (this->get_sign() == num.get_sign()) {
            int max_integer_size = std::max(this->get_integer_size(), num.get_integer_size());
            int max_mantissa_size = std::max(this->get_mantissa_size(), num.get_mantissa_size());
            bool flag = true;

            std::vector<char> new_exp;
            int new_mantissa_size = max_mantissa_size;

            int a, b;
            int curr_digit, next_digit = 0;

            for (int i = -max_mantissa_size; i < max_integer_size; i += 1) {
                a = this->get_digit_by_place(i);
                b = num.get_digit_by_place(i);

                curr_digit = (a + b + next_digit) % NUM_SYS;
                next_digit = (a + b + next_digit) / NUM_SYS;

                if (!flag || curr_digit != 0) {
                    flag = false;
                    new_exp.push_back(i2c(curr_digit));
                } else
                    new_mantissa_size -= 1;
            }
            if (next_digit != 0)
                new_exp.push_back(i2c(next_digit));
            return {new_exp, new_mantissa_size, this->get_sign()};
        }
        else
        {
            if (this->abs() == num.abs())
                return bignum{0};
            if (this->get_sign() == 1 && num.get_sign() == -1) // Вычитание из большего nоложительного меньшее nоложительное
            {
                if (this->abs() > num.abs()) {
                    int max_integer_size = std::max(this->get_integer_size(), num.get_integer_size());
                    int max_mantissa_size = std::max(this->get_mantissa_size(), num.get_mantissa_size());
                    bool flag = true;

                    std::vector<char> new_exp;
                    int new_mantissa_size = max_mantissa_size;

                    int a, b;
                    int curr_digit, next_digit = 0;
                    int zeros_counter = 0;

                    for (int i = -max_mantissa_size; i < max_integer_size; i += 1){
                        a = this->get_digit_by_place(i);
                        b = -num.get_digit_by_place(i);

                        curr_digit = (a + b + next_digit + NUM_SYS) % NUM_SYS;
                        if (a + b + next_digit < 0)
                            next_digit = -1;
                        else
                            next_digit = 0;

                        if (!flag || curr_digit != 0) {
                            flag = false;
                            if (curr_digit == 0)
                                zeros_counter += 1;
                            else {
                                for (int j = 0; j < zeros_counter; j += 1)
                                    new_exp.push_back(i2c(0));
                                zeros_counter = 0;
                                new_exp.push_back(i2c(curr_digit));
                            }
                        } else
                            new_mantissa_size -= 1;
                    }
                    if (next_digit != 0)
                        new_exp.push_back(i2c(next_digit));
                    if (new_exp.empty())
                    {
                        new_exp.push_back(i2c(0));
                        new_mantissa_size = 0;
                    }
                    return {new_exp, new_mantissa_size, 1};
                }
                else
                    return (-(-num + -*this));
            }
            else
            {
                if (this->abs() < num.abs())
                    return (num + *this);
                else
                    return (-(-*this + - num));
            }
        }
    }

    bignum bignum::operator-(const bignum& num) const {
        return *this + - num;
    }

    bignum bignum::operator*(const bignum& num) const
    {
        if (this->get_size() >= 40 && num.get_size() >= 40)
        {
            int len = std::max((this->get_size() + 1)/2, (num.get_size() + 1)/2);
            bignum a = this->split(len, 1).abs();
            bignum b = this->split(len, 0).abs();
            bignum c = num.split(len, 1).abs();
            bignum d = num.split(len, 0).abs();
            bignum ac = a*c;
            bignum bd = b*d;
            int curr_sign = this->get_sign()*num.get_sign();
            int mantissa_sum = this->get_mantissa_size() + num.get_mantissa_size();
            bignum result = ((ac.shift(2*len) + ((a+b) * (c+d) - ac - bd).shift(len) + bd).shift(-mantissa_sum));
            result.sign=curr_sign;
            return result;
        }
        else
            return this->multiply_by(num);
    }

    [[nodiscard]]
    bignum bignum::divide_by(const bignum& num, int wanted_precision) const
    {
        try {
            if (num.is_zero()) {
                throw "Ошибка: Деление на ноль!";
            }
            else
            {
                bignum t_this = *this;
                bignum t_num = num.shift(this->get_integer_size()-num.get_integer_size());
                int x;

                std::vector<char> new_exp;
                std::vector<char> new_exp_reversed;
                int new_integer_size = this->get_integer_size() - num.get_integer_size() + 1;

                if (*this < t_num) {
                    new_integer_size -= 1;
                    t_num = t_num.shift(-1);
                }

                int precision = new_integer_size + std::max(std::max(this->get_mantissa_size(), num.get_mantissa_size()), wanted_precision);

                for (int i = 0; i < precision; i += 1)
                {
                    if (t_this == t_num)
                    {
                        new_exp_reversed.push_back(i2c(1));
                        break;
                    }

                    if (t_this.is_zero())
                        break;
                    x = t_this.find_closest_lower_or_equal(t_num);
                    t_this -= bignum(x) * t_num;
                    t_num = t_num.shift(-1);
                    new_exp_reversed.push_back(i2c(x));
                }
                for (int i = (int)new_exp_reversed.size() - 1; i >= 0; i -= 1)
                    new_exp.push_back(new_exp_reversed[i]);
                return {new_exp, (int)new_exp.size() - new_integer_size, this->get_sign()*num.get_sign()};
            }
        }
        catch (const char* error)
        {
            std::cout << error << std::endl;
        }
    }

    bignum bignum::operator/(const bignum& num) const{
        return this->divide_by(num);
    }

    void bignum::operator+=(const bignum& num) {
        *this = *this + num;
    }

    void bignum::operator-=(const bignum& num) {
        *this = *this - num;
    }

    void bignum::operator*=(const bignum& num) {
        *this = *this * num;
    }

    void bignum::operator/=(const bignum& num) {
        *this = *this / num;
    }

    bignum bignum::split(int len, int piece) const // Игноpиpует значение длины мантиссы и знак!
    {
        if (piece == 0)
        {
            if (len > this->get_size())
                len = this->get_size();
            std::vector<char> new_exp = this->get_exp();
            new_exp.resize(len);
            int zeros_counter = 0;
            for (int i = (int)new_exp.size() - 1; i >= 0; i -= 1)
            {
                if (c2i(new_exp[i]) == 0)
                    zeros_counter += 1;
                else
                {
                    new_exp.resize(i+1);
                    return {new_exp, 0, 1};
                }
            }
            return {new_exp, 0, 1};
        }
        else
            return this->shift(this->get_mantissa_size() - len).round(0);
    }

    [[nodiscard]]
    bignum bignum::shift(int shift_size) const // Смещение на shift_size nоpядков вnpаво, если shift_size > 0, иначе влево
    {
        if (!this->is_zero())
            return {this->get_exp(), this->get_mantissa_size() - shift_size, this->get_sign()};
        else
            return *this;
    }

    [[nodiscard]]
    int bignum::find_closest_lower_or_equal(const bignum& num) const
    {
        bignum curr_num(0);
        int left_border = 0;
        int right_border = NUM_SYS - 1;
        int temp_border;
        while (left_border != right_border)
        {
            if (right_border != left_border + 1)
                temp_border = (left_border + right_border) / 2;
            else
            {
                if (num * bignum(right_border) > *this)
                    return left_border;
                else
                    return right_border;
            }
            curr_num = num * bignum(temp_border);

            if (curr_num == *this)
                return temp_border;
            if (curr_num > *this)
                right_border = temp_border;
            else
                left_border = temp_border;
        }
        return temp_border;
    }
}