// ------------------------------------------------------------------
// -----             R3BSofTrimContFact source file             -----
// ------------------------------------------------------------------

#pragma once

#include <FairContFact.h>

class FairContainer;

class R3BSofTrimContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofTrimContFact();
    ~R3BSofTrimContFact() = default;

    FairParSet* createContainer(FairContainer*);

    ClassDef(R3BSofTrimContFact, 0); // Factory for all R3BSofTrim parameter containers
};
