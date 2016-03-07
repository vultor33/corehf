#include "ReadGamessOutput.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

ReadGamessOutput::ReadGamessOutput(ofstream &logFile_in_)
:logFile_(logFile_in_)
{
	xyzCoordinatesFlag = "NOT AVAIBLE";
	energyFlag = "TOTAL ENERGY =";
	gradientFlag = "UNITS ARE HARTREE/BOHR";
	ionizationFlag = "EIGENVECTORS";
	dipoleFlag = "ELECTROSTATIC MOMENTS";
}

ReadGamessOutput::~ReadGamessOutput(){}

double ReadGamessOutput::getEnergy()
{
	return gamessEnergy + MMEnergy;
}

vector<double> ReadGamessOutput::getGradient()
{
	for (int i = 0; i < (int)gamessGradient.size(); i++)
	{
		gamessGradient[i] += MMGradient[i];
	}
	return gamessGradient;
}

vector<double> ReadGamessOutput::getCoordinates()
{
	return finalCoordinates;
}

vector<double> ReadGamessOutput::getDipole()
{
	return dipole;
}

void ReadGamessOutput::readOptimizationGamess(std::string outputToRead)
{
	ifstream gamessOut_;
	gamessOut_.open(outputToRead.c_str());
	if(!gamessOut_.is_open())
	{
		cout << "ERROR ON: ReadGamessOutput" << endl
			<< " Archive not found" << endl;
		exit(1);
	}

	string auxline;
	bool auxFoundEnergy;
	bool auxFoundGradient;
	bool auxFoundCoordinate;
	bool auxFoundIonization;
	bool auxFoundDipole;

	// FAIL VALUES
	double auxEnergy = 100.0e0;
	vector<double> auxCoordinates;
	double auxIonization = 10000.0e0;
	vector<double> auxDipole(4);
	auxDipole[0] = 100;
	auxDipole[1] = 100;
	auxDipole[2] = 100;
	auxDipole[3] = 100;
	vector<double> auxGradient;

	while(getline(gamessOut_, auxline))
	{	
		auxFoundEnergy = (auxline.find(energyFlag)!=string::npos);
		if(auxFoundEnergy)
		{
			auxEnergy = getEnergyFromString(auxline);
		}
		auxFoundCoordinate = (auxline.find(xyzCoordinatesFlag)!=string::npos);
		if(auxFoundCoordinate)
		{
			auxCoordinates = getCoordinatesFromFile(gamessOut_);
		}
		auxFoundGradient = (auxline.find(gradientFlag) != string::npos);
		if (auxFoundGradient)
		{
			auxGradient = getGradientFromFile(gamessOut_);
		}
		auxFoundIonization = (auxline.find(ionizationFlag) != string::npos);
		if (auxFoundIonization)
		{
			auxIonization = getIonizationFromFile(gamessOut_);
		}
		auxFoundDipole = (auxline.find(dipoleFlag) != string::npos);
		if (auxFoundDipole)
		{
			auxDipole = getDipoleFromFile(gamessOut_);
		}
	}
	gamessOut_.close();

	if (auxGradient.size() == 0)
	{
		logFile_ << "scf unconverged" << endl;
		logFile_.close();
		exit(0);
	}

	gamessGradient = auxGradient;
	finalCoordinates = auxCoordinates;
	gamessEnergy = auxEnergy;
	ionizationPotential = -auxIonization;
	dipole = auxDipole;
}

double ReadGamessOutput::getEnergyFromString(std::string auxline)
{
	stringstream line;
	line << auxline;

	string aux1;
	string aux2;
	string aux3;
	double energy;

	line >> aux1 >> aux2 >> aux3 >> energy;

	return energy;
}

vector<double> ReadGamessOutput::getCoordinatesFromFile(std::ifstream &gamessOut_)
{
	string auxline;
	getline(gamessOut_,auxline);//dummy line
	getline(gamessOut_,auxline);//dummy line

	bool auxFinished;
	double x,y,z;
	string aux1,aux2;
	vector<double> auxCoordinates;

	getline(gamessOut_,auxline);
	auxFinished = auxline.find_first_not_of("1234567890– ")!= string::npos;
	while(auxFinished)
	{
		stringstream line;
		line << auxline;
		line >> aux1 >> aux2 >> x >> y >> z;
		auxCoordinates.push_back(x);
		auxCoordinates.push_back(y);
		auxCoordinates.push_back(z);

		getline(gamessOut_,auxline);
		auxFinished = auxline.find_first_not_of("1234567890– ")!= string::npos;
	}

	int natm = auxCoordinates.size() / 3;
	vector<double> coordinates(3 * natm);
	int k = 0;
	for (int i = 0; i < natm; i++)
	{
		coordinates[i] = auxCoordinates[k];
		coordinates[i + natm] = auxCoordinates[k + 1];
		coordinates[i + 2 * natm] = auxCoordinates[k + 2];
		k += 3;
	}

	return coordinates;
}

vector<double> ReadGamessOutput::getGradientFromFile(std::ifstream &gamessOut_)
{
	string auxline;
	bool auxFinished;
	double x, y, z;
	string aux1, aux2;
	vector<double> auxGradient;

	getline(gamessOut_, auxline);
	auxFinished = auxline.find_first_not_of("1234567890– ") != string::npos;
	while (auxFinished)
	{
		stringstream line;
		line << auxline;
		line >> aux1 >> aux2 >> x >> y >> z;
		auxGradient.push_back(x);
		auxGradient.push_back(y);
		auxGradient.push_back(z);

		getline(gamessOut_, auxline);
		auxFinished = auxline.find_first_not_of("1234567890– ") != string::npos;
	}
	int natm = auxGradient.size() / 3;
	vector<double> gradient(3 * natm);
	int k = 0;
	for (int i = 0; i < natm; i++)
	{
		gradient[i] = auxGradient[k];
		gradient[i + natm] = auxGradient[k+1];
		gradient[i + 2*natm] = auxGradient[k+2];
		k += 3;
	}
	return gradient;
}


double ReadGamessOutput::getIonizationFromFile(std::ifstream &gamessOut_)
{
	string auxline;
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);

	bool auxRead = true;
	int k = ionizationPos;
	while (getline(gamessOut_, auxline))
	{
		if (auxRead)
		{
			if (k < 6)
			{
				return readnDoubles(auxline, k);
			}
			else
			{
				k -= 5;
				auxRead = false;
			}
		}
		else
		{
			if (auxline.size() == 0)
			{
				getline(gamessOut_, auxline);
				auxRead = true;
			}
		}
	}
	return 0.0e0;
}


double ReadGamessOutput::readnDoubles(string auxline, int nEigens)
{
	stringstream convert;
	convert << auxline;
	double a1, a2, a3, a4, a5;
	switch(nEigens)
	{
	case 1:
		convert >> a1;
		return a1;
		break;
		
	case 2:
		convert >> a1 >> a2;
		return a2;
		break;

	case 3:
		convert >> a1 >> a2 >> a3;
		return a3;
		break;

	case 4:
		convert >> a1 >> a2 >> a3 >> a4;
		return a4;
		break;

	case 5:
		convert >> a1 >> a2 >> a3 >> a4 >> a5;
		return a5;
		break;

	default:
		cout << "Error in readnDoubles - contact developers" << endl;
		exit(1);
	}
	return 0.0e0;
}

void ReadGamessOutput::rePrintReading()
{
	cout << "reprinting gamess output" << endl;
	cout << "final Energy:           " << gamessEnergy << endl;
	cout << "ionization potential:   " << ionizationPotential << endl;
	cout << "dipole (D, dx, dy, dz):" << endl;
	cout << dipole[0] << "   "
		<< dipole[1] << "   "
		<< dipole[2] << "   "
		<< dipole[3] << "   "
		<< endl;

	int natm = gamessGradient.size() / 3;
	cout << "gradient " << endl;
	for (int i = 0; i < natm; i++)
	{
		cout << "  " << gamessGradient[i]
			<< "  " << gamessGradient[i + natm]
			<< "  " << gamessGradient[i + 2 * natm]
			<< endl;
	}
}


vector<double> ReadGamessOutput::getDipoleFromFile(std::ifstream & gamessOut_)
{
	vector<double> auxDipole(4);
	double dx, dy, dz, d;
	string auxline;
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);
	getline(gamessOut_, auxline);

	stringstream convert;
	convert << auxline;
	convert >> dx >> dy >> dz >> d;

	auxDipole[0] = d;
	auxDipole[1] = dx;
	auxDipole[2] = dy;
	auxDipole[3] = dz;

	return auxDipole;
}


void ReadGamessOutput::setMMEnergyGradient(double MMEnergy_in, vector<double> &MMGradient_in)
{
	MMEnergy = MMEnergy_in;
	MMGradient = MMGradient_in;
}
