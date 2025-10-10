#ifndef HENKAMONO_H
#define HENKAMONO_H

#include <vector>
#include <string>
#include "block.h"

class Henkamono {
public:
    int count_of_blocks;
    int conf1, conf2;
    std::vector<Block> blocks;

    Henkamono(int p_conf1, int p_conf2, int p_count_of_blocks);
    Henkamono();

    void add_block(double input_a, double input_b, double state_of_block, double output);
    void add_blocks(int count_of_blocks);
    void init_blocks(std::vector<double> init_state);

    int calc_next_num(int n);
    int calc_prev_num(int n);

    void work_of_block(int i, std::string func_name);
    void send_values(int i);
    void send_change_value(int i);
    void send_processing_value(int i);

    void change_block(int i, std::string state_name);

    void work_change(std::string func_name, std::string state_name);
    void change_work(std::string func_name, std::string state_name);
    void work_and_change(std::string func_name, std::string state_name);

    void new_print_state();
    void classic_print_state();

    int work(std::string func_name, std::string state_name, bool print, std::vector<std::vector<double>>& all_worked_numbers);
};

#endif // HENKAMONO_H
