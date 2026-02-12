// dump_bulk.cxx
//
// Usage (from terminal):
// root -b -l -q 'dump_bulk.cxx("SiliInfo")'
//
// Processes ALL .root files in current directory
// and writes full tree content to matching .txt files
//

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TList.h>
#include <TSystemFile.h>
#include <TTreePlayer.h>
#include <iostream>

void dump_bulk(const char *treename = "SiliInfo")
{
    if (!treename || !(*treename)) return;

    TString dirname = gSystem->WorkingDirectory();
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();

    if (!files) {
        std::cout << "No files found in directory." << std::endl;
        return;
    }

    TSystemFile *file;
    TString fname;

    TIter next(files);
    while ((file = (TSystemFile*)next()))
    {
        fname = file->GetName();

        // Only process .root files
        if (!file->IsDirectory() && fname.EndsWith(".root"))
        {
            std::cout << "\nProcessing: " << fname << std::endl;

            TFile *f = TFile::Open(fname, "READ");
            if (!f) {
                std::cout << "  Could not open file." << std::endl;
                continue;
            }

            TTree *t = nullptr;
            f->GetObject(treename, t);

            if (!t) {
                std::cout << "  Tree '" << treename << "' not found." << std::endl;
                delete f;
                continue;
            }

            Long64_t nentries = t->GetEntries();
            std::cout << "  Entries in tree: " << nentries << std::endl;

            TString outname = fname;
            outname.ReplaceAll(".root", ".txt");

            t->SetScanField(0);
            ((TTreePlayer*)t->GetPlayer())->SetScanRedirect(kTRUE);
            ((TTreePlayer*)t->GetPlayer())->SetScanFileName(outname);

            // FORCE full scan
            t->Scan("*", "", "", nentries);

            std::cout << "  Wrote: " << outname << std::endl;

            delete f;
        }
    }

    delete files;

    std::cout << "\nBulk dump completed." << std::endl;
}
