// -------------------------------------------------------------------------
// -----                      R3BSofTwimMappedData header file         -----
// -----                  Created 29/09/19 by JL Rodriguez             -----
// -------------------------------------------------------------------------

#ifndef R3BSofTwimMappedData_H
#define R3BSofTwimMappedData_H
#include "TObject.h"

class R3BSofTwimMappedData : public TObject {

public:
  /** Default constructor **/
  R3BSofTwimMappedData();


  /** Constructor with arguments
   *@param secID    Section ID
   *@param anodeID  Anode ID 
   *@param dtime    Drift time [channels]
   *@param eLoss    Energy deposit [channels]
   **/
  R3BSofTwimMappedData(Int_t secID, 
                       Int_t anodeID, 
                       Int_t dtime,
	               Int_t energy);


  /** Destructor **/
  ~R3BSofTwimMappedData();


  /** Accessors **/
  inline const Int_t&    GetSecID()     const { return fSecID;   }
  inline const Int_t&    GetAnodeID()   const { return fAnodeID; }
  inline const Int_t&    GetDTime()     const { return fDT;      }
  inline const Int_t&    GetEnergy()    const { return fEnergy;  }


  /** Modifiers **/
  void SetSecID(Int_t id)      { fSecID = id;     };
  void SetAnodeID(Int_t id)    { fAnodeID = id;   };
  void SetDTime(Int_t dt)      { fDT = dt;        };
  void SetEnergy(Int_t energy) { fEnergy = energy;};


 protected:

  Int_t fSecID, fAnodeID;
  Int_t fDT, fEnergy;

  ClassDef(R3BSofTwimMappedData,1)
};

#endif
