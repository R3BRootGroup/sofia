#ifndef R3BSofMwpc2ContFact_H
#define R3BSofMwpc2ContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofMwpc2ContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BSofMwpc2ContFact();
    ~R3BSofMwpc2ContFact() {}
    FairParSet* createContainer(FairContainer*);
    void activateParIo(FairParIo* io);
    ClassDef(R3BSofMwpc2ContFact, 0) // Factory for all MWPC2 parameter containers
};

#endif /* !R3BSofMwpc2ContFact_H */
