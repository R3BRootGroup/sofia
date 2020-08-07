#include "R3BSofSciTcal2RawPosPar.h"

#include "R3BSofSciRawPosPar.h"
#include "R3BSofSciTcalData.h"

#include "R3BEventHeader.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>

// *** ************************************ *** //
// *** SofSci Pmt Right (Tcal Data)         *** //
// ***     * channel=1                      *** //
// *** ************************************ *** //
// *** SofSci Pmt Left (Tcal Data)          *** //
// ***     * channel=2                      *** //
// *** ************************************ *** //
// *** SofSci Common Ref (Tcal Data)        *** //
// ***     * channel=3                      *** //
// *** ************************************ *** //

// R3BSofSciTcal2RawPosPar: Default Constructor --------------------------
R3BSofSciTcal2RawPosPar::R3BSofSciTcal2RawPosPar()
    : FairTask("R3BSofSciTcal2RawPosPar", 1)
    , fNumDets(0)
    , fNumPmts(3) // Pmt + ref
    , fNumParsPerSignal(2)
    , fMinStatistics(0)
    , fTcal(NULL)
    , fRawPosPar(NULL)
    , fOutputFile(NULL)
{
    fNumSignals = fNumDets;
}

// R3BSofSciTcal2RawPosPar: Standard Constructor --------------------------
R3BSofSciTcal2RawPosPar::R3BSofSciTcal2RawPosPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumDets(0)
    , fNumPmts(3)
    , fNumParsPerSignal(2)
    , fMinStatistics(0)
    , fTcal(NULL)
    , fRawPosPar(NULL)
    , fOutputFile(NULL)

{
    fNumSignals = fNumDets;
}

// R3BSofSciTcal2RawPosPar: Destructor ----------------------------------------
R3BSofSciTcal2RawPosPar::~R3BSofSciTcal2RawPosPar()
{
    if (fRawPosPar)
        delete fRawPosPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciTcal2RawPosPar::Init()
{

    LOG(INFO) << "R3BSofSciTcal2RawPosPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }

    // --- --------------- --- //
    // --- INPUT TCAL DATA --- //
    // --- ---------------- --- //

    // scintillator at S2 and cave C
    fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");
    if (!fTcal)
    {
        LOG(ERROR) << "R3BSofSciTcal2RawPosPar::Init() Couldn't get handle on SofSciTcalData container";
        return kFATAL;
    }

    // --- ---------------------------------------- --- //
    // --- SOF SCI SINGLE TCAL PARAMETERS CONTAINER --- //
    // --- ---------------------------------------- --- //

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    fRawPosPar = (R3BSofSciRawPosPar*)rtdb->getContainer("SofSciRawPosPar");
    if (!fRawPosPar)
    {
        LOG(ERROR) << "R3BSofSciTcal2RawPosPar::Init() Couldn't get handle on SofSciRawPosPar container";
        return kFATAL;
    }

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh_RawPosMult1 = new TH1D*[fNumSignals];
    for (Int_t det = 0; det < fNumDets; det++)
    {
        sprintf(name, "PosRaw_Sci%i", det + 1);
        fh_RawPosMult1[det] = new TH1D(name, name, 20000, -10, 10);
    }

    return kSUCCESS;
}

// -----   Public method ReInit  --------------------------------------------
InitStatus R3BSofSciTcal2RawPosPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofSciTcal2RawPosPar::Exec(Option_t* opt)
{

    // --- ------------------------------ --- //
    // --- LOOP OVER TCAL HITS FOR SofSci --- //
    // --- ------------------------------ --- //

    // nHitsSci = number of hits per event
    UInt_t nHitsSci = fTcal->GetEntries(); // can be very high especially for S2 detector
    UShort_t mult[fNumDets * fNumPmts];
    Double_t iRawTimeNs[fNumDets * fNumPmts];
    UShort_t iDet; // 0 based Det number
    UShort_t iPmt; // 0 based Pmt number

    for (UShort_t d = 0; d < fNumDets; d++)
    {
        for (UShort_t ch = 0; ch < fNumPmts; ch++)
        {
            mult[d * fNumPmts + ch] = 0;
            iRawTimeNs[d * fNumPmts + ch] = 0;
        }
    }

    // CALCULATE THE MULTIPLICITY FOR EACH SIGNAL
    for (UInt_t ihit = 0; ihit < nHitsSci; ihit++)
    {
        R3BSofSciTcalData* hitSci = (R3BSofSciTcalData*)fTcal->At(ihit);
        if (!hitSci)
        {
            LOG(WARNING) << "R3BSofSciTcal2RawPosPar::Exec() : could not get hitSci";
            continue; // should not happen
        }
        iDet = hitSci->GetDetector() - 1; // get the 0 based Det number
        iPmt = hitSci->GetPmt() - 1;      // get the 0 based Pmt number
        iRawTimeNs[iDet * fNumPmts + iPmt] = hitSci->GetRawTimeNs();
        mult[iDet * fNumPmts + iPmt]++;
    } // end of for(ihit)

    // FILL THE HISTOGRAM ONLY FOR MULT=1 IN RIGHT AND MULT=1 IN LEFT
    for (UShort_t d = 0; d < fNumDets; d++)
    {
        // check if mult=1 at RIGHT PMT [0] and mult=1 at LEFT PMT [1]
        // ATTENTION : x increasing from left to right : TrawRIGHT-TrawLEFT
        if ((mult[d * fNumPmts] == 1) && (mult[d * fNumPmts + 1] == 1))
        {
            fh_RawPosMult1[d]->Fill(iRawTimeNs[d * fNumPmts] - iRawTimeNs[d * fNumPmts + 1]);
        }
    }
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofSciTcal2RawPosPar::Reset() {}

void R3BSofSciTcal2RawPosPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciTcal2RawPosPar::FinishTask()
{
    CalculateRawPosRawPosParams();
    // TO DO AND SHOULD BE IN #define #else #endif  ON NUMBER_OF_DETECTORS CASE : CalculateRawTofRawPosParams();
    fRawPosPar->printParams();
}

// ------------------------------
void R3BSofSciTcal2RawPosPar::CalculateRawPosRawPosParams()
{
    LOG(INFO) << "R3BSofSciTcal2RawPosPar: CalculateRawPosRawPosParams()";

    fRawPosPar->SetNumDets(fNumDets);
    fRawPosPar->SetNumPmts(fNumPmts);
    fRawPosPar->SetNumSignals(fNumSignals);
    fRawPosPar->SetNumParsPerSignal(fNumParsPerSignal);

    Double_t iMax;
    Int_t bin, binLimit;
    for (Int_t sig = 0; sig < fNumSignals; sig++)
    {
        if (fh_RawPosMult1[sig]->GetEntries() > fMinStatistics)
        {
            iMax = fh_RawPosMult1[sig]->GetMaximum();
            // LOWER LIMIT
            bin = 1;
            binLimit = 1;
            while ((bin <= fh_RawPosMult1[sig]->GetNbinsX()) && (binLimit == 1))
            {
                if (fh_RawPosMult1[sig]->GetBinContent(bin) > (iMax / 10000.))
                    binLimit = bin;
                bin++;
            }
            fRawPosPar->SetSignalParams(fh_RawPosMult1[sig]->GetBinLowEdge(binLimit), sig * 2);
            // HIGHER LIMIT
            bin = fh_RawPosMult1[sig]->GetNbinsX();
            binLimit = fh_RawPosMult1[sig]->GetNbinsX();
            while ((bin >= 1) && (binLimit == fh_RawPosMult1[sig]->GetNbinsX()))
            {
                if (fh_RawPosMult1[sig]->GetBinContent(bin) > (iMax / 10000.))
                    binLimit = bin;
                bin--;
            }
            fRawPosPar->SetSignalParams(fh_RawPosMult1[sig]->GetBinLowEdge(binLimit), sig * 2 + 1);
        }
        fh_RawPosMult1[sig]->Write();
    }

    fRawPosPar->setChanged();
    return;
}

ClassImp(R3BSofSciTcal2RawPosPar)
