#ifndef MOLECULARMECHANICSCORRECTION_H
#define MOLECULARMECHANICSCORRECTION_H

#include <vector>

class MolecularMechanicsCorrection
{
public:
	MolecularMechanicsCorrection();
	~MolecularMechanicsCorrection();

	void setParameters(std::vector<double> &parameters_in);

	void calculateCorrection(std::vector<double> &point);

	inline double getEnergy() { return energy; }
	std::vector<double> getGradient();

private:
	std::vector<double> parameters; //a1 b1 c1 ...
	double energy;
	std::vector<double> gradient;

	double calculateEnergy(double r);
	double calculateGradient(double r);

};

#endif
