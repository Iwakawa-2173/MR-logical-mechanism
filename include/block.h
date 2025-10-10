#ifndef BLOCK_H
#define BLOCK_H

#include <string>

class Block {
public:
    double input_a;
    double input_b;
    double state_of_block;
    double output;

    Block(double p_input_a, double p_input_b, double p_state_of_block, double p_output);
    Block();

    void Block_NegTau_change_state();
    void Block_NegTau_logical_function();

    void Block_ConPir_change_state();
    void Block_ConPir_logical_function();

    void Block_RandomBernoulli_logical_function();
    void Block_RandomBernoulli_change_state();
    void Block_RandomBernoulli_logical_function_self_change();

    void logical_function(std::string func_name);
    void change_state(std::string state_name);
};

#endif // BLOCK_H
