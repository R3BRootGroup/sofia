// ----------------------------------------------------------------
// -----          R3BSofMwpcDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#ifndef R3BSofMwpcDigitizer_H
#define R3BSofMwpcDigitizer_H 1

#include "FairTask.h"
#include "R3BSofMwpcHitData.h"
#include "TRotation.h"
#include "TVector3.h"
#include <map>
#include <string>

class TClonesArray;
class R3BTGeoPar;

class R3BSofMwpcDigitizer : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofMwpcDigitizer();

    /** Standard constructor **/
    R3BSofMwpcDigitizer(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    ~R3BSofMwpcDigitizer();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    // Fair specific
    virtual void SetParContainers();

    virtual void Finish();

    virtual void Reset();

    /** Setters for sigmas **/
    void SetSigma_x(Float_t sigma_x) { fsigma_x = sigma_x; }
    void SetSigma_y(Float_t sigma_y) { fsigma_y = sigma_y; }

  private:
    void SetParameter();

    TClonesArray* fMCTrack;
    TClonesArray* fMwpcPoints;
    TClonesArray* fMwpcHits;
    R3BTGeoPar* fMwpcGeoPar;
    TString fName;
    Float_t fsigma_x;
    Float_t fsigma_y;
    TVector3 fTrans;
    TRotation fRot;

    /** Private method AddHitData **/
    // Adds a R3BSofMwpcHitData to the MwpcHitCollection
    R3BSofMwpcHitData* AddHitData(Double_t x, Double_t y);

  public:
    // Class definition
    ClassDef(R3BSofMwpcDigitizer, 1);
};

#endif
