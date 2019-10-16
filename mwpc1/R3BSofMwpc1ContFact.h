// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofMwpc1          -----
//								    -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc1ContFact_H
#define R3BSofMwpc1ContFact_H

#include "FairContFact.h"

class FairContainer;

class R3BSofMwpc1ContFact : public FairContFact {
private:
  void setAllContainers();
public:
  R3BSofMwpc1ContFact();
  ~R3BSofMwpc1ContFact() {}
  FairParSet* createContainer(FairContainer*);
  void  activateParIo(FairParIo* io);
  ClassDef(R3BSofMwpc1ContFact,0) // Factory for all MWPC1 parameter containers
};

#endif  /* !R3BSofMwpc1ContFact_H */
