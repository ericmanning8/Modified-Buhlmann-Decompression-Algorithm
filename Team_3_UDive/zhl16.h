#ifndef __zhl16_
#define __zhl16_

#include <iostream>

#include "gas.h"
#include "data.h"
#include "tissue.h"

class ZHL16 {
private:
	// members
	GAS *gas;
	TISSUE *tissues[16];
	Data *data;
	double gradient_factor_low;
	double gradient_factor_high;
	bool isDecompressing; 

	double currAvgPtol;
	double currNDL;
public:
	// methods
	ZHL16(GAS*, Data*);
	~ZHL16();
	void zhl16_dive(double delta);
	void zhl16_set_current_gas(class GAS *);
	void zhl16_set_gradient_factors(double, double);
	double zhl16_get_ptol(double ambient, double a, double b, double gf);
	double zhl16_haldane_time(double k, double ptol, double load, double partial);
	void zhl16_get_ndl();
	void zhl16_update_tissue_loads(double pressure, double r, double time);
	void calculateDecompTime(double, double);
	double pressureToDepth(double);
	void getDecompressionTime(double ptol, double currentTime);
	double calculateWaitTime(double currentTime);
};

#endif
