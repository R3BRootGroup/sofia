// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofMwpc1          -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc1ContFact.h"
#include "R3BSofMwpc1CalPar.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofMwpc1ContFact gR3BSofMwpc1ContFact;

R3BSofMwpc1ContFact::R3BSofMwpc1ContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofMwpc1ContFact";
    fTitle = "Factory for parameter containers in libR3BSofMwpc1";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofMwpc1ContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the Mwpc1 library.

    FairContainer* p1 = new FairContainer("mwpc1CalPar", "Mwpc1 Cal Parameters", "Mwpc1CalParContext");
    p1->addContext("Mwpc1CalParContext");

    containers->Add(p1);

    FairContainer* p2 = new FairContainer("mwpc1GeoPar", "MWPC1 geometry parameters", "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2);
}

FairParSet* R3BSofMwpc1ContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofMwpc1ContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "mwpc1CalPar") == 0)
    {
        p = new R3BSofMwpc1CalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    if (strcmp(name, "mwpc1GeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofMwpc1ContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Mwpc1
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofMwpc1ParRootFileIo* p=new R3BSofMwpc1ParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofMwpc1ParAsciiFileIo* p=new R3BSofMwpc1ParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofMwpc1ContFact)
