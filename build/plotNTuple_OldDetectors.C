{
  gROOT->Reset();
  
  // Draw histos filled by Geant4 simulation 
  //   
  TFile f("ScintDetOutput.root");  
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,4);

  // Get ntuple
  TNtuple* ntuple = (TNtuple*)f.Get("SiliInfo");

  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  ntuple->Draw("scintFront");

  // Draw Labs histogram in the pad 2
  c1->cd(2);
  ntuple->Draw("scintRear2h");

  // Draw Egap histogram in the pad 
  c1->cd(3);
  //set logarithmic scale for y
  //gPad->SetLogy(1);
  ntuple->Draw("scintRear4h");

  // Draw Lgap histogram in the pad 4
  c1->cd(4);
  //set logarithmic scale for y
  //gPad->SetLogy(1); 
  ntuple->Draw("scintRear6h");
  
  c1->cd(5);
  //set logarithmic scale for y
  //gPad->SetLogy(1); 
  ntuple->Draw("scintRear8h");
  
  c1->cd(6);
  //set logarithmic scale for y
  //gPad->SetLogy(1); 
  ntuple->Draw("scintRear10h");
  
   c1->cd(7);
  //set logarithmic scale for y
  //gPad->SetLogy(1); 
  ntuple->Draw("scintRear12h");
}  
