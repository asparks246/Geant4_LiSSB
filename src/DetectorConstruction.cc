//
// $Id: DetectorConstruction.cc 12.16.2016 A. Fijalkowska $
//
/// \file DetectorConstruction.cc
/// \brief DetectorConstruction class
//
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh" 
#include "G4NistManager.hh" 
#include "G4Element.hh" 
#include "G4Box.hh" 
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh" 
#include "SampleHolder1.hh"
#include "SampleHolder2.hh"
#include "VITOMagneticField.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSPopulation.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PhysicalConstants.hh"
#include "CADMesh.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4FieldManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4ChordFinder.hh"
#include "G4UniformMagField.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4RKG3_Stepper.hh"
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4UserLimits.hh"


#define MAG 1          // Magnetic field grid

//-------------------------------------------------------------
DetectorConstruction::DetectorConstruction()
{
  
  //fField.Put(0);
  

 
}
//-------------------------------------------------------------
DetectorConstruction::~DetectorConstruction() {}


G4VPhysicalVolume* DetectorConstruction::Construct()  
{
    G4VPhysicalVolume* worldPhys = ConstructWorld();
    plasticScint2 = MakePlactic2();
    
    //ConstructFront();
    //ConstructAluFrame();
    //ConstructMagnetTube();
    //ConstructHolder();
    //ConstructRF();
    //ConstructSample();
    ConstructNewDetectors();
    ConstructSDandField();
    return worldPhys;
}
//-------------------------------------------------------------World
G4VPhysicalVolume* DetectorConstruction::ConstructWorld()
{   

    //G4double maxStep = 1.*mm;
    //G4UserLimits *stepLimit = new G4UserLimits();	
    //stepLimit->SetMaxAllowedStep(maxStep);
    
    fField.Put(0);
    G4double worldX = 10*cm;
    G4double worldY = 10*cm;
    G4double worldZ = 80*cm;
    
    G4double WorldExtent = worldX*worldY*worldZ; 
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(WorldExtent);
    
    G4Material* vacuum = new G4Material("GalacticVacuum", 1., 1.01*g/mole,
                           CLHEP::universe_mean_density, 
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
  
    G4Box* worldSolid = new G4Box("worldSolid",worldX,worldY,worldZ);
    worldLogic = 0L;
    worldLogic = new G4LogicalVolume(worldSolid, vacuum,"worldLogic", 0,0,0);
                                             
    //worldLogic->SetVisAttributes(G4ktVisAttributes::Invisible);
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "world", 0, false, 0);
    
    //worldLogic->SetUserLimits(stepLimit);
    
    return worldPhys;

}
//-------------------------------------------------------------New Detectors
void DetectorConstruction::ConstructNewDetectors()
{   
    //G4double maxStep = 100.*mm;
    //G4UserLimits *stepLimit = new G4UserLimits();	
    //stepLimit->SetMaxAllowedStep(maxStep);

    // Big Detector
    G4ThreeVector newBackDetectorPos(0,0,(-54.5-3.)*cm);
    G4RotationMatrix* newBackDetectorRot = new G4RotationMatrix();
    newBackDetectorRot ->rotateY(-90.*deg);
    newBackDetectorRot ->rotateZ(90.*deg);
   
    auto newBackDetectorMesh = CADMesh::TessellatedMesh::FromOBJ("./stl/BIG_DETECTOR.obj");
    newBackDetectorMesh->SetScale(10);
    G4VSolid* newBackDetectorSolid =  newBackDetectorMesh->GetSolid(); 
    newBackDetectorLog = new G4LogicalVolume(newBackDetectorSolid, plasticScint2, "newBackDetectorLog");
    G4VisAttributes* newBackDetectorVisAtt = new G4VisAttributes(G4Colour::Yellow());
    newBackDetectorVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    newBackDetectorVisAtt->SetForceSolid(true);
    newBackDetectorLog->SetVisAttributes(newBackDetectorVisAtt);
    new G4PVPlacement(newBackDetectorRot, newBackDetectorPos, newBackDetectorLog, "newBackDetectorPhys",  worldLogic, 0,0);
    
    //newBackDetectorLog->SetUserLimits(stepLimit);	
     
    // Small detector
    G4ThreeVector newFrontDetectorPos(0,0,31.5*cm);
    G4RotationMatrix* newFrontDetectorRot = new G4RotationMatrix();
    newFrontDetectorRot ->rotateY(-90.*deg);
    newFrontDetectorRot ->rotateZ(90.*deg);
    
    auto newFrontDetectorMesh = CADMesh::TessellatedMesh::FromOBJ("./stl/SMALL_DETECTOR.obj");
    newFrontDetectorMesh->SetScale(10);
    G4VSolid* newFrontDetectorSolid =  newFrontDetectorMesh->GetSolid(); 
    newFrontDetectorLog = new G4LogicalVolume(newFrontDetectorSolid, plasticScint2, "newFrontDetectorLog");
    G4VisAttributes* newFrontDetectorVisAtt = new G4VisAttributes(G4Colour::Yellow());
    newFrontDetectorVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    newFrontDetectorVisAtt->SetForceSolid(true);
    newFrontDetectorLog->SetVisAttributes(newFrontDetectorVisAtt);
    new G4PVPlacement(newFrontDetectorRot, newFrontDetectorPos, newFrontDetectorLog, "newFrontDetectorPhys",  worldLogic, 0,0);
    
    //newFrontDetectorLog->SetUserLimits(stepLimit);
      
}
void DetectorConstruction::ConstructMagnetTube()
{    
	double A;double Z;
        A  =  54.94*g/mole;
	G4Element* elMn   =  new G4Element("Manganese","Mn",Z = 25.,A);
	A = 28.09*g/mole;
 	G4Element* elSi  = new G4Element("Silicon","Si",Z = 14.,A);
 	A = 52.00*g/mole;
	G4Element* elCr  = new G4Element("Chromium","Cr",Z = 24.,A);
	A = 58.70*g/mole;
	G4Element* elNi  = new G4Element("Nickel","Ni",Z = 28.,A);
	A = 55.85*g/mole;
	G4Element* elFe  = new G4Element("Iron","Fe",Z = 26.,A);
	
     //Steel
     G4double d = 8.02*g/cm3 ;
     G4Material* matsteel = new G4Material("Stainless steel",d,5);
     matsteel->AddElement(elMn, 0.02);
     matsteel->AddElement(elSi, 0.01);
     matsteel->AddElement(elCr, 0.19);
     matsteel->AddElement(elNi, 0.10);
     matsteel->AddElement(elFe, 0.68);

     // Magnet tube
    G4ThreeVector tubePos(0,0,0*cm);
    G4RotationMatrix* tubeRot = new G4RotationMatrix();
    tubeRot ->rotateY(0.*deg);
    //tubeRot ->rotateZ(90.*deg);
    auto tubeMesh = CADMesh::TessellatedMesh::FromSTL("./stl/Tube152x2-rotCF160.stl");
    G4VSolid* tubeSolid =  tubeMesh->GetSolid(); 
    G4LogicalVolume* tubeLog = new G4LogicalVolume(tubeSolid, matsteel, "tubeSolid",0,0,0);
    G4VisAttributes* tubeVisAtt = new G4VisAttributes(G4Colour(0.75294, 0.75294, 0.75294, 0.4));
    tubeVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    tubeVisAtt->SetForceSolid(true);
    tubeLog->SetVisAttributes(tubeVisAtt);
    new G4PVPlacement(tubeRot, tubePos, tubeLog, "tubePhys",  worldLogic, 1,0);   //auto 
    
    
    // Detector box
    /*G4ThreeVector detBoxPos(0,0,-78.5*cm);
    G4RotationMatrix* detBoxRot = new G4RotationMatrix();
    detBoxRot ->rotateY(90.*deg);
    //tubeRot ->rotateZ(90.*deg);
    auto detBoxMesh = CADMesh::TessellatedMesh::FromSTL("./stl/DetBox.stl");
    G4VSolid* detBoxSolid =  detBoxMesh->GetSolid(); 
    G4LogicalVolume* detBoxLog = new G4LogicalVolume(detBoxSolid, matsteel, "detBoxSolid",0,0,0);
    G4VisAttributes* detBoxVisAtt = new G4VisAttributes(G4Colour(0.75294, 0.75294, 0.75294, 0.4));
    detBoxVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    detBoxVisAtt->SetForceSolid(true);
    detBoxLog->SetVisAttributes(detBoxVisAtt);
    new G4PVPlacement(detBoxRot, detBoxPos, detBoxLog, "detBoxPhys",  worldLogic, 1,0);   //auto   
    
    // Adapter
    G4ThreeVector boxAdapterPos(0,0,-50.68*cm);
    G4RotationMatrix* boxAdapterRot = new G4RotationMatrix();
    boxAdapterRot ->rotateY(0.*deg);
    //tubeRot ->rotateZ(90.*deg);
    auto boxAdapterMesh = CADMesh::TessellatedMesh::FromSTL("./stl/AdapterCF160-IsoF200-UnivMount.stl");
    G4VSolid* boxAdapterSolid =  boxAdapterMesh->GetSolid(); 
    G4LogicalVolume* boxAdapterLog = new G4LogicalVolume(boxAdapterSolid, matsteel, "boxAdapterSolid",0,0,0);
    G4VisAttributes* boxAdapterVisAtt = new G4VisAttributes(G4Colour(0.75294, 0.75294, 0.75294, 0.4));
    boxAdapterVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    boxAdapterVisAtt->SetForceSolid(true);
    boxAdapterLog->SetVisAttributes(boxAdapterVisAtt);
    new G4PVPlacement(boxAdapterRot, boxAdapterPos, boxAdapterLog, "boxAdapterPhys",  worldLogic, 1,0);   //auto
    */
  
}
//-------------------------------------------------------------Aluminium tube
void DetectorConstruction::ConstructAluFrame()
{
    //auto frameMesh = CADMesh::TessellatedMesh::FromSTL("./stl/alumin.stl");
    //G4VSolid* frameSolid = frameMesh->GetSolid();
    //double z, atomicMass, density;
    //int numberElements;
    //G4Element* Al = new G4Element("Aluminium","Al", z=13., atomicMass = 26.98*g/mole );
    //G4Material* aluminum = new G4Material( "Aluminium", density= 2.7*g/cm3, numberElements=1 );
    //aluminum->AddElement( Al, 1 );
    
    //G4LogicalVolume* frameLog = new G4LogicalVolume(frameSolid, aluminum, "frameSolid",0,0,0);
    //G4VisAttributes* frameVisAtt = new G4VisAttributes(G4Colour(0.75294, 0.75294, 0.75294, 0.4));
    //frameVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    //frameVisAtt->SetForceSolid(true);
    //frameLog->SetVisAttributes(frameVisAtt);
    //G4RotationMatrix* rot = new G4RotationMatrix();
    //rot->rotateZ(180.*deg);
    //new G4PVPlacement(0, G4ThreeVector(0,0,0), frameLog, "framePhys",  worldLogic, 1,0);
}
//-------------------------------------------------------------Scintillator Material
G4Material* DetectorConstruction::MakePlactic2()
{
    G4NistManager* man=G4NistManager::Instance();
    G4Element* H = man->FindOrBuildElement("H");
    G4Element* C = man->FindOrBuildElement("C");
    G4Material* EJ200 = new G4Material("EJ200", 1.023*g/cm3, 2);
    EJ200->AddElement(H, 517);
    EJ200->AddElement(C, 469);
    return EJ200;
}
//-------------------------------------------------------------
/*
void DetectorConstruction::ConstructMagneticFieldVolume()
{
    G4Material* vaccum2 = new G4Material("GalacticVacuum2", 1., 1.01*g/mole,
                           CLHEP::universe_mean_density, 
                           kStateGas, 3.e-18*pascal, 2.73*kelvin);
     G4double rMin = 0*cm;
     G4double rMax = 25*cm;
     G4double length = 50*cm;
     G4Tubs* theCylinder = new G4Tubs("theCylinder", rMin, rMax, length/2, 0*deg, 360*deg);
     cylinderLogVol = new G4LogicalVolume(theCylinder, vaccum2, "cylinderLogVol");

     G4VisAttributes* cylinderAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5, 0.7));
     cylinderAtt->SetForceAuxEdgeVisible(true);
     cylinderAtt->SetForceSolid(true);
     cylinderLogVol->SetVisAttributes(cylinderAtt);
     G4ThreeVector pos(0,0, 0);
     new G4PVPlacement(0,pos,cylinderLogVol,"cylinderPhys",worldLogic,0,0);


}*/
//-------------------------------------------------------------Old Detectors
void DetectorConstruction::ConstructFront()
{
    G4ThreeVector scint_pos(0,-400,0*cm);
    G4ThreeVector scint_pos2(0,0,(27.5+45.0)*cm);

    auto frontMesh = CADMesh::TessellatedMesh::FromSTL("./stl/scint-FrontDet.stl");                            
    G4VSolid* frontSolid =  frontMesh->GetSolid(); 
    scintFrontLog = new G4LogicalVolume(frontSolid, plasticScint2, "scintFrontLog");
    G4VisAttributes* scintFrontVisAtt = new G4VisAttributes(G4Colour::Yellow());
    scintFrontVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    scintFrontVisAtt->SetForceSolid(true);
    scintFrontLog->SetVisAttributes(scintFrontVisAtt);
    new G4PVPlacement(0, scint_pos, scintFrontLog, "scintFrontPhys",  worldLogic, 1,0);
                                                                                                
    auto rearMesh2h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-2h.stl"); 
    G4VSolid* rearSoli2h =  rearMesh2h->GetSolid(); 
    scintRear2hLog = new G4LogicalVolume(rearSoli2h, plasticScint2, "rearSoli2h");
    
    auto rearMesh4h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-4h.stl");                                    
    G4VSolid* rearSoli4h =  rearMesh4h->GetSolid(); 
    scintRear4hLog = new G4LogicalVolume(rearSoli4h, plasticScint2, "rearSoli4h");
    
    auto rearMesh6h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-6h.stl");                                    
    G4VSolid* rearSoli6h =  rearMesh6h->GetSolid(); 
    scintRear6hLog = new G4LogicalVolume(rearSoli6h, plasticScint2, "rearSoli6h");
    
    auto rearMesh8h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-8h.stl");                                    
    G4VSolid* rearSoli8h =  rearMesh8h->GetSolid(); 
    scintRear8hLog = new G4LogicalVolume(rearSoli8h, plasticScint2, "rearSoli8h");
    
    auto rearMesh10h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-10.stl");                                    
    G4VSolid* rearSoli10h =  rearMesh10h->GetSolid(); 
    scintRear10hLog = new G4LogicalVolume(rearSoli10h, plasticScint2, "rearSoli10h");
    
    auto rearMesh12h = CADMesh::TessellatedMesh::FromSTL("./stl/Rear-12.stl");                                    
    G4VSolid* rearSoli12h =  rearMesh12h->GetSolid(); 
    scintRear12hLog = new G4LogicalVolume(rearSoli12h, plasticScint2, "rearSoli12h"); 
	
    G4VisAttributes* scintRearVisAtt = new G4VisAttributes(G4Colour::Green());
    scintRearVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    scintRearVisAtt->SetForceSolid(true);
    
    scintRear2hLog->SetVisAttributes(scintRearVisAtt);
    scintRear4hLog->SetVisAttributes(scintRearVisAtt);
    scintRear6hLog->SetVisAttributes(scintRearVisAtt);
    scintRear8hLog->SetVisAttributes(scintRearVisAtt);
    scintRear10hLog->SetVisAttributes(scintRearVisAtt);
    scintRear12hLog->SetVisAttributes(scintRearVisAtt);
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateZ(180.*deg);
    
    new G4PVPlacement(rot, scint_pos, scintRear2hLog, "scintRear2hPhys",  worldLogic, 1,0);
    new G4PVPlacement(rot, scint_pos, scintRear4hLog, "scintRear4hPhys",  worldLogic, 1,0);
    new G4PVPlacement(rot, scint_pos, scintRear6hLog, "scintRear6hPhys",  worldLogic, 1,0);
    new G4PVPlacement(rot, scint_pos, scintRear8hLog, "scintRear8hPhys",  worldLogic, 1,0);
    new G4PVPlacement(rot, scint_pos, scintRear10hLog, "scintRear10hPhys",  worldLogic, 1,0);
    new G4PVPlacement(rot, scint_pos, scintRear12hLog, "scintRear12hPhys",  worldLogic, 1,0);
    zOffset = 4.4*mm;
}
//-------------------------------------------------------------Sample Holder 


void DetectorConstruction::ConstructRF()
{
    	//RF coil material
	G4NistManager* manager = G4NistManager::Instance();
	G4Material* glass = manager->FindOrBuildMaterial("G4_GLASS_PLATE");
	G4ThreeVector RF1Pos(1.5+20,0,0*cm);
	G4ThreeVector RF2Pos(1.5-20,0,0*cm);
    	G4RotationMatrix* RFRot = new G4RotationMatrix();
    	RFRot ->rotateX(90.*deg);
    	RFRot ->rotateZ(90.*deg);
    	auto RF1Mesh = CADMesh::TessellatedMesh::FromOBJ("./stl/RF_coil_v2.obj");
    	auto RF2Mesh = CADMesh::TessellatedMesh::FromOBJ("./stl/RF_coil_v2.obj");
    	RF1Mesh->SetScale(10);
    	RF2Mesh->SetScale(10);
    	G4VSolid* RF1Solid =  RF1Mesh->GetSolid(); 
    	G4VSolid* RF2Solid =  RF2Mesh->GetSolid();
    	G4LogicalVolume* RF1Log = new G4LogicalVolume(RF1Solid, glass, "RF1Solid",0,0,0);
    	G4LogicalVolume* RF2Log = new G4LogicalVolume(RF2Solid, glass, "RF2Solid",0,0,0);
    	G4VisAttributes* RF1VisAtt = new G4VisAttributes(G4Colour::Grey());
    	G4VisAttributes* RF2VisAtt = new G4VisAttributes(G4Colour::Grey());
    	RF1VisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    	RF2VisAtt->SetForceAuxEdgeVisible(true);
    	RF1VisAtt->SetForceSolid(true);
    	RF2VisAtt->SetForceSolid(true);
    	RF1Log->SetVisAttributes(RF1VisAtt);
    	RF2Log->SetVisAttributes(RF2VisAtt);
    	new G4PVPlacement(RFRot, RF1Pos, RF1Log, "RF1Phys",  worldLogic, 1,0);   //auto
    	new G4PVPlacement(RFRot, RF2Pos, RF2Log, "RF2Phys",  worldLogic, 1,0);   //auto
}	


void DetectorConstruction::ConstructHolder()
{
   
    //Sample holder materials
    double z, a, d;
    int ncomponents, natoms;
    G4Element* Al = new G4Element("Aluminium","Al", z=13., a = 26.98*g/mole );
    G4Element* N = new G4Element("Nitrogen","N", z=7., a = 14.01*g/mole );
    G4Element* Cu = new G4Element("Copper","Cu", z=29, a = 63.55*g/mole );
    G4Material* AlN = new G4Material( "Aluminium Nitride", d= 3.32*g/cm3, ncomponents=2);
    G4Material* Alu = new G4Material( "Aluminium", d= 2.27*g/cm3, ncomponents=1);
    G4Material* Cu_ = new G4Material( "Copper", d= 8.96*g/cm3, ncomponents=1);
    Cu_->AddElement(Cu, natoms=1);
    AlN->AddElement(Al,natoms=1);
    AlN->AddElement(N,natoms=1);
    Alu->AddElement(N,natoms=1);
    
    /*
    //Sample catcher	
    //G4ThreeVector holderPos(0,-69.25,-27*mm);
    G4ThreeVector holderPos(0,0,-1.5*mm);
    G4RotationMatrix* holderRot = new G4RotationMatrix();
    holderRot ->rotateX(0.*deg);
    holderRot ->rotateX(90.*deg);
    auto holder1Mesh = CADMesh::TessellatedMesh::FromOBJ("./stl/Jiri sample holder design/sample_catcher.obj");
    holder1Mesh->SetScale(10);
    G4VSolid* holder1Solid =  holder1Mesh->GetSolid(); 
    G4LogicalVolume* holder1Log = new G4LogicalVolume(holder1Solid, Alu, "holder1Solid",0,0,0);
    G4VisAttributes* holder1VisAtt = new G4VisAttributes(G4Colour::Magenta());
    holder1VisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    holder1VisAtt->SetForceSolid(true);
    holder1Log->SetVisAttributes(holder1VisAtt);
    new G4PVPlacement(holderRot, holderPos, holder1Log, "holder1Phys",  worldLogic, 1,0);   //auto*/
    
    //Heating ring
    //G4ThreeVector ringPos(0,-5.5,7*mm);
    /*G4ThreeVector ringPos(0,0,0*mm);
    G4RotationMatrix* ringRot = new G4RotationMatrix();
    ringRot ->rotateX(0.*deg);
    ringRot ->rotateX(90.*deg);
    auto ringMesh = CADMesh::TessellatedMesh::FromOBJ("./stl/Jiri sample holder design 2/sample_catcher_and_heating_ring.obj");
    ringMesh->SetScale(10);
    G4VSolid* ringSolid =  ringMesh->GetSolid(); 
    G4LogicalVolume* ringLog = new G4LogicalVolume(ringSolid, Alu, "ringSolid",0,0,0);
    G4VisAttributes* ringVisAtt = new G4VisAttributes(G4Colour::Grey());
    ringVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    ringVisAtt->SetForceSolid(true);
    ringLog->SetVisAttributes(ringVisAtt);
    new G4PVPlacement(ringRot, ringPos, ringLog, "ringPhys",  worldLogic, 1,0);   //auto */
    
    //Sample catcher with heater
    G4ThreeVector holderPos(0,0,0*mm);
    G4RotationMatrix* holderRot = new G4RotationMatrix();
    holderRot ->rotateX(0.*deg);
    holderRot ->rotateX(0.*deg);
    auto holder1Mesh = CADMesh::TessellatedMesh::FromOBJ("./stl/heater_beta_simulation2.obj");
    holder1Mesh->SetScale(10);
    G4VSolid* holder1Solid =  holder1Mesh->GetSolid(); 
    G4LogicalVolume* holder1Log = new G4LogicalVolume(holder1Solid, AlN, "holder1Solid",0,0,0);
    G4VisAttributes* holder1VisAtt = new G4VisAttributes(G4Colour::Magenta());
    holder1VisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    holder1VisAtt->SetForceSolid(true);
    holder1Log->SetVisAttributes(holder1VisAtt);
    new G4PVPlacement(holderRot, holderPos, holder1Log, "holder1Phys",  worldLogic, 1,0);   //auto*/


}
//-------------------------------------------------------------Sample Material
void DetectorConstruction::ConstructSample()
{

    //G4double maxStep = 10.*mm;
    //G4UserLimits *stepLimit = new G4UserLimits();	
    //stepLimit->SetMaxAllowedStep(maxStep);	
	
	
    G4double z, a, density;
    G4int    ncomponents, natoms;
    G4Element* Li  = new G4Element("Lithium","Li",  z=3.,  a=6.941*g/mole);
    G4Element* P  = new G4Element("Phosphorus",  "P",  z=15.,  a= 30.974 *g/mole);
    G4Element* S  = new G4Element("Sulfur","S",  z=16.,  a= 32.066*g/mole); 
    G4Element* Cl  = new G4Element("Chlorine","Cl",  z=17.,  a= 35.453*g/mole);
    G4Element* Al = new G4Element("Aluminium","Al",  z=13.,  a= 26.982*g/mole);
    G4Material* Alu = new G4Material("Alu",density= 2.7*g/cm3, ncomponents=1, kStateSolid, 298*kelvin); 
    Alu->AddElement(Al, natoms=1);
    G4Material* Li7PS6 = new G4Material("Li7PS6",density= 1.64*g/cm3, ncomponents=4, kStateSolid, 298*kelvin);    
    Li7PS6->AddElement(Li, natoms=6);
    Li7PS6->AddElement(P, natoms=1);
    Li7PS6->AddElement(S, natoms=5);
    Li7PS6->AddElement(Cl, natoms=1);

    fSampleSizeX =  13*mm;
    fSampleSizeY = 13*mm;
    fSampleSizeZ = 1*mm;
  
    fSampleMaterial = Li7PS6;
    fBackingPlateMaterial = Alu;
    
    //******** Sample ********//            
    G4Tubs* sSample = new G4Tubs("Sample", 0., fSampleSizeY/2, fSampleSizeZ , 0. * deg, 360. * deg);
    fSample = new G4LogicalVolume(sSample,fSampleMaterial, "Sample");                             
    new G4PVPlacement(0,G4ThreeVector(0,0,0), fSample,"Sample", worldLogic, false,0);                                                
    G4VisAttributes* sampleVisAtt= new G4VisAttributes(G4Colour::White());   
    sampleVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    sampleVisAtt->SetForceSolid(true); 
    fSample->SetVisAttributes(sampleVisAtt);
    
    //fSample->SetUserLimits(stepLimit); 
    
    //******** Backing Plate ********// 
    /*G4Box* sBackPlate = new G4Box("Backing plate", fSampleSizeX/2, fSampleSizeY*2, 0.1*mm);
    fBackPlate = new G4LogicalVolume(sBackPlate,fBackingPlateMaterial, "Backing plate");  
    new G4PVPlacement(0,G4ThreeVector(0.,0.,fSampleSizeZ+0.2), fBackPlate,"Backing plate", worldLogic, false,0);                                                
    G4VisAttributes* backPlateVisAtt = new G4VisAttributes(G4Colour::Grey()); 
    backPlateVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    backPlateVisAtt->SetForceSolid(true); 
    fBackPlate->SetVisAttributes(backPlateVisAtt);*/
    
    //fBackPlate->SetUserLimits(stepLimit); 
}
//-------------------------------------------------------------Energy Deposition Volumes

void DetectorConstruction::ConstructSingleSD2(G4String name2, G4LogicalVolume* logicVol2)
{
    G4MultiFunctionalDetector* detector2 = new G4MultiFunctionalDetector(name2);
    G4int depth2 = 0;
    G4VPrimitiveScorer* energyDepScorer2 = new G4PSEnergyDeposit("eDep",depth2);   
    detector2->RegisterPrimitive(energyDepScorer2);
    logicVol2->SetSensitiveDetector(detector2);
    G4SDManager* SDmanager2 = G4SDManager::GetSDMpointer();
    SDmanager2->AddNewDetector(detector2);

}
//-------------------------------------------------------------Apply Energy Deposition Volumes and VITO Magnetic Field
void DetectorConstruction::ConstructSDandField() 
{
    //ConstructSingleSD2("scintFrontSD", scintFrontLog);
    //ConstructSingleSD2("scintRear2hSD", scintRear2hLog);
    //ConstructSingleSD2("scintRear4hSD", scintRear4hLog);
    //ConstructSingleSD2("scintRear6hSD", scintRear6hLog);
    //ConstructSingleSD2("scintRear8hSD", scintRear8hLog);
    //ConstructSingleSD2("scintRear10hSD", scintRear10hLog);
    //ConstructSingleSD2("scintRear12hSD", scintRear12hLog);
    
    ConstructSingleSD2("newBackDetectorSD", newBackDetectorLog);
    ConstructSingleSD2("newFrontDetectorSD", newFrontDetectorLog);

	//UNIFORM MGNETIC FIELD
	//G4MagneticField* magField = new G4UniformMagField(G4ThreeVector(0.*tesla,0*tesla,-1.*tesla));
	//G4FieldManager* localFieldMgr = new G4FieldManager(magField);
	//unionLogVol->SetFieldManager(localFieldMgr, true);


//REAL VITO MAGNETIC FIELD
/*#if MAG
  if (fField.Get() == 0)
    {
      G4MagneticField* magField= new VITOMagneticField("./field1Axial.txt", "./field1Radial.txt", "./field2Axial.txt", "./field2Radial.txt");
      fField.Put(magField);
      
      G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
      pFieldMgr->SetDetectorField(fField.Get());
      pFieldMgr->CreateChordFinder(fField.Get());
    }

#endif*/


#if MAG
  if (fField.Get() == 0)
    {
      G4MagneticField* magField= new G4UniformMagField(G4ThreeVector(0.*tesla,0*tesla,-4.7*tesla));
      fField.Put(magField);
      
      G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
      pFieldMgr->SetDetectorField(fField.Get());
      pFieldMgr->CreateChordFinder(fField.Get());
    }

#endif

}
//-------------------------------------------------------------





