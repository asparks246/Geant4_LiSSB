#include "SampleHolder1.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh" 
#include "CADMesh.hh"
#include <iostream>

SampleHolder1::SampleHolder1()
{
	G4NistManager* man=G4NistManager::Instance();
	poliethylene=man->FindOrBuildMaterial("G4_POLYETHYLENE");
    mica = MakeMica();
	ConstructHolder();
	ConstructMica();
}


G4Material* SampleHolder1::MakeMica()
{
    G4NistManager* man=G4NistManager::Instance();
    G4Element* K = man->FindOrBuildElement("K");
    G4Element* Mg = man->FindOrBuildElement("Mg");
    G4Element* Fe = man->FindOrBuildElement("Fe");
    G4Element* Al = man->FindOrBuildElement("Al");
    G4Element* Si = man->FindOrBuildElement("Si");
    G4Element* O = man->FindOrBuildElement("O");
    G4Element* F = man->FindOrBuildElement("F");
    G4Element* H = man->FindOrBuildElement("H");
    G4Material* micaMat = new G4Material("mica", 2.8*g/cm3, 8);
    micaMat->AddElement(K, 1);
    micaMat->AddElement(Mg, 3);
    micaMat->AddElement(Fe, 3);
    micaMat->AddElement(Al, 1);
    micaMat->AddElement(Si, 3);
    micaMat->AddElement(O, 12);
    micaMat->AddElement(F, 1);
    micaMat->AddElement(H, 2);
    return micaMat;

}


void SampleHolder1::ConstructHolder()
{
    auto holderTopMesh = CADMesh::TessellatedMesh::FromSTL("./stl/sample_holder/1sample_holder/1sample_holder_top.stl");
    G4VSolid* holderTopSolid =  holderTopMesh->GetSolid(); 
    holderTopLog = new G4LogicalVolume(holderTopSolid, poliethylene, "holderTopLog");
    
    auto holderBottomMesh = CADMesh::TessellatedMesh::FromSTL("./stl/sample_holder/1sample_holder/1sample_holder_bottom.stl");                                    
    G4VSolid* holderBottomSolid =  holderBottomMesh->GetSolid(); 
    holderBottomLog = new G4LogicalVolume(holderBottomSolid, poliethylene, "holderBottomLog");
    
    
    G4VisAttributes* holderVisAtt = new G4VisAttributes(G4Colour::Green());
	holderVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	holderVisAtt->SetForceSolid(true);
	holderTopLog->SetVisAttributes(holderVisAtt);
	holderBottomLog->SetVisAttributes(holderVisAtt);
}

void SampleHolder1::ConstructMica()
{
    auto micaMesh = CADMesh::TessellatedMesh::FromSTL("./stl/sample_holder/1sample_holder/mica.stl");                                                                      
    G4VSolid* micaSolid =  micaMesh->GetSolid(); 
    micaLog = new G4LogicalVolume(micaSolid, mica, "micaLog");
         
            
    G4VisAttributes* micaVisAtt = new G4VisAttributes(G4Colour(1,0,0,0.5));
	micaVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	micaVisAtt->SetForceSolid(true);
	micaLog->SetVisAttributes(micaVisAtt);
	
}




void SampleHolder1::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{
    new G4PVPlacement(pRot, tlate, holderTopLog, pName,  pMotherLogical, 0,pCopyNo);
    new G4PVPlacement(pRot, tlate, holderBottomLog, pName,  pMotherLogical, 0,pCopyNo);
    new G4PVPlacement(pRot, tlate, micaLog, pName,  pMotherLogical, 0,pCopyNo);
}
