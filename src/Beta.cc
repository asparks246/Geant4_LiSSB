
#include "Beta.hh"
#include "Randomize.hh"
#include <assert.h>

Beta::Beta(double maxEnergyVal, int atomicNumberVal, double polarizationVal, double assymetryFactorVal)
    :maxBetaEnergy(maxEnergyVal), atomicNumber(atomicNumberVal), polarization(polarizationVal), assymetryFactor(assymetryFactorVal)
{

    betaEnergyDistribution = new FermiDistribution(atomicNumber, maxBetaEnergy, -1);
	MakeBetaSpectrum();

}

/*Beta::Beta(double maxEnergy, int atomicNumber, double polarizationVal, TransitionType transitionType, int deltaI, int initialSpin)
    :maxBetaEnergy(maxEnergyVal), atomicNumber(atomicNumberVal), polarization(polarizationVal)
{

    betaEnergyDistribution = new FermiDistribution(atomicNumber, maxBetaEnergy, -1);
    SetAssymetryFactor(transitionType,  deltaI,  initialSpin);
	MakeBetaSpectrum();

}*/

Beta::~Beta()
{
	if(betaEnergyDistribution != 0L)
		delete betaEnergyDistribution;
}


void Beta::MakeBetaSpectrum()
{		
	energy = betaEnergyDistribution->GetEnergyVector();
	probability = betaEnergyDistribution->GetProbabilityVector();
	assert(energy.size() == probability.size() && energy.size() != 0);

    for(unsigned int i=0; i!=energy.size(); ++i)
    {
        double velocity = FindVelocity(energy.at(i));
        TF1* distr = FindAsymDistrFunc(assymetryFactor, velocity, polarization);
        directions.push_back(distr);
    }
}



void Beta::SetAssymetryFactor(Beta::TransitionType transitionType, int deltaI, int initialSpin)
{
   if(deltaI != 0 && deltaI != -1 && deltaI != 1)
   {
       std::cout << "FindAsymmertyFactor designed only for allowed beta transitions. "
                 << "Possible Delta I = -1, 0, 1" << std::endl;
   }
   double asymFact = 0;
   switch(transitionType)
   {
       case TransitionType::FF: 
           asymFact = 0.;   
           break;
       case TransitionType::GT: 
           if(deltaI == -1)
               asymFact = -1.;
           if(deltaI == 1)
           {
              asymFact = (double)initialSpin/(initialSpin+1.);
           }
           if(deltaI == 0)
           {
              asymFact = -1./(initialSpin+1.);
           }
           break;
   }
   assymetryFactor= asymFact;
}




void  Beta::GenerateAsymDirection(G4ThreeVector* direction, TF1* asymDistr)
{
   double cosTheta = ( G4UniformRand() - 0.5 ) * 2.0;
   double sinTheta = sqrt( 1.0 - cosTheta * cosTheta );
   double phi = asymDistr->GetRandom();
   //double phi = randomGen->Uniform (0, 2.*TMath::Pi());
   double randomXaim = cos(phi) * sinTheta;
   double randomYaim = sin(phi) * sinTheta;
   double randomZaim = cosTheta;
   //*direction = G4ThreeVector (randomXaim, randomYaim, randomZaim);
   *direction = G4ThreeVector (randomYaim, randomZaim, -randomXaim);    
}


void Beta::GetRandomBeta(float* randEnergy, G4ThreeVector* randDirection)
{
    int randomIndex = FindRandomIndex();
    *randEnergy=(energy.at(randomIndex));
    GenerateAsymDirection(randDirection, directions.at(randomIndex));
}

int Beta::FindRandomIndex()
{
	int randomEnergyIndex = (int)(G4UniformRand()*probability.size());
	double maxBetaProbability = *max_element(std::begin(probability), std::end(probability));
	double randomProbability = G4UniformRand() * maxBetaProbability;
	while (randomProbability > probability.at(randomEnergyIndex))
	{
		randomEnergyIndex = (int)(G4UniformRand()* probability.size());
		randomProbability = G4UniformRand() * maxBetaProbability;
	}
	return randomEnergyIndex;
}	
	
TF1* Beta::FindAsymDistrFunc(double asymFactor, double velocity, double polarization)
{
   TF1 *asymDistrFunc = new TF1("asymDistrFunc","1+[0]*cos(x)",0,2.*TMath::Pi());
   asymDistrFunc->SetParameter(0,asymFactor*velocity*polarization);
   return asymDistrFunc;
}



double Beta::FindVelocity(double energyinMeV)
{
//Return electron velocity in c unit (beta).
   double electronMass = 0.511;//in MeV unit
   double beta = pow((1-(electronMass*electronMass/(energyinMeV*energyinMeV))), 0.5);
   return beta;
}

