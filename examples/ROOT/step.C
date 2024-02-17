#include <TCanvas.h>
#include <TGraph.h>

#include "StepPerTime.h"

using namespace steppo;

void Step()
{
    TGraph* graphSpeed = new TGraph();
        graphSpeed->SetTitle( "Speed;tick;speed" );
        graphSpeed->SetMarkerStyle( 6 );
    TGraph* graphPosition = new TGraph();
        graphPosition->SetTitle( "Position;tick;pos" );
        graphPosition->SetMarkerStyle( 6 );

    Stepper s( 10000, 3 );
    s.start( 2000 );

    for( uint32_t tick = 0; s.onInterrupt(); ++tick )
    {
        graphSpeed->AddPoint( tick, s.speedCurrent() );
        graphPosition->AddPoint( tick, s.stepsCurrent() );
    }

    TCanvas* c = new TCanvas( "c", "c", 800, 600 );
    c->Divide( 1, 2 );

    c->cd( 1 );
    graphPosition->Draw( "APL" );
    gPad->SetGrid( 1, 1 );

    c->cd( 2 );
    graphSpeed->Draw( "APL" );
    gPad->SetGrid( 1, 1 );
}
