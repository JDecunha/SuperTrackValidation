#pragma once

#include "G4UserRunAction.hh"

class G4Run;
class TFile;
class TTree;
class EventAction;
class SteppingAction;
class PrimaryGeneratorAction;

class RunAction : public G4UserRunAction
{
public:
  
  RunAction();
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
};
