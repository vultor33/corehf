#include "CoreHFReadInput.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

CoreHFReadInput::CoreHFReadInput()
{
	charge = 0;
	nProc = 1;
}

CoreHFReadInput::~CoreHFReadInput() {}

void CoreHFReadInput::readInput(string inputName)
{
	ifstream coreInput_(inputName.c_str());

	string auxline;

	getline(coreInput_, auxline);//project name
	if (auxline.size() == 0)
	{
		cout << "WRONG FORMAT - CHECK MANUAL" << endl;
		exit(1);
	}
	stringstream convert;
	convert << auxline;
	convert >> projectName;

	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessNproc") != string::npos)
		{
			getline(coreInput_, auxline);
			stringstream convert2;
			convert2 << auxline;
			convert2 >> nProc;
			getline(coreInput_, auxline);
		}
		if (auxline.find("gamessCharge") != string::npos)
		{
			getline(coreInput_, auxline);
			stringstream convert3;
			convert3 << auxline;
			convert3 >> charge;
			getline(coreInput_, auxline);
		}
		if (auxline.find("gamessPath") != string::npos)
		{
			getline(coreInput_, auxline);
			stringstream convert4;
			convert4 << auxline;
			convert4 >> gamessPath;
			getline(coreInput_, auxline);
			stringstream convert5;
			convert5 << auxline;
			convert5 >> gamessScr;
			getline(coreInput_, auxline);
		}

		else if (auxline.find("gamessHeader") != string::npos)
		{
			readHeader(coreInput_);		
		}
		else if (auxline.find("gamessBaseNames") != string::npos)
		{
			readBaseNames(coreInput_);
		}
		else if (auxline.find("gamessAtomNumbers") != string::npos)
		{
			readAtomNumbers(coreInput_);
		}
		else if (auxline.find("gamessInitialCoordinates") != string::npos)
		{
			readInitialCoordinates(coreInput_);
		}
		else if (auxline.find("gamessExpoentContraction") != string::npos)
		{
			readExpoentContraction(coreInput_);
		}
		else if (auxline.find("gamessGaussianCoeff") != string::npos)
		{
			readGaussianCoeff(coreInput_);
		}
	}
	coreInput_.close();

	calculateIonizationPosition();
}

void CoreHFReadInput::readHeader(std::ifstream &coreInput_)
{
	string auxline;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessHeaderEnd") != string::npos)
		{
			return;
		}
		gamessHeader += auxline + "\n";
	}
}


void CoreHFReadInput::readBaseNames(std::ifstream &coreInput_)
{
	string auxline;
	string baseName;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessBaseNamesEnd") != string::npos)
		{
			return;
		}
		stringstream convert;
		convert << auxline;
		convert >> baseName;
		baseTypes.push_back(baseName);
	}
}

void CoreHFReadInput::readAtomNumbers(std::ifstream &coreInput_)
{
	string auxline;
	int atomNumbers;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessAtomNumbersEnd") != string::npos)
		{
			return;
		}
		stringstream convert;
		convert << auxline;
		convert >> atomNumbers;
		indTypes.push_back(atomNumbers);
	}
}


void CoreHFReadInput::readInitialCoordinates(std::ifstream &coreInput_)
{
	int natm = 0;
	int size = indTypes.size();
	for (int i = 0; i < size; i++)
	{
		natm += indTypes[i];
	}

	initialCoordinates.resize(3 * natm);

	string auxline;
	int k = 0;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessInitialCoordinatesEnd") != string::npos)
		{
			return;
		}
		stringstream convert;
		convert << auxline;
		convert >> initialCoordinates[k]
			>> initialCoordinates[k + natm]
			>> initialCoordinates[k + 2 * natm];
		k++;
	}
}


void CoreHFReadInput::readExpoentContraction(std::ifstream &coreInput_)
{
	string auxline;
	double auxN;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessExpoentContractionEnd") != string::npos)
		{
			return;
		}
		stringstream convert;
		convert << auxline;
		convert >> auxN;
		baseExpoentCoefficients.push_back(auxN);
	}
}


void CoreHFReadInput::readGaussianCoeff(std::ifstream &coreInput_)
{
	string auxline;
	double auxN;
	while (getline(coreInput_, auxline))
	{
		if (auxline.find("gamessGaussianCoeffEnd") != string::npos)
		{
			return;
		}
		stringstream convert;
		convert << auxline;
		convert >> auxN;
		gaussianCoeff.push_back(auxN);
	}
}


void CoreHFReadInput::rePrintInput(std::ofstream &of_)
{
	of_ << "INPUT" << endl << endl;
	of_ << "project name:          " << projectName << endl;
	of_ << "Number of processors:  " << nProc << endl;
	of_ << "Molecule charge:       " << charge << endl;
	of_ << "Ionization position:   " << ionizationPosition << endl;
	of_ << "gamess header:  " << endl << gamessHeader << endl;
	of_ << "base file names:  " << endl;
	for (int i = 0; i < (int)baseTypes.size(); i++)
	{
		of_ << i << ":   " << baseTypes[i] << endl;
	}
	of_ << "number of atoms for each base:  " << endl;
	for (int i = 0; i < (int)indTypes.size(); i++)
	{
		of_ << i << ":   " << indTypes[i] << endl;
	}
	of_ << "initial coordinates:  " << endl;
	int natm = initialCoordinates.size() / 3;
	for (int i = 0; i < natm; i++)
	{
		of_ << i << ":   " << initialCoordinates[i]
			<< "   " << initialCoordinates[i + natm]
			<< "   " << initialCoordinates[i + 2 * natm] << endl;
	}
	of_ << "Expoents and contractions:  " << endl;
	for (int i = 0; i < (int)baseExpoentCoefficients.size(); i++)
	{
		of_ << i << ":   " << baseExpoentCoefficients[i] << endl;
	}
	of_ << "MM gaussians correction:  " << endl;
	for (int i = 0; i < (int)gaussianCoeff.size(); i++)
	{
		of_ << i << ":   " << gaussianCoeff[i] << endl;
	}
	of_ << "END OF INPUT" << endl << endl;
}

void CoreHFReadInput::calculateIonizationPosition()
{
	int ionTemp = 0;
	for (int i = 0; i < (int)indTypes.size(); i++)
	{
		ionTemp += indTypes[i];//atomos
	}
	int nElec = ionTemp - charge;
	if ((nElec % 2) == 1)
	{
		ionizationPosition = 1 + (nElec - 1) / 2;
	}
	else
	{
		ionizationPosition = nElec / 2;
	}
}


