// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofMwpc0          -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofMwpc0ContFact.h"
#include "R3BSofMwpc0CalPar.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofMwpc0ContFact gR3BSofMwpc0ContFact;

R3BSofMwpc0ContFact::R3BSofMwpc0ContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofMwpc0ContFact";
    fTitle = "Factory for parameter containers in libR3BSofMwpc0";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofMwpc0ContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the Mwpc0 library.

    FairContainer* p1 = new FairContainer("mwpc0CalPar", "Mwpc0 Cal Parameters", "Mwpc0CalParContext");
    p1->addContext("Mwpc0CalParContext");

    containers->Add(p1);

    FairContainer* p2 = new FairContainer("Mwpc0GeoPar", "Mwpc0 geometry parameters", "GeometryParameterContext");
    p2->addContext("GeometryParameterContext");

    containers->Add(p2);
}

FairParSet* R3BSofMwpc0ContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofMwpc0ContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "mwpc0CalPar") == 0)
    {
        p = new R3BSofMwpc0CalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    if (strcmp(name, "Mwpc0GeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofMwpc0ContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Mwpc0
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofMwpc0ParRootFileIo* p=new R3BSofMwpc0ParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofMwpc0ParAsciiFileIo* p=new R3BSofMwpc0ParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofMwpc0ContFact)
