#ifndef __R3BSOFSCITCAL2RAWTOFPAR_H__
#define __R3BSOFSCITCAL2RAWTOFPAR_H__

#include "FairTask.h"
#include "TH1D.h"
#include "TH1F.h"

#include "detectors_cfg.h"

class TClonesArray;
class R3BSofSciRawTofPar;
class R3BEventHeader;

class R3BSofSciTcal2RawTofPar : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofSciTcal2RawTofPar();

    /** Standard constructor **/
    R3BSofSciTcal2RawTofPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciTcal2RawTofPar();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    /** Virtual method FinishEvent **/
    virtual void FinishEvent();

    /** Virtual method FinishTask **/
    virtual void FinishTask();

    /** Virtual method Reset **/
    virtual void Reset();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method calculate the TofRaw Single Tcal Parameters **/
    virtual void CalculateRawTofParams();

    void SetOutputFile(const char* outFile);

    /** Accessor functions **/
    const Int_t GetFirstStaSci() { return fFirstStaSci; }
    const Double_t GetNumSignals() { return fNumSignals; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    void SetFirstStaSci(Int_t firststa) { fFirstStaSci = firststa; }
    void SetNumSignals() { fNumSignals = NUMBER_OF_SOFSCI_DETECTORS - 1; }
    void SetNumParsPerSignal(Int_t n) { fNumParsPerSignal = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

  protected:
    Int_t fFirstStaSci;
    Int_t fNumSignals;       // number of signal = number of detector -1 (Tof selection from Sci to Sci(caveC))
    Int_t fNumParsPerSignal; // =2 for each signal
    Int_t fMinStatistics;    // minimum statistics to proceed to the calibration

    // calibration parameters
    R3BSofSciRawTofPar* fRawTofPar;

    // input data
    TClonesArray* fTcal;

    // histograms
    TH1D** fh_RawTofMult1;
    char* fOutputFile;

  public:
    ClassDef(R3BSofSciTcal2RawTofPar, 0);
};

#endif //__R3BSOFSCITCAL2RAWTOFPAR_H__
