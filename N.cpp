#include"pch.h"
#include "N.h"

double Nk(double pc, int i) {
	
	if (i == 0)
		return -0.25*(1 - pc);
	else if (i == 1)
		return 0.25*(1 - pc);
	else if (i == 2)
		return 0.25*(1 + pc);
	else if (i == 3)
		return -0.25*(1 + pc);
}

double Ne(double pc, int i) {
	
	if (i == 0)
		return -0.25*(1 - pc);
	else if (i == 1)
		return -0.25*(1 + pc);
	else if (i == 2)
		return 0.25*(1 + pc);
	else if (i == 3)
		return 0.25*(1 - pc);
}

double N_f(double pcK, double pcE, int i) {
	if (i==0)
		return 0.25*(1 - pcK)*(1 - pcE);
	else if(i==1)
		return  0.25*(1 + pcK)*(1 - pcE);
	else if(i==2)
		return 0.25*(1 + pcK)*(1 + pcE);
	else if(i==3)
		return 0.25*(1 - pcK)*(1 + pcE);
}

