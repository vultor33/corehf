#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include <vector>
#include "ReadGamessOutput.h"

class Derivative
{
public:
	Derivative();
	~Derivative();

	void setGamessObjects(ReadGamessOutput * readOut_in_);

	std::vector<double> Dfit(std::vector<double> &point, int type);

private:
//	std::vector<double> DlennardJones(std::vector<double> &x);
	ReadGamessOutput * readOut_;



};

#endif
