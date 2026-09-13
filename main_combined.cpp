#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

// ============================================================
// 1. random_utils.h + random_utils.cpp
// ============================================================
double powerab(double a, int b) {
    return (b == 0) ? 1.0 : (b < 0 ? 1.0 / powerab(a, -b) : a * powerab(a, b - 1));
}

int factor(int a) {
    return (a == 0) ? 1 : a * factor(a-1);
}

double expn(double n) {
    double result = 0;
    for (int i = 0; i <= 17; i++) {
        result += powerab(n, i) / double(factor(i));
    }
    return result;
}

double RandomUniform(double min, double max) {
    double random = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    return min + random * (max - min);
}

double RandomBernoulli(double p) {
    return RandomUniform(0.0, 1.0) < p ? 1 : 0;
}

double RandomPoisson(double lambda) {
    double L = expn(-lambda);
    int k = 0;
    double p = 1.0;
    do {
        k++;
        p *= RandomUniform(0.0, 1.0);
    } while (p > L);
    return k-1;
}

double RandomNormal(double mean, double stddev) {
    double pi = 3.14159265358979323846;
    double u1 = RandomUniform(0.0, 1.0);
    double u2 = RandomUniform(0.0, 1.0);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * pi * u2);
    return z0 * stddev + mean;
}

// ============================================================
// 2. block.h + block.cpp
// ============================================================
class Block {
public:
    double input_a;
    double input_b;
    double state_of_block;
    double output;

    Block(double p_input_a, double p_input_b, double p_state_of_block, double p_output)
        : input_a(p_input_a), input_b(p_input_b), state_of_block(p_state_of_block), output(p_output) {}

    Block() : input_a(0), input_b(0), state_of_block(0), output(0) {}

    void Block_NegTau_change_state() {
        state_of_block = std::abs(state_of_block - input_b);
    }

    void Block_NegTau_logical_function() {
        output = std::abs(state_of_block - input_a);
    }

    void Block_ConPir_change_state() {
        state_of_block = std::abs((state_of_block - input_a)*(state_of_block - input_b));
    }

    void Block_ConPir_logical_function() {
        output = std::abs((state_of_block - input_a)*(state_of_block - input_b));
    }

    void Block_RandomBernoulli_logical_function() {
        output = RandomBernoulli(input_a);
    }

    void Block_RandomBernoulli_change_state() {
        state_of_block = RandomBernoulli(input_b);
    }

    void Block_RandomBernoulli_logical_function_self_change() {
        output = std::abs(state_of_block - RandomBernoulli(input_a));
    }

    void logical_function(std::string func_name) {
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

    void change_state(std::string state_name) {
        if (state_name == "NegTau") {
            Block_NegTau_change_state();
        } else if (state_name == "ConPir") {
            Block_ConPir_change_state();
        } else if (state_name == "RndBern") {
            Block_RandomBernoulli_change_state();
        }
    }
};

// ============================================================
// 3. henkamono.h + henkamono.cpp
// ============================================================
class Henkamono {
public:
    int count_of_blocks;
    int conf1, conf2;
    double henkamonoOutput;
    std::vector<Block> blocks;

    Henkamono(int p_conf1, int p_conf2, int p_count_of_blocks)
        : conf1(p_conf1), conf2(p_conf2), count_of_blocks(p_count_of_blocks) {}

    Henkamono() : conf1(0), conf2(1), count_of_blocks(3) {}

    void add_block(double input_a, double input_b, double state_of_block, double output) {
        Block block(input_a, input_b, state_of_block, output);
        blocks.push_back(block);
    }

    void add_blocks(int count_of_blocks) {
        for (int i = 0; i < count_of_blocks; ++i) {
            blocks.push_back(Block());
        }
    }

    int calc_next_num(int n) {
        return ((n + (count_of_blocks - 1)) % count_of_blocks);
    }

    int calc_prev_num(int n) {
        return (n % count_of_blocks) + 1;
    }

    void work_of_block(int i, std::string func_name) {
        blocks[i].logical_function(func_name);
    }

    void send_values(int i) {
        blocks[calc_next_num(i)].input_b = blocks[i].output;
        blocks[(i + 1) % count_of_blocks].input_a = blocks[i].output;
    }

    void send_change_value(int i) {
        blocks[calc_next_num(i)].input_b = blocks[i].output;
    }

    void send_processing_value(int i) {
        blocks[(i + 1) % count_of_blocks].input_a = blocks[i].output;
    }

    void change_block(int i, std::string state_name) {
        blocks[i].change_state(state_name);
    }

    void work_change(std::string func_name, std::string state_name) {
        for (int i = 0; i < count_of_blocks; i++) {
            work_of_block(i, func_name);
            send_values(i);
        }
        for (int i = 0; i < count_of_blocks; i++) {
            change_block(i, state_name);
        }
    }

    void change_work(std::string func_name, std::string state_name) {
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

    void work_and_change(std::string func_name, std::string state_name) {
        for (int i = 0; i < count_of_blocks; i++) {
            work_of_block(i, func_name);
            send_values(i);
            change_block(calc_next_num(i), state_name);
        }
    }

    void new_print_state() {
        for (const auto& block : blocks) {
            std::cerr << block.input_a << " " << block.state_of_block << " " << block.output << std::endl;
        }
    }

    void classic_print_state() {
        if (!blocks.empty()) {
            std::cerr << blocks[0].input_a << " ";
            for (const auto& block : blocks) {
                std::cerr << block.state_of_block << " " << block.output << " ";
            }
            std::cerr << std::endl;
        }
    }

    void init_blocks(std::vector<double> init_state) {
        int num_of_pos = 0;
        blocks[0].input_a = init_state[0];
        for (int i = 0; i < blocks.size(); i++) {
            num_of_pos = 1 + i*2;
            blocks[i].state_of_block = init_state[num_of_pos];
            blocks[i].output = init_state[num_of_pos+1];
        }
    }

    int work(std::string func_name, std::string state_name, bool print = false, std::vector<std::vector<double>> all_numbers = {}) {
        std::vector<std::vector<double>> history;
        int iters = 0;
        std::vector<double> currentState;  // ← ОБЪЯВЛЯЕМ ЗДЕСЬ

        while (true) {
            currentState.clear();
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
};

// ============================================================
// 4. utils.h + utils.cpp
// ============================================================
double CalcM(std::vector<double> v) {
    size_t size = v.size();
    if (size == 0)
        return 0;
    else {
        std::sort(v.begin(), v.end());
        if (size % 2 == 0)
            return (v[size / 2 - 1] + v[size / 2]) / 2;
        else
            return v[size / 2];
    }
}

int test_function(double input, double f1, double f2, double f3, int conf1, int conf2, double o1, double o2, double o3, bool print) {
    std::map<std::string, double> params;
    int count_of_blocks = 3;
    std::vector<std::vector<double>> all_numbers;
    std::string func_name = "RndBern";
    std::string state_name = "RndBern";

    params["input_a1"] = input; params["input_b1"] = 0; params["state1"] = f1; params["output1"] = o1;
    params["input_a2"] = 0; params["input_b2"] = 0; params["state2"] = f2; params["output2"] = o2;
    params["input_a3"] = 0; params["input_b3"] = 0; params["state3"] = f3; params["output3"] = o3;

    Henkamono henkamono(conf1, conf2, count_of_blocks);
    henkamono.add_block(params["input_a1"], params["input_b1"], params["state1"], params["output1"]);
    henkamono.add_block(params["input_a2"], params["input_b2"], params["state2"], params["output2"]);
    henkamono.add_block(params["input_a3"], params["input_b3"], params["state3"], params["output3"]);

    return henkamono.work(func_name, state_name, print, all_numbers);
}

std::vector<std::vector<double>> generateAllBinaryVectors(int count_of_blocks) {
    int n = 1 + count_of_blocks*2;
    std::vector<std::vector<double>> out;
    int total = 1 << n;
    out.reserve(total);
    for (int mask = 0; mask < total; ++mask) {
        std::vector<double> vec(n);
        for (int i = 0; i < n; ++i) {
            vec[i] = (mask >> i) & 1;
        }
        std::reverse(vec.begin(), vec.end());
        out.push_back(vec);
    }
    return out;
}

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i + 1 < v.size()) { os << " "; }
    }
    return os;
}

int binaryToDecimal(std::vector<double> digits) {
    int result = 0;
    for (int i = 0; i < digits.size(); ++i) {
        result <<= 1;
        result |= static_cast<int>(digits[i]);
    }
    return result;
}

void not_included_nums(int count_of_blocks, std::string notation) {
    std::vector<std::vector<double>> all_numbers;
    std::vector<std::vector<double>> all_worked_numbers;

    all_numbers = generateAllBinaryVectors(count_of_blocks);

    std::string func_name = "NegTau";
    std::string state_name = "NegTau";

    for (int conf1 = 0; conf1 < 2; conf1++) {
        for (int conf2 = 0; conf2 < 2; conf2++) {
            if ((conf1 == 0) && (conf2 == 0)) {
                continue;
            }
            for (int i = 0; i < all_numbers.size(); i++) {
                Henkamono henkamono(conf1, conf2, count_of_blocks);
                henkamono.add_blocks(count_of_blocks);
                henkamono.init_blocks(all_numbers[i]);
                henkamono.work(func_name, state_name, false, all_worked_numbers);
            }
        }
    }

    for (int i = 0; i < all_numbers.size(); i++) {
        auto it = std::find_if(all_worked_numbers.begin(), all_worked_numbers.end(), [&](const std::vector<double>& number) {
            return number == all_numbers[i];
        });
        if (it != all_worked_numbers.end()) {
            if (notation == "binary") {
                std::cerr << all_numbers[i] << std::endl;
            } else if (notation == "decimal") {
                std::cerr << binaryToDecimal(all_numbers[i]) << std::endl;
            }
        }
    }
}

void print_all_states(int count_of_blocks) {
    std::vector<std::vector<double>> all_numbers;
    std::vector<std::vector<double>> all_worked_numbers;

    all_numbers = generateAllBinaryVectors(count_of_blocks);

    std::string func_name = "RndBern";
    std::string state_name = "RndBernSC";

    int steps = 0;
    int avg = 0;
    int iter = 0;
    for (int conf1 = 0; conf1 < 2; conf1++) {
        for (int conf2 = 0; conf2 < 2; conf2++) {
            if ((conf1 == 0) && (conf2 == 0)) {
                continue;
            }
            for (int i = 0; i < all_numbers.size(); i++) {
                Henkamono henkamono(conf1, conf2, count_of_blocks);
                henkamono.add_blocks(count_of_blocks);
                henkamono.init_blocks(all_numbers[i]);
                std::cerr << "A B f*1 f f*2 Fi f*3" << std::endl;
                steps = henkamono.work(func_name, state_name, true, all_worked_numbers);
                std::cerr << "Steps = " << steps << std::endl;
                avg += steps;
                iter++;
            }
        }
    }
    std::cerr << "Avg = " << avg / iter << std::endl;
}

std::vector<double> multiplyMatrixByVector(std::vector<std::vector<double>> matrix, std::vector<double> vec) {
    std::vector<double> result;
    double element = 0;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            element += matrix[i][j] * vec[j];
        }
        result.push_back(element);
        element = 0;
    }
    return result;
}

// ============================================================
// 5. main.cpp
// ============================================================
int main() {
    clock_t tStart = clock();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
	/*
    std::cerr << "=== Testing not_included_nums ===" << std::endl;
    not_included_nums(5, "decimal");
	*/
	// === НАСТРОЙКА ЭКСПЕРИМЕНТА ===
    int conf1 = 0;          // 0 или 1
    int conf2 = 1;          // 0 или 1
    int blocks = 3;         // количество блоков
    std::string func = "NegTau";     // NegTau, ConPir, RndBern, RndBernSC
    std::string state = "NegTau";    // NegTau, ConPir, RndBern
    bool print = true;      // выводить состояния
    
    // Начальное состояние: [input_a, state0, out0, state1, out1, state2, out2, ...]
    std::vector<double> init = {0.5, 0.2, 0.8, 0.3, 0.7, 0.1, 0.9};
    
    // === ЗАПУСК ===
    Henkamono system(conf1, conf2, blocks);
    system.add_blocks(blocks);
    system.init_blocks(init);
    
    int steps = system.work(func, state, print);
    std::cout << "Шагов до цикла: " << steps << std::endl;
    double time_taken = (double)(clock() - tStart) / CLOCKS_PER_SEC;
    std::cerr << "Time = " << time_taken << " seconds" << std::endl;

    return 0;
}