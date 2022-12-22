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
#include <unordered_set> //Unordered set is a /really/ efficient way to store unique events


class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(const G4String& name
                                    , G4int& job_number
                                    , G4int& nReplicas);
    virtual ~SensitiveDetector();

  public:
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void EndOfEvent(G4HCofThisEvent* HCE);

    void RecordHits(bool record_hits);
    void Save(std::string filename);

  private:
    void SaveEnergy(std::string filename);
    void SaveHits(std::string filename);
    void SaveHitsDeposit(std::string filename);
    void SaveHitsKinetic(std::string filename);

    template <typename T>
    void WriteDictionary( std::fstream &file
                        , std::map<int, T> data); 

    void WriteDictionary( std::fstream &file
                        , std::map<int, std::map<int, int> > data); 

    void WriteDictionary( std::fstream &file
                        , std::map<int, std::vector<double> > data); 

    void WriteDictionary( std::fstream &file
                        , std::map<int, std::map<int, std::vector<double> > > data); 

    void WriteList( std::fstream &file
                  , std::vector<double> data);

    G4ThreeVector GetCellPosition (int index) { return G4ThreeVector(scoring_volume_information[index].body.x, scoring_volume_information[index].body.y, scoring_volume_information[index].body.z); }
    G4double GetNucleiRadius (int index) { return scoring_volume_information[index].nucleus.radius;}

    Cells scoring_volume_information;

  private:
    bool record_hits_;
    int replicas;
    
    int job_number_;
    SensitiveDetectorMessenger* messenger;

  public:

    //A vector of vectors, which hold the energy accumulated each event
    //This vector is what is output at the end of the simulation
    // energy[i][j], gives the energy in the ith cell for the jth event
    std::vector<std::vector<double> > energy;
    std::vector<std::vector<double> > nucleus_energy;

    //temporary scorer that accumulates energy per event, then zeroes
    std::vector<double> event_energy;
    std::vector<double> event_nucleus_energy;

    //A set of numbers referring to the indices of cells hit this event
    std::unordered_set<int> cell_hit_this_event;
    std::unordered_set<int> nucleus_hit_this_event;

    // replica : pdg : [energy]
    std::map<int, std::map<int, std::vector<double> > > hits_deposit;
    std::map<int, std::map<int, std::vector<double> > > hits_kinetic;
};

