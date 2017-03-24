#ifndef UINTSPY_H
#define UINTSPY_H

#include <cstdint>
#include <vector>

#include "builder.h"

template <unsigned int num_bits>
class UIntSpy
{
    template <unsigned int friend_num_bits>
    friend class UIntSpy;

public:
    UIntSpy()
    {}

    UIntSpy(std::uint_fast64_t val) {
        for (unsigned int i = 0; i < num_bits; i++) {
            bits[i] = builder.build_const(val & 0b1);
            val >>= 1;
        }
    }

    template <unsigned int other_num_bits>
    UIntSpy(const UIntSpy<other_num_bits> &other) {
        for (unsigned int i = 0; i < num_bits; i++) {
            bits[i] = i < other_num_bits ? other.bits[i] : builder.build_const(false);
        }
    }


    UIntSpy operator&(const UIntSpy<num_bits> &other) const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = builder.build_and(bits[i], other.bits[i]);
        }

        return res;
    }

    UIntSpy operator|(const UIntSpy<num_bits> &other) const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = builder.build_or(bits[i], other.bits[i]);
        }

        return res;
    }

    UIntSpy operator^(const UIntSpy<num_bits> &other) const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = builder.build_xor(bits[i], other.bits[i]);
        }

        return res;
    }

    UIntSpy operator~() const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = builder.build_not(bits[i]);
        }

        return res;
    }

    UIntSpy operator<<(unsigned int shift) const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = i < shift ? builder.build_const(false) : bits[i - shift];
        }

        return res;
    }

    UIntSpy operator>>(unsigned int shift) const {
        UIntSpy<num_bits> res;

        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = i + shift >= num_bits ? builder.build_const(false) : bits[i + shift];
        }

        return res;
    }

    UIntSpy operator+(const UIntSpy<num_bits> &other) const {
        UIntSpy<num_bits> res;

        Builder::Variable carry = builder.build_const(false);

        for (unsigned int i = 0; ; i++) {
            Builder::Variable tmp_xor_1 = builder.build_xor(bits[i], other.bits[i]);
            res.bits[i] = builder.build_xor(tmp_xor_1, carry);

            if (i == num_bits - 1) {break;}

            Builder::Variable tmp_carry_1 = builder.build_and(bits[i], other.bits[i]);
            Builder::Variable tmp_carry_2 = builder.build_and(bits[i], carry);
            Builder::Variable tmp_carry_3 = builder.build_and(other.bits[i], carry);

            Builder::Variable tmp_carry_4 = builder.build_or(tmp_carry_1, tmp_carry_2);
            carry = builder.build_or(tmp_carry_3, tmp_carry_4);
        }

        return res;
    }

    Builder::Variable fold_and() const {
        Builder::Variable res = builder.build_const(true);
        for (unsigned int i = 0; i < num_bits; i++) {
            res = builder.build_and(res, bits[i]);
        }
        return res;
    }

    static UIntSpy<num_bits> create_from_bits(const std::string &source_arr, unsigned int &begin_index) {
        UIntSpy<num_bits> res;
        for (unsigned int i = 0; i < num_bits; i++) {
            res.bits[i] = builder.build_variable(source_arr + "[" + std::to_string(begin_index) + "]");
            begin_index++;
        }
        return res;
    }

    void print() {
        builder.insert_comment("Printing variable...");
        for (unsigned int i = 0; i < num_bits; i++) {
            builder.print(bits[i]);
        }
    }

private:
    Builder::Variable bits[num_bits];
};

#endif // UINTSPY_H
