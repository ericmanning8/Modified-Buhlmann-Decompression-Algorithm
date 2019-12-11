#include "tissue.h"

#include <iostream>

using namespace std;

TISSUE::TISSUE(double a, double b, double k, double initial_load) {
	cout << "Creating tissue object with data: " << a << " " << b << " " << k << " " << initial_load << endl;

	this->a = a;
	this->b = b;
	this->k = k;

	tissue_set_load(initial_load);
}

void TISSUE::tissue_set_load(double load) {
	this->load = load;
}
