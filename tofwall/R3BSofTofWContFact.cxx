// ----------------------------------------------------------------------
//								    -----
//  Factory for the parameter containers in libR3BSofTofW           -----
//         Created 03/11/19  by J.L. Rodriguez-Sanchez              -----
//								    -----
// ----------------------------------------------------------------------

#include "R3BSofTofWContFact.h"
//#include "R3BSofTofWCalPar.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"
#include "TClass.h"

static R3BSofTofWContFact gR3BSofTofWContFact;

R3BSofTofWContFact::R3BSofTofWContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofTofWContFact";
    fTitle = "Factory for parameter containers in libR3BSofTofW";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofTofWContFact::setAllContainers()
{
    // Creates the Container objects with all accepted contexts and adds them to
    // the list of containers for the TofW library.

    FairContainer* p1 = new FairContainer("tofwCalPar", "TofW Cal Parameters", "TofWCalParContext");
    p1->addContext("TofWCalParContext");

    containers->Add(p1);

    FairContainer* p2 = new FairContainer("tofwGeoPar", "TofW geometry parameters", "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");

    containers->Add(p2);
}

FairParSet* R3BSofTofWContFact::createContainer(FairContainer* c)
{
    // Trals the constructor of the corresponding parameter container.
    // For an actual context, which is not an empty string and not the default context
    // of this container, the name is concatinated with the context.

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofTofWContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "tofwCalPar") == 0)
    {
        // p = new R3BSofTofWCalPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());//FIXME
    }

    if (strcmp(name, "tofwGeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofTofWContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the tofw
    /*
    if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
      R3BSofTofWParRootFileIo* p=new R3BSofTofWParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
      io->setDetParIo(p);
    }
    if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
      R3BSofTofWParAsciiFileIo* p=new R3BSofTofWParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
      }
    */
}

ClassImp(R3BSofTofWContFact)
