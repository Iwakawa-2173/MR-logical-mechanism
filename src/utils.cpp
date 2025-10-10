#include "utils.h"
#include <algorithm>
#include <map>
#include "henkamono.h" // нужен для test_function

double CalcM(std::vector<double> v) {
    size_t size = v.size();
    if (size == 0)
        return 0;  // Undefined
    else {
        std::sort(v.begin(), v.end());
        if (size % 2 == 0)
            return (v[size / 2 - 1] + v[size / 2]) / 2;
        else
            return v[size / 2];
    }
}

int test_function(double input, double f1, double f2, double f3, int conf1, int conf2, double o1, double o2, double o3, bool print) {
    std::map<std::string, double> params; int count_of_blocks = 3;
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
    int total = 1 << n; // 2^n
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
        if (i + 1 < v.size()) {os << " ";}
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
