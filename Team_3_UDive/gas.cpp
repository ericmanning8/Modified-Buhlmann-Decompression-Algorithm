#include "gas.h"

using namespace std;

GAS::GAS(double o2, double he) {
	cout << "Creating gas object with O2 partial pressure: " << o2 << endl;

	this->o2 = o2;
	this->he = he;
	this->n2 = 100 - o2 - he;
}

double GAS::gas_percent_to_partial(double percent) {
	return (percent / 100);
}

double GAS::gas_get_partial_pressure(double depth_in_bar) {
	// 0.0627 is water vapor pressure in bar
	return gas_percent_to_partial(this->n2) * (depth_in_bar - 0.0627); // returns partial pressure of nitrogen
}

// added by Eric Manning
double GAS::gas_get_adjusted_partial_pressure(double depth_in_bar) {
	double pp = gas_get_partial_pressure(depth_in_bar);
	return pp;
}

void GAS::updateN2(double VO2, double weight, double time) {
	
}

double GAS::getN2() {
	return this->n2;
}

double GAS::getHe() {
	return this->he;
}

double GAS::getO2() {
	return this->o2;
}

void GAS::setN2(double n2) {
	this->n2 = n2;
}