// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofTwimContFact                     -----
// -----             Created 07/10/19 by J.L. Rodriguez-Sanchez     -----
// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofTwim           -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofTwimContFact.h"
#include "R3BSofTwimCalPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofTwimContFact gR3BSofTwimContFact;

R3BSofTwimContFact::R3BSofTwimContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTwimContFact";
    fTitle = "Factory for parameter containers in libR3BSofTwim";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTwimContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the Twim library.

    FairContainer* p1 = new FairContainer("twimCalPar", "TWIM Cal Parameters", "TWIMCalParContext");
    p1->addContext("TWIMCalParContext");

    containers->Add(p1);
}

FairParSet* R3BSofTwimContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofTwimContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "twimCalPar") == 0)
    {
        p = new R3BSofTwimCalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

void R3BSofTwimContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Twim
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofTwimParRootFileIo* p=new R3BSofTwimParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofTwimParAsciiFileIo* p=new R3BSofTwimParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofTwimContFact)
