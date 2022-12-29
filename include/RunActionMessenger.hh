#pragma once

#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class RunActionMessenger : public G4UImessenger
{
	public:
  		RunActionMessenger(RunAction* run_action);
  		virtual ~RunActionMessenger();

  		void SetNewValue(G4UIcommand* command, G4String value);
                         
	private:
		RunAction* pRunAction;

		G4UIdirectory* run_directory;

		G4UIcmdWithAnInteger* numBins;
		G4UIcmdWithADouble* minBin;
		G4UIcmdWithADouble* maxBin;
		G4UIcmdWithAString* outputFilename;
};
