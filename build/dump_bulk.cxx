// dump_bulk.cxx
//
// Usage:
// root -b -l -q 'dump_bulk.cxx("SiliInfo")'
//
// This will process ALL .root files in the current directory
// and produce matching .txt files.
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

    // Get current directory
    TString dirname = gSystem->WorkingDirectory();
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();

    if (!files) return;

    TSystemFile *file;
    TString fname;

    TIter next(files);
    while ((file = (TSystemFile*)next()))
    {
        fname = file->GetName();

        // Process only .root files
        if (!file->IsDirectory() && fname.EndsWith(".root"))
        {
            std::cout << "Processing: " << fname << std::endl;

            TFile *f = TFile::Open(fname, "READ");
            if (!f) continue;

            TTree *t = nullptr;
            f->GetObject(treename, t);
            if (!t)
            {
                std::cout << "Tree not found in " << fname << std::endl;
                delete f;
                continue;
            }

            // Create output filename
            TString outname = fname;
            outname.ReplaceAll(".root", ".txt");

            t->SetScanField(0);
            ((TTreePlayer*)t->GetPlayer())->SetScanRedirect(kTRUE);
            ((TTreePlayer*)t->GetPlayer())->SetScanFileName(outname);
            t->Scan("*");

            delete f;
        }
    }

    delete files;
}

