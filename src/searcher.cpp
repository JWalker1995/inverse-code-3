class ProbFunc {
public:
    static constexpr unsigned int num_inputs = 3;

    static float eval(float *inputs) {
        return inputs[0] * inputs[1] * inputs[2];
    }
};

/*
int main() {
    ProbFunc::num_inputs

    return 0;
}
*/


/*

How to make the problem convex?

*/
