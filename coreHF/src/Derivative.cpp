#include "Derivative.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>

using namespace std;

Derivative::Derivative(){}

Derivative::~Derivative(){}

vector<double> Derivative::Dfit(vector<double> &point, int type)
{
	vector<double> deri;
	switch (type)
	{
	case 0:
		deri.resize(1);
		deri[0] = 2.0e0 * point[0];
		return deri;
		break;

//	case 1:
//		return lennardJones(point);
//		break;

	case 2:
		deri = readOut_->getGradient();
		return deri;
		break;

	default:
		cout << "FITNESS FUNCTION NOT FOUND" << endl;
		exit(3);
	}
}

void Derivative::setGamessObjects(ReadGamessOutput * readOut_in_)
{
	readOut_ = readOut_in_;
}

