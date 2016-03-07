#include "LocalOptimization.h"

#include <dlib/optimization.h>
#include <iostream>

#include "FunctionDlib.h"
#include "DerivativeDlib.h"

using namespace std;
using namespace dlib;

LocalOptimization::LocalOptimization(){}

LocalOptimization::~LocalOptimization(){}

void LocalOptimization::optimize(std::vector<double> &x, int fitType)
{
	int size = x.size();
	column_vector starting_point(size);
	for (int i = 0; i < size; i++)
	{
		starting_point(i) = x[i];
	}

	find_min(bfgs_search_strategy(),
		objective_delta_stop_strategy(1e-6),
		FunctionDlib(size, fitType, writeInput_, readOut_),
		DerivativeDlib(size,fitType, readOut_), 
		starting_point, 
		-1.0e99);
}

void LocalOptimization::includeGamessAuxObjects(
	WriteGamessInput * writeInput_in_,
	ReadGamessOutput * readOut_in_)
{
	writeInput_ = writeInput_in_;
	readOut_ = readOut_in_;
}


/*
NUMERICAL DERIVATIVE

find_min_using_approximate_derivatives(
bfgs_search_strategy(),
objective_delta_stop_strategy(1.0e-6
),
FunctionDlib(size, fitType),
starting_point,
-1.0e0
);
*/
