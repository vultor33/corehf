#ifndef READQUANTUMOPTIONS_H
#define READQUANTUMOPTIONS_H

#include <string>
#include <vector>

class ReadQuantumOptions
{
public:
	ReadQuantumOptions();
	~ReadQuantumOptions();

	std::vector<std::string> getOptions();

private:
	std::vector<std::string> options;
	std::string inputName = "quantumOptions.inp";

};


#endif

