

#ifndef BETA_H
#define	BETA_H

#include "FermiDistribution.hh"
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include "TF1.h"
#include "G4ThreeVector.hh"


class Beta {
public:

	Beta(double maxEnergy, int atomicNumber, double polarization, double assymetryFactor);
	//Beta(double maxEnergy, int atomicNumber, double polarization, TransitionType transitionType, int deltaI, int initialSpin);
	virtual ~Beta();
    
	void GetRandomBeta(float* energy, G4ThreeVector* direction);	
	enum class TransitionType { FF, GT };

private:
	void MakeBetaSpectrum();
	void SetAssymetryFactor(Beta::TransitionType transitionType, int deltaI, int initialSpin);	
	double FindVelocity(double energyinMeV);
	TF1* FindAsymDistrFunc(double asymFactor, double velocity, double polarization);
	void  GenerateAsymDirection(G4ThreeVector* direction, TF1* asymDistr);
	int FindRandomIndex();
	double maxBetaEnergy;
	int atomicNumber;
	double polarization;
	double assymetryFactor;
	FermiDistribution *betaEnergyDistribution;
	std::vector<float> energy;
	std::vector<float> probability;
	std::vector<TF1*> directions;
};

#endif	/* Beta_H */

