// -------------------------------------------------------------------
//								   -----
//  Factory for the parameter containers in libR3BSofTcal        -----
//								   -----
// -------------------------------------------------------------------

#ifndef R3BSOFTCALCONTFACT_H
#define R3BSOFTCALCONTFACT_H

#include "FairContFact.h"

class FairParSet;

class R3BSofTcalContFact : public FairContFact
{
  public:
    R3BSofTcalContFact();
    ~R3BSofTcalContFact() {}

    FairParSet* createContainer(FairContainer* c);

  private:
    void setAllContainers();

    ClassDef(R3BSofTcalContFact, 1);
};

#endif   // R3BTCALCONTFACT_H
