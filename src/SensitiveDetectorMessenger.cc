
// SIM //
#include "SensitiveDetector.hh"
#include "SensitiveDetectorMessenger.hh"

// GEANT4 //
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

#include "G4RunManager.hh"


SensitiveDetectorMessenger::SensitiveDetectorMessenger(
        SensitiveDetector* sensitive_detector)
                     : sensitive_detector_(sensitive_detector)
{
    std::string directory = "/sim/scorer/" + sensitive_detector->GetName() + "/";

    scorer_dir = new G4UIdirectory(directory.c_str());
    scorer_dir->SetGuidance("Commands for saving output.");

    // Turn the scorer on or off.
    record_hits_command = new G4UIcmdWithABool(
            (directory + "recordHits").c_str(), this);
    record_hits_command->SetGuidance(
            "Record all hits (WARNING: uses a lot of memory and disk space!).");
    record_hits_command->SetParameterName("flag", false);
    record_hits_command->AvailableForStates(G4State_Idle);

    // Load the cells from a file.
    save_command = new G4UIcmdWithAString((directory + "save").c_str(), this);
    save_command->SetGuidance("Save the scorer output.");
    save_command->SetParameterName("filename", false);
    save_command->AvailableForStates(G4State_Idle);
}


SensitiveDetectorMessenger::~SensitiveDetectorMessenger()
{
    delete record_hits_command;
    delete save_command;
}


void SensitiveDetectorMessenger::SetNewValue(G4UIcommand* command, G4String value)
{
    if (command == record_hits_command)
    {
        sensitive_detector_->RecordHits(
                record_hits_command->GetNewBoolValue(value.c_str()));
    }

    else if (command == save_command)
    {
        sensitive_detector_->Save(value);
    }
}

