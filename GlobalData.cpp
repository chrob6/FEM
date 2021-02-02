//0.1 0.1 4 4 2 25 700 7800 300 1200 100 50 500 test case 1
//0.1 0.1 31 31 2 25 700 7800 300 1200 100 1 1 test case 2
#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;

struct GlobalData {
	double H;
	double W;
	int nH;
	int nW;
	int nN;
	int nE;
	double K;
	double c;
	double ro;
	double alfa;
	double ambient_temp;
	double init_temp;
	double delta_tau;
	double sim_time;
	int schemaIntegralPoints;

	GlobalData();
	void printGlobalData();
} globaldata;


void GlobalData::printGlobalData() {
	cout << endl << endl;
	cout << "Height: " << H << endl;
	cout << "Width: " << W << endl;
	cout << "Nodes amount - Height: " << nH << endl;
	cout << "Nodes amount - Width: " << nW << endl;
	cout << "Integration schema: " << schemaIntegralPoints << endl;
	cout << "Conductivity K: " << K << endl;
	cout << "Specific heat c: " << c << endl;
	cout << "Density 'ro': " << ro << endl;
	cout << "Alfa: " << alfa << endl;
	cout << "Ambient temperature: " << ambient_temp << endl;
	cout << "Initial temperature: " << init_temp << endl;
	cout << "Simulation step deltaTau: " << delta_tau << endl;
	cout << "Simulation time: " << sim_time << endl;
	cout << endl << endl;
}

GlobalData::GlobalData() {
	//double H, W, nH, nW, K
	fstream file;
	file.open("data.txt", ios::in);

	file >> H;
	file >> W;
	file >> nH;
	file >> nW;
	file >> schemaIntegralPoints;
	file >> K;
	file >> c;
	file >> ro;
	file >> alfa;
	file >> ambient_temp;
	file >> init_temp;
	file >> delta_tau;
	file >> sim_time;
	nN = nH * nW;
	nE = (nH - 1)*(nW - 1);
	file.close();
}