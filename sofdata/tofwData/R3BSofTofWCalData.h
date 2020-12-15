// -------------------------------------------------------------------------
// -----              R3BSofTofWCalData header file                    -----
// -----              Created 28/11/17  by H. Alvarez Pol              -----
// -------------------------------------------------------------------------

/**  R3BSofTofWCalData.h
 **  SOFIA TIME-OF-FLIGHT WALL calibrated data
 **/

#ifndef R3BSofTofWCalData_H
#define R3BSofTofWCalData_H

#include "FairMultiLinkedData.h"
#include "TObject.h"

class R3BSofTofWCalData : public FairMultiLinkedData
{
  public:
    /** Default constructor **/
    R3BSofTofWCalData();

    /** Constructor with arguments
     *@param fDetector    Detector unique identifier
     *@param fPmt         Pmt identifier
     *@param fTime        Calibrated Time
     *@param fEnergy      Calibrated Energy
     **/
    R3BSofTofWCalData(UChar_t ident, UShort_t pmt, Double_t time, Float_t energy);

    /** Copy constructor **/
    R3BSofTofWCalData(const R3BSofTofWCalData&);

    R3BSofTofWCalData& operator=(const R3BSofTofWCalData&) { return *this; }

    /** Destructor **/
    virtual ~R3BSofTofWCalData();

    /** Accessors **/
    UChar_t GetDetector() const { return fDetector; }
    UShort_t GetPmt() const { return fPmt; }
    Double_t GetTime() const { return fTime; }
    Float_t GetEnergy() const { return fEnergy; }

    /** Modifiers **/
    void SetDetector(UChar_t ident) { fDetector = ident; }
    void SetPmt(UShort_t pmt) { fPmt = pmt; }
    void SetTime(Double_t time) { fTime = time; }
    void SetEnergy(Float_t energy) { fEnergy = energy; }

    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

  protected:
    UChar_t fDetector; // 1..28
    UShort_t fPmt;     // needed at this level?
    Double_t fTime;
    Float_t fEnergy;

    ClassDef(R3BSofTofWCalData, 1)
};

#endif
