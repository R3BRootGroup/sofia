#ifndef __R3BSOFSCITCAL2RAWPOSPAR_H__
#define __R3BSOFSCITCAL2RAWPOSPAR_H__

#include "FairTask.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TF1.h"

class TClonesArray;
class R3BSofSciRawPosPar;
class R3BEventHeader;

class R3BSofSciTcal2RawPosPar : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofSciTcal2RawPosPar();

    /** Standard constructor **/
    R3BSofSciTcal2RawPosPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciTcal2RawPosPar();

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

    /** Virtual method calculate the PosRaw Single Tcal Parameters **/
    virtual void CalculateRawPosRawPosParams();

    void SetOutputFile(const char* outFile);

    /** Accessor functions **/
    const Int_t GetNumDets()     { return fNumDets; }
    const Int_t GetNumPmts()     { return fNumPmts; }
    const Int_t GetNumSignals()  { return fNumSignals; }
    const Int_t GetMinStatistics()  { return fMinStatistics; }

    void SetNumDets(Int_t n) { fNumDets = n; }
    void SetNumPmts(Int_t n) { fNumPmts = n; }
    void SetNumSignals()
    {
        if (fNumDets)
            fNumSignals = fNumDets;
    }
    void SetNumParsPerSignal(Int_t n) { fNumParsPerSignal = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

  protected:
    Int_t fNumDets;     // number of detectors 2 if FRS, 1 if Cave C only
    Int_t fNumPmts;      // number of channels at the Tcal level
    Int_t fNumSignals;       // number of signal = fNumDets if RawPos used
    Int_t fNumParsPerSignal; // =2 for each signal
    Int_t fMinStatistics;    // minimum statistics to proceed to the calibration

    // calibration parameters
    R3BSofSciRawPosPar* fRawPosPar;

    // input data
    TClonesArray* fTcal;

    // histograms
    TH1D** fh_RawPosMult1;
    char* fOutputFile;

    // fit functions
    TF1** fitRawPos;

  public:
    ClassDef(R3BSofSciTcal2RawPosPar, 0);
};

#endif //__R3BSOFSCITCAL2RAWPOSPAR_H__
