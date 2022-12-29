#pragma once

// SIM //
class SensitiveDetectorMessenger;

// GEANT4 //
#include "G4VSensitiveDetector.hh"

// User //
#include "Types.hh"

// STL //
#include <map>
#include <vector>
#include <unordered_set> //Unordered set is a more efficient way to store unique events than an ordered one


class SensitiveDetector : public G4VSensitiveDetector
{
  public:

    SensitiveDetector(const G4String& name, G4int nReplicas);
    virtual ~SensitiveDetector();

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void EndOfEvent(G4HCofThisEvent* HCE);

  private:

    //A vector of vectors, which hold the energy accumulated each event
    //This vector is what is output at the end of the simulation
    // energy[i][j], gives the energy in the ith cell for the jth event
    std::vector<std::vector<double> > energy;

    //temporary scorer that accumulates energy per event, then zeroes
    std::vector<double> event_energy;

    //A set of numbers referring to the indices of cells hit this event
    std::unordered_set<int> cell_hit_this_event;

    double linealDenominator;
    int replicas; 
};

