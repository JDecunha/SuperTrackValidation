//This project
#include "SensitiveDetector.hh"
#include "DetectorConstruction.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector( const G4String& name
                                    , G4int nReplicas)
                 : G4VSensitiveDetector(name)
{
    replicas = nReplicas;

    // Initialise all of the accumulators.
    for (int i=0; i<replicas; ++i)
    {
        //initialize output vectors by filling them with vectors
        energy.push_back(std::vector<double>());

        //allocate memory for the temporary hit maps too
        event_energy.push_back(0.0);
    }
    
    //Sphere diam is converted from internal units into um.
    double sphereDiam = ((static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()))->GetSphereDiameter())/um;
    linealDenominator = 2./(3.*sphereDiam);
}


SensitiveDetector::~SensitiveDetector() {}


G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    double edep = step->GetTotalEnergyDeposit();

    if (edep != 0) 
    {
        int index = step->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(0);

        // Accumulate energy deposit.
        event_energy[index] += edep;
        cell_hit_this_event.insert(index);
    }

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
	auto analysisManager = G4AnalysisManager::Instance();

    //I've implemented the unordered sets because they are WAY 
	//more efficient that iterating over every cell each event
	//At the end of every event go through the volumes which were hit and record
	for (auto index_hit : cell_hit_this_event)
	{
        analysisManager->FillH1(0, (event_energy[index_hit]*linealDenominator));
		event_energy[index_hit] = 0.0;
	}

	//Clear the unordered sets for the next event
	cell_hit_this_event.clear();
}