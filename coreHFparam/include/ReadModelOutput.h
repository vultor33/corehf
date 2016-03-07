#ifndef READMODELOUTPUT_H
#define READMODELOUTPUT_H

#include <string>
#include <vector>

class ReadModelOutput
{
public:
	ReadModelOutput(std::string inputName);
	~ReadModelOutput();

	double energy;
	std::vector<double> coord;
	std::vector<double> dipole;
	double ioniz;
	bool unconverged;

private:




};


#endif
