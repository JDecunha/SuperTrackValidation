//This program
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
//Extern
#include "CommandLineParser.hh"
//Geant4
#include "RunActionMessenger.hh"
#include "Randomize.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

//These are static because they are shared by all threads
//Static variables are defined at class level
//TODO: See if it's possible for me to put these back into the class later
static std::string outputName;
static double minBin;
static double maxBin;
static int numBins;

void RunAction::SetNumBins(int nB) {numBins = nB;};
void RunAction::SetMinBin(double minB) {minBin = minB;};
void RunAction::SetMaxBin(double maxB) {maxBin = maxB;};
void RunAction::SetOutputFilename(std::string oF) {outputName = oF;};

RunAction::RunAction() : G4UserRunAction() 
{ 
  pMessenger = new RunActionMessenger(this);
}

RunAction::~RunAction() { }

void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);

  //Pull the random seed for the filename from the main thread
  if(isMaster)
  {
    long long rndmSeed = G4Random::getTheSeed();
    outputName = outputName + "_" + std::to_string(rndmSeed) + ".root";
  }

  //Create the output histogram
  analysisManager->CreateH1("f(y)","Lineal energy spectrum", numBins, minBin*(keV/um), maxBin*(keV/um));

  analysisManager->SetNtupleMerging(true); //This also sets histogram merging
  analysisManager->OpenFile(outputName);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  //According to documentation: https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Analysis/managers.html#parallel-processing
  //Analysis Manager->Write() on individual threads will merge the histograms. Very useful.
  analysisManager->Write();
  analysisManager->CloseFile();
}
