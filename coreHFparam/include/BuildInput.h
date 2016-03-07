#ifndef BUILDINPUT_H
#define BUILDINPUT_H

#include <string>
#include <vector>

class BuildInput
{
public:
	BuildInput();
	~BuildInput();

	void createInput(
		std::string projectName,
		std::string gamessPath,
		int model,
		double expoents,
		std::vector<double> &gaussianCoeff
		);
	//common segundo
	// model define header, natoms, coordinates e charge.



private:
	std::string gamessHeaderPo;
	std::string gamessHeaderPc;
	std::string gamessHeaderC;
	std::string common;
	std::string h2coord;
	std::string h2Pcoord;
	std::string h3Pcoord;
	std::string h4Pcoord;
	std::string h5Pcoord;

	void defineModel(int model, std::string &name, int &charge, int &nAtoms, std::string &header, std::string &coord);

	// intial coordinates


};


#endif
