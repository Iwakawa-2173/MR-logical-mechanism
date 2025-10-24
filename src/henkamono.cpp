#include "henkamono.h"
#include <iostream>
#include <algorithm>

Henkamono::Henkamono(int p_conf1, int p_conf2, int p_count_of_blocks)
    : conf1(p_conf1), conf2(p_conf2), count_of_blocks(p_count_of_blocks) {}

Henkamono::Henkamono() : conf1(0), conf2(1), count_of_blocks(3) {}

void Henkamono::add_block(double input_a, double input_b, double state_of_block, double output) {
    Block block(input_a, input_b, state_of_block, output);
    blocks.push_back(block);
}

void Henkamono::add_blocks(int count_of_blocks) {
    for (int i = 0; i < count_of_blocks; ++i) {
        blocks.push_back(Block());
    }
}

int Henkamono::calc_next_num(int n) {
    return ((n + (count_of_blocks - 1)) % count_of_blocks);
}

int Henkamono::calc_prev_num(int n) {
    return (n % count_of_blocks) + 1;
}

void Henkamono::work_of_block(int i, std::string func_name) {
    blocks[i].logical_function(func_name);
}

void Henkamono::send_values(int i) {
    blocks[calc_next_num(i)].input_b = blocks[i].output;
    blocks[(i + 1) % count_of_blocks].input_a = blocks[i].output;
}

void Henkamono::send_change_value(int i) {
    blocks[calc_next_num(i)].input_b = blocks[i].output;
}

void Henkamono::send_processing_value(int i) {
    blocks[(i + 1) % count_of_blocks].input_a = blocks[i].output;
}

void Henkamono::change_block(int i, std::string state_name) {
    blocks[i].change_state(state_name);
}

void Henkamono::work_change(std::string func_name, std::string state_name) {
    for (int i = 0; i < count_of_blocks; i++) {
        work_of_block(i, func_name);
        send_values(i);
    }
    for (int i = 0; i < count_of_blocks; i++) {
        change_block(i, state_name);
    }
}

void Henkamono::change_work(std::string func_name, std::string state_name) {
    for (int i = 0; i < count_of_blocks; i++) {
        send_change_value(i);
    }
    for (int i = 0; i < count_of_blocks; i++) {
        change_block(i, state_name);
    }
    for (int i = 0; i < count_of_blocks; i++) {
        work_of_block(i, func_name);
        send_values(i);
    }
}

void Henkamono::work_and_change(std::string func_name, std::string state_name) {
    for (int i = 0; i < count_of_blocks; i++) {
        work_of_block(i, func_name);
        send_values(i);
        change_block(calc_next_num(i), state_name);
    }
}

void Henkamono::new_print_state() {
    // пример вывода состояния
    for (const auto& block : blocks) {
        std::cerr << block.input_a << " " << block.state_of_block << " " << block.output << std::endl;
    }
}

void Henkamono::classic_print_state() {
    if (!blocks.empty()) {
        std::cerr << blocks[0].input_a << " ";
        for (const auto& block : blocks) {
            std::cerr << block.state_of_block << " " << block.output << " ";
        }
        std::cerr << std::endl;
    }
}

void Henkamono::init_blocks(std::vector<double> init_state) {
	int num_of_pos = 0;
	blocks[0].input_a = init_state[0];
	for (int i = 0; i < blocks.size(); i++) {
		num_of_pos = 1 + i*2;
		blocks[i].state_of_block = init_state[num_of_pos];
		blocks[i].output = init_state[num_of_pos+1];
	}
}

int Henkamono::work(std::string func_name, std::string state_name, bool print) {
    std::vector<std::vector<double>> history;
    int iters = 0;

    while (true) {
        std::vector<double> currentState;
        currentState.push_back(blocks[0].input_a);
        for (const auto& block : blocks) {
            currentState.push_back(block.state_of_block);
            currentState.push_back(block.output);
        }

        auto it = std::find_if(history.begin(), history.end(),
            [&](const std::vector<double>& state) {
                return state == currentState;
            });

        if (print) {
            classic_print_state();
        }

        if ((conf1 == 0) && (conf2 == 0)) {
            // no work
        } else if ((conf1 == 0) && (conf2 == 1)) {
            work_change(func_name, state_name);
        } else if ((conf1 == 1) && (conf2 == 0)) {
            change_work(func_name, state_name);
        } else if ((conf1 == 1) && (conf2 == 1)) {
            work_and_change(func_name, state_name);
        }

        iters++;
        if (it != history.end()) break;
        history.push_back(currentState);
    }
	henkamonoOutput = currentState.back();
    return iters;
}
