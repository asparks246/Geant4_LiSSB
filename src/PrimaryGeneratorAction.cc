#include <fstream>
#include <functional>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Exception.hh"
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Anisotropy.hh"

#include "G4IonTable.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4DecayProducts.hh"
#include "G4RadioactiveDecay.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	SetUp();
	SetUpBetaAssymetry();
	i=0;
}

void PrimaryGeneratorAction::SetUp( void )
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun( n_particle );

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* gammaPD = particleTable->FindParticle("gamma");
  particleGun->SetParticleDefinition(gammaPD);
  particleGun->SetParticleEnergy( 500.0 * keV );
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  
  

}

void PrimaryGeneratorAction::SetUpBetaAssymetry()
{
    //double maxEnergy= 13.1*MeV;  //26Na: 9354(4)keV
    double maxEnergy= (13.1*1000)*keV;  //26Na: 9354(4)keV
    int atomicNumber=3; //Z number (19-K, 11-Na)
    double polarization = 1; //polarisation
    double assymetryFactor= -1/3; //assymetry factor
    
    betaPart = new Beta(maxEnergy, atomicNumber, polarization, assymetryFactor);
  
}

void PrimaryGeneratorAction::SetUpGammaAssymetry()
{     
   double I0=3;
   std::vector<double> mVal= {-3, -2, -1, 0, 1, 2, 3};
   std::vector<double> am = {0.002, 0.002, 0.004, 0.08, 0.1667, 0.3333, 0.5};  
   double Ii=2;
   double If=0;
   double L1=2;
   double L2=2; 
   double mixRatio = 0;

   Anisotropy* gammaAnisotropy = new Anisotropy(I0, mVal, am, Ii, If, L1, L2, mixRatio);
   gammaAsymDistr = gammaAnisotropy->  GetThetaDistr(); 
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* betaPD = particleTable->FindParticle("e-");
	
	
	//Position
	G4ThreeVector startPos( 0.0*cm, 0.0*cm, 0.0*cm );
	
	//Direction and energy
	G4ThreeVector direction( 1.0, 0.0, 0.0 );
	float randEnergy = 0;
	betaPart->GetRandomBeta(&randEnergy, &direction);

	particleGun->SetParticleDefinition(betaPD);
	particleGun->SetParticleEnergy( randEnergy/1000. );	
	//particleGun->SetParticlePosition(GeneratePosition());
	//particleGun->SetParticleEnergy(randEnergy * MeV);   // or MeV
	particleGun->SetParticlePosition(GeneratePosition());
	particleGun->SetParticleMomentumDirection( direction );		
	particleGun->GeneratePrimaryVertex(anEvent);
	i++;
	if( i % 10 == 0 )
  	{
    		std::cout << "Finished Running Event # " << i << std::endl;
  	}

  G4cout
  << "Injected beta KE = "
  << randEnergy
  << " MeV"
  << G4endl;
 
}



    
G4ThreeVector PrimaryGeneratorAction::GenerateIsotropicDirection( G4double thetaMin,
                                                                 G4double thetaMax,
                                                                 G4double phiMin,
                                                                 G4double phiMax)
{
   if(thetaMin < 0 || thetaMin > 2.*M_PI || thetaMax < 0 || thetaMax > 2.*M_PI )
   {
       std::cout << "angles not in the limit " << std::endl;
       return G4ThreeVector(0,0,0);
   }
   if(thetaMin >= thetaMax)
   {
       std::cout << " theta min has to be smaller than theta max" << std::endl;
       return G4ThreeVector(0,0,0);
   }
   
   G4double randomPhi = G4UniformRand()*(phiMax - phiMin) + phiMin; 
   G4double cosThetaMin = cos(thetaMin);
   G4double cosThetaMax = cos(thetaMax); 
   G4double randomCosTheta = G4UniformRand()*(cosThetaMin - cosThetaMax) + cosThetaMax;
   G4double randomTheta = acos(randomCosTheta);

   G4double x =  sin(randomTheta)*cos(randomPhi);
   G4double y = sin(randomTheta)*sin(randomPhi);
   G4double z = randomCosTheta;                                                      
   G4ThreeVector randDir = G4ThreeVector(x, y, z);
   return randDir;
}  

G4ThreeVector PrimaryGeneratorAction::GeneratePosition()
{
   //G4double randomPhi = G4UniformRand()*2*M_PI;
   //G4double r = G4UniformRand()*4;
   //G4double x = r*cos(randomPhi);
   //G4double y = r*sin(randomPhi)/sqrt(2.);
   //G4double z = y;
   //G4ThreeVector pos = G4ThreeVector(x, y, z);
  
  //G4double envSizeX = 13.*mm;
  //G4double envSizeY = 13.*mm;
  //G4double envSizeZ = 1.*mm;
  //G4double size = 1.;
  //G4double x0 = size * envSizeX * (G4UniformRand()-0.5);
  //G4double y0 = size * envSizeY * (G4UniformRand()-0.5);
  //G4double z0 = size * envSizeZ * (G4UniformRand()-0.5);
  
  G4double radius = 5.5*mm;
  G4double phi =  360.*deg;
  G4double u = G4UniformRand() + G4UniformRand();
  G4double rRan = radius*(( u>1) ? 2-u : u);
  G4double phiRan = phi*G4UniformRand();
  
  G4double x0 = rRan*sin(phiRan);
  G4double y0 = rRan*cos(phiRan);
  G4double z0 = 0.*cm;
  
  G4ThreeVector pos = G4ThreeVector(x0, y0, z0);
   return pos;
}    




























