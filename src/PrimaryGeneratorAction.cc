#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
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

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    SetUp();
    SetUpBetaAssymetry();
    i = 0;
}

void PrimaryGeneratorAction::SetUp() {
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* gammaPD = particleTable->FindParticle("gamma");
    particleGun->SetParticleDefinition(gammaPD);
    particleGun->SetParticleEnergy(500.0 * keV);
    particleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, 0.*cm));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

void PrimaryGeneratorAction::SetUpBetaAssymetry() {
    double maxEnergy = 13100.0; // keV
    int atomicNumber = 3;       // Lithium-3
    double polarization = -1.0; 
    double assymetryFactor = 0.333; // / 3.0;

    betaPart = new Beta(maxEnergy, atomicNumber, polarization, assymetryFactor);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* betaPD = particleTable->FindParticle("e-");

    // Position;
    G4ThreeVector startPos = GeneratePosition();

    // Energy and direction
    G4ThreeVector direction;
    float randEnergy_keV;
    betaPart->GetRandomBeta(&randEnergy_keV, &direction);

    double randEnergy_MeV = randEnergy_keV * keV; // Convert keV -> MeV

    particleGun->SetParticleDefinition(betaPD);
    particleGun->SetParticleEnergy(randEnergy_MeV * MeV);
    particleGun->SetParticlePosition(startPos);
    particleGun->SetParticleMomentumDirection(direction);
    particleGun->GeneratePrimaryVertex(anEvent);

    i++;
    if (i % 10 == 0) {
        G4cout << "Finished Running Event # " << i << G4endl;
    }

    G4cout << "Injected beta KE = " << randEnergy_MeV << " MeV" << G4endl;
}

G4ThreeVector PrimaryGeneratorAction::GeneratePosition() {
    double radius = 5.5*mm;
    double phiRan = G4UniformRand() * 2.0 * M_PI;
    double u = G4UniformRand() + G4UniformRand();
    double rRan = radius * ((u > 1.0) ? 2.0 - u : u);

    double x0 = rRan * cos(phiRan);
    double y0 = rRan * sin(phiRan);
    double z0 = 0.0*cm;

    return G4ThreeVector(x0, y0, z0);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleGun;
    delete betaPart;
}
