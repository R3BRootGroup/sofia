// ------------------------------------------------------------
// -----                R3BSofSciOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciOnlineSpectra_H
#define R3BSofSciOnlineSpectra_H

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
class R3BSofSciOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciOnlineSpectra();

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
    Int_t GetNbDetectors() { return fNbDetectors; }
    Int_t GetNbChannels() { return fNbChannels; }
    Int_t GetIdS2() { return fIdS2; }
    Int_t GetIdS8() { return fIdS8; }

  private:
    TClonesArray* fMappedItemsSci;     /**< Array with mapped items. */
    TClonesArray* fTcalItemsSci;       /**< Array with tcal items. */
    TClonesArray* fSingleTcalItemsSci; /**< Array with tcal items. */
    TClonesArray* fMusHitItems;        /**< Array with MUSIC Hit items. */
    TClonesArray* fMusCalItems;        /**< Array with MUSIC Cal items. */
    TClonesArray* fCalItemsMwpc0;      /**< Array with cal items of mwpc0. */
    TClonesArray* fTofwHitData;

    Int_t fNbDetectors;
    Int_t fNbChannels;
    Int_t fIdS2;
    Int_t fIdS8;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas** cSciMult;        // [fNbDetectors];
    TCanvas** cSciRawPos;      // [fNbDetectors];
    TCanvas** cMusicZvsRawPos; // [fNbDetectors];
    TCanvas* cMwpc0vsRawPos;
    TCanvas* cMusicDTvsRawPos;
    TCanvas** cSciRawTof_FromS2;      // [fNbDetectors];
    TCanvas** cMusicZvsRawTof_FromS2; // [fNbDetectors];
    TCanvas** cSciRawTof_FromS8;      // [fNbDetectors];
    TCanvas** cMusicZvsRawTof_FromS8; // [fNbDetectors];
    TCanvas* cAqvsq;

    // Histograms for Mapped data : Fine Time and Mult
    TH1I** fh1_finetime; // [fNbDetectors * NbChannels];
    TH2I** fh2_mult;     // [fNbDetectors];

    // Histograms for PosRaw Data at Tcal and SingleTcal
    TH1F** fh1_RawPos_AtTcalMult1;  // [fNbDetectors];
    TH1F** fh1_RawPos_AtSingleTcal; // [fNbDetectors];

    TH1D** fh1_RawTof_FromS2_AtTcalMult1;        // [fNbDetectors];
    TH1D** fh1_RawTof_FromS2_AtTcalMult1_wTref;  // [fNbDetectors];
    TH1D** fh1_RawTof_FromS2_AtSingleTcal_wTref; // [fNbDetectors];

    TH1D** fh1_RawTof_FromS8_AtTcalMult1;        // [fNbDetectors];
    TH1D** fh1_RawTof_FromS8_AtTcalMult1_wTref;  // [fNbDetectors];
    TH1D** fh1_RawTof_FromS8_AtSingleTcal_wTref; // [fNbDetectors];

    // Histogram for correlation with R3B-Music
    TH2F** fh2_MusZvsRawPos; //[fNbDetectors];
    TH2F* fh2_MusDTvsRawPos;
    TH2F** fh2_MusZvsRawTof_FromS2; //[fNbDetectors];
    TH2F** fh2_MusZvsRawTof_FromS8; //[fNbDetectors];
    TH2F* fh2_Aqvsq;

    // Histogram for correlation with Mwpc0
    TH2F* fh2_Mwpc0vsRawPos;

    // check how many raw pos found

  public:
    ClassDef(R3BSofSciOnlineSpectra, 1)
};

#endif
