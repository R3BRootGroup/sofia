// -------------------------------------------------------------------
// -----          R3BSofAnaContFact source file                  -----
// -----     Created 27/01/20  by J.L. Rodriguez-Sanchez         -----
// -------------------------------------------------------------------
//
//  R3BSofAnaContFact
//
//  Factory for the parameter containers in libR3BSofAna
//

#include "R3BSofAnaContFact.h"

static R3BSofAnaContFact gR3BSofAnaContFact;

R3BSofAnaContFact::R3BSofAnaContFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BSofAnaContFact";
    fTitle = "Factory for parameter containers in libR3BSofAna";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BSofAnaContFact::setAllContainers()
{
    /** Creates the Container objects with all accepted contexts and adds them to
     *  the list of containers for the STS library.*/

    FairContainer* p1 = new FairContainer("soffrsAnaPar", "FRS-S2-Cave-C Parameters", "SofFRSParContext");
    p1->addContext("SofFRSParContext");

    containers->Add(p1);

    FairContainer* p2 =
        new FairContainer("soffragmentAnaPar", "Fragment in Cave-C Parameters", "SofFragmentParContext");
    p2->addContext("SofFragmentParContext");

    containers->Add(p2);
}

FairParSet* R3BSofAnaContFact::createContainer(FairContainer* c)
{
    /** Trals the constructor of the corresponding parameter container.
     * For an actual context, which is not an empty string and not the default context
     * of this container, the name is concatinated with the context. */

    const char* name = c->GetName();
    LOG(INFO) << "R3BSofAnaContFact: Create container name: " << name;
    FairParSet* p = 0;
    if (strcmp(name, "soffrsAnaPar") == 0)
    {
        p = new R3BSofFrsAnaPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    if (strcmp(name, "soffragmentAnaPar") == 0)
    {
        p = new R3BSofFragmentAnaPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return p;
}

void R3BSofAnaContFact::activateParIo(FairParIo* io)
{
    // activates the input/output class for the parameters
    // needed by the Sts
}

ClassImp(R3BSofAnaContFact)
