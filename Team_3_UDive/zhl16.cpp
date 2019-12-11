#include <stdio.h>
#include <math.h>

#include "zhl16.h"
#include "tissue.h"

using namespace std;

ZHL16::~ZHL16() {
	for (int i = 0; i < 16; i++) {
		delete tissues[i];
	}
}

void ZHL16::zhl16_set_gradient_factors(double low, double high) {
	this->gradient_factor_low = low / 100;
	this->gradient_factor_high = high / 100;
}

// TODO Move somewhere else?
double previous_pressure = 0;

void ZHL16::zhl16_dive(double delta) {
	double r = (gas->gas_get_adjusted_partial_pressure(data->currPressure) - gas->gas_get_adjusted_partial_pressure(data->prevPressure)) / delta;
	this->zhl16_update_tissue_loads(gas->gas_get_adjusted_partial_pressure(data->prevPressure), r, delta);
}

double ZHL16::zhl16_get_ptol(double ambient, double a, double b, double gf) {
	return ( a * gf + ((ambient * (gf - gf * b + b)) / b) );
}

double ZHL16::zhl16_haldane_time(double k, double ptol, double load, double partial) {
	double logArg = (ptol - partial) / (load - partial);

	if (logArg < 0)
		printf("Logarg < 0 in haldane time\n");

	return -(1 / k) * log(logArg);
}

void ZHL16::zhl16_get_ndl() {
	double ndl = 5940; // is 99 minutes in seconds..
	double avgPtol = 0;
	double maxPtol = 0.0;

	for (int i = 0; i < 16; i++) {
		double ptol = zhl16_get_ptol(1, tissues[i]->a, tissues[i]->b, this->gradient_factor_high);
		avgPtol = (avgPtol * (i - 1) + ptol) / (i + 1);

		// update the max ptol
		maxPtol = max(maxPtol, ptol);

		// Only if compartment is under pTol otherwise ndl is -1 and others are irrelevant
		if (tissues[i]->load < ptol) {
			//printf("Partial pressure: %f - ptol: %f - tissue load: %f\n", gas->gas_get_adjusted_partial_pressure(pressure), ptol, tissues[i]->load);
			if (ptol < gas->gas_get_adjusted_partial_pressure(data->currPressure)) { // if ptol can be reached

				double tmp = zhl16_haldane_time(tissues[i]->k, ptol, tissues[i]->load, gas->gas_get_adjusted_partial_pressure(data->currPressure));
				//printf("Tissue: %d - NDL: %f - NDLI: %f\n", i, ndl, tmp);

				if (tmp < ndl) 
					ndl = tmp;
			}
		} 
		else {
			ndl = -1;
			break;
		}
	}
	this->currAvgPtol = avgPtol;
	this->getDecompressionTime(maxPtol, data->diveTime);

	return;
}

void ZHL16::zhl16_update_tissue_loads(double pressure, double r, double time) {
	double k;
	double tmp;
	for (int i = 0; i < 16; i++) {
		k = tissues[i]->k;
		tmp = tissues[i]->load;
		tissues[i]->tissue_set_load((pressure + r * (time - (1/k)) - (pressure-tmp - (r/k)) * exp(-k*time)));
	}
}

// data classure containing the tissue compartment values for N2
ZHL16::ZHL16(GAS* gas, Data* d) {
	// set the pointers to the gas and data classes
	this->gas = gas;
	this->data = d;

	// set the gradient factors for the dive 
	this->gradient_factor_low = 0.2;
	this->gradient_factor_high = 0.7;

	// set the decompressing boolean to false
	this->isDecompressing = false;
	this->currAvgPtol = 0;

	printf("Initializing zhl16 tissues\n");
	
	// TODO set initial load (based on current pressure and/or left over from previous dive?)
	tissues[0] = new TISSUE(1.1696,	0.5578, 0.00231049060186648, gas->gas_get_adjusted_partial_pressure(1));
	tissues[1] = new TISSUE(1.0, 		0.6514, 0.00144405662616655, gas->gas_get_adjusted_partial_pressure(1));
	tissues[2] = new TISSUE(0.8618, 	0.7222, 0.00092419624074659, gas->gas_get_adjusted_partial_pressure(1));
	tissues[3] = new TISSUE(0.7562, 	0.7825, 0.00062445691942338, gas->gas_get_adjusted_partial_pressure(1));
	tissues[4] = new TISSUE(0.62,		0.8126, 0.00042786862997528, gas->gas_get_adjusted_partial_pressure(1));
	tissues[5] = new TISSUE(0.5043,	0.8434, 0.00030163062687552, gas->gas_get_adjusted_partial_pressure(1));
	tissues[6] = new TISSUE(0.441,		0.8693, 0.00021275235744627, gas->gas_get_adjusted_partial_pressure(1));
	tissues[7] = new TISSUE(0.4, 		0.891,	0.00015003185726406, gas->gas_get_adjusted_partial_pressure(1));
	tissues[8] = new TISSUE(0.375, 	0.9092, 0.00010598580742507, gas->gas_get_adjusted_partial_pressure(1));
	tissues[9] = new TISSUE(0.35, 		0.9222, 0.00007912639047488, gas->gas_get_adjusted_partial_pressure(1));
	tissues[10] = new TISSUE( 0.3295, 	0.9319, 0.00006177782357932, gas->gas_get_adjusted_partial_pressure(1));
	tissues[11] = new TISSUE( 0.3065, 	0.9403, 0.00004833662347001, gas->gas_get_adjusted_partial_pressure(1));
	tissues[12] = new TISSUE( 0.2835, 	0.9477, 0.00003787689511257, gas->gas_get_adjusted_partial_pressure(1));
	tissues[13] = new TISSUE( 0.261, 	0.9544, 0.00002962167438290, gas->gas_get_adjusted_partial_pressure(1));
	tissues[14] = new TISSUE( 0.248, 	0.9602, 0.00002319769680589, gas->gas_get_adjusted_partial_pressure(1));
	tissues[15] = new TISSUE( 0.2327, 	0.9653, 0.00001819283938478, gas->gas_get_adjusted_partial_pressure(1));
}

double ZHL16::pressureToDepth(double ptol) {
	return ( (ptol * 33) / 0.9866665);
}

void ZHL16::calculateDecompTime(double depth, double time) {
	cout << "Remain at " << depth << " feet for " << time << " minutes" << endl;
}

double ZHL16::calculateWaitTime(double currentTime) {
	double waitTime = 0.0;

	if (data->maxDepth <= 30) {
		if (data->diveTime <= 25) {
			waitTime = 27;
		}else if (data->diveTime <= 37) {
			waitTime = 39;
		}else if (data->diveTime <= 55) {
			waitTime = 57;
		}else {
			waitTime = 83;
		}
	}else if (data->maxDepth <= 39) {
		if (data->diveTime <= 19) {
			waitTime = 21;
		}else if (data->diveTime <= 25) {
			waitTime = 27;
		}else if (data->diveTime <= 37) {
			waitTime = 39;
		}else if (data->diveTime <= 57) {
			waitTime = 59;
		}else {
			waitTime = 84;
		}
	}else if (data->maxDepth <= 49) {
		if (data->diveTime <= 16) {
			waitTime = 21;
		}else if (data->diveTime <= 20) {
			waitTime = 27;
		}else if (data->diveTime <= 29) {
			waitTime = 39;
		}else if (data->diveTime <= 41) {
			waitTime = 43;
		}else if (data->diveTime <= 59) {
			waitTime = 61;
		}else if (data->diveTime <= 75) {
			waitTime = 77;
		}else {
			waitTime = 84;
		}
	}else /*if (data->maxDepth <= 59)*/ {
		if (data->diveTime <= 14) {
			waitTime = 17;
		}else if (data->diveTime <= 27) {
			waitTime = 20;
		}else if (data->diveTime <= 25) {
			waitTime = 28;
		}else if (data->diveTime <= 33) {
			waitTime = 36;
		}else if (data->diveTime <= 44) {
			waitTime = 47;
		}else if (data->diveTime <= 51) {
			waitTime = 54;
		}else if (data->diveTime <= 60) {
			waitTime = 67;
		}else if (data->diveTime <= 70) {
			waitTime = 83;
		}else {
			waitTime = 100;
		}
	}/*else if (data->maxDepth <= 68.8) {

	}else if (data->maxDepth <= 78.8) {

	}else if (data->maxDepth <= 88.6) {

	}else if (data->maxDepth <= 98.4) {

	}else if (data->maxDepth <= 108.3) {

	}else if (data->maxDepth <= 118.1) {

	}else if (data->maxDepth <= 128) {

	}else if (data->maxDepth <= 105) {

	}else if (data->maxDepth <= 114.8) {

	}else if (data->maxDepth <= 137.8) {

	}else if (data->maxDepth <= 147.6) {

	}else if (data->maxDepth <= 157.5) {

	}else if (data->maxDepth <= 167.3) {

	}else if (data->maxDepth <= 177.2) {

	}else if (data->maxDepth <= 187) {

	}else {

	} */

	return waitTime;
}

void ZHL16::getDecompressionTime(double ptol, double currentTime) {
	if (ptol <= 1.0) {
		cout << "Can ascend to surface" << endl;
	}
	else {
		double ceilingDepth = pressureToDepth(ptol);
		if (data->currDepth < ceilingDepth) {
			cout << "Can ascend to surface" << endl;
		}
		else {
			double waitTime = calculateWaitTime(currentTime);
			calculateDecompTime(ceilingDepth, waitTime);
		}
	}
	return;
}