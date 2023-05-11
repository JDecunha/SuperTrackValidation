//This project
#include "EventAction.hh"
#include "SteppingAction.hh"
//Geant4
#include "G4Types.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
//std
#include <algorithm>

EventAction::EventAction() : G4UserEventAction() 
{ 
  edeps.reserve(1e6);
}

EventAction::~EventAction() { }

void EventAction::EndOfEventAction(const G4Event*)
{
  //Grab a referece to our sphere IDs and energy depositions
  std::vector<std::pair<long long, double>>& sphereID_edepPair = ((SteppingAction*)G4RunManager::GetRunManager()->GetUserSteppingAction())->sphereID_edepPair;

  //Sort the list so that it's ordered by sphere ID
  std::sort(sphereID_edepPair.begin(),sphereID_edepPair.end());

  //Values we need in the loop
  double currentEdepAccumulator {0};
  long long currentSphereID = std::get<0>(sphereID_edepPair[0]); //get the first ID

  //The edeps are sorted, accumulate them
  for (auto const& pair:sphereID_edepPair)
  {
    long long sphereID = std::get<0>(pair);
    double edepVal = std::get<1>(pair);

    if (sphereID != currentSphereID) //alright, we've moved on to another sphere, push the value back
    {
      edeps.push_back(currentEdepAccumulator);
      currentEdepAccumulator = 0;
      currentSphereID = sphereID;
    }

    currentEdepAccumulator += edepVal;
  }

  //Clear out the stepping action values
  long long lengthOfEdepVector = sphereID_edepPair.size();
  sphereID_edepPair.clear();
  sphereID_edepPair.reserve(lengthOfEdepVector); //this is fine, if it's smaller than the previously reserved value no action is taken

  //Push the accumulated edeps to the histogram
  auto analysisManager = G4AnalysisManager::Instance();
  for (auto const& edep:edeps)
  {
    analysisManager->FillH1(0, edep); //this is actually edep / linealdenominator. You can see that from the stepping action
  }

  //Clear out the accumulated edep values
  long long lengthofAccumulatedEdeps = edeps.size();
  edeps.clear();
  edeps.reserve(lengthofAccumulatedEdeps);
}
