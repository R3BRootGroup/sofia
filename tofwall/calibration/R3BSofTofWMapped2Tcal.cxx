
#include "R3BSofTofWMapped2Tcal.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "R3BSofTcalPar.h"
#include "R3BSofTofWMappedData.h"
#include "R3BSofTofWTcalData.h"

R3BSofTofWMapped2Tcal::R3BSofTofWMapped2Tcal()
    : FairTask("R3BSofTofWMapped2Tcal", 1)
    , fMapped(NULL)
    , fTcalPar(NULL)
    , fTcal(NULL)
    , fNumTcal(0)
    , fOnline(kFALSE)
    , fNevent(0)
{
}

R3BSofTofWMapped2Tcal::~R3BSofTofWMapped2Tcal()
{
    if (fMapped)
        delete fMapped;
    if (fTcal)
        delete fTcal;
}

InitStatus R3BSofTofWMapped2Tcal::Init()
{
    LOG(info) << "R3BSofTofWMapped2Tcal::Init()";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        LOG(error) << "R3BSofTofWMapped2Tcal::Init() Couldn't instance the FairRootManager";
        return kFATAL;
    }

    // scintillator at S2 and cave C
    fMapped = (TClonesArray*)rm->GetObject("SofTofWMappedData");
    if (!fMapped)
    {
        LOG(error) << "R3BSofTofWMapped2Tcal::Init() Couldn't get handle on SofTofWMappedData container";
        return kFATAL;
    }
    else
        LOG(info) << "R3BSofTofWMapped2Tcal::Init() SofTofWMappedData items found";

    // Register output array in tree
    fTcal = new TClonesArray("R3BSofTofWTcalData", 10);
    rm->Register("SofTofWTcalData", "SofTofW", fTcal, !fOnline);

    return kSUCCESS;
}

void R3BSofTofWMapped2Tcal::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(error) << "FairRuntimeDb not opened!";
    }

    fTcalPar = (R3BSofTcalPar*)rtdb->getContainer("SofTofWTcalPar");
    if (!fTcalPar)
    {
        LOG(error) << "R3BSofTofWMapped2Tcal::SetParContainers() : Could not get access to SofTofWTcalPar-Container.";
        return;
    }
    else
        LOG(info) << "R3BSofTofWMapped2Tcal::SetParContainers() : SofTofWTcalPar-Container found with "
                  << fTcalPar->GetNumDetectors() << " detectors and " << fTcalPar->GetNumChannels() << " channels";
    return;
}

InitStatus R3BSofTofWMapped2Tcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BSofTofWMapped2Tcal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    UShort_t iDet;
    UShort_t iCh;
    UInt_t iTf;
    UInt_t iTc;
    Double_t tns = 0.0;

    Int_t nHitsPerEvent_SofTofW = fMapped->GetEntriesFast();
    for (int ihit = 0; ihit < nHitsPerEvent_SofTofW; ihit++)
    {
        R3BSofTofWMappedData* hit = (R3BSofTofWMappedData*)fMapped->At(ihit);
        if (!hit)
            continue;
        iDet = hit->GetDetector();
        iCh = hit->GetPmt();
        iTf = hit->GetTimeFine();
        iTc = hit->GetTimeCoarse();

        if ((iDet < 1) || (iDet > fTcalPar->GetNumDetectors()))
        {
            LOG(info) << "R3BSofTofWMapped2Tcal::Exec() : In SofTofWMappedData, iDet = " << iDet
                      << "is out of range, item skipped ";
            continue;
        }
        if ((iCh < 1) || (iCh > fTcalPar->GetNumChannels()))
        {
            LOG(info) << "R3BSofTofWMapped2Tcal::Exec() : In SofTofWMappedData, iCh = " << iCh
                      << "is out of range, item skipped ";
            continue;
        }
        tns = CalculateTimeNs(iDet, iCh, iTf, iTc);
        AddTCalData(iDet, iCh, tns);
    }

    ++fNevent;
    return;
}
// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWMapped2Tcal::Reset()
{
    LOG(debug) << "Clearing TcalData Structure";
    if (fTcal)
        fTcal->Clear();
}

// -----   Public method Finish   -----------------------------------------------
void R3BSofTofWMapped2Tcal::Finish() {}

// -----   Private method to calculate the time in ns  --------------------------
Double_t R3BSofTofWMapped2Tcal::CalculateTimeNs(UShort_t iDet, UShort_t iCh, UInt_t iTf, UInt_t iTc)
{
    UInt_t rank = iTf + fTcalPar->GetNumTcalParsPerSignal() * ((iDet - 1) * fTcalPar->GetNumChannels() + (iCh - 1));
    Double_t iPar = (Double_t)fTcalPar->GetSignalTcalParams(rank);
    Double_t iClock = (Double_t)fTcalPar->GetClockOffset((iDet - 1) * fTcalPar->GetNumChannels() + (iCh - 1));
    Double_t r = (Double_t)rand.Rndm() - 0.5;
    // Double_t iTf_ns;
    Double_t iTc_ns = ((Double_t)iTc - iClock) * 5.;
    /*
        if (r < 0)
        {
            Double_t iParPrev = fTcalPar->GetSignalTcalParams(rank - 1);
            iTf_ns = iPar + r * (iPar - iParPrev);
        }
        else
        {
            Double_t iParNext = fTcalPar->GetSignalTcalParams(rank + 1);
            iTf_ns = iPar + r * (iParNext - iPar);
        }
        return (iTc_ns - iTf_ns);*/
    return (iTc_ns - iPar);
}

// -----   Private method AddTCalData  --------------------------------------------
R3BSofTofWTcalData* R3BSofTofWMapped2Tcal::AddTCalData(UShort_t detector, UShort_t pmt, Double_t t)
{
    // It fills the R3BSofTofWTcalData
    TClonesArray& clref = *fTcal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWTcalData(detector, pmt, t);
}

ClassImp(R3BSofTofWMapped2Tcal)
