#ifndef R3BSofMwpc1MappedData_H
#define R3BSofMwpc1MappedData_H
#include "TObject.h"

class R3BSofMwpc1MappedData : public TObject {

public:
  /** Default constructor **/
  R3BSofMwpc1MappedData();


  /** Constructor with arguments
   *@param plane   1 for Xdown, 2 for Xup and 3 for Y
   *@param pad     Pad number
   *@param charge  Charge of each pad [channels]
   **/
  R3BSofMwpc1MappedData(UChar_t plane, 
                        UChar_t pad,
	                UShort_t charge);


  /** Destructor **/
  virtual ~R3BSofMwpc1MappedData() { }


  /** Accessors **/
  inline const UChar_t&    GetPlane()     const { return fPlane;   }
  inline const UChar_t&    GetPad()       const { return fPad;     }
  inline const UShort_t&   GetQ()         const { return fCharge;  }


  /** Modifiers **/
  void SetPlane(UChar_t plane)   { fPlane = plane;   };
  void SetPad(UChar_t pad)       { fPad = pad;       };
  void SetQ(UShort_t charge)     { fCharge = charge; };


 protected:

  UChar_t fPlane;
  UChar_t fPad;
  UShort_t fCharge;

  ClassDef(R3BSofMwpc1MappedData,1)
};

#endif
