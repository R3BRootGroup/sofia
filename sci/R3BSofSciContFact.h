#ifndef __R3BSOFSCICONTFACT_H__
#define __R3BSOFSCICONTFACT_H__

#include "FairContFact.h"

// --- --------------------------------------- --- //
// --- Factory for SofSci parameter containers --- //
// --- --------------------------------------- --- //

class FairContainer;

class R3BSofSciContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofSciContFact();
    ~R3BSofSciContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofSciContFact, 1)
};

#endif // R3BSCICONTFACT_H
