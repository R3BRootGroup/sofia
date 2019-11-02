// ---------------------------------------------------------
// -----                                               -----
// -----		R3BSofMwpc3ContFact 	       -----
// ----- 	Created 11/10/2019 by G.García Jiménez -----
// ---------------------------------------------------------

#include "R3BSofMwpc3ContFact.h"
#include "R3BSofMwpc3CalPar.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofMwpc3ContFact gR3BSofMwpc3ContFact;

R3BSofMwpc3ContFact::R3BSofMwpc3ContFact()
{
    /* ---- Constructor (called when the library is loaded) ---- */
    fName = "R3BSofMwpc3ContFact";
    fTitle = "Factory for parameter containers in libR3BSofMwpc3";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofMwpc3ContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the Mwpc3 library.

    FairContainer* p1 = new FairContainer("mwpc3CalPar", "Mwpc3 Cal Parameters", "Mwpc3CalParContext");
    p1->addContext("Mwpc3CalParContext");

    containers->Add(p1);

    FairContainer* p2 = new FairContainer("mwpc3GeoPar", "MWPC3 geometry parameters", "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2);
}

FairParSet* R3BSofMwpc3ContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofMwpc3ContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "mwpc3CalPar") == 0)
    {
        p = new R3BSofMwpc3CalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    if (strcmp(name, "mwpc3GeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofMwpc3ContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Mwpc3
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofMwpc3ParRootFileIo* p=new R3BSofMwpc3ParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofMwpc3ParAsciiFileIo* p=new R3BSofMwpc3ParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofMwpc3ContFact)
