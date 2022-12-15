#include "R3BSofTofWMapped2TcalPar.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "R3BSofTcalPar.h"
#include "R3BSofTofWMappedData.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"

#include <iostream>
#include <stdlib.h>

// R3BSofTofWMapped2TcalPar: Default Constructor --------------------------
R3BSofTofWMapped2TcalPar::R3BSofTofWMapped2TcalPar()
    : FairTask("R3BSofTofWMapped2TcalPar", 1)
    , fNumDetectors(28)
    , fNumChannels(2)
    , fNumTcalParsPerSignal(1000)
    , fMinStatistics(100)
    , fMappedTofW(NULL)
    , fTcalPar(NULL)
    , fOutputFile(NULL)
{}

// R3BSofTofWMapped2TcalPar: Standard Constructor --------------------------
R3BSofTofWMapped2TcalPar::R3BSofTofWMapped2TcalPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumDetectors(28)
    , fNumChannels(2)
    , fNumTcalParsPerSignal(1000)
    , fMinStatistics(100)
    , fMappedTofW(NULL)
    , fTcalPar(NULL)
    , fOutputFile(NULL)

{}

// R3BSofTofWMapped2TcalPar: Destructor ----------------------------------------
R3BSofTofWMapped2TcalPar::~R3BSofTofWMapped2TcalPar()
{
    if (fMappedTofW)
        delete fMappedTofW;
    if (fTcalPar)
        delete fTcalPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTofWMapped2TcalPar::Init()
{
    LOG(info) << "R3BSofTofWMapped2TcalPar::Init()";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm) {
        LOG(error) << "R3BSofTofWMapped2TcalPar::Init() FairRootManager not found";
        return kFATAL;
    }

    // --- ----------------- --- //
    // --- INPUT MAPPED DATA --- //
    // --- ----------------- --- //

    fMappedTofW = (TClonesArray*)rm->GetObject("SofTofWMappedData");   // see Instance->Register in R3BSofTofWReader.cxx
    if (!fMappedTofW) {
        LOG(error) << "R3BSofTofWMapped2TcalPar::Init() SofTofWMappedData not found";
        return kFATAL;
    }

    // --- ---------------------------------- --- //
    // --- SOF TOFW TCAL PARAMETERS CONTAINER --- //
    // --- ---------------------------------- --- //

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb) {
        LOG(error) << "R3BSofTofWMapped2TcalPar::Init() FairRuntimeDb not found";
        return kFATAL;
    }
    fTcalPar = (R3BSofTcalPar*)rtdb->getContainer("SofTofWTcalPar");
    if (!fTcalPar) {
        LOG(error) << "R3BSofTofWMapped2TcalPar::Init() Couldn't get handle on SofTofWTcalPar container";
        return kFATAL;
    }

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh_TimeFineBin = new TH1F*[fNumDetectors * fNumChannels];
    fh_TimeFineNs = new TH1F*[fNumDetectors * fNumChannels];
    for (Int_t det = 0; det < fNumDetectors; det++) {
        for (Int_t ch = 0; ch < fNumChannels; ch++) {
            sprintf(name, "TimeFineBin_TofW_P%i_Pmt%i_Sig%i", det + 1, ch + 1, det * fNumChannels + ch);
            fh_TimeFineBin[det * fNumChannels + ch] =
                new TH1F(name, name, fNumTcalParsPerSignal, 0, fNumTcalParsPerSignal);
            sprintf(name, "TimeFineNs_TofW_P%i_Pmt%i_Sig%i", det + 1, ch + 1, det * fNumChannels + ch);
            fh_TimeFineNs[det * fNumChannels + ch] =
                new TH1F(name, name, fNumTcalParsPerSignal, 0, fNumTcalParsPerSignal);
        }
    }

    return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofTofWMapped2TcalPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofTofWMapped2TcalPar::Exec(Option_t* opt)
{

    // --- --------------------- --- //
    // --- LOOP OVER MAPPED HITS --- //
    // --- --------------------- --- //

    // nHits = number of hits per event
    UInt_t nHits = fMappedTofW->GetEntries();
    for (UInt_t ihit = 0; ihit < nHits; ihit++) {
        R3BSofTofWMappedData* hit = (R3BSofTofWMappedData*)fMappedTofW->At(ihit);
        if (!hit) {
            LOG(warn) << "R3BSofTofWMapped2TcalPar::Exec() : could not get hit";
            continue;   // should not happen
        }

        // *** ************************************* *** //
        // *** Numbers in Mapped Data are 1-based    *** //
        // *** first plastic on the right is 1       *** //
        // *** first plastic on the left is 28       *** //
        // *** PmtDown is 1                          *** //
        // *** PmtUp is 2                            *** //
        // *** ************************************* *** //
        // *** SofTofW PmtDown SIGNAL for P1 is 0    *** //
        // *** SofTofW PmtUp SIGNAL for P1 is 1      *** //
        // *** SofTofW PmtDown SIGNAL for P2 is 2    *** //
        // *** SofTofW PmtUp SIGNAL for P2 is 3      *** //
        // ***     (...)                             *** //
        // *** SofTofW PmtDown SIGNAL for P28 is 54  *** //
        // *** SofTofW PmtUp SIGNAL for P28 is 55    *** //
        // *** ************************************* *** //
        UInt_t iSignal = (hit->GetDetector() - 1) * fNumChannels + (hit->GetPmt() - 1);
        if ((0 <= iSignal) && (iSignal < fNumDetectors * fNumChannels))
            fh_TimeFineBin[iSignal]->Fill(hit->GetTimeFine());
        else
            LOG(error) << "R3BSofTofWMapped2TcalPar::Exec() Number of signals out of range: " << iSignal
                       << " instead of [0," << fNumDetectors * fNumChannels << "] "
                       << " det = " << hit->GetDetector() << " fNumChannels = " << fNumChannels
                       << " pmt = " << hit->GetPmt();

    }   // end of loop over the number of hits per event in MappedTofW
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTofWMapped2TcalPar::Reset() {}

void R3BSofTofWMapped2TcalPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTofWMapped2TcalPar::FinishTask()
{
    CalculateVftxTcalParams();
    fTcalPar->printParams();
}

//------------------
void R3BSofTofWMapped2TcalPar::CalculateVftxTcalParams()
{
    LOG(info) << "R3BSofTofWMapped2TcalPar: CalculateVftxTcalParams()";

    fTcalPar->SetNumDetectors(fNumDetectors);
    fTcalPar->SetNumChannels(fNumChannels);
    fTcalPar->SetNumTcalParsPerSignal(fNumTcalParsPerSignal);

    UInt_t IntegralTot;
    UInt_t IntegralPartial;
    Double_t Bin2Ns[fNumTcalParsPerSignal];

    for (Int_t sig = 0; sig < fNumDetectors * fNumChannels; sig++) {
        if (fh_TimeFineBin[sig]->GetEntries() > fMinStatistics) {
            IntegralTot = fh_TimeFineBin[sig]->Integral();
            IntegralPartial = 0;
            for (Int_t bin = 0; bin < fNumTcalParsPerSignal; bin++) {
                IntegralPartial += fh_TimeFineBin[sig]->GetBinContent(bin + 1);
                Bin2Ns[bin] = 5. * ((Double_t)IntegralPartial) / (Double_t)IntegralTot;
                fh_TimeFineNs[sig]->SetBinContent(bin + 1, Bin2Ns[bin]);
                fTcalPar->SetSignalTcalParams(Bin2Ns[bin], sig * fNumTcalParsPerSignal + bin);
            }
            fTcalPar->SetClockOffset(0.0, sig);
        }
        fh_TimeFineNs[sig]->Write();   // will be empty if fh_TimeFineBin[sig]->GetEntries()<fMinStatistics
        fh_TimeFineBin[sig]->Write();
    }
    fTcalPar->setChanged();
    return;
}

ClassImp(R3BSofTofWMapped2TcalPar)
