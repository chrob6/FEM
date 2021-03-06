
#include "pch.h"
#include <math.h>
#include <string>
#include <utility>
#include "SOE.cpp"

using namespace std;

bool gauss(int n, double ** AB, double * X);
void calculate(double* x, double* y, int i, element*EL, node*ND);
void calulateLocalValues(element* EL, node* ND);


int main()
{

	
	globaldata.printGlobalData();

	pair <element*, node*> pairElementsNodes = buildElementWeb(); // zbudowanie siatku w oparcji o plik data.txt
	element* EL = pairElementsNodes.first; //zbudowane elementu siatki
	node * ND = pairElementsNodes.second; // wezły danych elementów
	
	calulateLocalValues(EL, ND);


	//agregacja
	for (int k = 0; k < globaldata.nE; k++) {

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				soe.HG[EL[k].ID[i] - 1][EL[k].ID[j] - 1] += EL[k].Hl[i][j];
				soe.CG[EL[k].ID[i] - 1][EL[k].ID[j] - 1] += EL[k].Cl[i][j];
			}
		}
		for (int i = 0; i < 4; i++) {
			soe.PG[EL[k].ID[i] - 1] += EL[k].Pl[i];
		}
	}

	double **H_plus_c_divide_dt = new double*[soe.size];
	for (int i = 0; i < soe.size; i++) H_plus_c_divide_dt[i] = new double[soe.size];

	
	for (int i = 0; i < soe.size; i++) {
		for (int j = 0; j < soe.size; j++) {
			H_plus_c_divide_dt[i][j] = soe.HG[i][j] + soe.CG[i][j] / globaldata.delta_tau;
		}
	}

	double *vectorP = new double[soe.size];

	for (int step = 0; step < soe.size; step++) {
		for (int i = 0; i < soe.size; i++) {
			vectorP[i] = 0;
		}

		for (int i = 0; i < soe.size; i++) {
			for (int j = 0; j < soe.size; j++) {
				vectorP[i] += ((soe.CG[i][j] / globaldata.delta_tau) * soe.t0[j]);
			}
		}

		for (int i = 0; i < soe.size; i++) {
			vectorP[i] += -1 * soe.PG[i];
		}

		double **HP = new double *[soe.size];
		double *T = new double[soe.size];
		for (int i = 0; i < soe.size; i++) HP[i] = new double[soe.size + 1];

		for (int i = 0; i < soe.size; i++)
			for (int j = 0; j < soe.size; j++) {
				HP[i][j] = H_plus_c_divide_dt[i][j];
			}

		for (int i = 0; i < soe.size; i++) {
			HP[i][soe.size] = vectorP[i];
		}

		double Tmax;
		double Tmin;
		if (gauss(soe.size, HP, T)) {
			Tmax = T[0];
			Tmin = T[0];
			for (int i = 0; i < soe.size; i++) {
				if (T[i] > Tmax) Tmax = T[i];
				if (T[i] < Tmin) Tmin = T[i];
				
			}
		}
		cout << "Time: " << globaldata.delta_tau + step * globaldata.delta_tau << "   Tmin: " << Tmin << "   Tmax: " << Tmax << endl;
		for (int i = 0; i < soe.size; i++) {
			soe.t0[i] = T[i];
		}
		if (globaldata.delta_tau + step * globaldata.delta_tau == globaldata.sim_time) break;
	}
	cin.get();
	system("Pause");
	return 0; 
}



bool gauss(int n, double ** AB, double * X)
{
	int i, j, k;
	double m, s;

	// eliminacja współczynników
	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (fabs(AB[i][i]) < 1e-12) return false;
			m = -AB[j][i] / AB[i][i];
			for (k = i + 1; k <= n; k++)
				AB[j][k] += m * AB[i][k];
		}
	}

	// wyliczanie niewiadomych
	for (i = n - 1; i >= 0; i--)
	{
		s = AB[i][n];
		for (j = n - 1; j >= i + 1; j--)
			s -= AB[i][j] * X[j];
		if (fabs(AB[i][i]) < 1e-12) return false;
		X[i] = s / AB[i][i];
	}
	return true;
}

void calculate(double* x, double* y, int i, element*EL, node*ND) {

	double J[2][2], oJ[2][2];
	double Xeta, Xksi, Yeta, Yksi, detJ;

	double Nx[4], Ny[4], N[4];
	double Hx[4][4], Hy[4][4], NN[4][4];
	double C[4][4] = { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0} };
	double H[4][4] = { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0} };
	double HBC[4][4] = { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0} };
	double P[4] = { 0,0,0,0 };


	for (int k = 0; k < elem.npc; k++) { 
		Xeta = 0, Xksi = 0, Yeta = 0, Yksi = 0;

		for (int i = 0; i < 4; i++) { 
			Xksi += elem.Nksi[k][i] * x[i];
			Yksi += elem.Nksi[k][i] * y[i];
			Xeta += elem.Neta[k][i] * x[i];
			Yeta += elem.Neta[k][i] * y[i];
		}

		J[0][0] = Xksi;
		J[0][1] = Yksi;
		J[1][0] = Xeta;
		J[1][1] = Yeta;

		detJ = (Xksi *  Yeta) - (Yksi *  Xeta);
		oJ[0][0] = J[1][1];
		oJ[0][1] = -J[0][1];
		oJ[1][0] = -J[1][0];
		oJ[1][1] = J[0][0];


		for (int i = 0; i < 4; i++) {
			Nx[i] = 1 / detJ * (elem.Nksi[k][i] * oJ[0][0] + elem.Neta[k][i] * oJ[0][1]);
			Ny[i] = 1 / detJ * (elem.Nksi[k][i] * oJ[1][0] + elem.Neta[k][i] * oJ[1][1]);
			N[i] = elem.N[k][i];
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Hx[i][j] = Nx[i] * Nx[j] * detJ;
				Hy[i][j] = Ny[i] * Ny[j] * detJ;
				NN[i][j] = N[i] * N[j] * detJ;
			}
		}


		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				H[i][j] += globaldata.K * (Hx[i][j] + Hy[i][j])* elem.w[k];
				C[i][j] += NN[i][j] * globaldata.ro * globaldata.c *  elem.w[k];
			}
		}
	}

	// Calculate HBC and P
	for (int l = 0; l < 4; l++) {
		int sec_node;
		if (l < 3) sec_node = l + 1;
		else sec_node = 0;

		if (ND[EL[i].ID[l] - 1].BC == 1 && ND[EL[i].ID[sec_node] - 1].BC == 1) {
			double len = sqrt(pow(((ND[EL[i].ID[sec_node] - 1].x) - (ND[EL[i].ID[l] - 1].x)), 2) + pow(((ND[EL[i].ID[sec_node] - 1].y) - (ND[EL[i].ID[l] - 1].y)), 2));
			//HBC
			for (int n = 0; n < 4; n++) {
				for (int m = 0; m < 4; m++) {
					HBC[n][m] += (elem.WallN[l][0][n] * elem.WallN[l][0][m] + elem.WallN[l][1][n] * elem.WallN[l][1][m]) * globaldata.alfa * len / 2;
				}
			}

			//P
			for (int n = 0; n < 4; n++) {
				P[n] += -1 * (elem.WallN[l][0][n] + elem.WallN[l][1][n]) * globaldata.ambient_temp * globaldata.alfa * len / 2;
			}
		}
	}

	// Insert calculated matrix into given element
	for (int l = 0; l < 4; l++) {
		for (int j = 0; j < 4; j++) {
			EL[i].Hl[l][j] = H[l][j] + HBC[l][j];
			EL[i].Cl[l][j] = C[l][j];
			EL[i].HBCl[l][j] = HBC[l][j];
		}
	}
	for (int l = 0; l < 4; l++) {
		EL[i].Pl[l] = P[l];
	}


}


void calulateLocalValues(element* EL, node* ND) {

	int nE = globaldata.nE;
	for (int i = 0; i < nE; i++) {

		double X[4] = { ND[EL[i].ID[0] - 1].x ,ND[EL[i].ID[1] - 1].x, ND[EL[i].ID[2] - 1].x, ND[EL[i].ID[3] - 1].x };
		double Y[4] = { ND[EL[i].ID[0] - 1].y ,ND[EL[i].ID[1] - 1].y, ND[EL[i].ID[2] - 1].y, ND[EL[i].ID[3] - 1].y };
		// X, Y values for given element's IDs 

		calculate(X, Y, i, EL, ND); // taking this values, number of element and global element web
	}
}