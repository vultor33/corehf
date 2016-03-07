#include "MolecularMechanicsCorrection.h"

#include <vector>
#include <cmath>

using namespace std;

MolecularMechanicsCorrection::MolecularMechanicsCorrection(){}

MolecularMechanicsCorrection::~MolecularMechanicsCorrection(){}

void MolecularMechanicsCorrection::setParameters(vector<double> &parameters_in)
{
	parameters = parameters_in;
}

vector<double> MolecularMechanicsCorrection::getGradient()
{
	return gradient;
}

void MolecularMechanicsCorrection::calculateCorrection(vector<double> &x)
{
	// x1 x2 x3 ... y1 y2 y3 ... z1 z2 z3
	int natm = x.size() / 3;
	if(gradient.size() == 0)
		gradient.resize(natm * 3);
	for (int i = 0; i < 3 * natm; i++)
		gradient[i] = 0.0e0;
	energy = 0.0e0;

	if (parameters.size() == 0)	
		return;

	double r;
	double auxenergy = 0.0e0;
	double gauss, dgauss;
	for (int i = 0; i < (natm - 1); i++)
	{
		for (int j = (i + 1); j < natm; j++)
		{
			r = sqrt(
				(x[i] - x[j])*(x[i] - x[j]) +
				(x[i + natm] - x[j + natm])*(x[i + natm] - x[j + natm]) +
				(x[i + 2 * natm] - x[j + 2 * natm])*(x[i + 2 * natm] - x[j + 2 * natm])
				);
			
			//(zazb/r) * (gauss_atom1 + gauss_atom2)
			gauss = 2.0e0 * calculateEnergy(r);
			auxenergy = (1 / r) * gauss;
			energy += auxenergy;

			// gradient:  (-1/r) * energy + 2 * (1/r) * (2 b (r-c) * gauss)
			dgauss = 2.0e0 * calculateGradient(r);
			dgauss += (-1 / r) * auxenergy;

			gradient[i] += ((x[i] - x[j]) / r) * dgauss;
			gradient[i + natm] += ((x[i + natm] - x[j + natm]) / r) * dgauss;
			gradient[i + 2 * natm] += ((x[i + 2 * natm] - x[j + 2 * natm]) / r) * dgauss;
			gradient[j] -= ((x[i] - x[j]) / r) * dgauss;
			gradient[j + natm] -= ((x[i + natm] - x[j + natm]) / r) * dgauss;
			gradient[j + 2 * natm] -= ((x[i + 2 * natm] - x[j + 2 * natm]) / r) * dgauss;

		}
	}
}

double MolecularMechanicsCorrection::calculateEnergy(double r)
{
	double auxGauss = 0.0e0;
	int ngauss = parameters.size() / 3;
	double a, b, c;
	for (int i = 0; i < ngauss; i++)
	{
		a = parameters[3 * i];
		b = parameters[3 * i + 1];
		c = parameters[3 * i + 2];
		auxGauss += a * exp(-b * (r - c) * (r - c));
	}
	return auxGauss;
}

double MolecularMechanicsCorrection::calculateGradient(double r)
{
	double auxGauss = 0.0e0;
	int ngauss = parameters.size() / 3;
	double a, b, c;
	for (int i = 0; i < ngauss; i++)
	{
		a = parameters[3 * i];
		b = parameters[3 * i + 1];
		c = parameters[3 * i + 2];
		auxGauss += - 2.0e0 * b * (r-c) * 
			a * exp(-b * (r - c) * (r - c));
	}
	return auxGauss;
}

