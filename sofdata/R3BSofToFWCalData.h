// -------------------------------------------------------------------------
// -----              R3BSofToFWCalData header file                    -----
// -----              Created 28/11/17  by H. Alvarez Pol              -----
// -------------------------------------------------------------------------

/**  R3BSofToFWCalData.h
 **  SOFIA TIME-OF-FLIGHT WALL calibrated data
 **/

#ifndef R3BSOFTOFWCALDATA_H
#define R3BSOFTOFWCALDATA_H

#include "TObject.h"
#include "FairMultiLinkedData.h"

class R3BSofToFWCalData : public FairMultiLinkedData {
 public:
  
  /** Default constructor **/
  R3BSofToFWCalData();
  
  /** Constructor with arguments
   *@param fDetector    Detector unique identifier
   *@param fPmt         Pmt identifier
   *@param fTime        Calibrated Time 
   *@param fEnergy      Calibrated Energy
   **/
  R3BSofToFWCalData(UChar_t ident, UChar_t pmt, 
		    Double_t time, Float_t energy) ;
  
  /** Copy constructor **/
  R3BSofToFWCalData(const R3BSofToFWCalData&);
  
  R3BSofToFWCalData& operator=(const R3BSofToFWCalData&) { return *this; }
  
  /** Destructor **/
  virtual ~R3BSofToFWCalData();
  
  /** Accessors **/
  inline const UChar_t& GetDetector()      const {return fDetector;}
  inline const UChar_t& GetPmt()           const {return fPmt;}
  inline const Double_t&  GetTime()          const {return fTime;}
  inline const Float_t&  GetEnergy()        const {return fEnergy;}  
  
  /** Modifiers **/
  void SetDetector(UChar_t ident){ fDetector = ident; }
  void SetPmt(UChar_t pmt){fPmt = pmt;}
  void SetTime(Double_t time){fTime = time;}
  void SetEnergy(Float_t energy){fEnergy = energy;}
  
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;
  
 protected:
  UChar_t fDetector;    // 1..28
  UChar_t fPmt;         // needed at this level?
  Double_t fTime;
  Float_t fEnergy;
  
  ClassDef(R3BSofToFWCalData,1)
};

#endif
