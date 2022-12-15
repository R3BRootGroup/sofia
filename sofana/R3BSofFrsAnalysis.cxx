// -----------------------------------------------------------------
// -----                                                       -----
// -----                R3BSofFrsAnalysis                      -----
// -----        Created 23/01/20  by J.L. Rodriguez-Sanchez    -----
// -----        Revised 07/08/20  by R. Taniuchi               -----
// -----------------------------------------------------------------

#include "R3BSofFrsAnalysis.h"
Double_t const c = 29.9792458;   // Light velocity

// R3BSofFrsAnalysis: Default Constructor --------------------------
R3BSofFrsAnalysis::R3BSofFrsAnalysis()
    : FairTask("R3B-Sof Analysis for FRS", 1)
    , fFrs_Par(NULL)
    //, fMwpcHitDataCA(NULL)
    //, fSciHitDataCA(NULL)
    , fSingleTcalItemsSci(NULL)
    , fMusicHitDataCA(NULL)
    , fFrsDataCA(NULL)
    , fOnline(kFALSE)
    , fBetaCorr(kTRUE)
    , fNbSci(4)
    , fNbTof(3)
    , fIdS2(2)
    , fIdS8(3)
    , fIdCave(4)
{}

// R3BSofFrsAnalysisPar: Standard Constructor --------------------------
R3BSofFrsAnalysis::R3BSofFrsAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fFrs_Par(NULL)
    //, fMwpcHitDataCA(NULL)
    //, fSciHitDataCA(NULL)
    , fSingleTcalItemsSci(NULL)
    , fMusicHitDataCA(NULL)
    , fFrsDataCA(NULL)
    , fOnline(kFALSE)
    , fBetaCorr(kTRUE)
    , fNbSci(4)
    , fNbTof(3)
    , fIdS2(2)
    , fIdS8(3)
    , fIdCave(4)
{}

// Virtual R3BSofFrsAnalysis: Destructor
R3BSofFrsAnalysis::~R3BSofFrsAnalysis()
{
    R3BLOG(info, "R3BSofFrsAnalysis: Delete instance");
    // Kept Mwpc and SciHit can be used for later analysis if needed
    /*if (fMwpcHitDataCA)
    {
        delete fMwpcHitDataCA;
    }
    if (fSciHitDataCA)
    {
        delete fSciHitDataCA;
    }
    */
    if (fSingleTcalItemsSci) {
        delete fSingleTcalItemsSci;
    }
    if (fMusicHitDataCA) {
        delete fMusicHitDataCA;
    }
    if (fFrsDataCA) {
        delete fFrsDataCA;
    }
}

void R3BSofFrsAnalysis::SetParContainers()
{
    R3BLOG(info, "R3BSofFrsAnalysis::SetParContainers()");
    // Parameter Container
    // Reading soffrsAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb) {
        R3BLOG(error, "FairRuntimeDb not opened!");
    }

    fFrs_Par = (R3BSofFrsAnaPar*)rtdb->getContainer("soffrsAnaPar");
    if (!fFrs_Par) {
        R3BLOG(error, "R3BSofFrsAnalysisPar:: Couldn't get handle on soffrsAnaPar container");
    } else {
        R3BLOG(info, "R3BSofFrsAnalysisPar:: soffrsAnaPar container open");
    }

    fCal_Par = (R3BMusicHitPar*)rtdb->getContainer("musicHitPar");
    if (!fCal_Par) {
        R3BLOG(error, "R3BSofFrsAnalysisPar::Init() Couldn't get handle on musicHitPar container");
    } else {
        R3BLOG(info, "R3BSofFrsAnalysisPar:: musicHitPar container open");
    }
}

void R3BSofFrsAnalysis::SetParameter()
{
    //--- Parameter Container ---
    fBrho0 = fFrs_Par->GetBrho();
    fNbTof = fFrs_Par->GetNumTof();
    fStaId = new UChar_t[fNbTof];
    fStoId = new UChar_t[fNbTof];
    fPathLength = new Double_t[fNbTof];
    fTofOffset = new Double_t[fNbTof];
    fUseS2x = new Int_t[fNbTof];
    for (Int_t i = 0; i < fNbTof; i++) {
        fStaId[i] = fFrs_Par->GetStaSciId(i);
        fStoId[i] = fFrs_Par->GetStoSciId(i);
        fPathLength[i] = fFrs_Par->GetPathLength(i);
        fTofOffset[i] = fFrs_Par->GetTofOffset(i);
        fUseS2x[i] = fFrs_Par->GetUseS2x(i);
    }
    fS2SciCoef0 = fFrs_Par->GetS2PosOffset();
    fS2SciCoef1 = fFrs_Par->GetS2PosCoef();
    fNumBrhoCorrPar = fFrs_Par->GetNumBrhoCorrPar();
    fBrhoCorrPar = new Float_t[fNumBrhoCorrPar];
    for (Int_t i = 0; i < fNumBrhoCorrPar; i++) {
        fBrhoCorrPar[i] = fFrs_Par->GetBrhoCorrPar(i);
    }
    //--- Parameter Container ---
    fNumMusicParams = fCal_Par->GetNumParZFit();   // Number of Parameters
    R3BLOG(info, "R3BSofFrsAnalysisPar:: R3BMusicCal2Hit: Nb parameters for charge-Z: " << (Int_t)fNumMusicParams);
    CalZParams = new TArrayF();
    CalZParams->Set(fNumMusicParams);
    CalZParams = fCal_Par->GetZHitPar();   // Array with the Cal parameters
    // Parameters detector
    if (fNumMusicParams == 2) {
        fZ0 = CalZParams->GetAt(0);
        fZ1 = CalZParams->GetAt(1);
    } else if (fNumMusicParams == 3) {
        fZ0 = CalZParams->GetAt(0);
        fZ1 = CalZParams->GetAt(1);
        fZ2 = CalZParams->GetAt(2);
    } else
        R3BLOG(info,
               "R3BSofFrsAnalysisPar:: R3BMusicCal2Hit parameters for charge-Z cannot be used here, number of "
               "parameters: "
                   << fNumMusicParams);
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFrsAnalysis::Init()
{
    R3BLOG(info, "R3BSofFrsAnalysis: Init FRS analysis from S2 to Cave-C");

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager) {
        return kFATAL;
    }
    /*
    fMwpcHitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0HitData");
    if (!fMwpcHitDataCA)
    {
        return kFATAL;
    }
    */
    fMusicHitDataCA = (TClonesArray*)rootManager->GetObject("MusicHitData");
    if (!fMusicHitDataCA) {
        return kFATAL;
    }
    /*
    fSciHitDataCA = (TClonesArray*)rootManager->GetObject("SofSciHitData");
    if (!fSciHitDataCA)
    {
        return kFATAL;
    }
    */
    fSingleTcalItemsSci = (TClonesArray*)rootManager->GetObject("SofSciSingleTcalData");
    if (!fSingleTcalItemsSci) {
        return kFATAL;
    }

    // OUTPUT DATA
    fFrsDataCA = new TClonesArray("R3BFrsData", 5);
    if (!fOnline) {
        rootManager->Register("FrsData", "Analysis FRS", fFrsDataCA, kTRUE);
    } else {
        rootManager->Register("FrsData", "Analysis FRS", fFrsDataCA, kFALSE);
    }
    // ReInit();
    SetParameter();

    xpos = new Double_t[fNbSci];

    R3BLOG(info, "R3BSofFrsAnalysis::Init() done");
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofFrsAnalysis::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofFrsAnalysis::Exec(Option_t* option)
{
    Int_t nHitMusic = fMusicHitDataCA->GetEntries();
    Int_t nHitSci = fSingleTcalItemsSci->GetEntries();
    // Int_t nHitMwpc = fMwpcHitDataCA->GetEntries();
    // LOG(debug) << nHitMusic << " " << nHitSci; //<< " " << nHitMwpc ;
    if (nHitSci < 1 || nHitMusic < 1)
        return;

    // --- -------------- --- //
    // --- MUSIC Hit data --- //
    // --- -------------- --- //
    R3BMusicHitData* hitmusic;
    for (Int_t ihit = 0; ihit < nHitMusic; ihit++) {
        // In case the MusicHitData container has several "realistic" values,
        // it's not possible to distinguish which is the "correct" event. Thus skipping events having several hits.
        if (MusicE > 0)
            return;
        hitmusic = (R3BMusicHitData*)fMusicHitDataCA->At(ihit);
        if (!hitmusic)
            continue;
        MusicE = hitmusic->GetEave();
    }
    // LOG(debug) << nHitMusic << " " << nHitSci << " " << MusicE;
    if (!(MusicE > 0))
        return;

    // --- ------------------------------ --- //
    // --- loop over sci single tcal data --- //
    // --- ------------------------------ --- //
    Int_t d = -1;
    for (Int_t ihit = 0; ihit < nHitSci; ihit++) {
        R3BSofSciSingleTcalData* hitsingletcal = (R3BSofSciSingleTcalData*)fSingleTcalItemsSci->At(ihit);
        if (!hitsingletcal)
            continue;
        d = hitsingletcal->GetDetector() - 1;
        if (d > -1 && d < fNbSci) {
            if (d != fIdS2 - 1) {
                xpos[d] = hitsingletcal->GetRawPosNs();
            } else {
                xpos[d] = hitsingletcal->GetRawPosNs() * fS2SciCoef1 + fS2SciCoef0;
            }
        }
        if (fIdS2 > 0) {
            if (d == fIdS8 - 1 && fIdS8 > 0)
                Tof_wTref_S2_S8 = hitsingletcal->GetRawTofNs_FromS2();
            if (d == fIdCave - 1)
                Tof_wTref_S2_Cave = hitsingletcal->GetRawTofNs_FromS2();
        }
        if (d == fIdCave - 1 && fIdS8 > 0)
            Tof_wTref_S8_Cave = hitsingletcal->GetRawTofNs_FromS8();
    }   // Loop over sci hits
    //
    std::vector<Double_t> beta;
    Int_t i_s2cave = -1, i_s8cave = -1, i_s2s8 = -1;
    for (Int_t i = 0; i < fNbTof; i++) {
        Double_t tof = NAN;
        if (fStaId[i] == fIdS2 && fStoId[i] == fIdCave) {
            tof = Tof_wTref_S2_Cave;
            i_s2cave = i;
        } else if (fStaId[i] == fIdS2 && fStoId[i] == fIdS8) {
            tof = Tof_wTref_S2_S8;
            i_s2s8 = i;
        } else if (fStaId[i] == fIdS8 && fStoId[i] == fIdCave) {
            tof = Tof_wTref_S8_Cave;
            i_s8cave = i;
        }
        if (isnan(tof) || tof < 0) {
            beta.push_back(NAN);
        } else {
            beta.push_back(fPathLength[i] / (tof + fTofOffset[i]));
        }
    }
    // Velocity correlation conditions
    if (fBetaCorr && i_s2cave >= 0 && i_s8cave >= 0 && beta.at(i_s2cave) > 0. && beta.at(i_s8cave) > 0.
        && TMath::Abs(beta.at(i_s2cave) - beta.at(i_s8cave)) < 0.01) {
        beta.push_back(beta.at(i_s2cave));
    } else {
        beta.push_back(NAN);
    }
    //
    // Under modification
    if (beta.size() == 0) {
        return;
    }
    const int betaindex = beta.size() - 1;
    int index_tof[4] = {betaindex, i_s2cave, i_s8cave, i_s2s8};
    for (Int_t ii = 0; ii < TMath::Min(4, betaindex + 1); ii++)   // starting from the good one
    {
        Int_t i = 0;
        if (index_tof[ii] >= 0) {
            i = index_tof[ii];
        } else {
            continue;
        }
        if (!(beta.at(i) > 0.)) {
            if (ii == 0)
                AddData(0, 0);
            continue;
        }
        Double_t gamma = 1. / (TMath::Sqrt(1. - beta.at(i) * beta.at(i)));
        Double_t correction = 1.;
        if (fUseS2x[i] != 0 && fNumBrhoCorrPar > 0) {
            for (Int_t j = 0; j < fNumBrhoCorrPar; j++)
                correction -= pow(xpos[fIdS2 - 1], j) * fBrhoCorrPar[j];
        }
        Double_t brho = fBrho0 * correction;
        Double_t aoq = brho / (3.10716 * gamma * beta.at(i));
        MusicZ = fZ0 + fZ1 * TMath::Sqrt(MusicE) * beta.at(i) + fZ2 * MusicE * beta.at(i) * beta.at(i);
        //
        if (ii == 0) {
            AddData(0, 0, MusicZ, aoq, beta.at(i), brho, xpos[fIdS2 - 1], xpos[fIdCave - 1]);
            hitmusic->SetZcharge(MusicZ);   // Set velocity corrected Z
        } else {
            AddData(fStaId[i], fStoId[i], MusicZ, aoq, beta.at(i), brho, xpos[fIdS2 - 1], xpos[fIdCave - 1]);
        }
    }
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofFrsAnalysis::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofFrsAnalysis::Reset()
{
    LOG(debug) << "FrsAnalysis Reset()";
    if (fSingleTcalItemsSci) {
        fSingleTcalItemsSci->Clear();
    }
    if (fMusicHitDataCA) {
        fMusicHitDataCA->Clear();
    }
    LOG(debug) << "Clearing FrsData Structure";
    if (fFrsDataCA)
        fFrsDataCA->Clear();

    for (Int_t i = 0; i < fNbSci; i++)
        xpos[i] = NAN;
    Tof_wTref_S2_Cave = NAN, Tof_wTref_S2_S8 = NAN, Tof_wTref_S8_Cave = NAN;
    MusicZ = NAN;
    MusicE = NAN;
}

// -----   Private method AddData  --------------------------------------------
R3BFrsData* R3BSofFrsAnalysis::AddData(Int_t StaId,
                                       Int_t StoId,
                                       Double_t z,
                                       Double_t aq,
                                       Double_t beta,
                                       Double_t brho,
                                       Double_t xs2,
                                       Double_t xc)
{
    // It fills the R3BFrsData
    TClonesArray& clref = *fFrsDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BFrsData(StaId, StoId, z, aq, beta, brho, xs2, xc);
}

ClassImp(R3BSofFrsAnalysis);
