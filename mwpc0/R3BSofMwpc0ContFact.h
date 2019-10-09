#ifndef R3BSOFMWPC0CONTFACT_H
#define R3BSOFMWPC0CONTFACT_H
#include "FairContFact.h"

class FairContainer;

class R3BSofMwpc0ContFact : public FairContFact {


 public:

  R3BSofMwpc0ContFact();
  ~R3BSofMwpc0ContFact();
  FairParSet* createContainer(FairContainer*);
  void  activateParIo(FairParIo* io);
  ClassDef( R3BSofMwpc0ContFact,0)

private:

  void setAllContainers();

};
#endif
