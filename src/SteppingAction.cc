//
// $Id: SteppingAction.cc 15.10.2018 A. Fijalkowska $
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//

#include "SteppingAction.hh"
#include "G4UserEventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4String.hh"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <string>  
#include <cmath>

SteppingAction::SteppingAction():G4UserSteppingAction()
{
    //outputFile_step=OutputRoot_step::GetInstance();
    //outputFile_step->CreateFile_step("./sim_results/ScintDetOutput_step.root");
   // outputfile.open("./sim_results/data_step.txt");
    //outputfile << "eventId" << "\t" << "stepId" << "\t" << "volId" << "\t" << "particleID" << "\t" << "processID" << "\t" << "deltaEn" << "\t" << "x_mom" << "\t" << "y_mom" << "\t" << "z_mom" << "\t" << "x_prePos" << "\t" << "y_prePos" << "\t" << "z_prePos" << "\t" << "x_postPos" << "\t" << "y_postPos" << "\t" << "z_postPos" << "\t" << "deltaTime" << "\t" << "glTime_pre" << "\t" << "glTime_post" << "\t" << "prTime_pre" << "\t" << "prTime_post" << "\t" << "trackID" << "\t" << "parentID" << "\t" << "kinEnergy" << std::endl;
    //outputfile_errors.open("./sim_results/errors_info.txt");

}

SteppingAction::~SteppingAction() 
{
	//outputFile_step->SaveOutput_step();
	//outputfile.close();
	//outputfile_errors.close();
}


void SteppingAction::UserSteppingAction(const G4Step* theStep) 
{
	PrintStep(theStep);
}


void SteppingAction::PrintStep(const G4Step* theStep)
{
    
    //STEP ID
    G4Track* theTrack = theStep->GetTrack();
    G4int stepId = theTrack->GetCurrentStepNumber();
    
    G4double EdepStep = theStep->GetTotalEnergyDeposit();
    G4double LengthStep = theStep->GetStepLength();

    //PARTICLE ID: e- -> 10, gamma ->20,...
    G4String particleName = theTrack->GetDefinition()->GetParticleName();
    G4int particleID;
    if(particleName == "e-") particleID=10;
    else if(particleName == "gamma") particleID=20;
    else if(particleName == "neutron") particleID=30;
    else if(particleName == "alpha") particleID=40;
    else if(particleName == "e+") particleID=50;
    else 
    {
	particleID=0;
	std::cout << particleName << std::endl;
	//outputfile_errors << "particle to add:"<< "\t" << particleName;
    }
    //MOMENTUM == DIRECTION
    G4ThreeVector momentum = theTrack->GetMomentum();
    G4double x_mom, y_mom, z_mom;
    x_mom = momentum.x();
    y_mom = momentum.y();
    z_mom = momentum.z();
    //DELTA EN in MeV
    G4double deltaEn = theStep->GetDeltaEnergy();
    //PRE POSITION - x,y,z
    G4ThreeVector prePos= theStep->GetPreStepPoint()->GetPosition();
    G4double x_prePos, y_prePos, z_prePos;
    x_prePos = prePos.x();
    y_prePos = prePos.y();
    z_prePos = prePos.z();
    //POST POSITION - x,y,z
    G4ThreeVector postPos= theStep->GetPostStepPoint()->GetPosition();
    G4double x_postPos, y_postPos, z_postPos;
    x_postPos = postPos.x();
    y_postPos = postPos.y();
    z_postPos = postPos.z();
    
    //
    G4double x_delta = x_postPos - x_prePos;
    G4double y_delta = y_postPos - y_prePos;
    G4double z_delta = z_postPos - z_prePos;
    
    
    //DELTA TIME
    G4double deltaTime = theStep->GetDeltaTime();
    //GLOBAL TIME
    G4double globalTime_pre = theStep->GetPreStepPoint()->GetGlobalTime();
    G4double globalTime_post = theStep->GetPostStepPoint()->GetGlobalTime();
    //PROPER TIME
    G4double properTime_pre = theStep->GetPreStepPoint()->GetProperTime();
    G4double properTime_post = theStep->GetPostStepPoint()->GetProperTime();
    //std::cout << properTime_pre << "\t" << properTime_post << std::endl; 
	

	
    //TRACK ID
    G4int trackID = theTrack->GetTrackID();
    //PARENT ID 
    G4int parentID = theTrack->GetParentID();
    //KINETIC ENERGY
    G4double kineticEnergy = theTrack->GetKineticEnergy();
    //EVENT ID
    

    //VOLUME ID: samlpe holder - 0, frontDet - 1 gramophoneDet - 2,4,6,8,10,12, aluFrame - 99, world - 100
    G4String volumeName = theTrack->GetVolume()->GetName();
    G4int volId;
    if (volumeName == "newFrontDetectorLog")    {volId=1;}
    else if(volumeName == "newBackDetectorLog") {volId=2;}
    else if(volumeName == "Sample") 		{volId=3;}
    else if(volumeName == "Backing plate") 	{volId=4;}
    else if(volumeName == "world") 		{volId=5;}
    else    					{volId=0;}


    //PROCESS NAME
    G4String processName =theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    G4int processID;
    if(processName == "eIoni") processID=10;
    else if(processName == "Transportation") processID=20;
    else if(processName == "eBrem") processID=30;
    else if(processName == "phot") processID=40;
    else if(processName == "compt") processID=50;
    else if(processName == "msc") processID=60;
    else if(processName == "Rayl") processID=70;
    else if(processName == "conv") processID=80;
    else if(processName == "annihil") processID=90;
    else 
    {
	processID=0;
	//outputfile_errors << "process name to add:" << "\t" << processName << std::endl;
    }
  	
  //Check if particle is at boundary
  	G4int AtBoundary;	
    if (theStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) AtBoundary = 1;
    if (theStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) AtBoundary = 2;
    else AtBoundary = 0;
    
    ID++;

        // Increment the number of launched particles for primary tracks
    if (stepId == 1 && trackID == 1 && parentID == 0)
    {
        eventId++;
        nLaunchedParticles++;
        G4cout << "Primary particle launched. Total so far: " << nLaunchedParticles << G4endl;
    }

    
    std::cout.precision(2);
    
    std::cout << "Beta No.:" << nLaunchedParticles << "|| Step: " << stepId << " || Track: " << trackID << " || PosX: " << prePos.x() << " || Y: " << prePos.y() << " || Z: " << prePos.z() << " || Proc: " << processName<< " || Vol: " << volumeName << "|| PosDelX: " << abs(prePos.x() - postPos.x())<< "|| Y: " << abs(prePos.y() - postPos.y()) <<" || Z: " << abs(prePos.z() - postPos.z()) << " || EDep: " <<EdepStep<<" || KE: "<<kineticEnergy<<" || LenStep: "<<LengthStep<<std::endl;
    
    //if (abs(prePos.x() - postPos.x())<0.1 && abs(prePos.y() - postPos.y())<0.1 && abs(prePos.z() - postPos.z())<0.1 && volId ==5 && stepId>5) theTrack->SetTrackStatus(fStopAndKill);
    //if (processName == "eIoni") theTrack->SetTrackStatus(fStopAndKill);
    //if (KEstep == 0 && stepId>1000) theTrack->SetTrackStatus(fStopAndKill);
    //if ((abs(prePos.z() - postPos.z()) < 0.5) && (abs(prePos.x() - postPos.x()) < 0.5)&& (abs(prePos.y() - postPos.y()) < 0.5)&& volId ==5) theTrack->SetTrackStatus(fStopAndKill);
    //if ((abs(prePos.z() - postPos.z()) < 0.5) && volId == 5) theTrack->SetTrackStatus(fKillTrackAndSecondaries);
   
     
    // ==== NEW: Kill particles with tiny Z displacement in the world ====
    const double stuckThreshold = 1e-3*CLHEP::mm;  // adjust if needed

    if (std::abs(z_delta) < stuckThreshold && volId == 5) // 5 = world
    {
        theTrack->SetTrackStatus(fKillTrackAndSecondaries);
        G4cout << "Killed stuck particle at step " 
            << stepId 
            << ", trackID " << trackID 
            << ", ΔZ = " << z_delta/CLHEP::mm << " mm" << G4endl;
    }

    
    //outputFile_step->AddHit_step(ID, eventId, stepId, volId, particleID, processID, deltaEn, x_mom, y_mom, z_mom, x_prePos, y_prePos, z_prePos, x_postPos, y_postPos, z_postPos, deltaTime, globalTime_pre, globalTime_post, properTime_pre, properTime_post, trackID, parentID, kineticEnergy);
}
