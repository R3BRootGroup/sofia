#ifndef __R3BSOFTRIMDTOFFSETPAR_H__
#define __R3BSOFTRIMDTOFFSETPAR_H__

#include "FairTask.h"

#include "TH1.h"

class TClonesArray;
class R3BSofTrimCalPar;
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

    const Float_t GetMwpc0OffsetX() { return fMwpc0OffsetX; }
    const Float_t GetMwpc1OffsetX() { return fMwpc1OffsetX; }

    const Float_t GetDistMwpc0Anode1() { return fDistMwpc0Anode1; }
    const Float_t GetDistMwpc0Mwpc1() { return fDistMwpc0Mwpc1; }

    const Float_t GetWidthAnode() { return fWidthAnode; }
    const Float_t GetDistInterSection() { return fDistInterSection; }
    const Float_t GetDriftVelocity() { return fDriftVelocity; }

    void SetNumSections(Int_t n) { fNumSections = n; }
    void SetNumAnodes(Int_t n) { fNumAnodes = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

    void SetMwpc0OffsetX(Float_t offset) { fMwpc0OffsetX = offset; }
    void SetMwpc1OffsetX(Float_t offset) { fMwpc1OffsetX = offset; }

    void SetDistMwpc0Anode1(Float_t d) { fDistMwpc0Anode1 = d; }
    void SetDistMwpc0Mwpc1(Float_t d) { fDistMwpc0Mwpc1 = d; }

    void SetWidthAnode(Float_t w) { fWidthAnode = w; }
    void SetDistInterSection(Float_t d) { fDistInterSection = d; }
    void SetDriftVelocity(Float_t v) { fDriftVelocity = v; }

  protected:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fMinStatistics; // minimum statistics to proceed to the calibration

    Float_t fMwpc0OffsetX;
    Float_t fMwpc1OffsetX;

    Float_t fDistMwpc0Anode1;
    Float_t fDistMwpc0Mwpc1;
    Float_t fWidthAnode;
    Float_t fDistInterSection;
    Float_t fDriftVelocity;

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

#endif //__R3BSOFTRIMDTOFFSETPAR_H__
