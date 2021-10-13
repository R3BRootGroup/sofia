// ------------------------------------------------------------------
// -----             R3BSofTrimContFact source file             -----
// ------------------------------------------------------------------

#ifndef R3BSofTrimContFact_H
#define R3BSofTrimContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofTrimContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofTrimContFact();
    ~R3BSofTrimContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef(R3BSofTrimContFact, 0) // Factory for all R3BSofTrim parameter containers
};

#endif /* R3BSofTrimContFact_H */
