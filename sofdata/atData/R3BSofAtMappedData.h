#ifndef R3BSofAtMappedData_H
#define R3BSofAtMappedData_H
#include "TObject.h"

class R3BSofAtMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofAtMappedData();

    /** Constructor with arguments
     *@param secID    Section ID
     *@param energy   Energy deposit [channels]
     **/
    R3BSofAtMappedData(UChar_t secID, UShort_t energy);

    /** Destructor **/
    virtual ~R3BSofAtMappedData() {}

    /** Accessors **/
    inline const UChar_t& GetSecID() const { return fSecID; }
    inline const UShort_t& GetEnergy() const { return fEnergy; }

    /** Modifiers **/
    void SetSecID(UChar_t id) { fSecID = id; };
    void SetEnergy(UShort_t energy) { fEnergy = energy; };

  protected:
    UChar_t fSecID;
    UShort_t fEnergy;

    ClassDef(R3BSofAtMappedData, 1)
};

#endif
