//MicroTrackGenerator
#include "RunAction.hh"
#include "RunActionMessenger.hh"
//Geant4
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

RunActionMessenger::RunActionMessenger(RunAction* run_action) : pRunAction(run_action)
{
    // Directories in which commands are organised.
    run_directory = new G4UIdirectory("/runAction/");
    run_directory->SetGuidance("Commands relating to run Action.");

    numBins = new G4UIcmdWithAnInteger("/runAction/numBins", this);
    numBins->SetGuidance("Number of histogram bins.");
    numBins->SetParameterName("numBins", false);
    numBins->AvailableForStates(G4State_PreInit);

    minBin = new G4UIcmdWithADouble("/runAction/minBin", this);
    minBin->SetGuidance("Smallest bin size.");
    minBin->SetParameterName("minBin", false);
    minBin->AvailableForStates(G4State_PreInit);

    maxBin = new G4UIcmdWithADouble("/runAction/maxBin", this);
    maxBin->SetGuidance("Maximal bin size.");
    maxBin->SetParameterName("maxBin", false);
    maxBin->AvailableForStates(G4State_PreInit);

    outputFilename = new G4UIcmdWithAString("/runAction/outputName", this);
    outputFilename->SetGuidance("Output name.");
    outputFilename->SetParameterName("outputName", false);
    outputFilename->AvailableForStates(G4State_PreInit);
}


RunActionMessenger::~RunActionMessenger()
{
    delete run_directory;
    delete numBins;
    delete minBin;
    delete maxBin;
    delete outputFilename;
}

void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == numBins)
    {
      pRunAction->SetNumBins(numBins->GetNewIntValue(value.c_str()));
    }
    if (command == minBin)
    {
      pRunAction->SetMinBin(minBin->GetNewDoubleValue(value.c_str()));
    }
    if (command == maxBin)
    {
      pRunAction->SetMaxBin(maxBin->GetNewDoubleValue(value.c_str()));
    }
    if (command == outputFilename)
    {
      pRunAction->SetOutputFilename(value);
    }
}

