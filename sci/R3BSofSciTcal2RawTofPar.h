#ifndef __R3BSOFSCITCAL2RAWTOFPAR_H__
#define __R3BSOFSCITCAL2RAWTOFPAR_H__

#include "FairTask.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TF1.h"

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
    const Int_t GetNumDets() { return fNumDets; }
    const Int_t GetNumChannels() { return fNumChannels; }
    const Int_t GetDetIdCaveC() { return fDetIdCaveC; }
    const Int_t GetDetIdS2() { return fDetIdS2; }
    const Int_t GetDetIdS8() { return fDetIdS8; }
    const Int_t GetNumSignals() { return fNumSignals; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    void SetNumDets(Int_t n) { fNumDets = n ;}
    void SetNumChannels(Int_t n) { fNumChannels = n ;}
    void SetDetIdCaveC(Int_t id) { fDetIdCaveC = id ;}
    void SetDetIdS2(Int_t id) { fDetIdS2 = id ;}
    void SetDetIdS8(Int_t id) { fDetIdS8 = id ;}
    void SetNumSignals() { fNumSignals = fNumDets -1 ;}
    void SetNumParsPerSignal(Int_t n) { fNumParsPerSignal = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

  protected:
    Int_t fNumDets;
    Int_t fNumChannels;
    Int_t fDetIdCaveC;           // detector number (1-based) at Cave C versus which ToFraw will be calculated
    Int_t fDetIdS2;           // detector number (1-based) at Cave C versus which ToFraw will be calculated
    Int_t fDetIdS8;           // detector number (1-based) at Cave C versus which ToFraw will be calculated
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

    // fit functions
    TF1** fitRawTof;
    
  public:
    ClassDef(R3BSofSciTcal2RawTofPar, 0);
};

#endif //__R3BSOFSCITCAL2RAWTOFPAR_H__
