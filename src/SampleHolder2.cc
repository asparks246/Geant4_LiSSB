#include "SampleHolder2.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh" 
#include "CADMesh.hh"
#include <iostream>

SampleHolder2::SampleHolder2()
{
	G4NistManager* man=G4NistManager::Instance();
	poliethylene=man->FindOrBuildMaterial("G4_POLYETHYLENE");
	ConstructHolder();
}


void SampleHolder2::ConstructHolder()
{
    auto holderTopMesh = CADMesh::TessellatedMesh::FromSTL("./stl/sample_holder/2sample_holder/2sample_holder_top.stl");
                                    
    G4VSolid* holderTopSolid =  holderTopMesh->GetSolid(); 
    holderTopLog = new G4LogicalVolume(holderTopSolid, poliethylene, "holderTopLog");
    
    auto holderBottomMesh = CADMesh::TessellatedMesh::FromSTL("./stl/sample_holder/2sample_holder/2sample_holder_bottom.stl");                                    
    G4VSolid* holderBottomSolid =  holderBottomMesh->GetSolid(); 
    holderBottomLog = new G4LogicalVolume(holderBottomSolid, poliethylene, "holderBottomLog");
    
    
    G4VisAttributes* holderVisAtt = new G4VisAttributes(G4Colour::Green());
	holderVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	holderVisAtt->SetForceSolid(true);
	holderTopLog->SetVisAttributes(holderVisAtt);
	holderBottomLog->SetVisAttributes(holderVisAtt);
}

void SampleHolder2::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{
    new G4PVPlacement(pRot, tlate, holderTopLog, pName,  pMotherLogical, 0,pCopyNo);
    new G4PVPlacement(pRot, tlate, holderBottomLog, pName,  pMotherLogical, 0,pCopyNo);
}
