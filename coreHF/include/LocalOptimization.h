#ifndef LOCALOPTIMIZATION_H
#define LOCALOPTIMIZATION_H

#include <vector>

#include "ReadGamessOutput.h"
#include "WriteGamessInput.h"

class LocalOptimization
{
public:
	LocalOptimization();
	~LocalOptimization();

	void optimize(std::vector<double> &x, int fitType);

	// and change dlib
	void includeGamessAuxObjects(WriteGamessInput * writeInput_in_, ReadGamessOutput * readOut_in_);
private:
	WriteGamessInput * writeInput_;
	ReadGamessOutput * readOut_;

};

#endif
