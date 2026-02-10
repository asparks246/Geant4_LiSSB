/* 
 * File:   FermiDistribution.cpp
 * Author: aleksandra
 * 
 * Created on 31 maj 2012, 14:57
 */

#include "FermiDistribution.hh"
#include "Exception.hh"
#include "MyTemplates.hh"
#include <iostream>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

FermiDistribution::FermiDistribution(int atomicNumber, double qVal, int eCharge):
atomicNumber_(atomicNumber), qVal_(qVal), eCharge_(eCharge) 
{
	if((eCharge_ -1) > 1e-6 && (eCharge_ +1) > 1e-6)
		throw Exception ("FermiDistribution::FermiDistribution wrong eCharge argument");
	CalculateBetaEnergyDistribution();
}

FermiDistribution::~FermiDistribution()
{
}

void FermiDistribution::CalculateBetaEnergyDistribution()
{
		/*Sampling the Fermi distribution for B-decay energy input to EGS4, W. R. Nelson, J. Liu, SLAC-TN_92-1   */
		nrOfkeVperChan_ = 10;
	 		
   gammaFinal_ = 0.9999999;//end of calculation euler gamma function
   alpha_ =1./ 137.;
   pi_ = 3.141592653589;//2*atan(1);
   electronMass_ = 511;
	
	
	std::cout<<"calculate beta energy distribution for E = "<<qVal_;
	nrOfSteps_ = static_cast<int> (qVal_) / nrOfkeVperChan_;
	v0_ = 1.13 * pow(alpha_, 2) * pow((atomicNumber_ - eCharge_), 4. / 3.); 
	S_ = pow((1 - alpha_ * alpha_ * atomicNumber_ * atomicNumber_), 0.5) - 1.;

	if(eCharge_ == 1)//beta plus
		xMin_ = 0.00;
	else //beta minus
		xMin_ = electronMass_ * v0_ / qVal_;
	xMax_ = 1.;


	double x, probabilityDensityValue;
	double totStepTimesProb (0); //needed for average energy calculation
	totProb_ = 0;
	maxBetaProbability_ = 0.;
	for (int i = 0; i < nrOfSteps_; ++i)
	{
		x = xMin_ + (xMax_ - xMin_) / nrOfSteps_*i;
		probabilityDensityValue = FindProbabilityDensityValue(x);
		betaEnergy_.push_back(x);
		energProbability_.push_back(probabilityDensityValue);

		if (maxBetaProbability_ < probabilityDensityValue)
			maxBetaProbability_ = probabilityDensityValue;
			
		totProb_ +=probabilityDensityValue;
		totStepTimesProb += probabilityDensityValue*x;
	}

	std::cout<< " average energy = " << totStepTimesProb/totProb_*qVal_ << std::endl;
}

double FermiDistribution::FindProbabilityDensityValue(double x)
{
	if(x < xMin_ || x > xMax_)
	{
		std::string message = num2string(x) + " is not within the range of [" 
		+ num2string(xMin_) + " , " +num2string(xMax_) + "] ";
		std::cout << message << std::endl;
		throw new Exception(message);
	}

	double probabilityDensityValue;
	double gamma0 = (1. + qVal_ * x / electronMass_) - eCharge_*v0_;
	double ni0 = pow((gamma0 * gamma0 - 1.), 0.5);
	if(gamma0 < 1)
		return 0;
	delta0_ = alpha_ * atomicNumber_ * gamma0 / ni0;
	double gammaValue = EulerGammaFunction(gammaFinal_);
	probabilityDensityValue = gamma0 * ni0 * pow((1. - x), 2) * pow(ni0, 2. * S_) * exp(-1. * eCharge_ * pi_ * delta0_) * gammaValue;
	double betaEn = qVal_ * x;
	double shapeFact = GetShapeCorrectionFactor(betaEn);
	return probabilityDensityValue*shapeFact;
}

double FermiDistribution::EulerGammaFunction(double finalValue)
{
	double gammaValue = 1;
	double i = 0.;
	double stepValue = 1. / (1. + pow(delta0_, 2) / pow((i + 1. + S_), 2));
	while (stepValue <= finalValue)
	{
		i = i + 1.;
		gammaValue *= stepValue;
		stepValue = 1. / (1. + pow(delta0_, 2) / pow((i + 1. + S_), 2));
	}
	gammaValue *= stepValue;
	return gammaValue;
}


double FermiDistribution::GetShapeCorrectionFactor(double betaEn)
{
/*
  F : Fermi Decay
  1-: pe^2  + Eν^2 + 2/3 β^2 * Eν * Ee
  GT : Gamow-Teller Decay
  0-: pe^2  + Eν^2 + 2β^2 * Eν * Ee
  1-: pe^2  + Eν^2 - 4/3 * β^2 * Eν * Ee
  2-: pe^2  + Eν^2 
  β = pe/Ee    

*/  
    double Ee = betaEn; 
    double Etote = Ee+511; 
    double Ev = qVal_ - Ee;
    double pe = pow( Etote * Etote - 511*511, 0.5 );
    double beta = pe/Ee;
    double s=1;
    s = pe * pe + Ev*Ev + 2.0/3.0 * beta * beta * Ev*Ee;//1- FF
//  s = pe * pe + Ev*Ev + 2.0 *  beta * beta * Ev*Ee;// 0- GT
//  s = pe * pe + Ev*Ev - 4.0 / 3.0 *  beta * beta * Ev*Ee;// 1- GT
//  s = pe * pe + Ev*Ev;// 2- GT
    //std::cout << betaEn << " " << s << std::endl;
    return s;
}


double FermiDistribution::GetRandomBetaEnergy()
{
	srand ( time(NULL) );
	if (energProbability_.size() == 0)
		throw Exception("FermiDistribution::getRandomBetaEnergy() energProbability_.size() == 0");
	int randomEnergyIndex = rand() % energProbability_.size();
	double randomProbability = (double) rand() / (double) RAND_MAX * maxBetaProbability_;
	while (randomProbability > energProbability_.at(randomEnergyIndex))
	{
		randomEnergyIndex = rand() % energProbability_.size();
		randomProbability = (double) rand() / (double) RAND_MAX * maxBetaProbability_;
	}
	return betaEnergy_.at(randomEnergyIndex)*qVal_;
}
