// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofMwpc2          -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc1ContFact.h"
#include "R3BSofMwpc1CalPar.h"

#include "FairRuntimeDb.h"
#include "FairParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairLogger.h"
#include "TClass.h"

static R3BSofMwpc1ContFact gR3BSofMwpc1ContFact;


R3BSofMwpc1ContFact::R3BSofMwpc1ContFact()
{
  // Constructor (called when the library is loaded)
  fName="R3BSofMwpc1ContFact";
  fTitle="Factory for parameter containers in libR3BSofMwpc1";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofMwpc1ContFact::setAllContainers()
{
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Mwpc1 library.

  FairContainer* p1= new FairContainer("mwpc1CalPar",
                                       "Mwpc1 Cal Parameters",
                                       "Mwpc1CalParContext");
  p1->addContext("Mwpc1CalParContext");

  containers->Add(p1);

}


FairParSet* R3BSofMwpc1ContFact::createContainer(FairContainer* c)
{
  // Trals the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatinated with the context.

  const char* name=c->GetName();
  LOG(INFO) << "R3BSofMwpc1ContFact: Create container name: " << name;
  FairParSet* p=0;
  if (strcmp(name,"mwpc1CalPar")==0){
    p=new R3BSofMwpc1CalPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return p;

}


void  R3BSofMwpc1ContFact::activateParIo(FairParIo* io)
{
  // activates the input/output class for the parameters
  // needed by the Mwpc2
  /*
  if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
    R3BSofMwpc2ParRootFileIo* p=new R3BSofMwpc2ParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
    R3BSofMwpc2ParAsciiFileIo* p=new R3BSofMwpc2ParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
    }
  */
}

ClassImp(R3BSofMwpc1ContFact)
