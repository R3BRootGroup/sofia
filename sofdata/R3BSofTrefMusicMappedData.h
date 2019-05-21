#ifndef R3BSOFTREFMUSICMAPPEDITEM_H
#define R3BSOFTREFMUSICMAPPEDITEM_H

#include "TObject.h"



class R3BSofTrefMusicMappedData : public TObject
{
  public:
    // Default Constructor
    R3BSofTrefMusicMappedData();

    // Standard Constructor
    R3BSofTrefMusicMappedData(UShort_t plane,
			      UShort_t section,
			      UShort_t time,
			      UShort_t mult,
			      UShort_t nanodes
			);

    // Destructor
    virtual ~R3BSofTrefMusicMappedData() { }

    // Getters
    inline const UShort_t& GetPlane()       const {return fPlane;}
    inline const UShort_t& GetSection()     const {return fSection;}
    inline const UShort_t& GetTime()        const {return fTime;}
    inline const UShort_t& GetMultTref()    const {return fMultTref;}
    inline const UShort_t& GetnAnodes()     const {return fnAnodes;}

  private:
    UShort_t fPlane; 
    UShort_t fSection;
    UShort_t fTime;
    UShort_t fMultTref;
    UShort_t fnAnodes;
    // For fMultTref != 1 event shoudl be discarded 
    // if fMultTref !=1 happen a lot : the searching window is not properly set


  public:
    ClassDef(R3BSofTrefMusicMappedData, 2)
};

#endif
