// ------------------------------------------------------------------
// -----         R3BSofGladFieldPar source file                 -----
// -----    Created 22/02/21  by J.L. Rodriguez-Sanchez         -----
// ------------------------------------------------------------------

#ifndef R3BSofGladFieldPar_H
#define R3BSofGladFieldPar_H

#include "FairLogger.h"
#include "FairParGenericSet.h"
#include "FairParamList.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TObject.h"
#include "TString.h"

#include <iostream>

class FairParamList;

class R3BSofGladFieldPar : public FairParGenericSet
{
  public:
    /** Standard constructor **/
    R3BSofGladFieldPar(const TString& name = "GladFieldPar",
                       const TString& title = "Glad Field Parameters",
                       const TString& context = "GladFieldParContext");

    /** Destructor **/
    virtual ~R3BSofGladFieldPar();

    /** Method to reset all parameters **/
    virtual void clear();

    /** Method to store all parameters using FairRuntimeDB **/
    virtual void putParams(FairParamList* list);

    /** Method to retrieve all parameters using FairRuntimeDB**/
    Bool_t getParams(FairParamList* list);

    /** Method to print values of parameters to the standard output **/
    void printParams();

    /** Accessor functions **/
    const Float_t GetMagneticField() { return fBz; }
    const Float_t GetEffectiveLength() { return fEffLength; }
    const Float_t GetFieldCentre() { return fFieldCentre; }

    void SetMagneticField(Float_t b) { fBz = b; }
    void SetEffectiveLength(Float_t l) { fEffLength = l; }
    void SetFieldCentre(Float_t c) { fFieldCentre = c; }
    /** Create more Methods if you need them! **/

  private:
    Float_t fBz;          // Magnetic field in the Z direction [Tesla]
    Float_t fEffLength;   // Effective magnetic field length [cm]
    Float_t fFieldCentre; // Field centre position in the Z direction [cm]

    const R3BSofGladFieldPar& operator=(const R3BSofGladFieldPar&); /*< an assignment operator>*/

    R3BSofGladFieldPar(const R3BSofGladFieldPar&); /*< a copy constructor >*/

    ClassDef(R3BSofGladFieldPar, 1);
};

#endif
