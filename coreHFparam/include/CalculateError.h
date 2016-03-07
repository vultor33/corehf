#ifndef CALCULATEERROR_H
#define CALCULATEERROR_H

#include <vector>

// passar coordenadas, dipolos, potencial de ionizacao e energia

class CalculateError
{
public:
	CalculateError();
	~CalculateError();

	double calculate(int model, std::vector<double> &x, std::vector<double> &dipole, double energy, double potIoniz);

private:
	void setSystems(int model);
	double refEnergy;
	double refIonizPot;
	std::vector<double> refDistances;
	std::vector<double> refDipole;

	std::vector<double> calculateDistance(std::vector<double> &x);

	double calculateDistanceError(std::vector<double> &dist);
	double calculateDipoleError(std::vector<double> &dipole);

};


#endif
