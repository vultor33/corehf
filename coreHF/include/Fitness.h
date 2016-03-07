#ifndef FITNESS_H
#define FITNESS_H

#include "WriteGamessInput.h"
#include "ReadGamessOutput.h"

#include <vector>

class Fitness
{
public:
	Fitness();
	~Fitness();

	void setGamessObjects(
		WriteGamessInput * writeInput_in_,
		ReadGamessOutput * readOut_in_
		);

	double fit(std::vector<double> &point, int type);

private:
	double lennardJones(std::vector<double> &x);

	double runGamess(std::vector<double> &point);

	WriteGamessInput * writeInput_;
	ReadGamessOutput * readOut_;

};

#endif
