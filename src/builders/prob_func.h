#ifndef PROB_FUNC_H
#define PROB_FUNC_H

#include <assert.h>
#include <string>

class Builder {
public:
    class Variable {
    public:
        static Variable make_constant(bool val) {
            Variable res;
            res.constant = val;
            assert(res.is_constant());
            return res;
        }

        static Variable make_variable(const std::string &name) {
            Variable res;
            res.variable = name;
            assert(res.is_variable());
            return res;
        }

        static Variable make_inverted(Variable var) {
            var.invert = !var.invert;
            return var;
        }

        bool is_constant() const {return variable.empty();}
        bool is_variable() const {return !variable.empty();}

        bool get_constant() const {
            assert(is_constant());
            return constant ^ invert;
        }

        std::string get_variable() const {
            assert(is_variable());
            if (invert) {
#ifdef OUTPUT_LANGUAGE_C
                return "(1.0f-" + variable + ")";
#endif
#ifdef OUTPUT_LANGUAGE_JS
                return "(1.0-" + variable + ")";
#endif
            } else {
                return variable;
            }
        }

    private:
        std::string variable;
        bool constant;
        bool invert = false;
    };

    Variable build_const(bool val) {
        return Variable::make_constant(val);
    }
    Variable build_variable(const std::string &var_name) {
        return Variable::make_variable(var_name);
    }

    Variable build_not(const Variable &val) {
        return Variable::make_inverted(val);
    }

    Variable build_and(const Variable &left, const Variable &right) {
        if (left.is_constant()) {
            return left.get_constant() ? right : build_const(false);
        } else if (right.is_constant()) {
            return right.get_constant() ? left : build_const(false);
        } else {
            return build_expr(left.get_variable() + "*" + right.get_variable());
        }
    }

    Variable build_or(const Variable &left, const Variable &right) {
        if (left.is_constant()) {
            return left.get_constant() ? build_const(true) : right;
        } else if (right.is_constant()) {
            return right.get_constant() ? build_const(true) : left;
        } else {
            return build_expr(left.get_variable() + "+" + right.get_variable() + "-" + left.get_variable() + "*" + right.get_variable());
        }
    }

    Variable build_xor(const Variable &left, const Variable &right) {
        if (left.is_constant()) {
            return left.get_constant() ? build_not(right) : right;
        } else if (right.is_constant()) {
            return right.get_constant() ? build_not(left) : left;
        } else {
#ifdef OUTPUT_LANGUAGE_C
            return build_expr(left.get_variable() + "+" + right.get_variable() + "-2.0f*" + left.get_variable() + "*" + right.get_variable());
#endif
#ifdef OUTPUT_LANGUAGE_JS
            return build_expr(left.get_variable() + "+" + right.get_variable() + "-2.0*" + left.get_variable() + "*" + right.get_variable());
#endif
        }
    }

    void insert_comment(const std::string &msg) {
        body += "// " + msg + "\n";
    }

    void print(const Variable &var) {
        body += "print(" + var.get_variable() + ");\n";
    }

    void verify_input_length(const std::string &source_arr, unsigned int size) {
#ifdef OUTPUT_LANGUAGE_C
        (void) source_arr;
        (void) size;
#endif
#ifdef OUTPUT_LANGUAGE_JS
        body += "if (" + source_arr + ".length !== " + std::to_string(size) + ") {\n";
        body += "throw new Error('Unexpected input length ' + " + source_arr + ".length + ', expected " + std::to_string(size) + "');\n";
        body += "}\n";
#endif
    }

    std::string get_output(const std::string &input_arr, const Variable &result) const {
        std::string res;
#ifdef OUTPUT_LANGUAGE_C
        res += "float prob_func(float *" + input_arr + ") {\n";
        res += body;
        res += "return " + result.get_variable() + ";\n";
        res += "}\n";
#endif
#ifdef OUTPUT_LANGUAGE_JS
        res += "let prob_func = function(" + input_arr + ") {\n";
        res += body;
        res += "return " + result.get_variable() + ";\n";
        res += "};\n";
#endif
        return res;
    }

private:
    Variable build_expr(const std::string &expr) {
        Variable var = Variable::make_variable(unique_ident(next_var_index++));
#ifdef OUTPUT_LANGUAGE_C
        body.append("float ").append(var.get_variable()).append("=").append(expr).append(";\n");
#endif
#ifdef OUTPUT_LANGUAGE_JS
        body.append("let ").append(var.get_variable()).append("=").append(expr).append(";\n");
#endif
        return var;
    }

    std::string unique_ident(unsigned int index) {
        const char init_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const unsigned int init_chars_size = 52;
        assert(sizeof(init_chars) - 1 == init_chars_size);

        const char cont_chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const unsigned int cont_chars_size = 62;
        assert(sizeof(cont_chars) - 1 == cont_chars_size);

        std::string res = "_";
        res.push_back(init_chars[index % init_chars_size]);
        index /= init_chars_size;

        while (index) {
            res.push_back(cont_chars[index % cont_chars_size]);
            index /= cont_chars_size;
        }

        return res;
    }

    std::string body;
    unsigned int next_var_index = 0;
};

#endif // PROB_FUNC_H
