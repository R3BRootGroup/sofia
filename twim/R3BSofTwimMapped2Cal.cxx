// ----------------------------------------------------------------
// -----         R3BSofTwimMapped2Cal source file             -----
// -----    Created 24/11/19  by J.L. Rodriguez-Sanchez       -----
// ----------------------------------------------------------------

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

// TWIM headers
#include "R3BSofTwimCalData.h"
#include "R3BSofTwimCalPar.h"
#include "R3BSofTwimMapped2Cal.h"
#include "R3BSofTwimMappedData.h"

// R3BSofTwimMapped2Cal: Default Constructor --------------------------
R3BSofTwimMapped2Cal::R3BSofTwimMapped2Cal()
    : FairTask("R3BSof Twim Cal Calibrator", 1)
    , fNumSec(MAX_NB_TWIMSEC)
    , fNumAnodes(MAX_NB_TWIMANODE)   // 16 anodes
    , fNumAnodesRef(MAX_NB_TWIMTREF) // 1 anode for TREF
    , fMaxMult(MAX_MULT_TWIM_CAL)
    , fNumParams(3)
    , fNumPosParams(2)
    , CalParams(NULL)
    , PosParams(NULL)
    , fCal_Par(NULL)
    , fTwimMappedDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofTwimMapped2CalPar: Standard Constructor --------------------------
R3BSofTwimMapped2Cal::R3BSofTwimMapped2Cal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSec(MAX_NB_TWIMSEC)
    , fNumAnodes(MAX_NB_TWIMANODE)   // 16 anodes
    , fNumAnodesRef(MAX_NB_TWIMTREF) // 1 anode for TREF
    , fMaxMult(MAX_MULT_TWIM_CAL)
    , fNumParams(3)
    , fNumPosParams(2)
    , CalParams(NULL)
    , PosParams(NULL)
    , fCal_Par(NULL)
    , fTwimMappedDataCA(NULL)
    , fTwimCalDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofTwimMapped2Cal: Destructor
R3BSofTwimMapped2Cal::~R3BSofTwimMapped2Cal()
{
    LOG(INFO) << "R3BSofTwimMapped2Cal: Delete instance";
    if (fTwimMappedDataCA)
        delete fTwimMappedDataCA;
    if (fTwimCalDataCA)
        delete fTwimCalDataCA;
}

void R3BSofTwimMapped2Cal::SetParContainers()
{
    // Parameter Container
    // Reading amsStripCalPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BSofTwimCalPar*)rtdb->getContainer("twimCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar::Init() Couldn't get handle on twimCalPar container";
    }
    else
    {
        LOG(INFO) << "R3BSofTwimMapped2CalPar:: twimCalPar container open";
    }
}

void R3BSofTwimMapped2Cal::SetParameter()
{

    //--- Parameter Container ---
    fNumSec = fCal_Par->GetNumSec();           // Number of sections
    fNumAnodes = fCal_Par->GetNumAnodes();     // Number of anodes per section
    fNumParams = fCal_Par->GetNumParamsEFit(); // Number of Parameters

    LOG(INFO) << "R3BSofTwimMapped2Cal: Nb sections: " << fNumSec;
    LOG(INFO) << "R3BSofTwimMapped2Cal: Nb anodes: " << fNumAnodes;
    LOG(INFO) << "R3BSofTwimMapped2Cal: Nb parameters from pedestal fit: " << fNumParams;

    CalParams = new TArrayF();
    Int_t array_size = (fNumSec * fNumAnodes) * fNumParams;
    CalParams->Set(array_size);
    CalParams = fCal_Par->GetAnodeCalParams(); // Array with the Cal parameters

    LOG(INFO) << "R3BSofTwimMapped2Cal: Nb parameters for position fit: " << fNumPosParams;
    PosParams = new TArrayF();
    Int_t array_pos = fNumSec * fNumAnodes * fNumPosParams;
    PosParams->Set(array_pos);
    PosParams = fCal_Par->GetPosParams(); // Array with the Cal parameters

    // Count the number of dead anodes
    for (Int_t s = 0; s < fNumSec; s++)
    {
        LOG(INFO) << "R3BSofTwimMapped2Cal::Dead anodes in section " << s;
        Int_t numdeadanodes = 0;
        for (Int_t i = 0; i < fNumAnodes; i++)
            if (CalParams->GetAt(s * fNumAnodes * fNumParams + fNumParams * i + 1) == -1)
                numdeadanodes++;
        LOG(INFO) << "Nb of dead anodes : " << numdeadanodes;
    }
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTwimMapped2Cal::Init()
{
    LOG(INFO) << "R3BSofTwimMapped2Cal: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTwimMappedDataCA = (TClonesArray*)rootManager->GetObject("TwimMappedData");
    if (!fTwimMappedDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Calibrated data
    fTwimCalDataCA = new TClonesArray("R3BSofTwimCalData", MAX_MULT_TWIM_CAL * (fNumAnodes + fNumAnodesRef));

    if (!fOnline)
    {
        rootManager->Register("TwimCalData", "TWIM Cal", fTwimCalDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("TwimCalData", "TWIM Cal", fTwimCalDataCA, kFALSE);
    }

    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTwimMapped2Cal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimMapped2Cal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTwimMapped2Cal: NOT Container Parameter!!";
    }

    // Reading the Input -- Mapped Data --
    Int_t nHits = fTwimMappedDataCA->GetEntries();
    // if (nHits != (NumSec * NumAnodes) && nHits > 0)
    //     LOG(WARNING) << "R3BSofTwimMapped2Cal: nHits!=NumSec*NumAnodes";
    if (!nHits)
        return;

    R3BSofTwimMappedData** mappedData;
    mappedData = new R3BSofTwimMappedData*[nHits];
    UShort_t secId = 0;
    UShort_t anodeId = 0;
    Double_t pedestal = 0.;

    for (Int_t s = 0; s < fNumSec; s++)
        for (Int_t i = 0; i < (fNumAnodes + fNumAnodesRef); i++)
        {
            mulanode[s][i] = 0;
            for (Int_t j = 0; j < fMaxMult; j++)
            {
                fE[s][j][i] = 0.;
                fDT[s][j][i] = 0.;
            }
        }

    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BSofTwimMappedData*)(fTwimMappedDataCA->At(i));
        secId = mappedData[i]->GetSecID();
        anodeId = mappedData[i]->GetAnodeID();

        if (anodeId < fNumAnodes && fCal_Par->GetInUse(secId + 1, anodeId + 1) == 1)
        {
            pedestal = CalParams->GetAt(secId * fNumAnodes * fNumParams + fNumParams * anodeId + 1);

            fE[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetEnergy() - pedestal;
            fDT[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetTime();
            mulanode[secId][anodeId]++;
        }
        else if (anodeId >= fNumAnodes)
        {
            fDT[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetTime(); // Ref. Time
            mulanode[secId][anodeId]++;
        }
    }

    // Fill data only if there is TREF signal
    for (Int_t s = 0; s < fNumSec; s++)
        if (mulanode[s][fNumAnodes] == 1)
        {
            for (Int_t i = 0; i < fNumAnodes; i++)
            {
                Int_t ii = s * fNumAnodes * fNumPosParams + fNumPosParams * i;
                Float_t a0 = PosParams->GetAt(ii);
                Float_t a1 = PosParams->GetAt(ii + 1);
                for (Int_t j = 0; j < mulanode[s][fNumAnodes]; j++)
                    for (Int_t k = 0; k < mulanode[s][i]; k++)
                    {
                        if (fE[s][k][i] > 0.)
                            AddCalData(s, i, a0 + a1 * (fDT[s][k][i] - fDT[s][j][fNumAnodes]), fE[s][k][i]);
                    }
            }
        }

    if (mappedData)
        delete mappedData;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTwimMapped2Cal::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTwimMapped2Cal::Reset()
{
    LOG(DEBUG) << "Clearing TwimCalData Structure";
    if (fTwimCalDataCA)
        fTwimCalDataCA->Clear();
}

// -----   Private method AddCalData  --------------------------------------------
R3BSofTwimCalData* R3BSofTwimMapped2Cal::AddCalData(UShort_t secID, UShort_t anodeID, Double_t dtime, Double_t energy)
{
    // It fills the R3BSofTwimCalData
    TClonesArray& clref = *fTwimCalDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTwimCalData(secID, anodeID, dtime, energy);
}

ClassImp(R3BSofTwimMapped2Cal)
