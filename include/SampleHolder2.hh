#ifndef SampleHolder2_H
#define SampleHolder2_H 1
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

class SampleHolder2
{
    public:
    SampleHolder2();
    void Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo);
    
    private:
    
    G4Material* poliethylene;
    void ConstructHolder();
    G4LogicalVolume* holderTopLog;
    G4LogicalVolume* holderBottomLog;
};

#endif
