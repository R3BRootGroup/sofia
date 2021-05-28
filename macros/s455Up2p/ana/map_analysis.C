/**
 **  Macro for analysis of mapped data
 **
 **  Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 **  @since May 30th, 2021
 **
 **  After defining the input file (filename), execute the macro
 **  1) for all the events
 **     root -l map_analysis.C
 **  2) for a given number of events, for instance 'nev = 1000'
 **     root -l 'map_analysis.C(1000)'
 **
 **/

void map_analysis(const Float_t nev = -1)
{
    TStopwatch timer;
    // Initialize -------------------------------------------
    timer.Start();

    // INPUT FILE
    TString filename = "../unpack/s455_map_data_offline_20210530_114634.root";
    std::cout << "Input: " << filename << std::endl;

    // SETUP: Selection of detectors ----------------------------------------------------
    // --- R3B standard -----------------------------------------------------------------
    Bool_t fNeuland = false; // NeuLAND for neutrons behind GLAD
    Bool_t fAms = false;     // AMS tracking detectors
    Bool_t fCalifa = false;  // Califa calorimeter
    // --- Sofia ------------------------------------------------------------------------
    Bool_t fMwpc0 = false; // MWPC0 for tracking at entrance of Cave-C
    Bool_t fTrim = false;  // Triple-MUSIC for the HI beam charge-Z id, with charge-q states
    Bool_t fAt = false;    // Active Targer for Coulomb-induced fission
    Bool_t fSci = false;   // Start: Plastic scintillator for ToF
    Bool_t fMwpc1 = false; // MWPC1 for tracking of fragments in front of target
    Bool_t fMwpc2 = false; // MWPC2 for tracking of fragments before GLAD
    Bool_t fTwim = true;   // Twim: Ionization chamber for charge-Z of fragments
    Bool_t fMwpc3 = false; // MWPC3 for tracking of fragments behind GLAD
    Bool_t fTofW = true;   // ToF-Wall for time-of-flight of fragments behind GLAD

    // HISTOGRAMS
    TH1F* h1_emap_twim[2];
    h1_emap_twim[0] = new TH1F("h1_emap_twim_mul1", "mul=1", 2000, 0, 64000);
    h1_emap_twim[1] = new TH1F("h1_emap_twim_mul2", "mul=2", 2000, 0, 64000);

    // READING FILE AND TREE
    TFile* file = TFile::Open(filename);
    TTree* tree = (TTree*)file->Get("evt");

    // ENTRIES ----------------------------------------------
    Float_t nevents = tree->GetEntries();
    if (nev > -1)
        nevents = nev;
    std::cout << "Max. events: " << nevents << std::endl;

    // Header Data
    R3BEventHeader* HeaderData = new R3BEventHeader();
    TBranch* brHeaderData = tree->GetBranch("EventHeader.");
    if (brHeaderData)
        brHeaderData->SetAddress(&HeaderData);
    else
        std::cout << "WARNING: EventHeader. was not found" << std::endl;

    // Twim Mapped Data
    TClonesArray* TwimMapData = new TClonesArray("R3BSofTwimMappedData", 32);
    TBranch* branchMapTwim = tree->GetBranch("TwimMappedData");
    if (branchMapTwim)
        branchMapTwim->SetAddress(&TwimMapData);
    else
    {
        std::cout << "WARNING: TwimMappedData was not found" << std::endl;
        fTwim = false;
    }

    // TofW Mapped Data
    TClonesArray* TofWMapData = new TClonesArray("R3BSofTofWMappedData", 4);
    TBranch* branchMapTofW = tree->GetBranch("SofTofWMappedData");
    if (branchMapTofW)
        branchMapTofW->SetAddress(&TofWMapData);
    else
    {
        std::cout << "WARNING: TofWMappedData was not found" << std::endl;
        fTofW = false;
    }

    // LOOP IN THE EVENTS  ----------------------------------
    for (Int_t ie = 0; ie < nevents; ie++)
    {
        // Print current event in the analysis  -------------
        printf("Processed: %d of %.0f (%.2f of 100) \r", ie + 1, nevents, 100. * (ie + 1.0) / nevents);
        fflush(stdout);

        // Clear TCloneArrays  ------------------------------
        TwimMapData->Clear();
        TofWMapData->Clear();

        // Get entry data  ----------------------------------
        tree->GetEntry(ie);

        // Get Twin-Music data  -----------------------------
        Int_t twimHits = TwimMapData->GetEntries();
        if (twimHits > 0 && fTwim)
        {
            int mula = 0;
            R3BSofTwimMappedData** twimData = new R3BSofTwimMappedData*[twimHits];
            for (Int_t j = 0; j < twimHits; j++)
            {
                twimData[j] = (R3BSofTwimMappedData*)(TwimMapData->At(j));
                if (twimData[j]->GetSecID() == 2 && twimData[j]->GetAnodeID() == 8 && mula < 2)
                {
                    h1_emap_twim[mula]->Fill(twimData[j]->GetEnergy());
                    mula++;
                }
            }
            if (twimData)
                delete[] twimData;
        }

        // Get Tof-Wall data  -------------------------------
        Int_t tofwHits = TofWMapData->GetEntries();
        if (tofwHits > 0 && fTofW)
        {
            R3BSofTofWMappedData** tofwData = new R3BSofTofWMappedData*[tofwHits];
            for (Int_t j = 0; j < tofwHits; j++)
            {
                tofwData[j] = (R3BSofTofWMappedData*)(TofWMapData->At(j));
            }
            if (tofwData)
                delete[] tofwData;
        }
    }
    // END OF LOOP IN THE EVENTS  ---------------------------

    h1_emap_twim[0]->Draw();
    h1_emap_twim[1]->SetLineColor(2);
    h1_emap_twim[1]->Draw("same");

    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Real time " << rtime << " min, CPU time " << ctime << " min" << std::endl << std::endl;
}
