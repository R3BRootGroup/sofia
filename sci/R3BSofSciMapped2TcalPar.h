#ifndef __R3BSOFSCIMAPPED2TCALPAR_H__
#define __R3BSOFSCIMAPPED2TCALPAR_H__ 1

#include "FairTask.h"
#include "TH1F.h"

class TClonesArray;
class R3BSofTcalPar;

class R3BSofSciMapped2TcalPar : public FairTask
{
  public:
    /** Default constructor **/
    R3BSofSciMapped2TcalPar();

    /** Standard constructor **/
    R3BSofSciMapped2TcalPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciMapped2TcalPar();

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

    /** Virtual method calculate the Vftx Tcal Parameters **/
    virtual void CalculateVftxTcalParams();

    void SetOutputFile(const char* outFile);

    /** Accessor functions **/
    const UShort_t GetNumSci() { return fNumSci; }
    const UShort_t GetNumChannels() { return fNumChannels; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    void SetNumSci(UShort_t num) { fNumSci = num; }
    void SetNumChannels(UShort_t num) { fNumChannels = num; }
    void SetNumTcalParsPerSignal(Int_t NumberOfTcalParsPerSignal) { fNumTcalParsPerSignal = NumberOfTcalParsPerSignal; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

  private:
    UShort_t fNumSci;              // Number of detectors (=1 if cave C only, 2 or more if FRS in used)
    UShort_t fNumChannels;         // Number of channels  (=2 if cave C only, 3 if FRS in used: LEFT, RIGHT, Tref)
    Int_t fNumTcalParsPerSignal;   // =1000 for each signal
    Int_t fMinStatistics;          // Minimum statistics to proceed to the calibration

    // calibration parameters
    R3BSofTcalPar* fTcalPar;   // Tcal Parameters

    // input data
    TClonesArray* fMapped;   // Array with mapped data from scintillator detectors

    // histograms
    TH1F** fh_TimeFineBin;
    TH1F** fh_TimeFineNs;
    char* fOutputFile;

  public:
    ClassDef(R3BSofSciMapped2TcalPar, 0);
};

#endif   //__R3BSOFSCIMAPPED2TCALPAR_H__
