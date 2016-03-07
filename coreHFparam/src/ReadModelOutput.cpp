#include "ReadModelOutput.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

ReadModelOutput::ReadModelOutput(string inputName) 
{
	unconverged = false;
	ifstream readModel_(inputName.c_str());

	string auxline;
	bool failed = true;
	while (getline(readModel_, auxline))
	{
		if (auxline.find("END OF INPUT") != string::npos)
		{
			failed = false;
			getline(readModel_, auxline);
			break;
		}
	}

	if (failed)
	{
		cout << "Error on: ReadModelOutput::ReadModelOutput" << endl
			<< "This shouldn't have to happen - contact developers" << endl;
		exit(1);
	}


	while (getline(readModel_, auxline))
	{
		if (auxline.find("scf unconverged") != string::npos)
		{
			unconverged = true;
			break;
		}
		if (auxline.find("energy") != string::npos)
		{
			readModel_ >> energy;
		}
		if (auxline.find("coordinates") != string::npos)
		{
			int size;
			readModel_ >> size;
			coord.resize(size);
			for (int i = 0; i < size; i++)
			{
				readModel_ >> coord[i];
			}
		}
		if (auxline.find("dipole") != string::npos)
		{
			dipole.resize(4);
			readModel_ >> dipole[0];
			readModel_ >> dipole[1];
			readModel_ >> dipole[2];
			readModel_ >> dipole[3];
		}
		if (auxline.find("ionization") != string::npos)
		{
			readModel_ >> ioniz;
		}
	}
}

ReadModelOutput::~ReadModelOutput() {}

