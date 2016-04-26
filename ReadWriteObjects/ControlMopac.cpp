#include "ControlMopac.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include "Coordstructs.h"
#include "ReadQuantumOutput.h"
#include "WriteQuantumInput.h"

using namespace std;

ControlMopac::ControlMopac() {}

ControlMopac::~ControlMopac(){}

bool ControlMopac::optimize(vector<CoordXYZ> & allAtoms, vector<string> & options)
{
	ReadQuantumOutput readmop_("mopac");

	WriteQuantumInput writeMop_(options);

	string inputName = writeMop_.createInput(allAtoms);

	system((mopacExecPath + inputName + ".mop").c_str());

	ReadQuantumOutput readGamess_(options[0]);

	readmop_.readOutput(inputName);

	vector<CoordXYZ> optimizedAtoms = readmop_.getCoordinates();

	if (optimizedAtoms.size() == 0) return false;

	return true;

	/*
	//CALCULATING FREQUENCY
	writeMop_.changeMopacHeader("FORCE");

	writeMop_.changeProjectName(inputName + "-freq");

	std::string freq = writeMop_.createInput(optimizedAtoms);

	system((mopacExecPath + freq + ".mop").c_str());

	readmop_.readOutput(freq);

	cout << "first frequency:  " << readmop_.getFirstFrequency() << endl;
	
	cin.get();

	return true;
	*/
}

