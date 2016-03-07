#include "BuildInput.h"

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

BuildInput::BuildInput()
{
	string auxHeader = "  NZVAR=0 COORD=UNIQUE RUNTYP=GRADIENT MAXIT=200 $END \n";
	auxHeader += " $SYSTEM MWORDS = 20 MEMDDI = 20 $END \n";
	auxHeader += " $DATA \n";
	auxHeader += "aug-cc-pvtz \n";
	auxHeader += "C1 \n";

	gamessHeaderPo = " $CONTRL SCFTYP=ROHF MULT=2 ICHARG=1 \n";
	gamessHeaderPo += auxHeader;

	gamessHeaderPc = " $CONTRL SCFTYP=RHF MULT=1 ICHARG=1 \n";
	gamessHeaderPc += auxHeader;

	gamessHeaderC = " $CONTRL SCFTYP=RHF MULT=1 ICHARG=0 \n";
	gamessHeaderC += auxHeader;

	common = "gamessNproc \n1 \ngamessNprocEnd \n";
	common += "gamessBaseNames \nh2base \ngamessBaseNamesEnd \n";

	h2coord = "-0.0215907187   0.0000000000   0.0000000000\n";
	h2coord += " 0.7215907187   0.0000000000   0.0000000000\n";

	h2Pcoord = "-0.1788677855   0.0000000000   0.0000000000\n";
	h2Pcoord += " 0.8788677855   0.0000000000   0.0000000000\n";

	h3Pcoord = "-0.86064160   -2.35389491    0.00000000\n";
	h3Pcoord += "-1.73498149   -2.43398081    0.00000000\n";
	h3Pcoord += "-1.36716797   -1.63673730    0.00000000\n";

	h4Pcoord = "-1.07634620   -2.36569490    0.00000000\n";
	h4Pcoord += "-1.92044244   -2.31521126    0.00000000\n";
	h4Pcoord += "-1.45159804   -1.55801030    0.00000000\n";
	h4Pcoord += "-1.45159804    0.16340270    0.00000000\n";

	h5Pcoord = "-1.04653711   -2.46693805    0.00000000\n";
	h5Pcoord += "-1.85665897   -2.46693805    0.00000000\n";
	h5Pcoord += "-1.45159804   -1.55801030    0.00000000\n";
	h5Pcoord += "-1.45159804   -0.30341030    0.38425000\n";
	h5Pcoord += "-1.45159804   -0.30341030   -0.38425000\n";

}

BuildInput::~BuildInput(){}

void BuildInput::createInput(
	string projectName, 
	string gamessPath, //endl depois
	int model, 
	double expoents, 
	vector<double>& gaussianCoeff
	)
{
	string name = projectName;

	int charge, nAtoms;
	string header, coord;

	defineModel(model, name, charge, nAtoms, header, coord);

	ofstream input_((name + ".txt").c_str());
	input_ << name << endl;
	input_ << "gamessCharge" << endl
		<< charge << endl
		<< "gamessChargeEnd" << endl
		<< common
		<< "gamessPath" << endl
		<< gamessPath << endl
		<< "gamessPathEnd" << endl
		<< "gamessHeader" << endl
		<< header
		<< "gamessHeaderEnd" << endl
		<< "gamessAtomNumbers" << endl
		<< nAtoms << endl
		<< "gamessAtomNumbersEnd" << endl
		<< "gamessInitialCoordinates" << endl
		<< coord
		<< "gamessInitialCoordinatesEnd" << endl
		<< "gamessExpoentContraction" << endl
		<< expoents << endl
		<< "1.0e0" << endl
		<< "gamessExpoentContractionEnd" << endl;

		if (gaussianCoeff.size() != 0)
		{
			input_ << "gamessGaussianCoeff" << endl;
			for (int i = 0; i < (int)gaussianCoeff.size(); i++)
			{
				input_ << gaussianCoeff[i] << endl;
			}

			input_ << "gamessGaussianCoeffEnd" << endl;
		}
}


void BuildInput::defineModel(int model, string &name, int &charge, int &nAtoms, std::string &header, std::string &coord)
{
	switch (model)
	{
	case 1:
		charge = 1;
		nAtoms = 2;
		header = gamessHeaderPo;
		coord = h2Pcoord;
		name += "-h2p";
		break;

	case 2:
		charge = 0;
		nAtoms = 2;
		header = gamessHeaderC;
		coord = h2coord;
		name += "-h2";
		break;

	case 3:
		charge = 1;
		nAtoms = 3;
		header = gamessHeaderPc;
		coord = h3Pcoord;
		name += "-h3p";
		break;

	case 4:
		charge = 1;
		nAtoms = 4;
		header = gamessHeaderPo;
		coord = h4Pcoord;
		name += "-h4p";
		break;

	case 5:
		charge = 1;
		nAtoms = 5;
		header = gamessHeaderPc;
		coord = h5Pcoord;
		name += "-h5p";
		break;

	default:
		cout << "ERROR on BUILDINPUT::DEFINEMODEL - CONTACT DEVELOPERS" << endl;
		exit(1);
		break;
	}



}
