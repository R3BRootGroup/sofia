#ifndef __R3BSofTrimDTOffsetPar_H__
#define __R3BSofTrimDTOffsetPar_H__

#include "FairTask.h"
#include "TH1.h"

#include "R3BSofTrimCalPar.h"
#include "R3BTGeoPar.h"
#include "TClonesArray.h"

class R3BEventHeader;

class R3BSofTrimCalculateDriftTimeOffsetPar : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTrimCalculateDriftTimeOffsetPar();

    /** Standard constructor **/
    R3BSofTrimCalculateDriftTimeOffsetPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTrimCalculateDriftTimeOffsetPar();

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

    /** Virtual method calculate the gain matching of both anodes  **/
    virtual void CalculateOffsets();

    void SetOutputFile(const char* outFile);

    /** Accessor functions **/
    const Int_t GetNumSections() { return fNumSections; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    const Double_t GetWidthAnode() { return fWidthAnode; }
    const Double_t GetDistInterSection() { return fDistInterSection; }
    const Double_t GetDriftVelocity() { return fDriftVelocity; }

    void SetNumSections(Int_t n) { fNumSections = n; }
    void SetNumAnodes(Int_t n) { fNumAnodes = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

    void SetWidthAnode(Double_t w) { fWidthAnode = w; }
    void SetDistInterSection(Double_t d) { fDistInterSection = d; }
    void SetDriftVelocity(Double_t v) { fDriftVelocity = v; }

  protected:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fMinStatistics; // minimum statistics to proceed to the calibration

    // Geometry for Mwpc0 and Mwpc1
    R3BTGeoPar* fMwpc0GeoPar;
    R3BTGeoPar* fMwpc1GeoPar;
    R3BTGeoPar* fTrimGeoPar;

    Double_t fWidthAnode;
    Double_t fDistInterSection;
    Double_t fDriftVelocity;

    // calibration parameters
    R3BSofTrimCalPar* fCalPar;

    // input data
    TClonesArray* fTrimCalData;
    TClonesArray* fMwpc0HitData;
    TClonesArray* fMwpc1HitData;

    // histograms
    TH1D** fh1_DeltaDT;

    char* fOutputFile;

  public:
    ClassDef(R3BSofTrimCalculateDriftTimeOffsetPar, 0);
};

#endif //__R3BSofTrimDTOffsetPar_H__
