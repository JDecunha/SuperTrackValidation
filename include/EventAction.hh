#pragma once

#include "G4UserEventAction.hh"
#include "G4Types.hh"
//std
#include <vector>

class RunAction;
class SteppingAction;

class EventAction : public G4UserEventAction
{
  public:
	EventAction();
	~EventAction();

	void EndOfEventAction(const G4Event*);

  private:
	friend class RunAction;
	friend class SteppingAction;

	std::vector<double> edeps;
};

		

