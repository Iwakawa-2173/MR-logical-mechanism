#include <iostream>
#include <ctime>
#include <vector>
#include "henkamono.h"
#include "utils.h"

int main() {

	clock_t tStart = clock();

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	
	not_included_nums(5, "decimal");
	
	double time_taken = (double)(clock() - tStart) / CLOCKS_PER_SEC;
	std::cerr << "Time = " << time_taken << " seconds"<< std::endl;
	
	return 0;
}
