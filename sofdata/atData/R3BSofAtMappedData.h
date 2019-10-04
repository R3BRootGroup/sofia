#ifndef R3BSofAtMappedData_H
#define R3BSofAtMappedData_H
#include "TObject.h"

class R3BSofAtMappedData : public TObject {

public:
  /** Default constructor **/
  R3BSofAtMappedData();


  /** Constructor with arguments
   *@param energy  Energy deposit [channels]
   **/
  R3BSofAtMappedData(UShort_t energy);


  /** Destructor **/
  virtual ~R3BSofAtMappedData() { }


  /** Accessors **/
  inline const UShort_t&   GetEnergy()    const { return fEnergy;  }


  /** Modifiers **/
  void SetEnergy(UShort_t energy) { fEnergy = energy;};


 protected:

  UShort_t fEnergy;

  ClassDef(R3BSofAtMappedData,1)
};

#endif
