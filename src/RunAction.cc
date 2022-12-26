//This program
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
//Extern
#include "CommandLineParser.hh"
//Geant4
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : G4UserRunAction() 
{ 
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  //Create the output histogram
  analysisManager->CreateH1("f(y)","Lineal energy spectrum", 256, 0., 1000*keV/um);
}

RunAction::~RunAction() { }

void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4String outputFileName = "testingOutput.root";
  analysisManager->OpenFile(outputFileName);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  //According to documentation: https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Analysis/managers.html#parallel-processing
  //Analysis Manager->Write() on individual threads will merge the histograms. Very useful.
  analysisManager->Write();
  analysisManager->CloseFile();
}
