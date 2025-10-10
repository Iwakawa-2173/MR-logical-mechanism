#include "block.h"
#include "random_utils.h"
#include <cmath>

Block::Block(double p_input_a, double p_input_b, double p_state_of_block, double p_output)
    : input_a(p_input_a), input_b(p_input_b), state_of_block(p_state_of_block), output(p_output) {}

Block::Block() : input_a(0), input_b(0), state_of_block(0), output(0) {}

void Block::Block_NegTau_change_state() {
    state_of_block = std::abs(state_of_block - input_b);
}

void Block::Block_NegTau_logical_function() {
    output = std::abs(state_of_block - input_a);
}

void Block::Block_ConPir_change_state() {
    state_of_block = std::abs((state_of_block - input_a)*(state_of_block - input_b));
}

void Block::Block_ConPir_logical_function() {
    output = std::abs((state_of_block - input_a)*(state_of_block - input_b));
}

void Block::Block_RandomBernoulli_logical_function() {
    output = RandomBernoulli(input_a);
}

void Block::Block_RandomBernoulli_change_state() {
    state_of_block = RandomBernoulli(input_b);
}

void Block::Block_RandomBernoulli_logical_function_self_change() {
    output = std::abs(state_of_block - RandomBernoulli(input_a));
}

void Block::logical_function(std::string func_name) {
    if (func_name == "NegTau") {
        Block_NegTau_logical_function();
    } else if (func_name == "ConPir") {
        Block_ConPir_logical_function();
    } else if (func_name == "RndBern") {
        Block_RandomBernoulli_logical_function();
    } else if (func_name == "RndBernSC") {
        Block_RandomBernoulli_logical_function_self_change();
    }
}

void Block::change_state(std::string state_name) {
    if (state_name == "NegTau") {
        Block_NegTau_change_state();
    } else if (state_name == "ConPir") {
        Block_ConPir_change_state();
    } else if (state_name == "RndBern") {
        Block_RandomBernoulli_change_state();
    }
}
