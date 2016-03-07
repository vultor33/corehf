#ifndef READGAMESSOUTPUT_H
#define READGAMESSOUTPUT_H

#include <string>
#include <vector>
#include <fstream>

class ReadGamessOutput
{
public:	
	ReadGamessOutput(std::ofstream &logFile_in_);
	~ReadGamessOutput();

	void readOptimizationGamess(std::string outputToRead);

	void rePrintReading();

	void setMMEnergyGradient(double MMEnergy_in, std::vector<double> &MMGradient_in);

	inline void setIonizationPosition(int pos) { ionizationPos = pos; }

	double getEnergy();
	std::vector<double> getCoordinates();
	std::vector<double> getGradient();
	inline double getIonizationPotential() { return ionizationPotential; }
	std::vector<double> getDipole();

private:
	std::string energyFlag;
	std::string gradientFlag;
	std::string xyzCoordinatesFlag;
	int ionizationPos;
	std::string ionizationFlag;
	std::string dipoleFlag;

	double gamessEnergy;

	std::vector<double> finalCoordinates;
	std::vector<double> gamessGradient;
	double ionizationPotential;
	std::vector<double> dipole;

	double MMEnergy;
	std::vector<double> MMGradient;

	double getEnergyFromString(std::string auxline);
	std::vector<double> getCoordinatesFromFile(std::ifstream &gamessOut_);
	std::vector<double> getGradientFromFile(std::ifstream &gamessOut_);
	double getIonizationFromFile(std::ifstream &gamessOut_);
	double readnDoubles(std::string auxline, int nEigens);
	std::vector<double> getDipoleFromFile(std::ifstream &gamessOut_);

	std::ofstream & logFile_;
};

#endif
