#ifndef DERIVATIVEDLIB_H
#define DERIVATIVEDLIB_H

#include <dlib/optimization.h>
#include <vector>

#include "Derivative.h"
#include "ReadGamessOutput.h"

typedef dlib::matrix<double, 0, 1> column_vector;

class DerivativeDlib
{
public:
	DerivativeDlib(int size_in, int fitType_in, ReadGamessOutput * readOut_in_)
	{
		size = size_in;
		fitType = fitType_in;
		readOut_ = readOut_in_;
	}

	~DerivativeDlib(){}

	const column_vector operator() (const column_vector& arg) const
	{
		std::vector<double> x(size);
		for (int i = 0; i < size; i++)
		{
			x[i] = arg(i);
		}

		Derivative dfit_;
		dfit_.setGamessObjects(readOut_);

		std::vector<double> dxvector = dfit_.Dfit(x, fitType);

		column_vector dxcolumn(size);
		for (int i = 0; i < size; i++)
		{
			dxcolumn(i) = dxvector[i];
		}

		return  dxcolumn;
	}

private:
	int size;
	int fitType;
	ReadGamessOutput * readOut_;
};

#endif
