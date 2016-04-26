#ifndef CONTROLMOPAC_H
#define CONTROLMOPAC_H

#include <vector>
#include <string>

#include "Coordstructs.h"
#include "ReadQuantumOutput.h"

#include <vector>
#include <string>

class ControlMopac
{
public:
	ControlMopac();
	~ControlMopac();

	bool optimize(std::vector<CoordXYZ> & allAtoms, std::vector<std::string> & options);

private:
	std::string projectName;
	std::string metalName;
	std::string mopacHeader;
	std::string mopacFreq;
	std::string externalParams;
	std::string mopacExecPath = "C:/\"Program Files\"/MOPAC/MOPAC2016.exe  ";
	double frequency;

};

#endif

