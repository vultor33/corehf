#include "ReadQuantumOptions.h"

#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

ReadQuantumOptions::~ReadQuantumOptions() {}

ReadQuantumOptions::ReadQuantumOptions()
{
	ifstream input_(inputName.c_str());
	string auxline;
	while (getline(input_, auxline))
	{
		stringstream line;
		string auxOption;
		line << auxline;
		line >> auxOption;
		if (auxOption == "end")
			break;

		options.push_back(auxOption);
	}
	input_.close();
}

vector<string> ReadQuantumOptions::getOptions()
{
	return options;
}

