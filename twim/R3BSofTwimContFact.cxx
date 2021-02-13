// --------------------------------------------------------------------
// -----          R3BSofTwimContFact source file                  -----
// -----     Created 24/11/19  by J.L. Rodriguez-Sanchez          -----
// --------------------------------------------------------------------
//
//  R3BSofTwimContFact
//
//  Factory for the parameter containers in libR3BSofTwim
//

#include "R3BSofTwimContFact.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "R3BSofTwimCalPar.h"
#include "R3BSofTwimHitPar.h"

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
    // the list of containers for the STS library.

    FairContainer* p1 = new FairContainer("twimCalPar", "TWIM Cal Parameters", "TwimCalParContext");
    p1->addContext("TwimCalParContext");

    containers->Add(p1);

    FairContainer* p2 = new FairContainer("twimHitPar", "TWIM Hit Parameters", "TwimHitParContext");
    p2->addContext("TwimHitParContext");

    containers->Add(p2);

    FairContainer* p3 = new FairContainer("TwimGeoPar", "Twim-Music geometry parameters", "GeometryParameterContext");
    p3->addContext("GeometryParameterContext");

    containers->Add(p3);
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
    if (strcmp(name, "twimHitPar") == 0)
    {
        p = new R3BSofTwimHitPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    if (strcmp(name, "TwimGeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

void R3BSofTwimContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Twim
}

ClassImp(R3BSofTwimContFact)
