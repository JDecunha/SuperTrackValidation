#pragma once

// SIM //
#include "Types.hh"

// GEANT4 //
#include "G4ThreeVector.hh"
#include "G4VPVParameterisation.hh"

class G4Orb;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Colour;


class SphereParameterisation : public G4VPVParameterisation
{
  public:
    SphereParameterisation(G4double boxHalfSideLength, G4double sphereDiameter);
    ~SphereParameterisation();

  public:
    using G4VPVParameterisation::ComputeTransformation;
    void ComputeTransformation( const int copy_number
                              , G4VPhysicalVolume*) const;
 
    using G4VPVParameterisation::ComputeDimensions;
    void ComputeDimensions( G4Orb& orb
                          , const int copy_number
                          , const G4VPhysicalVolume*) const;

    G4LogicalVolume* Placement( G4LogicalVolume* mother_logical
                              , G4Material* sphere_material
                              , G4Material* surrounds_material
                              , G4Colour colour);
  public:
    G4ThreeVector _extents;
    G4double _sphereDiameter;
    G4int _numInRow;
    G4int _totalSpheres;
};

