// -------------------------------------------------------------------
//								   -----
//  Factory for the parameter containers in libR3BSofTofW        -----
//								   -----
// -------------------------------------------------------------------

#ifndef R3BSofTofWContFact_H
#define R3BSofTofWContFact_H

#include "FairContFact.h"

class FairParSet;

class R3BSofTofWContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofTofWContFact();
    ~R3BSofTofWContFact() {}
    FairParSet* createContainer(FairContainer* c);
    ClassDef(R3BSofTofWContFact, 0) // Factory for all TofWall parameter containers
};

#endif // !R3BSofTofWContFact_H
