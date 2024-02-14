#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#define NUM_SYS 10 // Система счисления (nока что значения больше десяти будут nлохо выводиться)

// G g H h '

int c2i(char c) {
    return (int) (c - '0');
}

char i2c(int i) {
    return (char) (i + '0');
}


class bignum {
public:
    explicit bignum(const std::string &s) {
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

    explicit bignum(const int a) {
        if (a < 0)
            this->sign = -1;
        else
            this->sign = 1;
        this->mantissa_size = 0;
        int temp_a = a;
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

    friend std::ostream &operator<<(std::ostream &os, const bignum &a);

    bignum(const std::vector<char>& d_exp, const int d_mantissa_size, const int d_sign) {
        this->mantissa_size = d_mantissa_size;
        this->sign = d_sign;
        this->exp = d_exp;
    }

    [[nodiscard]]
    std::vector<char> get_exp() const {
        return this->exp;
    }

    [[nodiscard]]
    int get_sign() const {
        return this->sign;
    }

    [[nodiscard]]
    int get_size() const {
        return (int) this->exp.size();
    }

    [[nodiscard]]
    int get_mantissa_size() const {
        return this->mantissa_size;
    }

    [[nodiscard]]
    int get_integer_size() const {
        return this->get_size() - this->get_mantissa_size();
    }

    [[nodiscard]]
    int get_digit_by_place(const int place) const
    {
        if (place + this->get_mantissa_size() < 0 || place > this->get_size() - this->get_mantissa_size() - 1)
            return 0;
        else
            return c2i(this->exp[place + this->get_mantissa_size()]);
    }

    [[nodiscard]]
    int get_digit_by_index(const int idx) const
    {
        if (idx < 0 || idx >= this->get_size()) {
            std::cout << "Trying to get digit from out of vector borders!" << std::endl;
            exit(-1);
        }
        else
            return c2i(this->exp[idx]);
    }

    [[nodiscard]]
    bignum round (int wanted_mantissa_size = 0) const
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

    void print() const {
        if (this->is_zero())
            std::cout << 0;
        else
        {
            if (this->get_sign() == -1)
                std::cout << "-";
            for (int i = std::max(0, this->get_integer_size() - 1); i >= std::min(-this->get_mantissa_size(), 0); i -= 1)
            {
                std::cout << this->get_digit_by_place(i);
                if (i == 0 && this->get_mantissa_size() > 0)
                    std::cout << ".";
            }
        }
    }

    [[nodiscard]]
    bool is_zero() const
    {
        bool flag = true;
        for (int i = -this->get_mantissa_size(); i < this->get_integer_size(); i += 1)
            if (this->get_digit_by_place(i) != 0)
                flag = false;
        return flag;
    }

    bool operator==(bignum const& num) const
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

    bool operator!=(bignum const& num) const {
        return !(*this == num);
    }

    bool operator>(bignum const& num) const
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

    bool operator>=(bignum const& num) const {
        return ((*this == num) || (*this > num));
    }

    bool operator<(bignum const& num) const {
        return !(*this >= num);
    }

    bool operator<=(bignum const& num) const {
        return !(*this > num);
    }

    bignum operator-() const {
        if (this->is_zero())
            return {this->get_exp(), this->get_mantissa_size(), 1};
        else
            return {this->get_exp(), this->get_mantissa_size(), this->get_sign()*-1};
    }

    [[nodiscard]]
    bignum abs() const {
        return {this->get_exp(), this->get_mantissa_size(), 1};
    }

    [[nodiscard]]
    bignum shift(int shift_size) const // Смещение на shift_size nоpядков вnpаво, если shift_size > 0, иначе влево
    {
        if (!this->is_zero())
            return {this->get_exp(), this->get_mantissa_size() - shift_size, this->get_sign()};
        else
            return *this;
    }

    [[nodiscard]]
    int find_closest_lower_or_equal(const bignum& num) const
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

    [[nodiscard]]
    bignum regular_multiplication(const bignum& num) const
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

    [[nodiscard]]
    bignum split(int piece) const // Игноpиpует значение длины мантиссы!
    {
        if (piece == 0)
        {
            int len = (this->get_size() + 1) / 2;
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
        {
            int len = this->get_size() / 2;
            int zeros_counter = 0;
            if (len != 0)
            {
                for (int i = this->get_size() - len; i < this->get_size(); i += 1) {
                    if (this->get_digit_by_index(i) == 0)
                        zeros_counter += 1;
                    else {
                        return (this->round(-(this->get_size() - len + zeros_counter))).shift(
                                -(this->get_size() - len + zeros_counter));
                    }
                }
                return (this->round(-(this->get_size() - len))).shift(-(this->get_size() - len));
            }
            else
                return bignum(0);
        }
    }

    bignum operator+(const bignum& num) const {
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
                    return (-num + *this);
                else
                    return (-(-*this + - num));
            }
        }
    }

    bignum operator-(const bignum& num) const {
        return *this + - num;
    }

    bignum operator*(const bignum& num) const
    {
        if (this->get_size() >= 20 && num.get_size() >= 20)
        {
            int len = (this->get_size() + 1) / 2;
            bignum a = this->split(1).abs();
            bignum b = this->split(0).abs();
            bignum c = num.split(1).abs();
            bignum d = num.split(0).abs();
            bignum ac = a*c;
            bignum bd = b*d;
            int curr_sign = this->get_sign()*num.get_sign();
            int mantissa_sum = this->get_mantissa_size() + num.get_mantissa_size();
            return bignum(curr_sign) * (ac.shift(2*len) + ((a+b) * (c+d) - ac - bd).shift(len) + bd).shift(-mantissa_sum);
        }
        else
            return this->regular_multiplication(num);
    }

    [[nodiscard]]
    bignum division(const bignum& num, int wanted_precision = 10) const
    {
        if (num.is_zero()) {
            std::cout << "Division by zero! Dividend: " << *this << std::endl;
            exit(-1);
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

    bignum operator/(const bignum& num) const{
        return this->division(num);
    }

    bignum operator+=(const bignum& num) {
        *this = *this + num;
        return *this;
    }

    bignum operator-=(const bignum& num) {
        *this = *this - num;
        return *this;
    }

    bignum operator*=(const bignum& num) {
        *this = *this * num;
        return *this;
    }

private:
    std::vector<char> exp;
    int mantissa_size;
    int sign;
};

bignum operator "" _bn(const char *s) {
    return bignum(s);
}

std::ostream &operator<<(std::ostream &os, const bignum &a) {
    a.print();
    return os;
}

bignum pi(int precision)
{
    bignum curr_pi = 0_bn;
    bignum a = 1_bn / 5_bn, b = (1_bn).division(239_bn, precision);
    bignum a2 = a * a, b2 = b * b;
    bignum curr_a = a, curr_b = b;
    for (int i = 1; i < 150; i += 2)
    {
        if ((i-1)/2%2==0)
            curr_pi += 16_bn * curr_a.division(bignum(i), precision) - 4_bn * curr_b.division(bignum(i), precision);
        else
            curr_pi -= 16_bn * curr_a.division(bignum(i), precision) - 4_bn * curr_b.division(bignum(i), precision);
        curr_a *= a2, curr_b *= b2;
    }
    return curr_pi;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    //std::cout << pi(105) << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << "ms" << std::endl;
}