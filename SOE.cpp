#include"pch.h"
#include <iostream>
#include "Web.cpp"
#include <iomanip>

using namespace std;

struct SOE {
	int size = globaldata.nN;
	double **HG = new double *[size];
	double **CG = new double *[size];
	double *PG = new double[size];
	double *t0 = new double[size];

	SOE()
	{
		initialize();
	}

	void initialize();
	void printHG();
	void printCG();
	void printPG();

} soe;

void SOE::initialize() {
	for (int i = 0; i < size; i++) {
		HG[i] = new double[size];
		CG[i] = new double[size];
		for (int j = 0; j < size; j++) {
			HG[i][j] = 0;
			CG[i][j] = 0;
		}
	}
	for (int j = 0; j < size; j++) {
		PG[j] = 0;
		t0[j] = globaldata.init_temp;
	}
}

void SOE::printHG() {
	cout << endl << "HG" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << setw(10) << HG[i][j];
		}
		cout << endl;
	}
}

void SOE::printCG() {
	cout << endl << "CG" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << setw(10) << CG[i][j]; // globaldata.delta_tau;
		}
		cout << endl;
	}
}

void SOE::printPG() {
	cout << endl << "PG" << endl;
	for (int i = 0; i < size; i++) {
		cout << PG[i] << " ";
	}
}