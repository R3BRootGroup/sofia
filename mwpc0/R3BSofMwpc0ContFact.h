#ifndef R3BSofMwpc0ContFact_H
#define R3BSofMwpc0ContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofMwpc0ContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofMwpc0ContFact();
    ~R3BSofMwpc0ContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofMwpc0ContFact, 0) // Factory for all MWPC0 parameter containers
};

#endif /* !R3BSofMwpc0ContFact_H */
