#ifndef FUNCTIONDLIB_H
#define FUNCTIONDLIB_H

#include <dlib/optimization.h>
#include <vector>

#include "Fitness.h"
#include "WriteGamessInput.h"
#include "ReadGamessOutput.h"

typedef dlib::matrix<double, 0, 1> column_vector;

class FunctionDlib
{
public:
	FunctionDlib(int size_in, 
		int fitType_in,
		WriteGamessInput * writeInput_in_,
		ReadGamessOutput * readOut_in_
		)
	{
		size = size_in;
		fitType = fitType_in;
		writeInput_ = writeInput_in_;
		readOut_ = readOut_in_;
	}

	~FunctionDlib(){}

	double operator() (const column_vector& arg) const
	{
		std::vector<double> x(size);
		for (int i = 0; i < size; i++)
		{
			x[i] = arg(i);
		}

		Fitness fit_;
		fit_.setGamessObjects(writeInput_, readOut_);

		return  fit_.fit(x, fitType);
	}

private:
	int size;
	int fitType;

	WriteGamessInput * writeInput_;
	ReadGamessOutput * readOut_;
};

#endif





