
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "Beta.hh"
#include <vector>
#include <fstream>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
     PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;	
	void SetUp( void );
	void SetUpBetaAssymetry();
	void SetUpGammaAssymetry();
	int i=0;
	//direction
    G4ThreeVector GenerateIsotropicDirection( G4double thetaMin = 0,
                                              G4double thetaMax = M_PI,
                                              G4double phiMin = 0,
                                              G4double phiMax = 2.*M_PI);
                                              
                                              
    TF1* betaAsymDistr;
    TF1* gammaAsymDistr;
    Beta* betaPart;
    G4ThreeVector GeneratePosition(); 
    
     
};

#endif // PrimaryGeneratorAction_h
