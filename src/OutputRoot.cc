#include "OutputRoot.hh"

OutputRoot::OutputRoot()
{
   rootManager=G4RootAnalysisManager::Instance();   
   rootManager->SetFirstNtupleId(0);
   rootManager->SetFirstHistoId(0);
   rootManager->SetFirstNtupleColumnId(0); 
}

void OutputRoot::CreateFile(G4String filename)
{
    rootManager->OpenFile(filename);
    siliTupleId=0;
    CreateSiliTuple();
}

void OutputRoot::SaveOutput()
{
   rootManager->Write();
   rootManager->CloseFile();
}

void OutputRoot::CreateSiliTuple()
{
   rootManager->CreateNtuple("SiliInfo","Energy deposit sili detector");
   rootManager->CreateNtupleIColumn("eventId");
   rootManager->CreateNtupleDColumn("SmallDetector");
   rootManager->CreateNtupleDColumn("BigDetector");
      
   //rootManager->CreateNtupleDColumn("scintFront");
   //rootManager->CreateNtupleDColumn("scintRear2h");
   //rootManager->CreateNtupleDColumn("scintRear4h");
   //rootManager->CreateNtupleDColumn("scintRear6h");
   //rootManager->CreateNtupleDColumn("scintRear8h");
   //rootManager->CreateNtupleDColumn("scintRear10h");
   //rootManager->CreateNtupleDColumn("scintRear12h");
   rootManager->FinishNtuple();
   
}

//void OutputRoot::AddHit(int eventId, double enFront, double enRear2h, double enRear4h, 
				//double enRear6h, double enRear8h, double enRear10h, double enRear12h)
void OutputRoot::AddHit(int eventId, double enSmallDetector, double enBigDetector)
{
    int cloId=0;
    
    rootManager->FillNtupleIColumn(siliTupleId, cloId, eventId);
    rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enSmallDetector);
    rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enBigDetector);
      
    //rootManager->FillNtupleIColumn(siliTupleId, cloId, eventId);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enFront);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear2h);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear4h);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear6h);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear8h);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear10h);
    //rootManager->FillNtupleDColumn(siliTupleId, ++cloId, enRear12h);
    rootManager->AddNtupleRow(siliTupleId);
}

OutputRoot* OutputRoot::instance=0;

