#include "data.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor 

Data::Data(double weight, GAS *gas) { // default constructor uses generic values
  cout << "Creating data object with inputed data: " << weight << " " << endl;
  
  // inputed values
  this->weight = weight;

  this->gas = gas; 

  // initlize the other variables to 0
  this->currPressure = 1; 
  this->prevPressure = 0; 
  this->maxDepth = 0;
  this->prevHR = 0; 
  this->currHR = 0;
  this->runningAvgHR = 0; 
  this->prevTemp = 0;
  this->currTemp = 0;
  this->runningAvgTemp = 0;
  this->diveTime = 0; 
  this->N = 1; 
  this->currDepth = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// update functions 

void Data::updateData(double pressure, double heartRate, double temp, double time) {
  this->updateHR(heartRate);
  this->updatePressure(pressure);
  this->updateTemp(temp);
  this->diveTime = time;
  this->N++;
}

void Data::updatePressure(double p) {
  this->prevPressure = this->currPressure;
  this->currPressure = p;
  this->currDepth = pressureToDepth(currPressure);
  cout << "Current depth: " << currDepth << " feet" << endl;
  cout << "Current time: " << diveTime << " minutes" << endl;
  updateMaxDepth();
}

void Data::updateHR(double hr) {
  this->prevHR = this->currHR;
  this->currHR = hr;
  this->updateAvgHR();
  this->updateVO2();
}

void Data::updateTemp(double t) {
  this->prevTemp = this->currTemp;
  this->currTemp = t;
  this->updateAvgTemp();
}

void Data::updateAvgHR() {
  this->runningAvgHR = (this->runningAvgHR * (this->N - 1) + this->currHR) / this->N;
}

void Data::updateAvgTemp() {
  this->runningAvgTemp = (this->runningAvgTemp * (this->N - 1) + this->currTemp) / this->N;
}

void Data::updateMaxDepth() {
  if (pressureToDepth(this->currPressure) > this->maxDepth) {
    this->maxDepth = pressureToDepth(this->currPressure);
  }
  return;
}

void Data::updateVO2() {
  this->VO2 = calculateVO2();
  gas->setN2(findUpdatedN2());
}

double Data::findUpdatedN2() {
  return (0.79 / 0.21) * ( (this->diveTime / 60) * (this->VO2 * (this->weight * 0.453592) ) );
}

/////////////////////////////////////////////////////////////////////////////////////
// helper functions 

double Data::calculateVO2() {
  return this->VO2 = (0.111 * currHR) + ( (14.085 + (1.858 * (diveTime * (1/126.359493151)))) - (0.192 * (weight * 0.453592))) ;
}

double Data::pressureToDepth(double ptol) {
	return ( (ptol * 33) / 0.9866665);
}

/////////////////////////////////////////////////////////////////////////////////////
// getter functions 

double Data::getDiveTime() {
  return this->diveTime;
}

double Data::getMaxDepth() {
return this->maxDepth;
}

double Data::getAvgHR() { 
  return this->runningAvgHR;
}

double Data::getAvgTemp() {
  return this->runningAvgTemp;
}

/////////////////////////////////////////////////////////////////////////////////////
// SD Card File I/O