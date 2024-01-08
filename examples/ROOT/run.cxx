{
    gSystem->Load( "../../lib/libstepper.so" );
    gSystem->AddIncludePath( "-I../../src" );
    gROOT->LoadMacro( "step.C+" )
}
