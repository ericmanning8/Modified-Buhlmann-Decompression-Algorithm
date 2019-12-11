// basic libraries
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include <unistd.h>

// header files needed 
#include "Team_3_UDive/data.h"
#include "Team_3_UDive/zhl16.h"
#include "Team_3_UDive/gas.h"
#include "Team_3_UDive/tissue.h"

using namespace std;

int main(int argv, char** argc) {
	// basic check for correct inputs 
	if (argv != 4) {
		cerr << "please enter 3 file names" << endl;
		return 1;
	}

	// file initialization 
	ifstream inputFile(argc[1]);
	ofstream outputFile(argc[2]);
	ifstream testFile(argc[3]);

	// make sure all files open correctly 
	if (inputFile.fail()) {
		cerr << "File " << argc[1] << " could not be opened" << endl; 
		return 1;
	}
	else if (outputFile.fail()) {
		cerr << "File " << argc[2] << " could not be opened" << endl; 
		return 1;
	}
	else if (outputFile.fail()) {
		cerr << "File " << argc[3] << " could not be opened" << endl; 
		return 1;
	}

	// data aquistiion
	cout << "Parsing input file: " << argc[1] << endl;

	// recieve input for preprocessing data 
	double weight = 0;
	inputFile >> weight;

	// create data structures 
	GAS *gas = new GAS(21.0, 35.0);
	Data *data = new Data(weight, gas);
	ZHL16 *zhl16 = new ZHL16(gas, data);

	// actual loop to simulate dive - assume 1 minute delay between intervals 
	cout << endl;
	cout << "Running Buhlmann Algorithm with files: " << argc[1] << " " << argc[2] << " " << argc[3] << endl;
	double pressure, heartRate, temperature, diveTime = 0.0;
	string line;
	while (!testFile.eof()) {
		getline(testFile, line);
		cout << endl;
		cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
		cout << endl;
		stringstream ss(line);
		ss >> pressure >> temperature >> heartRate;

		cout << "Performing buhlmann algorithm on data: " << pressure << " bar, " << temperature << " Farenheit, " << heartRate << " BPM" << endl;

		// update all the data 
		data->updateData(pressure, heartRate, temperature, diveTime);
  		// calcuates the decompression time
  		zhl16->zhl16_dive(1.0);
  		//printf("NDL: %f seconds minutes: %f\n", zhl16_get_ndl(3), zhl16_get_ndl(3) / 60);
  		zhl16->zhl16_get_ndl();

		diveTime += 1.0;
		usleep(500000);
	}
	cout << endl;
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	cout << endl;
	cout << "Dive Complete!" << endl;
	cout << endl;

	// write output file 
	cout << "Building output file: " << argc[2] << endl;
	outputFile << "Dive time: " << data->getDiveTime() << " minutes" << endl;
	outputFile << "Maximum Depth: " << data->getMaxDepth() << " feet" << endl;
	outputFile << "Average Heart Rate: " << data->getAvgHR() << " BPM" << endl;
	outputFile << "Average Temperature: " << data->getAvgTemp() << " Farenheit" << endl;

	// deallocate all ram 
	cout << "Deallocating all dynamically-allocated memory in heap" << endl;
	delete data;
	delete gas;
	delete zhl16;

	// close all the files 
	cout << "Closing all files" << endl;
	inputFile.close();
	outputFile.close();
	testFile.close();

	return 0;
}