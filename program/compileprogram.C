#include "TString.h"
#include "TSystem.h"

void compileprogram(TString myopt="fast")
{
    TString opt;
    if (myopt.Contains("force")) {
        opt = "kfg";
    } else {
        opt = "kg";
    }
    gSystem->CompileMacro("Grid.cpp", opt.Data());
    gSystem->CompileMacro("Grid_movement.cpp", opt.Data());
    gSystem->CompileMacro("Ethnicity.cpp", opt.Data());
    gSystem->CompileMacro("Neighborhood.cpp", opt.Data());
    gSystem->CompileMacro("Box.cpp", opt.Data());
    gSystem->CompileMacro("Person.cpp", opt.Data());
    gSystem->CompileMacro("mainFile.C", opt.Data());
}
