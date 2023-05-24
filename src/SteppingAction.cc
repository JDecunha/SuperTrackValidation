//This Application
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
//Geant4
#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
//STD
#include <cmath>

SteppingAction::SteppingAction() : G4UserSteppingAction()
{
  double scoringRegionLength = (((DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetSideLength())/nanometer;
  scoringRegionHalfLength = scoringRegionLength/(2.);

  sphereDiameter = (((DetectorConstruction*)G4RunManager::GetRunManager()->GetUserDetectorConstruction())->GetSphereDiameter())/nanometer;
  double sphereRadius = sphereDiameter/2;
  sphereRadiusMag = sphereRadius*sphereRadius;
  linealDenominator = (2./3.)*sphereDiameter; 

  //The number of spheres in a line set with TMath::Floor, so that the box can fit all targets without cutting any off
  numSpheresLinear = std::floor(((scoringRegionHalfLength*2)/sphereDiameter)); 
  
  //We scale the size of the box to match the number of spheres that fit within it
  scoringRegionLength = numSpheresLinear*sphereDiameter;
  scoringRegionHalfLength = scoringRegionLength/2;

  //gSO is always just one radius off the edge
  greatestSphereOffset = -scoringRegionHalfLength+sphereRadius;

  //Check that radius is not larger than half length
  if(sphereRadius > scoringRegionHalfLength)
  {
    scoringRegionHalfLength = sphereRadius;
    numSpheresLinear = 1;
    scoringRegionLength = scoringRegionHalfLength*2;
    greatestSphereOffset = -scoringRegionHalfLength+sphereRadius;
  }

  sphereID_edepPair.reserve(1e6);

  // std::cout << "Scoring region half length: " << scoringRegionHalfLength << std::endl;
  // std::cout << "Sphere radius: " << sphereRadius << std::endl;
  // std::cout << "greatestSphereOffset: " << greatestSphereOffset << std::endl;

}

SteppingAction::~SteppingAction(){ }

void SteppingAction::UserSteppingAction(const G4Step* step) //Save the position and edep for every step
{ 

  double edep = step->GetTotalEnergyDeposit()/eV;

  if(edep != 0)
  {
    //Get the PreStepPoint and coordinates 
    G4StepPoint* preStep = step->GetPreStepPoint();
    double x = preStep->GetPosition().x()/nanometer;
    double y = preStep->GetPosition().y()/nanometer;
    double z = preStep->GetPosition().z()/nanometer;

    double distFromFurthestSphereX = x-greatestSphereOffset;
    double distFromFurthestSphereY = y-greatestSphereOffset;
    double distFromFurthestSphereZ = z-greatestSphereOffset;

    double distFromNearestSphereX = ((llrint(distFromFurthestSphereX/sphereDiameter))*sphereDiameter)-distFromFurthestSphereX;
    double distFromNearestSphereY = ((llrint(distFromFurthestSphereY/sphereDiameter))*sphereDiameter)-distFromFurthestSphereY;
    double distFromNearestSphereZ = ((llrint(distFromFurthestSphereZ/sphereDiameter))*sphereDiameter)-distFromFurthestSphereZ; 

    //Determine if inside the nearest sphere
    double dist = (distFromNearestSphereX*distFromNearestSphereX)+(distFromNearestSphereY*distFromNearestSphereY)+(distFromNearestSphereZ*distFromNearestSphereZ);
   
    //Check if we're in a sphere
    if (dist <= sphereRadiusMag)
    {
      //If in the sphere:
      //Take the position relative to the greatest sphere offset. Divide by the number of sphere diameters away,
      //and round to nearest int to find the index in each axis
      long long xIndex = llrint((x-greatestSphereOffset)/sphereDiameter);
      long long yIndex = llrint((y-greatestSphereOffset)/sphereDiameter);
      long long zIndex = llrint((z-greatestSphereOffset)/sphereDiameter);

      //Determine the Index of the sphere hit
      long long sphereHitIndex = xIndex + yIndex*numSpheresLinear+ zIndex*numSpheresLinear*numSpheresLinear; //Keep in mind that for the index it starts counting at zero

      //Add to the output vector
      sphereID_edepPair.push_back(std::pair<long long, double>(sphereHitIndex, edep/linealDenominator));
    }
  }

}    
