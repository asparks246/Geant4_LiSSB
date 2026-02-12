// Name this file “dump.cxx” and use as:
//
// root [0] .x dump.cxx("dump.txt")
//
// Produces “dump.txt” and “dump.xml” files.
//

void dump(const char *outputname = "15.txt", const char *fname = "15.root", const char *nname = "SiliInfo")
{
if (!fname || !(*fname) || !nname || !(*nname)) return; // just a precaution

TFile *f = TFile::Open(fname, "READ");
if (!f) return; // just a precaution

TTree *t; f->GetObject(nname, t);
if (!t) { delete f; return; } // just a precaution

t->SetScanField(0);
 ((TTreePlayer *)t->GetPlayer())->SetScanRedirect(kTRUE);
 ((TTreePlayer *)t->GetPlayer())->SetScanFileName(outputname);
t->Scan("*");
//t->SaveAs("dump.xml");

delete f; // no longer needed (automatically deletes "t")
}

//USE: root -b -l -q 'dump.cxx("ScintDetOutput_sampleonly.txt", "ScintDetOutput_sampleonly.root", "SiliInfo")'
//In directory where dump.cxx and root file is stored

