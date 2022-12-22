//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
//Extern
#include "CommandLineParser.hh"
//ROOT
#include"TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
//Geant4
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"

RunAction::RunAction() : G4UserRunAction() { }
RunAction::~RunAction() { }

//
// Beginning of run action
//

void RunAction::BeginOfRunAction(const G4Run* run)
{
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  if(isMaster && sequential == false ) //note that in sequential mode, BeginMaster will never be called. So put MT only things there
  {
    BeginMaster(run);
  }
  else BeginWorker(run);
}

void RunAction::BeginMaster(const G4Run*)
{
  ROOT::EnableThreadSafety(); //make ROOT thread safe from the main thread. ROOT will crash without this.
}

void RunAction::BeginWorker(const G4Run*)
{

}

//
// End of run action
//

void RunAction::EndOfRunAction(const G4Run* run)
{
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  if(isMaster && sequential == false)
  {
    EndMaster(run);
  }
  else
  {
    EndWorker(run);
  }
}

void RunAction::EndMaster(const G4Run*) { }

void RunAction::EndWorker(const G4Run*)
{

}

