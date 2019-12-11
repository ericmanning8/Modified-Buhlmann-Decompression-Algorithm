#ifndef __DATA_H_
#define __DATA_H_

#include <iostream>
#include <string>

#include "gas.h"

class Data {
private:
	//// public variables 

	// User data
	double weight; // users weight

	// pointer to gas composition class 
	GAS *gas;

	// presure and depth data 
	double currPressure; // store the current pressure 
	double prevPressure; // store the previous pressure 
	double maxDepth; // keep track of the max depth of the dive 
	double currDepth;

	// heart rate data 
	double prevHR; // keep track of the previous HR
	double currHR; // keep track of the current heart rate of the dive
	double runningAvgHR; // running avg of the dive 
	double VO2; // calculate the current VO2 compostion based on HR data

	// temperature data 
	double prevTemp;
	double currTemp; // keep track of the current temperature 
	double runningAvgTemp; // keep track of the average temperature of the water

	// Other data 
	double diveTime; // current duration of the dive 
	unsigned int N; // number of recordings during the dive 
public:
	// public methods

	// constructors 
	Data(double weight, GAS *gas); 

	// update functions 
	void updateData(double pressure, double heartRate, double temp, double time);
	void updatePressure(double p);
	void updateTemp(double t);
	void updateHR(double hr);
	void updateMaxDepth();
	void updateAvgHR();
	void updateAvgTemp();
	void updateVO2();
	double findUpdatedN2();

	// helper functions 
	double pressureToDepth(double ptol);
	double calculateVO2();

	// getter functions
	double getDiveTime();
	double getMaxDepth();
	double getAvgHR();
	double getAvgTemp();

	friend class ZHL16;
};

#endif

