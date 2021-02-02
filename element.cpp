#include"pch.h"
#include <iostream>
#include "GlobalData.cpp"
#include "N.h"

using namespace std;

struct GeneralElement {
	int schemaIntegralPoints;
	int npc;
	double v;
	double *pc_ksi;
	double *pc_eta;
	double *w_pc_ksi;
	double *w_pc_eta;
	double *w;
	double **Nksi;
	double **Neta;
	double **N;
	double v2;
	double ***WallN;

	GeneralElement();
} elem;

GeneralElement::GeneralElement() {
	schemaIntegralPoints = globaldata.schemaIntegralPoints;//getSchemaIntegralPoints();


	if (schemaIntegralPoints == 2) {

		npc = 4;
		v = (1 / sqrt(3));

		w_pc_ksi = new double[8];
		w_pc_eta = new double[8];
		double arr_w_pc_ksi[8] = { -v, v, 1, 1, v, -v,-1,-1 }; // wall's integral points 
		double arr_w_pc_eta[8] = { -1,-1,-v, v, 1,  1, v, -v };

		for (int i = 0; i < 8; i++) {
			w_pc_ksi[i] = arr_w_pc_ksi[i];
			w_pc_eta[i] = arr_w_pc_eta[i];
		}

		pc_ksi = new double[4];
		pc_eta = new double[4];
		w = new double[4];
		double arr_pc_ksi[4] = { -1 * v,1 * v,1 * v,-1 * v };
		double arr_pc_eta[4] = { -1 * v,-1 * v,1 * v,1 * v };
		double arr_w[4] = { 1,1,1,1 };

		for (int i = 0; i < npc; i++) {
			w[i] = arr_w[i];
			pc_ksi[i] = arr_pc_ksi[i];
			pc_eta[i] = arr_pc_eta[i];
		}

	}
	else if (schemaIntegralPoints == 3) {
		npc = 9;
		v = sqrt(3) / sqrt(5); //0.774597

		w_pc_ksi = new double[12];
		w_pc_eta = new double[12];
		double arr_w_pc_ksi[12] = { -v, 0, v, 1, 1, 1, v, 0, -v, -1, -1, -1 }; // wall's integral points 
		double arr_w_pc_eta[12] = { -1, -1,-1, -v, 0, v, 1, 1, 1, v, 0, -v };

		for (int i = 0; i < 12; i++) {
			w_pc_ksi[i] = arr_w_pc_ksi[i];
			w_pc_eta[i] = arr_w_pc_eta[i];
		}


		pc_ksi = new double[9];
		pc_eta = new double[9];
		w = new double[9];
		double arr_pc_ksi[9] = { -v, 0, v, -v, 0, v , -v, 0, v };
		double arr_pc_eta[9] = { -v,-v,-v,0,0,0 ,v,v,v };
		double arr_w[9] = { (5.0 / 9)*(5.0 / 9), (8.0 / 9)*(5.0 / 9), (5.0 / 9)*(5.0 / 9),
						(5.0 / 9)*(8.0 / 9), (8.0 / 9)*(8.0 / 9), (5.0 / 9)*(8.0 / 9) ,
						(5.0 / 9)*(5.0 / 9), (8.0 / 9)*(5.0 / 9), (5.0 / 9)*(5.0 / 9) };

		for (int i = 0; i < npc; i++) {
			w[i] = arr_w[i];
			pc_ksi[i] = arr_pc_ksi[i];
			pc_eta[i] = arr_pc_eta[i];
		}

	}
	else if (schemaIntegralPoints == 4) {

		npc = 16;
		v = sqrt((3.0 / 7.0) + (2.0 / 7.0)*sqrt((6.0 / 5.0)));  //0.861136;
		v2 = sqrt((3.0 / 7.0) - (2.0 / 7.0)*sqrt((6.0 / 5.0)));  //0.339981;

		w_pc_ksi = new double[16];
		w_pc_eta = new double[16];
		double arr_w_pc_ksi[16] = { -v, -v2, v2, v, 1, 1, 1, 1, v, v2, -v2, -v, -1,-1,-1,-1 }; // wall's integral points 
		double arr_w_pc_eta[16] = { -1,-1,-1,-1, -v, -v2, v2, v, 1, 1, 1, 1, v, v2, -v2, -v };

		for (int i = 0; i < 16; i++) {
			w_pc_ksi[i] = arr_w_pc_ksi[i];
			w_pc_eta[i] = arr_w_pc_eta[i];
		}

		double _w1 = (18 - sqrt(30.0)) / 36; //0.347855;
		double _w2 = (18 + sqrt(30.0)) / 36; //0.652145;

		pc_ksi = new double[16];
		pc_eta = new double[16];
		w = new double[16];
		double arr_pc_ksi[16] = { -v, -v2, v2, v, -v, -v2, v2, v, -v, -v2, v2, v, -v, -v2, v2, v };
		double arr_pc_eta[16] = { -v, -v, -v, -v,-v2, -v2, -v2, -v2, v2, v2, v2, v2, v, v, v, v };
		double arr_w[16] = { _w1*_w1, _w1*_w2, _w1*_w2, _w1*_w1,
							 _w1*_w2, _w2*_w2, _w2*_w2, _w1*_w2,
							 _w1*_w2, _w2*_w2, _w2*_w2, _w1*_w2,
							 _w1*_w1, _w1*_w2, _w1*_w2, _w1*_w1 };

		for (int i = 0; i < npc; i++) {
			w[i] = arr_w[i];
			pc_ksi[i] = arr_pc_ksi[i];
			pc_eta[i] = arr_pc_eta[i];
		}

	}

	N = new double*[npc];
	Nksi = new double*[npc];
	Neta = new double*[npc];
	WallN = new double**[npc];
	int pc_count = 0;

	for (int i = 0; i < npc; i++) {
		N[i] = new double[npc];
		Nksi[i] = new double[npc];
		Neta[i] = new double[npc];
		WallN[i] = new double*[schemaIntegralPoints];

		for (int j = 0; j < 4; j++) {
			N[i][j] = N_f(pc_ksi[i], pc_eta[i], j);
			Nksi[i][j] = Nk(pc_eta[i], j);
			Neta[i][j] = Ne(pc_ksi[i], j);
		}

		for (int pc = 0; pc < schemaIntegralPoints; pc++) {
			WallN[i][pc] = new double[npc];
			for (int n = 0; n < npc; n++) {
				WallN[i][pc][n] = N_f(w_pc_ksi[pc_count], w_pc_eta[pc_count], n);
			}
			pc_count++;
		}
	}
}