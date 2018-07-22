// differential_evolution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DifferentialEvolution.h"
#include "FunctionToOpt.h"

/*
TODO: 
- Dodaæ warunek zakoñczenia obliczeñ gdy b³¹d jest dostatecznie ma³y
*/

int main() {
	//Griewank Gfun(2, -600, 600);
	//Easom Efun(2, -100, 100);	// Pierwszy parametr bez znaczenia, i tak bêdzie 2
	Rosenbrock Rfun(10, -5, 10); 

	DifferentialEvolution DE(Rfun, 20);
	DE.optimalization(1000);

	system("pause");
    return 0;
}

