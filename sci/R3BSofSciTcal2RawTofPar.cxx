#include "R3BSofSciTcal2RawTofPar.h"

#include "R3BSofSciRawTofPar.h"
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

// R3BSofSciTcal2RawTofPar: Default Constructor --------------------------
R3BSofSciTcal2RawTofPar::R3BSofSciTcal2RawTofPar()
    : FairTask("R3BSofSciTcal2RawTofPar", 1)
    , fNumDets(1)
    , fNumChannels(3)
    , fDetIdCaveC(1)
    , fDetIdS2(0)
    , fDetIdS8(0)
    , fNumParsPerSignal(2)
    , fMinStatistics(0)
    , fTcal(NULL)
    , fRawTofPar(NULL)
    , fOutputFile(NULL)
{
    fNumSignals = fNumDets - 1;
}

// R3BSofSciTcal2RawTofPar: Standard Constructor --------------------------
R3BSofSciTcal2RawTofPar::R3BSofSciTcal2RawTofPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumDets(1)
    , fNumChannels(3)
    , fDetIdCaveC(1)
    , fDetIdS2(0)
    , fDetIdS8(0)
    , fNumParsPerSignal(2)
    , fMinStatistics(0)
    , fTcal(NULL)
    , fRawTofPar(NULL)
    , fOutputFile(NULL)

{
    fNumSignals = fNumDets - 1;
}

// R3BSofSciTcal2RawTofPar: Destructor ----------------------------------------
R3BSofSciTcal2RawTofPar::~R3BSofSciTcal2RawTofPar()
{
    if (fRawTofPar)
        delete fRawTofPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciTcal2RawTofPar::Init()
{

    LOG(INFO) << "R3BSofSciTcal2RawTofPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }

    // --- --------------- --- //
    // --- INPUT TCAL DATA --- //
    // --- ---------------- --- //

    fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");
    if (!fTcal)
    {
        LOG(ERROR) << "R3BSofSciTcal2RawTofPar::Init() Couldn't get handle on SofSciTcalData container";
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

    fRawTofPar = (R3BSofSciRawTofPar*)rtdb->getContainer("SofSciRawTofPar");
    if (!fRawTofPar)
    {
        LOG(ERROR) << "R3BSofSciTcal2RawTofPar::Init() Couldn't get handle on SofSciRawTofPar container";
        return kFATAL;
    }

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh_RawTofMult1 = new TH1D*[fNumSignals];
    fitRawTof = new TF1*[fNumSignals];
    for (Int_t detstart = 0; detstart < fNumDets - 1; detstart++)
    {
        sprintf(name, "TofRaw_Sci%i_to_Sci%i", detstart + 1, fDetIdCaveC);
        fh_RawTofMult1[detstart] = new TH1D(name, name, 40000, -1000, 3000);
        fitRawTof[detstart] = new TF1(Form("fitRawTof%i", detstart), "gaus", -1000, 3000);
    }

    return kSUCCESS;
}

// -----   Public method ReInit   --------------------------------------------
InitStatus R3BSofSciTcal2RawTofPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofSciTcal2RawTofPar::Exec(Option_t* opt)
{

    // --- ------------------------------ --- //
    // --- LOOP OVER TCAL HITS FOR SofSci --- //
    // --- ------------------------------ --- //

    // nHitsSci = number of hits per event
    UInt_t nHitsSci = fTcal->GetEntries(); // can be very high especially for S2 detector
    UShort_t mult[fNumDets * fNumChannels];
    Double_t iRawTimeNs[fNumDets * fNumChannels];
    UShort_t iDet; // 0 based Det number
    UShort_t iPmt; // 0 based Pmt number
    Double_t iTrawStart, iTrawStop;

    for (UShort_t d = 0; d < fNumDets; d++)
    {
        for (UShort_t ch = 0; ch < fNumChannels; ch++)
        {
            mult[d * fNumChannels + ch] = 0;
            iRawTimeNs[d * fNumChannels + ch] = 0;
        }
    }

    // CALCULATE THE MULTIPLICITY FOR EACH SIGNAL
    for (UInt_t ihit = 0; ihit < nHitsSci; ihit++)
    {
        R3BSofSciTcalData* hitSci = (R3BSofSciTcalData*)fTcal->At(ihit);
        if (!hitSci)
        {
            LOG(WARNING) << "R3BSofSciTcal2RawTofPar::Exec() : could not get hitSci";
            continue; // should not happen
        }
        iDet = hitSci->GetDetector() - 1; // get the 0 based DetiMax=20 number
        iPmt = hitSci->GetPmt() - 1;      // get the 0 based Pmt number
        iRawTimeNs[iDet * fNumChannels + iPmt] = hitSci->GetRawTimeNs();
        mult[iDet * fNumChannels + iPmt]++;
    } // end of for(ihit)

    // FILL THE HISTOGRAM ONLY FOR MULT=1 IN RIGHT AND MULT=1 IN LEFT
    UShort_t dstop = fDetIdCaveC - 1;
    for (UShort_t dstart = 0; dstart < fNumDets - 1; dstart++)
    {
        // check if mult=1 at RIGHT PMT [0] and mult=1 at LEFT PMT [1]
        if ((mult[dstart * fNumChannels] == 1) && (mult[dstart * fNumChannels + 1] == 1) &&
            (mult[dstop * fNumChannels] == 1) && (mult[dstop * fNumChannels + 1] == 1))
        {
            iTrawStart = 0.5 * (iRawTimeNs[dstart * fNumChannels] + iRawTimeNs[dstart * fNumChannels + 1]);
            iTrawStop = 0.5 * (iRawTimeNs[dstop * fNumChannels] + iRawTimeNs[dstop * fNumChannels + 1]);
            fh_RawTofMult1[dstart]->Fill(iTrawStop - iTrawStart + iRawTimeNs[dstart * fNumChannels + 2] -
                                         iRawTimeNs[dstop * fNumChannels + 2]);
        }
    }
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofSciTcal2RawTofPar::Reset() {}

void R3BSofSciTcal2RawTofPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciTcal2RawTofPar::FinishTask()
{
    CalculateRawTofParams();
    fRawTofPar->printParams();
}

// ------------------------------
void R3BSofSciTcal2RawTofPar::CalculateRawTofParams()
{
    LOG(INFO) << "R3BSofSciTcal2RawTofPar: CalculateRawTofParams()";

    fRawTofPar->SetNumDets(fNumDets);
    fRawTofPar->SetNumChannels(fNumChannels);
    fRawTofPar->SetDetIdCaveC(fDetIdCaveC);
    fRawTofPar->SetDetIdS2(fDetIdS2);
    fRawTofPar->SetDetIdS8(fDetIdS8);
    fRawTofPar->SetNumSignals(fNumSignals);
    fRawTofPar->SetNumParsPerSignal(fNumParsPerSignal);

    Int_t binmax = 0;
    Double_t maxx = 0.;
    Double_t iMax = 0;

    for (Int_t sig = 0; sig < fNumSignals; sig++)
    {
        if (fh_RawTofMult1[sig]->GetEntries() > fMinStatistics)
        {
            binmax = fh_RawTofMult1[sig]->GetMaximumBin();
            maxx = fh_RawTofMult1[sig]->GetXaxis()->GetBinCenter(binmax);
            iMax = fh_RawTofMult1[sig]->GetBinContent(binmax);
            // Set fit functions
            fitRawTof[sig]->SetParameter(0, iMax);
            fitRawTof[sig]->SetParameter(1, maxx);
            fitRawTof[sig]->SetParameter(2, 5.);
            fitRawTof[sig]->SetParLimits(2, .1, 20.);
            fh_RawTofMult1[sig]->Fit(fitRawTof[sig], "L", "", maxx - 20., maxx + 20.);
            //
            fRawTofPar->SetSignalParams(fitRawTof[sig]->GetParameter(1) - 5. * fitRawTof[sig]->GetParameter(2),
                                        sig * 2);
            fRawTofPar->SetSignalParams(fitRawTof[sig]->GetParameter(1) + 5. * fitRawTof[sig]->GetParameter(2),
                                        sig * 2 + 1);
        }
        fh_RawTofMult1[sig]->Write();
    }

    fRawTofPar->setChanged();
    return;
}

ClassImp(R3BSofSciTcal2RawTofPar)
