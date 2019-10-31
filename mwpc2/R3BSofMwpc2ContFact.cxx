// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofMwpc2          -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc2ContFact.h"
#include "R3BSofMwpc2CalPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofMwpc2ContFact gR3BSofMwpc2ContFact;

R3BSofMwpc2ContFact::R3BSofMwpc2ContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofMwpc2ContFact";
    fTitle = "Factory for parameter containers in libR3BSofMwpc2";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofMwpc2ContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the Mwpc2 library.

    FairContainer* p1 = new FairContainer("mwpc2CalPar", "Mwpc2 Cal Parameters", "Mwpc2CalParContext");
    p1->addContext("Mwpc2CalParContext");

    containers->Add(p1);
}

FairParSet* R3BSofMwpc2ContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofMwpc2ContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "mwpc2CalPar") == 0)
    {
        p = new R3BSofMwpc2CalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

void R3BSofMwpc2ContFact::activateParIo(FairParIo* io)
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

ClassImp(R3BSofMwpc2ContFact)
