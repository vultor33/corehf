#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include "WriteGamessInput.h"
#include "ReadGamessOutput.h"
#include "CoreHFReadInput.h"
#include "CoreHF.h"
#include "LocalOptimization.h"
#include "MolecularMechanicsCorrection.h"

using namespace std;

int main(int argc, char *argv[])
{
/////////////////////////////////////////
/// GETTING INPUT NAME //////////////////
/////////////////////////////////////////
	string inputName;
	if (argc == 1)
		inputName = "coreHFInput.txt";
	else
		inputName = argv[1];
////////////////////////////////////////

	CoreHF coreStart_(inputName);

//	cin.get();
	return 0;
}
// saidas se nao convergir

// output detalhado



/*
TESTE DAS ROTINAS SEPARADAS
chfInput_.rePrintInput();
//dlib
// gerando o input
WriteGamessInput wr_;
vector<int> indTypes(1);
indTypes[0] = 4;
vector<string> baseTypes(1);
baseTypes[0] = "h2base";
wr_.startWriteGamess(baseTypes, "nada\n", indTypes, "projetoHydra");

vector<double> coord(12);
coord[0] = 0.0e0;
coord[1] = 1.0e0;
coord[2] = 2.0e0;
coord[3] = 3.0e0;
coord[4] = 0.0e0;
coord[5] = 0.0e0;
coord[6] = 0.0e0;
coord[7] = 0.0e0;
coord[8] = 0.0e0;
coord[9] = 0.0e0;
coord[10] = 0.0e0;
coord[11] = 0.0e0;
string inp = wr_.createInput(coord, 1);

vector<double> ec(2);
ec[0] = 123.0e0;
ec[1] = 1.454e0;
wr_.changeExpoentsAndContractions(0, ec);

ReadGamessOutput ro_;
ro_.readOptimizationGamess("h5.log");

cout << "energia:  " << ro_.getEnergy() << endl;
vector<double> grad = ro_.getGradient();
int natm = grad.size()/3;
for (int i = 0; i < 5; i++)
{
cout << "gradient:  x  " << grad[i]
<< "  y  " << grad[i + natm]
<< "  z  " << grad[i + 2*natm]
<< endl;
}

cout << "nome do input:  " << inp << endl;

TESTANDO LOCAL OPTIMIZATION
( PRIMEIRO ENERGIA DEPOIS A DERIVADA CORRESPONDENTE )
LocalOptimization loc_;
vector<double> x(1);
x[0] = 3;
cout << "final energy:  " << loc_.optimize(x, 0) << endl;

TESTANDO MOLECULAR MECHANICS CORRECTION
MolecularMechanicsCorrection mm_;
vector<double> par(6);
par[0] = 1;
par[1] = 1;
par[2] = 1;
par[3] = 1;
par[4] = 1;
par[5] = 1;
mm_.setParameters(par);
vector<double> point(6);
point[0] = 1;
point[1] = 0;
point[2] = 0;
point[3] = 0;
point[4] = 0;
point[5] = 0;
mm_.calculateCorrection(point);
double energy0 = mm_.getEnergy();
cout << "energy:  " << mm_.getEnergy() << endl;
vector<double> grad = mm_.getGradient();
double h = 1.0e-5;
double ngrad;
vector<double> auxpoint = point;
for (int i = 0; i < (int)grad.size(); i++)
{
auxpoint = point;
auxpoint[i] += h;
mm_.calculateCorrection(auxpoint);
ngrad = (mm_.getEnergy() - energy0)/h;
cout << i << "  :  " << ngrad << "   " << grad[i] << endl;
}


*/
