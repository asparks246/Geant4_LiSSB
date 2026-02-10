#ifndef SampleHolder1_H
#define SampleHolder1_H 1
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

class SampleHolder1
{
    public:
    SampleHolder1();
    void Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo);
    
    private:
    
    G4Material* poliethylene;
    G4Material* mica;
    G4Material* MakeMica();
    void ConstructHolder();
    void ConstructMica();
    G4LogicalVolume* holderTopLog;
    G4LogicalVolume* holderBottomLog;
    G4LogicalVolume* micaLog;
};

#endif
