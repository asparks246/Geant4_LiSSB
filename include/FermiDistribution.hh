/*
 * FermiDistribution.h
 *
 *  Created on: 28-03-2014
 *      Author: aleksandra
 */

#ifndef FERMIDISTRIBUTION_H_
#define FERMIDISTRIBUTION_H_
#include <vector>
#include <cmath>
class FermiDistribution {
public:
	FermiDistribution(int atomicNumber, double qVal, int eCharge);
	virtual ~FermiDistribution();


    /**
     * Calculation of beta energy distribution (works for b+ and b-)
     * Algorithm:
     * Sampling the Fermi distribution for B-decay energy input to EGS4, W. R. Nelson, J. Liu, SLAC-TN_92-1
     * eCharge - possible only +/- 1 value. +1 - beta plus, -1 - beta minus
     */


    /** Get beta energies in keV
    return betaEnergy_ * qVal_ **/
    std::vector<float> GetEnergyVector()
    {
    	std::vector<float> energy;
    	std::vector<double>::iterator it = betaEnergy_.begin();
    	for(; it != betaEnergy_.end(); ++it)
    		energy.push_back((*it)*qVal_);
    	return energy;
    }

    std::vector<float> GetProbabilityVector()
    {
    	std::vector<float> probability;
    	std::vector<double>::iterator it = energProbability_.begin();
    	for(; it != energProbability_.end(); ++it)
    		probability.push_back((*it)/totProb_);
    	return probability;
    }

	double GetRandomBetaEnergy();
private:
    void CalculateBetaEnergyDistribution();
    double FindProbabilityDensityValue(double x);
    double GetShapeCorrectionFactor(double energy);
    double EulerGammaFunction (double finalValue);
    int atomicNumber_;
    double qVal_;

    /** beta energy distribution:
    betaEnergy_ - energy from 0 to 1
    to get energy in keV value has to be multiplied by qVal
    energProbability_ - corresponding probability**/
    std::vector<double> betaEnergy_;
    std::vector<double> energProbability_;

    int nrOfSteps_ ;
    double totProb_;

    int nrOfkeVperChan_;
    double gammaFinal_ ;//end of calculation euler gamma function
    double alpha_ ;
    double pi_ ;//2*atan(1);
    double electronMass_ ;


    int eCharge_ ;//+1 beta plus, -1 beta minus
    double v0_;
    double S_;
    double xMin_;
    double xMax_;
    double maxBetaProbability_;
    double delta0_;
};

#endif /* FERMIDISTRIBUTION_H_ */
