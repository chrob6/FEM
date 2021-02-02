#include "pch.h"
#include <iostream>
#include <fstream>
#include "element.cpp"

using namespace std;

struct node {
	double t, x, y;
	int BC;
	void PrintNode();
};

struct element {
	int e;
	int ID[4];
	double Hl[4][4];
	double Cl[4][4];
	double HBCl[4][4];
	double Pl[4];
	void PrintElement();
};

pair <element*, node*> buildElementWeb() {

	double dx, dy;
	GlobalData data;
	fstream file;

	file.open("data.txt", ios::in);

	if (file.good() != true) {
		cout << "File Failure, insert data manually!" << endl;
		cout << "H(height): ";
		cin >> data.H;

		cout << "W(width): ";
		cin >> data.W;

		cout << "nH(number of nodes vertically): ";
		cin >> data.nH;

		cout << "nW(number of nodes horizontally): ";
		cin >> data.nW;
	}
	else {
		file >> data.H;
		file >> data.W;
		file >> data.nH;
		file >> data.nW;
	}
	file.close();
	data.nN = data.nH * data.nW;
	data.nE = (data.nH - 1)*(data.nW - 1);

	dx = data.W / (data.nW - 1);
	dy = data.H / (data.nH - 1);

	node * ND = new node[data.nN];
	element * EL = new element[data.nE];

	//nodes 
	int number = 0;
	for (int i = 0; i <= data.nW - 1; i++) {
		for (int j = 0; j <= data.nH - 1; j++) {
			ND[number].x = i * dx;
			ND[number].y = j * dy;
			number++;
		}
	}

	for (int i = 0; i < globaldata.nN; i++) {
		if (ND[i].x == 0 || ND[i].x == globaldata.W || ND[i].y == 0 || ND[i].y == globaldata.H)
			ND[i].BC = 1;
		else
			ND[i].BC = 0;
	}

	//elements
	int id_assign = 1;

	for (int i = 0; i < data.nE; i++) {
		EL[i].e = i + 1;
		EL[i].ID[0] = id_assign;
		EL[i].ID[1] = EL[i].ID[0] + data.nH;
		EL[i].ID[2] = EL[i].ID[1] + 1;
		EL[i].ID[3] = id_assign + 1;

		if (EL[i].ID[3] % data.nH == 0) id_assign++;
		id_assign++;
	}
	pair <element*, node*> result(EL, ND);
	return result;
}

void element::PrintElement() {
	cout << endl;
	cout << "Element number: " << e << endl;
	cout << "Node IDs: " << ID[0] << " " << ID[1] << " " << ID[2] << " " << ID[3] << endl;
}

void node::PrintNode() {
	cout << endl;
	//cout << "Node number: " << nr_nd << endl;
	cout << "X: " << x << " Y: " << y << endl;
}