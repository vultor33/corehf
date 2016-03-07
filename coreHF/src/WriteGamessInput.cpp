#include "WriteGamessInput.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>

using namespace std;

WriteGamessInput::WriteGamessInput(){}

WriteGamessInput::~WriteGamessInput() {}

void WriteGamessInput::startWriteGamess(const vector<string> &pathOfBases,
										const string &gamessOptions_in,
										const vector<int> &individualTypes_in,
										string projectName_in,
										string nProc_in,
										string gamessPath_in,
										string gamessScr_in,
										vector<double> &gaussianCoeff_in
	)
{
	mmCorrection_.setParameters(gaussianCoeff_in);

	nProc = nProc_in;
	gamessPath = gamessPath_in;
	gamessScr = gamessScr_in;
	gamessOptions_ = gamessOptions_in;
	individualTypes = individualTypes_in;
	projectName = projectName_in;

	int nAtomTypes = (int) pathOfBases.size();
	readEcp=false;
	startVectorStrings(nAtomTypes);
	for(int i=0; i<nAtomTypes; i++)
		readBaseType(i,pathOfBases);
}

string WriteGamessInput::createInput(const vector<double> &coordinates, int individual)
{
	// managing names
	stringstream convertInd;
	convertInd << individual;
	string inputName;
	string inputNameToWrite;
	convertInd >> inputName;
	inputName = projectName+inputName;
	inputNameToWrite = inputName + ".inp";
	remove(inputNameToWrite.c_str());

	ofstream gamessInput_;
	gamessInput_.open(inputNameToWrite.c_str());
	gamessInput_ << gamessOptions_;

	int natoms = (int) coordinates.size()/3;
	int tPos = 0;
	vector<int> tempTypes = individualTypes;
	for(int i =0; i<natoms; i++)
	{
		if(tPos>(int)tempTypes.size())
		{
			cout << "ERROR ON:WriteGamessInput::createInput"
				<< " this doesn't have to happen" << endl;
			exit(1);
		}
		if(tempTypes[tPos]==0)
		{
			tPos++;
		}
		else
			tempTypes[tPos]--;

		//tPos is the type of atom
		stringstream auxConverter;
		auxConverter << atomName[tPos]+"   "
		    << coordinates[i] << "  " 
			<< coordinates[i + natoms] << "  "
			<< coordinates[i + 2*natoms];
		string xyzLine;
		getline(auxConverter,xyzLine);

		gamessInput_ << xyzLine << endl;
		gamessInput_ << baseCoefficients[tPos] << endl;
	}
	// end of coordinates and bases
	gamessInput_ << " $END " << endl;

	//complicacao se o read ecp estiver ativo
	gamessInput_.close();

	return inputName;
}


void WriteGamessInput::startVectorStrings(int nAtomTypes)
{
	atomName.resize(nAtomTypes);
	baseCoefficients.resize(nAtomTypes);
	if(readEcp)
	{
		ecpFlag.resize(nAtomTypes);
		ecpCoefficients.resize(nAtomTypes);
	}

	for(int i = 0; i<nAtomTypes; i++)
	{
		atomName[i]="";
		baseCoefficients[i] = "";
		if(readEcp)
		{
			ecpFlag[i] = "";
			ecpCoefficients[i] = "";
		}
	}
}

void WriteGamessInput::readBaseType(int baseType, const vector<string> &pathOfBases)
{
	string basePath = pathOfBases[baseType];
	basePath+=".txt";

	ifstream readBase_;
	readBase_.open(basePath.c_str());
	if(!readBase_.is_open())
	{
		cout << "ERROR ON: WriteGamessInput" << endl
			<< "Archive not found in: "
			<< basePath << endl;
		exit(1);
	}

	string auxline;
	getline(readBase_,auxline);
	if(auxline.size()==0)
	{
		cout << "ERROR ON: WriteGamessInput" << endl
			<< "First line empty, probably all file as well "
			<< endl;
		exit(1);
	}

	auxline.erase(auxline.length()-1);//removing \n

	atomName[baseType]+=auxline;

	readBaseI(readBase_, baseCoefficients, baseType);

	readBase_.close();
}


void WriteGamessInput::readBaseI(ifstream &readBase_, vector<string> &baseCoefficients, int baseType)
{
	string auxline;
	while (getline(readBase_, auxline))
	{
		if (auxline.size() == 0)
			break;

		auxline += "\n";
		baseCoefficients[baseType] += auxline;
	}
}

void WriteGamessInput::readEcpFromFile(std::ifstream &readBase_, std::vector< std::string > &baseCoefficients, int baseType)
{
	string auxline;
	if (readEcp)
	{
		getline(readBase_, auxline);
		auxline += "\n";
		ecpFlag[baseType] = auxline;

		getline(readBase_, auxline);//empty
		getline(readBase_, auxline);
		while (!readBase_.eof())
		{
			auxline += "\n";
			ecpCoefficients[baseType] += auxline;
			getline(readBase_, auxline);
			if (auxline.size() == 0)
				break;
		};
	}
}


void WriteGamessInput::changeExpoentsAndContractions(
	int baseType, 
	const vector<double>& expoentsAndContractions)
{
	//o cara tem que saber o que esta fazendo, mas vou ajudar um pouco
	stringstream convert;
	convert << baseCoefficients[baseType];

	string aux;
	string newBase = "";
	int contracCount = 1;
	string auxNumber, auxExpoent, auxContrac;

	int k = 0; //run through all expoents and contractions
	while (getline(convert, aux))
	{
		if (checkNumber(aux))
		{
			//add numbers to new base
			stringstream ss;
			ss << contracCount;
			ss >> auxNumber;
			stringstream ss1;
			ss1 << expoentsAndContractions[k];
			ss1 >> auxExpoent;
			stringstream ss2;
			ss2 << expoentsAndContractions[k+1];
			ss2 >> auxContrac;
		
			newBase += auxNumber + 
				"   " +
				auxExpoent + 
				"   " + 
				auxContrac +
				"\n";

			k += 2;
			contracCount++;
		}
		else
		{
			newBase += aux + "\n";
			contracCount = 1;
		}

		if (k > (int)(expoentsAndContractions.size()+1))
		{
			cout << "ERROR on WriteGamessInput::changeExpoentsAndContractions"
				<< endl;
			exit(1);
		}
	}

	baseCoefficients[baseType] = newBase;
}

bool WriteGamessInput::checkNumber(string base)
{
	int i = 0;
	while (isspace(base[i]))
	{
		i++;
	}
	if (isdigit(base[i]) == 0)
		return false;
	else
		return true;
}


void WriteGamessInput::calculateMMCorrection(vector<double> &point)
{
	mmCorrection_.calculateCorrection(point);
}

double WriteGamessInput::getMMEnergy()
{
	return mmCorrection_.getEnergy();
}

vector<double> WriteGamessInput::getMMGradient()
{
	return mmCorrection_.getGradient();
}

