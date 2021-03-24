// ------------------------------------------------------------
// -----                R3BSofSciVsTrimOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciVsTrimOnlineSpectra_H
#define R3BSofSciVsTrimOnlineSpectra_H

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
class R3BSofSciVsTrimOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciVsTrimOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciVsTrimOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciVsTrimOnlineSpectra();

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
    void SetTofS2min(Float_t tof, Int_t rank) { fTofS2min->AddAt(tof, rank); }
    void SetTofS2max(Float_t tof, Int_t rank) { fTofS2max->AddAt(tof, rank); }
    void SetEmin(Float_t emin, Int_t rank) { fEmin->AddAt(emin, rank); }
    void SetEmax(Float_t emax, Int_t rank) { fEmax->AddAt(emax, rank); }
    void SetBrho0(Float_t brho0) { fBrho0 = brho0; }
    void SetDispersionS2(Float_t dispersion_mm) { fDS2 = dispersion_mm; }
    void SetDispersionCC(Float_t dispersion_mm) { fDCC = dispersion_mm; }

    Int_t GetNbDetectors() { return fNbDetectors; }
    Int_t GetNbChannels() { return fNbChannels; }
    Int_t GetIdS2() { return fIdS2; }
    Float_t GetBrho0() { return fBrho0; }
    Float_t GetDispersionS2() { return fDS2; }
    Float_t GetDispersionCC() { return fDCC; }
    Float_t GetTofS2min(Int_t rank) { return fTofS2min->GetAt(rank); }
    Float_t GetTofS2max(Int_t rank) { return fTofS2max->GetAt(rank); }
    Float_t GetEmin(Int_t rank) { return fEmin->GetAt(rank); }
    Float_t GetEmax(Int_t rank) { return fEmax->GetAt(rank); }

  private:
    TClonesArray* fSciSTcal; // Array with SofSci Tcal items.
    TClonesArray* fSciCal;   // Array with SofSci Cal items.
    TClonesArray* fTrimCal;  // Array with Trim Cal items.
    TClonesArray* fTrimHit;  // Array with Trim Hit items.

    Int_t fNbDetectors;
    Int_t fNbChannels;
    Int_t fIdS2;
    Float_t fBrho0;
    Float_t fDS2;
    Float_t fDCC;
    TArrayF* fTofS2min;
    TArrayF* fTofS2max;
    TArrayF* fEmin;
    TArrayF* fEmax;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; // Event header.
    Int_t fNEvents;         // Event counter.

    // Canvas
    TCanvas** c_TrimEraw_vs_SciPosRaw; // [fNbDetectors]
    TCanvas** c_TrimDT_vs_SciPosRaw;   // [fNbDetectors]
    TCanvas** c_TrimE_vs_SciPosCal;    // [fNbDetectors]
    TCanvas* c_TrimE_vs_BetaS2;
    TCanvas* c_TrimE_vs_AoQraw;
    TCanvas* c_TrimZ_vs_AoQ;
    TCanvas* c_AoQ_vs_PosS2_condTrim;

    // Histogram for correlation with Trim
    TH2F** fh2_TrimEraw_vs_SciPosRaw; // [fNbDetectors*3]
    TH2F** fh2_TrimDT_vs_SciPosRaw;   // [fNbDetectors*3]
    TH2F** fh2_TrimE_vs_SciPosCal;    // [fNbDetectors*3]
    TH2F** fh2_TrimE_vs_BetaS2;       // [4]
    TH2F** fh2_TrimE_vs_AoQraw;       // [4]
    TH2F** fh2_TrimZ_vs_AoQ;          // [4]
    TH2F** fh2_AoQ_vs_PosS2_condTrim; // [4]

  public:
    ClassDef(R3BSofSciVsTrimOnlineSpectra, 1)
};

#endif
