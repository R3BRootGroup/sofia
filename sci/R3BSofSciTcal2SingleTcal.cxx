// REMINDER : RawPos = TrawRIGHT - TrawLEFT
//                   = 5*(CCr-CCl) + (FTl-FTr)
//                   --> x is increasing from RIGHT to LEFT
#include "R3BSofSciTcal2SingleTcal.h"
#include "R3BSofSciTcalData.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

R3BSofSciTcal2SingleTcal::R3BSofSciTcal2SingleTcal()
    : FairTask("R3BSofSciTcal2SingleTcal", 1)
    , fTcal(NULL)
    , fRawPosPar(NULL)
    , fRawTofPar(NULL)
    , fSingleTcal(NULL)
    , fNumSingleTcal(0)
    , fOnline(kFALSE)
    , fNevent(0)
{
}

R3BSofSciTcal2SingleTcal::~R3BSofSciTcal2SingleTcal()
{
    if (fTcal)
    {
        delete fTcal;
    }
    if (fSingleTcal)
    {
        delete fSingleTcal;
    }
}

void R3BSofSciTcal2SingleTcal::SetParContainers()
{
    fRawPosPar = (R3BSofSciRawPosPar*)FairRuntimeDb::instance()->getContainer("SofSciRawPosPar");
    if (!fRawPosPar)
    {
        LOG(ERROR)
            << "R3BSofSciTcal2SingleTcal::SetParContainers() : Could not get access to SofSciRawPosPar-Container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofSciTcal2SingleTcal::SetParContainers() : SofSciRawPosPar-Container found with "
                  << fRawPosPar->GetNumSignals() << " signals";

    fRawTofPar = (R3BSofSciRawTofPar*)FairRuntimeDb::instance()->getContainer("SofSciRawTofPar");
    if (!fRawTofPar)
    {
        LOG(ERROR)
            << "R3BSofSciTcal2SingleTcal::SetParContainers() : Could not get access to SofSciRawTofPar-Container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofSciTcal2SingleTcal::SetParContainers() : SofSciRawTofPar-Container found with "
                  << fRawTofPar->GetNumSignals() << " signals";
}

InitStatus R3BSofSciTcal2SingleTcal::Init()
{

    LOG(INFO) << "R3BSofSciTcal2SingleTcal::Init()";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        LOG(ERROR) << "R3BSofSciTcal2SingleTcal::Couldn't instance the FairRootManager";
        return kFATAL;
    }

    // --- --------------- --- //
    // --- INPUT TCAL DATA --- //
    // --- --------------- --- //

    // scintillator at S2 and/or S8 and/or cave C
    fTcal = (TClonesArray*)rm->GetObject("SofSciTcalData");
    if (!fTcal)
    {
        LOG(ERROR) << "R3BSofSciTcal2SingleTcal::Couldn't get handle on SofSciTcalData container";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciTcal2SingleTcal::SofSciTcalData items found";

    // --- ----------------------- --- //
    // --- OUTPUT SINGLE TCAL DATA --- //
    // --- ----------------------- --- //

    // Register output array in tree
    fSingleTcal = new TClonesArray("R3BSofSciSingleTcalData", 5);
    if (!fOnline)
    {
        rm->Register("SofSciSingleTcalData", "SofSci", fSingleTcal, kTRUE);
    }
    else
    {
        rm->Register("SofSciSingleTcalData", "SofSci", fSingleTcal, kFALSE);
    }

    LOG(INFO) << "R3BSofSciTcal2SingleTcal::SofSciSingleTcalData items created";

    // --- -------------------------- --- //
    // --- CHECK THE TCALPAR VALIDITY --- //
    // --- -------------------------- --- //
    if (fRawPosPar->GetNumSignals() == 0)
    {
        LOG(ERROR) << " There are no RawPosPar Tcal parameters for SofSci";
        return kFATAL;
    }
    else
    {
        LOG(INFO) << "  R3BSofSciTcal2SingleTcal::Init() : fRawPosPar: fNumSignals=" << fRawPosPar->GetNumSignals();
    }

    if (fRawTofPar->GetNumDets() == 0)
    { // ATTENTION IN CASE OF PRIMARY BEAM, fRawTofPar->GetNumSignals() could be == 0
        LOG(ERROR) << " There are no SofSci detectors declared";
        return kFATAL;
    }
    else
    {
        LOG(INFO) << "  R3BSofSciTcal2SingleTcal::Init() : fRawTofPar: fNumSignals=" << fRawTofPar->GetNumSignals();
    }

    if (fRawTofPar->GetDetIdCaveC() == 0)
    {
        LOG(ERROR) << "id number of the cave C SofSci should be >0 (1-based)";
        return kFATAL;
    }

    if (fRawTofPar->GetDetIdCaveC() != 1 && fRawTofPar->GetNumDets() == 1)
    {
        LOG(ERROR) << "Number of SofSci declared to 1 (primary beam), but idCave = " << fRawTofPar->GetDetIdCaveC()
                   << " instead of 1";
        return kFATAL;
    }

    if (fRawTofPar->GetDetIdCaveC() != fRawTofPar->GetNumDets() && fRawTofPar->GetNumDets() > 1)
    {
        LOG(ERROR) << "Number of SofSci declared >1 (secondary beam), but idCave = " << fRawTofPar->GetDetIdCaveC()
                   << " instead of " << fRawTofPar->GetNumDets();
        return kFATAL;
    }

    if (fRawTofPar->GetDetIdS2() == 0 && fRawTofPar->GetNumDets() > 1)
    {
        LOG(ERROR) << "Number of SofSci declared >1 (secondary beam), but idS2 = " << fRawTofPar->GetDetIdS2()
                   << " but should be [1;" << fRawTofPar->GetNumDets() - 1 << "]";
        return kFATAL;
    }

    if (fRawPosPar->GetNumDets() > 1)
    {
        if (fRawPosPar->GetNumDets() != fRawTofPar->GetNumDets())
        {
            LOG(ERROR) << "Mismatch between the number of SofSci detectors in RawPosPar (" << fRawPosPar->GetNumDets()
                       << ") and RawTofPar )" << fRawTofPar->GetNumDets() << ")";
            return kFATAL;
        }

        if (fRawPosPar->GetNumPmts() != fRawTofPar->GetNumChannels())
        {
            LOG(ERROR) << "Mismatch between the number of SofSci channels in RawPosPar and RawTofPar";
            return kFATAL;
        }
    }

    LOG(INFO) << "R3BSofSciTcal2SingleTcal::Init DONE";

    return kSUCCESS;
}

InitStatus R3BSofSciTcal2SingleTcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BSofSciTcal2SingleTcal::Exec(Option_t* option)
{

    UShort_t nDets = fRawPosPar->GetNumDets();
    UShort_t nChs = fRawPosPar->GetNumPmts();
    UShort_t idCaveC = nDets;
    UShort_t iDet; // 0-based
    UShort_t iCh;  // 0-based
    Double_t iTraw[nDets * nChs][32];
    UShort_t mult[nDets * nChs];
    UShort_t mult_max = 0;
    UInt_t maskR[nDets]; // if mult_max>=32, doesn't work
    UInt_t maskL[nDets]; // if mult_max>=32, doesn't work

    for (UShort_t i = 0; i < nDets * nChs; i++)
        mult[i] = 0;

    Int_t nHitsPerEvent_SofSci = fTcal->GetEntries();
    for (int ihit = 0; ihit < nHitsPerEvent_SofSci; ihit++)
    {
        R3BSofSciTcalData* hit = (R3BSofSciTcalData*)fTcal->At(ihit);
        if (!hit)
            continue;
        iDet = hit->GetDetector() - 1;
        iCh = hit->GetPmt() - 1;
        if (mult_max > 32)
            continue; // if multiplicity in a Pmt is higher than 32, this code cannot handle it
        iTraw[iDet * nChs + iCh][mult[iDet * nChs + iCh]] = hit->GetRawTimeNs();
        mult[iDet * nChs + iCh]++;
        if (mult[iDet * nChs + iCh] > mult_max)
            mult_max = mult[iDet * nChs + iCh];
    } // end of loop over the TClonesArray of Tcal data

    // It makes no sense to continue if there is no Left and Right signal on the SofSci at cave C
    if ((nHitsPerEvent_SofSci > 0) && (mult[(idCaveC - 1) * nChs] > 0) && (mult[(idCaveC - 1) * nChs + 1] > 0))
    {
        Double_t iRawPos = -100000., iRawTime = -100000.;
        UShort_t mult_selectHits[nDets];
        Int_t selectLeftHit[nDets];
        Int_t selectRightHit[nDets];
        for (UShort_t det = 0; det < nDets; det++)
        {
            selectLeftHit[det] = -1;
            selectRightHit[det] = -1;
            mult_selectHits[det] = 0;
        }

        // --- ------------------------------------------ --- //
        // --- CASE 1: PRIMARY BEAM = CAVE C ONLY, NO FRS --- //
        // --- ------------------------------------------ --- //
        if (nDets == 1)
        {
            maskL[0] = 0x0;
            maskR[0] = 0x0;
            for (UShort_t multR = 0; multR < mult[0]; multR++)
            {
                for (UShort_t multL = 0; multL < mult[1]; multL++)
                {
                    // RawPos = TrawRIGHT - TrawLEFT corresponds to x increasing from RIGHT to LEFT
                    iRawPos = iTraw[0][multR] - iTraw[1][multL];
                    // if the raw position is outside the range: continue
                    if (iRawPos < fRawPosPar->GetSignalTcalParams(0))
                        continue;
                    if (iRawPos > fRawPosPar->GetSignalTcalParams(1))
                        continue;
                    // if the left or right hit has already been used, continue
                    if ((((maskR[0] >> multR) & (0x1)) == 1) || (((maskL[0] > multL) & (0x1)) == 1))
                        continue;
                    iRawTime = 0.5 * (iTraw[0][multL] + iTraw[1][multR]);
                    // tag which hit is used
                    maskR[0] |= (0x1) << multR;
                    maskL[0] |= (0x1) << multL;
                    AddSingleTcalData(1, iRawTime, iRawPos, -100000., -100000.);
                } // end of loop over the hits of the left PMTs
            }     // end of loop over the hits of the right PMTs

        } // --- end of CAVE C only --- //

        // --- ----------------------------------------------- --- //
        // --- CASE 2: SECONDARY BEAM = SofSci at CAVE C + FRS --- //
        // --- ----------------------------------------------- --- //
        // SELECTION OF THE MULTIPLICITY LOOKING AT THE ToFraw VERSUS Cave C SCINTILLATOR
        // Since the multiplicity at Cave C is much lower than at S2 or S8
        // all the hit finder are done looking at the Tof from S2 or S8 to cave C

        else
        {
            UShort_t idS2 = fRawTofPar->GetDetIdS2(); // 1-based
            UShort_t idS8 = fRawTofPar->GetDetIdS8(); // 1-based if 0: no detector at S8
            Double_t iRawTime_dSta = -100000., iRawTime_dSto = -100000., iRawTime_S2 = -100000., iRawTime_S8 = -100000.;
            Double_t iRawTof = -100000., iRawTof_S2 = -100000., iRawTof_S8 = -100000.;
            Int_t dSto = idCaveC - 1; // Fix the CaveC SofSci as the stop detector

            // --- selection for the scintillators along FRS versus SofSci at Cave C --- //
            // --- only if a proper selection has been found at cave C               --- //
            // --- in this case : fill the SingleTcalItem for all detectors          --- //

            for (Int_t dSta = 0; dSta < nDets - 1; dSta++)
            {
                UShort_t multLstotmp = 0, multRstotmp = 0;
                for (UShort_t multRsto = 0; multRsto < mult[dSto * nChs]; multRsto++)
                {
                    for (UShort_t multLsto = 0; multLsto < mult[dSto * nChs + 1]; multLsto++)
                    {
                        // check the position in the stop detector
                        iRawPos = (iTraw[dSto * nChs][multRsto] - iTraw[dSto * nChs + 1][multLsto]);
                        if ((iRawPos < fRawPosPar->GetSignalTcalParams(2 * dSto)) ||
                            (iRawPos > fRawPosPar->GetSignalTcalParams(2 * dSto + 1)))
                            continue;
                        for (UShort_t multRsta = 0; multRsta < mult[dSta * nChs]; multRsta++)
                        {
                            for (UShort_t multLsta = 0; multLsta < mult[dSta * nChs + 1]; multLsta++)
                            {
                                // RawPos = TrawRIGHT - TrawLEFT corresponds to x increasing from RIGHT to LEFT
                                iRawPos = (iTraw[dSta * nChs][multRsta] - iTraw[dSta * nChs + 1][multLsta]);
                                if ((iRawPos < fRawPosPar->GetSignalTcalParams(2 * dSta)) ||
                                    (iRawPos > fRawPosPar->GetSignalTcalParams(2 * dSta + 1)))
                                    continue;
                                iRawTime_dSta = 0.5 * (iTraw[dSta * nChs][multRsta] + iTraw[dSta * nChs + 1][multLsta]);
                                iRawTime_dSto = 0.5 * (iTraw[dSto * nChs][multRsto] + iTraw[dSto * nChs + 1][multLsto]);
                                iRawTof = iRawTime_dSto - iRawTime_dSta + iTraw[dSta * nChs + 2][0] -
                                          iTraw[dSto * nChs + 2][0];
                                if ((fRawTofPar->GetSignalRawTofParams(2 * dSta) <= iRawTof) &&
                                    (iRawTof <= fRawTofPar->GetSignalRawTofParams(2 * dSta + 1)))
                                {
                                    selectLeftHit[dSta] = multLsta;
                                    selectRightHit[dSta] = multRsta;
                                    multLstotmp = multLsto;
                                    multRstotmp = multRsto;
                                    mult_selectHits[dSta]++;
                                }
                            } // end of multLsta
                        }     // end of multRsta
                    }         // end of multLsto
                }             // end of multRsto
                if (mult_selectHits[dSta] > 0 &&
                    (mult_selectHits[dSto] == 0 || mult_selectHits[dSta] < mult_selectHits[dSto]))
                {
                    // Check if this TOF is better than previous TOF conditions
                    selectLeftHit[dSto] = multLstotmp;
                    selectRightHit[dSto] = multRstotmp;
                    mult_selectHits[dSto] = mult_selectHits[dSta];
                }
            } // end of calculation of all Tof and Pos = end of for(dSta)

            // Fill the TClonesArray of R3BSofSciSingleTcal
            if (idS2 > 0)
                if (mult_selectHits[idS2 - 1] == 1)
                    iRawTime_S2 = 0.5 * (iTraw[(idS2 - 1) * nChs][selectRightHit[idS2 - 1]] +
                                         iTraw[(idS2 - 1) * nChs + 1][selectLeftHit[idS2 - 1]]);
            if (idS8 > 0)
                if (mult_selectHits[idS8 - 1] == 1)
                    iRawTime_S8 = 0.5 * (iTraw[(idS8 - 1) * nChs][selectRightHit[idS8 - 1]] +
                                         iTraw[(idS8 - 1) * nChs + 1][selectLeftHit[idS8 - 1]]);

            for (UShort_t d = 0; d < nDets; d++)
            {
                if (mult_selectHits[d] == 1)
                {
                    // RawPos = TrawRIGHT - TrawLEFT corresponds to x increasing from RIGHT to LEFT
                    iRawPos = iTraw[d * nChs][selectRightHit[d]] - iTraw[d * nChs + 1][selectLeftHit[d]];
                    iRawTime = 0.5 * (iTraw[d * nChs][selectRightHit[d]] + iTraw[d * nChs + 1][selectLeftHit[d]]);
                    iRawTof_S2 = -100000.;
                    if (iRawTime_S2 > 0)
                        iRawTof_S2 = iRawTime - iRawTime_S2 + iTraw[(idS2 - 1) * nChs + 2][0] - iTraw[d * nChs + 2][0];
                    ;
                    iRawTof_S8 = -100000.;
                    if (iRawTime_S8 > 0)
                        iRawTof_S8 = iRawTime - iRawTime_S8 + iTraw[(idS8 - 1) * nChs + 2][0] - iTraw[d * nChs + 2][0];
                    ;
                    AddSingleTcalData(d + 1, iRawTime, iRawPos, iRawTof_S2, iRawTof_S8);
                }
            } // end of if the first selection succeed

        } // end of CAVE C + FRS

    } // end of if nHitsPerEvent_SofSci>0 + data on LEFT and data RIGHT on SofSci at CAVE C

    ++fNevent;
}

void R3BSofSciTcal2SingleTcal::FinishEvent()
{
    fSingleTcal->Clear();
    fNumSingleTcal = 0;
}

void R3BSofSciTcal2SingleTcal::FinishTask() {}

// -----   Private method AddSingleTcalData  --------------------------------------------
R3BSofSciSingleTcalData* R3BSofSciTcal2SingleTcal::AddSingleTcalData(UShort_t iDet,
                                                                     Double_t traw,
                                                                     Double_t posraw,
                                                                     Double_t tofrawS2,
                                                                     Double_t tofrawS8)
{
    // It fills the R3BSofSciSingleTcalData
    TClonesArray& clref = *fSingleTcal;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofSciSingleTcalData(iDet, traw, posraw, tofrawS2, tofrawS8);
}

ClassImp(R3BSofSciTcal2SingleTcal)
