// ---------------------------------------------------------------
//  Factory for the parameter containers in libR3BSofTrim  -------
// ---------------------------------------------------------------

#include "R3BSofTrimContFact.h"
#include "R3BSofTrimCalPar.h"
#include "R3BSofTrimHitPar.h"
#include "R3BTGeoPar.h"

#include "FairLogger.h"
#include "FairRuntimeDb.h"
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

    FairContainer* p2 = new FairContainer("trimHitPar", "Triple MUSIC Hit Parameters", "TrimHitParContext");
    p2->addContext("TrimHitParContext");
    containers->Add(p2);

    FairContainer* p3 = new FairContainer("TrimGeoPar", "Triple MUSIC geometry parameters", "GeometryParameterContext");
    p3->addContext("GeometryParameterContext");
    containers->Add(p3);
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
    else if (strcmp(name, "trimHitPar") == 0)
    {
        p = new R3BSofTrimHitPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    else if (strcmp(name, "TrimGeoPar") == 0)
    {
        p = new R3BTGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

ClassImp(R3BSofTrimContFact);
