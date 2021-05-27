// SofSci: scintillator at S2 and/or S8 and/or cave C
// REMINDER: x is increasing from RIGHT to LEFT
#include "R3BSofSciSingleTcal2Cal.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#define SPEED_OF_LIGHT_MNS 0.299792458

R3BSofSciSingleTcal2Cal::R3BSofSciSingleTcal2Cal()
    : FairTask("R3BSofSciSingleTcal2Cal", 1)
    , fSingleTcal(NULL)
    , fCal(NULL)
    , fPosPar(NULL)
    , fTofPar(NULL)
    , fOnline(kFALSE)
    , fNevent(0)
{
}

R3BSofSciSingleTcal2Cal::~R3BSofSciSingleTcal2Cal()
{
    LOG(INFO) << "R3BSofSciSingleTcal2Cal: Delete instance";
    if (fSingleTcal)
        delete fSingleTcal;
    if (fCal)
        delete fCal;
}

void R3BSofSciSingleTcal2Cal::SetParContainers()
{
    fPosPar = (R3BSofSciCalPosPar*)FairRuntimeDb::instance()->getContainer("SofSciCalPosPar");
    if (!fPosPar)
    {
        LOG(ERROR)
            << "R3BSofSciSingleTcal2Cal::SetParContainers() : Could not get access to SofSciCalPosPar-Container.";
        return;
    }

    fTofPar = (R3BSofSciCalTofPar*)FairRuntimeDb::instance()->getContainer("SofSciCalTofPar");
    if (!fTofPar)
    {
        LOG(ERROR)
            << "R3BSofSciSingleTcal2Cal::SetParContainers() : Could not get access to SofSciCalTofPar-Container.";
        return;
    }
}

InitStatus R3BSofSciSingleTcal2Cal::Init()
{

    LOG(INFO) << "R3BSofSciSingleTcal2Cal::Init()";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        LOG(ERROR) << "R3BSofSciSingleTcal2Cal::Couldn't instance the FairRootManager";
        return kFATAL;
    }

    // --- ---------------------- --- //
    // --- INPUT SINGLE TCAL DATA --- //
    // --- ---------------------- --- //

    fSingleTcal = (TClonesArray*)rm->GetObject("SofSciSingleTcalData");
    if (!fSingleTcal)
    {
        LOG(ERROR) << "R3BSofSciSingleTcal2Cal::Couldn't get handle on SofSciSingleTcalData container";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciSingleTcal2Cal::SofSciSingleTcalData items found";

    // --- --------------- --- //
    // --- OUTPUT CAL DATA --- //
    // --- --------------- --- //

    // Register output array in tree
    fCal = new TClonesArray("R3BSofSciCalData", fTofPar->GetNumDets());
    if (!fOnline)
    {
        rm->Register("SofSciCalData", "SofSci Cal", fCal, kTRUE);
    }
    else
    {
        rm->Register("SofSciCalData", "SofSci Cal", fCal, kFALSE);
    }

    LOG(INFO) << "R3BSofSciSingleTcal2Cal::SofSciCalData items created";

    return kSUCCESS;
}

InitStatus R3BSofSciSingleTcal2Cal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BSofSciSingleTcal2Cal::Exec(Option_t* option)
{
    UShort_t iDet, rank;
    Double_t RawPos_Ns, RawTofS2_Ns, RawTofS8_Ns;
    Double_t CalPos_Mm;
    Double_t CalVeloS2_MNs, CalVeloS8_MNs; // m/ns
    Double_t BetaS2, BetaS8;
    Double_t CalTofS2_Ns, CalTofS8_Ns;

    // initialize the output data
    Reset();

    Int_t nEntries = fSingleTcal->GetEntries();
    for (Int_t entry = 0; entry < nEntries; entry++)
    {
        R3BSofSciSingleTcalData* hitSTcal = (R3BSofSciSingleTcalData*)fSingleTcal->At(entry);
        if (!hitSTcal)
            continue;
        iDet = hitSTcal->GetDetector() - 1;

        RawPos_Ns = hitSTcal->GetRawPosNs();
        CalPos_Mm = fPosPar->GetParam(iDet * 2) + RawPos_Ns * fPosPar->GetParam(iDet * 2 + 1);

        RawTofS2_Ns = hitSTcal->GetRawTofNs_FromS2();
        if (RawTofS2_Ns != -100000. && fTofPar->GetDetIdS2() > 0 && hitSTcal->GetDetector() > fTofPar->GetDetIdS2())
        {
            rank = hitSTcal->GetDetector() - fTofPar->GetDetIdS2() - 1;
            CalVeloS2_MNs =
                1. / (fTofPar->GetTof2InvV_FromS2(rank * 2) + fTofPar->GetTof2InvV_FromS2(rank * 2 + 1) * RawTofS2_Ns);
            CalTofS2_Ns = fTofPar->GetFlightLength_FromS2(rank) / CalVeloS2_MNs;

            BetaS2 = CalVeloS2_MNs / (Double_t)SPEED_OF_LIGHT_MNS;
        }
        else
        {
            CalVeloS2_MNs = -1;
            BetaS2 = -1;
            CalTofS2_Ns = -1;
        }

        RawTofS8_Ns = hitSTcal->GetRawTofNs_FromS8();
        if (RawTofS8_Ns != -100000. && fTofPar->GetDetIdS8() > 0 && hitSTcal->GetDetector() > fTofPar->GetDetIdS8())
        {
            rank = hitSTcal->GetDetector() - fTofPar->GetDetIdS8() - 1;
            CalVeloS8_MNs =
                1. / (fTofPar->GetTof2InvV_FromS8(rank * 2) + fTofPar->GetTof2InvV_FromS8(rank * 2 + 1) * RawTofS8_Ns);
            CalTofS8_Ns = fTofPar->GetFlightLength_FromS8(rank) / CalVeloS8_MNs;
            BetaS8 = CalVeloS8_MNs / (Double_t)SPEED_OF_LIGHT_MNS;
        }
        else
        {
            CalVeloS8_MNs = -1;
            BetaS8 = -1;
            CalTofS8_Ns = -1;
        }

        AddCalData(iDet + 1, CalPos_Mm, BetaS2, BetaS8, CalTofS2_Ns, CalTofS8_Ns);
    }

    // if (fSingleTcal)
    //    delete fSingleTcal;

    ++fNevent;
}

// -----   Public method  --------------------------------------------
void R3BSofSciSingleTcal2Cal::Reset()
{
    if (fCal)
    {
        fCal->Clear();
    }
}

// -----   Private method AddCalData  --------------------------------------------
R3BSofSciCalData* R3BSofSciSingleTcal2Cal::AddCalData(Int_t iDet,
                                                      Double_t x,
                                                      Double_t b2,
                                                      Double_t b8,
                                                      Double_t t2,
                                                      Double_t t8)
{
    // It fills the R3BSofSciCalData
    TClonesArray& clref = *fCal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofSciCalData(iDet, x, b2, b8, t2, t8);
}

ClassImp(R3BSofSciSingleTcal2Cal)
