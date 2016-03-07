#include "CoreHF.h"

#include <iostream>
#include <fstream>

#include "CoreHFReadInput.h"
#include "WriteGamessInput.h"
#include "ReadGamessOutput.h"
#include "LocalOptimization.h"
#include "Fitness.h"

using namespace std;

CoreHF::CoreHF(string inputName)
{
	int fitnessType = 2;

	CoreHFReadInput chfInput_;
	chfInput_.readInput(inputName);
	ofstream of_((chfInput_.projectName + "-saida.log").c_str());
	chfInput_.rePrintInput(of_);

	WriteGamessInput gamessInput_;
	gamessInput_.startWriteGamess(
		chfInput_.baseTypes, 
		chfInput_.gamessHeader, 
		chfInput_.indTypes, 
		chfInput_.projectName,
		chfInput_.nProc,		
		chfInput_.gamessPath,
		chfInput_.gamessScr,
		chfInput_.gaussianCoeff
		);
	gamessInput_.changeExpoentsAndContractions(0, chfInput_.baseExpoentCoefficients);// 0 - only one base

	ReadGamessOutput readOut_(of_);
	readOut_.setIonizationPosition(chfInput_.ionizationPosition);

	LocalOptimization opt_;
	opt_.includeGamessAuxObjects(&gamessInput_, &readOut_);
	vector<double> x = chfInput_.initialCoordinates;
	opt_.optimize(x, fitnessType); //gamess - fit = 2

	Fitness fit_;
	fit_.setGamessObjects(&gamessInput_, &readOut_);
	fit_.fit(x, fitnessType);

	of_ << "energy" << endl;
	of_ << readOut_.getEnergy() << endl;
	of_ << "coordinates   " << endl << x.size() << endl;
	for (int i = 0; i < (int)x.size(); i++)
	{
		of_ << x[i] << endl;
	}
	of_ << "dipole" << endl;
	vector<double> dipole = readOut_.getDipole();
	of_ << dipole[0] << endl
		<< dipole[1] << endl
		<< dipole[2] << endl
		<< dipole[3] << endl;
	of_ << "ionization" << endl
		<< readOut_.getIonizationPotential() << endl;
	of_.close();
}

CoreHF::~CoreHF(){}
