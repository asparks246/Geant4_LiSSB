//
// $Id: SteppingAction.hh 15.10.2018 A. Fijalkowska $
//
/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class
//
#ifndef SteppingAction_H
#define SteppingACtion_H 1

#include "G4UserSteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "OutputRoot_step.hh"
#include "EventAction.hh"
#include <time.h>
#include <iostream>


class SteppingAction : public G4UserSteppingAction
{
  public:
  
    SteppingAction();
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*); 

  private:

    void PrintStep(const G4Step*);
    OutputRoot_step* outputFile_step;
    G4int eventId=0;
    std::ofstream outputfile_errors;
    int ID=0;
    int nLaunchedParticles = 0;  // total primary particles launched
};

#endif
