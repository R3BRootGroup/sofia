#ifndef R3BSofMwpcbMappedData_H
#define R3BSofMwpcbMappedData_H
#include "TObject.h"

class R3BSofMwpcbMappedData : public TObject {

public:
  /** Default constructor **/
  R3BSofMwpcbMappedData();


  /** Constructor with arguments
   *@param plane   1 for X and 3 for Y
   *@param pad     Pad number
   *@param charge  Charge of each pad [channels]
   **/
  R3BSofMwpcbMappedData(UChar_t plane, 
                        UChar_t pad,
	                UShort_t charge);


  /** Destructor **/
  virtual ~R3BSofMwpcbMappedData() { }


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

  ClassDef(R3BSofMwpcbMappedData,1)
};

#endif
