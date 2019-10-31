#ifndef __R3BSOFTCALCONTFACT_H__
#define __R3BSOFTCALCONTFACT_H__

#include "FairContFact.h"

// --- ------------------------------------- --- //
// --- Factory for tcal parameter containers --- //
// --- ------------------------------------- --- //

class FairContainer;

class R3BSofTcalContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofTcalContFact();
    ~R3BSofTcalContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofTcalContFact, 1)
};

#endif // R3BTCALCONTFACT_H
