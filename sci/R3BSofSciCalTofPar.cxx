#include "R3BSofSciCalTofPar.h"

#include "FairLogger.h"
#include "FairParamList.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"

#include <iostream>

#define MAX_PARS 10

using std::cout;
using std::endl;

// ---- Standard Constructor ---------------------------------------------------
R3BSofSciCalTofPar::R3BSofSciCalTofPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNumDets(2)
    , fDetIdCaveC(2)
    , fDetIdS2(1)
    , fDetIdS8(0)
{
    fTof2InvV_FromS2 = new TArrayF(MAX_PARS);
    fTof2InvV_FromS8 = new TArrayF(MAX_PARS);
    fFlightLength_FromS2 = new TArrayF(MAX_PARS);
    fFlightLength_FromS8 = new TArrayF(MAX_PARS);
}

// ----  Destructor ------------------------------------------------------------
R3BSofSciCalTofPar::~R3BSofSciCalTofPar()
{
    clear();
    if (fTof2InvV_FromS2)
    {
        delete fTof2InvV_FromS2;
    }
    if (fTof2InvV_FromS8)
    {
        delete fTof2InvV_FromS8;
    }
    if (fFlightLength_FromS2)
    {
        delete fFlightLength_FromS2;
    }
    if (fFlightLength_FromS8)
    {
        delete fFlightLength_FromS8;
    }
}

// ----  Method clear ----------------------------------------------------------
void R3BSofSciCalTofPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BSofSciCalTofPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciCalTofPar::putParams() called";
    if (!list)
    {
        return;
    }

    list->add("nDets", fNumDets);
    list->add("idDetS2", fDetIdS2);
    list->add("idDetS8", fDetIdS8);
    list->add("idDetCaveC", fDetIdCaveC);

    Int_t array_size;

    if (fDetIdS8 > 0)
    {
        array_size = (fNumDets - fDetIdS8) * 2;
        fTof2InvV_FromS8->Set(array_size);
        list->add("Tof2InvV_FromS8", *fTof2InvV_FromS8);
        LOG(INFO) << "R3BSofSciCalTofPar::putParams - Array Size for Tof from S8 2 InvV: " << array_size;

        array_size = fNumDets - fDetIdS8;
        fFlightLength_FromS8->Set(array_size);
        list->add("LS8", *fFlightLength_FromS8);
        LOG(INFO) << "R3BSofSciCalTofPar::putParams - Array Size for Flight Length from S8: " << array_size;
    }

    if (fDetIdS2 > 0)
    {
        array_size = (fNumDets - fDetIdS2) * 2;
        fTof2InvV_FromS2->Set(array_size);
        list->add("Tof2InvV_FromS2", *fTof2InvV_FromS2);
        LOG(INFO) << "R3BSofSciCalTofPar::putParams - Array Size for Tof from S2 2 InvV: " << array_size;

        array_size = fNumDets - fDetIdS2;
        fFlightLength_FromS2->Set(array_size);
        list->add("LS2", *fFlightLength_FromS2);
        LOG(INFO) << "R3BSofSciCalTofPar::putParams - Array Size for Flight Length from S2: " << array_size;
    }
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BSofSciCalTofPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BSofSciCalTofPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("nDets", &fNumDets))
    {
        return kFALSE;
    }
    if (!list->fill("idDetCaveC", &fDetIdCaveC))
    {
        return kFALSE;
    }
    if (!list->fill("idDetS2", &fDetIdS2))
    {
        return kFALSE;
    }
    if (!list->fill("idDetS8", &fDetIdS8))
    {
        return kFALSE;
    }

    Int_t array_size;

    if (fDetIdS8 > 0)
    {
        array_size = (fNumDets - fDetIdS8) * 2;
        fTof2InvV_FromS8->Set(array_size);
        LOG(INFO) << "R3BSofSciCalTofPar::getParams - Array Size for Tof from S8: " << array_size;

        array_size = fNumDets - fDetIdS8;
        fFlightLength_FromS8->Set(array_size);
        LOG(INFO) << "R3BSofSciCalTofPar::getParams - Array Size for Flight Length from S8: " << array_size;
    }
    if (!(list->fill("Tof2InvV_FromS8", fTof2InvV_FromS8)))
    {
        if (fDetIdS8 == 0)
            LOG(INFO) << "---Could not initialize fTof2InvV_FromS8: OK";
        else
        {
            LOG(INFO) << "---Could not initialize fTof2InvV_FromS8";
            return kFALSE;
        }
    }
    if (!(list->fill("LS8", fFlightLength_FromS8)))
    {
        if (fDetIdS8 == 0)
            LOG(INFO) << "---Could not initialize fFlightLength_FromS8: OK";
        else
        {
            LOG(INFO) << "---Could not initialize fFlightLength_FromS8";
            return kFALSE;
        }
    }

    if (fDetIdS2 > 0)
    {
        array_size = (fNumDets - fDetIdS2) * 2;
        fTof2InvV_FromS2->Set(array_size);
        LOG(INFO) << "R3BSofSciCalTofPar::getParams - Array Size for Tof from S2: " << array_size;
        array_size = fNumDets - fDetIdS2;
        fFlightLength_FromS2->Set(array_size);
        LOG(INFO) << "R3BSofSciCalTofPar::getParams - Array Size for Flight Length from S2: " << array_size;
    }
    if (!(list->fill("Tof2InvV_FromS2", fTof2InvV_FromS2)))
    {
        if (fDetIdS2 == 0)
            LOG(INFO) << "---Could not initialize fTof2InvV_FromS2: OK";
        else
        {
            LOG(INFO) << "---Could not initialize fTof2InvV_FromS2";
            return kFALSE;
        }
    }
    if (!(list->fill("LS2", fFlightLength_FromS2)))
    {
        if (fDetIdS2 == 0)
            LOG(INFO) << "---Could not initialize fFlightLength_FromS2: OK";
        else
        {
            LOG(INFO) << "---Could not initialize fFlightLength_FromS2";
            return kFALSE;
        }
    }
    return kTRUE;
}

// ----  Method printParams ----------------------------------------------------
void R3BSofSciCalTofPar::printParams()
{
    LOG(INFO) << "R3BSofSciCalTofPar: SofSciCalTof Parameters: ";

    cout << "fNumDets = " << fNumDets << endl;
    cout << "fDetIdS2 = " << fDetIdS2 << endl;
    cout << "fDetIdS8 = " << fDetIdS8 << endl;
    cout << "fDetIdCaveC = " << fDetIdCaveC << endl;

    Int_t rankL;

    if (fDetIdS8 > 0)
    {
        for (Int_t det = fDetIdS8 + 1; det <= fNumDets; det++)
        {
            rankL = det - fDetIdS8 - 1;
            cout << "SofSci " << det << ": Flight Length From S8 = " << fFlightLength_FromS8->GetAt(rankL) << endl;
            cout << "          Tof2InvV_FromS8, p0 = " << fTof2InvV_FromS8->GetAt(rankL * 2)
                 << ", p1 = " << fTof2InvV_FromS8->GetAt(rankL * 2 + 1) << endl;
        }
    }
    if (fDetIdS2 > 0)
    {
        for (Int_t det = fDetIdS2 + 1; det <= fNumDets; det++)
        {
            rankL = det - fDetIdS2 - 1;
            cout << "SofSci " << det << ": Flight Length From S2 = " << fFlightLength_FromS2->GetAt(rankL) << endl;
            cout << "          Tof2InvV_FromS2, p0 = " << fTof2InvV_FromS2->GetAt(rankL * 2)
                 << ", p1 = " << fTof2InvV_FromS2->GetAt(rankL * 2 + 1) << endl;
        }
    }
}
