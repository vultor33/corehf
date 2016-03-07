#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "CalculateError.h"
#include "ReadModelOutput.h"
#include "BuildInput.h"

using namespace std;

// conferir se eu estou conseguindo ler o potencial de ionizacao

void unconverged(bool uncon)
{
	if(uncon)
	{
		ofstream fit_("fitness.txt");
		fit_ << 1.0e5 << endl;
		fit_.close();
		exit(0);
	}
}


int main()
{
	//read input
	string projectName;
	double expoent;
	string gamessPath, auxGamessPath;
	ifstream param_("coreHFparameterizationInput.txt");
	param_ >> projectName;
	param_ >> gamessPath;
	param_ >> auxGamessPath;
	gamessPath += "\n" +  auxGamessPath;
	param_ >> expoent;
	vector<double> gaussianCoeff;
	double auxCoeff;
	string auxline;
	getline(param_, auxline);
	while (getline(param_,auxline))
	{
		if (auxline.size() == 0)
			break;

		stringstream convert;
		convert << auxline;
		convert >> auxCoeff;
		gaussianCoeff.push_back(auxCoeff);
	}
	param_.close();
	//input read


	BuildInput buldInp_;
	CalculateError ce_;
	string roda;
	int model;

	model = 1;
	buldInp_.createInput(projectName,gamessPath,model,expoent,gaussianCoeff);
	roda = "./CoreHF.exe " + projectName + "-h2p.txt";
	system(roda.c_str());
	ReadModelOutput rmoh2p_(projectName + "-h2p-saida.log");
	unconverged(rmoh2p_.unconverged);
	double fit1 = ce_.calculate(model, rmoh2p_.coord, rmoh2p_.dipole, rmoh2p_.energy, rmoh2p_.ioniz);

	model = 2;
	buldInp_.createInput(projectName, gamessPath, model, expoent, gaussianCoeff);
	roda = "./CoreHF.exe " + projectName + "-h2.txt";
	system(roda.c_str());
	ReadModelOutput rmoh2_(projectName + "-h2-saida.log");
	unconverged(rmoh2_.unconverged);
	double fit2 = ce_.calculate(model, rmoh2_.coord, rmoh2_.dipole, rmoh2_.energy, rmoh2_.ioniz);

	model = 3;
	buldInp_.createInput(projectName, gamessPath, model, expoent, gaussianCoeff);
	roda = "./CoreHF.exe " + projectName + "-h3p.txt";
	system(roda.c_str());
	ReadModelOutput rmoh3p_(projectName + "-h3p-saida.log");
	unconverged(rmoh3p_.unconverged);
	double fit3 = ce_.calculate(model, rmoh3p_.coord, rmoh3p_.dipole, rmoh3p_.energy, rmoh3p_.ioniz);

	model = 4;
	buldInp_.createInput(projectName, gamessPath, model, expoent, gaussianCoeff);
	roda = "./CoreHF.exe " + projectName + "-h4p.txt";
	system(roda.c_str());
	ReadModelOutput rmoh4p_(projectName + "-h4p-saida.log");
	unconverged(rmoh4p_.unconverged);
	double fit4 = ce_.calculate(model, rmoh4p_.coord, rmoh4p_.dipole, rmoh4p_.energy, rmoh4p_.ioniz);

	model = 5;
	buldInp_.createInput(projectName, gamessPath, model, expoent, gaussianCoeff);
	roda = "./CoreHF.exe " + projectName + "-h5p.txt";
	system(roda.c_str());
	ReadModelOutput rmoh5p_(projectName + "-h5p-saida.log");
	unconverged(rmoh5p_.unconverged);
	double fit5 = ce_.calculate(model, rmoh5p_.coord, rmoh5p_.dipole, rmoh5p_.energy, rmoh5p_.ioniz);

	ofstream fit_("fitness.txt");
	fit_ << (fit1 + fit2 + fit3 + fit4 + fit5) / 5.0e0 << endl;
	fit_.close();


	// tenho q ler o output (q eu mesmo criei)

//	cout << "press enter to exit" << endl;
//	cin.get();
	return 0;
}

/*
TESTANDO ROTINAS
vector<double> x(12);
x[0] = -1.07634620;
x[1] = -1.92044244;
x[2] = -1.45159804;
x[3] = -1.45159804;
x[4] = -2.36569490;
x[5] = -2.31521126;
x[6] = -1.55801030;
x[7] = 0.16340270;
x[8] = 0.0e0;
x[9] = 0.0e0;
x[10] = 0.0e0;
x[11] = 0.0e0;
vector<double> dist = ce_.calculateDistance(x);
*/
//H       1.0 - 1.07634620 - 2.36569490    0.00000000
//H       1.0 - 1.92044244 - 2.31521126    0.00000000
//H       1.0 - 1.45159804 - 1.55801030    0.00000000
//H       1.0 - 1.45159804    0.16340270    0.00000000
