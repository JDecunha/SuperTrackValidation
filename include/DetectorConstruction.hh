#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "SphereParameterisation.hh"

class DetectorConstructionMessenger;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	void SetSideLength(G4double sidelength);
	void SetSphereDiameter(G4double sphereDiam);
	G4double GetSideLength();

                         
private:
	G4VPhysicalVolume* ConstructDetector();
	void ConstructSDandField();    

    DetectorConstructionMessenger* pMessenger;
    SphereParameterisation* tiledSpheres;
	
	G4double sideLength;
	G4bool sideLengthInitialized;
	G4double sphereDiameter;
	G4bool sphereDiameterInitialized;

	G4LogicalVolume* tiles_logical;
};

