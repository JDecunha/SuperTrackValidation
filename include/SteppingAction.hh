#pragma once

#include "G4UserSteppingAction.hh"
#include "G4Types.hh"
#include <vector>

class RunAction;
class EventAction;
class G4StepPoint;
class TTree;

class SteppingAction : public G4UserSteppingAction
{
  public:
	SteppingAction();
	~SteppingAction();

	void UserSteppingAction(const G4Step*);

  private:
	friend class RunAction;
	friend class EventAction;

  std::vector<std::pair<long long, double>> sphereID_edepPair; //Stores the sphere ID and edep in that sphere for a single step
	int edepPositionPointer {0}; //stores which position we are currently at in the sphereID_edepPair. This exists so we never have actually clear all the values in the vector to 0, we can just set this value to 0.

	//We also need geometry values
	double scoringRegionHalfLength;
	double greatestSphereOffset;
	double sphereDiameter;
	double sphereRadiusMag;
	double linealDenominator;
	long long numSpheresLinear;

};

