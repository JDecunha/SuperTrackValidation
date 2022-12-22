// STD //
#include <iostream>
#include <iomanip>

// SIM //
#include "SensitiveDetector.hh"
#include "SensitiveDetectorMessenger.hh"

// GEANT4 //
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"


SensitiveDetector::SensitiveDetector( const G4String& name
                                    , G4int& job_number
                                    , G4int& nReplicas)
                 : G4VSensitiveDetector(name)
{
    job_number_ = job_number;
    replicas = nReplicas;

    messenger = new SensitiveDetectorMessenger(this);

    // Initialise all of the accumulators.
    for (int i=0; i<replicas; ++i)
    {
        //initialize output vectors by filling them with vectors
        energy.push_back(std::vector<double>());

        //allocate memory for the temporary hit maps too
        event_energy.push_back(0.0);
    
        hits_deposit[i][11] = std::vector<double>();
        hits_kinetic[i][11] = std::vector<double>();
    }

    record_hits_ = false;
}


SensitiveDetector::~SensitiveDetector()
{
}


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
	//I've implemented the unordered sets because they are WAY 
	//more efficient that iterating over every cell each event

	//At the end of every event go through the volumes which were hit and record
	for (auto index_hit : cell_hit_this_event) //this is a clever C++11 method to iterate over a range python style
	{
		energy[index_hit].push_back(event_energy[index_hit]);
		event_energy[index_hit] = 0.0;
	}

	//Clear the unordered sets for the next event
	cell_hit_this_event.clear();
}


void SensitiveDetector::RecordHits(bool record_hits)
{
    record_hits_ = record_hits;
}


void SensitiveDetector::Save(std::string filename)
{
    //filename += "_" + std::to_string(job_number_);

    SaveEnergy(filename);

    if (record_hits_)
    {
        SaveHits(filename);
    }
}


void SensitiveDetector::SaveEnergy(std::string filename)
{
    std::fstream file;
    file.open(filename +std::to_string(job_number_) + "_sphere_energy.dat", std::fstream::out);

    //You can set this if you don't want scientific notation in your output
    //file << std::fixed << << std::showpoint;

    for (unsigned int i =0; i < energy.size(); i++)
    {
        file << i; //write the cell number
        for (unsigned int j = 0; j < energy[i].size(); j++)
        {
            file << "," << energy[i][j]; //write each energy
        }
        file << std::endl;

    }

    file.close();
}

void SensitiveDetector::SaveHits(std::string filename)
{
    SaveHitsDeposit(filename);
    SaveHitsKinetic(filename);
}


void SensitiveDetector::SaveHitsDeposit(std::string filename)
{
    std::fstream file;
    file.open(filename + "_hits_deposit.json", std::fstream::out);

    WriteDictionary(file, hits_deposit);
    file.close();
}


void SensitiveDetector::SaveHitsKinetic(std::string filename)
{
    std::fstream file;
    file.open(filename + "_hits_kinetic.json", std::fstream::out);

    WriteDictionary(file, hits_kinetic);
    file.close();
}


template <typename T>
void SensitiveDetector::WriteDictionary( std::fstream &file
                                       , std::map<int, T> data)
{
    file << "{" << std::endl;

    bool first = true;
    for (auto d : data)
    {
        if (first) file << "  ";
        else file << ", ";
        first = false;

        file << "\"" << d.first << "\" : " << d.second << std::endl;
    }

    file << "}";
}


void SensitiveDetector::WriteDictionary( std::fstream &file
                                       , std::map<int, std::map<int, int> > data)
{
    file << "{" << std::endl;

    bool first = true;
    for (auto d : data)
    {
        if (first) file << "  ";
        else file << ", ";
        first = false;

        file << "\"" << d.first << "\" : ";

        WriteDictionary<int>(file, d.second);
    }

    file << "}";

}


void SensitiveDetector::WriteDictionary( std::fstream &file
                                       , std::map<int, std::vector<double> > data)
{
    file << "{" << std::endl;

    bool first = true;
    for (auto d : data)
    {
        if (first) file << "  ";
        else file << ", ";
        first = false;

        file << "\"" << d.first << "\" : ";

        WriteList(file, d.second);
    }

    file << "}";

}


void SensitiveDetector::WriteDictionary( std::fstream &file
                                       , std::map<int, std::map<int, std::vector<double> > > data)
{
    file << "{" << std::endl;

    bool first = true;
    for (auto d : data)
    {
        if (first) file << "  ";
        else file << ", ";
        first = false;

        file << "\"" << d.first << "\" : ";

        WriteDictionary(file, d.second);
    }

    file << "}";

}


void SensitiveDetector::WriteList( std::fstream &file
                                 , std::vector<double> data)
{
    file << "[ ";

    bool first = true;
    for (auto d : data)
    {
        if (first) file << "  ";
        else file << ", ";
        first = false;

        file << d << std::endl;
    }

    file << "]" << std::endl;
}
