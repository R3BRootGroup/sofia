#pragma once

#include "FairContFact.h"
#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

class FairContainer;

class R3BSofAnaContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofAnaContFact();
    ~R3BSofAnaContFact() {}
    FairParSet* createContainer(FairContainer*);

    ClassDef(R3BSofAnaContFact, 0) // Factory for all Sofia Analysis parameter containers
};
