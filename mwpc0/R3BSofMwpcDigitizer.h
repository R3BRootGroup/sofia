// ----------------------------------------------------------------
// -----          R3BSofMwpcDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#ifndef R3BSofMwpcDigitizer_H
#define R3BSofMwpcDigitizer_H 1

#include "FairTask.h"
#include "R3BSofMwpcHitData.h"
#include <map>
#include <string>

class TClonesArray;

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

    virtual void Finish();
    virtual void Reset();

    /** Setters for sigmas **/
    void SetSigma_x(Float_t sigma_x) { fsigma_x = sigma_x; }
    void SetSigma_y(Float_t sigma_y) { fsigma_y = sigma_y; }
    void SetPosX(Float_t x) { fPosX = x; }
    void SetPosZ(Float_t z) { fPosZ = z; }
    void SetAngle(Float_t a) { fangle = a; }

  private:
    TClonesArray* fMCTrack;
    TClonesArray* fMwpcPoints;
    TClonesArray* fMwpcHits;
    Float_t fsigma_x;
    Float_t fsigma_y;
    Float_t fangle;
    Float_t fPosX, fPosZ;
    TString fName;

    /** Private method AddHitData **/
    // Adds a R3BSofMwpcHitData to the MwpcHitCollection
    R3BSofMwpcHitData* AddHitData(Double_t x, Double_t y);

  public:
    // Class definition
    ClassDef(R3BSofMwpcDigitizer, 1);
};

#endif
