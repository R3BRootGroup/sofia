#include "R3BSofTofWTcal2SingleTcal.h"

#include "R3BSofSciSingleTcalData.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTcalData.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

R3BSofTofWTcal2SingleTcal::R3BSofTofWTcal2SingleTcal()
    : FairTask("R3BSofTofWTcal2SingleTcal", 1)
    , fSciSingleTcal(NULL)
    , fTofWTcal(NULL)
    , fTofWSingleTcal(NULL)
    , fSciRawTofPar(NULL)
    , fOnline(kFALSE)
    , fNevent(0)
    , fNumPaddles(28)
    , fNumPmts(2)
{
}
R3BSofTofWTcal2SingleTcal::R3BSofTofWTcal2SingleTcal(Int_t nPaddles, Int_t nPmts)
    : FairTask("R3BSofTofWTcal2SingleTcal", 1)
    , fSciSingleTcal(NULL)
    , fTofWTcal(NULL)
    , fTofWSingleTcal(NULL)
    , fSciRawTofPar(NULL)
    , fOnline(kFALSE)
    , fNevent(0)
    , fNumPaddles(nPaddles)
    , fNumPmts(nPmts)
{
}

R3BSofTofWTcal2SingleTcal::~R3BSofTofWTcal2SingleTcal()
{
    if (fSciSingleTcal)
    {
        delete fSciSingleTcal;
    }
    if (fTofWTcal)
    {
        delete fTofWTcal;
    }
    if (fTofWSingleTcal)
    {
        delete fTofWSingleTcal;
    }
}

void R3BSofTofWTcal2SingleTcal::SetParContainers()
{

    fSciRawTofPar = (R3BSofSciRawTofPar*)FairRuntimeDb::instance()->getContainer("SofSciRawTofPar");
    if (!fSciRawTofPar)
    {
        LOG(ERROR)
            << "R3BSofTofWTcal2SingleTcal::SetParContainers() : Could not get access to SofSciRawTofPar-Container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofTofWTcal2SingleTcal::SetParContainers() : SofSciRawTofPar-Container found with "
                  << fSciRawTofPar->GetNumSignals() << " signals";
}

InitStatus R3BSofTofWTcal2SingleTcal::Init()
{

    LOG(INFO) << "R3BSofTofWTcal2SingleTcal::Init()";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        LOG(ERROR) << "R3BSofTofWTcal2SingleTcal::Couldn't instance the FairRootManager";
        return kFATAL;
    }

    // --- ----------------------------- --- //
    // --- INPUT TCAL DATA FROM ToF WALL --- //
    // --- ----------------------------- --- //

    fTofWTcal = (TClonesArray*)rm->GetObject("SofTofWTcalData");
    if (!fTofWTcal)
    {
        LOG(ERROR) << "R3BSofTofWTcal2SingleTcal::Couldn't get handle on SofTofWTcalData container";
        return kFATAL;
    }

    // --- --------------------------------- --- //
    // --- INPUT SINGLETCAL DATA FROM SofSci --- //
    // --- --------------------------------- --- //

    fSciSingleTcal = (TClonesArray*)rm->GetObject("SofSciSingleTcalData");
    if (!fSciSingleTcal)
    {
        LOG(ERROR) << "R3BSofTofWTcal2SingleTcal::Couldn't get handle on SofSciSingleTcalData container";
        return kFATAL;
    }

    // --- ----------------------- --- //
    // --- OUTPUT SINGLE TCAL DATA --- //
    // --- ----------------------- --- //

    // Register output array in tree
    fTofWSingleTcal = new TClonesArray("R3BSofTofWSingleTcalData", 10);

    if (!fOnline)
    {
        rm->Register("SofTofWSingleTcalData", "SofTofW", fTofWSingleTcal, kTRUE);
    }
    else
    {
        rm->Register("SofTofWSingleTcalData", "SofTofW", fTofWSingleTcal, kFALSE);
    }

    LOG(INFO) << "R3BSofTofWTcal2SingleTcal::Init DONE";

    return kSUCCESS;
}

InitStatus R3BSofTofWTcal2SingleTcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BSofTofWTcal2SingleTcal::Exec(Option_t* option)
{

    // Reset entries in output arrays, local arrays
    Reset();

    UShort_t iDet; // 0-based
    UShort_t iPmt; // 0-based
    Double_t iTraw[fNumPaddles * fNumPmts][16];
    UShort_t mult[fNumPaddles * fNumPmts];
    UShort_t mult_max = 0;

    for (UShort_t i = 0; i < fNumPaddles * fNumPmts; i++)
        mult[i] = 0;

    // --- ------------------------------------------- --- //
    // --- SOFSCI: GET THE Traw FROM THE SCI AT CAVE C --- //
    // --- ------------------------------------------- --- //
    UInt_t mult_SofSciCaveC = 0;
    Double_t iRawTime_SofSci = -1000000.;
    UInt_t nHitsPerEvent_SofSci = fSciSingleTcal->GetEntries();

    for (UInt_t i = 0; i < nHitsPerEvent_SofSci; i++)
    {
        R3BSofSciSingleTcalData* hitSci = (R3BSofSciSingleTcalData*)fSciSingleTcal->At(i);
        if (hitSci->GetDetector() == fSciRawTofPar->GetDetIdCaveC())
        {
            mult_SofSciCaveC++;
            iRawTime_SofSci = hitSci->GetRawTimeNs();
        }
    }

    // --- ------------------------------------------------------------------------- --- //
    // --- SOFTOFW: CALCULATE THE RAW TIME, TOF AND POSITION FOR THE PLASTICS HITTED --- //
    // --- ------------------------------------------------------------------------- --- //
    if (mult_SofSciCaveC == 1)
    {
        Int_t nHitsPerEvent_SofTofW = fTofWTcal->GetEntries();
        // get the multiplicity per PMT
        for (int ihit = 0; ihit < nHitsPerEvent_SofTofW; ihit++)
        {
            R3BSofTofWTcalData* hit = (R3BSofTofWTcalData*)fTofWTcal->At(ihit);
            if (!hit)
                continue;
            iDet = hit->GetDetector() - 1;
            iPmt = hit->GetPmt() - 1;
            if (mult_max >= 16)
                continue; // if multiplicity in a Pmt is higher than 16 are discarded, this code cannot handle it
            iTraw[iDet * fNumPmts + iPmt][mult[iDet * fNumPmts + iPmt]] = hit->GetRawTimeNs();
            mult[iDet * fNumPmts + iPmt]++;
            if (mult[iDet * fNumPmts + iPmt] > mult_max)
                mult_max = mult[iDet * fNumPmts + iPmt];
        } // end of loop over the TClonesArray of Tcal data

        if (nHitsPerEvent_SofTofW > 0)
        {
            Double_t iRawPos;
            Double_t iRawTime;
            Double_t iRawTof;
            for (UShort_t d = 0; d < fNumPaddles; d++)
            {
                iRawPos = -1000000.;
                iRawTime = -1000000.;
                iRawTof = -1000000.;
                // check mult==1 for the PMTup and PMTdown
                if ((mult[d * fNumPmts + 1] == 1) && (mult[d * fNumPmts] == 1))
                {
                    iRawPos = iTraw[d * fNumPmts + 1][0] - iTraw[d * fNumPmts][0]; // Raw position = Tdown - Tup
                    iRawTime = 0.5 * (iTraw[d * fNumPmts][0] + iTraw[d * fNumPmts + 1][0]);
                    iRawTof = (iRawTime - iRawTime_SofSci);
                    AddHitData(d + 1, iRawTime, iRawTof, iRawPos);
                }
            }
            ++fNevent;
        }
    }
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWTcal2SingleTcal::Reset()
{
    LOG(DEBUG) << "Clearing SofTofWSingleTcalData structure";
    if (fTofWSingleTcal)
        fTofWSingleTcal->Clear();
}

void R3BSofTofWTcal2SingleTcal::Finish() {}

// -----   Private method AddData  --------------------------------------------
R3BSofTofWSingleTcalData* R3BSofTofWTcal2SingleTcal::AddHitData(Int_t plastic,
                                                                Double_t time,
                                                                Double_t tof,
                                                                Double_t pos)
{
    // It fills the R3BSofTofWSingleTcalData
    TClonesArray& clref = *fTofWSingleTcal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWSingleTcalData(plastic, time, tof, pos);
}

ClassImp(R3BSofTofWTcal2SingleTcal)
