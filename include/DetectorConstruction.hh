// $Id: DetectorConstruction.hh 15.10.2018 A. Fijalkowska $
//
/// \file DetectorConstruction.hh
/// \brief Kasa trzymająca geometrię detektora
//
//
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Cache.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction(); 
    virtual ~DetectorConstruction(); 
    virtual G4VPhysicalVolume* Construct(); 
    virtual void ConstructSDandField(); 

  private:
  
    G4Material* MakePlactic2();
    void ConstructSingleSD2(G4String name2, G4LogicalVolume* logVol2);
    G4Material* plasticScint2;
    
    G4LogicalVolume* newBackDetectorLog;
    G4LogicalVolume* newFrontDetectorLog;
    
    G4LogicalVolume* scintFrontLog;
    G4LogicalVolume* scintRear2hLog;
    G4LogicalVolume* scintRear4hLog;
    G4LogicalVolume* scintRear6hLog;
    G4LogicalVolume* scintRear8hLog;
    G4LogicalVolume* scintRear10hLog;
    G4LogicalVolume* scintRear12hLog;
    
    //Sample
    G4double            fSampleSizeX;
    G4double            fSampleSizeY;
    G4double            fSampleSizeZ;
    G4Material*         fSampleMaterial;
    G4LogicalVolume*    fSample;
    G4Material*         fBackingPlateMaterial;
    G4LogicalVolume*    fBackPlate;
    G4LogicalVolume* 	fRFcoil1;
    G4Material*         fRFcoilMaterial;
    G4LogicalVolume* 	fRFcoil2;
 
    
    
    G4LogicalVolume* worldLogic;
    //G4VPhysicalVolume* worldPhys;
    G4VPhysicalVolume* ConstructWorld(); 
    void ConstructNewDetectors();
    void ConstructFront();
    void ConstructScintillator();
    void ConstructAluFrame();
    void ConstructHolder();
    void ConstructSample();
    void ConstructRF();
    G4Cache<G4MagneticField*> fField;
    void ConstructMagnetTube();
    
    //void ConstructMagneticFieldVolume();
    G4LogicalVolume* cylinderLogVol;
    G4LogicalVolume* cylinderLogVol2;
    G4LogicalVolume* unionLogVol;
    G4double           zOffset;
    

 
};

#endif
