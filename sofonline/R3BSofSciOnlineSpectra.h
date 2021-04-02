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
    void SetCalTofS2min(Float_t tof, Int_t rank) { fCalTofS2min->AddAt(tof, rank); }
    void SetCalTofS2max(Float_t tof, Int_t rank) { fCalTofS2max->AddAt(tof, rank); }
    void SetCalTofS8min(Float_t tof, Int_t rank) { fCalTofS8min->AddAt(tof, rank); }
    void SetCalTofS8max(Float_t tof, Int_t rank) { fCalTofS8max->AddAt(tof, rank); }
    Int_t GetNbDetectors() { return fNbDetectors; }
    Int_t GetNbChannels() { return fNbChannels; }
    Int_t GetIdS2() { return fIdS2; }
    Int_t GetIdS8() { return fIdS8; }
    Float_t GetTofS2min(Int_t rank) { return fCalTofS2min->GetAt(rank); }
    Float_t GetTofS2max(Int_t rank) { return fCalTofS2max->GetAt(rank); }
    Float_t GetTofS8min(Int_t rank) { return fCalTofS8min->GetAt(rank); }
    Float_t GetTofS8max(Int_t rank) { return fCalTofS8max->GetAt(rank); }

  private:
    TClonesArray* fMapped;     /**< Array with R3BSofSciMappedData */
    TClonesArray* fTcal;       /**< Array with R3BSofSciTcalData */
    TClonesArray* fSingleTcal; /**< Array with R3BSofSciSingleTcalData */
    TClonesArray* fCal;        /**< Array with R3BSofSciCalData */

    Int_t fNbDetectors; // fNbDetectors is also equal to fIdCaveC
    Int_t fNbChannels;
    Int_t fIdS2;
    Int_t fIdS8;
    TArrayF* fCalTofS2min;
    TArrayF* fCalTofS2max;
    TArrayF* fCalTofS8min;
    TArrayF* fCalTofS8max;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* fEventHeader; /**< Event header.      */
    Int_t fNEvents;               /**< Event counter.     */

    // Canvas
    TCanvas* cDeltaClockPerSci;
    TCanvas* cDeltaTref;
    TCanvas** cMapped;     // [fNbDetectors]
    TCanvas** cPos;        // [fNbDetectors]
    TCanvas** cTofFromS2;  // [fNbDetectors - fIdS2]
    TCanvas** cBetaFromS2; // [fNbDetectors - fIdS2]
    TCanvas** cTofFromS8;  // [fNbDetectors - fIdS8]
    TCanvas** cBetaFromS8; // [fNbDetectors - fIdS8]

    TCanvas* cMultMap;
    TCanvas* cMultTcal;
    TCanvas* cMultSingleTcal;
    TCanvas* cMultCal;
    TCanvas* cMultMap2D;
    TCanvas* cMultMap2D_RvsL;
    TCanvas* cPosVsTofS2;
    TCanvas* cPosVsTofS8;
    TCanvas** cRawPosVsCalPos; // [fNbDetectors]

    // Histograms - Delta Clock
    TH1F** fh1_deltaClockPerSci;
    // Histograms - Delta Clock with condition on Tpat
    TH1F** fh1_deltaClockPerSci_condTpat;

    // Histograms - 1D multiplicity
    TH1I** fh1_multMap;        // [fNbDetectors * fNbChannels];
    TH1I** fh1_multTcal;       // [fNbDetectors * fNbChannels];
    TH1I** fh1_multSingleTcal; // [fNbDetectors]
    TH1I** fh1_multCal;        // [fNbDetectors]
    // Histograms - 1D multiplicity with condition on Tpat
    TH1I** fh1_multMap_condTpat;        // [fNbDetectors * fNbChannels];
    TH1I** fh1_multTcal_condTpat;       // [fNbDetectors * fNbChannels];
    TH1I** fh1_multSingleTcal_condTpat; // [fNbDetectors]
    TH1I** fh1_multCal_condTpat;        // [fNbDetectors]

    // Histograms - 2D multiplicity
    TH2I** fh2_mult_RvsL;      // [fNbDetectors];
    TH2I** fh2_mult_TrefVsPmt; //[fNbDetectors * (NbChannels-1)]

    // Histograms - 2D multiplicity with condition on Tpat
    TH2I** fh2_mult_RvsL_condTpat;      // [fNbDetectors];
    TH2I** fh2_mult_TrefVsPmt_condTpat; //[fNbDetectors * (NbChannels-1)]

    // Histograms for Mapped data : Fine Time and Mult
    TH1I** fh1_finetime; // [fNbDetectors * NbChannels];
    TH2I** fh2_mult;     // [fNbDetectors];
    TH1D** fh1_DeltaTref;

    // Histograms for X position at Tcal, SingleTcal and Cal levels
    TH1F** fh1_RawPos_TcalMult1;  // [fNbDetectors];
    TH1F** fh1_RawPos_SingleTcal; // [fNbDetectors];
    TH1F** fh1_CalPos;            // [fNbDetectors];

    // Histograms for time of flight from S2
    TH1D** fh1_RawTofFromS2_TcalMult1;  // [fNbDetectors - fIdS2];
    TH1D** fh1_RawTofFromS2_SingleTcal; // [fNbDetectors - fIdS2];
    TH1D** fh1_CalTofFromS2;            // [fNbDetectors - fIdS2];

    // Histograms for beta calculated from time of flight from S2
    TH1D** fh1_BetaFromS2; // [fNbDetectors - fIdS2];

    // Histograms for time of flight from S8
    TH1D** fh1_RawTofFromS8_TcalMult1;  // [fNbDetectors - fIdS8];
    TH1D** fh1_RawTofFromS8_SingleTcal; // [fNbDetectors - fIdS8];
    TH1D** fh1_CalTofFromS8;            // [fNbDetectors - fIdS8];

    // Histograms for beta calculated from time of flight from S8
    TH1D** fh1_BetaFromS8; // [fNbDetectors - fIdS8];

    // Histograms PosRaw vs PosCal
    TH2F** fh2_RawPosVsCalPos;

    // Histograms Pos vs Tof
    TH2D** fh2_PosVsTofS2; //[2*(fNbDetectors-fIdS2)]
    TH2D** fh2_PosVsTofS8; //[2*(fNbDetectors-fIdS8)]

  public:
    ClassDef(R3BSofSciOnlineSpectra, 1)
};

#endif
