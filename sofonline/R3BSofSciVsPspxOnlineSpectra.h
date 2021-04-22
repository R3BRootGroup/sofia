// ------------------------------------------------------------
// -----             R3BSofSciVsPspxOnlineSpectra         -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciVsPspxOnlineSpectra_H
#define R3BSofSciVsPspxOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
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
class R3BSofSciVsPspxOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciVsPspxOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciVsPspxOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciVsPspxOnlineSpectra();

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

    void SetNbDetectors(Int_t ndets) { fNbDetectors = ndets; }
    void SetNbChannels(Int_t nchs) { fNbChannels = nchs; }
    void SetIdS2(Int_t id) { fIdS2 = id; }
    void SetTofS2min(Float_t tof, Int_t rank) { fTofS2min->AddAt(tof, rank); }
    void SetTofS2max(Float_t tof, Int_t rank) { fTofS2max->AddAt(tof, rank); }
    void SetBrho0(Float_t brho0) { fBrho0 = brho0; }
    void SetDispersionS2(Float_t dispersion_mm) { fDS2 = dispersion_mm; }
    void SetDispersionCC(Float_t dispersion_mm) { fDCC = dispersion_mm; }

  private:
    TClonesArray* fSciSTcal; // Array with SofSci Tcal items.
    TClonesArray* fSciCal;   // Array with SofSci Cal items.
    TClonesArray* fPspxCal;  // Array with Pspx Cal items.
    TClonesArray* fPspxHit;  // Array with Pspx Hit items.

    Int_t fNbDetectors;
    Int_t fNbChannels;
    Int_t fIdS2;
    Float_t fBrho0;
    Float_t fDS2;
    Float_t fDCC;
    TArrayF* fTofS2min;
    TArrayF* fTofS2max;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; // Event header.
    Int_t fNEvents;         // Event counter.

    // Canvas
    TCanvas** c_PspxE_vs_SciPosCal; // [fNbDetectors]
    TCanvas* c_PspxE_vs_BetaS2;
    TCanvas* c_PspxE_vs_AoQraw;

    TH2F** fh2_PspxE_vs_SciPosCal; // [fNbDetectors]
    TH2F** fh2_PspxE_vs_BetaS2;    //
    TH2F** fh2_PspxE_vs_AoQraw;    //

  public:
    ClassDef(R3BSofSciVsPspxOnlineSpectra, 1)
};

#endif
