#ifndef R3BSofAnaContFact_H
#define R3BSofAnaContFact_H

#include "FairContFact.h"
#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofFrsAnaPar.h"
#include "R3BSofFragmentAnaPar.h"

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
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofAnaContFact, 0) // Factory for all Sofia Analysis parameter containers
};

#endif /* !R3BSofAnaContFact_H */
