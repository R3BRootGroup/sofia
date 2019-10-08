#ifndef R3BSofTwimContFact_H
#define R3BSofTwimContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofTwimContFact : public FairContFact {
private:
  void setAllContainers();
public:
  R3BSofTwimContFact();
  ~R3BSofTwimContFact() {}
  FairParSet* createContainer(FairContainer*);
  void  activateParIo(FairParIo* io);
  ClassDef(R3BSofTwimContFact,0) // Factory for all TWIM parameter containers
};

#endif  /* !R3BSofTwimContFact_H */
