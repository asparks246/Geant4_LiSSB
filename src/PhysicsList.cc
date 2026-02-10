// $Id: PhysicsList.cc 68752 2016-11-28 A. Fijalkowska $
//
/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class based on optical/LXe/LXePhysicsList 
//
//
#include "PhysicsList.hh"
#include "GeneralPhysics.hh"
#include "EMPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"


PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  //defaultCutValue = 0.000001 * mm;

  // General Physics
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics
  //RegisterPhysics( new EMPhysics("standard EM"));
  
  RegisterPhysics(new G4EmStandardPhysics_option4());
  
  // Step Limiter Physics
  G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
  stepLimitPhys->SetApplyToAll(true);
  RegisterPhysics(stepLimitPhys);
}


PhysicsList::~PhysicsList() {}


void PhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}




