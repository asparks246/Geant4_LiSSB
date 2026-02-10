// $Id: EventAction.hh 15.10.2018 A. Fijalkowska $
//
/// \file EventAction.hh
/// \brief Definition of the EventAction class
//

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4Event.hh"
#include "G4String.hh"
#include "OutputRoot.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
  private:    
    OutputRoot* outputFile;
    double GetEnergy(G4HCofThisEvent* HCE, G4String detectorName);

};

#endif
