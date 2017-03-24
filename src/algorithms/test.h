#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <algorithm>

#define MAX_NUM_VARIABLES 64
#define OUTPUT_SIZE 2

template <typename UInt8, typename UInt32>
void algorithm(UInt8 *input, unsigned int input_size, UInt32 *output)
{
    output[0] = ((UInt32(input[0]) >> 3) | UInt32(input[1] + 7)) + ((UInt32(input[2]) << 13) & ~UInt32(input[3]));
    output[1] = input[2] + input[3];
}

#endif // TEST_H
