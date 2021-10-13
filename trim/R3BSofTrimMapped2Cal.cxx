// ------------------------------------------------------------
// -----         R3BSofTrimMapped2Cal source file         -----
// ------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// Trim headers
#include "R3BSofTrimCalPar.h"
#include "R3BSofTrimMapped2Cal.h"
#include "R3BSofTrimMappedData.h"

// R3BSofTrimMapped2Cal: Default Constructor --------------------------
R3BSofTrimMapped2Cal::R3BSofTrimMapped2Cal()
    : FairTask("R3BSof Trim Cal Calibrator", 1)
    , fCal_Par(NULL)
    , fTrimMappedData(NULL)
    , fTrimCalData(NULL)
    , fOnline(kFALSE)
    , fNumSections(3)
    , fNumAnodes(6)
{
    fNumChannels = fNumAnodes + 2; // anodes + Tref + Ttrig
}

// R3BSofTrimMapped2CalPar: Standard Constructor --------------------------
R3BSofTrimMapped2Cal::R3BSofTrimMapped2Cal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fCal_Par(NULL)
    , fTrimMappedData(NULL)
    , fTrimCalData(NULL)
    , fOnline(kFALSE)
    , fNumSections(3)
    , fNumAnodes(6)
{
    fNumChannels = fNumAnodes + 2; // anodes + Tref + Ttrig
}

// Virtual R3BSofTrimMapped2Cal: Destructor
R3BSofTrimMapped2Cal::~R3BSofTrimMapped2Cal()
{
    LOG(INFO) << "R3BSofTrimMapped2Cal: Delete instance";
    if (fTrimMappedData)
        delete fTrimMappedData;
    if (fTrimCalData)
        delete fTrimCalData;
}

void R3BSofTrimMapped2Cal::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BSofTrimCalPar*)rtdb->getContainer("trimCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTrimMapped2CalPar::Init() Couldn't get handle on trimCalPar container";
        return;
    }
    else
    {
        if (fNumSections != fCal_Par->GetNumSections())
            LOG(INFO) << "R3BSofTrimMapped2CalPar::Init() fNumSections=" << fNumSections << " different from parameter "
                      << fCal_Par->GetNumSections();
        if (fNumAnodes != fCal_Par->GetNumAnodes())
            LOG(INFO) << "R3BSofTrimMapped2CalPar::Init() fNumAnodes=" << fNumAnodes << " different from parameter "
                      << fCal_Par->GetNumAnodes();
        LOG(INFO) << "R3BSofTrimMapped2CalPar:: trimCalPar container open";
    }
    return;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimMapped2Cal::Init()
{
    LOG(INFO) << "R3BSofTrimMapped2Cal::Init()";

    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    // --- ----------------- --- //
    // --- INPUT MAPPED DATA --- //
    // --- ----------------- --- //
    fTrimMappedData = (TClonesArray*)rootManager->GetObject("TrimMappedData");
    if (!fTrimMappedData)
    {
        return kFATAL;
    }

    // --- --------------- --- //
    // --- OUTPUT CAL DATA --- //
    // --- --------------- --- //
    fTrimCalData = new TClonesArray("R3BSofTrimCalData", MAX_MULT_TRIM_CAL * 8);
    rootManager->Register("TrimCalData", "Trim Cal", fTrimCalData, !fOnline);

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTrimMapped2Cal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTrimMapped2Cal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // get the parameters
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTrimMapped2Cal: NOT Container Parameter!!";
    }

    // Reading input mapped data per anode
    //   --> number of channels = number of anodes (6: id=1..6)
    //                             + number of Tref (1: id=fNumAnodes+1)
    //                             + number of Ttrig (1: id=fNumAnodes+2)
    Int_t nHits = fTrimMappedData->GetEntries();
    if (!nHits)
        return;
    // FIX ME TO BE MOVED TO Init()
    UInt_t mult[fNumSections * fNumChannels];
    UShort_t iTraw[fNumSections * fNumChannels][MAX_MULT_TRIM_CAL];
    UShort_t iEraw[fNumSections * fNumChannels][MAX_MULT_TRIM_CAL];
    Int_t iSec;
    Int_t iCh;
    for (UInt_t sec = 0; sec < fNumSections; sec++)
    {
        for (UInt_t a = 0; a < fNumChannels; a++)
        {
            mult[a + fNumChannels * sec] = 0;
            for (UShort_t m = 0; m < MAX_MULT_TRIM_CAL; m++)
            {
                iTraw[a + fNumChannels * sec][m] = 0;
                iEraw[a + fNumChannels * sec][m] = 0;
            }
        }
    }

    for (Int_t ihit = 0; ihit < nHits; ihit++)
    {
        R3BSofTrimMappedData* hit = (R3BSofTrimMappedData*)fTrimMappedData->At(ihit);
        if (hit->GetPileupStatus() || hit->GetOverflowStatus())
            continue;
        iSec = hit->GetSecID() - 1;
        iCh = hit->GetAnodeID() - 1;
        if (mult[iCh + fNumChannels * iSec] > MAX_MULT_TRIM_CAL)
            continue;
        iTraw[iCh + fNumChannels * iSec][mult[iCh + fNumChannels * iSec]] = hit->GetTime();
        iEraw[iCh + fNumChannels * iSec][mult[iCh + fNumChannels * iSec]] = hit->GetEnergy();
        mult[iCh + fNumChannels * iSec]++;
    } // end of loop over the mapped data

    // Fill data only if there is a unique TREF signal
    Double_t dtraw, dtal;
    Float_t esub, ematch, eal;
    for (Int_t s = 0; s < fNumSections; s++)
    {
        if (mult[fNumAnodes + fNumChannels * s] == 1)
        {
            for (Int_t a = 0; a < fNumAnodes; a++)
            {
                if (mult[a + fNumChannels * s] > 0)
                {
                    for (Int_t i = 0; i < mult[a + fNumChannels * s]; i++)
                    {
                        dtraw = (Double_t)iTraw[a + fNumChannels * s][i] -
                                (Double_t)iTraw[fNumAnodes + fNumChannels * s][0];
                        dtal = dtraw + fCal_Par->GetDriftTimeOffset(s + 1, a + 1);
                        esub = (Float_t)iEraw[a + fNumChannels * s][i] - fCal_Par->GetEnergyPedestal(s + 1, a + 1);
                        ematch = esub * fCal_Par->GetEnergyMatchGain(s + 1, a + 1);
                        AddCalData(s + 1, a + 1, dtraw, dtal, esub, ematch);
                        // std::cout << "Add Cal Data: s=" << s+1 << ", a=" << a+1<< ", ematch=" << ematch << std::endl;
                    }
                } // end of check that anode has data
            }     // end of loop over the anodes
        }         // end of one Tref per Section
    }             // end of loop over section

    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTrimMapped2Cal::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTrimMapped2Cal::Reset()
{
    LOG(DEBUG) << "Clearing TrimCalData Structure";
    if (fTrimCalData)
        fTrimCalData->Clear();
}

// -----   Private method AddCalData  --------------------------------------------
R3BSofTrimCalData* R3BSofTrimMapped2Cal::AddCalData(Int_t secID,
                                                    Int_t anodeID,
                                                    Double_t dtraw,
                                                    Double_t dtal,
                                                    Float_t esub,
                                                    Float_t ematch)
{
    // It fills the R3BSofTrimCalData
    TClonesArray& clref = *fTrimCalData;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrimCalData(secID, anodeID, dtraw, dtal, esub, ematch);
}

ClassImp(R3BSofTrimMapped2Cal)
