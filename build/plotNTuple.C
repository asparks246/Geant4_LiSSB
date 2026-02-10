{
  gROOT->Reset();
  
  // Draw histos filled by Geant4 simulation 
  //   
  TFile f("ScintDetOutput.root");  
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(1,2);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f.Get("SiliInfo");

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple->Draw("SmallDetector");

  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple->Draw("BigDetector");

  
}  
