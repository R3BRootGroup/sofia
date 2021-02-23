// ------------------------------------------------------------
// -----                R3BSofSciVsMusicOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciVsMusicOnlineSpectra_H
#define R3BSofSciVsMusicOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads SCI data and plots online histograms
 */
class R3BSofSciVsMusicOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciVsMusicOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciVsMusicOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciVsMusicOnlineSpectra();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

    /**
     * Method for event loop implementation.
     * Is called by the framework every time a new event is read.
     * @param option an execution option.
     */
    virtual void Exec(Option_t* option);

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    /**
     * Method for finish of the task execution.
     * Is called by the framework after processing the event loop.
     */
    virtual void FinishTask();

    /**
     * Methods to clean histograms.
     */
    virtual void Reset_Histo();

    /** Virtual method Reset **/
    virtual void Reset();

    void SetNbDetectors(Int_t ndets) { fNbDetectors = ndets; }
    void SetNbChannels(Int_t nchs) { fNbChannels = nchs; }
    void SetIdS2(Int_t id) { fIdS2 = id; }
    void SetIdS8(Int_t id) { fIdS8 = id; }
    void SetCalTofS2min(Float_t tof, Int_t rank) { fCalTofS2min->AddAt(tof, rank); }
    void SetCalTofS2max(Float_t tof, Int_t rank) { fCalTofS2max->AddAt(tof, rank); }
    void SetCalTofS8min(Float_t tof, Int_t rank) { fCalTofS8min->AddAt(tof, rank); }
    void SetCalTofS8max(Float_t tof, Int_t rank) { fCalTofS8max->AddAt(tof, rank); }
    void SetBrho0(Float_t brho0) { fBrho0 = brho0; }
    void SetDispersionS2(Float_t dispersion_mm) { fDS2 = dispersion_mm; }
    void SetDispersionCC(Float_t dispersion_mm) { fDCC = dispersion_mm; }
    Int_t GetNbDetectors() { return fNbDetectors; }
    Int_t GetNbChannels() { return fNbChannels; }
    Int_t GetIdS2() { return fIdS2; }
    Int_t GetIdS8() { return fIdS8; }
    Float_t GetBrho0() { return fBrho0; }
    Float_t GetDispersionS2() { return fDS2; }
    Float_t GetDispersionCC() { return fDCC; }
    Float_t GetTofS2min(Int_t rank) { return fCalTofS2min->GetAt(rank); }
    Float_t GetTofS2max(Int_t rank) { return fCalTofS2max->GetAt(rank); }
    Float_t GetTofS8min(Int_t rank) { return fCalTofS8min->GetAt(rank); }
    Float_t GetTofS8max(Int_t rank) { return fCalTofS8max->GetAt(rank); }

  private:
    TClonesArray* fTcalSci;     /**< Array with SofSci Tcal items. */
    TClonesArray* fCalSci;      /**< Array with SofSci Cal items. */
    TClonesArray* fMusHitItems; /**< Array with MUSIC Hit items. */
    TClonesArray* fMusCalItems; /**< Array with MUSIC Cal items. */

    Int_t fNbDetectors;
    Int_t fNbChannels;
    Int_t fIdS2;
    Int_t fIdS8;
    Float_t fBrho0;
    Float_t fDS2;
    Float_t fDCC;
    TArrayF* fCalTofS2min;
    TArrayF* fCalTofS2max;
    TArrayF* fCalTofS8min;
    TArrayF* fCalTofS8max;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas** cMusicZvsCalPos; // [fNbDetectors];
    TCanvas* cMusicDTvsCalPos;
    TCanvas** cMusicZvsCalTofS2; // [fNbDetectors-fIdS2];
    TCanvas** cMusicZvsCalTofS8; // [fNbDetectors-fIdS8];
    TCanvas* cAqvsq;
    TCanvas* cAqvsq_mult1Tref;
    TCanvas* cEvsAoQ;
    TCanvas* cEcorrvsAoQ;
    TCanvas* cEvsBeta;
    TCanvas* cEcorr;
    TCanvas* cEcorrvsAoQall;

    // Histogram for correlation with R3B-Music
    TH2F** fh2_MusZvsCalPos; //[fNbDetectors];
    TH2F* fh2_MusDTvsCalPos;
    TH2F** fh2_MusZvsCalTofS2; //[fNbDetectors-fIdS2];
    TH2F** fh2_MusZvsCalTofS8; //[fNbDetectors-fIdS8];
    TH2F* fh2_Aqvsq;
    TH2F* fh2_Aqvsq_mult1Tref;
    TH2F** fh2_EvsAoQ;
    TH2F** fh2_EcorrvsAoQ;
    TH2F* fh2_ErawVsBeta;
    TH2F* fh2_EcorrBetaVsBeta;
    TH1F* fh1_Esum;
    TH1F* fh1_EcorrBeta;
    TH1F* fh1_EcorrBetaDT;
    TH2F* fh2_EcorrBetaVsDT;
    TH2F* fh2_EcorrBetaDTVsDT;
    TH2F* fh2_EcorrVsAoQ_all;

  public:
    ClassDef(R3BSofSciVsMusicOnlineSpectra, 1)
};

#endif
