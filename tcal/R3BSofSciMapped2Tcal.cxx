#include "R3BSofSciMapped2Tcal.h"
#include <iomanip>

// --- Default Constructor
R3BSofSciMapped2Tcal::R3BSofSciMapped2Tcal()
    : FairTask("R3BSofSciMapped2Tcal", 1)
    , fNevent(0)
    , fTcal(NULL)
    , fMapped(NULL)
    , fTcalPar(NULL)
    , fOnline(kFALSE)
{
}

// --- Standard Constructor
R3BSofSciMapped2Tcal::R3BSofSciMapped2Tcal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNevent(0)
    , fTcal(NULL)
    , fMapped(NULL)
    , fTcalPar(NULL)
    , fOnline(kFALSE)
{
}

// --- Destructor
R3BSofSciMapped2Tcal::~R3BSofSciMapped2Tcal()
{
    LOG(INFO) << "R3BSofSciMapped2Tcal: Delete instance";
    if (fMapped)
    {
        delete fMapped;
    }
    if (fTcal)
    {
        delete fTcal;
    }
}

// --- Parameter container : reading SofSciTcalPar from FairRuntimeDb
void R3BSofSciMapped2Tcal::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fTcalPar = (R3BSofTcalPar*)rtdb->getContainer("SofSciTcalPar");
    if (!fTcalPar)
    {
        LOG(ERROR) << "R3BSofSciMapped2Tcal::SetParContainers() : Could not get access to SofSciTcalPar-Container.";
        return;
    }
    else
    {
        LOG(INFO) << "R3BSofSciMapped2Tcal::SetParContainers() : SofSciTcalPar-Container found with "
                  << fTcalPar->GetNumDetectors() << " detectors and " << fTcalPar->GetNumChannels() << " channels";
    }
}

InitStatus R3BSofSciMapped2Tcal::Init()
{

    LOG(INFO) << "R3BSofSciMapped2Tcal: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        LOG(ERROR) << "R3BSofSciMapped2Tcal::Init() Couldn't instance the FairRootManager";
        return kFATAL;
    }

    // --- ----------------- --- //
    // --- INPUT MAPPED DATA --- //
    // --- ----------------- --- //
    fMapped = (TClonesArray*)rm->GetObject("SofSciMappedData");
    if (!fMapped)
    {
        LOG(ERROR) << "R3BSofSciMapped2Tcal::Init() Couldn't get handle on SofSciMappedData container";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciMapped2Tcal::Init() SofSciMappedData items found";

    // --- ---------------- --- //
    // --- OUTPUT TCAL DATA --- //
    // --- ---------------- --- //
    fTcal = new TClonesArray("R3BSofSciTcalData", 25);
    if (!fOnline)
    {
        rm->Register("SofSciTcalData", "SofSci", fTcal, kTRUE);
    }
    else
    {
        rm->Register("SofSciTcalData", "SofSci", fTcal, kFALSE);
    }

    LOG(INFO) << "R3BSofSciMapped2Tcal::Init() output R3BSofSciTcalData ";

    // --- -------------------------- --- //
    // --- CHECK THE TCALPAR VALIDITY --- //
    // --- -------------------------- --- //
    if (fTcalPar->GetNumDetectors() == 0)
    {
        LOG(ERROR) << "R3BSofSciMapped2Tcal::Init() : There are no Tcal parameters for SofSci";
        return kFATAL;
    }
    else
    {
        LOG(INFO) << "R3BSofSciMapped2Tcal::Init(): Number of SofSci =" << fTcalPar->GetNumDetectors();
        LOG(INFO) << "R3BSofSciMapped2Tcal::Init(): Number of Channel per SofSci =" << fTcalPar->GetNumChannels();
    }
    return kSUCCESS;
}

InitStatus R3BSofSciMapped2Tcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BSofSciMapped2Tcal::Exec(Option_t* option)
{
    UInt_t det;
    UInt_t ch;
    UInt_t tf;
    UInt_t tc;
    Double_t tns;

    // Reset entries in output arrays, local arrays
    Reset();

    // Loop over the entries of the Mapped TClonesArray
    Int_t nHitsPerEvent_SofSci = fMapped->GetEntries();
    for (Int_t ihit = 0; ihit < nHitsPerEvent_SofSci; ihit++)
    {
        R3BSofSciMappedData* hit = (R3BSofSciMappedData*)fMapped->At(ihit);
        if (!hit)
            continue;
        det = hit->GetDetector();
        ch = hit->GetPmt();
        tf = hit->GetTimeFine();
        tc = hit->GetTimeCoarse();
        if ((det < 1) || (det > fTcalPar->GetNumDetectors()))
        {
            LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iDet = " << det
                      << "is out of range, item skipped ";
            continue;
        }
        if ((ch < 1) || (ch > fTcalPar->GetNumChannels()))
        {
            LOG(INFO) << "R3BSofSciMapped2Tcal::Exec() : In SofSciMappedData, iCh = " << ch
                      << "is out of range, item skipped ";
            continue;
        }
        tns = CalculateTimeNs(det, ch, tf, tc);
        AddTcalData(det, ch, tns, tc);
    }

    if (nHitsPerEvent_SofSci != fTcal->GetEntries())
        LOG(WARNING) << "R3BSofSciMapped2Tcal::Exec() mismatch between TClonesArray entries ";

    ++fNevent;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciMapped2Tcal::Reset()
{
    LOG(DEBUG) << "Clearing TcalData Structure";
    if (fTcal)
        fTcal->Clear();
}

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciMapped2Tcal::Finish() {}

// -----   Private method AddCalData  --------------------------------------------
R3BSofSciTcalData* R3BSofSciMapped2Tcal::AddTcalData(Int_t det, Int_t ch, Double_t tns, UInt_t clock)
{
    // It fills the R3BSofSciTcalData
    TClonesArray& clref = *fTcal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofSciTcalData(det, ch, tns, clock);
}

Double_t R3BSofSciMapped2Tcal::CalculateTimeNs(UShort_t iDet, UShort_t iCh, UInt_t iTf, UInt_t iTc)
{
    UInt_t rank = iTf + fTcalPar->GetNumTcalParsPerSignal() * ((iDet - 1) * fTcalPar->GetNumChannels() + (iCh - 1));
    Double_t iPar = (Double_t)fTcalPar->GetSignalTcalParams(rank);
    Double_t iDeltaClock = (Double_t)fTcalPar->GetClockOffset((iDet - 1) * fTcalPar->GetNumChannels() + (iCh - 1));
    Double_t r = (Double_t)rand.Rndm() - 0.5;
    Double_t iTf_ns;
    Double_t iTc_ns = ((Double_t)iTc - iDeltaClock) * 5.;
    //  std::cout << "R3BSofSciMapped2Tcal::CalculateTimeNs : iDet=" << iDet << ", iCh=" << iCh << ", iTf=" << iTf << ",
    //  rank=" << rank  << std::endl;

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

    // std::cout << "Tf_ns=" << iTf_ns << ", iTc_ns=" << iTc_ns << " : TimeNs = " << 5.*iTc_ns - iTf_ns << std::endl;
    return (iTc_ns - iTf_ns);
}

ClassImp(R3BSofSciMapped2Tcal)
