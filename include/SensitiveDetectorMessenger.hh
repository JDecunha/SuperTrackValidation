#pragma once

// SIM //
class SensitiveDetector;

// GEANT4 //
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;


class SensitiveDetectorMessenger : public G4UImessenger
{
  public:
    SensitiveDetectorMessenger(SensitiveDetector* sensitive_detector);
    virtual ~SensitiveDetectorMessenger();

  public:
    void SetNewValue(G4UIcommand* command, G4String value);

  private:
    SensitiveDetector* sensitive_detector_;

    G4UIdirectory* scorer_dir;

    // Commands under /sim/scorer/
    G4UIcmdWithABool* record_hits_command; 
    G4UIcmdWithAString* save_command; 
};

