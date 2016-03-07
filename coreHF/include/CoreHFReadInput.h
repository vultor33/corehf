#ifndef COREHFREADINPUT_H
#define COREHFREADINPUT_H

#include <vector>
#include <string>
#include <fstream>

class CoreHFReadInput
{
public:
	CoreHFReadInput();
	~CoreHFReadInput();

	void readInput(std::string inputName);

	void rePrintInput(std::ofstream &of_);

	std::string projectName;
	int charge;
	std::string nProc;
	std::string gamessPath;
	std::string gamessScr;
	std::string gamessHeader;
	std::vector<std::string> baseTypes;
	std::vector<int> indTypes;
	std::vector<double> initialCoordinates;
	std::vector<double> baseExpoentCoefficients; //change to matrix if more bases are allowed
	std::vector<double> gaussianCoeff;

	int ionizationPosition;

private:
	void readHeader(std::ifstream &coreInput_);
	void readBaseNames(std::ifstream &coreInput_);
	void readAtomNumbers(std::ifstream &coreInput_);
	void readInitialCoordinates(std::ifstream &coreInput_);
	void readExpoentContraction(std::ifstream &coreInput_);
	void readGaussianCoeff(std::ifstream &coreInput_);
	void calculateIonizationPosition();//very specific need changes
};

#endif


