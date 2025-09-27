#include <cstdlib>  // Для функции rand() и srand()
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <array>

using namespace std;

// Функции логического отрицания и тавтологии
int _negation(int input) {return !input;}

int tautology(int input) {return input;}

// Функция для преобразования ленты в число
int binaryArrayToInt(std::array<int, 7> args) {

    int result = 0;
    
    // Преобразование двоичной записи в целое число
    for (int i = 0; i < 7; ++i) {
        // Сдвигаем результат влево на 1 (умножаем на 2)
        result <<= 1;
        // Добавляем текущий бит (0 или 1)
        result |= args[i];
    }

    return result;
}

int henkamono(int input, int f1, int f2, int f3, int cyc, int conf1, int conf2, int prob, int o1, int o2, int o3, int print, vector<vector<int>>& all_numbers) {
	int output1; 
	int output2; 
	int output3;
	int negation_value;
	int not_neg_v;
	int iters = 0;
	std::array<int, 7> args = {};

    /*
	00 - копирует из одной ячейки в другую.
	01 - сначала прогоняются данные, потом процессы меняются.
	10 - сначала меняются процессы, потом прогоняются данные.
	11 - сначала идёт процесс, потом изменение, потом данные во второй...
	*/

	if (prob == 100) {
		negation_value = 1;
	} else if (prob == 0) {
		negation_value = 0;
	}
	not_neg_v = !negation_value;
	
	// Изначальные функции
    vector<int> functions = {f1, f2, f3};
	
    vector<vector<int>> history; // Для хранения истории состояний
	output1 = o1;
	output2 = o2;
	output3 = o3;
	if ((conf1 != 0) && (conf2 != 0)) {
		;
	}
	
    while (true) {
		
        // Запоминаем состояние функций и входного значения
        vector<int> currentState = {input, functions[0], output1, functions[1], output2, functions[2], output3};
		
		if (iters > 0) {
			all_numbers.push_back(currentState);
		}

        // Проверяем, было ли это состояние ранее
        auto it = std::find_if(history.begin(), history.end(), [&](const vector<int>& state) {return state == currentState;});
		args = {input, functions[0], output1, functions[1], output2, functions[2], output3};
		
		if (print == 1) {
			std::cerr << iters << " " << input << " " << functions[0] << " " << output1 << " " << functions[1] << " " << output2 << " " << functions[2] << " " << output3 << std::endl;
		}
		
		if (print == 2) {
			std::cerr << input << functions[0] << output1 << functions[1] << output2 << functions[2] << output3 << std::endl;
		}

		if (prob > 0 && prob < 100) {
			// Генерация случайного числа от 0 до 99
			int randomNumber = std::rand() % 100;
			if (randomNumber < prob) {negation_value = 1;} 
			else {negation_value = 0;}
			not_neg_v = !negation_value;
		}
		
		if ((conf1 == 0) && (conf2 == 0)) {
			;
		} else if ((conf1 == 0) && (conf2 == 1)) {
			// Запоминаем выходы функций
			output1 = (functions[0] == negation_value) ? _negation(input) : tautology(input);
			output2 = (functions[1] == negation_value) ? _negation(output1) : tautology(output1);
			output3 = (functions[2] == negation_value) ? _negation(output2) : tautology(output2);

			// Изменяем процессы на основе выходных значений
			if (output3 == negation_value) {
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output2 == negation_value) {
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output1 == negation_value) {
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
		} else if ((conf1 == 1) && (conf2 == 0)) {
			if (output3 == negation_value) {
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output2 == negation_value) {
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			if (output1 == negation_value) {
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			output1 = (functions[0] == negation_value) ? _negation(input) : tautology(input);
			output2 = (functions[1] == negation_value) ? _negation(output1) : tautology(output1);
			output3 = (functions[2] == negation_value) ? _negation(output2) : tautology(output2);
		} else if ((conf1 == 1) && (conf2 == 1)) {			
			output1 = (functions[0] == negation_value) ? _negation(input) : tautology(input);
			if (output1 == negation_value) {
				functions[2] = (functions[2] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			
			output2 = (functions[1] == negation_value) ? _negation(output1) : tautology(output1);
			if (output2 == negation_value) {
				functions[0] = (functions[0] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
			
			output3 = (functions[2] == negation_value) ? _negation(output2) : tautology(output2);
			if (output3 == negation_value) {
				functions[1] = (functions[1] == not_neg_v) ? negation_value : not_neg_v; // Тавтология <-> отрицание
			}
		}
		input = output3;
		
		iters++;

        if (it != history.end()) {break;}

        history.push_back(currentState); // Запоминаем текущее состояние

        // Устанавливаем новое входное значение для следующей итерации
        if (cyc == 1) {input = output3;}
    }
	
	if (print == 1) {
		// std::cerr << "Iters: " << iters << std::endl;
	}
	
	return iters;
	
}

double CalcM(vector<int> v) {
    size_t size = v.size();
    if (size == 0)
        return 0;  // Undefined
    else
    {
        sort(v.begin(), v.end());
        if (size % 2 == 0)
            return (v[size / 2 - 1] + v[size / 2]) / 2;
        else
            return v[size / 2];
    }
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
	if (i + 1 < v.size()) {os << " ";}
    }
    return os;
}

int main() {
	
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	vector<int> values;
	vector<int> probs = {0, 100};
	vector<vector<int>> all_numbers;
	std::array<int, 7> digits = {};

	int odd, prob;
	int value = 0;
	float sum = 0;
	float all = 0;
	int cyc = 1;
	int max = 0;
	//int conf1 = 0; int conf2 = 1;
	// input, f1, f2, f3, cyc, conf1, conf2, prob, o1, o2, o3, print
	//henkamono(0, 0, 0, 0, 1, conf1, conf2, 100, 0, 0, 1, 1);
	int iters = 0;
	//int conf1 = (mask >> 9) & 1;
	//int conf2 = (mask >> 8) & 1;
	for (int conf1 = 0; conf1 < 2; conf1++) {
		for (int conf2 = 0; conf2 < 2; conf2++) {
			for (int mask = 0; mask < (1 << 7); mask++) {
				int input = (mask >> 6) & 1;
				int f1 = (mask >> 5) & 1;
				int o1 = (mask >> 4) & 1;
				int f2 = (mask >> 3) & 1;
				int o2 = (mask >> 2) & 1;
				int f3 = (mask >> 1) & 1;
				int o3 = mask & 1;
				if ((conf1 == 0) && (conf2 == 0)){
					continue;
				} else {
					//std::cerr << "A B f*1 f f*2 Fi f*3" << std::endl;
					henkamono(input, f1, f2, f3, 1, conf1, conf2, 100, o1, o2, o3, 0, all_numbers);
				}
				
			}
		}
	}


	// std::cerr << "Max:" << max << std::endl;
	
	int n = 7;
    int total = 1 << n; // 2^7 = 128

    for (int mask = 0; mask < total; ++mask) {
        vector<int> digits(n);
        for (int i = 0; i < n; ++i) {
            digits[i] = (mask >> i) & 1;  // Получаем i-й бит из mask
        }
		std::reverse(digits.begin(), digits.end());

        auto it = find_if(all_numbers.begin(), all_numbers.end(), [&](const vector<int>& number) {
            return number == digits;
        });

        if (it != all_numbers.end()) {
            array<int, 7> arr;
            copy(digits.begin(), digits.end(), arr.begin());

            // Вывод результата
            for (int d = 0; d < n; ++d) {
                cerr << arr[d] << (d < n - 1 ? " " : "");
            }
            //cerr << " = " << binaryArrayToInt(arr) << endl;
			cerr << endl;
        }
    }
	
	// vector<int> probs = {0, 100};
	
	//double max;
	//int conf1 = 1; int conf2 = 1; double count_of_iters = 0; int iters = 0;
	// input, f1, f2, f3, conf1, conf2, o1, o2, o3, print
	//count_of_iters = test_function(0.5, 0.5, 0, 0.5, conf1, conf2, 0.5, 1, 0.5, true);
	//std::cerr << "Count of all iters = " << count_of_iters << std::endl;
	/*
	for (int conf1 = 0; conf1 < 2; conf1++) {
		for (int conf2 = 0; conf2 < 2; conf2++) {
			for (double input = 0; input < 1.5; input += 0.5) {
				for (double f1 = 0; f1 < 1.5; f1 += 0.5) {
					for (double f2 = 0; f2 < 1.5; f2 += 0.5) {
						for (double f3 = 0; f3 < 1.5; f3 += 0.5) {
							for (double o1 = 0; o1 < 1.5; o1 += 0.5) {
								for (double o2 = 0; o2 < 1.5; o2 += 0.5) {
									for (double o3 = 0; o3 < 1.5; o3 += 0.5) {
										if ((conf1 == 0) && (conf2 == 0)) {
											;
										}
										// std::cerr << "A B f*1 f f*2 Fi f*3" << std::endl;
										iters = test_function(input, f1, f2, f3, conf1, conf2, o1, o2, o3, false);
										std::cerr << "Count of iters:" << iters << std::endl;
										std::cerr << std::endl;
										if (max < iters) {
											//std::cerr << "i:" << input << " c:" << conf1 << conf2 << " f:" << f1 << f2 << f3 << " o:" << o1 << o2 << o3 << std::endl;
											max = iters;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	std::cerr << "Max:" << max << std::endl;
	/*
	std::cerr << "Count of all cycles = " << all << std::endl;
	std::cerr << "Medium value = " << sum / all << std::endl;
	std::cerr << "Mediana value = " << CalcM(values) << std::endl;
	*/
	
	return 0;
	
}