#ifndef __GAS_H_
#define __GAS_H_

#include <iostream>

/*!
 * Gases are in percentage
 * eg: 21 o2
 */
class GAS {
private:
	double o2;
	double he;
	double n2;
public:
	GAS(double o2, double he); // constructor - set the gas composition 
	double gas_percent_to_partial(double percent);
	double gas_get_partial_pressure(double depth_in_bar); // Returns the partial pressure of the gas based on the current depth in bars
	double gas_get_adjusted_partial_pressure(double depth_in_bar);
	void updateN2(double VO2, double weight, double time);

	// getter functions 
	double getN2();
	double getHe();
	double getO2();

	// setter functions
	void setN2(double n2);
};

#endif
