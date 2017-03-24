#include <iostream>
#include <iomanip>
#include <assert.h>

#include "uintspy.h"
#include "algorithm.h"

int main(int argc, char *argv[])
{
    /*
    std::uint8_t input[] = {
        'a', 'b', 'c', 'd'
    };
    std::uint32_t output[4];

    md5<std::uint8_t, std::uint32_t>(input, 4, output);

    for (unsigned int i = 0; i < 4; i++)
    {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << output[i];
    }
    std::cout << std::endl;
    return 0;
    */

    const std::string input_var = "i";

    unsigned int inputs_index = 0;
    UIntSpy<8> input[] = {
        UIntSpy<8>::create_from_bits(input_var, inputs_index),
        UIntSpy<8>::create_from_bits(input_var, inputs_index),
        UIntSpy<8>::create_from_bits(input_var, inputs_index),
        UIntSpy<8>::create_from_bits(input_var, inputs_index),
    };
    builder.verify_input_length(input_var, inputs_index);

    UIntSpy<32> output[OUTPUT_SIZE];

    algorithm<UIntSpy<8>, UIntSpy<32>>(input, 4, output);

    /*
    UIntSpy<32> goal[OUTPUT_SIZE] = {
        ((0x61 >> 3) | (0x62 + 7)) + ((0x63 << 13) & ~0x64),
        0x63 + 0x64,
    };
    */

    // LSB (first digit in the md5 hex output) is in the least significant position in goal[0]
    UIntSpy<32> goal[OUTPUT_SIZE] = {
        0x4c71fce2,
        0x93ee2747,
        0xcd24f395,
        0x1f337f2e,
    };

    builder.insert_comment("Folding... ");

    Builder::Variable folded = builder.build_const(true);
    for (unsigned int i = 0; i < OUTPUT_SIZE; i++)
    {
        UIntSpy<32> equal = ~(output[i] ^ goal[i]);
        folded = builder.build_and(folded, equal.fold_and());
    }

    std::cout << builder.get_output(input_var, folded);

    return 0;
}
