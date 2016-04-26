#include "WriteQuantumInput.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <stdlib.h>

#include "Coordstructs.h"

using namespace std;

WriteQuantumInput::WriteQuantumInput(
	vector<string> options)
{
	type = options[0];
	projectName = options[1];
	setInputProperties(options);
}

WriteQuantumInput::~WriteQuantumInput() {}

void WriteQuantumInput::setInputProperties(vector<string> &options)
{
	if ((type == "mopac") || (type == "mopac2009"))
	{
		mopacHeader = options[2];
		externalParams = options[3];
		centralMetal = options[4];
	}
	else if (type == "gamess")
	{
		int iEndHeader;
		for (size_t i = 2; i < options.size(); i++)
		{
			if (options[i] == "EndOfHeader")
			{
				iEndHeader = i;
				break;
			}

			gamessHeader.push_back(options[i]);
		}

		int iEndBasis;
		for (size_t i = iEndHeader + 1; i < options.size(); i++)
		{
			if (options[i] == "EndOfBasis")
			{
				iEndBasis = i;
				break;
			}

			gamessAtomBasisFiles.push_back(options[i]);
		}

		if (iEndBasis != (options.size() - 1))
		{
			int iEndEcp;
			if (options[iEndBasis + 1] == "ActivateEcp")
			{
				for (size_t i = iEndBasis + 2; i < options.size(); i++)
				{
					if (options[i] == "EndOfEcp")
					{
						iEndEcp = i;
						break;
					}

					gamessEcpFiles.push_back(options[i]);
				}
			}
		}
	
		if ((gamessEcpFiles.size() != gamessAtomBasisFiles.size())
			&&
			(gamessEcpFiles.size()!= 0))
		{
			cout << "number of ecp and atomic bassis files don't match"
				<< endl << "check input" << endl;
			exit(1);
		}
		readGamessAuxFiles();
	}
}

void WriteQuantumInput::readGamessAuxFiles()
{
	bool ecp = gamessEcpFiles.size() != 0;
	int nAtoms = gamessAtomBasisFiles.size();
	atomBasis.resize(nAtoms);
	if (ecp)
		atomEcp.resize(nAtoms);

	for (int i = 0; i < nAtoms; i++)
	{
		ifstream readBasis_(gamessAtomBasisFiles[i].c_str());
		string auxline;
		while (getline(readBasis_, auxline))
		{
			stringstream line;
			string aux;
			line << auxline;
			line >> aux;
			if (aux == "end")
				break;

			atomBasis[i].push_back(auxline);
		}
		readBasis_.close();
		if (ecp)
		{
			ifstream readEcp_(gamessEcpFiles[i].c_str());
			string auxlineEcp;
			while (getline(readEcp_, auxlineEcp))
			{
				stringstream line;
				string aux;
				line << auxlineEcp;
				line >> aux;
				if (aux == "end")
					break;

				atomEcp[i].push_back(auxlineEcp);
			}
			readEcp_.close();
		}
	}





}

string WriteQuantumInput::createInput(
	vector<CoordXYZ> &coordinates,
	int index)
{
	// managing names
	stringstream convertInd;
	string inputName, indexString;
	convertInd << index;
	convertInd >> indexString;
	inputName = projectName + indexString;

	if ((type == "mopac") || (type == "mopac2009"))
		buildMopacInput(coordinates, inputName);
	else if (type == "gamess")
		buildGamessInput(coordinates, inputName);

	return inputName;
}

void WriteQuantumInput::buildGamessInput(vector<CoordXYZ> &coordinates, string inputName)
{
	inputName += ".inp";
	remove(inputName.c_str());
	ofstream gamessInput_(inputName.c_str());

	//writing header
	for (size_t i = 0; i < gamessHeader.size(); i++)
		gamessInput_ << gamessHeader[i] << endl;

	if (gamessAtomBasisFiles.size() != coordinates.size())
	{
		cout << "basis quantity and atoms quantity dont match" << endl;
		exit(1);
	}

	//writing coordinates and basis
	for (size_t i = 0; i < coordinates.size(); i++)
	{
		gamessInput_ << coordinates[i].atomlabel << "  "
			<< getChargeFromAtom(coordinates[i].atomlabel) << "  "
			<< coordinates[i].x << "  "
			<< coordinates[i].y << "  "
			<< coordinates[i].z << "  "
			<< endl;
		for (size_t j = 0; j < atomBasis[i].size(); j++)
			gamessInput_ << atomBasis[i][j] << endl;
	}
	gamessInput_ << " $END" << endl;
	if (gamessEcpFiles.size() != 0)
	{
		gamessInput_ << " $ECP" << endl;
		for (size_t i = 0; i < atomEcp.size(); i++)
		{
			for (size_t j = 0; j < atomEcp[i].size(); j++)
				gamessInput_ << atomEcp[i][j] << endl;
		}

		gamessInput_ << " $END" << endl;
	}

	gamessInput_.close();
}

int WriteQuantumInput::getChargeFromAtom(string atomName)
{
	if (atomName == "H")
		return 1;
	else if (atomName == "He")
		return 2;
	else if (atomName == "Li")
		return 3;
	else if (atomName == "Be")
		return 4;
	else if (atomName == "B")
		return 5;
	else if (atomName == "C")
		return 6;
	else if (atomName == "N")
		return 7;
	else if (atomName == "O")
		return 8;
	else if (atomName == "F")
		return 9;
	else if (atomName == "Ne")
		return 10;
	else if (atomName == "Na")
		return 11;
	else if (atomName == "Mg")
		return 12;
	else if (atomName == "Al")
		return 13;
	else if (atomName == "Si")
		return 14;
	else if (atomName == "P")
		return 15;
	else if (atomName == "S")
		return 16;
	else if (atomName == "Cl")
		return 17;
	else if (atomName == "Ar")
		return 18;
	else if (atomName == "K")
		return 19;
	else if (atomName == "Ca")
		return 20;
	else if (atomName == "Sc")
		return 21;
	else if (atomName == "Ti")
		return 22;
	else if (atomName == "V")
		return 23;
	else if (atomName == "Cr")
		return 24;
	else if (atomName == "Mn")
		return 25;
	else if (atomName == "Fe")
		return 26;
	else if (atomName == "Co")
		return 27;
	else if (atomName == "Ni")
		return 28;
	else if (atomName == "Cu")
		return 29;
	else if (atomName == "Zn")
		return 30;
	else if (atomName == "Ga")
		return 31;
	else if (atomName == "Ge")
		return 32;
	else if (atomName == "As")
		return 33;
	else if (atomName == "Se")
		return 34;
	else if (atomName == "Br")
		return 35;
	else if (atomName == "Kr")
		return 36;
	else if (atomName == "Rb")
		return 37;
	else if (atomName == "Sr")
		return 38;
	else if (atomName == "Y")
		return 39;
	else if (atomName == "Zr")
		return 40;
	else if (atomName == "Nb")
		return 41;
	else if (atomName == "Mo")
		return 42;
	else if (atomName == "Tc")
		return 43;
	else if (atomName == "Ru")
		return 44;
	else if (atomName == "Rh")
		return 45;
	else if (atomName == "Pd")
		return 46;
	else if (atomName == "Ag")
		return 47;
	else if (atomName == "Cd")
		return 48;
	else if (atomName == "In")
		return 49;
	else if (atomName == "Sn")
		return 50;
	else if (atomName == "Sb")
		return 51;
	else if (atomName == "Te")
		return 52;
	else if (atomName == "I")
		return 53;
	else if (atomName == "Xe")
		return 54;
	else
	{
		cout << "ATOM NOT REGISTERED CONTACT DEVELOPERS" << endl;
		exit(1);
	}
}










void WriteQuantumInput::buildMopacInput(
	vector<CoordXYZ> &allAtoms,
	string inputName)
{
	inputName += ".mop";
	remove(inputName.c_str());
	ofstream mopInput_(inputName.c_str());

	if (!(externalParams == ""))
		mopInput_ << "EXTERNAL=" + externalParams + ".inp  +" << endl;

	mopInput_ << mopacHeader << endl;
	mopInput_ << projectName << endl << endl;

	if (!(centralMetal == ""))
		mopInput_ << centralMetal << " 0.0   0   0.0   0   0.0   0 " << endl;

	for (size_t j = 0; j < allAtoms.size(); j++)
	{
		mopInput_ << allAtoms[j].atomlabel << "   "
			<< setprecision(16) << allAtoms[j].x << "  1  "
			<< setprecision(16) << allAtoms[j].y << "  1  "
			<< setprecision(16) << allAtoms[j].z << "  1  "
			<< endl;
	}
	mopInput_.close();
}


