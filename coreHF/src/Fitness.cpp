#include "Fitness.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>

#include "WriteGamessInput.h"
#include "ReadGamessOutput.h"

using namespace std;

Fitness::Fitness(){}

void Fitness::setGamessObjects(
	WriteGamessInput * writeInput_in_,
	ReadGamessOutput * readOut_in_)
{
	writeInput_ = writeInput_in_;
	readOut_ = readOut_in_;
}

Fitness::~Fitness(){}

double Fitness::fit(vector<double> &point, int type)
{
	switch (type)
	{
	case 0:
		return point[0] * point[0];
		break;

	case 1:
		return lennardJones(point);
		break;

	case 2:
		return runGamess(point);
		//version and cpus
		break;

	default:
		cout << "FITNESS FUNCTION NOT FOUND" << endl;
		exit(3);
	}
}

double Fitness::lennardJones(vector<double> &x)
{
	// x1 x2 x3 ... y1 y2 y3 ... z1 z2 z3
	int natm = x.size() / 3;
	double r, r2, r4, r6, r12;
	double vlj = 0.0e0;
	for (int i = 0; i < (natm - 1); i++)
	{
		for (int j = (i + 1); j < natm; j++)
		{
			r = sqrt(
				(x[i] - x[j])*(x[i] - x[j]) +
				(x[i + natm] - x[j + natm])*(x[i + natm] - x[j + natm]) +
				(x[i + 2 * natm] - x[j + 2 * natm])*(x[i + 2 * natm] - x[j + 2 * natm])
				);
			r2 = r * r;
			r4 = r2 * r2;
			r6 = r4 * r2;
			r12 = r6 * r6;
			vlj += 4.0e0 * (-1 / r6 + 1 / r12);
		}
	}
	return vlj;
}

double Fitness::runGamess(vector<double> &point)
{
	string gamessInput;
	string rodaGamess, removeScr;

	gamessInput = writeInput_->createInput(point, 0);

	rodaGamess = writeInput_->getGamessPah() + "  " +
		gamessInput + ".inp" + "  00  " +
		writeInput_->getNProc() +
		"  >  " +
		gamessInput + ".log";

	removeScr = writeInput_->getGamessScr() + gamessInput + ".dat";

	remove(removeScr.c_str());

	system(rodaGamess.c_str());

	readOut_->readOptimizationGamess(gamessInput + ".log");

	//molecular mechanics
	writeInput_->calculateMMCorrection(point);
	vector<double> gradMM = writeInput_->getMMGradient();
	readOut_->setMMEnergyGradient(
		writeInput_->getMMEnergy(),
		gradMM);

	return readOut_->getEnergy();
}


