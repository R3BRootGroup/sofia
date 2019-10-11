// --------------------------------------------------------
// -----                                              -----
// -----		      R3BSofMwpc3ContFact 			          -----
// ----- 	Created 11/10/2019 by G.García Jiménez		  -----
// --------------------------------------------------------


#ifndef R3BSOFMWPC3CONTFACT_H
#define R3BSOFMWPC3CONTFACT_H

#include "FairContFact.h"

class FairContainer;

class R3BSofMwpc3ContFact : public FairContFact {

 private:
  void setAllContainers();

 public:
  R3BSofMwpc3ContFact();
  ~R3BSofMwpc3ContFact(){}
  FairParSet* createContainer (FairContainer*);
  void activateParIo(FairParIo* io);
  ClassDef(R3BSofMwpc3ContFact,0)

};

#endif
