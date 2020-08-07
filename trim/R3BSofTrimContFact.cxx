// --------------------------------------------------------------------
// -----          R3BSofTrimContFact source file                  -----
// --------------------------------------------------------------------
//
//  R3BSofTrimContFact
//
//  Factory for the parameter containers in libR3BSofTrim
//

#include "R3BSofTrimContFact.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofTrimCalPar.h"

#include "TClass.h"

static R3BSofTrimContFact gR3BSofTrimContFact;

R3BSofTrimContFact::R3BSofTrimContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTrimContFact";
    fTitle = "Factory for parameter containers in libR3BSofTrim";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTrimContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the STS library.

    FairContainer* p1 = new FairContainer("trimCalPar", "Triple MUSIC Cal Parameters", "TrimCalParContext");
    p1->addContext("TrimCalParContext");

    containers->Add(p1);
}

FairParSet* R3BSofTrimContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofTrimContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "trimCalPar") == 0)
    {
        p = new R3BSofTrimCalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

void R3BSofTrimContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Trim
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofTrimParRootFileIo* p=new R3BSofTrimParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofTrimParAsciiFileIo* p=new R3BSofTrimParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofTrimContFact)
