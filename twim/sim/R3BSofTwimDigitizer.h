// ----------------------------------------------------------------
// -----          R3BSofTwimDigitizer source file             -----
// -----         Created 06/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#ifndef R3BSofTwimDigitizer_H
#define R3BSofTwimDigitizer_H 1

#include "FairTask.h"
#include "R3BSofTwimHitData.h"
#include <map>
#include <string>

class TClonesArray;

class R3BSofTwimDigitizer : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTwimDigitizer();

    /** Standard constructor **/
    R3BSofTwimDigitizer(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTwimDigitizer();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    virtual void Reset();

    /** Setters for sigmas **/
    void SetSigma_x(Float_t sigma_x) { fsigma_x = sigma_x; }
    void SetPosX(Float_t x) { fPosX = x; }
    void SetPosZ(Float_t z) { fPosZ = z; }
    void SetAngle(Float_t a) { fangle = a; }

  private:
    TClonesArray* fMCTrack;
    TClonesArray* fTwimPoints;
    TClonesArray* fTwimHits;
    Int_t fDetId;
    Float_t fsigma_x;
    Float_t fangle;
    Float_t fPosX, fPosZ;
    TString fName;

    // Adds a R3BSofTwimHitData to the HitCollection
    R3BSofTwimHitData* AddR3BHitData(Int_t detId, Double_t theta, Double_t z);

  public:
    // Class definition
    ClassDef(R3BSofTwimDigitizer, 1);
};

#endif /* R3BSofTwimDigitizer_H */
