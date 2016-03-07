#ifndef WRITEGAMESSINPUT_H
#define WRITEGAMESSINPUT_H

#include <fstream>
#include <string>
#include <vector>

#include "MolecularMechanicsCorrection.h"

//build input with individual name.
//we could want to check the best when GA ends.
class WriteGamessInput
{
public:
	WriteGamessInput();
	~WriteGamessInput();

	void startWriteGamess(const std::vector<std::string> &pathOfBases,
		const std::string &gamessOptions_in,
		const std::vector<int> &individualTypes_in,
		std::string projectName_in,
		std::string nProc_in,
		std::string gamessPath_in,
		std::string gamessScr_in,
		std::vector<double> &gaussianCoeff_in
		);//string para o tipo de bases

	std::string createInput(const std::vector<double> &coordinates, int individual); //types number are the same of pathOfBases

	void changeExpoentsAndContractions(int baseType, const std::vector<double> & expoentsAndContractions);

	inline void setEcpReading(bool toReadEcp) { readEcp = toReadEcp; }

	inline std::string getNProc() { return nProc; }
	inline std::string getGamessPah() { return gamessPath; }
	inline std::string getGamessScr() { return gamessScr; }

	void calculateMMCorrection(std::vector<double> &point);
	double getMMEnergy();
	std::vector<double> getMMGradient();

private:
	bool readEcp;
	std::string nProc;
	std::string gamessOptions_; //gamess header
	std::vector<int> individualTypes;
	std::string projectName;
	std::string gamessPath;
	std::string gamessScr;

	void startVectorStrings(int nAtomTypes);
	std::vector<std::string> atomName;
	std::vector<std::string> baseCoefficients;
	std::vector<std::string> ecpFlag;
	std::vector<std::string> ecpCoefficients;

	bool checkNumber(std::string base);
	void readBaseType(int baseType,const std::vector<std::string> &pathOfBases);
	void readBaseI(std::ifstream &readBase_, std::vector< std::string > &baseCoefficients, int baseType);
	void readEcpFromFile(std::ifstream &readBase_, std::vector< std::string > &baseCoefficients, int baseType);

	MolecularMechanicsCorrection mmCorrection_;

};

#endif


