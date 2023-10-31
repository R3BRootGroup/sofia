// ------------------------------------------------------------
// -----        R3BSofMwpcvsMusicOnlineSpectra            -----
// -----    Created 16/02/20  by J.L. Rodriguez-Sanchez   -----
// -----       Fill mwpc vs music online histograms       -----
// ------------------------------------------------------------

#ifndef R3BSofMwpcvsMusicOnlineSpectra_H
#define R3BSofMwpcvsMusicOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TMath.h"
#include <R3BShared.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define NbAnodesMus 8

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads MWPC data and plots online histograms
 */
class R3BSofMwpcvsMusicOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofMwpcvsMusicOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     * @param namedet1 a name of the detector 1.
     */
    R3BSofMwpcvsMusicOnlineSpectra(const TString& name, Int_t iVerbose = 1, const TString& namedet1 = "Mwpc0");

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofMwpcvsMusicOnlineSpectra();

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

  private:
    TClonesArray* fCalItemsMwpc;   /**< Array with cal items. */
    TClonesArray* fHitItemsMwpc;   /**< Array with hit items. */
    TClonesArray* fMappedItemsMus; /**< Array with map items. */
    TClonesArray* fHitItemsMus;    /**< Array with hit items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */
    TString fNameDet1;
    Double_t fE[NbAnodesMus], fT[NbAnodesMus + 2];
    Int_t multhit[NbAnodesMus + 2];

    // Canvas
    TCanvas* cMusECorMwpc0;
    TCanvas* cMusDTCorMwpc0;

    // Histograms for cal and hit data
    TH2F* fh2_MusCorMwpc0_EsumVsX0mm;
    TH2F* fh2_MusCorMwpc0_EsumVsY0mm;
    TH2F* fh2_MusCorMwpc0_DTvsX0[NbAnodesMus];

  public:
    ClassDef(R3BSofMwpcvsMusicOnlineSpectra, 1)
};

#endif
