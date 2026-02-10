#include "OutputRoot_step.hh"

OutputRoot_step::OutputRoot_step()
{
   //rootManager=G4RootAnalysisManager::Instance();   
   //rootManager->SetFirstNtupleId(0);
   //rootManager->SetFirstHistoId(0);
   //rootManager->SetFirstNtupleColumnId(0); 
}

void OutputRoot_step::CreateFile_step(G4String filename)
{
    //rootManager->OpenFile(filename);
    //siliTupleId_step=0;
    //CreateSiliTuple_step();
}

void OutputRoot_step::SaveOutput_step()
{
   //rootManager->Write();
   //rootManager->CloseFile();
}

void OutputRoot_step::CreateSiliTuple_step()
{
   rootManager->CreateNtuple("StepInfo","step informations");
   rootManager->CreateNtupleIColumn("ID");
   rootManager->CreateNtupleIColumn("eventId");
   rootManager->CreateNtupleIColumn("stepId");
   rootManager->CreateNtupleIColumn("volId");
   rootManager->CreateNtupleIColumn("particleID");
   rootManager->CreateNtupleIColumn("processID");
   rootManager->CreateNtupleDColumn("deltaEn");
   rootManager->CreateNtupleDColumn("x_mom");
   rootManager->CreateNtupleDColumn("y_mom");
   rootManager->CreateNtupleDColumn("z_mom");
   rootManager->CreateNtupleDColumn("x_prePos");
   rootManager->CreateNtupleDColumn("y_prePos");
   rootManager->CreateNtupleDColumn("z_prePos");
   rootManager->CreateNtupleDColumn("x_postPos");
   rootManager->CreateNtupleDColumn("y_postPos");
   rootManager->CreateNtupleDColumn("z_postPos");
   rootManager->CreateNtupleDColumn("deltaTime");
   rootManager->CreateNtupleDColumn("globalTime_pre");
   rootManager->CreateNtupleDColumn("globalTime_post");
   rootManager->CreateNtupleDColumn("properTime_pre");
   rootManager->CreateNtupleDColumn("properTime_post");
   rootManager->CreateNtupleIColumn("trackID");
   rootManager->CreateNtupleIColumn("parentID");
   rootManager->CreateNtupleDColumn("kineticEnergy");
   rootManager->FinishNtuple();
   
}

void OutputRoot_step::AddHit_step(int ID, int eventId, int stepId, int volId, int particleID, int processID, double deltaEn, double x_mom, double y_mom, double z_mom, double x_prePos, double y_prePos, double z_prePos, double x_postPos, double y_postPos, double z_postPos, double deltaTime, double globalTime_pre, double globalTime_post, double properTime_pre, double properTime_post, int trackID, int parentID, double kineticEnergy)
{
    int cloId_step=0;
    rootManager->FillNtupleIColumn(siliTupleId_step, cloId_step, ID);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, eventId);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, stepId);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, volId);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, particleID);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, processID);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, deltaEn);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, x_mom);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, y_mom);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, z_mom);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, x_prePos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, y_prePos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, z_prePos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, x_postPos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, y_postPos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, z_postPos);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, deltaTime);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, globalTime_pre);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, globalTime_post);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, properTime_pre);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, properTime_post);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, trackID);
    rootManager->FillNtupleIColumn(siliTupleId_step, ++cloId_step, parentID);
    rootManager->FillNtupleDColumn(siliTupleId_step, ++cloId_step, kineticEnergy);
    rootManager->AddNtupleRow(siliTupleId_step);
}

OutputRoot_step* OutputRoot_step::instance=0;

