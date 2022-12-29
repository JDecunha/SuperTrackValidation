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

	void SetSideLength(G4double sidelength) { sideLength = sidelength; sideLengthInitialized = true; };
	void SetSphereDiameter(G4double sphereDiam) { sphereDiameter = sphereDiam; sphereDiameterInitialized = true; };

	G4double GetSideLength() const {return sideLength;};
	G4double GetSphereDiameter() const {return sphereDiameter;};
                    
private:

	G4VPhysicalVolume* ConstructDetector();
	void ConstructSDandField();    
	
	G4double sideLength;
	G4double sphereDiameter;
	G4bool sphereDiameterInitialized;
	G4bool sideLengthInitialized;

	SphereParameterisation* tiledSpheres;
	G4LogicalVolume* tiledSpheresLogical;

	DetectorConstructionMessenger* pMessenger;
    
};

