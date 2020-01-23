// ------------------------------------------------------------------
// -----         R3BSofFrsAnaPar source file                    -----
// -----         Created 27/01/20  by J.L. Rodriguez-Sanchez    -----
// ------------------------------------------------------------------

#ifndef R3BSofFrsAnaPar_H
#define R3BSofFrsAnaPar_H

#include "FairParGenericSet.h"

#include "TArrayF.h"
#include "TObjArray.h"
#include "TObject.h"
#include <TObjString.h>

class FairParamList;

class R3BSofFrsAnaPar : public FairParGenericSet
{
  public:
    /** Standard constructor **/
    R3BSofFrsAnaPar(const TString& name = "soffrsAnaPar",
                    const TString& title = "FRS-S2-Cave-C Parameters",
                    const TString& context = "SofFRSParContext");

    /** Destructor **/
    virtual ~R3BSofFrsAnaPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Double_t GetMagnificationS2Cave() { return fMagS2Cave; }
    const Double_t GetDispersionS2Cave() { return fDispS2Cave; }
    const Double_t GetPathS2Cave() { return fPathS2Cave; }
    const Double_t GetTOFS2Cave() { return fTOFS2Cave; }
    const Double_t GetRhoS0S2() { return frho_S0_S2; }
    const Double_t GetRhoS2Cave() { return frho_S2_Cave; }
    const Double_t GetBfiedS0S2() { return fBfield_S0_S2; }
    const Double_t GetBfiedS2Cave() { return fBfield_S2_Cave; }
    const Double_t GetPosFocalS2() { return fPosFocalS2; }
    const Double_t GetPosFocalCave() { return fPosFocalCave; }

    void SetMagnificationS2S4(Double_t theMag) { fMagS2Cave = theMag; }
    void SetDispersionS2S4(Double_t theDisp) { fDispS2Cave = theDisp; }
    void SetPathS2S4(Double_t thePath) { fPathS2Cave = thePath; }
    void SetTOFS2S4(Double_t theTOF) { fTOFS2Cave = theTOF; }
    void SetRhoS0S2(Double_t theRho) { frho_S0_S2 = theRho; }
    void SetRhoS2S4(Double_t theRho) { frho_S2_Cave = theRho; }
    void SetBfiedS0S2(Double_t theB) { fBfield_S0_S2 = theB; }
    void SetBfiedS2S4(Double_t theB) { fBfield_S2_Cave = theB; }
    void SetPosFocalS2(Double_t thePos) { fPosFocalS2 = thePos; }
    void SetPosFocalCave(Double_t thePos) { fPosFocalCave = thePos; }

    /** Create more Methods if you need them! **/

  private:
    Double_t fMagS2Cave;
    Double_t fDispS2Cave;
    Double_t fPathS2Cave, fTOFS2Cave;
    Double_t fPosFocalS2, fPosFocalCave;
    Double_t frho_S0_S2, fBfield_S0_S2, frho_S2_Cave, fBfield_S2_Cave;

    const R3BSofFrsAnaPar& operator=(const R3BSofFrsAnaPar&); /*< an assignment operator>*/

    R3BSofFrsAnaPar(const R3BSofFrsAnaPar&); /*< a copy constructor >*/

    ClassDef(R3BSofFrsAnaPar, 1);
};

#endif
