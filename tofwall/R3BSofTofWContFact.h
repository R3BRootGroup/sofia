// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofTofW           -----
//								    -----
// ----------------------------------------------------------------------

#ifndef R3BSofTofWContFact_H
#define R3BSofTofWContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofTofWContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofTofWContFact();
    ~R3BSofTofWContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofTofWContFact, 0) // Factory for all TofWall parameter containers
};

#endif /* !R3BSofTofWContFact_H */
